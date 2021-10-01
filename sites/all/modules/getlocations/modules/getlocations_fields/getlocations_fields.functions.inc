<?php

/**
 * @file
 * getlocations_fields.functions.inc
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Supplies getlocations fields functions.
 */

// form elements


/**
 * @param string $default
 *
 * @param bool $distance
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_origin($default, $distance=FALSE) {
  $options = array(
    'nid_arg' => t("Node's Lat/Lon from views nid argument"),
    'uid_arg' => t("User's Lat/Lon from views uid argument"),
  );
  if (getlocations_get_vocabularies()) {
    $options += array('tid_arg' => t("Term's Lat/Lon from views tid argument"));
  }
  if (module_exists('comment')) {
    $options += array('cid_arg' => t("Comment's Lat/Lon from views cid argument"));
  }
  if ($distance) {
    $options += array('distance_arg' => t("Lat/Lon from views argument"));
  }
  if (module_exists('getlocations_gps')) {
    $options += array('gps' => t("Lat/Lon from Getlocations GPS"));
  }
  $options += array(
    'user' => t("User's Lat/Lon (blank if unset)"),
    'hybrid' => t("User's Lat/Lon (fall back to Static if unset)"),
    'static' => t('Static Lat/Lon'),
#    'tied' => t("Use Distance / Proximity filter"),
#    'postal' => t('Postal Code / Country'),
#    'postal_default' => t('Postal Code (assume default country)'),
    'php' => t('Use PHP code to determine Lat/Lon'),
    'search' => t('Google Autocomplete search'),
  );

  $element = array(
    '#type' => 'select',
    '#title' => t('Origin'),
    '#options' => $options,
    '#description' => t("The way the latitude/longitude is determined. If the origin has multiple locations the first will be used."),
    '#default_value' => $default,
  );
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_latitude($default, $ctools=TRUE) {
  $element = array(
    '#type' => 'textfield',
    '#title' => t('Latitude'),
    '#default_value' => $default,
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('hybrid', 'static'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_longitude($default, $ctools=TRUE) {
  $element = array(
    '#type' => 'textfield',
    '#title' => t('Longitude'),
    '#default_value' => $default,
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('hybrid', 'static'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_postal_code($default, $ctools=TRUE) {
  $element = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#default_value' => $default,
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('postal', 'postal_default'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_country($default, $title="", $ctools=TRUE) {
  if (empty($title)) {
    $title = t('Country');
  }
  $element = array(
    '#type' => 'select',
    '#title' => $title,
    '#options' => array('' => '') + getlocations_get_countries_list(),
    '#default_value' => $default,
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('postal'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_php_code($default, $ctools=TRUE) {
  $element = array(
    '#type' => 'textarea',
    '#title' => t('PHP code for latitude, longitude'),
    '#default_value' => $default,
    '#description' => t("Enter PHP code that returns a latitude/longitude.  Do not use &lt;?php ?&gt;. You must return only an array with float values set for the 'latitude' and 'longitude' keys."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('php'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_nid_arg($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Node ID argument to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => empty($options) ? t("Select which of the view's arguments to use as the node ID. The latitude / longitude of the first location of that node will be used as the origin. You must have added arguments to the view to use this option.") : t("Select which of the view's arguments to use as the node ID. The latitude / longitude of the first location of that node will be used as the origin."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('nid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_nid_loc_field($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Location to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => t("Select which field to use as the origin. If the location supports multiple entries the first one will be used."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('nid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_uid_arg($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('User ID argument to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => empty($options) ? t("Select which of the view's arguments to use as the user ID. The latitude / longitude of the first location of that user will be used as the origin. You must have added arguments to the view to use this option.") : t("Select which of the view's arguments to use as the user ID. The latitude / longitude of the first location of that user will be used as the origin."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('uid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_uid_loc_field($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Location to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => t("Select which field to use as the origin. If the location supports multiple entries the first one will be used."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('uid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_tid_arg($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Term ID argument to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => empty($options) ? t("Select which of the view's arguments to use as the term ID. The latitude / longitude of the first location of that term will be used as the origin. You must have added arguments to the view to use this option.") : t("Select which of the view's arguments to use as the term ID. The latitude / longitude of the first location of that term will be used as the origin."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('tid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_tid_loc_field($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Location to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => t("Select which field to use as the origin. If the location supports multiple entries the first one will be used."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('tid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_cid_arg($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Comment ID argument to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => empty($options) ? t("Select which of the view's arguments to use as the comment ID. The latitude / longitude of the first location of that comment will be used as the origin. You must have added arguments to the view to use this option.") : t("Select which of the view's arguments to use as the comment ID. The latitude / longitude of the first location of that comment will be used as the origin."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('cid_arg'));
  }
  return $element;
}

/**
 * @param string $default
 *
 * @param bool $ctools
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_cid_loc_field($default, $options, $ctools=TRUE) {
  $element = array(
    '#type' => 'select',
    '#title' => t('Location to use'),
    '#options' => $options,
    '#default_value' => $default,
    '#description' => t("Select which field to use as the origin. If the location supports multiple entries the first one will be used."),
  );
  if ($ctools) {
    $element['#dependency'] = array('edit-options-origin' => array('cid_arg'));
  }
  return $element;
}

/**
 * @param string $title
 *
 * @param string $default
 *
 * @param string $description
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_weight($title, $default, $description='') {
  $options = array('' => '');
  $options += drupal_map_assoc(range(-50, 50));
  $element = array(
    '#type' => 'select',
    '#title' => $title,
    '#default_value' => $default,
    '#options' => $options,
  );
  if (! empty($description)) {
    $element['#description'] = $description;
  }
  return $element;
}

/**
 * @param string $title
 *
 * @param string $default
 *
 * @param string $description
 *
 * @return
 *   Returns form element
 *
 */
