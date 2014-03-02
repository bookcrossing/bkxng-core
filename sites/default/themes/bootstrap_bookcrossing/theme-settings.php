<?php

function bootstrap_bookcrossing_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  $form['main_menu'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Main menu settings'),
  );

  $form['main_menu']['visible_menu_items'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Number of visible menu items.'),
    '#default_value' => theme_get_setting('visible_menu_items'),
    '#description'   => t('Visible number of menu items. Rest of items will be hidden under More dropdown item.'),
    '#size'               => '10',
  );

}

