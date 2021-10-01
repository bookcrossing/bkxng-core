<?php

/**
 * @file
 * Catalog functionality for bookcrossing site.
 */

require_once ('BookCatalog.inc');

/**
 * Helper function that generates charset for different languages and numbers.
 *
 * @param string $type
 *   Type of the charset to be returned.
 *
 * @return array
 *   An array of chars keyed by same chars.
 */
function bookcrossing_get_charset($type)
{
    switch ($type) {
        case 'eng':
            $charset = drupal_map_assoc(array('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'));
            break;
        case 'rus':
            $charset = drupal_map_assoc(array('а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я'));
            break;
        case 'num':
            $charset = drupal_map_assoc(range(0, 9));
            break;
        default:
            $charset = '';
            break;
    }
    return $charset;
}

/**
 * Menu callback: catalog page.
 *
 * @todo Investigate why $book in $output .= $function($type, $books) is sent with FALSE value for 7th item (/books?page=23)
 *
 * @param string $sort_type
 *      If $sort_type not set, the first sort type defined in bookcrossing_catalog_get_sort_types() will be used.
 * @param string $view_type
 *      If $mode not set,  the first view mode defined in bookcrossing_catalog_get_view_types() will be used.
 *
 * @return string
 */
function bookcrossing_catalog_page($sort_type = "date", $view_type = "tiles")
{
    $page_limit = variable_get('bookcrossing_catalog_books_per_page', 30);
    $sort_types = bookcrossing_catalog_get_sort_types();
    $view_types = bookcrossing_catalog_get_view_types();
    if (!array_key_exists($sort_type, $sort_types))
    {
        $sort_type = array_keys($sort_types)[0];
    }
    if (!array_key_exists($view_type, $view_types))
    {
        $view_type = array_keys($view_types)[0];
    }

    $catalog_title_constant = t('Books for Bookcrossing');
    $catalog_title_type = $sort_type;
    $catalog_title_view = $view_type;
    $catalog_title_value = '';
    $catalog_title_value_type = '';
    $books = array();
    $sort_form = '';

    if (isset($sort_types[$sort_type]['sort_form']))
    {
        $sort_form = drupal_get_form($sort_types[$sort_type]['sort_form']);
    }

    if ((isset($_GET['sort']) || isset($_GET['place']) || isset($_GET['country'])) && !isset($_GET['find'])) 
    {
        /**
         * Defines function used to filter book items
         * See definition of bookcrossing_catalog_filter_by_{$sort}() function
         */
        $catalog_filter_by_function = isset($sort_types[$sort_type]['filter_callback']) ? $sort_types[$sort_type]['filter_callback'] : '';
        if (!function_exists($catalog_filter_by_function)) {
            watchdog('bookcrossing.catalog.inc', 'Function %func referenced in bookcrossing_catalog_page() is not defined!', array('%func' => $catalog_filter_by_function), WATCHDOG_ERROR);
            return '';
        }

        if (isset($_GET['place'])) {
            $books = $catalog_filter_by_function($_GET['place'], $page_limit, $_GET['place']);
            $catalog_title_value_type = t('place');
            $catalog_title_value = ucfirst($_GET['place']);
        } elseif (isset($_GET['country'])) {
            $books = $catalog_filter_by_function('', $page_limit);
            $catalog_title_value_type = t('country');
            $catalog_title_value = ucfirst($_GET['country']);
        } else {
            $books = $catalog_filter_by_function($_GET['sort'], $page_limit);
            if ($sort_type == 'place') {
                $catalog_title_value_type = t('city');
            }
            $catalog_title_value = ucfirst($_GET['sort']);
        }
    } elseif (isset($_GET['find'])) {
        $books = bookcrossing_catalog_find_books($sort_type, $view_type);
    } else {
        /**
         * Defines function used to sort book items
         * See definition of bookcrossing_catalog_sort_by_{$sort}() function
         */
        $catalog_sort_by_function = isset($sort_types[$sort_type]['callback']) ? $sort_types[$sort_type]['callback'] : '';
        if (!function_exists($catalog_sort_by_function)) {
            watchdog('bookcrossing.catalog.inc', 'Function %func referenced in bookcrossing_catalog_page() is not defined!', array('%func' => $catalog_sort_by_function), WATCHDOG_ERROR);
            drupal_not_found();
            drupal_exit();
        }

        $books = $catalog_sort_by_function($view_type, $page_limit);
    }

//    dpm($function,'$function');
//    dpm($books,'$books');
    $output = '';

    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $search_msg = t('You are currently searching for "@search"', array('@search' => $_GET['find']));
        $query = drupal_get_query_parameters();
        unset($query['page']);
        unset($query['find']);
        unset($query['sort']);
        $link = l(t('cancel searching'), $_GET['q'], array('query' => $query));

        $output .= '<div class="searching-for">' . $search_msg . $link . '</div>';
    }

    $output .= '<div class="catalog-controls">';
    $output .= '<div class="catalog-sorts">' . bookcrossing_catalog_build_sort_types($view_type) . '</div>';
    $output .= '<div class="catalog-view-types">' . bookcrossing_catalog_build_view_types($sort_type) . '</div>';
    $output .= paginator3000pager();
    $output .= '</div>';

    $output .= render($sort_form);

    $catalog_view_function = 'bookcrossing_catalog_view_' . $view_type;
    if (function_exists($catalog_view_function)) {
        $output .= $catalog_view_function($sort_type, $books);
    }
    else {
        watchdog('bookcrossing.catalog.inc', 'Function %func referenced in bookcrossing_catalog_page() is not defined!', array('%func' => $catalog_view_function), WATCHDOG_ERROR);
        return '';
    }

    /**
     * Defines function used to create array of indices
     * See definition of bookcrossing_catalog_{$sort}_index() function
     */
    $catalog_index_function = $sort_types[$sort_type]['index_callback'];
    if (function_exists($catalog_index_function)) {
        $output .= bookcrossing_catalog_build_index( $catalog_index_function() );
    }
    else {
        watchdog('bookcrossing.catalog.inc', 'Function %func referenced in bookcrossing_catalog_page() is not defined!', array('%func' => $catalog_index_function), WATCHDOG_ERROR);
        return '';
    }

    $catalog_title = $catalog_title_constant;
    if (!empty($catalog_title_value)) {
        $catalog_title .= ' (' . $catalog_title_value . ')';
    }
    $catalog_title .= ' - ' . t('sorted by ' . $catalog_title_type . 's') . ', ' . t($catalog_title_view . ' view');
    drupal_set_title($catalog_title);

    return $output;
}


