<?php

/**
 *
 */
function bookcrossing_node_info()
{
    return array(
        'book_edition' => array(
            'name' => t('Book Edition'),
            'base' => 'bookcrossing',
            'description' => t('Edition or variation of a book, identified by ISBN'),
            'has_title' => TRUE,
            'title_label' => t('Book Title'),
            'locked' => TRUE,
        ),
        'book_item' => array(
            'name' => t('Book Item'),
            'base' => 'bookcrossing',
            'description' => t('Unique single book, identified by BCID'),
            'has_title' => TRUE,
            'title_label' => t('Book Title'),
            'locked' => TRUE,
        ),
    );
}

function bookcrossing_form($node, &$form_state)
{
    $form = array();

    switch($node->type)
    {
        case 'book_edition':
            $form['book_edition_fields']['title'] =
                [
                    '#type' => 'textfield',
                    '#title' => t('Title'),
                    '#description' => t('Book title.'),
                ];
            $form['book_edition_fields']['title_sort'] =
            [
                '#type' => 'textfield',
                '#title' => t('Sort title'),
                '#description' => t('Title that will be used for sorting.'),
            ];
            $form['book_edition_fields']['title_original'] =
            [
                '#type' => 'textfield',
                '#title' => t('Original title'),
                '#description' => t('Title of the first edition of the book.'),
            ];
            $form['book_edition_fields']['authors'] =
            [
                '#type' => 'textfield',
                '#title' => t('List of authors'),
                '#description' => t('Comma-separated list of authors.'),
            ];
            $form['book_edition_fields']['translators'] =
            [
                '#type' => 'textfield',
                '#title' => t('List of translators'),
                '#description' => t('Comma-separated list of translators.'),
            ];
            $form['book_edition_fields']['rating'] =
                [
                    '#type' => 'radios',
                    '#title' => t('Rating'),
                    '#description' => t('Book rating.'),
                ];
            $form['book_edition_fields']['book_items'] =
                [
                    '#type' => '',
                    '#title' => t('Book items'),
                    '#description' => t('Related book items.'),
                ];
            $form['book_edition_fields']['cover'] =
                [
                    '#type' => 'file',
                    '#title' => t('Cover'),
                    '#description' => t('Book cover.'),
                ];
            $form['book_edition_fields']['language'] =
                [
                    '#type' => 'textfield',
                    '#title' => t('Language'),
                    '#description' => t('Language of edition.'),
                ];
            $form['book_edition_fields']['annotation'] =
                [
                    '#type' => 'textarea',
                    '#title' => t('Annotation'),
                    '#description' => t('Annotation of the book.'),
                ];
            $form['book_edition_fields']['published'] =
                [
                    '#type' => 'textfield',
                    '#title' => t('Published'),
                    '#description' => t('Year when the book was published.'),
                ];
            $form['book_edition_fields']['publisher'] =
                [
                    '#type' => 'textfield',
                    '#title' => t('Publisher'),
                    '#description' => t('Publisher name.'),
                ];
            $form['book_edition_fields']['format'] =
                [
                    '#type' => 'textfield',
                    '#title' => t('Format'),
                    '#description' => t('Book format.'),
                ];
            break;
        case 'book_item':
            $form['book_item_veld'] = array(
                '#type' => 'file',
                '#description' => 'You might wanna upload a file!',
                '#title' => 'Bestand',
            );
            break;
    }

    return $form;
}
