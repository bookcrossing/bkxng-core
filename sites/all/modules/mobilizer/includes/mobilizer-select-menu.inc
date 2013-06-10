<?php
/**
 * @file
 * The Menu system, which controls the flow of mobile site.
 *
 * Contains functions to manipulate the web menu to mobile menu.
 */

/**
* Provide a single block from the administration menu as a page.
*
* This function is often a destination for these blocks.
* For example, 'admin/structure/types' needs to have a destination to be valid
* in the Drupal menu system, but too much information there might be
* hidden, so we supply the contents of the block.
*
* @return
*   The output HTML.
*/
function mobilizer_select_menu_admin_settings_page() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_select_menu_admin_settings");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}
function mobilizer_select_menu_admin_settings($form, &$form_state) {
  global $base_url;
  $default_value = NULL;
  $result = db_query("SELECT * FROM {menu_custom} ORDER BY title", array(), array(
      'fetch' => PDO::FETCH_ASSOC
  ));
//$menu_options['select'] = 'Select';
  foreach ( $result as $menu ) {
    if (($menu ['menu_name'] != 'navigation') && ($menu ['menu_name'] != 'user-menu') && ($menu ['menu_name'] != 'management') && ($menu ['menu_name'] != 'menu-website-mobilizer')) {
      $menu_options [$menu ['menu_name']] = $menu ['title'];
    }
  }
  if (variable_get('mobilizer_default_menu', NULL) != '') {
    $default_value = variable_get('mobilizer_default_menu', NULL);
    variable_set('mobilizer_default_menu_name', $menu_options [$default_value]);
  }
  $form ['menu_list'] ['copy_menu'] = array(
      '#type' => 'select',
      '#options' => $menu_options,
      '#title' => t('Select Menu'),
      '#default_value' => $default_value,
      '#required' => 'TRUE',
      '#ajax' => array(
          'event' => 'change',
          'callback' => 'mobilizer_select_menu_details',
          'wrapper' => 'menu-items-wrapper',
          'method' => 'replace'
      )
  )
  ;
  $form ['menu_list'] ['apply'] = array(
      '#type' => 'submit',
      '#value' => t('Apply'),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary apply'
          )
      ),
      '#suffix' => '<div class="clear"></div>'
  );
  $form ['menu_list'] ['wrapper'] = array(
      '#prefix' => '<div id="menu-items-wrapper" class="menu_confi">',
      '#suffix' => '</div>',
      '#after_build' => array(
          'mobilizer_get_default_data'
      )
  );
  $form ['menu_list'] ['menu_selected'] = array(
      '#type' => 'hidden',
      '#attributes' => array(
          'id' => 'menu_selected'
      ),
      '#value' => $default_value
  );
  $form ['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed'),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary'
          ),
          'onclick' => 'javascript:if( jQuery("#edit-copy-menu").val() != jQuery("#menu_selected").val() ){ if(confirm("Your Selected Menu has not been applied. Your changes shall be lost if clicked on continue. Click Cancel to stay on same page and \"Apply\" before continue.")){return true;}else{return false;}}'
      ),
      '#prefix' => '<div class="submit_btn_bottom">',
      '#suffix' => '</div>'
  );