/**
 * Levels of index links
 */
function bookcrossing_catalog_get_index_levels()
{
    $defaults = array(
        '0' => '14',
        '10' => '16',
        '20' => '20',
        '30' => '24',
        '40' => '28',
        '50' => '32',
        '60' => '36',
        '70' => '40',
        '80' => '44',
        '90' => '48',
        '100' => '52',
    );

    $font_sizes = variable_get('bookcrossing_catalog_font_sizes', $defaults);

    return $font_sizes;
}


/**
 * Helper function: get list of sorting types
 *
 * @todo filter callback
 * @todo implement callbacks for date type
 *
 */
function bookcrossing_catalog_get_sort_types()
{
    $types = array(
        'date' => array(
            'title' => t('date'),
            'path' => 'date',
            'callback' => 'bookcrossing_catalog_sort_by_date',
            'grouping_callback' => 'bookcrossing_catalog_get_date',
            'filter_callback' => 'bookcrossing_catalog_filter_by_date',
            'index_callback' => 'bookcrossing_catalog_date_index',
        ),
        'title' => array(
            'title' => t('title'),
            'path' => 'title',
            'callback' => 'bookcrossing_catalog_sort_by_title',
            'grouping_callback' => 'bookcrossing_catalog_get_title_char',
            'filter_callback' => 'bookcrossing_catalog_filter_by_title',
            'index_callback' => 'bookcrossing_catalog_title_index',
        ),
        'author' => array(
            'title' => t('author'),
            'path' => 'author',
            'callback' => 'bookcrossing_catalog_sort_by_author',
            'grouping_callback' => 'bookcrossing_catalog_get_author',
            'filter_callback' => 'bookcrossing_catalog_filter_by_author',
            'index_callback' => 'bookcrossing_catalog_author_index',
        ),
        'genre' => array(
            'title' => t('genre'),
            'path' => 'genre',
            'callback' => 'bookcrossing_catalog_sort_by_genre',
            'grouping_callback' => 'bookcrossing_catalog_get_genre',
            'filter_callback' => 'bookcrossing_catalog_filter_by_genre',
            'index_callback' => 'bookcrossing_catalog_genre_index',
        ),
        'place' => array(
            'title' => t('place'),
            'path' => 'place',
            'callback' => 'bookcrossing_catalog_sort_by_place',
            'grouping_callback' => 'bookcrossing_catalog_get_city',
            'filter_callback' => 'bookcrossing_catalog_filter_by_place',
            'index_callback' => 'bookcrossing_catalog_place_index',
            'sort_form' => 'bookcrossing_catalog_select_country',
        ),
    );

    return $types;
}


/**
 * Helper function: returns catalog view types
 *
 * @return array
 */
function bookcrossing_catalog_get_view_types()
{
    $types = array(
        'tiles' => array(
            'title' => t('tiles'),
            'path' => 'tiles',
        ),
        'covers' => array(
            'title' => t('covers'),
            'path' => 'covers',
        ),
    );

    return $types;
}


/**
 * @param $elements
 * @param $sum
 *
 * @return int
 */
function bookcrossing_catalog_letter_percentage($elements, $sum)
{
    return round($elements / $sum, 1) * 100;
}


/**
 * Output for letters links
 *
 * @param array $letters
 *
 * @return string
 *   Prints HTML output of catalog index
 */