function getlocations_fields_element_opts($title, $default, $description='') {

  $element = array(
    '#type' => 'select',
    '#title' => $title,
    '#default_value' => $default,
    '#options' => array(
      '0' => t('Normal'),
      '1' => t('Required'),
      '2' => t('Read only'),
      '3' => t('Display only'),
      '4' => t('Hidden'),
    ),
  );
  if (! empty($description)) {
    $element['#description'] = $description;
  }
  return $element;
}

/**
 * ajax callback
 *
 * @return
 *   Returns country code
 *
 */
#function getlocations_fields_countryinfo() {
#  $country = $_GET['country'];
#  $content = FALSE;
#  if (drupal_strlen($country) == 2 ) {
#    $content = drupal_strtoupper($country);
#  }
#  else {
#    $content = getlocations_get_country_id($country);
#  }
#  // fix 'The Netherlands' which is what google returns
#  if ($country == 'The Netherlands' || $country == 'Pays-Bas') {
#    $content = 'NL';
#  }
#  drupal_json_output(array('content' => $content));
#}

/**
 * autocomplete for country
 *
 * @param string $string
 *
 * @return
 *   Returns country names
 *
 */
function getlocations_fields_country_autocomplete($string = '') {
    $matches = array();
  if ($string) {
    $countries = getlocations_get_countries_list();
    foreach ($countries AS $country) {
      $s = drupal_strtolower($string);
      $c = drupal_strtolower($country);
      preg_match_all("/^$s/", $c, $m);
      if (count( $m[0])) {
        $matches[$country] = $country;
      }
    }
  }
  drupal_json_output($matches);
}

/**
 * autocomplete for province
 *
 * @param string $string
 *
 * @return
 *   Returns province names
 *
 */
function getlocations_fields_province_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    //
    $query = db_select('getlocations_fields', 'f');
    $query->fields('f', array('province'));
    $query->where("LOWER(province) LIKE LOWER(:st)", array(':st' => $string . '%'));
    $query->range(0, 15);
    $result = $query->execute();
    foreach ($result AS $row) {
      $matches[$row->province] = check_plain($row->province);
    }
  }
  drupal_json_output($matches);
}

