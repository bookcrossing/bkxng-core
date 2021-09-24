<?php
/**
 * CCK fields entry support
 *
 */

/**
 * Implements hook_field_formatter_info().
 */
function bookcrossing_field_formatter_info()
{
    $info = array(
        'list_user' => array(
            'label' => 'Имя пользователя',
            'field types' => array('number_integer'),
            'behaviors' => array(
                'multiple values' => FIELD_BEHAVIOR_CUSTOM,
            ),
            'settings' => array('display_label' => 0),
        ),
    );
    return $info;
}

/**
 * Implements hook_field_formatter_view().
 */
function bookcrossing_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display)
{
    $element = array();
    $cache = & drupal_static(__FUNCTION__, array());
    switch ($display['type']) {
        case 'list_user':
            if (isset($items[0]['value'])) {
                foreach ($items as $delta => $item) {
                    if (isset($cache[$item['value']])) {
                        $elements[] = $cache[$item['value']];
                    } else {
                        $user_name = db_select('users', 'u')
                            ->fields('u', array('name'))
                            ->condition('u.uid', $item['value'])
                            ->execute()
                            ->fetchField();
                        $elements[] = $cache[$item['value']] = $user_name;
                    }
                }
                $element[] = array('#markup' => implode(', ', $elements));
            }
            break;
    }
    return $element;
}