function bookcrossing_catalog_build_index($letters)
{
    $output = '';
    $percentage = 0;
    $max = 0;

    $font_sizes = bookcrossing_catalog_get_index_levels();
    $rus_charset = bookcrossing_get_charset('rus');
    $eng_charset = bookcrossing_get_charset('eng');
    $num_charset = bookcrossing_get_charset('num');

    if (isset($letters)) {
        foreach ($letters as $letter => $elements) {
            if ($elements > $max) $max = $elements;
        }
    }
    else {
        watchdog('bookcrossing.catalog.inc', 'Argument in bookcrossing_catalog_build_index() function is not defined. Check $letters array.', '', WATCHDOG_NOTICE);
    }

    $output .= '<div class="letters-container">';
    foreach ($rus_charset as $letter) {
        $attributes = array();
        if (isset($letters[$letter]) && $max) {
            $percentage = bookcrossing_catalog_letter_percentage($letters[$letter], $max);
            $attributes['style'] = array('font-size: ' . $font_sizes[$percentage] . 'px');
            $attributes['class'] = array('index-level-' . $percentage);
        }

        $query = array();
        $query['sort'] = $letter;
        if (isset($_GET['find']) && !empty($_GET['find'])) {
            $query['find'] = $_GET['find'];
        }

        $output .= isset($letters[$letter]) ? l($letter, $_GET['q'], array('query' => $query, 'attributes' => $attributes)) : '<span>' . $letter . '</span>';
    }
    $output .= '</div>';

    $output .= '<div class="letters-container">';
    foreach ($eng_charset as $letter) {
        $attributes = array();
        if (isset($letters[$letter]) && $max) {
            $percentage = bookcrossing_catalog_letter_percentage($letters[$letter], $max);
            $attributes['style'] = array('font-size: ' . $font_sizes[$percentage] . 'px');
            $attributes['class'] = array('index-level-' . $percentage);
        }
        $query = array();
        $query['sort'] = $letter;
        if (isset($_GET['find']) && !empty($_GET['find'])) {
            $query['find'] = $_GET['find'];
        }
        $output .= isset($letters[$letter]) ? l($letter, $_GET['q'], array('query' => $query, 'attributes' => $attributes)) : '<span>' . $letter . '</span>';
    }
    $output .= '</div>';

    $output .= '<div class="letters-container">';
    $names_with_numbers = FALSE;
    $letters_sum = 0;
    foreach ($num_charset as $num) {
        if (isset($letters[$num])) {
            $names_with_numbers = TRUE;
            $letters_sum += $letters[$num];
            break;
        }
    }
    $query = array();
    $query['sort'] = 'num';
    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $query['find'] = $_GET['find'];
    }
    if ($letters_sum) {
        $percentage = bookcrossing_catalog_letter_percentage($letters_sum, $max);
    }
    $output .= $names_with_numbers ? l('123', $_GET['q'], array('query' => $query, 'attributes' => array('style' => 'font-size: ' . $font_sizes[$percentage] . 'px'))) : '<span>123</span>';
    $output .= '</div>';

    return $output;
}


/**
 * Links for catalog sorting types
 *
 * @param string $current_view_mode
 *
 * @return string
 *   Available sorting types:
 *
 */
function bookcrossing_catalog_build_sort_types($current_view_mode = 'tiles')
{
    $sort_types = bookcrossing_catalog_get_sort_types();
    $view_types = bookcrossing_catalog_get_view_types();

    $output = '';
    foreach ($sort_types as $id => $type) {
        $sort_path = !empty($type['path']) ? '/' . $type['path'] : '';
        $view_path = !empty($view_types[$current_view_mode]['path']) ? '/' . $view_types[$current_view_mode]['path'] : '';

        $path = $sort_path . $view_path;

        $query = array();
        if (isset($_GET['find']) && !empty($_GET['find'])) {
            $query = array(
                'find' => $_GET['find'],
            );
        }

        $output .= l($type['title'], 'books' . $path, array('query' => $query, 'attributes' => array('class' => array('catalog-sort-' . $id))));
    }

    return $output;
}


/**
 * Links for catalog view types
 *
 * @param string $current_sort_type
 *
 * @return string
 */
function bookcrossing_catalog_build_view_types($current_sort_type)
{
    $sort_types = bookcrossing_catalog_get_sort_types();
    $view_types = bookcrossing_catalog_get_view_types();

    $output = '';
    foreach ($view_types as $id => $view_type) {
        $sort_path = !empty($sort_types[$current_sort_type]['path']) ? '/' . $sort_types[$current_sort_type]['path'] : '';
        $view_path = !empty($view_type['path']) ? '/' . $view_type['path'] : '';

        $path = $sort_path . $view_path;

        $parameters = drupal_get_query_parameters();
        $sort = array();
        if (isset($parameters['sort'])) {
            $sort['sort'] = $parameters['sort'];
        }
        if (isset($parameters['find'])) {
            $sort['find'] = $parameters['find'];
        }

        $output .= l($view_type['title'], 'books' . $path, array('query' => $sort, 'attributes' => array('class' => array('catalog-view-' . $id))));
    }

    return $output;
}


/**
 * Form for country select
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function bookcrossing_catalog_select_country($form, $form_state)
{
    $result_form = array();

    $result_form['descr'] = array(
        '#markup' => t('Choose the country'),
    );

    $terms = taxonomy_get_tree(3, 0, 1);
    $options['all'] = t('All countries');
    foreach ($terms as $term) {
        $options[$term->tid] = $term->name;
    }

    $result_form['country'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($_SESSION['country']) ? $_SESSION['country'] : 'all',
    );

    $result_form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('apply filter'),
    );

    return $result_form;
}


/**
 * Submit for selecting country
 *
 * @param array $form
 * @param array $form_state
 */
function bookcrossing_catalog_select_country_submit($form, $form_state)
{
    if ($form_state['values']['country'] != 'all') {
        $_SESSION['country'] = $form_state['values']['country'];
    } else {
        unset($_SESSION['country']);
    }
    //$url = url($_GET['q'], array('query' => drupal_get_query_parameters(), 'absolute' => TRUE));
    //drupal_goto($url);
}


/**
 * Get index for dates
 *
 * @return array
 */
function bookcrossing_catalog_date_index()
{
//    @todo: Implement _catalog_date_index function

    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query->addExpression('LOWER(LEFT(node.title, 1))');
    $query->addExpression('COUNT(*)');

//    if (isset($_GET['find']) && !empty($_GET['find'])) {
//        $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
//        $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');
//        $query->innerJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
//        $query->where('(n.title REGEXP :letter OR i.field_bookcrossing_isbn_value REGEXP :letter OR t.name REGEXP :letter)', array(':letter' => $_GET['find']));
//    }
//
    $letters = $query
        ->distinct()
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->groupBy('LOWER(LEFT(node.title, 1))')
        ->execute()
        ->fetchAllKeyed();

    return $letters;

    return "";
}


/**
 * Get letters for titles
 *
 * @return array
 */
