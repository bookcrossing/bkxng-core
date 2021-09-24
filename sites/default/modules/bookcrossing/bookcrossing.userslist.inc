<?php

/**
 * @file
 * Users list
 */

/**
 * Array of available users sort types.
 * 
 * @return array
 *   types of user sorting in users list
 */
function bookcrossing_users_list_sort_types() {
  $types = array(
    'date' => array(
      'title' => t('date'),
      'callback' => 'bookcrossing_users_list_sort_by_date',
      'path' => '',
    ),
    'name' => array(
      'title' => t('name'),
      'callback' => 'bookcrossing_users_list_sort_by_name',
      'path' => 'name',
    ),
  );
  
  return $types;
}

/**
 * Menu callback: show users list
 */
function bookcrossing_users_list($type = 'date') {
  $types = bookcrossing_users_list_sort_types();
  
  // if there is no type with this name, show not found
  if (!isset($types[$type])) {
    drupal_not_found();
    drupal_exit();
  }
  
  $users = array();
  if (isset($types[$type]['callback'])) {
    $function = $types[$type]['callback'];
    if (function_exists($function)) {
      $users = $function();
    }
    else {
      drupal_not_found();
      drupal_exit();
    }
  }

  $output  = '';
  $output .= '<div class="catalog-controls user-list-controls">';
  $output .= '<div class="catalog-sorts">';
  $output .= bookcrossing_users_list_veiw_sort_types();
  $output .= '</div>';
  $output .= paginator3000pager();
  $output .= '</div>';
  
//  dpm($users);

  if ($users) {
    $output .= bookcrossing_users_list_show_users($users);
  }
  
  return $output;
}

/**
 * View sort types.
 */
function bookcrossing_users_list_veiw_sort_types() {
  $types = bookcrossing_users_list_sort_types();
  
  $output = '';
  foreach ($types as $name => $type) {
    $title = isset($type['title']) ? $type['title'] : '';
    $path = 'users';
    $view_path = isset($type['path']) ? $type['path'] : '';

    if (!empty($view_path)) {
      $path .= '/' . $type['path'];
    } 
    
    $classes = array();
    if (arg(1) == $view_path) {
      $classes[] = 'active';
    }

    $output .= l($type['title'], $path, array('attributes' => array('class' => $classes)));
  }
  
  return $output;
}

/**
 * Show users list
 */
function bookcrossing_users_list_show_users($users = array()) {
  $output = '';
  
  $i = 1;
  foreach ($users as $user) {
    $zebra = $i % 2 == 0 ? 'user-even' : 'user-odd';
    $view = user_view($user, 'user_list');
    $output .= '<div class="' . $zebra . '">' . render($view) . '</div>';
    $i++;
  }

  return $output;
}

/**
 * List of users sorted by user date
 */
function bookcrossing_users_list_sort_by_name() {
  $limit = variable_get('bookcrossing_users_list_pager_limit', 10);
  
  $query = db_select('users', 'u');
  $query->innerJoin('hybridauth_identity', 'ha', 'u.uid = ha.uid');
  $query = $query->extend('PagerDefault');
  $query->limit($limit);
  
  $uids = $query->fields('u', array('uid'))
      ->condition('u.status', 1, '=')
      ->orderBy('u.name', 'ASC')
      ->execute()
      ->fetchCol();
  
  return user_load_multiple($uids);
}

/**
 * List of users sorted by user name
 */
function bookcrossing_users_list_sort_by_date() {
  $limit = variable_get('bookcrossing_users_list_pager_limit', 10);

  $query = db_select('users', 'u');
  $query->innerJoin('hybridauth_identity', 'ha', 'u.uid = ha.uid');
  $query = $query->extend('PagerDefault');

  $query->fields('u', array('uid'));
  $query->condition('u.status', 1, '=');
  $query->orderBy('u.created', 'DESC');
  $query->limit($limit);

  $uids = $query->execute()->fetchCol();

  return user_load_multiple($uids);
}
