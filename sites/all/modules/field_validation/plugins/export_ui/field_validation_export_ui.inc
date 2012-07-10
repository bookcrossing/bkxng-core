<?php

/**
 * @file
 * A Ctools Export UI plugin for Field Validation rules.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'field_validation_rule',
  'access' => 'administer site configuration',
  'menu' => array(
    'menu item' => 'field_validation',
    'menu prefix' => 'admin/structure',
    'menu title' => 'Field Validation',
    'menu description' => 'Administer Field Validation rules.',
  ),

  'title singular' => t('rule'),
  'title plural' => t('rules'),
  'title singular proper' => t('Field Validation rule'),
  'title plural proper' => t('Field Validation rules'),
/*
  'handler' => array(
    'class' => 'ctools_export_ui',
   // 'parent' => 'ctools_export_ui',
  ),
	*/

  'form' => array(
    'settings' => 'field_validation_ctools_export_ui_form',
    'validate' => 'field_validation_ctools_export_ui_form_validate',
    'submit' => 'field_validation_ctools_export_ui_form_submit',
  ),
);

/**
 * Define the add/edit form of validation rule.
 */
function field_validation_ctools_export_ui_form(&$form, &$form_state) {
  ctools_include('export');
  $rule = $form_state['item'];
  $default_rulename = isset($rule->rulename) ? $rule->rulename : '';
  $default_entity_type = isset($rule->entity_type) ? $rule->entity_type : '';
  $default_bundle = isset($rule->bundle) ? $rule->bundle : '';
  $default_field_name = isset($rule->field_name) ? $rule->field_name : '';
  $default_col = isset($rule->col) ? $rule->col : '';
  $default_validator = isset($rule->validator) ? $rule->validator : '';
  $default_data = isset($rule->data) ? $rule->data : '';
  $default_error_message = isset($rule->error_message) ? $rule->error_message : '';
  //print debug($form_state);
  
  $default_rulename = isset($form_state['values']['rulename']) ? $form_state['values']['rulename'] : $default_rulename;
  $default_entity_type = isset($form_state['values']['entity_type']) ? $form_state['values']['entity_type'] : $default_entity_type;
  $default_bundle = isset($form_state['values']['bundle']) ? $form_state['values']['bundle'] : $default_bundle;
  $default_field_name = isset($form_state['values']['field_name']) ? $form_state['values']['field_name'] : $default_field_name;
  $default_col = isset($form_state['values']['col']) ? $form_state['values']['col'] : $default_col;
  $default_validator = isset($form_state['values']['validator']) ? $form_state['values']['validator'] : $default_validator;
  $default_data = isset($form_state['values']['data']) ? $form_state['values']['data'] : $default_data;
  $default_error_message = isset($form_state['values']['error_message']) ? $form_state['values']['error_message'] : $default_error_message;
  //print debug($rule);
  
  $form['rulename'] = array(
    '#type' => 'textfield',
    '#title' => t('Rule name'),
    '#default_value' => $default_rulename,
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 255,
   // '#weight' => 1,
  );
  /*
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => $rule->name,
	'#machine_name' => array(
	  'exists' => 'field_validation_rule_load',
	  'source' => array('rulename'),
	),

  );
*/
  $entity_type_options = array(
    '' => 'Choose an entity type',
  );
  $entity_types = entity_get_info();
  foreach ($entity_types as $key => $entity_type) {
    $entity_type_options[$key] = $entity_type['label'];  
  }
  $form['entity_type'] = array(
    '#type' => 'select', 
	'#options' => $entity_type_options,
	'#title' => t('Entity type'),
    '#default_value' => $default_entity_type,
    '#required' => TRUE,
    '#ajax' => array(      
      'callback' => 'field_validation_entity_type_callback',      
      'wrapper' => 'validation-rule-wrapper-div',      
      'method' => 'replace',      
      'effect' => 'fade',    
    ),
  );


  $bundle_options = array(
    '' => 'Choose a bundle',
  );
 // print debug($entity_types['node']);
  $bundles = !empty($entity_types[$default_entity_type]['bundles']) ? $entity_types[$default_entity_type]['bundles'] : array();
 // $bundles = !empty($entity_types['node']['bundles']) ? $entity_types['node']['bundles'] : array();
 // print debug($default_entity_type);
  foreach ($bundles as $key => $bundle) {
    $bundle_options[$key] = $bundle['label'];  
  }
  $form['bundle'] = array(
    '#type' => 'select',
	'#options' => $bundle_options,
	'#title' => t('Bundle name'),
    '#default_value' => $default_bundle,
    '#required' => TRUE,
    '#prefix' => '<div id="bundle-wrapper-div">',
    '#suffix' => '</div>',
    '#ajax' => array(      
      'callback' => 'field_validation_bundle_callback',      
      'wrapper' => 'validation-rule-wrapper-div',      
      'method' => 'replace',      
      'effect' => 'fade',    
    ),
  );
  
  $field_name_options = array(
    '' => 'Choose a field',
  );
  $instances = array();
  if(!empty($default_entity_type) && !empty($default_bundle)){
    $instances = field_info_instances($default_entity_type, $default_bundle);
  }
  //$instances = field_info_instances('node', 'article');

  foreach ($instances as $key => $instance) {
    $field_name_options[$key] = $instance['label'];  
  }
  if(!in_array($default_field_name, array_keys($field_name_options))){
    $default_field_name = '';
  }
  $form['field_name'] = array(
    '#type' => 'select',
	'#options' => $field_name_options,
    '#title' => t('Field name'),
    '#default_value' => $default_field_name,
    '#required' => TRUE,
    '#prefix' => '<div id="field-name-wrapper-div">',
    '#suffix' => '</div>',
    '#ajax' => array(      
      'callback' => 'field_validation_field_name_callback',      
      'wrapper' => 'col-wrapper-div',      
      'method' => 'replace',      
      'effect' => 'fade',    
    ),
  ); 
  $field = field_info_field($default_field_name);
  //print debug($field);
  $col_options = array(
    '' => t('Choose a column'),
  );
  $columns = !empty($field['columns']) ? $field['columns'] : array();
  foreach ($columns as $key => $column) {
    $col_options[$key] = $key;  
  }
  if(!in_array($default_col, array_keys($col_options))){
    $default_col = '';
  }  
  $form['col'] = array(
    '#type' => 'select',
	'#options' => $col_options,
    '#title' => t('Column'),
    '#description' => t('A column defined in the hook_field_schema() of this field.'),
    '#default_value' => $default_col,
    '#required' => TRUE,
    '#weight' => 2,
    '#prefix' => '<div id="col-wrapper-div">',
    '#suffix' => '</div>',
  );

  $validator_options = array(
    '' => 'Choose a validator',
  );
  $validators = field_validation_get_validators();
  foreach ($validators as $validator_key => $validator_info) {
    $validator_options[$validator_key] = $validator_info['name'];
  }
  $form['validator'] = array(
    '#type' => 'select',
	'#options' => $validator_options,
    '#title' => t('Validator'),
    '#description' => t('A column defined in the hook_field_schema() of this field.'),
    '#default_value' => $default_validator,
    '#required' => TRUE,
    '#weight' => 3,
    '#ajax' => array(      
      'callback' => 'field_validation_validator_callback',      
      'wrapper' => 'data-wrapper-div',      
      'method' => 'replace',      
      'effect' => 'fade',    
    ),
  );
  
  $form['data'] = array(
    '#type' => 'textfield',
    '#title' => t('Config Data'),
    '#required' => FALSE,
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $default_data,
    '#weight' => 4,
    '#prefix' => '<div id="data-wrapper-div">',
    '#suffix' => '</div>',	
  );
  //$rule_validator = $validators[$default_validator];
  if (isset($validators[$default_validator]['custom_data']) && is_array($validators[$default_validator]['custom_data'])) {
    $required = isset($validators[$default_validator]['custom_data']['required']) ? $validators[$default_validator]['custom_data']['required'] : TRUE;
	$form['data']['#title'] = isset($validators[$default_validator]['custom_data']['label']) ? $validators[$default_validator]['custom_data']['label'] : t('Config Data');
	$form['data']['#description'] = isset($validators[$default_validator]['custom_data']['description']) ? $validators[$default_validator]['custom_data']['description'] : t('Config Data');
	$form['data']['#required'] = ($required !== FALSE) ? TRUE : FALSE;
  }
  
  $form['error_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom error message'),
    '#description' => t("Specify an error message that should be displayed when user input doesn't pass validation"),
    '#required' => TRUE,
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => $default_error_message,
    '#weight' => 5,
  );
   $form['#prefix'] = '<div id="validation-rule-wrapper-div">';
   $form['#suffix'] = '</div>';

}

