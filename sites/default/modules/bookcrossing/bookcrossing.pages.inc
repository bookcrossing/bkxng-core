<?php

/**
 * @file
 * Menu callbacks for bookcrossing site.
 */

/**
 * Menu callback: book releasing page.
 */
function bookcrossing_release_book_page() {
  if (user_access('release book')) {
    return drupal_get_form('bookcrossing_release_book_form');
  }

  /**
   * @todo add page output for unauthorized users.
   */
}

/**
 * Menu callback: reserve bcid.
 */
function bookcrossing_reserve_bcid($js = 'ajax') {
  if ($js == 'ajax') {
    global $user;

    $bcids = db_select('bookcrossing_books', 'b')
        ->fields('b', array('bcid'))
        ->condition('owner', $user->uid, '=')
        ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '=')
        ->execute()
        ->fetchCol();

    $num = count($bcids);
    if ($num == BOOKCROSSING_RESRVED_BCIDS) {
      return NULL;
    }

    $commands = array();
    $generate = BOOKCROSSING_RESRVED_BCIDS - $num;

    for ($i = 0; $i < $generate; $i++) {
      $bcid = bookcrossing_generate_bcid();
      db_insert('bookcrossing_books')
          ->fields(array(
            'nid' => 0,
            'status' => BOOKCROSSING_BOOK_STATUS_RESERVED,
            'owner' => $user->uid,
            'bcid' => $bcid,
            'current_place' => 0,
          ))
          ->execute();



      $reserved_html = '<div class="bcid-container" id="' . $bcid . '">';
      $reserved_html .= l('<b>BCID:</b>' . $bcid, 'add-reserved-book/' . $bcid, array('html' => TRUE));
      $reserved_html .= '</div>';
      $commands[] = ajax_command_append('.reserved-bcids-container', $reserved_html);
    }

    //if ($num == BOOKCROSSING_RESRVED_BCIDS - 1) {
      $commands[] = ajax_command_remove('#reserve-bcid');
    //}

    $commands[] = ajax_command_remove('#reserve-bcid-description');

    ajax_deliver(array('#type' => $js, '#commands' => $commands));
  }
}

/**
 * Menu callback: delete reserved bcid.
 */
function bookcrossing_delete_reserved_bcid($bcid = '', $js = 'ajax') {
  if ($js == 'ajax') {
    global $user;

    db_delete('bookcrossing_books')
        ->condition('bcid', $bcid, '=')
        ->condition('owner', $user->uid)
        ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED)
        ->execute();

    $commands = array();

    $commands[] = ajax_command_remove('#' . $bcid);

    $reserved = db_select('bookcrossing_books', 'b')
        ->fields('b', array('bcid'))
        ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '=')
        ->condition('owner', $user->uid, '=')
        ->execute()
        ->fetchCol();

    $num = count($reserved);

    if ($num == 0) {
      $commands[] = ajax_command_after('#reserve-bcid', '<div id="reserve-bcid-description">' . t('You can reserve multiple BCID.') . '</div>');
    }

    ajax_deliver(array('#type' => $js, '#commands' => $commands));
  }
}

/**
 * Menu callback: html output for a front page.
 */
function bookcrossing_front_page() {
  $bids = db_select('bookcrossing_books', 'b')
      ->fields('b', array('bid'))
      ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->range(0, 3)
      ->orderRandom('bid')
      ->execute()
      ->fetchCol();
  $books = bookcrossing_load_multiple($bids);

  $output = '';
  foreach ($books as $book) {
    $view = node_view($book['node'], 'front_page');
    $build = bookcrossing_prepare_book_view($view, $book);
    $output .= render($build);
  }

  return $output . '<div style="clear: left"></div>';
}

/**
 * Menu callback: release book.
 */
function bookcrossing_release_book($book) {
  /**
   * Book wasnt found in db.
   */
  if (!$book) {
    return array(
      '#markup' => t('There is no book with that BCID.'),
    );
  }

  /**
   * Book is already released.
   */
  if (!$book['status']) {
    return array(
      '#markup' => t('Cannot to release book because its already was released.'),
    );
  }

  global $user;
  /**
   * Can only release books user reading.
   */
  if ($book['user']->uid != $user->uid) {
    return array(
      '#markup' => t('You cannot release this book because u are not reading it now.'),
    );
  }

  $build = node_view($book['node'], 'found_book');
  $build = bookcrossing_prepare_book_view($build, $book, FALSE);

  /**
   * Form for adding comment and place.
   */
  $build['book_found'] = drupal_get_form('bookcrossing_book_comment', $book);

  /**
   * Status of the found book.
   */
  $build['book_found_status'] = array(
    '#markup' => bookcrossing_book_status($book),
  );

  $build['author_and_year'] = array(
    '#markup' => bookcrossing_author_and_year($book),
  );
  
  return $build;
}

