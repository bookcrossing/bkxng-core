<?php

/**
 * @file send.inc
 * 
 * Main (ctools) plugin file for "send" plugin type
 */

$plugin = array(
    'title' => t('send'), 
    'description' => t('Facebook send plugin'), 
    'html tag name' => 'send', 
    
    // hooks 
    'hook_preprocess_fb_social_plugin' => '_fb_social_send_preprocess_fb_social_plugin',
    'hook_nodeapi_view' => '_fb_social_send_nodeapi_view', 
    'hook_link' => '_fb_social_send_link', 
)
;

function send_defaults() {
  return array(
      'font' => '', 
      'colorscheme' => 'light' 
  );
}

function send_fb_settings($options) {
  
  $form = array();
  
  $form['font'] = array(
      '#type' => 'select', 
      '#title' => t('Font'), 
      '#description' => t('The font of the plugin'), 
      '#options' => array(
          'arial' => t('arial'), 
          'lucida grande' => t('lucida grande'), 
          'segoe ui' => t('segoe ui'), 
          'tahoma' => t('tahoma'), 
          'trebuchet ms' => t('trebuchet ms'), 
          'verdana' => t('verdana') 
      ) 
  );
  
  $form['colorscheme'] = array(
      '#type' => 'select', 
      '#title' => t('Color'), 
      '#description' => t('The color scheme of the plugin'), 
      '#options' => array(
          'dark' => t('dark'), 
          'light' => t('light') 
      ) 
  );
  
  $defaults = send_defaults();
  
  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  
  return $form;

}

function send_drupal_settings($options) {
  
  $form = array();
  $form['node_types'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Content types'), 
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE 
  );
  $form['node_types']['types'] = array(
      '#type' => 'checkboxes', 
      '#description' => t('Select types that will use the facebook send plugin'), 
      '#default_value' => isset($options['node_types']['types']) ? array_keys(array_filter($options['node_types']['types'])) : array(), 
      '#options' => node_type_get_names() 
  );
  
  $form['plugin_location'] = array(
      '#type' => 'fieldset', 
      '#title' => t('plugin location and display'), 
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE 
  );
  $form['plugin_location']['location'] = array(
      '#type' => 'radios', 
      '#title' => t('plugin location'), 
      '#default_value' => isset($options['plugin_location']['location']) ? $options['plugin_location']['location'] : 0, 
      '#options' => array(
          t('Node links'), 
          t('Node content') 
      ), 
      '#description' => t('The plugin can be printed in the "links" are of the node or as part of the node content') 
  );
  
  $form['plugin_location']['node_view_modes'] = array(
    '#type'=>'checkboxes',
      '#title' => t('View modes'),
      '#description' => t('Select view mode where it will be displayed.'),
      '#options' => _fb_social_get_node_view_modes(),
      '#default_value' => (isset($options['plugin_location']['node_view_modes']))? $options['plugin_location']['node_view_modes'] : array('full','teaser'),
  );
  
  return $form;

}

function _fb_social_send_preprocess_fb_social_plugin(&$variables) {
  $options = &$variables['options'];
  $options['href'] = empty($options['href']) ? $url = fb_social_url($_GET['q']) : $options['href'];
}

/**
 * nodeapi_view callback for this plugin
 */
function _fb_social_send_nodeapi_view($preset, &$node, $view_mode = 'full') {
  
  if ($node->status && fb_social_preset_node_types($preset, $node->type) && $preset->settings['plugin_location']['location']) {
    if (!empty($preset -> settings['plugin_location']['node_view_modes'][$view_mode] )) {
      $preset->fb_attrs['href'] = fb_social_url('node/' . $node->nid);
      $output = fb_social_preset_view($preset);
      $node->content['fb_social_' . $preset->name] = array(
          '#markup' => $output, 
          '#weight' => 15 
      );
    
    }
  }
}

/**
 * Pseudo hook_link for this plugin
 */
function _fb_social_send_link($preset, $type, $object, $view_mode) {
  $links = array();
  
  // no like for unpublished nodes
  if (! $object->status) {
    return $links;
  }
  
  // if no plugin on this particular view_mode, return
  if (empty($preset->settings['plugin_location']['node_view_modes'][$view_mode])) {
    return $links;
  }
  
  // plugin is not to be displayed in the node links. continue
  if ($preset->settings['plugin_location']['location']) {
    return $links;
  }
  
  // good to go
  if (fb_social_preset_node_types($preset, $object->type)) {
    $preset->fb_attrs['href'] = fb_social_url('node/' . $object->nid);
    $link_title = fb_social_preset_view($preset);
    $links['fb-social-send-' . $preset->name] = array(
      'title' => $link_title, 
      'html' => TRUE 
    );
  }
  
  return $links;

}