function bookcrossing_catalog_title_index()
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query->addExpression('LOWER(LEFT(node.title, 1))');
    $query->addExpression('COUNT(*)');

    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
        $query->innerJoin('taxonomy_term_data', 'term', 'author.field_bookcrossing_author_tid = term.tid');
        $query->innerJoin('field_data_field_bookcrossing_isbn', 'isbn', 'book.nid = isbn.entity_id');
        $query->where('(node.title REGEXP :letter OR isbn.field_bookcrossing_isbn_value REGEXP :letter OR term.name REGEXP :letter)', array(':letter' => $_GET['find']));
    }

    $letters = $query
        ->distinct()
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->groupBy('LOWER(LEFT(node.title, 1))')
        ->execute()
        ->fetchAllKeyed();

    return $letters;
}


/**
 * Get letters for authors
 */
function bookcrossing_catalog_author_index()
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'author.field_bookcrossing_author_tid = term.tid');
    $query->addExpression('LOWER(LEFT(term.name, 1))');
    $query->addExpression('COUNT(*)');

    dpm($query);


    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $query->innerJoin('node', 'node', 'book.nid = node.nid');
        $query->innerJoin('field_data_field_bookcrossing_isbn', 'isbn', 'book.nid = isbn.entity_id');
        $query->where('(node.title REGEXP :letter OR isbn.field_bookcrossing_isbn_value REGEXP :letter OR term.name REGEXP :letter)', array(':letter' => $_GET['find']));
    }

    $letters = $query
        ->distinct()
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->groupBy('LOWER(LEFT(term.name, 1))')
        ->execute()
        ->fetchAllKeyed();

    return $letters;
}


/**
 * Get letters for books genres
 */
function bookcrossing_catalog_genre_index()
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_genre', 'genre', 'book.nid = genre.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'genre.field_bookcrossing_genre_tid = term.tid');
    $query->addExpression('LOWER(LEFT(term.name, 1))');
    $query->addExpression('COUNT(*)');

    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $query->innerJoin('node', 'node', 'book.nid = node.nid');
        $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
        $query->innerJoin('taxonomy_term_data', 'term_data', 'author.field_bookcrossing_author_tid = term_data.tid');
        $query->innerJoin('field_data_field_bookcrossing_isbn', 'isbn', 'book.nid = isbn.entity_id');
        $query->where('(node.title REGEXP :letter OR isbn.field_bookcrossing_isbn_value REGEXP :letter OR term_data.name REGEXP :letter)', array(':letter' => $_GET['find']));
    }

    $letters = $query
        ->distinct()
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->groupBy('LOWER(LEFT(term.name, 1))')
        ->execute()
        ->fetchAllKeyed();

    return $letters;
}


/**
 * Get index for book places
 */
function bookcrossing_catalog_place_index()
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('taxonomy_term_hierarchy', 'term_tree', 'book.current_place = term_tree.tid');
    $query->innerJoin('taxonomy_term_data', 'term', 'term_tree.parent = term.tid');
    $query->addExpression('LOWER(LEFT(term.name, 1))');
    $query->addExpression('COUNT(*)');

    if (isset($_GET['find']) && !empty($_GET['find'])) {
        $query->innerJoin('node', 'node', 'book.nid = node.nid');
        $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
        $query->innerJoin('taxonomy_term_data', 'term_data', 'author.field_bookcrossing_author_tid = term_data.tid');
        $query->innerJoin('field_data_field_bookcrossing_isbn', 'isbn', 'book.nid = isbn.entity_id');
        $query->where('(node.title REGEXP :letter OR isbn.field_bookcrossing_isbn_value REGEXP :letter OR term_data.name REGEXP :letter)', array(':letter' => $_GET['find']));
    }

    $letters = $query
        ->distinct()
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->groupBy('LOWER(LEFT(term.name, 1))')
        ->execute()
        ->fetchAllKeyed();

    return $letters;
}


/**
 * Returns books with date filtering
 *
 * @param $letter
 * @param int $limit
 *
 * @return array
 */
// @todo: This function needs to be re-written, as it was only copied from bookcrossing_catalog_filter_by_title
function bookcrossing_catalog_filter_by_date($letter, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query = $query->extend('PagerDefault');
    $query->limit($limit);

    $args = array(':letter' => $letter);
    $condition = ' = :letter';
    if ($letter == 'num') {
        $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
        $args = array();
    }

    $bids = $query
        ->fields('book', array('bid'))
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->where('LEFT(node.title, 1) ' . $condition, $args)
        ->orderBy("node.title")
        ->execute()
        ->fetchCol();

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }
    //$books = bookcrossing_load_multiple($bids);
    return $books;
}