// $form['#theme'] = 'start_mobilizer_form';
//$form = mobilizer_iframe_include_form_preview($form , $base_url);
//javascript:if(confirm("Are you sure to proceed with the current selection?"))return true;else return false;
  return $form;
}
/**
* Function to load the default menu items creted @ Start Mobilizer Step
*/
function mobilizer_get_default_data() {
  $form = array();
  $default_menu = variable_get('mobilizer_default_menu', NULL);
  $default_value ['values'] ['copy_menu'] = $default_menu;
  $default_value ['triggering_element'] ['#options'] = array(
      $default_menu => variable_get('mobilizer_default_menu_name', NULL)
  );
  return mobilizer_select_menu_details($form, $default_value);
}
/**
* @author Bhupendra Singh
* @param $form $form_state
* @return html grid with menu items
*/
function mobilizer_select_menu_details($form, $form_state) {
// Get the menu item of the selected menu from dropdown
  $parrent_menu_items = menu_tree_page_data($form_state ['values'] ['copy_menu'], $max_depth = 1, $only_active_trail = FALSE);
  $total = count($parrent_menu_items);
// Output grid's header title
  $header_table_edit = array(
      array(
          'data' => t('Title')
      )
  );
// create the table rows for each item in selected menu
  foreach ( $parrent_menu_items as $key => $value ) {
    $rows_table_edit [] = array(
        array(
            'data' => t($value ['link'] ['link_title'])
        )
    );
  }
// create $caption with Menu value 'Menu items for <menu name>'
  if ($form_state ['values'] ['copy_menu'] != 'select') {
    $caption_table_edit = t('Menu Items for ' . $form_state ['triggering_element'] ['#options'] [$form_state ['values'] ['copy_menu']]);
  }
  else {
    $header_table_edit = array(
        array(
            'data' => t('')
        )
    );
    $caption_table_edit = '';
  }
//return the grid of menu items	
  return array(
      '#markup' => '<div id="menu-items-wrapper" class="menu_confi">' . theme('table', array(
          'header' => $header_table_edit,
          'rows' => $rows_table_edit,
          'caption' => $caption_table_edit,
          'sticky' => FALSE,
          'empty' => t('No menu items found for the selection!')
      )) . theme('pager') . '</div>'
  );
}
function mobilizer_select_menu_admin_settings_submit($form, &$form_state) {
// Create new menu.
// Begin code taken from menu_edit_menu_submit().
//$menu['i18n_mode'] = I18N_MODE_MULTIPLE;// throwing notice
  $path = 'admin/structure/menu/manage/';
// Add 'menu-' to the menu name to help avoid name-space conflicts.
  if ($form_state ['input'] ['op'] == 'Apply') {
    $menu ['menu_name'] = 'menu-website-mobilizer';
    $menu ['title'] = t('Menu for theme mobilizer');
    $menu ['description'] = '';
    $link ['link_title'] = t('Menu for theme mobilizer');
    $link ['link_path'] = $path . $menu ['menu_name'];
    $link ['router_path'] = $path . '%';
    $link ['module'] = 'menu';
    $link ['plid'] = db_query("SELECT mlid FROM {menu_links} WHERE link_path = :link AND module = :module", array(
        ':link' => 'admin/structure/menu',
        ':module' => 'system'
    )) -> fetchField();
// Delete existing mobile menu
    menu_delete($menu);
    
    menu_link_save($link);
    menu_save($menu);
    
    $parrent_menu_items = menu_tree_page_data($form_state ['values'] ['copy_menu'], $max_depth = 1, $only_active_trail = FALSE);
    foreach ( $parrent_menu_items as $key => $value ) {
      $menu_item = array();
      $menu_item ['link_title'] = $value ['link'] ['link_title'];
      $menu_item ['link_path'] = $value ['link'] ['link_path'];
      $menu_item ['router_path'] = $value ['link'] ['router_path'];
      $menu_item ['mlid'] = 0;
      $menu_item ['module'] = 'menu';
      $menu_item ['has_children'] = 0;
      $menu_item ['options'] = $value ['link'] ['options'];
      $menu_item ['customized'] = 1;
//$menu_item['original_item'] = $value['link'][''];
      $menu_item ['description'] = '';
      $menu_item ['expanded'] = 0;
      $menu_item ['weight'] = $value ['link'] ['weight'];
      $menu_item ['hidden'] = 0;
      $menu_item ['plid'] = 0;
      $menu_item ['menu_name'] = $menu ['menu_name'];
      menu_link_save($menu_item);
    }
    if (db_query("SELECT 1 FROM {menu_custom} WHERE menu_name = :menu", array(
        ':menu' => $menu ['menu_name']
    )) -> fetchField()) {
      variable_set('mobilizer_selected_web_menu_item', $form_state ['values'] ['copy_menu']);
      variable_set('mobilizer_menu_exist', 1);
  //drupal_set_message(t('Your configuration has been saved.'));
    }
    else {
      drupal_set_message(t('There was an error saving the menu link.'), 'error');
    }
    
    db_delete('mobilizer_menu_icons') -> execute();
    db_delete('mobilizer_node') -> execute();
    _mobilizer_menu_blocks_save('menu-website-mobilizer');
    variable_set('mobilizer_default_menu', $form_state ['values'] ['copy_menu']);
    variable_set('mobilizer_menu_process', 1);
  }
  elseif ($form_state ['input'] ['op'] == 'Proceed') {
    variable_set('mobilizer_menu_process', 1);
    $form_state ['redirect'] = 'admin/mobilizer/select-menu-configure';
  }
}
function mobilizer_menu_overview_page() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_overview_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
* Form for editing an entire menu tree at once.
*
* Shows for one menu the menu links accessible to the current user and
* relevant operations.
*/
function mobilizer_menu_overview_form($form, &$form_state) {
  global $base_url;
  $menu ['menu_name'] = variable_get('mobilizer_menu', 'menu-website-mobilizer');
  
  global $menu_admin;
  $form ['#prefix'] = '<div class="menu_confi">';
  $form ['#suffix'] = '</div>';
//$form['#attached']['css'] = array(drupal_get_path('module', 'menu') . '/menu.css');
  $sql = "
SELECT m.load_functions, m.to_arg_functions, m.access_callback, m.access_arguments, m.page_callback, m.page_arguments, m.delivery_callback, m.title, m.title_callback, m.title_arguments, m.type, m.description, ml.*
FROM {menu_links} ml LEFT JOIN {menu_router} m ON m.path = ml.router_path
WHERE ml.menu_name = :menu
ORDER BY p1 ASC, p2 ASC, p3 ASC, p4 ASC, p5 ASC, p6 ASC, p7 ASC, p8 ASC, p9 ASC";
  $result = db_query($sql, array(
      ':menu' => $menu ['menu_name']
  ), array(
      'fetch' => PDO::FETCH_ASSOC
  ));
  $links = array();
  foreach ( $result as $item ) {
    $links [] = $item;
  }
//print_r($links); exit;
  $tree = menu_tree_data($links);
  $node_links = array();
  menu_tree_collect_node_links($tree, $node_links);
// We indicate that a menu administrator is running the menu access check.
  $menu_admin = TRUE;
  menu_tree_check_access($tree, $node_links);
  $menu_admin = FALSE;
  
  $form = array_merge($form, _mobilizer_menu_overview_tree_form($tree));
  $form ['#menu'] = $menu;
  
  if (element_children($form)) {
//echo "<pre>";
//print_r($form); die;
//$form['actions'] = array('#type' => 'actions');
    $form ['preview'] = array(
        '#type' => 'submit',
        '#value' => t('Preview'),
        '#attributes' => array(
            'class' => array(
                'btn btn-primary'
            )
        ),
        '#prefix' => '<div class="submit_btn_bottom">'
    );
    $form ['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Proceed'),
        '#attributes' => array(
            'class' => array(
                'btn btn-primary'
            )
        ),
        '#suffix' => '</div>'
    );
  }
  else {
    $form ['#empty_text'] = t('There are no menu links yet. <a href="@link">Add link</a>.', array(
        '@link' => url('admin/structure/menu/manage/' . $form ['#menu'] ['menu_name'] . '/add')
    ));
  }
  return $form;
}