/**
 * autocomplete for city
 *
 * @param string $string
 *
 * @return
 *   Returns city names
 *
 */
function getlocations_fields_city_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    //
    $query = db_select('getlocations_fields', 'f');
    $query->fields('f', array('city'));
    $query->where("LOWER(city) LIKE LOWER(:st)", array(':st' => $string . '%'));
    $query->range(0, 15);
    $result = $query->execute();
    foreach ($result AS $row) {
      $matches[$row->city] = check_plain($row->city);
    }
  }
  drupal_json_output($matches);
}

/**
 * @return
 *   Returns location array
 *
 */
function getlocations_fields_smart_ip() {
  $location = getlocations_fields_smart_ip_get();
  drupal_json_output($location);
}

/**
 * @return
 *   Returns location array
 *
 */
function getlocations_fields_smart_ip_get() {
  $location = FALSE;
  if (module_exists('smart_ip')) {
    global $user;
    if ($user->uid > 0 && isset($user->data['geoip_location'])) {
      $location = $user->data['geoip_location'];
    }
    elseif (isset($_SESSION['smart_ip']['location'])) {
      $location = $_SESSION['smart_ip']['location'];
    }
  }
  return $location;
}

/**
 * Provides a form for congfiguring search marker
 * @param array $defaults
 *
 * @return
 *   Returns form
 *
 */
