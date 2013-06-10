<?php

/**
 * @file
 * The theme system, which controls the output of mobile device.
 *
 */
function mobilizer_preprocess_html(&$vars) {
  //Get the menu navigation style and set menu css accordingly
  $nav_style = variable_get('mobilizer_nav_style', 'menu-horizontal');

  if ($vars['is_front']) {

    if ($nav_style == 'menu-horizontal') {
      drupal_add_css(path_to_theme() . '/css/menu/mobilizer-menuOne.css');
    }
    elseif ($nav_style == 'menu-box') {
      drupal_add_css(path_to_theme() . '/css/menu/mobilizer-menuTwo.css');
    }
    else {
      drupal_add_css(path_to_theme() . '/css/menu/mobilizer-menuThree.css');
    }
  }
  else {
    drupal_add_css(path_to_theme() . '/css/menu/mobilizer-menuThree.css');
  }
}

/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function mobilizer_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width'
    )
  );
  $head_elements['viewport'] = $viewport;
}

function mobilizer_link($variables) {
  return '<a href="' . check_plain(url($variables['path'], $variables['options'])) . '"' . drupal_attributes($variables['options']['attributes']) . '><span>' . ($variables['options']['html'] ? $variables['text'] : check_plain($variables['text'])) . '</span></a>';
}

/**
 * Insert themed breadcrumb page navigation at top of the node content.
 */
function mobilizer_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // comment below line to hide current page to breadcrumb
    $breadcrumb[] = drupal_get_title();
    $output .= '<nav class="breadcrumb">' . implode(' » ', $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Override or insert variables into the page template.
 */
function mobilizer_preprocess_page(&$vars) {
  $vars['theme_path'] = path_to_theme();

// HEADER connfig setting
  $vars['logo'] = variable_get('mobilizer_logo_path');
  if (variable_get('mobilizer_header_preview')) { // for only header preview setting
    variable_set('mobilizer_header_preview', 0);
    if (variable_get('mobilizer_header_type_preview') == 'header_image_type') {
      $vars['logo'] = variable_get('mobilizer_logo_path_preview');
    }
    else {
      $vars['logo'] = NULL;
    }

    if (variable_get('mobilizer_header_type_preview') == 'header_text_type') {
      $vars['site_name'] = variable_get('mobilizer_text_header_preview');
    }
    else {
      $vars['site_name'] = $site_name; //NULL;
    }
  }
  else {
// After Proceed and update changes 
    if (variable_get('mobilizer_header_type') == 'header_image_type') {
      $vars['logo'] = variable_get('mobilizer_logo_path');
    }
    else {
      $vars['logo'] = NULL;
    }
    if (variable_get('mobilizer_header_type') == 'header_text_type') {
      $vars['site_name'] = variable_get('mobilizer_text_header');
    }
    else {
      $vars['site_name'] = $site_name; //NULL;
    }
  }
// END of header config

  if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
      ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
      ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }

  // render mobilizer menu item
  $vars['mobilizer_menu_custom'] = _mobilizer_menu();

  $vars['mobilizer_menu_custom_drop'] = _mobilizer_menu_drop();
}

function _mobilizer_menu() {
  global $base_url;

  $main_menu_tree = menu_tree_page_data('menu-website-mobilizer', $max_depth = 1, TRUE); //menu_tree(variable_get('menu_main_links_source', 'menu-website-mobilizer'));
  // dpr($main_menu_tree);
  $menu_list = '<ul class="menu">';
  $i = 1;
  $menu_count = 0;
  foreach ($main_menu_tree as $key => $value) {
    if (!$value['link']['hidden']) {
      $menu_count++;
      $menu_image = mobilizer_get_menu_icons_menu_id($value['link']['mlid'], $value['link']['link_title']);
      if ($i <= 5) {
        $menu_list .= '<li>' . l(t('<div class="ico">' . $menu_image . '
      </div><span>' . $value['link']['link_title'] . '</span> <span class="arrow"><img src="' . $base_url . '/' . path_to_theme('mobilizer') . '/images/arrow.png" />'), $value['link']['link_path'], array('html' => TRUE, 'attributes' => array('title' => $value['link']['link_title']))) . '</span><div class="clear"></div></li>';
      }
      else {
        $menu_list .= '<li class="hide">' . l(t('<div class="ico">' . $menu_image . '
      </div><span>' . $value['link']['link_title'] . '</span> <span class="arrow"><img src="' . $base_url . '/' . path_to_theme('mobilizer') . '/images/arrow.png" />'), $value['link']['link_path'], array('html' => TRUE, 'attributes' => array('title' => $value['link']['link_title']))) . '</span><div class="clear"></div></li>';
      }
      $i++;
    }
  }
  if ($menu_count > 5) {
    $menu_list .= '<li class="more-link">' . '<div class="ico"><img src="' . $base_url . '/' . path_to_theme('mobilizer') . '/images/more-link.png" /></div><a href="javascript: void(0);"><span class="label">More Links</span> <span class="arrow"><img src="' . $base_url . '/' . path_to_theme('mobilizer') . '/images/arrow.png" /></span></a><div class="clear"></div></li>';
  }
  $menu_list .= '</ul>';
  return $menu_list;
}

function _mobilizer_menu_drop() {
  global $base_url;

  $main_menu_tree = $parrent_menu_items = menu_tree_page_data('menu-website-mobilizer', $max_depth = 1, $only_active_trail = FALSE); //menu_tree(variable_get('menu_main_links_source', 'menu-website-mobilizer'));
  $menu_list = '<ul class="menu">';
  $menu_count = count($main_menu_tree);
  foreach ($main_menu_tree as $key => $value) {
    if (!$value['link']['hidden']) {
      $menu_image = mobilizer_get_menu_icons_menu_id($value['link']['mlid'], $value['link']['link_title']);
      $menu_list .= '<li>' . l(t('<div class="ico">' . $menu_image . '
      </div><span>' . $value['link']['link_title'] . '</span> <span class="arrow"><img src="' . $base_url . '/' . path_to_theme('mobilizer') . '/images/arrow.png" />'), $value['link']['link_path'], array('html' => TRUE, 'attributes' => array('title' => $value['link']['link_title']))) . '</span><div class="clear"></div></li>';
    }
  }
  $menu_list .= '</ul>';
  return $menu_list;
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function mobilizer_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

/**
 * Override or insert variables into the node template.
 */
function mobilizer_preprocess_node(&$variables) {
  $node = $variables['node'];
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/*
function mobilizer_page_alter(&$page) {
	unset($page['page_top']['toolbar']);
  // <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1"/>
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
    'name' =>  'viewport',
    'content' =>  'width=device-width'
    )
  );
  drupal_add_html_head($viewport, 'viewport');
}
*/
/**
 * Add javascript files for front-page jquery slideshow.
 */
//if (drupal_is_front_page()) {
//  drupal_add_js(drupal_get_path('theme', 'mobilizer') . '/js/jquery.flexslider-min.js');
//  drupal_add_js(drupal_get_path('theme', 'mobilizer') . '/js/slide.js');
//}
