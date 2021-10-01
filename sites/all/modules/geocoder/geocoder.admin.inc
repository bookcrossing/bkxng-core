<?php

/**
 * @file
 * Settings form.
 */

/**
 * Module settings page.
 */
function geocoder_admin_settings(array $form, array &$form_state) {
  $geocoder_settings = variable_get('geocoder_settings', array());

  $yahoo_url = 'http://developer.yahoo.com/geo/placefinder/';
  $form['geocoder_apikey_yahoo'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo PlaceFinder API Key'),
    '#description' => t(
      'You can obtain a Yahoo PlaceFinder consumer key at !link',
      array(
        '!link' => l($yahoo_url, $yahoo_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_yahoo']) ? '' : $geocoder_settings['geocoder_apikey_yahoo'],
    '#required' => FALSE,
  );

  $yandex_url = 'http://api.yandex.ru/maps/getkey.xml';
  $form['geocoder_apikey_yandex'] = array(
    '#type' => 'textfield',
    '#title' => t('Yandex Maps API Key'),
    '#description' => t(
      'You can obtain a Yandex API Key at !link',
      array(
        '!link' => l($yandex_url, $yandex_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_yandex']) ? '' : $geocoder_settings['geocoder_apikey_yandex'],
    '#required' => FALSE,
  );

  $bing_url = 'http://msdn.microsoft.com/en-us/library/ff428642.aspx';
  $form['geocoder_apikey_bing'] = array(
    '#type' => 'textfield',
    '#title' => t('Bing API Key'),
    '#description' => t(
      'You can obtain a Bing API Key at !link',
      array(
        '!link' => l($bing_url, $bing_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_bing']) ? '' : $geocoder_settings['geocoder_apikey_bing'],
    '#required' => FALSE,
  );

  $form['geocoder_apikey_mapzen'] = array(
    '#type' => 'textfield',
    '#title' => t('Mapzen Search API Key'),
    '#description' => t(
      'You can obtain a Mapzen Search API Key at !url.',
      array(
        '!url' => 'https://mapzen.com/developers',
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_mapzen']) ? '' : $geocoder_settings['geocoder_apikey_mapzen'],
    '#required' => FALSE,
  );

  $form['geocoder_apikey_mapbox'] = array(
    '#type' => 'textfield',
    '#title' => t('Mapbox Access Token'),
    '#description' => t(
      'You can obtain a Mapbox access token at !url.',
      array(
        '!url' => 'http://www.mapbox.com',
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_mapbox']) ? '' : $geocoder_settings['geocoder_apikey_mapbox'],
    '#required' => FALSE,
  );

  $form['geocoder_apikey_mapquest'] = array(
    '#type' => 'textfield',
    '#title' => t('MapQuest API Key'),
    '#description' => t(
      'You can obtain a Mapquest API key at !url.',
      array(
        '!url' => 'https://developer.mapquest.com/plan_purchase/steps/business_edition/business_edition_free/register',
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_mapquest']) ? '' : $geocoder_settings['geocoder_apikey_mapquest'],
    '#required' => FALSE,
  );

  $form['geocoder_google_auth_method'] = array(
    '#type' => 'select',
    '#title' => t('Google API Authorization Method'),
    '#description' => t("If your website runs on shared hosting, you'll want to authenticate requests to the Google Geocoding API to reduce the likelihood of being rate limited (2500 requests per day / 5 requests per second). Alternatively, Google Maps for Work customers may use their Client ID and Signing Key to authenticate."),
    '#default_value' => variable_get('geocoder_google_auth_method', GEOCODER_GOOGLE_AUTH_NONE),
    '#options' => array(
      GEOCODER_GOOGLE_AUTH_NONE => 'None',
      GEOCODER_GOOGLE_AUTH_KEY => 'API Key (free)',
      GEOCODER_GOOGLE_AUTH_WORK => 'Google Maps API for Work',
    ),
  );

  $google_url = 'https://developers.google.com/maps/documentation/geocoding/#api_key';
  $form['geocoder_apikey_google'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API Key'),
    '#description' => t(
      'Obtain a free Google Geocoding API Key at !link',
      array(
        '!link' => l($google_url, $google_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => empty($geocoder_settings['geocoder_apikey_google']) ? '' : $geocoder_settings['geocoder_apikey_google'],
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="geocoder_google_auth_method"]' => array('value' => GEOCODER_GOOGLE_AUTH_KEY),
      ),
    ),
  );

  $google_client_url = 'https://developers.google.com/maps/documentation/business/webservices/auth#business-specific_parameters';
  $form['geocoder_google_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API for Work: Client ID'),
    '#description' => t(
      'For more information, visit !link',
      array(
        '!link' => l($google_client_url, $google_client_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => variable_get('geocoder_google_client_id'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="geocoder_google_auth_method"]' => array(
          'value' => GEOCODER_GOOGLE_AUTH_WORK,
        ),
      ),
    ),
  );

  $google_private_url = 'https://developers.google.com/maps/documentation/business/webservices/auth#how_do_i_get_my_signing_key';
  $form['geocoder_google_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API for Work: Private/Signing Key'),
    '#description' => t(
      'For more information, visit !link',
      array(
        '!link' => l($google_private_url, $google_private_url, array('external' => TRUE)),
      )
    ),
    '#default_value' => variable_get('geocoder_google_private_key'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="geocoder_google_auth_method"]' => array(
          'value' => GEOCODER_GOOGLE_AUTH_WORK,
        ),
      ),
    ),
  );

  $form['geocoder_google_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Delay between Google geocoding requests (in milliseconds)'),
    '#description' => t('Adds a delay between geocoding requests, to avoid OVER_QUERY_LIMIT errors from Google. 200ms is recommended.'),
    '#default_value' => variable_get('geocoder_google_delay', 0),
    '#size' => 10,
  );

  $form['geocoder_cache_empty_results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache empty results'),
    '#default_value' => variable_get('geocoder_cache_empty_results', TRUE),
    '#description' => t('Geocoder caches all queries by default. Do you want that to include the ones with no results? If not, it will be checked every time, probably with the same no-result.'),
  );

  $form['geocoder_log_empty_results'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log empty results'),
    '#default_value' => variable_get('geocoder_log_empty_results', FALSE),
    '#description' => t('Whether to generate a watchdog log when geocoding returns no results.'),
  );
  $form['geocoder_recode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force re-geocoding'),
    '#description' => t('Enable forced re-geocoding of all geocoder fields on entity save (even if there are no changes to the source of the geocoded data). This option can slow down bulk saves and increases the amount of calls to the geocoding service.'),
    '#default_value' => variable_get('geocoder_recode', FALSE),
  );

  $form['#submit'][] = 'geocoder_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Form submit callback.
 */
function geocoder_admin_settings_submit(array $form, array &$form_state) {
  variable_set(
    'geocoder_settings',
    array_map(
      'trim',
      $form_state['values']
    )
  );
}