/**
* Recursive helper function for menu_overview_form().
*
* @param $tree
*   The menu_tree retrieved by menu_tree_data.
*/
function _mobilizer_menu_overview_tree_form($tree) {
  $form = &drupal_static(__FUNCTION__, array(
      '#tree' => TRUE
  ));
  foreach ( $tree as $data ) {
    $title = '';
    $item = $data ['link'];
// Don't show callbacks; these have $item['hidden'] < 0.
    if ($item && $item ['hidden'] >= 0) {
      $mlid = 'mlid:' . $item ['mlid'];
      $form [$mlid] ['#item'] = $item;
      $form [$mlid] ['#attributes'] = $item ['hidden'] ? array(
          'class' => array(
              'menu-disabled'
          )
      ) : array(
          'class' => array(
              'menu-enabled'
          )
      );
      $form [$mlid] ['title'] ['#markup'] = t($item ['title']);
      $form [$mlid] ['hidden'] = array(
          '#type' => 'checkbox',
          '#title' => t('Enable @title menu link', array(
              '@title' => $item ['title']
          )),
          '#title_display' => 'invisible',
          '#default_value' => ! $item ['hidden']
      );
      $form [$mlid] ['weight'] = array(
          '#type' => 'weight',
          '#delta' => 50,
          '#default_value' => $item ['weight'],
          '#title_display' => 'invisible',
          '#title' => t('Weight for @title', array(
              '@title' => $item ['title']
          ))
      );
      $form [$mlid] ['mlid'] = array(
          '#type' => 'hidden',
          '#value' => $item ['mlid']
      );
      $form [$mlid] ['plid'] = array(
          '#type' => 'hidden',
          '#default_value' => $item ['plid']
      );
// Build a list of operations.
      $operations = array();
      $operations ['edit'] = array(
          '#type' => 'link',
          '#title' => t('edit'),
          '#href' => 'admin/mobilizer/menu/item/' . $item ['mlid'] . '/edit'
      );
// Set the reset column.
      if ($item ['module'] == 'system' && $item ['customized']) {
        $operations ['reset'] = array(
            '#type' => 'link',
            '#title' => t('reset'),
            '#href' => 'admin/structure/menu/item/' . $item ['mlid'] . '/reset'
        );
      }
      $form [$mlid] ['operations'] = $operations;
    }
    
    if ($data ['below']) {
      _mobilizer_menu_overview_tree_form($data ['below']);
    }
  }
  return $form;
}