/**
 * Menu callback: change users comment.
 */
function bookcrossing_change_comment($bid = '', $js = 'ajax') {
  if ($js == 'ajax') {
    global $user;

    $is_read_book = db_select('bookcrossing_history', 'h')
        ->fields('h', array('bid'))
        ->condition('uid', $user->uid, '=')
        ->condition('bid', $bid, '=')
        ->execute()
        ->fetchField();

    if (!$is_read_book) {
      return NULL;
    }

    $info = array(
      'uid' => $user->uid,
      'bid' => $bid,
    );


    $form = drupal_get_form('bookcrossing_change_comment_form', $info);

    ajax_deliver(array('#type' => $js, '#commands' => array(ajax_command_html('#my-comment-text', render($form)))));
  }
}

/**
 * Menu callback: view histroy page.
 */
function bookcrossing_view_history($history) {
  $build = node_view($history['node'], 'history');
  global $user;
  $build = bookcrossing_prepare_book_view($build, $history, FALSE);

  drupal_set_title(t('Book history') . ' - ' . $history['node']->title);
  /**
   * Adding comment of the logged user to the output.
   */
  $isset = FALSE;
  $my_comment = '<div id="my-comment-text">';
  if (isset($history['comments'][$user->uid])) {
    $my_comment .= $history['comments'][$user->uid];
    $isset = TRUE;
  }

  if (!$isset) {
    $query = db_select('bookcrossing_history', 'h');
    //$query->innerJoin('bookcrossing_books', 'b');
    $exist = $query
      ->fields('h', array('bid'))
      ->condition('h.uid', $user->uid, '=')
      ->condition('h.bid', $history['bid'], '=')
      ->execute()
      ->fetchField();

    if ($exist) {
      $my_comment .= t('Review wasnt added. You can add it by pressing "change" link.');
    }
    else {
      $my_comment .= t('You cant write reviews for book u didnt find.');
    }
  }
  
  $my_comment .= '</div>';

  $is_read_book = db_select('bookcrossing_history', 'h')
      ->fields('h', array('bid'))
      ->condition('uid', $user->uid, '=')
      ->condition('bid', $history['bid'], '=')
      ->execute()
      ->fetchField();

  $change_link = '';
  if ($is_read_book) {
    $change_link = l(t('(change)'), 'change-comment/' . $history['bid'], array('attributes' => array('class' => array('use-ajax'))));
  }
  $build['book_comment'] = array(
    '#markup' => '<div class="back-to-book">' . l(t('<<back to book'), 'book/' . $history['bid']) . '</div><div class="my-comment"><div class="my-comment-title">' . t('My review') . $change_link . '</div>' . $my_comment . '</div>',
  );
  
  $build['author_and_year'] = array(
    '#markup' => bookcrossing_author_and_year($history),
  );

  /**
   * Generate output for history of the book.
   */
  $output = '<div class="history-wrapper">';
  foreach ($history['places'] as $place) {
    $comment = isset($history['comments'][$place['user']->uid]) ? $history['comments'][$place['user']->uid] : '';
    $my_comment_class = $place['user']->uid == $user->uid ? ' m-c' : '';
    $user_link = '';
    $fbid = bookcrossing_load_fbid($place['user']->uid);
    if ($fbid) {
      $vars = array();
      $filepath = (isset($place['user']->picture->uri)) ? $place['user']->picture->uri : '';
      $name = !empty($place['user']->name) ? $place['user']->name : variable_get('anonymous', t('Anonymous'));
      $alt = t("@user's picture", array('@user' => $name));
      $vars = array('path' => $filepath, 'alt' => $alt, 'title' => $alt);
      $vars['style_name'] = 'user-in-history';
      $image = theme('image_style', $vars);
      $user_link = l($image, 'http://www.facebook.com/profile.php?id=' . $fbid . '&sk=app_bookcrossing_by', array('html' => TRUE));
    }
    $output .= '<div class="place-wrapper clearfix">
                  <div class="history-comment' . $my_comment_class . '">' . $comment . '</div>
                  <div class="history-user-foto">' . $user_link . '</div>
                  <div class="history-user-info">'
        . l($place['user']->name, $fbid ? 'http://www.facebook.com/profile.php?id=' . $fbid . '&sk=app_bookcrossing_by' : 'user/' . $place['user']->uid)
        . '<div class="history-places">';

    if ($place['place']) {
      $output .= '<div class="place-found">' . t('Found') . ':<div class="place-date-found">' . format_date($place['found']) . ',' . $place['place']->name . '</div></div>';
    }

    if ($place['place_left']) {
      $output .= '<div class="place-left">' . t('Released') . ':<div class="place-date-left">' . format_date($place['time_left']) . ', ' . $place['place_left']->name . '</div></div>';
    }

    $output .= '</div></div></div>';
  }
  $output .= '</div>';

  $build['book_history'] = array(
    '#markup' => $output,
  );

  /**
   * Generate item list with status of the book.
   */
  $build['book_status'] = array(
    '#markup' => bookcrossing_book_status($history),
  );

  $build['history_pager'] = array(
    '#theme' => 'pager',
  );

  return $build;
}

