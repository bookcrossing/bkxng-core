<?php

/**
 * @file
 * Containg function for pages, listing and create new pages.
 */

/**
 * Function to render featured page item listing. 
 * @return themed html of list of pages.
 */
function mobilizer_featured_page_listing_add() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_featured_page_listing");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to get featured page listing.
 * @return html of list of pages.
 */
function mobilizer_featured_page_listing($form, &$form_state) {
  global $base_url;
  variable_set('mobilizer_page_process', 1);
  $output = '';
  drupal_add_library('system', 'drupal.ajax');
  drupal_add_library('system', 'jquery.form');
  $default_page_header = array(
    t('Page Title'),
    array(
      'data' => 'Edit'
    ),
    array(
      'data' => 'Delete'
    ),
    array(
      'data' => 'Preview'
    )
  );
  $mobilizer_menu = variable_get('mobilizer-menu', 'menu-website-mobilizer');
  $path_obj = _mobilizer_get_menu_parent_items($mobilizer_menu);
  $default_page_rows = array();
  foreach ($path_obj as $obj) {
    $edit_path = 'admin/mobilizer/select-menu-configure/page/' . $obj ['mlid'] . '/edit';
    $delete_path = 'admin/mobilizer/select-menu-configure/page/' . $obj ['mlid'] . '/delete';
    $query = db_select('mobilizer_featured_page', 'm');
    $query->fields('m', array(
      'nid',
      'page_type',
      'mid'
    ));
    $query->condition('mid', $obj ['mlid']);
    $result = $query->execute();
    $query_result = $result->fetchAssoc();
    if (isset($query_result ['mid'])) {
      $edit_path = 'admin/mobilizer/featured-mobilizer/' . $query_result ['page_type'] . '/' . $query_result ['nid'] . '/edit';
      $delete_path = 'admin/mobilizer/featured-mobilizer/' . $query_result ['nid'] . '/delete';
    }
    $default_page_rows [] = array(
      t($obj ['link_title']),
      l(t('edit'), $edit_path, array(
        'attributes' => array(
          'class' => 'edit',
          'title' => 'edit'
        )
      )),
      l(t('delete'), $delete_path, array(
        'attributes' => array(
          'class' => 'delete',
          'title' => 'delete'
        )
      )),
      l(t('Preview'), 'admin/featured-page/ajax/nojs/' . base64_encode($obj ['link_path']), array(
        'attributes' => array(
          'class' => array(
            'use-ajax',
            'preview-page'
          ),
          'title' => 'Preview'
        )
      ))
    );
  }
  $output = theme('table', array(
    'header' => $default_page_header,
    'rows' => $default_page_rows,
    'attributes' => array(
      'id' => 'sort-table'
    )
    ));
  $output .= theme('pager');
  $options_addpege = array(
    'attributes' => array(
      'class' => 'btn btn-primary add-page'
    ),
    'html' => TRUE
  );
  $options_proceed = array(
    'attributes' => array(
      'class' => 'btn btn-primary proceed-page'
    ),
    'html' => TRUE
  );
  $output .= l(t('Add New Page'), 'admin/mobilizer/featured-pages/add-page', $options_addpege);
  $output .= l(t('Proceed'), 'admin/mobilizer/config/theme-setting-mobilizer', $options_proceed);
  $form ['current_pages'] = array(
    '#markup' => t($output)
  );
  return $form;
}

/**
 * Function to show page for various type page creation options.
 * @return themed html of list of pages to add.
 */
function _mobilizer_featured_page_custom_page_add() {
  global $base_url;
  $image_base_path = $base_url . '/' . drupal_get_path('module', 'mobilizer');
  $output = '<div class = "add-page-block" >';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/Basic_page.png' . ' />'), 'admin/mobilizer/featured-mobilizer/basicpage', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'Basic Page'
      )
    )) . t('Basic Page') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/about_us.png' . ' />'), 'admin/mobilizer/featured-mobilizer/aboutus', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'About Us'
      )
    )) . t('About Us') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/contact_us.png' . ' />'), 'admin/mobilizer/featured-mobilizer/contactus', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'Contact Us'
      )
    )) . t('Contact Us') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/rss.png' . ' />'), 'admin/mobilizer/featured-mobilizer/rss', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'RSS Feed'
      )
    )) . t('RSS') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/twitter.png' . ' />'), 'admin/mobilizer/featured-mobilizer/twitter', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'Twitter'
      )
    )) . t('Twitter') . '</div>';
  $output .= '<div class = "page-type">' . l(t('<img src = ' . $image_base_path . '/images/you_tube.png' . ' />'), 'admin/mobilizer/featured-mobilizer/youtube', $options = array(
      'html' => TRUE,
      'attributes' => array(
        'title' => 'Youtube'
      )
    )) . t('Youtube') . '</div>';
  $output .= '</div>';
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to get the rendered form for RSS page.
 * @return themed html RSS Page.
 */