/**
 * Returns books with title starts with $letter
 *
 * @param $letter
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_filter_by_title($letter, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query = $query->extend('PagerDefault');
    $query->limit($limit);

    $args = array(':letter' => $letter);
    $condition = ' = :letter';
    if ($letter == 'num') {
        $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
        $args = array();
    }

    $bids = $query
        ->fields('book', array('bid'))
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->where('LEFT(node.title, 1) ' . $condition, $args)
        ->orderBy("node.title")
        ->execute()
        ->fetchCol();

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }
    //$books = bookcrossing_load_multiple($bids);
    return $books;
}


/**
 * Returns books with title starts with $letter
 *
 * @param $letter
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_filter_by_author($letter, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'author.field_bookcrossing_author_tid = term.tid');

    // if ($letter == 'num') {
    $query = $query->extend('PagerDefault');
    $query->limit($limit);
    //}

    $args = array(':letter' => $letter);
    $condition = ' = :letter';
    if ($letter == 'num') {
        $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
        $args = array();
    }

    $bids = $query
        ->fields('book', array('bid'))
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->where('LEFT(term.name, ' . drupal_strlen($letter) . ') ' . $condition, $args)
        ->execute()
        ->fetchCol();

//  if ($letter != 'num') {
//    $query = db_select('bookcrossing_books', 'b');
//    $query->innerJoin('field_data_field_bookcrossing_genre', 'a', 'b.nid = a.entity_id');
//    $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_genre_tid = t.tid');
//    $query = $query->extend('PagerDefault');
//    $query->limit($limit);
//
//    $bids = $query
//        ->fields('b', array('bid'))
//        ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
//        ->condition('b.bid', $bids, 'IN')
//        ->execute()
//        ->fetchCol();
//  }

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Returns books with title starts with $letter
 *
 * @param $letter
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_filter_by_genre($letter, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_genre', 'genre', 'book.nid = genre.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'genre.field_bookcrossing_genre_tid = term.tid');
    $query = $query->extend('PagerDefault');
    $query->limit($limit);

    $args = array(':letter' => $letter);
    $condition = ' = :letter';
    if ($letter == 'num') {
        $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
        $args = array();
    }

    $bids = $query
        ->fields('book', array('bid'))
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->where('LEFT(term.name, ' . drupal_strlen($letter) . ') ' . $condition, $args)
        ->execute()
        ->fetchCol();

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Returns books with title starts with $letter
 *
 * @param string $letter
 * @param int $limit
 * @param string $place
 *
 * @return array
 */
function bookcrossing_catalog_filter_by_place($letter = '', $limit = 10, $place = '')
{
    $tids = array();
    if (isset($_GET['country'])) {
        $_SESSION['country'] = db_select('taxonomy_term_data', 'term')
            ->fields('term', array('tid'))
            ->condition('term.name', $_GET['country'], '=')
            ->execute()
            ->fetchField();
    }
    if (isset($_SESSION['country'])) {
        $terms = taxonomy_get_children_all($_SESSION['country'], 3);
        foreach ($terms as $term) {
            $tids[] = $term->tid;
        }
    }
    if (empty($tids) && isset($_SESSION['country'])) {
        return array();
    }
    if (isset($_GET['place'])) {
        $query = db_select('bookcrossing_books', 'book');
        $query->innerJoin('taxonomy_term_data', 'term', 'book.current_place = term.tid');
    } else {
        $query = db_select('bookcrossing_books', 'book');
        $query->innerJoin('taxonomy_term_hierarchy', 'term_tree', 'book.current_place = term_tree.tid');
        $query->innerJoin('taxonomy_term_data', 'term', 'term_tree.parent = term.tid');
    }

    $args = array(':letter' => $letter);
    $condition = ' = :letter';
    if ($letter == 'num') {
        $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
        $args = array();
    }

    $query->fields('book', array('bid'));
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

    if (!empty($tids)) {
        $query->condition('term.tid', $tids, 'IN');
    }

    $query->where('LEFT(term.name, ' . drupal_strlen($letter) . ') ' . $condition, $args);
    $query = $query->extend('PagerDefault');
    $query->limit($limit);
    $query->orderBy('term.name', 'ASC');
    $bids = $query->execute()->fetchCol();

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Sorts books by date
 *
 * @param $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_sort_by_date($mode, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query->innerJoin('bookcrossing_history', 'history', 'book.bid = history.book_id');
    $query->fields('book', array('bid'));
    $query->distinct();
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
        $query = $query->limit($limit);
    }

    $query->orderBy('history.date_time', 'DESC');

    $bids = $query->execute()->fetchCol();

//  $books = bookcrossing_load_multiple($bids);
    $books = array();

    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Sorts books by title
 *
 * @param $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_sort_by_title($mode, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query->fields('book', array('bid'));
    $query->distinct();
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
        $query = $query->limit($limit);
    }

//    $query->orderBy("if(node.title regexp '^[а-яА-Я]', concat('1', node.title), node.title)");
//  @todo: Need to filter out preceding white-spaces and special symbols, so that sorting is done based on letters and digits only
    $rexp = '[А-Яа-яA-Za-z0-9].*';
    $query->orderBy("IF(node.title REGEXP '^[а-яА-Я]', CONCAT('1', node.title) REGEXP '[А-Яа-яA-Za-z0-9].*', node.title REGEXP '[А-Яа-яA-Za-z0-9].*')");
    $bids = $query->execute()->fetchCol();

//  $books = bookcrossing_load_multiple($bids);
    $books = array();

    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Sorts by books author
 *
 * @param $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_sort_by_author($mode, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'author.field_bookcrossing_author_tid = term.tid');

    if ($mode == 'covers') {
        $query->distinct();
    }

    $query->fields('book', array('bid'));
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
    $query->addExpression('LOWER(LEFT(term.name, 1))', 'letter');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
        $query = $query->limit($limit);
    }

    $query->orderBy("if(term.name regexp '^[а-яА-Я]', concat('1', term.name), term.name)");
    //$query->orderBy("if(LEFT(t.name, 1) regexp '^[а-яА-Я]', concat('a', LEFT(t.name, 1)), LEFT(t.name, 1))");
    $info = $query->execute()->fetchAll();

    $bids = array();
    $_SESSION['letters'] = array();
    foreach ($info as $bid) {
        $_SESSION['letters'][] = $bid->letter;
        $bids[] = $bid->bid;
    }

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }
    return $books;
}


