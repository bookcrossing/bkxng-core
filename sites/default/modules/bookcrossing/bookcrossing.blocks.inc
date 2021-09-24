<?php

/**
 * Implements hook_block_info().
 *
 */
function bookcrossing_block_info()
{
    $blocks = array();

    /**
     * Block with the form for entering bcid of the found book.
     */
    $blocks['book-found'] = array(
        'info' => t('Form for entering bcid of the found book'),
        'cache' => DRUPAL_NO_CACHE,
    );

    /**
     * Block with facebook and twitter links.
     */
    $blocks['facebook-and-twitter-link'] = array(
        'info' => t('Facebook and twitter links'),
        'cache' => DRUPAL_CACHE_GLOBAL,
    );

    /**
     * Block with FAQ link.
     */
    $blocks['faq-link'] = array(
        'info' => t('FAQ link'),
        'cache' => DRUPAL_CACHE_GLOBAL,
    );

    /**
     * Block with ajax links to reserve bcids.
     */
    $blocks['reserve-bcid'] = array(
        'info' => t('Reserve BCID'),
        'cache' => DRUPAL_NO_CACHE,
    );

    $blocks['search'] = array(
        'info' => t('Book search form'),
        'cache' => DRUPAL_NO_CACHE,
    );

    return $blocks;
}


/**
 * Implements hook_block_view()
 *
 * @param string $block_type
 *   defines type of the block to be displayed
 *
 * @return string
 *   html string of the block
 */
function bookcrossing_block_view($block_type = '')
{
    $block = array();

    switch ($block_type) {
        case 'book-found':
            $block['subject'] = '';
            $block['content'] = drupal_get_form('bookcrossing_books_form');
        break;

        case 'reserve-bcid':
            global $user;
            $block['subject'] = t('Reserved BCID');
            // Select bcids we have already reserved
            $reserved = db_select('bookcrossing_books', 'b')
                ->fields('b', array('bcid'))
                ->condition('status', BOOKCROSSING_BOOK_STATUS_RESERVED, '=')
                ->condition('owner', $user->uid, '=')
                ->execute()
                ->fetchCol();
            $num = count($reserved);
            $output = '<div class="reserved-bcids-container">';
            // Show our reserved bcids
            for ($i = 0; $i < $num; $i++) {
                $output .= '<div class="bcid-container" id="' . $reserved[$i] . '">' . l('<b>BCID:</b>' . $reserved[$i], 'add-reserved-book/' . $reserved[$i], array('html' => TRUE)) . '</div>';
            }
// @todo: Move BOOKCROSSING_RESERVED_BCIDS to configuration
            // Do not show link for reserving bcid if we have already reserved the number of bcids defined by BOOKCROSSING_RESERVED_BCIDS constant
            if ($num < BOOKCROSSING_RESERVED_BCIDS) {
                $output .= '<div id="reserve-bcid">' . l(t('+Reserve BCID'), 'reserve-bcid', array('attributes' => array('class' => array('use-ajax')))) . '</div>';
            }
            // Show explanation text if there are no bcids reserved
            if ($num == 0) {
                $output .= '<div id="reserve-bcid-description">' . t('You can reserve multiple BCID.') . '</div>';
            }
            $output .= '</div><div style="clear: both;"></div>';
            $block['content'] = $output;
        break;

        case 'search':
            $block['subject'] = '';
            $block['content'] = drupal_get_form('bookcrossing_search_form');
        break;

        default:
        break;
    }

    return $block;
}


/**
 * Implements hook_block_configure().
 * @param string $block_type
 * @return array
 */
function bookcrossing_block_configure($block_type = '')
{
    $form = array();

    switch ($block_type) {
        case 'faq-link':
            $form['faq-link'] = array(
                '#type' => 'textfield',
                '#title' => t('Page where FAQ is located'),
                '#default_value' => variable_get('bookcrossing_faq_link', ''),
            );

            $form['help-link'] = array(
                '#type' => 'textfield',
                '#title' => t('Page where help is located'),
                '#default_value' => variable_get('bookcrossing_help_link', ''),
            );
            break;

        default:
            break;
    }

    return $form;
}

/**
 * Implements hook_block_save().
 *
 * @param string $block_type
 *   defines type of the block to be displayed
 * @param array $edit
 *   array containing values to be saved
 */
function bookcrossing_block_save($block_type, $edit = array())
{
    switch ($block_type) {
        case 'facebook-and-twitter-link':
            variable_set('bookcrossing_facebook_link', $edit['facebook-link']);
            variable_set('bookcrossing_twitter_link', $edit['twitter-link']);
            break;

        case 'faq-link':
            variable_set('bookcrossing_faq_link', $edit['faq-link']);
            variable_set('bookcrossing_help_link', $edit['help-link']);
            break;

        default:
            break;
    }

    return;
}
