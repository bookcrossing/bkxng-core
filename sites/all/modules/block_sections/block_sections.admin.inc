<?php

/**
 * @file
 * Administrative functionality for block_sections module.
 */

/**
 * Form constructor for admin overview.
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array
 *   The rendered form.
 *
 * @see block_sections_get_sections()
 */
function block_sections_admin_overview($form, &$form_state) {
  // Check user access.
  $admin_access = user_access(BLOCK_SECTIONS_ADMINISTER);

  // Get all saved sections.
  $sections = block_sections_get_sections();

  $form = $options = array();

  // Build the table header.
  $header = array(
    'title' => array('data' => t('Title')),
    'operations' => array('data' => t('Operations')),
  );

  foreach ($sections as $section) {
    $options[$section->sid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'item',
          '#title' => check_plain($section->title),
        ),
      ),
    );

    $operations = array();
    if ($admin_access) {
      $operations['edit'] = array(
        'title' => t('edit'),
        'href' => 'admin/structure/block-sections/' . $section->sid . '/edit',
      );
      $operations['delete'] = array(
        'title' => t('delete'),
        'href' => 'admin/structure/block-sections/' . $section->sid . '/delete',
      );
    }
    $options[$section->sid]['operations'] = array();
    if (!empty($operations)) {
      // Render an unordered list of operations links.
      $options[$section->sid]['operations'] = array(
        'data' => array(
          '#theme' => 'links',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
  }

  $admin_add = t('<a href="@url">Add one now!</a>', array('@url' => url('admin/structure/block-sections/add')));

  $form['sections'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $options,
    '#empty' => t('No sections available. !add_link', array('!add_link' => $admin_add)),
  );

  return $form;
}

/**
 * Form constructor for the section add / edit form.
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 * @param object $section
 *   (optional) Section object to edit.
 *
 * @return array
 *   The rendered form.
 *
 * @see block_sections_get_sections()
 * @see block_sections_add_section_form_submit()
 */
function block_sections_add_section_form($form, &$form_state, $section = FALSE) {
  // If section exists, we're editing.
  if ($section) {
    $form['sid'] = array(
      '#type' => 'hidden',
      '#value' => $section->sid,
    );
    $form_title = t('Edit section');
  }
  else {
    $form_title = t('Create a new section');
  }

  $form['block_sections'] = array(
    '#type' => 'fieldset',
    '#title' => $form_title,
  );

  $form['block_sections']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Section title'),
    '#required' => TRUE,
    '#default_value' => $section ? $section->title : '',
  );

  $form['block_sections']['paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('Section is active when any of the paths above match the page path. Put each path on a separate line. You can use the "*" character as a wildcard and ~ to exclude one or more paths. Use &lt;front&gt; for the site front page.'),
    '#required' => TRUE,
    '#default_value' => $section ? $section->paths : '',
  );

  $form['block_sections']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => $section ? t('Save') : t('Add'),
  );

  return $form;
}

/**
 * Form submission handler for block_sections_add_section_form().
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @see block_sections_save_section()
 */
function block_sections_add_section_form_submit($form, &$form_state) {
  // Check is section sid is provided.
  $is_existing_section = isset($form_state['values']['sid']);

  // Prepare $section object.
  $section = new stdClass();

  if ($is_existing_section) {
    // We are editing an existing section.
    $section->sid = $form_state['values']['sid'];
  }

  // Set section values.
  $section->title = $form_state['values']['title'];
  $section->paths = $form_state['values']['paths'];

  // Save section.
  block_sections_save_section($section);

  // Clear menu cache.
  block_sections_clear_menu_cache();

  drupal_set_message(t('Changes saved.'));

  $form_state['redirect'] = 'admin/structure/block-sections';
}

/**
 * Form constructor for the section delete confirmation form.
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 * @param object $section
 *   Section object to delete.
 *
 * @return array
 *   The rendered form.
 *
 * @see block_sections_get_sections()
 * @see block_sections_delete_section_form_submit()
 */
function block_sections_delete_section_form($form, &$form_state, $section) {
  // If section does not exist, redirect user.
  if (!$section) {
    drupal_goto('admin/structure/block-sections');
  }

  drupal_set_title(t('Delete section'));

  $form = confirm_form(
    array(
      'section' => array(
        '#type' => 'value',
        '#value' => $section,
      ),
    ),
    t('Are you sure you want to delete the section %title?', array('%title' => $section->title)),
    'admin/structure/block-sections',
    t('This action cannot be undone.'),
    t('Delete section'),
    t('Cancel')
  );

  return $form;
}

