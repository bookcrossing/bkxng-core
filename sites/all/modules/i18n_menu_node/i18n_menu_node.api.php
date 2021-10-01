<?php

/**
 * @file
 * Hooks provided by the Menu Translation (Node) module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the information used to provide [i18n] blocks.
 *
 * This hook can be implemented to add support for additional modules needing
 * to render menu trees without incurring in language selection problems, or
 * altering the default information.
 *
 * @param $info
 *   A nested array keyed by module name. Each entry is an associative array
 *   having the following (optional) keys:
 *   - delta: An array of delta used to restrict the block set returned by the
 *     module. If this is empty all the returned blocks will be used.
 *   - view: A view callback that will be called to render the block identified
 *     by the given delta. If empty the default hook_block('view', $delta) will
 *     be used.
 *   - file: A file to be included before calling the view callback.
 */
function hook_i18n_menu_node_block_info_alter(&$info) {
  $info['custom_module'] = array(
    'delta' => array('custom_delta_1', 'custom_delta_2'),
    'view' => 'custom_module_view_callback',
    'file' => drupal_get_path('module', 'custom_module') .'/custom_module.inc',
  );
}

/**
 * @} End of "addtogroup hooks".
 */