function mobilizer_featured_page_rss_page($nid = '') {
  global $base_url;
  $render_ary = drupal_get_form('featured_page_rss_page_form', $nid);
  $html_rss = drupal_render($render_ary);
  return _mobilizer_themer($html_rss, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function create add form for RSS page.
 * @return form
 */
function featured_page_rss_page_form($form, &$form_state, $nid = '') {
  global $base_url;
  $simplepie_path_autoloder = DRUPAL_ROOT . '/' . 'sites/all/libraries/simplepie/autoloader.php';
  $simplepie_path_idn = DRUPAL_ROOT . '/' . 'sites/all/libraries/simplepie/idn/idna_convert.class.php';
 
  if (file_exists($simplepie_path_autoloder) && file_exists($simplepie_path_idn)) {
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('rss_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  $featured_page_options_visible_items = array(
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '5' => '5',
    'All items' => 'All items'
  );
  $options = array(
    'yes' => 'yes',
    'no' => 'no'
  );
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($node_data ['title']) ? $node_data ['title'] : ''
  );
  $form ['url'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed URL'),
    '#default_value' => isset($node_data ['url']) ? $node_data ['url'] : ''
  );
  $form ['number_of_items'] = array(
    '#type' => 'select',
    '#options' => $featured_page_options_visible_items,
    '#title' => t('Number of Visible Items'),
    '#default_value' => isset($node_data) ? $node_data ['number_of_visible_items'] : ''
  );
  $form ['include_content'] = array(
    '#type' => 'radios',
    '#title' => t('Collapsed Feed Content'),
    '#options' => $options,
    '#default_value' => isset($node_data ['include_content']) ? $node_data ['include_content'] : 'yes'
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_rss_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );
  }
  else {
    $form['html'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="file-permission" style="text-align:center;margin-top:90px;margin-bottom:10px;">' . t('Simplepie rss parser does not exists...! To enable rss parsering, Please download it from ') . l('www.simplepie.org/downloads','http://www.simplepie.org/downloads',array('attributes' => array('target'=>'_blank'))) . t(' and copy extracted folder into "sites/all/libraries". and rename extracted folder to "simplepie" Please make sure that autoloader.php exist as "sites/all/libraries/simplepie/autoloader.php"'). '</div>',
      '#tree' => TRUE,
    );
    $form ['cancel'] = array(
      '#markup' => l(t('Go back'), 'admin/mobilizer/featured-pages/add-page', array(
        'attributes' => array(
          'class' => 'btn btn-primary'
        )
      )),
      '#prefix' => '<div class="submit_btn_bottom">',
      '#suffix' => '</div>',
    );
	  }
  return $form;
}
/**
 * Submit function to create RSS page.
 */
function featured_page_rss_page_submit($form, &$form_state) {
  global $user;
  $type = 'rss';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  } 
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'url' => $form_state ['values'] ['url'],
    'number_of_visible_items' => $form_state ['values'] ['number_of_items'],
    'include_content' => $form_state ['values'] ['include_content']
  );
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for twitter page.
 * @return themed html RSS Page.
 */
