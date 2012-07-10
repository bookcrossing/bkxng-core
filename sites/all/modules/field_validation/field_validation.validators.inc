<?php

/**
 * @file
 * Provides validation functionality and hooks
 */

/**
 * Implements hook_field_validation_validators().
 *
 * This function returns an array of validators, in the validator key => options array form.
 * Possible options:
 * - name (required): name of the validator
 * - field types (required): defines which field types can be validated by this validator. Specify 'all' to allow all types
 * - custom_error (optional): define whether a user can specify a custom error message upon creating the validation rule.
 * - custom_data (optional): define whether custom data can be added to the validation rule
 * - description (optional): provide a descriptive explanation about the validator
 */
function field_validation_field_validation_validators() {
  return array(
    'regex' => array(
      'name' => "Regular expression",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Regex code'),
        'description' => t('Specify regex code to validate the user input against.'),
      ),
      'description' => t("Validates user-entered text against a specified regular expression. Note: don't include delimiters such as /."),
    ),
    'numeric' => array(
      'name' => "Numeric values",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Specify numeric validation range'),
        'description' => t('Optionally specify the minimum-maximum range to validate the user-entered numeric value against.') . ' ' . t('Usage') . ':'
        . theme('item_list', array('items' => array(t('empty: no value validation'), t('"100": greater than or equal to 100'), t('"|100": less than or equal to 100 (including negative numbers)'), t('"0|100": greater than or equal to 0 &amp; less than or equal to 100'), t('"10|100": greater than or equal to 10 &amp; less than or equal to 100'), t('"-100|-10": greater than or equal to -100 &amp; less than or equal to -10')))),
        'required' => FALSE,
      ),
      'description' => t('Verifies that user-entered values are numeric, with the option to specify min and / or max values.'),
    ),
    'min_length' => array(
      'name' => "Minimum length",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Minimum number of characters'),
        'description' => t('Specify the minimum number of characters that have to be entered to pass validation.'),
      ),
      'description' => t('Verifies that a user-entered value contains at least the specified number of characters'),
    ),
    'max_length' => array(
      'name' => "Maximum length",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Maximum number of characters'),
        'description' => t('Specify the maximum number of characters that can be entered to pass validation.'),
      ),
      'description' => t('Verifies that a user-entered value contains at most the specified number of characters'),
    ),
    'min_words' => array(
      'name' => "Minimum number of words",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Minimum number of words'),
        'description' => t('Specify the minimum number of words that have to be entered to pass validation. Words are defined as strings of letters separated by spaces.')
      ),
      'description' => t('Verifies that a user-entered value contains at least the specified number of words'),
    ),		
    'max_words' => array(
      'name' => "Maximum number of words",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Maximum number of words'),
        'description' => t('Specify the maximum number of words that have to be entered to pass validation. Words are defined as strings of letters separated by spaces.')
      ),
      'description' => t('Verifies that a user-entered value contains at most the specified number of words'),
    ),
    'plain_text' => array(
      'name' => "Plain text (disallow tags)",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'description' => t("Verifies that user-entered data doesn't contain HTML tags"),
    ),
    'must_be_empty' => array(
      'name' => "Must be empty",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'description' => t('Verifies that a specified textfield remains empty - Recommended use case: used as an anti-spam measure by hiding the element with CSS'),
    ),
    'blacklist' => array(
      'name' => "Words blacklist",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Blacklisted words'),
        'description' => t('Specify illegal words, seperated by commas. Make sure to escape reserved regex characters with an escape (\) character.'),
      ),
      'description' => t("Validates that user-entered data doesn't contain any of the specified illegal words"),
    ),
    'select_min' => array(
      'name' => "Minimum number of selections required",
      'field_types' => array(
        'list_integer',
        'list_float',
        'list_text',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Minimum number of selections'),
        'description' => t('Specify the minimum number of options a user should select.'),
      ),
      'description' => t('Forces the user to select at least a defined number of options from the specified list field'),
    ),
    'select_max' => array(
      'name' => "Maximum number of selections allowed",
      'field_types' => array(
        'list_integer',
        'list_float',
        'list_text',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Maximum number of selections'),
        'description' => t('Specify the maximum number of options a user can select.'),
      ),
      'description' => t('Forces the user to select at most a defined number of options from the specified list field'),
    ),
    'select_exact' => array(
      'name' => "Exact number of selections required",
      'field_types' => array(
        'list_integer',
        'list_float',
        'list_text',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Number of selections'),
        'description' => t('Specify how many options a user can select.'),
      ),
      'description' => t('Forces the user to select exactly the defined number of options from the specified list field'),
    ),
    'unique' => array(
      'name' => "Unique values",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Scope of unique values'),
        'description' => t('Specify the scope of Unique values, support: global, entity, bundle.'),
      ),
      'description' => t('Verifies that all values are unique in current entity or bundle.'),
    ),
   'match_another_field' => array(
      'name' => "Must match another field",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Arguments of another field'),
        'description' => t('Specify the arguments of another field, support: entity_type, field_name, bundle, column, is_property. for example,entity_type=node&bundle=article&column=title&is_property=TRUE, entity_type=user&column=name&is_property=TRUE.').t('It also support argument "reverse", which means must not match the field if you set this argument.'),
      ),
      'description' => t("Validates that user-entered data matches another field or entity property"),
    ),
    'specific_value' => array(
      'name' => "Specific value(s)",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('(Key) value'),
        'description' => t('Specify the specific value(s) you want the field to contain. Separate multiple options by a comma. For fields that have keys, use the key value instead.'),
      ),
      'description' => t('Verifies that the specified field contains a defined value'),
    ),
    'oneofseveral' => array(
      'name' => "Require at least one of several fields",
      'field_types' => array(
        'textfield',
        'textarea',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('group name'),
        'description' => t('Specify the group name for those fields, it should be the same across those fields. Validation rules with the same group name work together.'),
      ),
      'description' => t('Forces the user to specify / select at least one of several fields'),
    ),
    'equal_values' => array(
      'name' => "Equal values on multiple fields",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('group name'),
        'description' => t('Specify the group name for those fields, it should be the same across those fields. Validation rules with the same group name work together.'),
      ),
      'description' => t('Verifies that all specified fields contain equal values'),
    ),
    'unique_values' => array(
      'name' => "Unique values on multiple fields",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('group name'),
        'description' => t('Specify the group name for those fields, it should be the same across those fields. Validation rules with the same group name work together.'),
      ),
      'description' => t('Verifies that all specified fields contain unique values'),
    ),
    'custom' => array(
      'name' => "Custom PHP function",
      'field_types' => array(
        'textfield',
      ),
      'custom_error' => TRUE,
      'custom_data' => array(
        'label' => t('Function name'),
        'description' => t('Specify the custom PHP function name for this field. It contains one argument, $variables, for example, mymodule_validate_myfield($variables). Then you could enter its name at here: mymodule_validate_myfield'),
      ),
      'description' => t('Validate current field using custom PHP function'),
    ),		
  );
}