/**
* Submit handler for the menu overview form.
*
* This function takes great care in saving parent items first, then items
* underneath them. Saving items in the incorrect order can break the menu tree.
*
* @see menu_overview_form()
*/
function mobilizer_menu_overview_form_submit($form, &$form_state) {
// When dealing with saving menu items, the order in which these items are
// saved is critical. If a changed child item is saved before its parent,
// the child item could be saved with an invalid path past its immediate
// parent. To prevent this, save items in the form in the same order they
// are sent by $_POST, ensuring parents are saved first, then their children.
// See http://drupal.org/node/181126#comment-632270
  $order = array_flip(array_keys($form_state ['input'])); // Get the $_POST order.
  $form = array_merge($order, $form); // Update our original form with the new order.
  $updated_items = array();
  $fields = array(
      'weight',
      'plid'
  );
  foreach ( element_children($form) as $mlid ) {
    if (isset($form [$mlid] ['#item'])) {
      $element = $form [$mlid];
// Update any fields that have changed in this menu item.
      foreach ( $fields as $field ) {
        if ($element [$field] ['#value'] != $element [$field] ['#default_value']) {
          $element ['#item'] [$field] = $element [$field] ['#value'];
          $updated_items [$mlid] = $element ['#item'];
        }
      }
// Hidden is a special case, the value needs to be reversed.
      if ($element ['hidden'] ['#value'] != $element ['hidden'] ['#default_value']) {
// Convert to integer rather than boolean due to PDO cast to string.
        $element ['#item'] ['hidden'] = $element ['hidden'] ['#value'] ? 0 : 1;
        $updated_items [$mlid] = $element ['#item'];
      }
    }
  }

// Save all our changed items to the database.
  foreach ( $updated_items as $item ) {
    $item ['customized'] = 1;
    menu_link_save($item);
  }
  if ($form_state ['input'] ['op'] != 'Preview') {
//drupal_set_message(t('Your configuration has been saved.'));
    $form_state ['redirect'] = 'admin/mobilizer/featured-pages';
  }
}

/**
* Returns HTML for the menu overview form into a table.
*
* @param $variables
*   An associative array containing:
*   - form: A render element representing the form.
*
* @ingroup themeable
*/
function theme_mobilizer_menu_overview_form($variables) {
  $form = $variables ['form'];

//drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');
  
  $header = array(
      t('Menu Title'),
      array(
          'data' => t('Enabled')
      ),
      t('Weight'),
      array(
          'data' => t('Edit'),
          'colspan' => '3'
      )
  );
  
  $rows = array();
  
  foreach ( element_children($form) as $mlid ) {
    if (isset($form [$mlid] ['hidden'])) {
      $element = &$form [$mlid];
// Build a list of operations.
      $operations = array();
      $element ['operations'] ['edit'] ['#attributes'] = array(
          'class' => array(
              'edit'
          ),
          'title' => 'edit'
      );
//$element['operations']['edit_page']['#attributes'] = array('class' => array('edit-page'));
//unset($element['operations']['delete']);
//$element['operations']['preview']= array( '#type' => 'link','#title'=>'Preview','#href' => '#','#attributes' => array('class' => array('preview-page')));
      foreach ( element_children($element ['operations']) as $op ) {
        $operations [] = array(
            'data' => drupal_render($element ['operations'] [$op]),
            'class' => array(
                'menu-operations'
            )
        );
      }
      while ( count($operations) < 2 ) {
        $operations [] = '';
      }
// Add special classes to be used for tabledrag.js.
      $element ['plid'] ['#attributes'] ['class'] = array(
          'menu-plid'
      );
      $element ['mlid'] ['#attributes'] ['class'] = array(
          'menu-mlid'
      );
      $element ['weight'] ['#attributes'] ['class'] = array(
          'menu-weight'
      );

// Change the parent field to a hidden. This allows any value but hides the field.
      $element ['plid'] ['#type'] = 'hidden';
      
      $row = array();
      $row [] = theme('indentation', array(
          'size' => $element ['#item'] ['depth'] - 1
      )) . drupal_render($element ['title']);
      $row [] = array(
          'data' => drupal_render($element ['hidden']),
          'class' => array(
              'menu-enabled'
          )
      );
      $row [] = drupal_render($element ['weight']) . drupal_render($element ['plid']) . drupal_render($element ['mlid']);
      unset($operations [1]);
      $row = array_merge($row, $operations);
      
      $row = array_merge(array(
          'data' => $row
      ), $element ['#attributes']);
      $row ['class'] [] = 'draggable';
      $rows [] = $row;
    }
  }
  $output = '';
  if (empty($rows)) {
    $rows [] = array(
        array(
            'data' => $form ['#empty_text'],
            'colspan' => '7'
        )
    );
  }
  $output .= theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(
          'id' => 'menu-overview'
      )
  ));
  hide($form ['iframe']);
  $output .= drupal_render_children($form);