function mobilizer_featured_page_twitter_page($nid = '') {
  global $base_url;
  $render_ary = drupal_get_form('featured_page_twitter_page_form', $nid);
  $html_twitter = drupal_render($render_ary);
  return _mobilizer_themer($html_twitter, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the twitter page form.
 * @return form.
 */
function featured_page_twitter_page_form($form, &$form_state, $nid = NULL) {
  global $base_url;
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('twitter_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  $featured_page_options_number_of_tweets = array(
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '5' => '5',
    'All items' => 'All items'
  );
  $options = array(
    'yes' => 'yes',
    'no' => 'no'
  );
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($node_data) ? $node_data ['title'] : ''
  );
  $form ['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($node_data) ? $node_data ['username'] : ''
  );
  $form ['number_of_tweets'] = array(
    '#type' => 'select',
    '#options' => $featured_page_options_number_of_tweets,
    '#title' => t('Number of Tweets'),
    '#default_value' => isset($node_data) ? $node_data ['number_of_tweets'] : ''
  );
  $form ['display_header_footer'] = array(
    '#type' => 'radios',
    '#title' => t('Include Header/Footer'),
    '#options' => $options,
    '#default_value' => isset($node_data ['display_header_footer']) ? $node_data ['display_header_footer'] : 'yes'
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_twitter_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Create twitter page form submit.
 */
function featured_page_twitter_page_submit($form, &$form_state) {
  global $user;
  $type = 'twitter';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  } 
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'username' => $form_state ['values'] ['username'],
    'number_of_tweets' => $form_state ['values'] ['number_of_tweets'],
    'display_header_footer' => $form_state ['values'] ['display_header_footer']
  );
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for Basic page.
 * @return themed html Basic Page.
 */
function mobilizer_featured_page_basic_page($nid = '') {
  global $base_url;
  $html_basic = '';
  $render_ary = drupal_get_form('featured_page_basic_page_form', $nid);
  $html_basic = drupal_render($render_ary);
  return _mobilizer_themer($html_basic, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the form for Basic page.
 * @return form.
 */
function featured_page_basic_page_form($form, &$form_state, $nid = '') {
  global $base_url;
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('basicpage_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#required' => TRUE,
    '#default_value' => isset($node_data) ? $node_data ['title'] : ''
  );
  $form ['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#cols' => 60,
    '#rows' => 5,
    '#default_value' => isset($node_data) ? $node_data ['description'] : '',
    '#format' => 'full_html'
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_basic_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit function for basic page creation.
 */
function featured_page_basic_page_submit($form, &$form_state) {
  global $user;
  $type = 'basicpage';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  }
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'description' => $form_state ['values'] ['description'] ['value']
  );
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for About page.
 * @return themed html About Page.
 */
function mobilizer_featured_page_about_page($nid = '') {
  global $base_url;
  $render_ary = drupal_get_form('featured_page_about_page_form', $nid);
  $html_color = drupal_render($render_ary);
  return _mobilizer_themer($html_color, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the form for About page.
 * @return form.
 */
function featured_page_about_page_form($form, &$form_state, $nid = '') {
  global $base_url;
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('aboutus_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Page Title'),
    '#required' => TRUE,
    '#default_value' => isset($node_data) ? $node_data ['title'] : ''
  );
  $form ['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#cols' => 60,
    '#resizable' => TRUE,
    '#rows' => 5,
    '#default_value' => isset($node_data) ? $node_data ['description'] : '',
    '#format' => 'full_html'
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_about_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Submit function to create About page
 */
function featured_page_about_page_submit($form, &$form_state) {
  global $user;
  $type = 'aboutus';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  }
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'description' => $form_state ['values'] ['description'] ['value']
  );
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for Youtube page.
 * @return themed html Youtube Page.
 */
function mobilizer_featured_page_youtube_page($nid = '') {
  global $base_url;
  $render_ary = drupal_get_form('featured_page_youtube_page_form', $nid);
  $html_youtube = drupal_render($render_ary);
  return _mobilizer_themer($html_youtube, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the form for Youtube page.
 * @return form.
 */
function featured_page_youtube_page_form($form, &$form_state, $nid = NULL) {
  global $base_url;
  $node_data = array();
  drupal_add_js(drupal_get_path('module', 'mobilizer') . '/js/featured_youtube_page.js');
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('youtube_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  $featured_page_options_feed_type = array(
    'playlist' => 'playlist',
    'search' => 'search',
    'favorites' => 'favorites'
  );
  $featured_page_options_visible_items = array(
    '1' => '1',
    '2' => '2',
    '3' => '3',
    '4' => '4',
    '5' => '5',
    'All items' => 'All items'
  );
  $featured_page_options_order_by = array(
    'title' => 'Title',
    'relevance' => 'Relevance',
    'published' => 'Published',
    'viewCount ' => 'View Count ',
    'rating' => 'Rating'
  );
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => isset($node_data ['title']) ? $node_data ['title'] : ''
  );
  $form ['feed_type'] = array(
    '#type' => 'select',
    '#options' => $featured_page_options_feed_type,
    '#title' => t('Feed Type'),
    '#default_value' => isset($node_data ['feed_type']) ? $node_data ['feed_type'] : ''
  );
  $form ['channel'] = array(
    '#prefix' => '<div id = "contacts-replace">',
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($node_data ['channel']) ? $node_data ['channel'] : '',
    '#suffix' => '</div>'
  );
  $form ['number_of_items'] = array(
    '#type' => 'select',
    '#options' => $featured_page_options_visible_items,
    '#title' => t('Number of Visible Items'),
    '#default_value' => isset($node_data ['number_of_visible_items']) ? $node_data ['number_of_visible_items'] : ''
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_youtube_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Submit function for youtube page creation.
 */
function featured_page_youtube_page_submit($form, &$form_state) {
  global $user;
  $type = 'youtube';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  } 
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'feed_type' => $form_state ['values'] ['feed_type'],
    'number_of_visible_items' => $form_state ['values'] ['number_of_items'],
    'channel' => $form_state ['values'] ['channel'],
  );
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for Contact page.
 * @return themed html Contact Page.
 */
function mobilizer_featured_page_contact_page($nid = '') {
  global $base_url;
  $render_array = drupal_get_form('featured_page_contact_page_form', $nid);
  $html_contact = drupal_render($render_array);
  return _mobilizer_themer($html_contact, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the form for Contact page.
 * @return form.
 */
function featured_page_contact_page_form($form, &$form_state, $nid = NULL) {
  global $base_url;
  $node_data = array();
  if (isset($nid)) {
    $featured_page_serialized_string = variable_get('contactus_' . $nid);
    $node_data = unserialize($featured_page_serialized_string);
  }
  include_once DRUPAL_ROOT . '/includes/iso.inc';
  $countries = _country_get_predefined_list();
  $form ['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Contact Form Title'),
    '#required' => TRUE,
    '#prefix' => '<div class = "first-section">',
    '#default_value' => isset($node_data ['title']) ? $node_data ['title'] : ''
  );
  $form ['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#cols' => 60,
    '#suffix' => '</div>',
    '#resizable' => TRUE,
    '#rows' => 5,
    '#default_value' => isset($node_data ['description']) ? $node_data ['description'] : '',
    '#format' => 'full_html'
  );
  $form ['email_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Admin Email'),
    '#default_value' => isset($node_data ['email_to']) ? $node_data ['email_to'] : ''
  );
  $form ['Contact_no_label'] = array(
    '#markup' => '<label>Contact No.</label>',
    '#required' => TRUE,
    '#prefix' => '<div class = "second-section">'
  );
  $form ['contact_code_plus'] = array(
    '#markup' => '<label class = "contact-country-code-class">+</label>'
  );
  $form ['contact_no_country_code'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($node_data ['contact_no_country_code']) ? $node_data ['contact_no_country_code'] : '91',
    '#size' => 5
  );
  $form ['contact_no'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($node_data ['contact_no']) ? $node_data ['contact_no'] : '',
    '#required' => TRUE
  );
  $form ['button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Call Button Label'),
    '#suffix' => '</div>',
    '#default_value' => isset($node_data ['button_label']) ? $node_data ['button_label'] : '',
    '#required' => TRUE
  );
  $form ['map_button_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Map Button Label'),
    '#prefix' => '<div class = "map-section">',
    '#default_value' => isset($node_data ['map_button_label']) ? $node_data ['map_button_label'] : '',
    '#required' => TRUE
  );
  $form ['street_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Street Address'),
    '#default_value' => isset($node_data ['street_address']) ? $node_data ['street_address'] : ''
  );
  $form ['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => isset($node_data ['city']) ? $node_data ['city'] : ''
  );
  $form ['zipcode'] = array(
    '#type' => 'textfield',
    '#title' => t('Zipcode'),
    '#default_value' => isset($node_data ['zipcode']) ? $node_data ['zipcode'] : ''
  );
  $form ['country'] = array(
    '#type' => 'textfield',
    '#title' => t('Country'),
    '#default_value' => isset($node_data ['country']) ? $node_data ['country'] : ''
  );
  $form ['state'] = array(
    '#type' => 'textfield',
    '#title' => t('State'),
    '#default_value' => isset($node_data ['state']) ? $node_data ['state'] : '',
    '#suffix' => '</div>'
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_contact_page_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * Submit function for contact page creation.
 */
function featured_page_contact_page_submit($form, &$form_state) {
  global $user;
  $type = 'contactus';
  $nid = arg(4);
  $mid = '';
  if (isset($nid)) {
    $node = node_load($nid);
    node_save($node);
  }
  else {
    $node = new stdClass();
    $node->type = 'page';
    $node->title = $form_state ['values'] ['title'];
    $node->user = $user->name;
    $node->language = 'und';
    node_save($node);
    $link = array(
      'link_title' => $form_state ['values'] ['title'],
      'link_path' => 'node/' . $node->nid,
      'menu_name' => 'menu-website-mobilizer',
      'weight' => 10,
      'expanded' => 0
    );
    $mid = mobilizer_create_custom_page_links($link);
    db_insert('mobilizer_featured_page')->fields(array(
      'nid' => $node->nid,
      'page_type' => $type,
      'mid' => $mid
    ))->execute();
  }
  $array = array(
    'title' => $form_state ['values'] ['title'],
    'description' => $form_state ['values'] ['description'],
    'email_to' => $form_state ['values'] ['email_to'],
    'button_label' => $form_state ['values'] ['button_label'],
    'contact_no_country_code' => $form_state ['values'] ['contact_no_country_code'],
    'contact_no' => $form_state ['values'] ['contact_no'],
    'map_button_label' => $form_state ['values'] ['map_button_label'],
    'street_address' => $form_state ['values'] ['street_address'],
    'city' => $form_state ['values'] ['city'],
    'zipcode' => $form_state ['values'] ['zipcode'],
    'country' => $form_state ['values'] ['country'],
    'state' => $form_state ['values'] ['state']
  );
  $email_address = $form_state ['values'] ['email_to'];
  if (!filter_var($email_address, FILTER_VALIDATE_EMAIL)) {
    mobilizer_set_messages('Invalid Email Address', 'error');
    return FALSE;
  }
  $phone_no = $form_state ['values'] ['contact_no'];
  if (!preg_match('/^[0-9]+$/', $phone_no) || $phone_no == '') {
    mobilizer_set_messages('Invalid Contact Number', 'error');
    return FALSE;
  }
  if ($form_state ['values'] ['contact_no_country_code'] == '' || !preg_match('/^[0-9]+$/', $form_state ['values'] ['contact_no_country_code'])) {
    mobilizer_set_messages('Invalid Country Code', 'error');
    return FALSE;
  }
  variable_set($type . '_' . $node->nid, serialize($array));
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to get the rendered form for Node delete.
 * @return themed html Node delete form.
 */
function mobilizer_featured_page_node_delete_page($nid = '') {
  global $base_url;
  $render_array = drupal_get_form("featured_page_node_delete_form", $nid);
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Function to create the form for Node delete.
 * @return form.
 */
function featured_page_node_delete_form($form, &$form_state, $nid = '') {
  $form ['confirm_message'] = array(
    '#markup' => t('Are you sure you want to delete this featured page?')
  );
  $form ['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array(
      'attributes' => array(
        'class' => 'btn btn-primary'
      )
    )),
    '#prefix' => '<div class="submit_btn_bottom">',
  );
  $form ['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#submit' => array(
      'featured_page_node_delete_form_submit'
    ),
    '#attributes' => array('class' => array(
        'btn btn-primary'
    )),
    '#suffix' => '</div>',
  );
  $form ['nid'] = array(
    '#type' => 'hidden',
    '#value' => $nid
  );
  return $form;
}

/**
 * Submit function for Node delete.
 */
function featured_page_node_delete_form_submit($form, &$form_state) {
  $nid = $form_state ['values'] ['nid'];
  node_delete($nid);
  db_delete('mobilizer_featured_page')->condition('nid', $nid)->execute();
  drupal_goto('admin/mobilizer/featured-pages');
}

/**
 * Function to create the menu link of newly added pages.
 * @return menu link id.
 */
function mobilizer_create_custom_page_links($link = array()) {
  $item = '';
  $mid = '';
  $item = array(
    'link_path' => $link ['link_path'],
    'link_title' => $link ['link_title'],
    'menu_name' => $link ['menu_name'],
    'weight' => $link ['weight'],
    'expanded' => $link ['expanded']
  );
// Look the table first if the data does exist
  $exists = db_query("SELECT mlid from {menu_links} WHERE link_title=:link_title AND link_path=:link_path", array(
    ':link_title' => $link ['link_title'],
    ':link_path' => $link ['link_path']
    ))->fetchField();
// Save the record if the data does not exist
  if (!$exists) {
    $mid = menu_link_save($item);
  }

  return $mid;
}
