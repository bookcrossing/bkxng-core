<?php

/**
 * @file
 * Provide views handler for Facebook Social plugins.
 */

/**
 * A handler to provide a field for Facebook Social plugins.
 *
 * @ingroup views_field_handlers
 */
class fb_social_handler_field extends views_handler_field {

  /**
   * Implements views_object#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['fb_social_plugins'] = array('default' => '');
    return $options;
  }

  /**
   * Implements views_handler#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $presets = fb_social_get_presets();
    foreach ($presets as $preset) {
      $preset_list[$preset->name] = $preset->name;
    }

    $form['fb_social_plugins'] = array(
      '#title' => t('Facebook Social Preset'),
      '#options' => array('' => '<select>') + $preset_list,
      '#type' => 'select',
      '#required' => TRUE,
      '#default_value' => $this->options['fb_social_plugins'],
      '#description' => t('Choose a Facebook Social Preset'),
    );

  }

  /**
   * Implements views_handler_field#render().
   */
  function render($values) {

    if (!empty($this->options['fb_social_plugins'])) {
      $presets = fb_social_get_presets();
      $preset = $presets[$this->options['fb_social_plugins']];
      $type = $presets[$this->options['fb_social_plugins']]->plugin_type;
      $url = NULL;
      // Only supply url for node if nid exists and FB preset accepts url.
      // TODO: instead of hardcoded values, get this info from plugin defaults
      if ($type == 'like' || $type == 'send' || $type == 'facepile' || $type == 'comments') {
        // TODO: with a little work this could be expanded to other entities.
        if (!empty($values->nid)) {
          $url = fb_social_url('node/' . $values->nid);
        }
        elseif (!empty($values->uid)) {
          $url = fb_social_url('user/' . $values->uid);
        }
        $preset -> fb_attrs['href'] = $url;
      }
      $value = fb_social_preset_view($preset);
    }

    return $value;
  }
}
