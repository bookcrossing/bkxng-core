<?php

/**
 * @file
 * Header configuration.
 */

/**
 * Page for header configuration.
 */
function mobilizer_header_configuration() {
  global $base_url;
  $render_ary = drupal_get_form("mobilizer_header_configuration_form");
  $header_page = drupal_render($render_ary);
  return _mobilizer_themer($header_page, _mobilizer_iframe_with_html($base_url));
}

/**
 * Header configuration form. 
 */
function mobilizer_header_configuration_form($form, &$form_state) {
  global $base_url;
  drupal_add_library('system', 'ui.widget');
  drupal_add_library('system', 'ui.mouse');
  drupal_add_library('system', 'ui.slider');
  drupal_add_js(drupal_get_path('module', 'mobilizer') . '/js/header_config.js', 'file');
  $logo_path = variable_get('mobilizer_logo_path');
  $unserialize_header_config = variable_get('mobilizer_header_config', NULL);

  if ($unserialize_header_config) {
    $config = unserialize($unserialize_header_config);
  }
  else {
    $config = array();
  }
  $resize_value = isset($config['mobilizer_header_image_type']['resize']) ? $config['mobilizer_header_image_type']['resize'] : NULL;
  if ($resize_value) {
    $resize = str_replace('%', '', $resize_value);
  }
  else {
    $resize = 100;
  }

  drupal_add_js('
    jQuery(document).ready(function ($) { 
      $(function() {
        $("#slider").slider({
        value:' . $resize . ',
        min: 10,
        max: 100,
        animate: true,
        step: 1,
        slide: function( event, ui ) {
                $( "#amount" ).val(ui.value + "%");
                }
       });
       $( "#amount" ).val($( "#slider" ).slider( "value" ) + "%" );
      });
    });', 'inline');

  $header_options = array(
    'header_text_type' => 'Text Header',
    'header_image_type' => 'Image Header',
  );
  $form['header_type'] = array(
    '#type' => 'select',
    '#options' => $header_options,
    '#title' => t('Header Type'),
    '#default_value' => isset($config['header_type']) ? $config['header_type'] : 'header_image_type',
  );
  $form['title'] = array(
    '#type' => 'text_format',
    '#title' => t('Title'),
    '#default_value' => isset($config['header_text_type']['title']) ? $config['header_text_type']['title'] : variable_get('site_name'),
  );

  $form['header_image_title'] = array(
    '#markup' => t('Header Image'),
    '#weight' => 1,
  );

  $form['header_image'] = array(
    '#type' => 'managed_file',
    '#size' => 10,
    '#default_value' => isset($config['header_image_type']['header_image']) ? $config['header_image_type']['header_image'] : NULL,
    '#upload_location' => 'public://mobilizer_header_images/',
    '#theme' => 'preview_image_thumbnail',
    '#weight' => 2,
  );

  if (variable_get('mobilizer_default_logo', 1)) {
    $form['default_logo'] = array(
      '#markup' => '<img height="100" width="100" src="' . $logo_path . '">',
      '#weight' => 1,
    );
  }

  $form['resize_title'] = array(
    '#markup' => 'Resize',
    '#weight' => 1,
  );
  $form['resize'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#id' => 'amount',
    '#size' => 5,
    '#default_value' => isset($config['header_image_type']['resize']) ? $config['header_image_type']['resize'] : '100%',
  );

  $form['slider'] = array(
    '#markup' => '<div id="slider"></div>',
    '#weight' => 1,
  );

  $form['align_title'] = array(
    '#markup' => 'Align',
    '#weight' => 1,
  );
  $active = array('left' => t('Left'), 'center' => t('Center'), 'right' => t('Right'));
  $form['align'] = array(
    '#type' => 'radios',
    '#default_value' => isset($config['header_image_type']['align']) ? $config['header_image_type']['align'] : 'center',
    '#options' => $active,
  );

  $form['hbimage'] = array(
    '#title' => t('Header background Image'),
    '#type' => 'managed_file',
    '#size' => 10,
    '#upload_location' => 'public://mobilizer_header_images/',
    '#default_value' => isset($config['header_common_fields']['hbimage']) ? $config['header_common_fields']['hbimage'] : NULL,
    '#theme' => 'preview_image_thumbnail',
    '#weight' => 2,
  );

  $css_creation = variable_get("mobilizer_css_creation");
  $color_style_name = isset($config['header_common_fields']['hbcolor']) ? $config['header_common_fields']['hbcolor'] : "#ffffff";
  $mobilizer_style = variable_get("mobilizer_style");

  $color_style_name = isset($css_creation['#header']['background-color']) ? $css_creation['#header']['background-color'] : $mobilizer_style['colorpalatte']['header_background'];
  $form['hbcolor'] = array(
    '#title' => check_plain(t('Header background Color')),
    '#default_value' => $color_style_name,
    '#type' => 'textfield',
    '#size' => 7,
    '#maxlength' => 7,
    '#weight' => 1,
    '#element_validate' => array('mobilizer_header_validate_hex_color'),
    '#prefix' => '<div class="palette-colorpicker-wrapper">',
    '#suffix' => '<div class="palette-colorpicker"></div></div>',
    '#attributes' => array('class' => array('edit-palette-colorpicker'), 'style' => 'background-color:' . $color_style_name),
    '#attached' => array('library' => array(array('system', 'farbtastic'), ),
      // Add javascript to trigger the colorpicker.
      'js' => array(drupal_get_path('module', 'mobilizer') . '/js/mobilizer-colorpicker.js'),
    ),
  );
  $form['proceed'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#weight' => 10,
    '#attributes' => array('class' => array('btn btn-primary')),
  );
  $form['preview'] = array(
    '#type' => 'button',
    '#value' => t('Preview'),
    '#weight' => 10,
    '#attributes' => array('class' => array('btn btn-primary')),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'mobilizer_ajax_callback',
      'wrapper' => 'iframe_div',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['#theme'] = 'mobilizer_header_theme_layout_form';
  return $form;
}

function mobilizer_header_configuration_form_submit($form, &$form_state) {
  $header_type = $form_state['values']['header_type'];
  variable_set('mobilizer_header_preview', 0);
  variable_set('mobilizer_header_type', $header_type);
  $header_text_type = array(
    'title' => $form_state['values']['title']['value'],
  );
  $header_image_type = array(
    'header_image' => $form_state['values']['header_image'],
    'resize' => $form_state['values']['resize'],
    'align' => $form_state['values']['align'],
  );
  $header_common_fields = array(
    'hbcolor' => $form_state['values']['hbcolor'],
    'hbimage' => $form_state['values']['hbimage'],
  );
  $header_config = array(
    'header_type' => $header_type,
    'header_text_type' => $header_text_type,
    'header_image_type' => $header_image_type,
    'header_common_fields' => $header_common_fields,
  );

  if (isset($form_state['values']['hbcolor'])) {
    $mobilizer_style = variable_get("mobilizer_style");
    $color_palette_arr = mobilizer_color_palette_style();
    $color_label_name = str_replace(" ", "_", strtolower($color_palette_arr[0]));
    $mobilizer_style['colorpalatte'][$color_label_name] = $form_state['values']['hbcolor'];
    variable_set("mobilizer_style", $mobilizer_style);
  }

  if ($form_state['values']['header_image']) {
    $fileobj = file_load($form_state['values']['header_image']);
    $imgpath = isset($fileobj->uri) ? $fileobj->uri : NULL;
  }
  else {
    $imgpath = '';
  }
  if ($imgpath) {
    $logo = file_create_url($imgpath);
    variable_set('mobilizer_logo_path', $logo);
    variable_set('mobilizer_default_logo', 0);
  }
  else {
    $web_theme_path = variable_get('mobilizer_logo_path');
    variable_set('mobilizer_logo_path', $web_theme_path);
  }

  if ($form_state['values']['title']['value']) {
    variable_set('mobilizer_text_header', $form_state['values']['title']['value']);
  }
  else {
    variable_set('mobilizer_text_header', variable_get('site_name'));
  }

  $css_creation = variable_get('mobilizer_css_creation');
  if (!is_array($css_creation)) {
    $css_creation = array();
  }
  if ($form_state['values']['hbcolor']) {
    $css_creation['#header']['background-color'] = $form_state['values']['hbcolor'];
  }
  if ($form_state['values']['hbimage']) {
    $bgimage = file_create_url(file_load($form_state['values']['hbimage'])->uri);
    $css_creation['#header']['background-image'] = "url('" . $bgimage . "')";
    $css_creation['#header']['background-repeat'] = "repeat";
  }
  if ($form_state['values']['align']) {
    if ($form_state['values']['align'] == 'left') {
      $css_creation['#logo']['padding'] = '5px 60% 0 5px';
    }
    elseif ($form_state['values']['align'] == 'right') {
      $css_creation['#logo']['padding'] = '5px 10% 0 50%';
    }
    elseif ($form_state['values']['align'] == 'center') {
      $css_creation['#logo']['padding'] = '5px 30% 0';
    }
    else {
      $css_creation['#logo']['padding'] = '5px 30% 0';
    }
    $css_creation['#logo']['text-align'] = $form_state['values']['align'];
  }

  if ($form_state['values']['resize']) {
    $css_creation['#logo a img']['width'] = $form_state['values']['resize'];
    $css_creation['#logo a img']['height'] = 'auto';
  }

  variable_set("mobilizer_css_creation", $css_creation);
  mobilizer_css_file_generator($css_creation);
  $serialize_header_config = serialize($header_config);
  variable_set('mobilizer_header_config', $serialize_header_config);
  variable_set('mobilizer_header_process', 1);
  $mobilizer_message['message'] = '<div class="mob_success">Save Header configuration</div>';
  variable_set('mobilizer_message', $mobilizer_message);
  if ($form_state['values']['op'] == 'Proceed') {
    drupal_goto('admin/mobilizer/finish-page');
  }
}

/**
 * Returns Image in with thumbnail preview.
 */
function theme_image_multifield_multitype($variables) {
  $output = '';
  $base = drupal_render_children($element); // renders element as usual
  if ($element['fid']['#value'] != 0) {
    $output .= '<div class="multifield-thumbnail">';
    $output .= theme('image_style', array('style_name' => 'thumbnail', 'path' => file_load($element['fid']['#value'])->uri, 'getsize' => FALSE));
    $output .= '</div>';
  }
  return $output;
}

/**
 * Check to make sure the user has entered a valid 6 digit hex color.
 */
function mobilizer_header_validate_hex_color($element, &$form_state) {
  if (!$element['#required'] && empty($element['#value'])) {
    return;
  }
  if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $element['#value'])) {
    form_error($element, t("'@color' is not a valid hex color", array('@color' => $element['#value'])));
  }
  else {
    form_set_value($element, $element['#value'], $form_state);
  }
}