function getlocations_fields_views_search_form($defaults, $map_type = '') {

  $form = array();
    // show_search_distance
    $form['show_search_distance'] = getlocations_element_map_checkbox(
      t('Show search distance'),
      $defaults['show_search_distance'],
      t('Show search distance from origin where relevant. Applies to Views using a distance filter.')
    );
    $form['views_search_center'] = getlocations_element_map_checkbox(
      t('Center map on search'),
      $defaults['views_search_center'],
      t('Place the search result location in the middle of the map. Applies to Views using a distance filter.')
    );

    // views_search_marker
    $form['views_search_marker_enable'] = getlocations_element_map_checkbox(
      t('Mark the search center on the map'),
      $defaults['views_search_marker_enable'],
      t('Show search marker on origin where relevant. Applies to Views using a distance filter.')
    );
    $form['views_search_marker_enable']['#suffix'] = '<div id="wrap-getlocations-views-search-marker">';

    if ($map_type == 'leaflet') {
      // TODO TEST THIS
      if ($defaults['awesome'] && $defaults['marker_type'] == 'fa') {
        $labels = array(
          'awesome_icon' => t('Search Awesome icon'),
          'awesome_icon_color' => t('Search Awesome icon color'),
          'awesome_marker_color' => t('Search Awesome marker color'),
          'awesome_title' => t('Search Awesome Fonts icon settings'),
          'awesome_icon_spin' => t('Search Awesome icon spin'),
          'awesome_icon_size' => t('Search Awesome icon size'),
          'awesome_icon_type' => t('Search Awesome icon type'),
          'awesome_icon_flip' => t('Search Awesome icon flip and rotate'),
          'awesome_icon_html' => t('Search Awesome icon HTML'),
          'awesome_icon_class' => t('Search Awesome icon class'),
        );
        $prefix = 'search';
        $form += getlocations_leaflet_awesome_markers_elements_get($defaults, $labels, $prefix);
      }
      else {
        // getlocations markers
        $markers = getlocations_get_marker_titles();
        $form['views_search_marker'] = getlocations_element_map_marker(
          t('Search Map marker'),
          $markers,
          $defaults['views_search_marker'],
          ''
        );
      }
    }
    else {
      $markers = getlocations_get_marker_titles();
      $form['views_search_marker'] = getlocations_element_map_marker(
        t('Search Map marker'),
        $markers,
        $defaults['views_search_marker'],
        ''
      );
    }

    $form['views_search_marker_toggle'] = getlocations_element_map_checkbox(
      t('Enable Search Marker toggle button'),
      $defaults['views_search_marker_toggle'],
      t('Show a button to switch visibility on or off.')
    );
    $form['views_search_marker_toggle_active'] = getlocations_element_map_checkbox(
      t('Initial state of Search Marker'),
      $defaults['views_search_marker_toggle_active'],
      t('Set initial visibility on.')
    );
    $form['views_search_marker_toggle_active']['#suffix'] = '</div>';

    if ($map_type !== 'mapquest') {
      // search area shape
      $form['views_search_radshape_enable'] = getlocations_element_map_checkbox(
        t('Enable Show search area'),
        $defaults['views_search_radshape_enable'],
        t('Show Search area as a rectangle or circle. Applies to Views using a distance filter.')
      );
      $form['views_search_radshape_enable']['#suffix'] = '<div id="wrap-getlocations-views-search-radshape">';

      // jquery_colorpicker
      if (module_exists('jquery_colorpicker') && $defaults['jquery_colorpicker_enabled']) {
        $form['views_search_radshape_strokecolor'] = array(
          '#type' => 'jquery_colorpicker',
          '#title' => t('Search area line color'),
          '#default_value' => preg_replace("/^#/", '', $defaults['views_search_radshape_strokecolor']),
          '#description' => t('Click on the Colorpicker icon to select a color'),
        );
      }
      else {
        $form['views_search_radshape_strokecolor'] = getlocations_element_map_tf(
          t('Search area line color'),
          $defaults['views_search_radshape_strokecolor'],
          t('HTML hex, eg #FF0000.'),
          10,
          10,
          TRUE
        );
      }

      $form['views_search_radshape_strokeopacity'] = getlocations_element_map_tf(
        t('Search area line opacity'),
        $defaults['views_search_radshape_strokeopacity'],
        t('Must be between 0 and 1.'),
        10,
        10,
        TRUE
      );
      $form['views_search_radshape_strokeweight'] = getlocations_element_map_tf(
        t('Search area line thickness'),
        $defaults['views_search_radshape_strokeweight'],
        t('Must be an integer.'),
        10,
        10,
        TRUE
      );

      // jquery_colorpicker
      if (module_exists('jquery_colorpicker') && $defaults['jquery_colorpicker_enabled']) {
        $form['views_search_radshape_fillcolor'] = array(
          '#type' => 'jquery_colorpicker',
          '#title' => t('Search area fill color'),
          '#default_value' => preg_replace("/^#/", '', $defaults['views_search_radshape_fillcolor']),
          '#description' => t('Click on the Colorpicker icon to select a color'),
        );
      }
      else {
        $form['views_search_radshape_fillcolor'] = getlocations_element_map_tf(
          t('Search area fill color'),
          $defaults['views_search_radshape_fillcolor'],
          t('HTML hex, eg #FF0000.'),
          10,
          10,
          TRUE
        );
      }

      $form['views_search_radshape_fillopacity'] = getlocations_element_map_tf(
        t('Search area fill opacity'),
        $defaults['views_search_radshape_fillopacity'],
        t('Must be between 0 and 1.'),
        10,
        10,
        TRUE
      );
      $form['views_search_radshape_toggle'] = getlocations_element_map_checkbox(
        t('Enable Search area toggle button'),
        $defaults['views_search_radshape_toggle'],
        t('Show a button to switch visibility on or off.')
      );
      $form['views_search_radshape_toggle_active'] = getlocations_element_map_checkbox(
        t('Initial state of Search area'),
        $defaults['views_search_radshape_toggle_active'],
        t('Set initial visibility on.')
      );
      $form['views_search_radshape_toggle_active']['#suffix'] = '</div>';

    }


  return $form;
}

/**
 * Implements hook_token_info().
 * derived from location_cck module
 */
