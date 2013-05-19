<?php

/**
 * @file like.inc
 * 
 * Main (ctools) plugin file for "like" plugin type
 */

$plugin = array(
    'title' => t('Like'), 
    'description' => t('Facebook like button plugin'), 
    'html tag name' => 'like', 
    
    // hooks 
    'hook_nodeapi_view' => '_fb_social_like_nodeapi_view', 
    'hook_field_extra_fields' => '_fb_social_like_field_extra_fields', 
    'hook_link' => '_fb_social_like_link', 
    'hook_preprocess_fb_social_plugin' => '_fb_social_like_preprocess_fb_social_plugin' 
);

function like_defaults() {
  return array(
      'href' => '', 
      'send' => 0, 
      'layout' => 'button_count', 
      'show_faces' => 0, 
      'width' => 350, 
      'action' => 'like', 
      'font' => 'verdana', 
      'colorscheme' => 'dark' 
  );
}

function like_fb_settings($options) {
  
  $form = array();
  $form['send'] = array(
      '#type' => 'checkbox', 
      '#title' => t('Send button'), 
      '#description' => t('Include a Send button with the Like button') 
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
  
  $form['action'] = array(
      '#type' => 'select', 
      '#title' => t('Verb'), 
      '#description' => t('The verb to display in the button'), 
      '#options' => array(
          'like' => t('like'), 
          'recommend' => t('recommend') 
      ) 
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
  
  $defaults = like_defaults();
  
  foreach ( $form as $id => $f ) {
    $form[$id]['#default_value'] = isset($options[$id]) ? $options[$id] : $defaults[$id];
  }
  
  return $form;

}

function like_drupal_settings($options) {
  
  $form = array();
  $form['node_types'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Content types'), 
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE 
  );
  $form['node_types']['types'] = array(
      '#type' => 'checkboxes', 
      '#description' => t('Select types that will use the facebook like plugin'), 
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

function _fb_social_like_preprocess_fb_social_plugin(&$variables) {
  $options = &$variables['options'];
  $options['send'] = $options['send'] ? "true" : "false";
  $options['show_faces'] = $options['show_faces'] ? "true" : "false";
  $options['href'] = empty($options['href']) ? $url = fb_social_url($_GET['q']) : $options['href'];
}

/**
 * content_extra_fields callback for this plugin
 */
function _fb_social_like_field_extra_fields(&$extras, $preset) {
  
  $types = ($preset->settings['node_types']['types']);
  
  foreach ( $types as $type => $value ) {
    if (! empty($value) && $preset->settings['plugin_location']['location']) {
      $extras['node'][$type]['display']['fb_social_' . $preset->name] = array(
          'label' => t('Facebook like: ' . $preset->name), 
          'description' => t('The "like" plugin field from ' . $preset->name . ' preset'), 
          'weight' => 15 
      );
    
    }
  }
}

/**
 * nodeapi_view callback for this plugin
 */
function _fb_social_like_nodeapi_view($preset, &$node, $view_mode = 'full') {
  
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
function _fb_social_like_link($preset, $type, $object, $view_mode) {
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
    $links['fb-social-like-' . $preset->name] = array(
      'title' => $link_title, 
      'html' => TRUE 
    );
  }
  
  return $links;

}
