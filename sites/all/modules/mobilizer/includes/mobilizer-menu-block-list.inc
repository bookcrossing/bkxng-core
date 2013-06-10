<?php

/**
 * @file
 * Web block and page content list into mobilizer.
 */

/**
 * Page callback: block and page content list.
 */
function mobilizer_menu_block_list() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_list_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

/**
 * Page callback: block and page content list.
 */
function mobilizer_menu_block_list_form($form, &$form_state) {
  $mlid = arg(4);
  global $base_url;
  $blockobj = db_select('mobilizer_node', 'nm')
      ->fields('nm', array('mid', 'mlid', 'bid', 'block_description', 'block_title', 'weight', 'ebabled', 'type'))
      ->condition('mlid', $mlid)->orderBy('weight')
      ->execute()->fetchAll();
  $rows = array();
  if (count($blockobj)) {
    foreach ($blockobj as $block) {
      $form_type = array('form_block', 'form_page');
      if (in_array($block->type, $form_type)) {
        $block_desc = t($block->block_description . '[FROM]');
        $edit = '';
      }
      else {
        $block_desc = t($block->block_description);
        $edit = l(t('Edit'), 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit/' . $block->mid . '/blockedit', array('attributes' => array('class' => 'edit')));
      }

      $rows[$block->mid] = array(
        'description' => $block_desc,
        'action' => $edit,
        'ebabled' => $block->ebabled,
        'weight' => $block->weight,
      );
    }
  }
  else {
    $form['#empty_text'] = t('There are no block under this menu links yet.');
  }
  $form['#prefix'] = '<div class="menu_confi">';
  $form['#suffix'] = '</div>';
  $form['rearrange'] = array(
    '#prefix' => '<div id="red-dwarf">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'mobilizer_form_menu_block_list_form'
  );

  foreach ($rows as $key => $dataobj) {
    $form['rearrange'][$key]['description'] = array(
      '#markup' => $dataobj['description'],
    );
    $form['rearrange'][$key]['action'] = array(
      '#markup' => $dataobj['action'],
    );
    $form['rearrange'][$key]['ebabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $dataobj['ebabled'],
    );
    $form['rearrange'][$key]['weight'] = array(
      '#type' => 'textfield',
      '#default_value' => $dataobj['weight'],
      '#size' => 3,
      '#attributes' => array('class' => array('rank-weight')), // needed for table dragging
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed'),
    '#attributes' => array('class' => array('btn btn-primary')),
    '#prefix' => '<div class="submit_btn_bottom">',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * theme_form  hendler for menu_block_list_form.
 */
function theme_mobilizer_form_menu_block_list_form($vars) {
  $element = $vars['element'];
  drupal_add_tabledrag('menu_block_list_form', 'order', 'sibling', 'rank-weight'); // needed for table dragging
  $header = array(
    'description' => t('BLOCKS LIST'),
    'ebabled' => t('ENABLED'),
    'action' => t('EDIT'),
    'weight' => t('WEIGHT'),
  );

  $rows = array();
  foreach (element_children($element) as $key) {
    $row = array();

    $row['data'] = array();

    foreach ($header as $fieldname => $title) {
      $row['data'][] = drupal_render($element[$key][$fieldname]);
      $row['class'] = array('draggable'); // needed for table dragging
    }
    $rows[] = $row;
  }
  if (!count($rows)) {
    $header = array('description' => t('No PAGE'));
  }
  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => 'menu_block_list_form'), // needed for table dragging
    ));
}

/**
 * Submit handler for the block list.
 */
function mobilizer_menu_block_list_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == 'Proceed') {
    if (isset($form_state['values']['rearrange'])) {
      foreach ($form_state['values']['rearrange'] as $mid => $obj) {
        $update = db_update('mobilizer_node')
            ->fields(array(
              'ebabled' => $obj['ebabled'],
              'weight' => $obj['weight'],
            ))->condition('mid', $mid)->execute();
      }
      mobilizer_set_messages('Update successfully');
    }
    drupal_goto('admin/mobilizer/featured-pages');
  }
  else {
    mobilizer_set_messages('Invalid opration', 'error');
  }
}

/**
 * @param $nid
 *    $nid id contain menu mlid.
 * Page callback handler for delete block listmenu
 * Delete menu and it's under all blocks 
 */
function mobilizer_menu_block_list_delete_page($nid = NULL) {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_menu_block_list_delete_form", $nid);
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

function mobilizer_menu_block_list_delete_form($form, &$form_state, $nid = NULL) {
  $form['confirm_message'] = array(
    '#markup' => t('Are you sure you want to delete this menu page?'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Confirm'),
    '#submit' => array('menu_block_list_delete_form_submit')
  );
  $form['cancel'] = array(
    '#markup' => l(t('Cancel'), 'admin/mobilizer/featured-pages', array('attributes' => array('class' => 'btn btn-primary'))),
  );
  $form['mlid'] = array(
    '#type' => 'hidden',
    '#value' => $nid,
  );
  return $form;
}

/**
 * Submit hadnler for menu delete block list
 */
function mobilizer_menu_block_list_delete_form_submit($form, &$form_state) {
  $mlid = $form_state['values']['mlid'];
  menu_link_delete($mlid, $path = NULL);
  db_delete('mobilizer_node')
    ->condition('mlid', $mlid)
    ->execute();
  drupal_goto('admin/mobilizer/featured-pages');
}