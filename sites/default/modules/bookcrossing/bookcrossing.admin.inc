<?php

/**
 * @file
 * Admin pages for bookcrossing
 * @param $js
 * @param $book
 * @return array|mixed
 */

function bookcrossing_modal_edit_book($js, $book) {
  if (!$js) {
    return drupal_get_form('bookcrossing_edit_book', $book);
  }

  ctools_include('modal');
  ctools_include('ajax');
  $form_state = array(
    'title' => t('Edit @book'. array('@book' => $book['node']->title)),
    'ajax' => TRUE,
  );

  $form_state += array(
    're_render' => FALSE,
    'no_redirect' => !empty($form_state['ajax']),
  );

  $output = druapl_get_form('bookcrossing_edit_book', $book);

  if (!empty($form_state['ajax']) && empty($form_state['executed'])) {
    $output = ctools_modal_form_render($form_state, $output);
  }

  if (!empty($form_state['executed'])) {
    // We'll just overwrite the form output if it was successful.
    $output = array();
    $output[] = ctools_modal_command_dismiss();
  }

  print ajax_render($output);
  exit; 
}

function bookcrossing_edit_book($form, $form_state, $book) {
  drupal_set_title(t('Edit @book', array('@book' => $book['node']->title)));

  $form = array();

  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.status', 1, '=')
    ->condition('type', 'bookcrossing', '=')
    ->execute()
    ->fetchCol();

  $n = node_load_multiple($nids);
  foreach ($n as $node) {
    $nodes[$node->nid] = $node->title;
  }

  $form['book_class'] = array(
    '#type' => 'select',
    '#title' => t('Books classificator'),
    '#options' => $nodes,
    '#default_value' => $book['node']->nid,
  );

  $form['bcid'] = array(
    '#type' => 'textfield',
    '#title' => t('BCID'),
    '#default_value' => $book['bcid'],
  );

  $status = $book['status'] ? t('Found') : t('Free');
  $form['status'] = array(
    '#markup' => '<div><b>' . t('Book status') . ':</b> ' . $status . '</div>',
  );

  /*if ($book['color']) {
    $form['color'] = array(
      '#type' => 'textfield',
      '#title' => t('Cover'),
      '#default_value' => $book['color'],
    );
  }*/

  $uids = db_select('users', 'u')
    ->fields('u', array('uid'))
    ->condition('u.status', 1, '=')
    ->execute()
    ->fetchCol();

  $u = user_load_multiple($uids);
  foreach ($u as $user) {
    $users[$user->uid] = $user->name;
  }

  $default = !$book['status'] ? array($book['place_id_left']) : array();

  if ($book['status']) {
  $form['activate_release'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change status to released'),
  );

  $form['release'] = array(
    '#type' => 'fieldset',
    '#title' => t('Changing status to released'),
    '#states' => array(
      'visible' => array(
        '#edit-activate-release' => array('checked' => TRUE),
      ),
    ),
  );

  $form['release']['user_released'] = array(
    '#type' => 'select',
    '#title' => t('User who read this book'),
    '#default_value' => $book['status'] ? $book['owner'] : '',
    '#options' => $users,
  );

  $form['release']['place_left'] = array(
    '#type' => 'hierarchical_select',
    '#title' => t('Place where book will be left.'),
    '#size' => 1,
    '#default_value' => $default,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => BOOKCROSSING_PLACES_VID,
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 1,
      'entity_count'    => 0,
      'require_entity'  => 0,
      'resizable'       => 1,
      'level_labels' => array(
        'status' => 0,
        'labels' => array(
          0 => t('Country'),
          1 => t('City'),
          2 => t('Place'),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'),
        'limit'    => 0,
        'reset_hs' => 1,
      ),
      'editability' => array(
        'status'           => 1,
        'item_types'       => array(
          1 => t('city'),
          2 => t('place'),
        ),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 3,
      ),
      'animation_delay'    => 400,
      'special_items'      => array(),
      'render_flat_select' => 0,
    ),
  ); 
  }
  else {
  $form['activate_found'] = array(
    '#type' => 'checkbox',
    '#title' => t('Change status to found'),
  );

  $form['found'] = array(
    '#type' => 'fieldset',
    '#title' => t('Changing status to found'),
    '#states' => array(
      'visible' => array(
        '#edit-activate-found' => array('checked' => TRUE),
      ),
    ),
  );

  $form['found']['owner'] = array(
    '#type' => 'select',
    '#title' => t('Owner'),
    '#default_value' => $book['status'] ? $book['owner'] : '',
    '#options' => $users,
  );

  $form['found']['place_found'] = array(
    '#type' => 'hierarchical_select',
    '#title' => t('Place where book was found.'),
    '#size' => 1,
    '#default_value' => $default,
    '#config' => array(
      'module' => 'hs_taxonomy',
      'params' => array(
        'vid' => BOOKCROSSING_PLACES_VID,
      ),
      'save_lineage'    => 0,
      'enforce_deepest' => 1,
      'entity_count'    => 0,
      'require_entity'  => 0,
      'resizable'       => 1,
      'level_labels' => array(
        'status' => 0,
        'labels' => array(
          0 => t('Country'),
          1 => t('City'),
          2 => t('Place'),
        ),
      ),
      'dropbox' => array(
        'status'   => 0,
        'title'    => t('All selections'),
        'limit'    => 0,
        'reset_hs' => 1,
      ),
      'editability' => array(
        'status'           => 1,
        'item_types'       => array(
          1 => t('city'),
          2 => t('place'),
        ),
        'allowed_levels'   => array(
          0 => 0,
          1 => 0,
          2 => 1,
        ),
        'allow_new_levels' => 0,
        'max_levels'       => 3,
      ),
      'animation_delay'    => 400,
      'special_items'      => array(),
      'render_flat_select' => 0,
    ),
  ); 
  }

  $form_state['book'] = $book;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function bookcrossing_edit_book_validate($form, $form_state) {
  if ($form_state['book']['bcid'] != $form_state['values']['bcid']) {
    $exist = db_select('bookcrossing_books', 'book')
      ->fields('book', array('bid'))
      ->condition('book.bcid', $form_state['values']['bcid'], '=')
      ->execute()
      ->fetchField();
    
    if ($exist) {
      $book = bookcrossing_load($exist);
      form_set_error('bcid', t('BCID you have entered already exists') . ': ' . l($book['node']->title, 'book/' . $book['bid']));
    }
  }
  
  if ($form_state['values']['activate_release'] && empty($form_state['values']['place_left'][0])) {
    form_set_error('place_left', t('Select place where left book'));
  }
}


//  @todo: !Important! This function needs to be refactored in accordance with new structure of 'bookcrossing_history' table
function bookcrossing_edit_book_submit($form, $form_state) {
  if ($form_state['book']['status']) {
    $query = db_update('bookcrossing_books');
    
    if ($form_state['values']['activate_release']) {
      $query->fields(array(
        'nid' => $form_state['values']['book_class'],
        'bcid' => filter_xss($form_state['values']['bcid'], array()),
        'owner' => -1,
        'current_place' => $form_state['values']['place_left'][0],
        'status' => BOOKCROSSING_BOOK_STATUS_FREE,
      ));

//  @todo: 'place_id_left' - needs to be replaced with 'place_id' and corresponding 'event_type'
      $update = db_update('bookcrossing_history')
        ->fields(array(
            'user_id' => $form_state['values']['user_released'],
            'place_id' => $form_state['values']['place_left'][0],
            'date_time' => REQUEST_TIME,
          ))
        ->condition('book_id', $form_state['book']['bid'], '=')
        ->condition('date_time', 0, '=')
        ->execute();
    }
    else {
      $query->fields(array(
        'nid' => $form_state['values']['book_class'],
        'bcid' => filter_xss($form_state['values']['bcid'], array()),
      ));
    }

    $query->condition('bid', $form_state['book']['bid'], '=');
    $query->execute();
  }
  else {
    $query = db_update('bookcrossing_books');
    
    if ($form_state['values']['activate_found']) {
      $query->fields(array(
        'nid' => $form_state['values']['book_class'],
        'bcid' => filter_xss($form_state['values']['bcid'], array()),
        'owner' => $form_state['values']['owner'],
        'current_place' => 0,
        'status' => BOOKCROSSING_BOOK_STATUS_FOUND,
      ));

      $insert = db_insert('bookcrossing_history')
        ->fields(array(
            'user_id' => $form_state['values']['owner'],
            'place_id' => $form_state['values']['place_found'][0],
            'event_type' => BOOKCROSSING_BOOK_STATUS_FOUND,
            'book_id' => $form_state['book']['bid'],
            'date_time' => REQUEST_TIME,
          ))
        ->execute();
    }
    else {
      $query->fields(array(
        'nid' => $form_state['values']['book_class'],
        'bcid' => filter_xss($form_state['values']['bcid'], array()),
      ));
    }

    $query->condition('bid', $form_state['book']['bid'], '=');
    $query->execute();
  }

  drupal_set_message(t('Book was saved.'));
}

function bookcrossing_admin_show_comments($form, $form_state) {
  $limit = 50;

  $header = array(
    'id' => array('data' => t('ID'), 'field' => 'comment.cid'),
    'book_title' => array('data' => t('Book Item'), 'field' => 'node.title'),
    'bcid' => array('data' => t('BCID')),
    'username' => array('data' => t('Username'), 'field' => 'comment.uid'),
    'review' => array('data' => t('Review'), 'field' => 'comment.comment'),
  );

  $query = db_select('bookcrossing_comment', 'comment');
  $query->innerJoin('bookcrossing_books', 'book', 'comment.bid = book.bid');
  $query->innerJoin('node', 'node', 'book.nid = node.nid');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('book', array('bcid'))
    ->fields('comment', array('cid', 'uid', 'bid', 'comment'))
    ->fields('node', array('title'))
    ->limit($limit)
    ->orderByHeader($header);

  $comments = $query->execute()->fetchAll();

  foreach ($comments as $comment) {
//    $book = bookcrossing_load($comment->bid);
    $user = user_load($comment->uid);
    $rows[$comment->cid] = array(
      'id' => $comment->cid,
      'book_title' => l($comment->title, 'book/' . $comment->bid),
      'bcid' => $comment->bcid,
      'username' => l($user->name, 'user/' . $user->uid),
      'review' => $comment->comment,
    );
  }

  $form = array();

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  $form['comments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No reviews available.'),
    '#attributes' => array(),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;
}


function bookcrossing_admin_show_comments_submit($form, $form_state) {
  $cids = array_filter($form_state['values']['comments']);
  if (!empty($cids)) {
    db_delete('bookcrossing_comment')
        ->condition('cid', $cids, 'IN')
        ->execute();
  }

  drupal_set_message(t('@count comments were deleted', array('@count' => count($cids))));
}


/**
 * Form shows all books
 * Can change book class
 */
function bookcrossing_admin_show_books() {
  $limit = 50;

  $header = array(
    'book_title' => array('data' => t('Book Item'), 'field' => 'node.title'),
    'bcid' => array('data' => t('BCID'), 'field' => 'book.bcid'),
    'status' => array('data' => t('Status'), 'field' => 'book.status'),
    'place' => array('data' => t('Where?'), 'field' => 'book.current_place'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('bookcrossing_books', 'book');
  $query->innerJoin('node', 'node', 'book.nid = node.nid');
  $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

  $query = $query->extend('PagerDefault')->extend('TableSort');
  $query
    ->fields('book', array('bid', 'bcid', 'status', 'owner', 'current_place'))
    ->fields('node', array('title'))
    ->limit($limit)
    ->orderByHeader($header);

  $book_items = $query->execute()->fetchAll();

  foreach ($book_items as $book_item) {
    $where = $book_item->status ? l(user_load($book_item->uid)->name, 'user/$book_item->uid') : taxonomy_term_load($book_item->current_place)->name;
    if ($book_item->status == BOOKCROSSING_BOOK_STATUS_FOUND) $status = t('found');
    if ($book_item->status == BOOKCROSSING_BOOK_STATUS_FREE) $status = t('travelling');
//    $status = $book_item->status ? t('Found') : t('Travelling');
    $rows[$book_item->bid] = array(
      'book_title' => $book_item->title,
      'bcid' => $book_item->bcid,
      'status' => $status,
      'place' => $where,
      'operations' => l(t('Change'), 'admin/config/bookcrossing/books/' . $book_item->bid . '/edit'),
    );
  }

  $form = array();

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Operation'),
    '#title_display' => 'invisible',
    '#options' => array(
      'status_found' => t('Change status of selected book items to Found'),
      'status_travelling' => t('Change status of selected book items to Travelling'),
      'delete' => t('Delete selected book items'),
    ),
//    '#default_value' => 'status_found',
  );

  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  $form['books'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No book items available.'),
    '#attributes' => array(),
  );

  $form['pager'] = array('#markup' => theme('pager'));

//  return theme('table', array('header' => $header, 'rows' => $rows));

  return $form;
}

/**
 * Submit for deleting books
 */
function bookcrossing_admin_show_books_submit($form, $form_state) {
  $bids = array_filter($form_state['values']['books']);

  dpm($form_state);
  dpm(db_like($form_state['values']['book_item']));

  switch ($form_state['input']['operation']) {
    case 'status_found':
    case 'status_travelling':
      if(isset($form_state['values']['book_item'])) {
        $query = db_select('bookcrossing_books', 'book');
        $query->innerJoin('node', 'node', 'book.nid = node.nid');
        $query->condition('node.title', '%'.db_like($form_state['values']['book_item']).'%', 'LIKE');
      }
      // if (isset($form_state['filters']['bcid'])) {
      //   $query->condition('b.bcid', '%' . db_like($form_state['filters']['bcid']) . '%', 'LIKE');
      // }
      // if (isset($form_state['filters']['status'])) {
      //   $query->condition('b.status', '%' . db_like($form_state['filters']['status']) . '%', 'LIKE');
      // }
      // if (isset($form_state['filters']['place'])) {
      //   $query->condition('b.current_place', '%' . db_like($form_state['filters']['place']) . '%', 'LIKE');
      // }
      break;

    case 'delete':
      if (!empty($bids)) {
        db_delete('bookcrossing_books')
            ->condition('bid', $bids, 'IN')
            ->execute();

        db_delete('bookcrossing_history')
            ->condition('book_id', $bids, 'IN')
            ->execute();

        db_delete('bookcrossing_comment')
            ->condition('bid', $bids, 'IN')
            ->execute();

        db_delete('bookcrossing_favourites')
            ->condition('bid', $bids, 'IN')
            ->execute();
      }

      drupal_set_message(t('@count books were deleted', array('@count' => count($bids))));

      break;

//    case t('Refine'):
//      // Apply every filter that has a choice selected other than 'any'.
//      foreach ($filters as $filter => $options) {
//        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
//          // Flatten the options array to accommodate hierarchical/nested options.
//          $flat_options = form_options_flatten($filters[$filter]['options']);
//          // Only accept valid selections offered on the dropdown, block bad input.
//          if (isset($flat_options[$form_state['values'][$filter]])) {
//            $_SESSION['node_overview_filter'][] = array($filter, $form_state['values'][$filter]);
//          }
//        }
//      }
//      break;
    case t('Undo'):
      array_pop($_SESSION['node_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['node_overview_filter'] = array();
      break;
  }

}

/**
 * Form for merging books descriptions
 */
function bookcrossing_merge_books($form, $form_state) {
  $form = array();

  $nids = db_select('node', 'node')
      ->fields('node', array('nid'))
      ->condition('type', 'bookcrossing', '=')
      ->execute()
      ->fetchCol();

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    $options[$node->nid] = $node->title;
  }

  $form['nodes'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#multiple' => TRUE,
    '#title' => t('Select book descriptions to merge.'),
    '#size' => 10,
  );

  $new_options = array('<select>' => t('Select node')) + $options;

  $form['new-name'] = array(
    '#type' => 'select',
    '#options' => $new_options,
    '#title' => t('Merge into'),
  );

//  $form['new-node-name'] = array(
//    '#type' => 'textfield',
//    '#title' => t('New book description'),
//  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Merging books
 */
function bookcrossing_merge_books_submit($form, $form_state) {
  if ($form_state['values']['new-name'] != '<select>') {
    $nids = array_filter($form_state['values']['nodes']);

    $nids_to_delete = $nids;
    unset($nids_to_delete[$form_state['values']['new-name']]);

    db_update('bookcrossing_books')
        ->fields(array('nid' => $form_state['values']['new-name']))
        ->condition('nid', $nids_to_delete, 'IN')
        ->execute();

    node_delete_multiple($nids_to_delete);
  }
  else {
    drupal_set_message(t('Select to which node merge selected.'), 'error');
  }
}

/**
 * Bookcrossing Settings Page
 *
 * @todo Add bookcrossing_catalog_font_sizes to settings
 *
 */
function bookcrossing_page_settings($form, &$form_state, $key = '')
{

    $form['default_location'] = array(
        '#type' => 'fieldset',
        '#title' => t('Default location'),
    );

    $countries = bookcrossing_terms_to_options(taxonomy_get_tree(3, 0, 1));
    if (isset($form_state['values']['country']) && $form_state['values']['country']) {
        $country = $form_state['values']['country'];
    } else {
        $country = variable_get('safezone-default-country', key($countries));
    }

    $form['default_location']['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => $countries,
        '#default_value' => $country,
        '#ajax' => array(
            'wrapper' => 'default-location-cities',
            'callback' => 'bookcrossing_callback_update',
        ),
    );

    $cities = bookcrossing_terms_to_options(taxonomy_get_tree(3, $country, 1));

    if (isset($form_state['values']['city']) && array_key_exists($form_state['values']['city'], $cities)) {
        $city = $form_state['values']['city'];
    } else {
        $city = variable_get('safezone-default-city', key($cities));
    }

    $form['default_location']['city'] = array(
        '#type' => 'select',
        '#title' => t('City'),
        '#options' => $cities,
        '#default_value' => $city,
        '#prefix' => '<div id="default-location-cities">',
        '#suffix' => '</div>',
    );

    $form['anonymous'] = array(
        '#type' => 'textfield',
        '#title' => t('Default name for Anonymous user'),
        '#default_value' => variable_get('anonymous', t('Anonymous')),
    );

    $form['rrssb_youtube_username'] = array(
        '#type' => 'textfield',
        '#title' => t('YouTube username'),
        '#default_value' => variable_get('rrssb_youtube_username', ''),
    );

    $form['rrssb_github_username'] = array(
        '#type' => 'textfield',
        '#title' => t('GitHub username'),
        '#default_value' => variable_get('rrssb_github_username', ''),
    );

    $form['fboauth_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Facebook OAuth Id'),
        '#default_value' => variable_get('fboauth_id', ''),
    );

    $form['bookcrossing_history_pager_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Page size of Book History log'),
        '#default_value' => variable_get('bookcrossing_history_pager_limit', '5'),
        '#size' => 10,
    );

    $form['bookcrossing_users_list_pager_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of users to be displayed per page'),
        '#default_value' => variable_get('bookcrossing_users_list_pager_limit', '10'),
        '#size' => 10,
    );

    // $defaults = array(
    //     '0' => '14',
    //     '10' => '16',
    //     '20' => '20',
    //     '30' => '24',
    //     '40' => '28',
    //     '50' => '32',
    //     '60' => '36',
    //     '70' => '40',
    //     '80' => '44',
    //     '90' => '48',
    //     '100' => '52',
    // );
    // variable_get('bookcrossing_catalog_font_sizes', $defaults);

    $form['bookcrossing_catalog_books_per_page'] = array(
        '#type' => 'textfield',
        '#title' => t('Number of book per page displayed in catalog'),
        '#default_value' => variable_get('bookcrossing_catalog_books_per_page', '30'),
        '#size' => 10,
    );

    $form['access_login_form'] = array(
        '#type' => 'checkbox',
        '#title' => t('Allow access to standard login form'),
        '#default_value' => variable_get('access_login_form', 0),
    );


    $form['#submit'][] = 'bookcrossing_page_settings_submit';
    return system_settings_form($form);
}

function bookcrossing_callback_update($form, $form_state)
{
    return $form['default_location']['city'];
}

function bookcrossing_page_settings_submit($form, &$form_state)
{
    $values = $form_state['values'];

    variable_set('safezone-default-country', (int)$values['country']);
    variable_set('safezone-default-city', (int)$values['city']);
    variable_set('anonymous', (string)$values['anonymous']);
    variable_set('rrssb_youtube_username', (string)$values['rrssb_youtube_username']);
    variable_set('rrssb_github_username', (string)$values['rrssb_github_username']);
    variable_set('fboauth_id', (string)$values['fboauth_id']);
    variable_set('bookcrossing_history_pager_limit', (int)$values['bookcrossing_history_pager_limit']);
    variable_set('bookcrossing_users_list_pager_limit', (int)$values['bookcrossing_users_list_pager_limit']);
    // variable_set('bookcrossing_catalog_font_sizes', (int)$values['bookcrossing_catalog_font_sizes']);
    variable_set('bookcrossing_catalog_books_per_page', (int)$values['bookcrossing_catalog_books_per_page']);
    variable_set('access_login_form', (int)$values['access_login_form']);
}

/**
 *
 */
function bookcrossing_goto_merge($form, $form_state)
{
    drupal_goto('admin/config/bookcrossing/merge-books');
}

/**
 * Implements hook_form_FORM_ID_alter()
 */
function bookcrossing_form_node_admin_content_alter(&$form, $form_state)
{
    $form['merge-book'] = array(
        '#type' => 'submit',
        '#submit' => array(
            'bookcrossing_goto_merge',
        ),
        '#value' => t('Merge book descriptions'),
        '#weight' => -10,
    );
}