/**
 * Menu callback: shows book.
 */
function bookcrossing_view_book($book = array()) {
  $build = node_view($book['node'], 'default');
  $build = bookcrossing_prepare_book_view($build, $book);
  drupal_set_title($book['node']->title);
  /**
   * Generate output for status of the book.
   */
  $build['book_status'] = array(
    '#markup' => bookcrossing_book_status($book),
  );

  /**
   * Generate links for book: release, view history, add to favourites.
   */
  $items = array();

  global $user;
  if ($user->uid == $book['user']->uid && $book['status']) {
    $items[] = l(t('Release'), 'release-book/' . $book['bid']);
  }

  $items[] = l(t('View history'), 'view-history/' . $book['bid']);

  $query = db_select('bookcrossing_favourites', 'f');
  $query->fields('f', array('uid'));
  $query->condition('f.bid', $book['bid'], '=');
  $query->condition('f.uid', $user->uid, '=');
  $already_added = $query->execute()->fetchField();

  if ($user->uid && !$already_added) {
    /**
     * @todo add facebook like action.
     */
    $items[] = l(t('Add to bookmarks'), 'add-to-bookmarks/' . $book['bid'], array('attributes' => array('onclick' => 'return like()')));
//fboauth_action_display('like-book');
//l(t('Add to favourites'), 'ajax/favourites/' . $book['bid'], array('absolute' => TRUE, 'attributes' => array('id' => 'add-to-favourites', 'class' => array('use-ajax'))));
  }

  $app_id = variable_get('fboauth_id', '');
  $init = "window.fbAsyncInit = function() {
      FB.init({
        appId      : $app_id,
        status     : true,
        cookie     : true,
        xfbml      : true
      });
    };

    // Load the SDK Asynchronously
    (function(d){
      var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
      js = d.createElement('script'); js.id = id; js.async = true;
      js.src = '//connect.facebook.net/ru_RU/all.js';
      d.getElementsByTagName('head')[0].appendChild(js);
    }(document));";
 
  //$script = '<script>' . $init . '</script>';
  drupal_add_js($init, 'inline');

  $build['book_links'] = array(
    '#markup' => theme_item_list(array('title' => '', 'items' => $items, 'type' => 'ul', 'attributes' => array('class' => 'book-links'))),
  );

  bookcrossing_meta_og($book);

  //$_SESSION['book_to_like'] = $book['bid'];
  
  bookcrossing_add_fb_js($book['bid'], url('book/' . $book['bid'], array('absolute' => TRUE)));

  $build['facepile'] = array(
    '#markup' => '<fb:facepile href="' . url($_GET['q'], array('absolute' => TRUE)) . '" action="bookcrossing_by:bookmark" width="300" size="large" max_rows="3"></fb:facepile>',
  );
//    '#markup' => '<script src="http://connect.facebook.net/ru_RU/all.js#appId=' . variable_get('fboauth_id', '') . '&amp;xfbml=1"></script><fb:facepile href="' . url($_GET['q'], array('absolute' => TRUE)) . '" action="bookcrossing_by:bookmark" width="300" size="large" max_rows="3"></fb:facepile>',

  $data_href = url('book/' . $book['bid'], array('absolute' => TRUE));

  $build['fb_recommend_bar'] = array(
    '#markup' => '<div class="fb-recommendations-bar" data-href="' . $data_href . '" data-trigger="onvisible" data-read-time="20" data-action="like" data-side="right" data-num_recommendations="3"></div>',
  );

  $build['fb_comments'] = array(
    '#markup' => '<div class="fb-comments" data-href="' . $data_href . '" data-num-posts="3" data-width="635"></div>',
  );

