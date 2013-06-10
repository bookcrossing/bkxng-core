<?php
/**
* @file
* The theme system, which controls the output of mobile site.
*
* The theme system allows for the major items of the site.
*/
function mobilizer_setting_color_platter_style() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_setting_color_platter");
  $html_color = drupal_render($render_array);
  return _mobilizer_themer($html_color, _mobilizer_iframe_with_html($base_url));
}

/**
* 
* @param unknown $color_paletter_no
* @param unknown $palette_no
* @return string
*/
function mobilizer_color_palette_div($color_paletter_no, $palette_no) {
  $html = "";
  $color_palette_arr = mobilizer_color_palette_style();
  $html = '<div id = list_color_palette' . $palette_no . ' class="palette_border">';
  foreach ( $color_palette_arr as $key => $color_label ) {
    $color_label_name = str_replace(" ", "_", strtolower($color_label));
    $html .= '<div class="' . $color_paletter_no . '_' . $color_label_name . '_cs edit-' . str_replace(" ", "-", strtolower($color_label)) . '-txt" ></div>';
  }
  
  if ($palette_no != "no") {
    $html .= "<div class = customize_link_cs ><a class=customize" . $palette_no . " href='#'>Customize</a></div>";
  }
  $html .= '</div>';
  return $html;
}

