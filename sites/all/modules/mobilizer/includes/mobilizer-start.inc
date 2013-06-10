<?php

/**
 * @file
 * Admin page callbacks for the Mobilizer module.
 *
 * @ingroup Mobilizer
 */
/**
 * Batch processing callback form; Generate new menu item from existing menu like Main Menu, copying mobilizer theme into themes directory. Copying css, js, images form existing theme to new mobilizer theme.
 */
function mobilizer_start_page() {
  // Render form
  $render_ary = drupal_get_form('mobilizer_start_form');
  $output = render($render_ary);
  // Message in preview frame
  $dummy_content = t('<div class="dummy-mobilizer-Text">Your mobile site preview will appear here...!</div>');
  // render complete page including form & other blocks on page
  return _mobilizer_themer($output, $dummy_content);
}
/**
 * Form builder; Bulk processing for Mobilizer theme generation.
 *
 * @ingroup forms
 */
function mobilizer_start_form($form, &$form_state) {
  global $base_url;
  $path = DRUPAL_ROOT . '/sites/all/themes';
  // Check theme directory permission that it is writable or not.
  // If not writable then show an alert message
  if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY)) {
    $form['html'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="file-permission" style="text-align:center;margin-top:90px;">' . t('Current theme directory "sites/all/themes" is not writable. Please provide write permission to your theme directory.') . '</div>',
      '#tree' => TRUE,
    );
    // Enter message into watchdog table
    watchdog('file', 'The file permissions could not be set on %uri.', array('%uri' => $path), WATCHDOG_ERROR);
  }
  else {
    $progress_img_path = $base_url . '/' . drupal_get_path('module', 'mobilizer') . '/images/progress_bar_new.gif';
    $form['html'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="progress-bar" style="text-align:center;display:none;margin-top:90px;"><img src="' . $progress_img_path . '" /></div>',
      '#tree' => TRUE,
    );
    $form['html_top'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="content-area-top">
                    <h2 class="content-top-heading">mobilize</h2>
                    <p class="content-top-text">Make your websites available on mobile in minutes.<br/>  
                    No coding skills required!!! </p>
                    </div>',
      '#tree' => TRUE,
    );
    drupal_add_js('jQuery(document).ready(function () { jQuery(".form-submit").click(function () { jQuery(".progress-bar").css("display", "block"); jQuery(".form-submit").css("display", "none"); }); });', 'inline');
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Start Mobilizer'),
      '#attributes' => array('class' => array('btn btn-primary')),
      '#prefix' => '<div class="loader_confi">',
      '#suffix' => '</div>',
    );
    $form['html_bottom'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="content-area-bottom">
                  <h4 class="content-bottom-heading">It\'s Amazing, Quick and Easy!</h4>
                  <div class="col-1">
                  <h3 class="bottom-inner-title">Build</h3>
                  <div class="bottom-inner-img">
                  </div>
                  <p class="list-style">Generate your desktop site for the mobile web</p>
                  <p class="list-style">Publish or further customize your site</p>
                  </div>
                  <span class="inner-arrow"></span>
                  <div class="col-2">
                  <h3 class="bottom-inner-title">customize</h3>
                  <div class="bottom-inner-img">
                  </div>
                  <p class="list-style">Easily edit your site’s copy, images, backgrounds, and layout and more.</p>
                  </div>
                  <span class="inner-arrow"></span>
                  <div class="col-3">
                  <h3 class="bottom-inner-title">publish</h3>
                  <div class="bottom-inner-img"></div>
                  <p class="list-style">Preview your site just as your mobile customers will see it and publish it.</p>
                  </div>
                  </div>',
      '#tree' => TRUE,
    );   
  }
  return $form;
}

/**
 * Form submission handler for bulk processing
 * @mobilizer_start_form_submit() 
 */
