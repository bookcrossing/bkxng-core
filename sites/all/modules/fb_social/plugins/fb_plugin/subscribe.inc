<?php

/**
 * @file subscribe.inc
 * 
 * Main (ctools) plugin file for "subscribe" plugin type
 */

$plugin = array(
    'title' => t('Subscribe'), 
    'description' => t('Facebook subscribe plugin'), 
    'html tag name' => 'subscribe', 
    
    // hooks 
    'hook_nodeapi_view' => '_fb_social_subscribe_nodeapi_view', 
    'hook_field_extra_fields' => '_fb_social_subscribe_field_extra_fields', 
    'hook_link' => '_fb_social_subscribe_link', 
    'hook_preprocess_fb_social_plugin' => '_fb_social_subscribe_preprocess_fb_social_plugin' 
);

function subscribe_defaults() {
  return array(
      'href' => '', 
      'layout' => 'standard', 
      'show_faces' => 1, 
      'width' => 450, 
      'font' => 'verdana', 
      'colorscheme' => 'light' 
  );
}

function subscribe_fb_settings($options) {
  
  $form = array();
  
  $form['href'] = array(
      '#type' => 'textfield', 
      '#title' => t('Profile URL'), 
      '#description' => t('Profile URL of the user to subscribe to'),
  );
  
  $form['layout'] = array(
      '#type' => 'select', 
      '#title' => t('Layout style'), 
      '#description' => t('Determines the size and the amount of the social context next to the button'), 
      '#options' => array(
          'standard' => t('standard'), 
          'button_count' => t('button_count'), 
          'box_count' => t('box_count') 
      ) 
  );
  
  $form['show_faces'] = array(
      '#type' => 'checkbox', 
      '#title' => t('Show faces'), 
      '#description' => t('Show profiles pictures below the button') 
  );
  
  $form['width'] = array(
      '#type' => 'textfield', 
      '#title' => t('Width'), 
      '#description' => t('The width of the plugin in pixel') 
  );
    
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
  
  $defaults = subscribe_defaults();
  
  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  
  return $form;

}

function subscribe_drupal_settings($options) {
  
  $form = array();
  $form['node_types'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Content types'), 
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE 
  );
  $form['node_types']['types'] = array(
      '#type' => 'checkboxes', 
      '#description' => t('Select types that will use the facebook subscribe plugin'), 
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
      '#default_value' => (isset($options['plugin_location']['node_view_modes']))? $options['plugin_location']['node_view_modes'] : array('full'),
  );
  return $form;

}

function _fb_social_subscribe_preprocess_fb_social_plugin(&$variables) {
  $options = &$variables['options'];
  $options['show_faces'] = $options['show_faces'] ? "true" : "false";
}

/**
 * content_extra_fields callback for this plugin
 */
function _fb_social_subscribe_field_extra_fields(&$extras, $preset) {
  
  $types = ($preset->settings['node_types']['types']);
  
  foreach ( $types as $type => $value ) {
    if (! empty($value) && $preset->settings['plugin_location']['location']) {
      $extras['node'][$type]['display']['fb_social_' . $preset->name] = array(
          'label' => t('Facebook subscribe: ' . $preset->name), 
          'description' => t('The "subscribe" plugin field from ' . $preset->name . ' preset'), 
          'weight' => 15 
      );
    
    }
  }
}

/**
 * nodeapi_view callback for this plugin
 */
function _fb_social_subscribe_nodeapi_view($preset, &$node, $view_mode = 'full') {
  
  if ($node->status && fb_social_preset_node_types($preset, $node->type) && $preset->settings['plugin_location']['location']) {
    if (!empty($preset -> settings['plugin_location']['node_view_modes'][$view_mode] )) {
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
function _fb_social_subscribe_link($preset, $type, $object, $view_mode) {
  $links = array();
  
  // no subscribe for unpublished nodes
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
    $link_title = fb_social_preset_view($preset);
    $links['fb-social-subscribe-' . $preset->name] = array(
      'title' => $link_title, 
      'html' => TRUE 
    );
  }
  
  return $links;

}