/**
 * Form submission handler for block_sections_delete_section_form().
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @see block_sections_delete_section()
 * @see block_sections_delete_content()
 * @see block_sections_clear_menu_cache()
 */
function block_sections_delete_section_form_submit($form, &$form_state) {
  $sid = $form_state['values']['section']->sid;
  $title = $form_state['values']['section']->title;

  // Delete the section.
  block_sections_delete_section($sid);

  // Delete all content from the deleted section.
  block_sections_delete_content($sid, 'sid');

  // Clear menu cache.
  block_sections_clear_menu_cache();

  drupal_set_message(t('Section %title deleted.', array('%title' => $title)));

  $form_state['redirect'] = 'admin/structure/block-sections';
}

/**
 * Form constructor for the block add / edit form.
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @return array
 *   The rendered form.
 *
 * @see block_sections_get_block_by_name()
 * @see block_sections_add_block_form_submit()
 */
function block_sections_add_block_form($form, &$form_state) {
  $form = array();

  $form['human_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Block name'),
    '#required' => TRUE,
    '#size' => 32,
    '#default_value' => '',
    '#maxlength' => 255,
    '#description' => t('The human-readable name of the block.'),
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'block_sections_get_block_by_name',
      'source' => array('human_name'),
    ),
    '#description' => t('A unique machine-readable name for the block. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );

  return $form;
}

/**
 * Form submission handler for block_sections_add_block_form().
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @see block_sections_save_block()
 * @see block_sections_clear_menu_cache()
 */
function block_sections_add_block_form_submit($form, &$form_state) {
  // Prepare $block object.
  $block = new stdClass();

  // Set block values.
  $block->name = $form_state['values']['human_name'];
  $block->machine_name = $form_state['values']['name'];

  // Save block.
  block_sections_save_block($block);

  // Clear menu cache.
  block_sections_clear_menu_cache();

  drupal_set_message(t('Changes saved.'));

  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Form constructor for the block delete confirmation form.
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 * @param string $delta
 *   (optional) String containing the machine name of a block.
 *
 * @return array
 *   The rendered form.
 *
 * @see block_sections_get_block_by_name()
 * @see block_sections_delete_block_form_submit()
 */
function block_sections_delete_block_form($form, &$form_state, $delta = '') {
  // Load the block.
  $block = block_sections_get_block_by_name($delta);

  // If block does not exist, redirect user.
  if (!$block) {
    drupal_goto('admin/structure/block');
  }

  $form = confirm_form(
    array(
      'block_name' => array(
        '#type' => 'hidden',
        '#value' => $block->name,
      ),
      'block_machine_name' => array(
        '#type' => 'hidden',
        '#value' => $block->machine_name,
      ),
      'block_bid' => array(
        '#type' => 'hidden',
        '#value' => $block->bid,
      ),
    ),
    t('Are you sure you want to delete block %name?', array('%name' => $block->name)),
    'admin/structure/block',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

  return $form;
}


/**
 * Form submission handler for block_sections_delete_block_form().
 *
 * @param array $form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 *
 * @see block_sections_delete_block()
 * @see block_sections_delete_content()
 */
function block_sections_delete_block_form_submit($form, &$form_state) {
  $human_name = $form_state['values']['block_name'];
  $machine_name = $form_state['values']['block_machine_name'];
  $bid = $form_state['values']['block_bid'];

  // Delete the block.
  block_sections_delete_block($bid, $machine_name);

  // Delete all content from the deleted block.
  block_sections_delete_content($bid, 'bid');

  // Clear caches.
  cache_clear_all();

  drupal_set_message(t('Block %name has been removed.', array('%name' => $human_name)));

  $form_state['redirect'] = 'admin/structure/block';
}

/**
 * Alters the block admin form to add 'Delete' links next to section blocks.
 *
 * @param array &$form
 *   Array of form elements that comprise the form.
 * @param array &$form_state
 *   A keyed array containing the current state of the form.
 * @param string $form_id
 *   String representing the name of the form itself.
 *
 * @see block_sections_get_blocks()
 */
function _block_sections_form_block_admin_display_form_alter(&$form, &$form_state, $form_id) {
  // Get all saved blocks.
  $blocks = block_sections_get_blocks();

  // Set deletion base path.
  $delete_base_path = 'admin/structure/block/delete-section-block/';

  foreach ($blocks as $block) {
    $form['blocks']['block_sections_' . $block->machine_name]['delete'] = array(
      '#type' => 'link',
      '#title' => t('delete'),
      '#href' => $delete_base_path . $block->machine_name,
    );
  }
}