/**
 * Implements hook_field_validation_validate().
 */
function field_validation_field_validation_validate($validator_name, $rule, $entity, $langcode, $items, &$errors) {
  if(!empty($items)){
    switch ($validator_name) {
     
      case "regex":
        mb_regex_encoding('UTF-8');
        $regex = $rule['data'];
				
        foreach($items as $delta => $item){
          if($item[$rule['col']] != '' && (!mb_ereg("$regex", $item[$rule['col']]))){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }

        break;
				
      case "numeric":
        $num_range = _field_validation_numeric_check_data($rule['data']);
        foreach($items as $delta => $item){
          if($item[$rule['col']] != ''){
            $error_flag = FALSE;
            if(!is_numeric($item[$rule['col']])){
              $error_flag = TRUE;
            }
            if(isset($num_range['min']) && $item[$rule['col']] < $num_range['min']){
              $error_flag = TRUE;
            }
            if(isset($num_range['max']) && $item[$rule['col']] > $num_range['max']){
              $error_flag = TRUE;
            }
            if($error_flag){
              $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
              $message = t($rule['error_message']);
              form_set_error($error,$message);
            }
          }
        }
        break;
      case "min_length":
        $min_length = $rule['data'];
        foreach($items as $delta => $item){
          if($item[$rule['col']] != '' && (drupal_strlen($item[$rule['col']]) < $min_length)){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
        break;				
      case "max_length":
        $max_length = $rule['data'];
        foreach($items as $delta => $item){
          if($item[$rule['col']] != '' && (drupal_strlen($item[$rule['col']]) > $max_length)){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
        break;
      case "min_words":
        $min_words = $rule['data'];
        foreach($items as $delta => $item){
          $value = $item[$rule['col']];  
          $count = count(explode(' ', trim(preg_replace('/\s+/', ' ', str_replace('&nbsp;', ' ', (strip_tags(str_replace('<', ' <', $value))))))));		
          if($value != '' && ($count < $min_words)){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);						
          }
        }
		break;
      case "max_words":
        $max_words = $rule['data'];
        foreach($items as $delta => $item){
          $value = $item[$rule['col']];  
          $count = count(explode(' ', trim(preg_replace('/\s+/', ' ', str_replace('&nbsp;', ' ', (strip_tags(str_replace('<', ' <', $value))))))));
          if($value != '' && ($count > $max_words)){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);							
          }
        }
        break;
      case "plain_text":
        foreach($items as $delta => $item){
          if($item[$rule['col']] != '' && (strcmp($item[$rule['col']], strip_tags($item[$rule['col']])))){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
        break;
      case "must_be_empty":
        foreach($items as $delta => $item){
          if($item[$rule['col']]){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
        break;
      case "blacklist":
        $blacklist = explode(',', $rule['data']);
        $blacklist = array_map('trim', $blacklist);
        $blacklist_regex = implode('|', $blacklist);
        foreach($items as $delta => $item){
          if ($item[$rule['col']] != '' && (preg_match("/$blacklist_regex/i", $item[$rule['col']]))) {
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
		break;
      case "select_min":
        $min_selections = $rule['data'];
        //$selected = 0;
        $total_items = count($items);

        if($total_items < $min_selections){
          $errors[$rule['field_name']][$langcode][0][] = array(
            'error' => 'select_min_'.$rule['ruleid'],
            'message' => t($rule['error_message']),
          );
        }
        break;
      case "select_max":
        $max_selections = $rule['data'];
        //$selected = 0;
        $total_items = count($items);

        if($total_items > $max_selections){
          $errors[$rule['field_name']][$langcode][0][] = array(
            'error' => 'select_max_'.$rule['ruleid'],
            'message' => t($rule['error_message']),
          );
        }
        break;
      case "select_exact":
        $allowed_selections = $rule['data'];
        //$selected = 0;
        $total_items = count($items);

        if($total_items != $allowed_selections){
          $errors[$rule['field_name']][$langcode][0][] = array(
            'error' => 'select_exact_'.$rule['ruleid'],
            'message' => t($rule['error_message']),
          );
        }
        break;
      case "unique":
        $scope = $rule['data'];
        $total_items = count($items);
        foreach($items as $delta => $item){
          $flag = FALSE;
					
          foreach ($items as $delta1 => $item1) {
            if($delta != $delta1){
              if($item[$rule['col']] == $item1[$rule['col']]){
                $flag = TRUE;
                break;
              }
            }
          }
					
          if(!$flag){
            $query = new EntityFieldQuery();
            if($scope == 'global'){
            }elseif($scope == 'entity'){
              $query->entityCondition('entity_type', $rule['entity_type']);
            }elseif($scope == 'bundle'){
              $query->entityCondition('entity_type', $rule['entity_type']);
              $query->entityCondition('bundle', $rule['bundle']);
            }
						
            list($id, $vid, $bundle) = entity_extract_ids($rule['entity_type'], $entity);
            if($rule['entity_type'] == 'user' && arg(0) =='user' && arg(2) =='edit' && empty($id)){
              $id = arg(1);
            }
            if(!empty($id)){
              $query->entityCondition('entity_id', $id, '!=');
            }
            $flag = (bool)$query->fieldCondition($rule['field_name'],$rule['col'],$item[$rule['col']])
                                ->range(0, 1)
                                ->count()
                                ->execute();
						
          }
					
          if($flag){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);						
          }
        }
        break;
				
      case "match_another_field":
        $arguments = $rule['data'];
        $arguments = explode('&',$rule['data']);
        $arg_array = array();
        //debug($arguments);
        foreach($arguments as $arg){
          $argument = explode('=',$arg);
          $name = trim($argument[0]);
          $value = trim($argument[1]);
          $arg_array[$name] = $value;
        }
        //debug($arg_array);
        foreach ($items as $delta => $item) {
          if(empty($item[$rule['col']])){
            continue;
          }
          $query = new EntityFieldQuery();
          if(!empty($arg_array['entity_type'])){
            $query->entityCondition('entity_type', $arg_array['entity_type']);
          }
          if(!empty($arg_array['bundle'])){
            $query->entityCondition('bundle', $arg_array['bundle']);
          }
          if(!empty($arg_array['is_property'])){
            if(!empty($arg_array['column'])){
              $query->propertyCondition($arg_array['column'],$item[$rule['col']]);
            }	
          }else{
            if(!empty($arg_array['field_name']) && !empty($arg_array['column'])){
              $query->fieldCondition($arg_array['field_name'], $arg_array['column'], $item[$rule['col']]);
            }
          }
          $flag = (bool)$query->range(0, 1)->count()->execute();
          if(!empty($arg_array['reverse'])){
            $flag = $flag ? FALSE : TRUE;
          }
          if(!$flag){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);	
          }
        }
        break;
      case "specific_value":
        $specific_values = explode(',',$rule['data']);
        $specific_values = array_map('trim', $specific_values);
        foreach ($items as $delta => $item) {
          if(empty($item[$rule['col']])){
            continue;
          }
          if(!in_array($item[$rule['col']], $specific_values)){
            $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
            $message = t($rule['error_message']);
            form_set_error($error,$message);
          }
        }
        break;
      case "oneofseveral":
        $flag = FALSE;
        $group_name = $rule['data'];
        $field_values = _field_validation_get_field_column_value($items, $rule['col']);
        $field_values = array_flip($field_values);
        if(count($field_values) > 0){
          $flag = TRUE;
        }
        if(!$flag){
          $other_group_rules = db_select('field_validation_rule','fvr')
                                 ->fields('fvr')
                                 ->condition('fvr.entity_type',$rule['entity_type'])
                                 ->condition('fvr.bundle',$rule['bundle'])
                                 ->condition('fvr.validator',$rule['validator'])
                                 ->condition('fvr.data',$group_name)
                                 ->condition('fvr.ruleid',$rule['ruleid'],'!=')
                                 ->execute()
                                 ->fetchAll();
					
          foreach($other_group_rules as $other_group_rule){
            $other_items = isset($entity->{$other_group_rule->field_name}[$langcode])?$entity->{$other_group_rule->field_name}[$langcode]: array();
            $other_field_values = _field_validation_get_field_column_value($other_items, $other_group_rule->col);
            $other_field_values = array_flip($other_field_values);
            if(count($other_field_values) > 0){
              $flag = TRUE;
              break;
            }
          }
        }

        if(!$flag){
          $error = $rule['field_name'].']['.$langcode.'][0]['.$rule['col'];
          $message = t($rule['error_message']);
          form_set_error($error,$message);
        }
        break;
      case "equal_values":
        $flag = FALSE;
        $group_name = $rule['data'];
        $other_group_rules = db_select('field_validation_rule','fvr')
                               ->fields('fvr')
                               ->condition('fvr.entity_type',$rule['entity_type'])
                               ->condition('fvr.bundle',$rule['bundle'])
                               ->condition('fvr.validator',$rule['validator'])
                               ->condition('fvr.data',$group_name)
                               ->condition('fvr.ruleid',$rule['ruleid'],'!=')
                               ->execute()
                               ->fetchAll();
        foreach ($items as $delta => $item) {
          foreach($other_group_rules as $other_group_rule){
            if(isset($entity->{$other_group_rule->field_name}[$langcode][$delta])){
              if($item[$rule['col']] == $entity->{$other_group_rule->field_name}[$langcode][$delta][$other_group_rule->col]){	
              }else{
                $flag = TRUE;
                break;
              }
            }else{
              $flag = TRUE;
              break;
            }
          }
          if($flag){
            break;
          }
        }
        if($flag){
          $error = $rule['field_name'].']['.$langcode.'][0]['.$rule['col'];
          $message = t($rule['error_message']);
          form_set_error($error,$message);
        }

        break;
      case "unique_values":
        $flag = FALSE;
        $group_name = $rule['data'];
        $total_items = count($items);
        $field_values = _field_validation_get_field_column_value1($items, $rule['col']);
        $field_values = array_flip($field_values);
        if(count($field_values) < $total_items){
          $flag = TRUE;
        }
        if(!$flag){
          $other_group_rules = db_select('field_validation_rule','fvr')
                                 ->fields('fvr')
                                 ->condition('fvr.entity_type',$rule['entity_type'])
                                 ->condition('fvr.bundle',$rule['bundle'])
                                 ->condition('fvr.validator',$rule['validator'])
                                 ->condition('fvr.data',$group_name)
                                 ->condition('fvr.ruleid',$rule['ruleid'],'!=')
                                 ->execute()
                                 ->fetchAll();
          foreach($other_group_rules as $other_group_rule){
            if(isset($entity->{$other_group_rule->field_name}[$langcode])){
              $other_items = $entity->{$other_group_rule->field_name}[$langcode];
              $other_field_values = _field_validation_get_field_column_value1($other_items, $other_group_rule->col);
              $other_field_values = array_flip($other_field_values);
              $total_items = $total_items + count($other_items);
							
              $field_values = $field_values + $other_field_values;
              if(count($field_values) < $total_items){
                $flag = TRUE;
                break;
              } 
            }
          }
        }
        if($flag){
          $error = $rule['field_name'].']['.$langcode.'][0]['.$rule['col'];
          $message = t($rule['error_message']);
          form_set_error($error,$message);					
        }
        break;
				
      case "custom":
        $custom_function_name = $rule['data'];	
        if (function_exists($custom_function_name)) {				
          foreach ($items as $delta => $item) {
            $flag = TRUE;
            $variables = array();
            $variables['value'] = $item[$rule['col']];
            $variables['item'] = $item;
            $variables['delta'] = $delta;
            $variables['items'] = $items;
            $variables['rule'] = $rule;
            $variables['validator_name'] = $validator_name;
            $variables['langcode'] = $langcode;
            $variables['entity'] = $entity;
						
            $flag = $custom_function_name($variables);
            if(!$flag){
              $error = $rule['field_name'].']['.$langcode.']['.$delta.']['.$rule['col'];
              $message = t($rule['error_message']);
              form_set_error($error,$message);							
            }
          }
        }
        break;				
    }
  }
}

/**
 * Get a list of validator definitions
 */
function field_validation_get_validators() {
  $validators = module_invoke_all("field_validation_validators");
  // let modules use hook_field_validator_alter($validators) to change validator settings
  drupal_alter('field_validator', $validators);
  return $validators;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function field_validation_get_validators_selection() {
  $selection = array();
  $validators = field_validation_get_validators();
  if ($validators) {
    foreach($validators as $validator_key => $validator_info){
      $selection[$validator_key] = $validator_info['name'];
    }
  }
  return $selection;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function field_validation_get_validator_info($validator_key) {
  $validators = field_validation_get_validators();
  return $validators[$validator_key];
}

/**
 * Process the numeric value validation range that was provided in the numeric validator options
 */
function _field_validation_numeric_check_data($data) {
  $range = array('min' => NULL, 'max' => NULL);
  // if no value was specified, don't validate
  if($data == ''){
    return $range;
  }

  // If only one numeric value was specified, this is the min value
  if(is_numeric($data)){
    $range['min'] = (int) $data;
  }

  if(strpos($data, '|') !== FALSE){
    list($min, $max) = explode('|', $data);
    if ($min != '' && is_numeric($min)) {
      $range['min'] = (int) $min;
    }
    if($max != '' && is_numeric($max)){
      $range['max'] = (int) $max;
    }
  }
  return $range;
}

/**
 * helper function to get field value
 */
function _field_validation_get_field_column_value($items, $column ='value') {
  $field_values = array();
	foreach($items as $delta => $item){
	  if(isset($item[$column]) && $item[$column] !=''){
	    $field_values[] = $item[$column];
      }
	}
  return $field_values;
}

/**
 * helper function to get field value include empty string.
 */
function _field_validation_get_field_column_value1($items, $column ='value') {
  $field_values = array();
	foreach($items as $delta => $item){
	  if(isset($item[$column])){
	    $field_values[] = $item[$column];
      }
	}
  return $field_values;
}
