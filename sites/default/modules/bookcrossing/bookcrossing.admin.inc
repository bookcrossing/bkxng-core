<?php

/**
 * @file
 * Admin pages for bookcrossing
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
    $exist = db_select('bookcrossing_books', 'b')
      ->fields('b', array('bid'))
      ->condition('b.bcid', $form_state['values']['bcid'], '=')
      ->execute()
      ->fetchField();
    
    if ($exist) {
      $b = bookcrossing_load($exist);
      form_set_error('bcid', t('BCID you have entered already exists') . ': ' . l($b['node']->title, 'book/' . $b['bid']));
    }
  }
  
  if ($form_state['values']['activate_release'] && empty($form_state['values']['place_left'][0])) {
    form_set_error('place_left', t('Select place where left book'));
  }
}

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

      $update = db_update('bookcrossing_history')
        ->fields(array(
            'uid' => $form_state['values']['user_released'],
            'place_id_left' => $form_state['values']['place_left'][0],
            'time_left' => REQUEST_TIME,
          ))
        ->condition('bid', $form_state['book']['bid'], '=')
        ->condition('time_left', 0, '=')
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
        'status' => BOOKCROSSING_BOOK_STATUS_READ,
      ));

      $insert = db_insert('bookcrossing_history')
        ->fields(array(
            'uid' => $form_state['values']['owner'],
            'place_id' => $form_state['values']['place_found'][0],
            'place_id_left' => 0,
            'bid' => $form_state['book']['bid'],
            'found' => REQUEST_TIME,
            'time_left' => 0,
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
  $limit = 20;
  $query = db_select('bookcrossing_comment', 'c');
  $query->fields('c', array('cid', 'uid', 'bid', 'comment'));
  $query = $query->extend('PagerDefault');
  $query->limit($limit);

  $comments = $query->execute()->fetchAll();

  $header = array('book' => t('Book'), 'user' => t('User'), 'comment' => t('Comment'));
  foreach ($comments as $comment) {
    $book = bookcrossing_load($comment->bid);
    $user = user_load($comment->uid);
    $rows[$comment->cid] = array(
      'book' => l($book['node']->title . '- ID:' . $book['bid'], 'book/' . $book['bid']),
      'user' => l($user->name, 'user/' . $user->uid),
      'comment' => $comment->comment,
    );
  }

  $form = array();

  $form['comments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#attributes' => array(),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

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
  $limit = 10;

  $query = db_select('bookcrossing_books', 'b');
  $query->fields('b', array('bid'));
  $query->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
  $query = $query->extend('PagerDefault');
  $query->limit($limit);
  $bids = $query->execute()->fetchCol();

//  drupal_add_library('system', 'ui.dialog');
  $books = bookcrossing_load_multiple($bids);
//  print "<pre>"; print_r($books); print "</pre>";
  $header = array('bcid' => 'BCID', 'title' => t('Book title'), 'status' => t('Book status'), 'where' => t('Where?'), 'change' => t('Edit book'));
  foreach ($books as $book) {
    $where = $book['status'] ? l($book['user']->name, 'user/' . $book['user']->uid) : taxonomy_term_load($book['place_id_left'])->name;
    $status = $book['status'] ? t('Found') : t('Free book');
    $rows[$book['bid']] = array(
      'bcid' => $book['bcid'],
      'title' => l($book['node']->title, 'book/' . $book['bid']),
      'status' => $status,
      'where' => $where,
      'change' => l(t('Change'), 'admin/bookcrossing/books/' . $book['bid'] . '/edit'),
    );
  }

  $form = array();

  $form['books'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#attributes' => array(),
  );

  $form['pager'] = array(
    '#theme' => 'pager',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

//  return theme('table', array('header' => $header, 'rows' => $rows));

  return $form;
}

/**
 * Submit for deleting books
 */
function bookcrossing_admin_show_books_submit($form, $form_state) {
  $bids = array_filter($form_state['values']['books']);
  if (!empty($bids)) {
    db_delete('bookcrossing_books')
        ->condition('bid', $bids, 'IN')
        ->execute();

    db_delete('bookcrossing_history')
        ->condition('bid', $bids, 'IN')
        ->execute();

    db_delete('bookcrossing_comment')
        ->condition('bid', $bids, 'IN')
        ->execute();

    db_delete('bookcrossing_favourites')
        ->condition('bid', $bids, 'IN')
        ->execute();
  }

  drupal_set_message(t('@count books were deleted', array('@count' => count($bids))));
}

/**
 * Form for merging books descriptions
 */
function bookcrossing_merge_books($form, $form_state) {
  $form = array();

  $nids = db_select('node', 'n')
      ->fields('n', array('nid'))
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
