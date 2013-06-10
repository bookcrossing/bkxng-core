<?php

/**
* @file
* Mobiliser block editale
*/

/**
* Page callback for block content edit
* 
*/
function mobilizer_block_edit_page() {
 global $base_url;
 $render_array = drupal_get_form("mobilizer_block_edit");
 $output = drupal_render($render_array);
 return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}

function mobilizer_block_edit($form, &$form_state) {
 $mid = arg(6);
 global $base_url;
 $mblock_obj = db_select('mobilizer_node', 'nm')->fields('nm')->condition('mid', $mid)->execute()->fetchObject();

 $form['mlid'] = array('#type' => 'hidden', '#value' => $mblock_obj->mlid);
 $form['block_description'] = array(
   '#type' => 'textfield',
   '#title' => t('Title'),
   '#default_value' => $mblock_obj->block_description,
   '#size' => 60,
   '#maxlength' => 128,
 );

 $form['block_html'] = array(
   '#title' => t('Mobilizer Block Content'),
   '#type' => 'text_format',
   '#default_value' => $mblock_obj->node_block_html,
   '#format' => 'full_html',
 );
 $form['submit'] = array('#type' => 'submit',
   '#value' => t('Save'),
   '#attributes' => array('class' => array('btn btn-primary')),
   '#prefix' => '<div class="submit_btn_bottom">',
   '#suffix' => '</div>',
 );
 $form['cancel'] = array('#type' => 'submit',
   '#value' => t('Cancel'),
   '#attributes' => array('class' => array('btn btn-primary')),
   '#prefix' => '<div class="submit_btn_bottom">',
   '#suffix' => '</div>',
 );
 return $form;
}

/**
* Submit handler for block content editable.
*/
function mobilizer_block_edit_submit($form, &$form_state) {

 $mlid = $form_state['values']['mlid'];
 $mid = arg(6);
 $mlid = arg(4);

 if ($form_state['values']['op'] == 'Cancel') {
   $path = 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit';
   drupal_goto($path);
 }

 $block_html = $form_state['values']['block_html']['value'];
 $block_description = $form_state['values']['block_description'];
 if ($form_state['values']['op'] == 'Save') {
   $status = db_update('mobilizer_node')
           ->fields(array(
             'block_description' => $block_description,
             'node_block_html' => $block_html,
             'time' => REQUEST_TIME,
           ))
           ->condition('mid', $mid)
           ->execute();
   if ($status) {
     $mobilizer_message['message'] = '<div class="mob_success">Content updated</div>';
     variable_set('mobilizer_message', $mobilizer_message);
   }
 }
 $path = 'admin/mobilizer/select-menu-configure/page/' . $mlid . '/edit';
 drupal_goto($path);
}