/*
<div class="fb-facepile" data-href="' . url($_GET['q'], array('absolute' => TRUE)) . '" data-action="bookcrossing_by:bookmark" data-size="large" data-max-rows="1" data-width="300"></div>
*/
  return $build;
}


function bookcrossing_add_fb_js($bid, $path = '', $type = 'bookmark') {
  $msg_text = t('Facebook returned the following message: ');
  $msg_type = t('');
  $msg_summary = t('Book could be added to bookmarks only once. You\'ve already added this book.');
  $msg_success = t('Post was successful! Action ID: ');
  $like = "function like(){
      FB.api(
        '/me/bookcrossing_by:bookmark?book=$path',
        'post',
	function(response) {
	  var msg = '$msg_text';
	  if (!response || response.error) {
	    //window.location = Drupal.settings.base_path + 'add-to-favourites'
           if (response.error) {
             msg += '$msg_type' + response.error.message;
             jQuery('.messages.error').remove();
             jQuery('#content-wrapper').prepend('<div class=\"messages warning\">' + msg + '</div>');
             /* jQuery('#content-wrapper').prepend('<div class=\"messages warning\">' + '$msg_summary' + '</div>'); */s
	    }
	  }
	  else {
           /*path = window.location.pathname;
           path = path.replace(Drupal.settings.basePath, '');
           path = path.split('/');*/

           window.location = Drupal.settings.basePath + 'add-to-bookmarks/' + $bid;
         }
	});
     return false;
  }";

  drupal_add_js($like, 'inline');
}

/**
 * Menu callback: page for registering found book.
 */
function bookcrossing_found_book_page($bcid = '') {
  if (empty($bcid)) {
    return '';
  }

  $book = bookcrossing_load_by_bcid($bcid);
  
  /**
   * There is no that book in db.
   */
  if (!$book) {
    return array(
      '#markup' => t('There is no book with that BCID.'),
    );
  }

  /**
   * Someone already found this book.
   */
  if ($book['status']) {
    return array(
      '#markup' => t('Someone is already reading this book.'),
    );
  }
  drupal_set_title(t('You have found the book') . ' ' . $book['node']->title);
  $build = node_view($book['node'], 'found_book');
  $build = bookcrossing_prepare_book_view($build, $book, FALSE);
  $build['book_found'] = drupal_get_form('bookcrossing_book_comment', $book);

  /**
   * Generate output for found book status.
   */
  $status_string = $book['status'] ? t('Found') : t('Free book');
  $items = array(
    '<div class="status-label">' . t('Book status') . ':</div>' . $status_string,
  );

  if ($book['status']) {
    $items[] = '<div class="status-label">' . t('Reader') . ':</div>' . l($book['user']->name, 'user/' . $book['user']->uid);
  }
  else {
    $items[] = '<div class="status-label">' . t('Released') . ':</div>' . l($book['user']->name, 'user/' . $book['user']->uid) . ', ' . format_date($book['time_left']);
  }

  $build['book_found_status'] = array(
    '#markup' => theme('item_list', array('title' => '', 'items' => $items, 'type' => 'ul', 'attributes' => array('class' => 'status-info'))),
  );
  
  $build['author_and_year'] = array(
    '#markup' => bookcrossing_author_and_year($book),
  );

  return $build;
}

/**
 * Form for addding reserved book.
 */
function bookcrossing_add_reserved_book_form($bcid = '') {
  global $user;

  // check if we have reserved bcids
  $reserved = db_select('bookcrossing_books', 'b')
      ->fields('b', array('bcid'))
      ->condition('bcid', $bcid, '=')
      ->condition('owner', $user->uid, '=')
      ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '=')
      ->execute()
      ->fetchField();

  if ($reserved) {
    $types = node_type_get_types();
    $node = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => 'bookcrossing', 'language' => LANGUAGE_NONE);
    //drupal_set_title(t('Create @name', array('@name' => $types['bookcrossing']->name)), PASS_THROUGH);
    return drupal_get_form('bookcrossing_node_form', $node, $bcid);
  }
}

