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
    if ($num == BOOKCROSSING_RESERVED_BCIDS) {
      return NULL;
    }

    $commands = array();
    $generate = BOOKCROSSING_RESERVED_BCIDS - $num;

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
  $bids = db_select('bookcrossing_books', 'book')
      ->fields('book', array('bid'))
      ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->range(0, 3)
      ->orderBy('bid', 'DESC')
      ->execute()
      ->fetchCol();
  $books = bookcrossing_load_multiple($bids);

  $output = '';
  foreach ($books as $book) {
    $view = node_view($book['node'], 'front_page');
    $build = bookcrossing_prepare_book_view($view, $book);
    $output .= render($build);
  }

  return '<div class="front-page-books">' . $output . '</div>';
}

/**
 * Menu callback: release book.
 */
function bookcrossing_release_book($book) {
    // Book is not found in DB
    if (!$book) {
        watchdog('bookcrossing.pages.inc', 'Incorrect object was passed into bookcrossing_release_book() function.', WATCHDOG_NOTICE);
        return array(
            '#markup' => t("Book with such BCID is not registered. Please make sure you are entering correct BCID."),
        );
    }

    // Book is already released
    if (!$book['status']) {
        return array(
            '#markup' => t("Book with such BCID was already released."),
        );
    }

    global $user;
    // Can only release books user reading
    if ($book['user']->uid != $user->uid) {
        return array(
            '#markup' => t("You are trying to register as found book that was not released. If you believe this is incorrect behaviour, please report to <a href='mailto:bug@bookcrossing.by'>bug@bookcrossing.by</a>."),
        );
    }

    $build = node_view($book['node'], 'found_book');
    $build = bookcrossing_prepare_book_view($build, $book, FALSE);

    // Form for adding comment and place
    $build['book_found'] = drupal_get_form('bookcrossing_book_comment', $book);

    // Status of the found book
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

    $is_read_book = db_select('bookcrossing_history', 'history')
        ->fields('history', array('book_id'))
        ->condition('user_id', $user->uid, '=')
        ->condition('book_id', $bid, '=')
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
 * Menu callback: view history page.
 */
// @todo: Function needs to be refactred after re-structure of 'bookcrossing_history' table
function bookcrossing_view_history($book)
{
    $build = node_view($book['node'], 'history');
    global $user;
    $build = bookcrossing_prepare_book_view($build, $book, FALSE);

    drupal_set_title(t('Book history') . ' - ' . $book['node']->title);
    // Adding comment of the logged user to the output.
    $isset = FALSE;
    $my_comment = '<div id="my-comment-text">';
    if (isset($book['comments'][$user->uid]))
    {
        $my_comment .= $book['comments'][$user->uid];
        $isset = TRUE;
    }

    if (!$isset)
    {
        $query = db_select('bookcrossing_history', 'history');
        //$query->innerJoin('bookcrossing_books', 'b');
        $exist = $query
            ->fields('history', array('book_id'))
            ->condition('history.user_id', $user->uid, '=')
            ->condition('history.book_id', $book['bid'], '=')
            ->execute()
            ->fetchField();

        if ($exist)
        {
            $my_comment .= t("Would you recommend this book to a friend and why? Click 'change' to add a comment.");
        }
        else
        {
            $my_comment .= t("You can not write a review for a book you did not find.");
        }
    }
  
    $my_comment .= '</div>';

    $is_read_book = db_select('bookcrossing_history', 'history')
        ->fields('history', array('book_id'))
        ->condition('user_id', $user->uid, '=')
        ->condition('book_id', $book['bid'], '=')
        ->execute()
        ->fetchField();

    $change_link = '';
    if ($is_read_book)
    {
        $change_link = l(t('(change)'), 'change-comment/' . $book['bid'], array('attributes' => array('class' => array('use-ajax'))));
    }
    $build['book_comment'] = array(
        '#markup' => '<div class="back-to-book">' . l(t('<<back to book'), 'book/' . $book['bid']) . '</div><div class="my-comment"><div class="my-comment-title">' . t('My review') . $change_link . '</div>' . $my_comment . '</div>',
    );

    $build['author_and_year'] = array(
        '#markup' => bookcrossing_author_and_year($book),
    );

    // Generate output for history of the book.
    $output = '<div class="history-wrapper">';
    foreach ($book['history'] as $history)
    {
        $comment = isset($history['comment']) ? $history['comment'] : '';
        $my_comment_class = $history['user']->uid == $user->uid ? ' my-comment' : '';
        $user_link = '';
        $user_profile = bookcrossing_get_social_profile($history['user']);
        if ($user_profile)
        {
            $filepath = (isset($history['user']->picture->uri)) ? $history['user']->picture->uri : '';
            $name = !empty($history['user']->name) ? $history['user']->name : variable_get('anonymous', t('Anonymous'));
            $alt = t("@user's picture", array('@user' => $name));
            $image = theme('image_style', array('path' => $filepath, 'alt' => $alt, 'title' => $alt, 'style_name' => 'user-in-history'));
            $user_link = l($image, $user_profile['profileURL'], array('html' => TRUE));
        }
        $output .= '<div class="place-wrapper clearfix">
                      <div class="history-comment' . $my_comment_class . '">' . $comment . '</div>
                      <div class="history-user-foto">' . $user_link . '</div>
                      <div class="history-user-info">'
                . l($history['user']->name, $user_profile ? $user_profile['profileURL'] : 'user/' . $history['user']->uid)
                . '<div class="history-places">';

        switch ($history['event_type'])
        {
            case BOOKCROSSING_BOOK_STATUS_FREE:
                $output .= '<div class="place-left">' . t('Released') . ':<div class="place-date-left">' . format_date($history['date_time']) . ', ' . $history['place']->name . '</div></div>';
            break;
            case BOOKCROSSING_BOOK_STATUS_FOUND:
                $output .= '<div class="place-found">' . t('Found') . ':<div class="place-date-found">' . format_date($history['date_time']) . ',' . $history['place']->name . '</div></div>';
            break;
            default:
            break;
        }
        $output .= '</div></div></div>';
    }

    $output .= '</div>';

    $build['book_history'] = array(
        '#markup' => $output,
    );

    // Generate item list with status of the book.
    $build['book_status'] = array(
        '#markup' => bookcrossing_book_status($book, TRUE),
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
    '#markup' => bookcrossing_book_status($book, TRUE),
  );

  /**
   * Generate links for book: release, view history, add to favourites.
   */
  $items = array();

  global $user, $language;
  if ($user->uid == $book['user']->uid && $book['status']) {
    $items[] = l(t('Release'), 'release-book/' . $book['bid']);
  }

  // $items[] = l("<i class='icon-time'></i>" . t('View history'), 'view-history/' . $book['bid']);
  $items[] = '<i class="icon-time"></i> <a href="/' . $language->language . '/view-history/' . $book['bid'] . '">' . t('View history') . '</a>';

  $query = db_select('bookcrossing_favourites', 'f');
  $query->fields('f', array('uid'));
  $query->condition('f.bid', $book['bid'], '=');
  $query->condition('f.uid', $user->uid, '=');
  $already_added = $query->execute()->fetchField();

  if ($user->uid && !$already_added) {
    /**
     * @todo add facebook like action.
     */
    // $items[] = l(t('Add to bookmarks'), 'add-to-bookmarks/' . $book['bid'], array('attributes' => array('onclick' => 'return like()')));
    $items[] = '<i class="icon-bookmark"></i> <a href="/add-to-bookmarks/' . $book['bid'] . '" onclick="return like()">' . t('Add to bookmarks') . '</a>';
  }

  $build['book_links'] = array(
    '#markup' => theme_item_list(array('title' => '', 'items' => $items, 'type' => 'ul', 'attributes' => array('class' => 'book-links'))),
  );

  bookcrossing_meta_og($book);

  // $_SESSION['book_to_like'] = $book['bid'];
  
  // bookcrossing_add_fb_js($book['bid'], url('book/' . $book['bid'], array('absolute' => TRUE)));

  $data_href = url('book/' . $book['bid'], array('absolute' => TRUE));


  return $build;
}


function bookcrossing_add_fb_js($bid, $path = '', $type = 'bookmark') {
  $msg_text = t('Facebook returned the following message: ');
  $msg_type = t('');
  $msg_summary = t('Book could be added to bookmarks only once. You\'ve already added this book.');
  $msg_success = t('Post was successful! Action ID: ');
  $like = "function like(){
      FB.api(
        'https://graph.facebook.com/me/bookcrossing_by:bookmark?book=$path',
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
      '#markup' => t('There is no book with such BCID.'),
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
  $status_string = $book['status'] ? t('Found') : t('Travelling');
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
  global $user;

  if (empty($bcid)) {
    drupal_not_found();
    drupal_exit();
  }

  if (!isset($_SESSION['added-new-book'])) {
    drupal_not_found();
    drupal_exit();
  }

  $book = bookcrossing_load_by_bcid($bcid);

  $parents = taxonomy_get_parents_all($book['place']->tid);
  $country = $parents[2]->name;
  $city = $parents[1]->name;
  $place = $parents[0]->name;
  $path = url('book/' . $book['bid'], array('absolute' => TRUE));

//  $output .= '<img class="qr-code" src="https://chart.googleapis.com/chart?cht=qr&chs=150x150&chl=' . $path . '&chld=H|1" />';

  $exlibris_url = '/sites/default/files/stickers/'. rand(1,30) .'.jpg';
  $gift_url = '/sites/default/files/gifts/'. rand(1,7) .'.jpg';

  $output = '';
  $output .= '<div class="span8">';
  $output .= '  <div class="bcid-wrapper">';
  $output .= '    <div class="bcid-instructions">';
  $output .= '      <p>' . t('Your book is registered. Now you may release it in a public place, so the next reader could find it. To track the journey of the book you should mark it with BCID.') . '</p>';
  $output .= '      <p>' . t('We have made a bookplate for you, that you could cut and paste into your book flyleaf:') . '</p>';
  $output .= '    </div>';
  $output .= '    <div class="bcid-number-wrapper">';
  $output .= '      <div class="bcid-image-plate" style="background-image: url(\''. $exlibris_url .'\');">&nbsp;</div>';
  $output .= '      <div class="qr-code" style="background-image: url(\'https://chart.googleapis.com/chart?cht=qr&chs=100x100&chl=http%3A%2F%2Fbookcrossing.by%2Fbook%2F' . $book['bid'] . '&chld=L|1\');">&nbsp;</div>';
  $output .= '      <div class="bcid-descr">' . t('I am a wonderful book. I travel the world. Read me, share your review and release me so others could find me') . '<br><br>' . t('Go to') . '<p style="font-weight: bold; font-size: 15px; padding: 5px;">bookcrossing.by</p>' . t('enter BCID (find below) to book history') . '</div>';
  $output .= '      <div class="bcid">' . $bcid . '</div>';
  $output .= '    </div>';
  $output .= '    <div class="bcid-scissors">&nbsp;</div>';
  $output .= '    <div class="bcid-instructions hidden-phone">';
  $output .= '      <p style="width:300px;"><img src="/sites/default/files/help/book-added-help.png" /></p>';
  $output .= '    </div>';
  $output .= '  </div>';
  $output .= '</div>';

  // Add css and js here, so it's always available when we're showing buttons.
  // Don't have to worry about adding them more than once.
  $output .= '<div class="span3">';
  $loaded = libraries_load('rrssb');
  if($loaded) 
  {
    $output .= '<div class="bcid-instructions"><h3>' . t('Spread the word') . '</h3>';
    $output .= '<p>' . t('Share with your friends where you have left the book') . '</p>';
    // $output .= '<a href="https://www.facebook.com/sharer/sharer.php?u='.$path.'" class="btn btn-primary">Facebook</a>';
    // $output .= '<a href="#" class="btn btn-primary">Vkontakte</a>';
    // $output .= '<a href="https://twitter.com/intent/tweet?text='.t('I have just released a book. Check it out.').' '.$path.'" class="btn btn-primary">Twitter</a>';
    $output .= '<p>' . bookcrossing_share_buttons(array('facebook','vk','twitter','pinterest'), t('I have just released a book. Check it out.'), 'img', $path) . '</p>';
    $output .= '</div>';
  }

  $output .= '<div class="bcid-instructions"><h3>' . t('Thank you for a book!') . '</h3><p>'.t('And here is a gift for you').'</p></div>';
  $output .= '<p><img src="'.$gift_url.'"/></p>';
  $output .= '<div class="bcid-instructions"><p>' . t('More books you read, more gifts you get ;)') . '</p></div>';

  $output .= '<h3>'.t('What next').'</h3>';
  $output .= '<p><i class="icon-chevron-right"></i> '.l(t('Release one more book'), url('release-book', array('absolute' => TRUE)) ).'</p>';
  $output .= '<p><i class="icon-chevron-right"></i> '.l(t('See a book you have released'), $path).'</p>';
  $output .= '<p><i class="icon-chevron-right"></i> '.l(t('Go hunting for books in our catalog'), url('books', array('absolute' => TRUE)) ).'</p>';
  $output .= '</div>';

  // $output .= '<div class="row">';
  // $output .= '<div class="span12">';

  // $output .= '</div></div>';

  unset($_SESSION['added-new-book']);

  return $output;
}