/**
 * Sorts by books genre
 *
 * @param $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_sort_by_genre($mode, $limit = 10)
{
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('field_data_field_bookcrossing_genre', 'genre', 'book.nid = genre.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'genre.field_bookcrossing_genre_tid = term.tid');

    if ($mode == 'covers') {
        $query->distinct();
    }

    $query->fields('book', array('bid'));
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
    $query->addExpression('LOWER(LEFT(term.name, 1))', 'letter');

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
        $query = $query->limit($limit);
    }

    $query->orderBy("if(term.name regexp '^[а-яА-Я]', concat('1', term.name), term.name)");
    //$query->orderBy("if(t.name regexp '^[а-яА-Я]', concat('a', t.name), t.name)");
    $info = $query->execute()->fetchAll();

    $bids = array();
    $_SESSION['letters'] = array();

    foreach ($info as $bid) {
        $_SESSION['letters'][$bid->letter] = $bid->letter;
        $bids[] = $bid->bid;
    }

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Sorts by books place
 *
 * @param $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_sort_by_place($mode, $limit = 10)
{
    $tids = array();
    if (isset($_SESSION['country'])) {
        $terms = taxonomy_get_children($_SESSION['country'], 3);
        foreach ($terms as $term) {
            $tids[] = $term->tid;
        }
    }
    if (empty($tids) && isset($_SESSION['country'])) {
        return array();
    }

    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('taxonomy_term_hierarchy', 'term_tree', 'book.current_place = term_tree.tid');
    $query->innerJoin('taxonomy_term_data', 'term', 'term_tree.parent = term.tid');
    $query->fields('book', array('bid'));
    $query->distinct();
    $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

    if (!empty($tids)) {
        $query->condition('term.tid', $tids, 'IN');
    }

    $query = $query->extend('PagerDefault');
    if ($limit !== FALSE) {
        $query = $query->limit($limit);
    }

    $query->orderBy("if(term.name regexp '^[а-яА-Я]', concat('1', term.name), term.name)");
    //$query->orderBy("if(t.name regexp '^[а-яА-Я]', concat('a', t.name), t.name)");
    $bids = $query->execute()->fetchCol();

    //$books = bookcrossing_load_multiple($bids);
    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * Helper function: returns first letter of the books title.
 *
 * @param object $book
 *      Object of the book.
 * @param array $charset_types
 *   Array of charset types (e.g. 'rus', 'eng', 'num') that needs to be explored.
 *
 * @return string
 *   Capitalized first character that found in one of the charsets.
 *   # in case character not found in any of charsets.
 */
// @todo: Implement watchdog for this function
function bookcrossing_catalog_get_title_char($book, $charset_types)
{
    foreach ($charset_types as $type) {
        $charset[$type] = bookcrossing_get_charset($type);
    }
    for ($i=0; $i < strlen($book['node']->title); $i++)
    { 
    	$char = drupal_strtoupper( drupal_substr($book['node']->title, $i, 1) );
        foreach ($charset_types as $type) {
            if (isset($charset[$type][$char])) {
                return $char;
            }
        }
    }
    return '#';
}


/**
 * Helper function: returns book release date
 *
 * @param $book
 *
 * @return string
 */
function bookcrossing_catalog_get_date($book)
{
/*
    static $bids = array();
    $bids[$book['bid']] = isset($bids[$book['bid']]) ? $bids[$book['bid']] + 1 : 0;

    if (isset($_GET['sort'])) {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_author[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_author[LANGUAGE_NONE][$i]['tid']);
            if ($_GET['sort'] == drupal_strtolower(drupal_substr($term->name, 0, drupal_strlen($_GET['sort'])))) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    } else {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_author[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_author[LANGUAGE_NONE][$i]['tid']);
            if (in_array(drupal_strtolower(drupal_substr($term->name, 0, 1)), $_SESSION['letters'])) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    }

    if (isset($_GET['sort'])) {
        $name = $term->name;
    } else {
        $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
    }

    return $name;
*/

    return "date";
}


/**
 * Helper function: returns author name
 *
 * @param $book
 *
 * @return string
 */
function bookcrossing_catalog_get_author($book)
{
    static $bids = array();
    $bids[$book['bid']] = isset($bids[$book['bid']]) ? $bids[$book['bid']] + 1 : 0;

//  $term = taxonomy_term_load($book['node']->field_bookcrossing_author[LANGUAGE_NONE][$bids[$book['bid']]]['tid']);
//
//  if (isset($_GET['sort'])) {
//    $letter = drupal_strtolower(drupal_substr($term->name, 0, 1));
//    if ($letter == $_GET['sort']) {
//      $name = $term->name;
//    }
//    else {
//      $name = bookcrossing_catalog_get_genre($book);
//    }
//  }
//  else {
//    $name = $term->name;
//  }

    if (isset($_GET['sort'])) {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_author[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_author[LANGUAGE_NONE][$i]['tid']);
            if ($_GET['sort'] == drupal_strtolower(drupal_substr($term->name, 0, drupal_strlen($_GET['sort'])))) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    } else {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_author[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_author[LANGUAGE_NONE][$i]['tid']);
            if (in_array(drupal_strtolower(drupal_substr($term->name, 0, 1)), $_SESSION['letters'])) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    }

    if (isset($_GET['sort'])) {
        $name = $term->name;
    } else {
        $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
    }

    return $name;
}


/**
 * Helper function: returns books genre
 *
 * @param $book
 *
 * @return string
 */
