<?php

/**
 * Implements hook_preprocess_page().
 */
function bootstrap_bookcrossing_preprocess_page(&$vars) {
  $image = theme('image', array('path' => path_to_theme() . '/images/ira-yohng.png', 'alt' => t('Ira Yohng')));
  $vars['ira_yohng'] = l($image, 'http://ira.yohng.com/', array('html' => TRUE, 'attributes' => array('target' => '_blank')));
}

/**
 * Implements hook_preprocess_block().
 */
function bootstrap_bookcrossing_preprocess_block(&$vars) {
  $vars['title_attributes_array']['class'][] = 'block-title';
}

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