/**
 * Form for adding book
 */
function bookcrossing_add_new_book_form() {
  global $user;

  $types = node_type_get_types();
  $node = (object) array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => 'bookcrossing', 'language' => LANGUAGE_NONE);
  //drupal_set_title(t('Create @name', array('@name' => $types['bookcrossing']->name)), PASS_THROUGH);
  return drupal_get_form('bookcrossing_node_form', $node, 'add-new-book');
}

/**
 * Menu callback: page for showing bcid of added book
 */
function bookcrossing_book_added($bcid = '') {
  if (empty($bcid)) {
    drupal_not_found();
    drupal_exit();
  }

  if (!isset($_SESSION['added-new-book'])) {
    drupal_not_found();
    drupal_exit();
  }





//  $output = '<div class="bcid-wrapper"><div class="bcid-number-wrapper">';
//  $output .= '<div class="bcid-number">' . t('This is the number of your book') . '<br />(BookcrossingID):</div>';
//  $output .= '<div class="bcid">' . $bcid . '</div>';
//  $output .= '<div class="bcid-descr">' . t('Write it on your book.') . '</div>';

  $book = bookcrossing_load_by_bcid($bcid);

//  drupal_add_js('http://connect.facebook.net/ru_RU/all.js', 'external');

  $app_id = variable_get('fboauth_id', '');
  $init = "window.fbAsyncInit = function() {
      FB.init({
        appId      : $app_id,
        status     : true,
        cookie     : true,
        xfbml      : true
      });
      release();
    };

     //Load the SDK Asynchronously
    (function(d){
      var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
      js = d.createElement('script'); js.id = id; js.async = true;
      js.src = '//connect.facebook.net/ru_RU/all.js';
      d.getElementsByTagName('head')[0].appendChild(js);
    }(document));
  ";

  drupal_add_js($init, 'inline');

  $parents = taxonomy_get_parents_all($book['place']->tid);
  $country = $parents[2]->name;
  $city = $parents[1]->name;
  $place = $parents[0]->name;
  $path = url('book/' . $book['bid'], array('absolute' => TRUE));

  $release = "function release(){
       FB.getLoginStatus(function(response) {
         if (response.status === 'connected') {
           accessToken = response.authResponse.accessToken;
           country = '{$country}';
           city = '{$city}';
           place = '{$place}';
           message = '';

           if (country != '' && city != '' && location != '') {
             FB.api(
               '/me/bookcrossing_by:release?book={$path}&country=' + country + '&city=' + city + '&location=' + place + '&message=' + message,
               'post',
                function(response) {
                  if (!response || response.error) {
                    //alert('Sorry, but error happened while publishing action.');
                    //console.log(response);
                  } 
               }
             );
           }
         }
       });
    }";

  drupal_add_js($release, 'inline');  

//  $output .= '<img class="qr-code" src="https://chart.googleapis.com/chart?cht=qr&chs=150x150&chl=' . $path . '&chld=H|1" />';

  $output = '<div class="bcid-wrapper">';
  $output .= '<div class="bcid-instructions"><p>' . t('Your book is registered. Now you may release it in a public place, so the next reader could find it. To track the journey of the book you should mark it with BCID.') . '</p><p>' . t('We have made a bookplate for you, that you could cut and paste into your book flyleaf:') . '</p></div>';
  $output .= '<div class="bcid-number-wrapper">';
  $rand_plate_img_num = rand(1,30);
  $output .= '<div class="bcid-image-plate" style="background-image: url(\'/sites/default/files/stickers/'. $rand_plate_img_num .'.jpg\');">&nbsp;</div>';
  $output .= '<div class="qr-code" style="background-image: url(\'https://chart.googleapis.com/chart?cht=qr&chs=100x100&chl=http%3A%2F%2Fbookcrossing.by%2Fbook%2F' . $book['bid'] . '&chld=L|1\');">&nbsp;</div>';
  $output .= '<div class="bcid-descr">' . t('I am a wonderful book. I travel the world. Read me, share your review and release me so others could find me') . '<br><br>' . t('Go to') . '<p style="font-weight: bold; font-size: 15px; padding: 5px;">bookcrossing.by</p>' . t('enter BCID (find below) to book history') . '</div>';
  $output .= '<div class="bcid">' . $bcid . '</div>';
  $output .= '</div><div class="bcid-scissors">&nbsp;</div></div>';

  unset($_SESSION['added-new-book']);

  return $output;
}