function bookcrossing_catalog_get_genre($book)
{
    static $bids = array();
    $bids[$book['bid']] = isset($bids[$book['bid']]) ? $bids[$book['bid']] + 1 : 0;

    if (isset($_GET['sort'])) {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_genre[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_genre[LANGUAGE_NONE][$i]['tid']);
            if ($_GET['sort'] == drupal_strtolower(drupal_substr($term->name, 0, drupal_strlen($_GET['sort'])))) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    } else {
        for ($i = $bids[$book['bid']]; $i < count($book['node']->field_bookcrossing_genre[LANGUAGE_NONE]); $i++) {
            $term = taxonomy_term_load($book['node']->field_bookcrossing_genre[LANGUAGE_NONE][$i]['tid']);
            if (in_array(drupal_strtolower(drupal_substr($term->name, 0, 1)), $_SESSION['letters'])) {
                $bids[$book['bid']] = $i;
                break;
            }
        }
    }

//  if (isset($_GET['sort'])) {
//    $letter = drupal_strtolower(drupal_substr($term->name, 0, 1));
//    if ($letter == $_GET['sort']) {
//      $name = $term->name;
//    }
//    else {
//      //$name = bookcrossing_catalog_get_genre($book);
//    }
//  }
//  else {
//    $name = $term->name;
//  }

    if (isset($_GET['sort'])) {
        $name = $term->name;
    } else {
        $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
    }

    return $name;
}


/**
 * Helper function: returns books city
 *
 * @param $book
 * @param bool $trim
 *
 * @return string
 */
function bookcrossing_catalog_get_city($book, $trim = FALSE)
{
    if (isset($_GET['place'])) {
        $term = $book['status'] ? $book['place'] : taxonomy_term_load($book['place_id_left']);
    } else {
        $parents = $book['status'] ? taxonomy_get_parents($book['place']->tid) : taxonomy_get_parents($book['place_id_left']);
        $term = reset($parents);
    }

    if (isset($_GET['sort']) || isset($_GET['place']) || $trim) {
        $name = $term->name;
    } elseif (!$trim) {
        $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
    }

    return $name;
}


/**
 * Output for catalog sorting with a tiles view mode
 *
 * @param string $sort
 * @param array $books
 *
 * @return string
 */
