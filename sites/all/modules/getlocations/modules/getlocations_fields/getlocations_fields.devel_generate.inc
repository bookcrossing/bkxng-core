<?php

/**
 * @file
 * getlocations_fields.devel_generate.inc
 * @author Bob Hutchinson https://www.drupal.org/u/hutch
 * @copyright GNU GPL
 *
 * Provides Getlocations field Devel Generate code, see issue #2603376.
 * Contributed by https://www.drupal.org/u/drikc
 */

/**
 * Implements hook_devel_generate().
 *
 * Included only when needed.
 */
function getlocations_fields_devel_generate($entity, $field, $instance, $bundle) {
  $places = _get_locations_get_random_places(1);

  return $places[0];
}

/**
 * Get random places.
 *
 * @param type $quantity the minimum number of items to return
 * @return array
 */
function _get_locations_get_random_places($quantity = 1) {
  $places = array();
  while ($quantity > count($places)) {
    $qs = array(
      'lat' => rand(0, 90),
      'lng' => rand(0, 180),
      'radius' => 300,
      'username' => variable_get('geonames_webservice_username_parameter', 'demo'),
    );
    $url = 'http://ws.geonames.org/findNearbyPlaceNameJSON?' . http_build_query($qs);
    $result = drupal_http_request($url);
    $data = json_decode($result->data);
    if (!empty($data->status)) {
      drupal_set_message(t('Warning: getlocations random places generator function (%f) failed to use geonames web service; status: "%message" with url @url.', array(
        '%f' => __FUNCTION__,
        '%message' => $data->status->message,
        '@url' => $url,
      )), 'warning');
      break;
    }
    if (!empty($data->geonames)) {
      foreach ($data->geonames as $geoname) {
        $places[] = array(
          'name' => $geoname->name,
          'latitude' => $geoname->lat,
          'longitude' => $geoname->lng,
          'country' => $geoname->countryCode,
        );
      }
    }
    sleep(rand(1, 4));
  }
  return $places;
}