function mobilizer_start_form_submit($form, &$form_state) {
  // Set mobile theme name.
  $mobile_theme_name = variable_get('mobilizer_theme_name', 'mobilizer');

  // set mobile theme menu name
  variable_set('mobilizer_menu', 'menu-website-mobilizer');
  
  // set default navingation menu style
  variable_set('mobilizer_nav_style', 'menu-horizontal');
  
  // set default color pallate scheme for mobile site
  mobilizer_color_palette_style_config();

  //Move mobile theme to sites/all/themes folder; requires write permission to sites/all/themes folder
  mobilizer_copy_theme_in_theme_folder($mobile_theme_name);
  
  // Clear all cache
  $GLOBALS['conf']['cache'] = 0;
  drupal_flush_all_caches();
  cache_clear_all(NULL, 'cache_page', '*');
  
  // Copying web theme css into mobile theme css
  mobilizer_copy_web_css_to_mobile_css($mobile_theme_name);
  
  // start copying web theme menu css to mobile theme css for some specified tags
  mobilizer_copy_web_menu_css_to_mobile_css($mobile_theme_name);
  
  // Copying web theme js into mobile theme js 
  mobilizer_copy_web_theme_js($mobile_theme_name);
  
  // Copy web theme images (folder name should be images) into mobile theme images 
  mobilizer_copy_web_theme_images($mobile_theme_name);
  
  // Save web theme logo path
  mobilizer_save_logo_path();
  
  //Create default menu for Mobile theme
  mobilizer_create_default_menu();
  
  //Enable Mobile theme.
  $theme_list = array($mobile_theme_name);
  theme_enable($theme_list);
  
  // set default header css setting
  $css_creation['#logo a img']['width'] = '100%';
  $css_creation['#logo a img']['height'] = 'auto';
  $css_creation['#logo']['padding'] = '5px 30% 0';
  variable_set("mobilizer_css_creation", $css_creation);
  
  // Generate header style css with some default tags 
  mobilizer_css_file_generator($css_creation);
  
  // Set some default variables for header image selection types
  variable_set('mobilizer_header_type_preview', 'header_image_type');
  variable_set('mobilizer_header_type', 'header_image_type');
  
  //Clear all cache
  $GLOBALS['conf']['cache'] = 0;
  drupal_flush_all_caches();
  cache_clear_all(NULL, 'cache_page', '*');
  
  // Assign mobile generated content in mobile theme region
  mobilizer_assign_mobile_content_in_mobilizer_theme_region('mobilizer');
  
  // Set a flag for future perspect
  variable_set('mobilizer_start_process', 1);
  $form_state['redirect'] = 'admin/mobilizer/select-layout';
}

/**
 * Callback function for writing mobile theme css by parsing
 * all existing css file from web theme
 * only some specified tags will be parsed
 * css parsing is done by using css parser library
 * @param $mtheme - mobile theme name
 */