/**
 * Validation handler for the validation rule add/edit form.
 */
function field_validation_ctools_export_ui_form_validate($form, &$form_state) {
/*
  if (!isset($form_state['values']['ffmpeg_wrapper'])) {
    form_set_error('ffmpeg_wrapper', t('The FFmpeg Wrapper configuration is missing.'));
  }
	*/
	
	$values = $form_state['values'];
//print debug($values);
/*
$rule = $form_state['item'];
if(!empty($form_state['values']['entity_type'])){
  $rule->entity_type = $form_state['values']['entity_type'];
}
if(!empty($form_state['values']['bundle'])){
  $rule->bundle = $form_state['values']['bundle'];
}
if(!empty($form_state['values']['entity_type'])){
  $rule->entity_type = $form_state['values']['entity_type'];
}
$form_state['item'] = $rule;
//print debug($form_state['item']);
*/
}

/**
 * Submit handler for the preset edit form.
 */
function field_validation_ctools_export_ui_form_submit($form, &$form_state) {
/*
  // Flatten and serialize the ffmpeg_wrapper settings array.
  $form_state['values']['ffmpeg_wrapper'] = serialize(_ffmpeg_converter_array_flatten($form_state['values']['ffmpeg_wrapper']));
*/
$values = $form_state['values'];
//print debug($values);
}

function field_validation_entity_type_callback($form,&$form_state){
 //drupal_set_message(print_r($form)); 
 //debug($form_state);

  return $form;
}

function field_validation_bundle_callback($form,&$form_state){

  return $form;
}

function field_validation_field_name_callback($form,&$form_state){
  return $form['col'];
}

function field_validation_validator_callback($form,&$form_state){
  return $form['data'];
}