function bookcrossing_catalog_view_tiles($sort, $books)
{
    $sort_types = bookcrossing_catalog_get_sort_types();

/**
 * Defines function used to group book items
 * See definition of bookcrossing_catalog_get_{$sort}() function
 */
    $catalog_get_function = $sort_types[$sort]['grouping_callback'];
    if (!function_exists($catalog_get_function)) {
        watchdog('bookcrossing.catalog.inc', 'Function %func referenced in bookcrossing_catalog_view_tiles() is not defined!', array('%func' => $catalog_get_function), WATCHDOG_ERROR);
        return '';
    }

    $letters = array();
    foreach ($books as $book) {
        $letters[$catalog_get_function($book, ['rus', 'eng', 'num'])][] = $book;
    }

    uksort($letters, 'bookcrossingRusFirst');

    $output = '';
    $l = '';
    foreach ($letters as $group_name => $books) {
        $group_descr = '';
        if ($sort != 'title' && $l != $group_name && drupal_strlen($group_name) == 1) {
            if ($sort == 'place') {
                $names = db_select('taxonomy_term_data', 'term')
                    ->fields('t', array('name'))
                    ->where('LOWER(LEFT(t.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
                    ->condition('term.vid', BOOKCROSSING_PLACES_VID, '=')
                    ->orderBy('term.name')
                    ->execute()
                    ->fetchCol();

                $start = $names[0];
                $end = $names[count($names) - 1];
            } elseif ($sort == 'genre') {
                $names = db_select('taxonomy_term_data', 'term')
                    ->fields('term', array('name'))
                    ->where('LOWER(LEFT(term.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
                    ->condition('term.vid', 1, '=')
                    ->orderBy('term.name')
                    ->execute()
                    ->fetchCol();

                $start = $names[0];
                $end = $names[count($names) - 1];
            } elseif ($sort == 'author') {
                $names = db_select('taxonomy_term_data', 'term')
                    ->fields('term', array('name'))
                    ->where('LOWER(LEFT(term.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
                    ->condition('term.vid', 2, '=')
                    ->orderBy('term.name')
                    ->execute()
                    ->fetchCol();

                $start = $names[0];
                $end = $names[count($names) - 1];
            }

            if ($start != $end) {
                $group_descr = ' <span>(' . $start . ' - ' . $end . ')</span>';
            } else {
                $group_descr = ' <span>(' . $start . ')</span>';
            }
            $l = $group_name;
        }

        $output .= '<div class="catalog-container">';
        $output .= '<div class="catalog-group">' . $group_name . $group_descr . '</div>';
        $output .= '<div class="catalog-container-content">';
        foreach ($books as $book) {
            $view = node_view($book['node'], 'catalog');
            $view = bookcrossing_prepare_book_view($view, $book, FALSE);

            $status_string = $book['status'] ? t('Found') : t('Travelling');
            $view['book_status_string'] = array(
                '#markup' => '<div class="book-status-string">' . $status_string . '</div>',
            );

            $view['author_and_year'] = array(
                '#markup' => bookcrossing_author_and_year($book),
            );

            $output .= render($view);
        }
        $output .= '</div><div style="clear: both"></div></div>';
    }

    return $output;
}


/**
 * Output for catalog sorting with a covers view mode
 *
 * @param string $sort
 * @param array $books
 *
 * @return string
 */
function bookcrossing_catalog_view_covers($sort, $books)
{
    $output = '<div class="covers-container">';

    foreach ($books as $book) {
        $view = node_view($book['node'], 'book_cover');
        $view = bookcrossing_prepare_book_view($view, $book);
        $output .= render($view);
    }

    $output .= '</div>';

    return $output;
}


/**
 * @param $a
 * @param $b
 *
 * @return int
 */
function bookcrossingRusFirst($a, $b)
{
    $a = drupal_strtolower($a);
    $b = drupal_strtolower($b);
    $rus_charset = bookcrossing_get_charset('ru');
    $eng_charset = bookcrossing_get_charset('en');
    $num_charset = bookcrossing_get_charset('#');

    if (isset($rus_charset[$a]) && isset($rus_charset[$b])) {
        return strcmp($rus_charset[$a], $rus_charset[$b]);
    }
    if (isset($rus_charset[$a]) && isset($eng_charset[$b])) {
        return -1;
    }
    if (isset($rus_charset[$b]) && isset($eng_charset[$a])) {
        return 1;
    }
    if (isset($eng_charset[$a]) && isset($eng_charset[$b])) {
        return strcmp($eng_charset[$a], $eng_charset[$b]);
    }
    if (isset($num_charset[$a]) && isset($num_charset[$b])) {
        if ($a < $b) {
            return -1;
        } else {
            return 1;
        }
    }
    if (isset($num_charset[$a]) && !isset($num_charset[$b])) {
        return 1;
    }
    if (!isset($num_charset[$a]) && isset($num_charset[$b])) {
        return -1;
    }

    return -2;
}


/**
 * Finds books
 *
 * @param string $type
 * @param string $mode
 * @param int $limit
 *
 * @return array
 */
function bookcrossing_catalog_find_books($type, $mode, $limit = 10)
{
    if (!isset($_GET['find']) || empty($_GET['find'])) {
        return array();
    }

    /**
     * @todo Add ability to search for 13-digit ISBN that starts from 978 even if 10-digit ISBN was entered
     * @todo Ignore spaces and - while searching by ISBN
     */
    /* 
    $search = preg_replace("/^978/",'',$_GET['find']);
    $search = strtolower(str_replace('-', '', $search));
    $search = strtolower(str_replace(' ', '', $search));
    */
    $query = db_select('bookcrossing_books', 'book');
    $query->innerJoin('node', 'node', 'book.nid = node.nid');
    $query->innerJoin('field_data_field_bookcrossing_author', 'author', 'book.nid = author.entity_id');
    $query->innerJoin('taxonomy_term_data', 'term', 'author.field_bookcrossing_author_tid = term.tid');
    $query->leftJoin('field_data_field_bookcrossing_isbn', 'isbn', 'book.nid = isbn.entity_id');

    if ($mode == 'covers') {
        $query->distinct();
    }

    switch ($type) {
        case 'title':
            $query->orderBy("if(node.title regexp '^[а-яА-Я]', concat('author', node.title), node.title)");
            if (isset($_GET['sort'])) {
                $left = ' AND LEFT(node.title, ' . drupal_strlen($_GET['sort']) . ') = :sort';
            }
        break;
        case 'author':
            $query->orderBy("if(LEFT(term.name, 1) regexp '^[а-яА-Я]', concat('author', LEFT(term.name, 1)), LEFT(term.name, 1))");
            if (isset($_GET['sort'])) {
                $left = ' AND LEFT(term.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
            } else {
                $query->addExpression('LEFT(term.name, 1)', 'letter');
                $left = '';
            }
        break;
        case 'genre':
            $query->innerJoin('field_data_field_bookcrossing_genre', 'genre', 'book.nid = genre.entity_id');
            $query->innerJoin('taxonomy_term_data', 'term', 'genre.field_bookcrossing_genre_tid = term.tid');
            $query->orderBy("if(term.name regexp '^[а-яА-Я]', concat('author', term.name), term.name)");
            if (isset($_GET['sort'])) {
                $left = ' AND LEFT(term.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
            } else {
                $query->addExpression('LEFT(term.name, 1)', 'letter');
                $left = '';
            }
        break;
        case 'place':
            $query->innerJoin('taxonomy_term_hierarchy', 'term_tree', 'book.current_place = term_tree.tid');
            $query->innerJoin('taxonomy_term_data', 'term', 'term_tree.parent = term.tid');
            $query->orderBy('term.name', 'ASC');
            if (isset($_GET['sort'])) {
                $left = ' AND LEFT(term.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
            }
        break;
        default:
            $left = '';
    }

    $query->distinct();
    $query = $query->extend('PagerDefault');
    $query->limit($limit);
    $info = $query
        ->fields('book', array('bid'))
        ->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
        ->where('(LOWER(node.title) REGEXP LOWER(:letter) OR LOWER(isbn.field_bookcrossing_isbn_value) REGEXP LOWER(:letter) OR LOWER(term.name) REGEXP LOWER(:letter))' . $left, array(':letter' => $_GET['find'], ':sort' => isset($_GET['sort']) ? $_GET['sort'] : ''))
        ->execute()
        ->fetchAll();

    $bids = array();
    $_SESSION['letters'] = array();

    foreach ($info as $bid) {
        if (isset($bid->letter)) {
            $_SESSION['letters'][] = $bid->letter;
        }

        $bids[] = $bid->bid;
    }

    $books = array();
    foreach ($bids as $bid) {
        $books[] = bookcrossing_load($bid);
    }

    return $books;
}


/**
 * @param $tid
 * @param int $vid
 * @param string $key
 *
 * @return array
 */
function taxonomy_get_children_all($tid, $vid = 0, $key = 'tid')
{
    $c = taxonomy_get_children($tid, $vid);
    $result = array();
    foreach ($c as $t => $d) {
        $result[$t] = $d;
        $below = taxonomy_get_children_all($t, $vid, $key);
        if (!empty($below)) {
            foreach ($below as $nt => $nd) {
                $result[$nt] = $nd;
            }
        }
    }
    return $result;
}