function mobilizer_copy_web_css_to_mobile_css($mtheme_name) {
  //set default page header style
  $mobilizer_style['text_links']['text'] = '#737373';
  $mobilizer_style['text_links']['link'] = '#1d8dbb';
  $mobilizer_style['text_links']['underline'] = 1;
  $mobilizer_style['text_links']['font_type'] = 'Arial';
  $mobilizer_style['text_links']['font_size'] = '12';
  $mobilizer_style['headers']['heading1'] = '#333333';
  $mobilizer_style['headers']['heading2'] = '#656f95';
  $mobilizer_style['headers']['heading3'] = '#103dea';
  $mobilizer_style['colorpalatte']['palette_id'] = 'color_palette1';
  $mobilizer_style['colorpalatte']['header_background'] = '#ffffff';
  $mobilizer_style['colorpalatte']['text_color'] = '#737373';
  $mobilizer_style['colorpalatte']['navigation'] =  '#1e7097';
  $mobilizer_style['colorpalatte']['link'] = '#1d8dbb';
  $mobilizer_style['colorpalatte']['site_background'] = '#ffffff';
  $mobilizer_style['colorpalatte']['navigation_label'] = '#ffffff';
  //Get list of all available css files.
  $get_css_files = mobilizer_get_theme_css_file();
  if (is_array($get_css_files)) {
    $module_path = drupal_get_path('module', 'mobilizer');
    $theme_path = drupal_get_path('theme', 'mobilizer');
    require_once DRUPAL_ROOT . '/' . $module_path . '/cssparser/cssparser.php';
    $css = new cssparser();
    // Getting list of some specified tags as an array which
    // needs to be parsed from web theme css
    $css_tags = _mobilizer_predefind_css_tags();
    // Base css files for mobile theme
    $mobile_theme_css_path = DRUPAL_ROOT . '/' . $theme_path . '/css/mobilizer-base-mobile.css';
    $f = fopen($mobile_theme_css_path, 'w');
    // Iterate & parse each css files from web theme
    foreach ($get_css_files as $key => $val) {
      $filename = DRUPAL_ROOT . '/' . $val;
      $css->Parse($filename);
      // Iterate predefined css tags and look if it is available in web theme css
      foreach ($css_tags as $tagname) {
        if (isset($css->css[$tagname])) {
          $css_arr = $css->css[$tagname];
          // get final css writing string for this exiting tag
          $test_css = _mobilizer_get_css_string($tagname, $css_arr);
          fwrite($f, $test_css);
        }
      }
      // Set some default variables for some specified tags which are used in color pallete settings
      // Check if clor scheme is not in hexadecimal then get hexadecimal color code
      if (isset($css->css['p']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['p']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['p']['color']);
          if ($hex_code) {
            $mobilizer_style['text_links']['text'] = $hex_code;
            $mobilizer_style['colorpalatte']['text_color'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['text_links']['text'] = $css->css['p']['color'];
          $mobilizer_style['colorpalatte']['text_color'] = $css->css['p']['color'];
        }
      }
      if (isset($css->css['a']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['a']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['a']['color']);
          if ($hex_code) {
            $mobilizer_style['text_links']['text'] = $hex_code;
            $mobilizer_style['colorpalatte']['text_color'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['text_links']['link'] = $css->css['a']['color'];
          $mobilizer_style['colorpalatte']['link'] = $css->css['a']['color'];
        }
      }
      
      if (isset($css->css['a']['text-decoration']) && ($css->css['a']['text-decoration'] == 'underline')) {
        $mobilizer_style['text_links']['underline'] = 1;
      }
      
      if (isset($css->css['h1']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['h1']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['h1']['color']);
          if ($hex_code) {
            $mobilizer_style['headers']['heading1'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['headers']['heading1'] = $css->css['h1']['color'];
        }
      }
      
      if (isset($css->css['h2']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['h2']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['h2']['color']);
          if ($hex_code) {
            $mobilizer_style['headers']['heading2'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['headers']['heading2'] = $css->css['h1']['color'];
        }
      }
      
      if (isset($css->css['h3']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['h3']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['h3']['color']);
          if ($hex_code) {
            $mobilizer_style['headers']['heading3'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['headers']['heading1'] = $css->css['h1']['color'];
        }
      }
      
     
      if (isset($css->css['body']['font-type'])) {
        $mobilizer_style['text_links']['font_type'] = $css->css['body']['font-type'];
      }
      
      if (isset($css->css['body']['font-size'])) {
        $mobilizer_style['text_links']['font_size'] = $css->css['body']['font-size'];
      }
      
      if (isset($css->css['header']['background-color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['header']['background-color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['header']['background-color']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['header_background'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['header_background'] = $css->css['header']['background-color'];
        }
      }
      
      if (isset($css->css['body']['background-color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['body']['background-color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['body']['background-color']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['site_background'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['site_background'] = $css->css['body']['background-color'];
        }
      }

      
      // check for primary links color codes in css
      if (isset($css->css['ul.primary li a']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['ul.primary li a']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['ul.primary li a']['color']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['navigation_label'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['navigation_label'] = $css->css['ul.primary li a']['color'];
        }
      }
      
      if (isset($css->css['ul.primary li']['background'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['ul.primary li']['background'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['ul.primary li']['background']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['navigation'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['navigation'] = $css->css['ul.primary li']['background'];
        }
      }

      // check for navigation links
      if (isset($css->css['ul.main-menu li a']['color'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['ul.main-menu li a']['color'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['ul.main-menu li a']['color']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['navigation_label'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['navigation_label'] = $css->css['ul.main-menu li a']['color'];
        }
      }
      
      if (isset($css->css['ul.main-menu li']['background'])) {
        if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $css->css['ul.main-menu li']['background'])) {
          $hex_code = _mobilizer_color_name_to_hex($css->css['ul.main-menu li']['background']);
          if ($hex_code) {
            $mobilizer_style['colorpalatte']['navigation'] = $hex_code;
          }
        }
        else {
          $mobilizer_style['colorpalatte']['navigation'] = $css->css['ul.main-menu li']['background'];
        }
      }
    }
    variable_set("mobilizer_style", $mobilizer_style);
    fclose($f);
  }
  // mobilizer previews css should be blank incase if start mobilizer will run again
  // Initialize mobile preview css
  $preview_mobile_theme_css_path = DRUPAL_ROOT . '/' . drupal_get_path('theme', 'mobilizer') . '/css/mobilizer-page-header-style-preview.css';
  if (file_exists($preview_mobile_theme_css_path)) {
    $pf = fopen($preview_mobile_theme_css_path, 'w') or die("can't open file");
    $stringdata = "";
    fwrite($pf, $stringdata);
    fclose($pf);
  } 
  // end here 
}

/**
 * Callback function for writing mobile theme css by parsing
 * all existing css file from web theme
 * only some specified menu tags will be parsed
 * css parsing is done by using css parser library
 * @param $mtheme - mobile theme name
 */
function mobilizer_copy_web_menu_css_to_mobile_css($mtheme_name) {
  $get_css_files = mobilizer_get_theme_css_file();
  if (is_array ($get_css_files)) {
    $module_path = drupal_get_path('module', 'mobilizer');
    $theme_path = drupal_get_path('theme', 'mobilizer');
    require_once DRUPAL_ROOT . '/' . $module_path . '/cssparser/cssparser.php';
    $css = new cssparser();
    $css_tags = _mobilizer_predefind_menu_css_tags();
    $mobile_theme_css_path = DRUPAL_ROOT . '/' . $theme_path . '/css/mobilizer-color-mobile.css';
    $f = fopen($mobile_theme_css_path, 'w');
    foreach ($get_css_files as $key => $val) {
      $filename = DRUPAL_ROOT . '/' . $val;
      $css->Parse($filename);
      foreach ($css_tags as $tagname) {
        if (isset($css->css[$tagname])) {
          $css_arr = $css->css[$tagname];
            $test_css = _mobilizer_get_css_string($tagname, $css_arr);
            fwrite($f, $test_css);
        }
      }
    }
    fclose($f);
  }
}

/**
 * Callback function for copying images from current web theme to mobile theme
 * @param $mtheme - mobile theme name
 */
function mobilizer_copy_web_theme_images($mtheme_name) {
  $active_theme = variable_get('theme_default');
  $theme_path = drupal_get_path('theme', $active_theme);
  $src_image_dir = DRUPAL_ROOT . '/' . $theme_path . '/images/';
  if (is_dir ($src_image_dir)) {
    $module_path = drupal_get_path('module', 'mobilizer');
    $dest_theme_path = drupal_get_path('theme', 'mobilizer');
    $dest_image_dir = DRUPAL_ROOT . '/' . $dest_theme_path . '/images/';
    $srcdir = opendir($src_image_dir);
    while ($readfile = readdir($srcdir)) {
      if ($readfile != '.' && $readfile != '..') {
        if (!file_exists($dest_image_dir . $readfile)) {
          if (@copy($src_image_dir . $readfile, $dest_image_dir . $readfile)) {
            //echo "Copy file";
          }
          else {
            //echo "Canot Copy file";
          }
        }
      }
    }
    closedir($srcdir);
  }
}

/**
 * Callback function for saving logo path of current web theme
 * @param $mtheme - mobile theme name
 */
function mobilizer_save_logo_path() {
  $active_theme = variable_get('theme_default');
  $web_theme_path = theme_get_setting('logo', $active_theme);
  variable_set('mobilizer_logo_path', $web_theme_path);
  variable_set('mobilizer_header_preview', 0);
  variable_set('mobilizer_header_type', 0);
}

/**
 * Callback function for copying JS file from current web theme to mobile theme
 * @param $mtheme - mobile theme name
 */
function mobilizer_copy_web_theme_js($mtheme_name) {
  $get_js_files =  mobilizer_get_theme_js_file();
  if (is_array($get_js_files)) {
    $module_path = drupal_get_path('module', 'mobilizer');
    $theme_path = drupal_get_path('theme', 'mobilizer');
    $mobile_theme_js_path = DRUPAL_ROOT . '/' . $theme_path . '/js/mobilizer.js';
    $f = fopen($mobile_theme_js_path, 'w');
    foreach ($get_js_files as $key => $val) {
      $filename = DRUPAL_ROOT . '/' . $val;
      $src_f = fopen($filename, 'r');
      $data = fread($src_f, filesize($filename));
      fwrite($f, $data);
      fclose($src_f);
    }
    fclose($f);
  }
}

/**
 * Callback function for copying mobile theme to sites/all/themes folder.
 * @param $mtheme - mobile theme name.
 */
function mobilizer_copy_theme_in_theme_folder($mtheme_name) {
  $active_theme = variable_get('theme_default');
  $theme_path = drupal_get_path('theme', $active_theme);
  $dest_dir = DRUPAL_ROOT . '/sites/all/themes/' . $mtheme_name;
  $module_path = drupal_get_path('module', 'mobilizer');
  $src_dir = DRUPAL_ROOT . '/' . $module_path . '/' . $mtheme_name;
  mobilizer_copy_directory( $src_dir, $dest_dir );
}

/**
 * Callback function for copying source directory files into destination directory
 * @param $source - source directory name
 * @param $destination - destination directory name
 */
function mobilizer_copy_directory($source, $destination) {
  if ( is_dir( $source ) ) {
    @mkdir( $destination );
    $directory = dir( $source );
      while ( FALSE !== ( $readdirectory = $directory->read() ) ) {
      if ( $readdirectory == '.' || $readdirectory == '..' ) {
        continue;
      }
      $path_dir = $source . '/' . $readdirectory; 
      if (is_dir( $path_dir)) {
        mobilizer_copy_directory( $path_dir, $destination . '/' . $readdirectory );
        continue;
      }
      copy($path_dir, $destination . '/' . $readdirectory);
    }
    $directory->close();
  }
  else {
    copy($source, $destination);
  }
}

/**
 * Callback function for creating menu for mobilizer.
 */
function mobilizer_create_default_menu() {
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array('fetch' => PDO::FETCH_ASSOC));
  $menu_options['select'] = 'Select';
  foreach ($result as $menu) {
    if (($menu['menu_name'] != 'navigation') && ($menu['menu_name'] != 'user-menu') && ($menu['menu_name'] != 'management')) {
      $menu_options[] = $menu['menu_name'];
    }
  }
  // If main menu exists then we will use this for 
  // creating mobile theme menu else choose first menu from existing menu items
  if (array_search('main-menu', $menu_options)) {
    $menu_name = 'main-menu';
  }
  else {
    $menu_name = $menu_options[0];
  }
  // set the default selected menu to load by default in Finalize menu page
  variable_set('mobilizer_default_menu', $menu_name);
  mobilizer_copy_default_menu($menu_name);
}

/**
 * Callback function for copying menu for mobilizer menu.
 * @param $source - machine name of menu.
 */
function mobilizer_copy_default_menu($web_menu_name = NULL) {
  if (!($web_menu_name)) {
    return FALSE;
  }
  // Create new menu.
  // Begin code taken from menu_edit_menu_submit().
  $path = 'admin/structure/menu/manage/';
  // Add 'menu-' to the menu name to help avoid name-space conflicts.
  $mobilizer_menu = variable_get('mobilizer_menu', 'menu-website-mobilizer');
  $menu['menu_name'] = $mobilizer_menu;
  $menu['title'] = t('Menu for theme mobilizer');
  $menu['description'] = '';
  $link['link_title'] = t('Menu for theme mobilizer');
  $link['link_path'] = $path . $menu['menu_name'];
  $link['router_path'] = $path . '%';
  $link['module'] = 'menu';
  $link['plid'] = db_query("SELECT mlid FROM {menu_links} WHERE link_path = :link AND module = :module", array(
    ':link' => 'admin/structure/menu',
    ':module' => 'system'
  ))
  ->fetchField();
  // Delete existing mobile menu
  menu_delete($menu);
  menu_link_save($link);
  menu_save($menu);
  $parrent_menu_items = menu_tree_page_data($web_menu_name, $max_depth = 1, $only_active_trail = FALSE);
  foreach ($parrent_menu_items as $key => $value) {
    $menu_item = array();
    $menu_item['link_title'] = $value['link']['link_title'];
    $menu_item['link_path'] = $value['link']['link_path'];
    $menu_item['router_path'] = $value['link']['router_path'];
    $menu_item['mlid'] = 0;
    $menu_item['module'] = 'menu';
    $menu_item['has_children'] = 0;
    $menu_item['options'] = $value['link']['options'];
    $menu_item['customized'] = 1;
    $menu_item['description'] = '';
    $menu_item['expanded'] = 0;
    $menu_item['weight'] = $value['link']['weight'];
    $menu_item['hidden'] = 0;
    $menu_item['plid'] = 0;
    $menu_item['menu_name'] = $menu['menu_name'];
    menu_link_save($menu_item);
  }
  if (db_query("SELECT 1 FROM {menu_custom} WHERE menu_name = :menu", array(':menu' => $menu['menu_name']))->fetchField()) {
    variable_set('mobilizer_selected_web_menu_item', $web_menu_name);
  }
  // Delete menu icons entry if already exists
  db_delete('mobilizer_menu_icons')->execute();
  // Delete block content entry if already exists
  db_delete('mobilizer_node')->execute();
  variable_set("mobilizer_css_creation", '');
  // Get block content html on every menu links and save it into tables
  _mobilizer_menu_blocks_save($mobilizer_menu);
}

/**
 * Callback function for assigning mobilizer content block in mobilizer content region.
 * @param $source - mobile theme name.
 */
function mobilizer_assign_mobile_content_in_mobilizer_theme_region($mtheme_name) {
  db_update('block')
  ->fields(array(
    'status' => 1,
    'region' => 'mobilizer_content',
    'title' => '<none>',
  ))
  ->condition('delta', 'mobilizer_content', '=')
  ->condition('theme', $mtheme_name, '=')
  ->execute();
}

/**
 * Callback function for getting list of css files for existing web theme.
 */
function mobilizer_get_theme_css_file() {
  $active_theme = variable_get('theme_default'); 
  $themes = list_themes();
  if (isset($themes[$active_theme]->info['stylesheets']['all'])) {
    $get_css_files = $themes[$active_theme]->info['stylesheets']['all'];
  }
  else {
    return FALSE;
  }
  return $get_css_files;
}

/**
 * Callback function for getting list of js files for existing web theme.
 */
function mobilizer_get_theme_js_file() {
  $active_theme = variable_get('theme_default'); 
  $themes = list_themes();
  if ($themes[$active_theme]->info['scripts']) {
    $get_js_files = $themes[$active_theme]->info['scripts'];
  }
  else {
    return FALSE;
  }
  return $get_js_files;
}

/**
 * Callback function for getting css style content.
 * @param $key - css tag name.
 * @param $css_get_section - css style elements.  
 */
function _mobilizer_get_css_string($key = '', $css_get_section = NULL) {
  if (!$css_get_section)
  return FALSE;
  $css_string_temp = '';
  $css_string = mobilizer_map_css_tag($key) . "{ ";
  foreach ($css_get_section as $css_attr => $css_attr_val) {
    if (($css_attr != "width") && ($css_attr != "padding") && ($css_attr != "margin") && ($css_attr != "min-width") && ($css_attr != "float"))
    $css_string .= $css_attr . " : " . $css_attr_val . ";";
    if ($key == 'header') {
      if ($css_attr == 'background-color')
      $css_string_temp = "#footer{background: " . $css_attr_val . ";}\n";
    }
  }
  $css_string .= "}\n" . $css_string_temp;
  return (string)$css_string;
}

/**
 * Callback function for mapping web theme css tags with mobile theme css tags.
 * @param $key - web theme css tag name.
 */
function mobilizer_map_css_tag($key) {
  $tags_map = array('body' => '#page', 'header' => '#header', 'ul.main-menu li' => '#main-menu ul.menu li', 'ul.main-menu li a' => '#main-menu ul.menu li a');
  if (isset($tags_map[$key]))
  return $tags_map[$key];
  return $key;
}

/**
 * Callback function for getting list of predefined css tags.
 * Using these tags for css parsing from web theme.
 */ 
function _mobilizer_predefind_css_tags() {
  return array(
    'head',
    'header',
    'body',
    'h1',
    'h2',
    'h3',
    'h4',
    'h5',
    'h6',
    'quote',
    'code',
    'fieldset',
    'pre',
    'kbd',
    'p',
    'a',
    'a:link',
    'a:visited',
    'a:hover',
    'a:active',
    'hr',
    'ol',
    'li.leaf',
    'ul.menu',
    'ul.menu li',
    'dl',
    'dl dd',
    'img',
    'a img',
    'table',
    'thead',
    'th',
    'th a:link',
    'td',
    'tr.even',
    'tr.odd',
    'tbody th',
    'tr.info',
    'tr.drag',
    'tr.drag-previous',
    'tr.odd td.active',
    'tr.even td.active',
    'td.region-title',
    'td.module',
    'td.container',
    'td.category',
    'span.form-required',
  );
}

/**
 * Callback function for getting list of predefined css tags for menu style.
 * Using these tags for css parsing from web theme.
 */ 
function _mobilizer_predefind_menu_css_tags() {
  return array('ul.main-menu', 'ul.main-menu li', 'ul.main-menu li a', 'ul.secondary-menu', 'ul.secondary-menu li', 'ul.secondary-menu li a', 'ul.primary', 'ul.primary li', 'ul.primary li a', 'ul.secondary', 'ul.secondary li', 'ul.secondary li a');
}

/**
 * Callback function for getting hex color code.
 * @param $color_name - color name. 
 */ 
function _mobilizer_color_name_to_hex($color_name) {
  // standard 147 HTML color names
  $colors  =  array(
    'aliceblue' => 'F0F8FF',
    'antiquewhite' => 'FAEBD7',
    'aqua' => '00FFFF',
    'aquamarine' => '7FFFD4',
    'azure' => 'F0FFFF',
    'beige' => 'F5F5DC',
    'bisque' => 'FFE4C4',
    'black' => '000000',
    'blanchedalmond ' => 'FFEBCD',
    'blue' => '0000FF',
    'blueviolet' => '8A2BE2',
    'brown' => 'A52A2A',
    'burlywood' => 'DEB887',
    'cadetblue' => '5F9EA0',
    'chartreuse' => '7FFF00',
    'chocolate' => 'D2691E',
    'coral' => 'FF7F50',
    'cornflowerblue' => '6495ED',
    'cornsilk' => 'FFF8DC',
    'crimson' => 'DC143C',
    'cyan' => '00FFFF',
    'darkblue' => '00008B',
    'darkcyan' => '008B8B',
    'darkgoldenrod' => 'B8860B',
    'darkgray' => 'A9A9A9',
    'darkgreen' => '006400',
    'darkgrey' => 'A9A9A9',
    'darkkhaki' => 'BDB76B',
    'darkmagenta' => '8B008B',
    'darkolivegreen' => '556B2F',
    'darkorange' => 'FF8C00',
    'darkorchid' => '9932CC',
    'darkred' => '8B0000',
    'darksalmon' => 'E9967A',
    'darkseagreen' => '8FBC8F',
    'darkslateblue' => '483D8B',
    'darkslategray' => '2F4F4F',
    'darkslategrey' => '2F4F4F',
    'darkturquoise' => '00CED1',
    'darkviolet' => '9400D3',
    'deeppink' => 'FF1493',
    'deepskyblue' => '00BFFF',
    'dimgray' => '696969',
    'dimgrey' => '696969',
    'dodgerblue' => '1E90FF',
    'firebrick' => 'B22222',
    'floralwhite' => 'FFFAF0',
    'forestgreen' => '228B22',
    'fuchsia' => 'FF00FF',
    'gainsboro' => 'DCDCDC',
    'ghostwhite' => 'F8F8FF',
    'gold' => 'FFD700',
    'goldenrod' => 'DAA520',
    'gray' => '808080',
    'green' => '008000',
    'greenyellow' => 'ADFF2F',
    'grey' => '808080',
    'honeydew' => 'F0FFF0',
    'hotpink' => 'FF69B4',
    'indianred' => 'CD5C5C',
    'indigo' => '4B0082',
    'ivory' => 'FFFFF0',
    'khaki' => 'F0E68C',
    'lavender' => 'E6E6FA',
    'lavenderblush' => 'FFF0F5',
    'lawngreen' => '7CFC00',
    'lemonchiffon' => 'FFFACD',
    'lightblue' => 'ADD8E6',
    'lightcoral' => 'F08080',
    'lightcyan' => 'E0FFFF',
    'lightgoldenrodyellow' => 'FAFAD2',
    'lightgray' => 'D3D3D3',
    'lightgreen' => '90EE90',
    'lightgrey' => 'D3D3D3',
    'lightpink' => 'FFB6C1',
    'lightsalmon' => 'FFA07A',
    'lightseagreen' => '20B2AA',
    'lightskyblue' => '87CEFA',
    'lightslategray' => '778899',
    'lightslategrey' => '778899',
    'lightsteelblue' => 'B0C4DE',
    'lightyellow' => 'FFFFE0',
    'lime' => '00FF00',
    'limegreen' => '32CD32',
    'linen' => 'FAF0E6',
    'magenta' => 'FF00FF',
    'maroon' => '800000',
    'mediumaquamarine' => '66CDAA',
    'mediumblue' => '0000CD',
    'mediumorchid' => 'BA55D3',
    'mediumpurple' => '9370D0',
    'mediumseagreen' => '3CB371',
    'mediumslateblue' => '7B68EE',
    'mediumspringgreen' => '00FA9A',
    'mediumturquoise' => '48D1CC',
    'mediumvioletred' => 'C71585',
    'midnightblue' => '191970',
    'mintcream' => 'F5FFFA',
    'mistyrose' => 'FFE4E1',
    'moccasin' => 'FFE4B5',
    'navajowhite' => 'FFDEAD',
    'navy' => '000080',
    'oldlace' => 'FDF5E6',
    'olive' => '808000',
    'olivedrab' => '6B8E23',
    'orange' => 'FFA500',
    'orangered' => 'FF4500',
    'orchid' => 'DA70D6',
    'palegoldenrod' => 'EEE8AA',
    'palegreen' => '98FB98',
    'paleturquoise' => 'AFEEEE',
    'palevioletred' => 'DB7093',
    'papayawhip' => 'FFEFD5',
    'peachpuff' => 'FFDAB9',
    'peru' => 'CD853F',
    'pink' => 'FFC0CB',
    'plum' => 'DDA0DD',
    'powderblue' => 'B0E0E6',
    'purple' => '800080',
    'red' => 'FF0000',
    'rosybrown' => 'BC8F8F',
    'royalblue' => '4169E1',
    'saddlebrown' => '8B4513',
    'salmon' => 'FA8072',
    'sandybrown' => 'F4A460',
    'seagreen' => '2E8B57',
    'seashell' => 'FFF5EE',
    'sienna' => 'A0522D',
    'silver' => 'C0C0C0',
    'skyblue' => '87CEEB',
    'slateblue' => '6A5ACD',
    'slategray' => '708090',
    'slategrey' => '708090',
    'snow' => 'FFFAFA',
    'springgreen' => '00FF7F',
    'steelblue' => '4682B4',
    'tan' => 'D2B48C',
    'teal' => '008080',
    'thistle' => 'D8BFD8',
    'tomato' => 'FF6347',
    'turquoise' => '40E0D0',
    'violet' => 'EE82EE',
    'wheat' => 'F5DEB3',
    'white' => 'FFFFFF',
    'whitesmoke' => 'F5F5F5',
    'yellow' => 'FFFF00',
    'yellowgreen' => '9ACD32'
  );
  $color_name = strtolower($color_name);
  if (isset($colors[$color_name])) {
    return ('#' . $colors[$color_name]);
  }
  else {
    return ('');
  }
}