function getlocations_fields_token_info() {
  $module = 'getlocations_fields';
  $supported_entities = getlocations_get_supported_entities($module);
  $columns = getlocations_fields_columns();
  $info = array();
  $fields = field_info_field_map();
  // Loop over the entities and fields and setup tokens.
  foreach ($fields as $field_key => $field_value) {
    if ($field_value['type'] == $module) {
      foreach ($supported_entities AS $entity) {
        $info['types'][$field_key] = array(
          'name' => t('GetLocations fields: !field', array('!field' => $field_key)),
          'description' => t('Tokens for the field: !field. Replace the "?" with the delta you want. Defaults to delta 0..', array('!field' => $field_key)),
          'needs-data' => $entity,
        );
        $info['tokens'][$entity][$field_key] = array(
          'name' => t('GetLocations field: !field', array('!field' => $field_key)),
          'description' => t('Tokens for the field: !field. Replace the "?" with the delta you want. Defaults to delta 0.', array('!field' => $field_key)),
          'type' => $field_key,
        );
      }
      foreach ($columns as $col_key => $col_value) {
        $info['tokens'][$field_key][$col_key . ":?"] = array(
          'name' => t($col_value),
          'description' => t($col_value),
          'type' => $field_key,
        );
      }
    }
  }
  return $info;
}

/**
 * Implements hook_tokens().
 */
function getlocations_fields_tokens($type, $tokens, array $data = array(), array $options = array()) {

  // Setup our replacements array.
  $sanitize = ( isset($options['sanitize']) && $options['sanitize'] ? TRUE : FALSE);
  $replacements = array();
  $module = 'getlocations_fields';
  $obj = FALSE;

  // Make sure we have an entity.
  if (isset($data['entity_type']) && isset($data['entity'])) {

    $obj = $data['entity'];
    $lang = (isset($obj->language) && $obj->language ? $obj->language : LANGUAGE_NONE);

  }

  if ($obj) {
    // Loop through the tokens.
    foreach ($tokens as $name => $original) {
      // Break token into an array.
      $pieces = explode(':', str_replace(array('[', ']'), '', $original));

      // Must have entity, field, and item to be a getlocations_fields token.
      if (count($pieces) < 3) {
        continue;
      }
      if (count($pieces) > 3 && $pieces[0] == 'user' && $pieces[1] == 'original' ) {
        $entity = $pieces[0];
        $field = $pieces[2];
        $item = $pieces[3];
        $delta = (isset($pieces[4]) && is_numeric($pieces[4]) ? $pieces[4] : 0);
      }
      else {
        $entity = $pieces[0];
        $field = $pieces[1];
        $item = $pieces[2];
        $delta = (isset($pieces[3]) && is_numeric($pieces[3]) ? $pieces[3] : 0);
      }

      $field_info = field_info_field($field);
      $supported_entities = getlocations_get_supported_entities($module);
      if ($entity == 'current-user') {
        $supported_entities[] = 'current-user';
      }
      $fieldnames = getlocations_get_fieldnames();
      $col_keys = getlocations_fields_columns(TRUE);
      // Make sure this is a field and it is a getlocations_fields field.
      if (!isset($field_info) || (isset($field_info) && $field_info['type'] != $module)
        || ! in_array($field, $fieldnames)
        || ! in_array($item, $col_keys)
        || ! in_array($entity, $supported_entities)
        ) {
        continue;
      }
      $content = (isset($obj->{$field}[$lang][$delta][$item]) ? $obj->{$field}[$lang][$delta][$item] : '');
      if ($content && $sanitize) {
        $content = getlocations_apoclean($content);
      }
      $replacements[$original] = $content;
    }
  }

  return $replacements;

}

function getlocations_fields_columns($keyonly = FALSE) {
  $columns = array(
    'name'         => t('Name'),
    'street'       => t('Street'),
    'additional'   => t('Additional'),
    'city'         => t('City'),
    'province'     => t('Province'),
    'postal_code'  => t('Postal code'),
    'latitude'     => t('Latitude'),
    'longitude'    => t('Longitude'),
    'country'      => t('Country'),
    'country_name' => t('Country name'),
    'phone'        => t('Phone'),
    'mobile'       => t('Mobile'),
    'fax'          => t('Fax'),
    'marker'       => t('Marker'),
  );
  if ($keyonly) {
    return array_keys($columns);
  }
  return $columns;
}