//  $output_final = website_mobilizer_themer($output,render($form['iframe']));
  

  return $output;
}
function mobilizer_menu_edit_item_page($item) {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_edit_item", $item);
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
* Menu callback; Build the menu link editing form.
*/
function mobilizer_menu_edit_item($form, &$form_state, $item) {
  global $base_url;
  drupal_add_css(drupal_get_path('module', 'mobilizer') . '/css/mobilizer.css', array(
      'group' => CSS_DEFAULT,
      'every_page' => FALSE
  ));
  $form ['link_mlid'] = array(
      '#type' => 'value',
      '#value' => $item ['mlid']
  );
  $form ['link_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Menu Title'),
      '#default_value' => $item ['link_title'],
      '#description' => t('The text to be used for this link in the menu.'),
      '#required' => TRUE
  );
  
  $images = _mobilizer_get_menu_icons();
  $default_icon = _mobilizer_get_default_menu_icon($item ['link_title']);
//get_menu_icons_menu_id($menu_id, $menu_title);
  $existing_icon = db_query("SELECT icon_id FROM {mobilizer_menu_icons} WHERE mlid = :mlid", array(
      ':mlid' => $item ['mlid']
  )) -> fetchField();
  if ($existing_icon)
    $default_icon = $existing_icon;
  
  $form ['link_icon'] = array(
      '#type' => 'radios',
      '#options' => $images,
      '#title' => t('Choose a menu icon'),
      '#default_value' => $default_icon,
      '#required' => 'TRUE'
  );
  
  $form ['actions'] ['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed'),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary'
          )
      ),
      '#prefix' => '<div class="edit_menu_icon">',
      '#suffix' => '</div>'
  );
  return $form;
}
function mobilizer_menu_edit_item_submit($form, &$form_state) {
  $mlid = $form_state ['values'] ['link_mlid'];
  $menu_item_load = menu_link_load($mlid);
  $menu_item_load ['link_title'] = $form_state ['values'] ['link_title'];
  menu_link_save($menu_item_load);
  
  db_delete('mobilizer_menu_icons') -> condition('mlid', $mlid) -> execute();
  
  db_insert('mobilizer_menu_icons') ->   // Table name no longer needs {}
fields(array(
      'mlid' => $mlid,
      'icon_id' => $form_state ['values'] ['link_icon']
  )) -> execute();

//variable_set('menu_icon_' . $mlid, $form_state['values']['link_icon']);
  mobilizer_set_messages('Configuration saved successfully', 'success');
  $form_state ['redirect'] = 'admin/mobilizer/select-menu-configure';
}
/**
* Menu callback; Check access and present a confirm form for deleting a menu link.
*/
function mobilizer_menu_item_delete_page($item) {
// Links defined via hook_menu may not be deleted. Updated items are an
// exception, as they can be broken.
  if ($item ['module'] == 'system' && ! $item ['updated']) {
    drupal_access_denied();
    return;
  }
  return drupal_get_form('mobilizer_menu_item_delete_form', $item);
}

/**
* Build a confirm form for deletion of a single menu link.
*/
function mobilizer_menu_item_delete_form($form, &$form_state, $item) {
  $form ['#item'] = $item;
  return confirm_form($form, t('Are you sure you want to delete the custom menu link %item?', array(
      '%item' => $item ['link_title']
  )), 'admin/structure/menu/manage/' . $item ['menu_name']);
}

/**
* Process menu delete form submissions.
*/
function mobilizer_menu_item_delete_form_submit($form, &$form_state) {
  $item = $form ['#item'];
  menu_link_delete($item ['mlid']);
  $t_args = array(
      '%title' => $item ['link_title']
  );
  drupal_set_message(t('The menu link %title has been deleted.', $t_args));
  watchdog('menu', 'Deleted menu link %title.', $t_args, WATCHDOG_NOTICE);
  $form_state ['redirect'] = 'admin/website-mobilizer/select-menu-configure';
}

