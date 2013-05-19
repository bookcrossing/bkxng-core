<?php

/**
 * @file
 * Provide views handlers and plugins that allow display of .
 */

/**
 * Implements hook_views_data().
 */
function fb_social_views_data() {

  $data['node']['fb_social'] = array(
    'title' => t('Facebook Social Plugin'),
    'help' => t('Display a Facebook Social plugin.'),
    'group' => t('Node'),
    'field' => array(
      'help' => t('Display a Facebook Social plugin.'),
      'handler' => 'fb_social_handler_field',
      'real field' => 'nid',
    ),
  );
  $data['users']['fb_social'] = array(
    'title' => t('Facebook Social Plugin'),
    'help' => t('Display a Facebook Social plugin.'),
    'group' => t('User'),
    'field' => array(
      'help' => t('Display a Facebook Social plugin.'),
      'handler' => 'fb_social_handler_field',
      'real field' => 'uid',
    ),
  );
  return $data;
}