/**
* 
* @param unknown $form
* @param unknown $form_state
* @return multitype:string Ambigous <The, string, A, Optional>
*/
function mobilizer_setting_color_platter($form, &$form_state) {
  global $base_url;
  $form ['#attributes'] ['enctype'] = 'multipart/form-data';
//this is move to start.mobilizer.inc
  $mobilizer_color_palette = variable_get("mobilizer_color_palette", NULL);
  
  drupal_add_js(drupal_get_path('module', 'mobilizer') . '/js/color_palette.js');
  drupal_add_css(drupal_get_path('module', 'mobilizer') . '/css/mobilizer.css');
  
  $mobilizer_style = variable_get("mobilizer_style", NULL);
  /*set in js for run time changes*/
  
  drupal_add_js(array(
      'mobilizer_color_palette' => $mobilizer_color_palette
  ), 'setting');
  drupal_add_js(array(
      'mobilizer_style' => $mobilizer_style
  ), 'setting');
  
  /* -- end -- */
  
  $form ['color_palette'] = array(
      '#type' => 'radios',
      '#title' => t('Color Palette'),
      '#default_value' => isset($mobilizer_style ['colorpalatte'] ['palette_id']) ? $mobilizer_style ['colorpalatte'] ['palette_id'] : '',
      '#attributes' => array(
          'class' => array(
              'color_palette_main_cs'
          )
      ),
      '#options' => array(
          'color_palette1' => t(mobilizer_color_palette_div("color_palette1", "1")),
          'color_palette2' => t(mobilizer_color_palette_div("color_palette2", "2")),
          'color_palette3' => t(mobilizer_color_palette_div("color_palette3", "3")),
          'color_palette4' => t(mobilizer_color_palette_div("color_palette4", "4")),
          'color_palette5' => t(mobilizer_color_palette_div("color_palette5", "5"))
      )
  );
  
  $form ['color_palette'] ['color_palette1'] = array(
      '#attributes' => array(
          'class' => array(
              'color_palette1'
          )
      )
  );
  $form ['color_palette'] ['color_palette2'] = array(
      '#attributes' => array(
          'class' => array(
              'color_palette2'
          )
      )
  );
  $form ['color_palette'] ['color_palette3'] = array(
      '#attributes' => array(
          'class' => array(
              'color_palette3'
          )
      )
  );
  $form ['color_palette'] ['color_palette4'] = array(
      '#attributes' => array(
          'class' => array(
              'color_palette4'
          )
      )
  );
  $form ['color_palette'] ['color_palette5'] = array(
      '#attributes' => array(
          'class' => array(
              'color_palette5'
          )
      )
  );
  
  /**
@CUSTOMIZE CONTAINER 
*/
  $form ['customize_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Customize Color Palette'),
      '#weight' => 5,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array(
          'style' => 'display:none',
          'class' => array(
              'customize_color_palette_container'
          )
      ),
      '#prefix' => '<div style = "display:none" class="customize_container_main_cs">',
      '#suffix' => '</div>'
  );
  
  $form ['customize_container'] ['current_palette'] = array(
      '#markup' => t("<div class='current_palette_main_cs clearfix'><div class='current_palette'>Current Palette</div>" . mobilizer_color_palette_div("selected_palette", "no") . '</div>')
  );
  
  $color_palette_arr = mobilizer_color_palette_style();
  
  foreach ( $color_palette_arr as $key => $color_label ) {
    $html_div = ($key + 1) % 3 == 0 ? "<div class='clear'></div>" : "";
    $color_label_name = str_replace(" ", "_", strtolower($color_label));
    /* @color palette */
    $field_name = 'event_color_picker';
    $color_style_name = ! empty($mobilizer_style ['colorpalatte'] [$color_label_name]) ? $mobilizer_style ['colorpalatte'] [$color_label_name] : "#ffffff";
    
    $form ['customize_container'] [$color_label_name . '_txt'] = array(
        '#title' => check_plain(t($color_label)),
        '#default_value' => $color_style_name,
// All 2 of the following #dependencies must be met.
        '#dependency_count' => 1,
        '#dependency' => array(
            'edit-row-options-colors-legend' => array(
                'taxonomy'
            ),
            'edit-row-options-colors-taxonomy-field' => array(
                $field_name
            )
        ),
        '#type' => 'textfield',
        '#size' => 7,
        '#maxlength' => 7,
        '#element_validate' => array(
            'mobilizer_validate_hex_color'
        ),
        '#prefix' => '<div class="palette-colorpicker-wrapper">',
        '#suffix' => '<div class="palette-colorpicker"></div></div>' . $html_div,
        '#attributes' => array(
            'class' => array(
                'edit-palette-colorpicker',
                $color_label_name . '_cs'
            ),
            'style' => 'background-color:' . $color_style_name
        ),
        '#attached' => array(
// Add Farbtastic color picker.
            'library' => array(
                array(
                    'system',
                    'farbtastic'
                )
            ),
// Add javascript to trigger the colorpicker.
            'js' => array(
                drupal_get_path('module', 'mobilizer') . '/js/mobilizer-colorpicker.js'
            )
        )
    );
  }
  
  $form ['customize_container'] ['customize_done'] = array(
      '#type' => 'button',
      '#value' => t('Done'),
      '#attributes' => array(
          'class' => array(
              'customize_done_btn',
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="customize_done_main_cs">',
      '#suffix' => '</div>'
  );
  
  /**
@END CUSTOMIZE CONTAINER
*/
  
  $form ['site_background'] = array(
      '#type' => 'managed_file',
      '#upload_location' => 'public://mobilizer_header_images/',
      '#title' => t('Site Background Image'),
      '#default_value' => ! empty($mobilizer_style ['colorpalatte'] ['style_bg'] ['fid']) ? $mobilizer_style ['colorpalatte'] ['style_bg'] ['fid'] : "",
      '#theme' => 'preview_image_thumbnail',
      '#size' => 30,
      '#attributes' => array(
          'class' => array(
              'site_background_main_cs'
          )
      )
  );
  
  /**
@Start Start of the Text and Links and Heading Section
*/
  $options_font = array(
      'Georgia' => 'Georgia',
      'serif' => 'Serif',
      'Times New Roman' => 'Times New Roman',
      'Arial' => 'Arial',
      'Helvetica' => 'Helvetica',
      'sans-serif' => 'Sans-serif',
      'Arial Black' => 'Arial Black',
      'Gadget' => 'Gadget',
      'Comic Sans MS' => 'Comic Sans MS',
      'cursive' => 'Cursive',
      'Impact' => 'Impact',
      'Charcoal' => 'Charcoal',
      'Lucida Sans Unicode' => 'Lucida Sans Unicode',
      'Lucida Grande' => 'Lucida Grande',
      'Tahoma' => 'Tahoma',
      'Geneva' => 'Geneva',
      'Trebuchet MS' => 'Trebuchet MS',
      'Verdana' => 'Verdana',
      'Courier New' => 'Courier New',
      'Courier' => 'Courier',
      'monospace' => 'Monospace',
      'Lucida Console' => 'Lucida Console',
      'Monaco' => 'Monaco'
  );
  asort($options_font);
  $form ['text_style'] = array(
      '#markup' => t("<div class='text_style_main_cs'>Text Style</div>")
  );
  
  $form ['text_links'] = array(
      '#markup' => t("<div class='text_links_main_cs'>" . l(t("Text & Link"), '#', array(
          'attributes' => array(
              'class' => 'text_links_cs',
              'title' => 'Text & Link'
          ),
          'html' => TRUE
      )) . "</div>"),
      '#prefix' => '<div class="text_link_header_outer_main_cs">'
  );
  
  $form ['header'] = array(
      '#markup' => t("<div class='header_main_cs'>" . l(t("Header"), '#', array(
          'attributes' => array(
              'class' => 'header_cs',
              'title' => 'Header'
          ),
          'html' => TRUE
      )) . "</div>"),
      '#suffix' => '</div>'
  );
  
  $form ['text_style_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Text Style'),
      '#id' => t('text_style_fs'),
      '#weight' => 11,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array(
          'style' => 'display:none',
          'class' => array(
              'mobilizer_text_style'
          )
      ),
      '#prefix' => '<div style = "display:none" class="text_style_container_main_cs">',
      '#suffix' => '</div>'
  );
  
  $text_links = array(
      'Text',
      'Link'
  );
  foreach ( $text_links as $key => $text_label ) {
    $text_color_label_name = str_replace(" ", "_", strtolower($text_label));
    $color_style_name_text = ! empty($mobilizer_style ['text_links'] [$text_color_label_name]) ? $mobilizer_style ['text_links'] [$text_color_label_name] : "#ffffff";
    $form ['text_style_container'] [$text_color_label_name] = array(
        '#title' => check_plain(t($text_label)),
        '#default_value' => $color_style_name_text,
// All 2 of the following #dependencies must be met.
        '#dependency_count' => 1,
        '#dependency' => array(
            'edit-row-options-colors-legend' => array(
                'taxonomy'
            ),
            'edit-row-options-colors-taxonomy-field' => array(
                $field_name
            )
        ),
        '#type' => 'textfield',
        '#size' => 7,
        '#maxlength' => 7,
        '#element_validate' => array(
            'mobilizer_validate_hex_color'
        ),
        '#prefix' => '<div class="palette-colorpicker-wrapper">',
        '#suffix' => '<div class="palette-colorpicker"></div></div>',
        '#attributes' => array(
            'class' => array(
                'edit-palette-colorpicker'
            ),
            'style' => 'background-color:' . $color_style_name_text
        ),
        '#attached' => array(
// Add Farbtastic color picker.
            'library' => array(
                array(
                    'system',
                    'farbtastic'
                )
            ),
// Add javascript to trigger the colorpicker.
            'js' => array(
                drupal_get_path('module', 'mobilizer') . '/js/mobilizer-colorpicker.js'
            )
        )
    );
  }
  
  $form ['text_style_container'] ['underline_links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Underline Links'),
      '#default_value' => ! empty($mobilizer_style ['text_links'] ['underline']) ? $mobilizer_style ['text_links'] ['underline'] : 0
  );
  
  $form ['text_style_container'] ['font_type'] = array(
      '#type' => 'select',
      '#title' => t('Font Type'),
      '#default_value' => ! empty($mobilizer_style ['text_links'] ['font_type']) ? $mobilizer_style ['text_links'] ['font_type'] : '',
      '#options' => $options_font
  );
  
  $form ['text_style_container'] ['font_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Font Size'),
      '#default_value' => ! empty($mobilizer_style ['text_links'] ['font_size']) ? $mobilizer_style ['text_links'] ['font_size'] : 12,
      '#size' => 5,
      '#field_suffix' => t('px')
  );
  
  $form ['text_style_container'] ['customize_done'] = array(
      '#type' => 'button',
      '#value' => t('Done'),
      '#attributes' => array(
          'class' => array(
              'customize_done_btn_text',
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="text_style_container_done_main_cs">',
      '#suffix' => '</div>'
  );
  
  $form ['text_heading_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Headings'),
      '#id' => t('headings_fs'),
      '#weight' => 12,
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#attributes' => array(
          'style' => 'display:none',
          'class' => array(
              'mobilizer_heading'
          )
      ),
      '#prefix' => '<div style = "display:none" class="text_heading_container_main_cs">',
      '#suffix' => '</div>'
  );
  
  $heading_links = array(
      'Heading1',
      'Heading2',
      'Heading3'
  );
  foreach ( $heading_links as $key => $heading_label ) {
    $heading_links_color_label_name = str_replace(" ", "_", strtolower($heading_label));
    $color_style_name_heading = ! empty($mobilizer_style ['headers'] [$heading_links_color_label_name]) ? $mobilizer_style ['headers'] [$heading_links_color_label_name] : "#ffffff";
    
    $form ['text_heading_container'] [$heading_links_color_label_name] = array(
        '#title' => check_plain(t($heading_label)),
        '#default_value' => $color_style_name_heading,
// All 2 of the following #dependencies must be met.
        '#dependency_count' => 1,
        '#dependency' => array(
            'edit-row-options-colors-legend' => array(
                'taxonomy'
            ),
            'edit-row-options-colors-taxonomy-field' => array(
                $field_name
            )
        ),
        '#type' => 'textfield',
        '#size' => 7,
        '#maxlength' => 7,
        '#element_validate' => array(
            'mobilizer_validate_hex_color'
        ),
        '#prefix' => '<div class="heading_field_container_cs"><div class="palette-colorpicker-wrapper">',
        '#suffix' => '<div class="palette-colorpicker"></div></div>',
        '#attributes' => array(
            'class' => array(
                'edit-palette-colorpicker'
            ),
            'style' => 'background-color:' . $color_style_name_heading
        ),
        '#attached' => array(
// Add Farbtastic color picker.
            'library' => array(
                array(
                    'system',
                    'farbtastic'
                )
            ),
// Add javascript to trigger the colorpicker.
            'js' => array(
                drupal_get_path('module', 'mobilizer') . '/js/mobilizer-colorpicker.js'
            )
        )
    );
    
    $form ['text_heading_container'] [$heading_links_color_label_name . '_font_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Font Size'),
        '#default_value' => ! empty($mobilizer_style ['headers'] [$heading_links_color_label_name . '_font_size']) ? $mobilizer_style ['headers'] [$heading_links_color_label_name . '_font_size'] : 12,
        '#size' => 5,
        '#field_suffix' => t('px'),
        '#suffix' => '</div>'
    );
  }
  
  $form ['text_heading_container'] ['customize_done'] = array(
      '#type' => 'button',
      '#value' => t('Done'),
      '#attributes' => array(
          'class' => array(
              'customize_done_btn_heading',
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="text_heading_container_done_main_cs">',
      '#suffix' => '</div>'
  );
  /**
@END End of the Text and Links and Heading Section
*/
  
  $form ['preview'] = array(
      '#type' => 'button',
      '#value' => t('Preview'),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="preview_and_proceed_main_cs"><div class="preview_main_cs">',
      '#suffix' => '</div>',
      '#ajax' => array(
          'event' => 'click',
          'callback' => 'mobilizer_ajax_callback',
          'wrapper' => 'iframe_div',
          'method' => 'replace',
          'effect' => 'fade'
      )
  );
  
  $form ['proceed'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed'),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="proceed_main_cs">',
      '#suffix' => '</div></div>'
  );

//$form['#theme'] = 'style_theme_layout_form';	
//$form = mobilizer_iframe_include_form_preview($form , $base_url);
  return $form;
}

/**
*  Check to make sure the user has entered a valid 6 digit hex color.
*/
function mobilizer_validate_hex_color($element, &$form_state) {
  if (! $element ['#required'] && empty($element ['#value'])) {
    return;
  }
  if (! preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $element ['#value'])) {
    form_error($element, t("'@color' is not a valid hex color", array(
        '@color' => $element ['#value']
    )));
  } 
  else {
    form_set_value($element, $element ['#value'], $form_state);
  }
}

