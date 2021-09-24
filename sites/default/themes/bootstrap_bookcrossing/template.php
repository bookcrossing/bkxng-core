<?php

/**
 * Implements hook_preprocess_page().
 */
function bootstrap_bookcrossing_preprocess_page(&$vars, $hook) {
  if (isset($vars['node']->type)) {
    // If the content type's machine name is "my_machine_name" the file
    // name will be "page--my-machine-name.tpl.php".
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
  }
  if (isset($_GET['response_type']) && $_GET['response_type'] == 'raw') {
    $vars['theme_hook_suggestions'][] = 'page__raw';
  }
  if (isset($_GET['response_type']) && $_GET['response_type'] == 'facebook') {
    $vars['theme_hook_suggestions'][] = 'page__fb';
  }
}

/**
 * Implements hook_preprocess_html().
 */
function bootstrap_bookcrossing_preprocess_html(&$vars) {
  if (isset($_GET['response_type']) && $_GET['response_type'] == 'raw') {
    $vars['theme_hook_suggestions'][] = 'html__raw';
  }
  if (isset($_GET['response_type']) && $_GET['response_type'] == 'facebook') {
    $vars['theme_hook_suggestions'][] = 'html__fb';
  }
}

/**
 * Implements hook_preprocess_block().
 */
function bootstrap_bookcrossing_preprocess_block(&$vars) {
  $vars['title_attributes_array']['class'][] = 'block-title';
}

/*
function bootstrap_bookcrossing_fboauth_action__connect($variables) {
  $variables['action']['permissions'][] = 'publish_actions';
  $action = $variables['action'];
  $link = $variables['properties'];
  $link['query']['scope'] .= ',publish_actions';
  $url = url($link['href'], array('query' => $link['query']));
  //$link['attributes']['class'] = isset($link['attributes']['class']) ? $link['attributes']['class'] : 'facebook-action-connect';
  //$attributes = isset($link['attributes']) ? drupal_attributes($link['attributes']) : '';
  //$title = isset($link['title']) ? check_plain($link['title']) : '';
  //$prefix = t('Welcome! You can');
  //$suffix = t('using yout Facebook account.');
  //return $prefix . ' <a ' . $attributes . ' href="' . $url . '">' . t('log in') . '</a> ' . $suffix;
  return $url;
}
*/

/**
 * Implements hook_preprocess_image_style().
 */
function bootstrap_bookcrossing_preprocess_image_style(&$vars) {
    $vars['attributes']['class'][] = 'img-responsive'; // https://www.drupal.org/node/2169157
}


/**
 * Implements hook_theme().
 * 
 * In this case, overrides three user functions
 */
// function bootstrap_bookcrossing_theme() {
//   return array(
//     'user_login' => array(
//       'template' => 'page-no-login',
//       'arguments' => array('form' => NULL),
//     ),
//     'user_register' => array(
//       'template' => 'page-no-login',
//       'arguments' => array('form' => NULL),
//     ),
//     'user_pass' => array(
//       'template' => 'page-no-login',
//       'arguments' => array('form' => NULL),
//     ),
//   );
// }

function bootstrap_bookcrossing_preprocess_user_login(&$variables) {
  $variables['intro_text'] = t('Awesome login form');
  $variables['rendered'] = drupal_render($variables['form']);
}

function bootstrap_bookcrossing_preprocess_user_register(&$variables) {
  $variables['intro_text'] = t('Awesome registration form');
  $variables['rendered'] = drupal_render($variables['form']);
}

function bootstrap_bookcrossing_preprocess_user_pass(&$variables) {
  $variables['intro_text'] = t('Awesome password reset form');
  $variables['rendered'] = drupal_render($variables['form']);
}