/**
* 
* @param unknown $form
* @param unknown $form_state
*/
function mobilizer_setting_color_platter_submit($form, &$form_state) {
  $mobilizer_style ['colorpalatte'] = array();
  $color_palette_arr = mobilizer_color_palette_style();
  $mobilizer_style ['colorpalatte'] ['palette_id'] = $form_state ['input'] ['color_palette'];
  foreach ( $color_palette_arr as $key => $color_label ) {
    $color_label_name = str_replace(" ", "_", strtolower($color_label));
    $mobilizer_style ['colorpalatte'] [$color_label_name] = $form_state ['input'] [$color_label_name . '_txt'];
  }
  
  if (isset($form_state ['values'] ['site_background']) && $form_state ['values'] ['site_background'] != '0' && $form_state ['values'] ['site_background'] ['fid'] != '0') {
    $site_bg_image = file_load($form_state ['values'] ['site_background'] ['fid']);
    if (isset($site_bg_image)) {
      $mobilizer_style ['colorpalatte'] ['style_bg'] ['filename'] = $site_bg_image->filename;
      $mobilizer_style ['colorpalatte'] ['style_bg'] ['uri'] = $site_bg_image->uri;
      $mobilizer_style ['colorpalatte'] ['style_bg'] ['fid'] = $site_bg_image->fid;
    }
  } 
  else {
    $mobilizer_style ['colorpalatte'] ['style_bg'] ['filename'] = "";
    $mobilizer_style ['colorpalatte'] ['style_bg'] ['uri'] = "";
  }
  /**
* Start setting up values for Text & Links and Headings
*/
  $mobilizer_style ['text_links'] ['text'] = $form_state ['input'] ['text'];
  $mobilizer_style ['text_links'] ['link'] = $form_state ['input'] ['link'];
  $mobilizer_style ['text_links'] ['underline'] = $form_state ['input'] ['underline_links'];
  $mobilizer_style ['text_links'] ['font_type'] = $form_state ['input'] ['font_type'];
  $mobilizer_style ['text_links'] ['font_size'] = $form_state ['input'] ['font_size'];
  $mobilizer_style ['headers'] ['heading1'] = $form_state ['input'] ['heading1'];
  $mobilizer_style ['headers'] ['heading2'] = $form_state ['input'] ['heading2'];
  $mobilizer_style ['headers'] ['heading3'] = $form_state ['input'] ['heading3'];
  $mobilizer_style ['headers'] ['heading1_font_size'] = $form_state ['input'] ['heading1_font_size'];
  $mobilizer_style ['headers'] ['heading2_font_size'] = $form_state ['input'] ['heading2_font_size'];
  $mobilizer_style ['headers'] ['heading3_font_size'] = $form_state ['input'] ['heading3_font_size'];
  
  /**
* Start setting up values for Text & Links and Headings
*/
  variable_set("mobilizer_style", $mobilizer_style);
  
  /* this is for css writing code for theme reflection */
  $css_creation = variable_get("mobilizer_css_creation");
  if (isset($form_state ['input'] ['header_background_txt'])) {
    $css_creation ['#header'] ['background-color'] = $form_state ['input'] ['header_background_txt'];
  }
  
  if (isset($form_state ['input'] ['text_color_txt'])) {
    $css_creation ['.node-bdy p'] ['color'] = $form_state ['input'] ['text_color_txt'];
    $css_creation ['p'] ['color'] = $form_state ['input'] ['text_color_txt'];
  }
  
  if (isset($form_state ['input'] ['navigation_txt'])) {
    $css_creation ['#main-menu ul.menu li'] ['background'] = $form_state ['input'] ['navigation_txt'];
  }
  
  if (isset($form_state ['input'] ['link_txt'])) {
    $css_creation ['a'] ['color'] = $form_state ['input'] ['link_txt'];
  }
  
  if (isset($form_state ['input'] ['site_background_txt'])) {
    $css_creation ['#page'] ['background-color'] = $form_state ['input'] ['site_background_txt'];
  }
  
  if (isset($form_state ['input'] ['navigation_label_txt'])) {
    $css_creation ['#main-menu ul.menu li a'] ['color'] = $form_state ['input'] ['navigation_label_txt'];
  }
  
  if (isset($form_state ['values'] ['site_background']) && $form_state ['values'] ['site_background'] != '0' && $form_state ['values'] ['site_background'] ['fid'] != '0') {
    $site_bg_image = file_load($form_state ['values'] ['site_background'] ['fid']);
    if (isset($site_bg_image)) {
      $bg_image = file_create_url($site_bg_image->uri);
      $css_creation ['#page'] ['background-image'] = "url('" . $bg_image . "')";
      $css_creation ['#page'] ['background-repeat'] = "no-repeat";
    }
  }
  
  /**
* Text & Links and Headers
*/
  if (isset($form_state ['input'] ['underline_links'])) {
    $css_creation ['a'] ['text-decoration'] = 'underline';
  }
  else {
    $css_creation ['a'] ['text-decoration'] = 'none';
  }
  
  if (isset($form_state ['input'] ['font_type'])) {
    $css_creation ['.node-bdy p'] ['font-family'] = $form_state ['input'] ['font_type'];
    $css_creation ['a'] ['font-family'] = $form_state ['input'] ['font_type'];
    $css_creation ['p'] ['font-family'] = $form_state ['input'] ['font_type'];
  }
  
  if (isset($form_state ['input'] ['font_size'])) {
    $css_creation ['.node-bdy p'] ['font-size'] = $form_state ['input'] ['font_size'] . 'px';
    $css_creation ['a'] ['font-size'] = $form_state ['input'] ['font_size'] . 'px';
    $css_creation ['p'] ['font-size'] = $form_state ['input'] ['font_size'] . 'px';
  }
  
  if (isset($form_state ['input'] ['heading1'])) {
    $css_creation ['h1'] = array(
        'color' => $form_state ['input'] ['heading1']
    );
  }
  
  if (isset($form_state ['input'] ['heading2'])) {
    $css_creation ['h2'] = array(
        'color' => $form_state ['input'] ['heading2']
    );
  }
  
  if (isset($form_state ['input'] ['heading3'])) {
    $css_creation ['h3'] = array(
        'color' => $form_state ['input'] ['heading3']
    );
  }
  
  if (isset($form_state ['input'] ['heading1_font_size'])) {
    $css_creation ['h1'] ['font-size'] = $form_state ['input'] ['heading1_font_size'] . 'px';
  }
  
  if (isset($form_state ['input'] ['heading2_font_size'])) {
    $css_creation ['h2'] ['font-size'] = $form_state ['input'] ['heading2_font_size'] . 'px';
  }
  
  if (isset($form_state ['input'] ['heading3_font_size'])) {
    $css_creation ['h3'] ['font-size'] = $form_state ['input'] ['heading3_font_size'] . 'px';
  }
  
  /**
* ENDS Text & Links and Headers
*/
  
  variable_set("mobilizer_css_creation", $css_creation);
  mobilizer_css_file_generator($css_creation);
  variable_set('mobilizer_style_process', 1);
  if ($form_state ['values'] ['op'] == 'Proceed') {
    drupal_goto('admin/mobilizer/config/header');
  }
}





