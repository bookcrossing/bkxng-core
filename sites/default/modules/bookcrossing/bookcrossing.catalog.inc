<?php

/**
 * @file
 * Catalog functionality for bookcrossing site.
 */

/**
 * Menu callback: catalog page.
 */
function bookcrossing_catalog_page($type = 'title', $mode = 'tile') {
  $limit = 30;
  $sort_types = bookcrossing_catalog_get_sort_types();

//  if (!isset($_SESSION['type'])) {
//    $_SESSION['catalog_type'] = $type;
//  }
//  
//  if ($_SESSION['catalog_type'] != $type) {
//    $_SESSION['catalog_type'] = $type;
//    $_SESSION['catalog_bids'] = array();
//  }
//  
  $books = array();

  $sort_form = '';
  if (isset($sort_types[$type]['sort_form'])) {
    $sort_form = drupal_get_form($sort_types[$type]['sort_form']);
  }

  if ((isset($_GET['sort']) || isset($_GET['place']) || isset($_GET['country'])) && !isset($_GET['find'])) {
    $function = isset($sort_types[$type]['filter_callback']) ? $sort_types[$type]['filter_callback'] : '';
    if (!function_exists($function)) {
      return '';
    }
    
    if (isset($_GET['place'])) {
      $books = $function($_GET['place']);
    }
    elseif (isset($_GET['country'])) {
      $books = $function();
    }
    else {
      $books = $function($_GET['sort']);
    }
  }
  elseif (isset($_GET['find'])) {
    $books = bookcrossing_catalog_find_books($type, $mode);
  }
  else {
    $function = isset($sort_types[$type]['callback']) ? $sort_types[$type]['callback'] : '';
    if (!function_exists($function)) {
      drupal_not_found();
      drupal_exit();
    }

    $books = $function($mode);
  }

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
  $output .= '<div class="catalog-sorts">';
  $output .= bookcrossing_catalog_sort_types($mode);
  $output .= '</div>';
  $output .= '<div class="catalog-view-types">';
  $output .= bookcrossing_catalog_view_types($type);
  $output .= '</div>';
  $output .= paginator3000pager();
  $output .= '</div>';

  $output .= render($sort_form);

  $function = 'bookcrossing_catalog_sort_' . $mode;
  if (function_exists($function)) {
    $output .= $function($type, $books);
  }

  $letters = array();
  $function = $sort_types[$type]['letters_callback'];
  if (function_exists($function)) {
    $letters = $function();
  }
  
  $output .= bookcrossing_catalog_letters($letters);
//  drupal_set_message('<pre>' . print_r($letters, 1) . '</pre>');

  return $output;
}

/**
 * Levels of letter links
 */
function bookcrossing_catalog_get_letters_levels() {
  $defaults = array(
    '0'  => '14',
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
 * 
 */
function bookcrossing_catalog_letter_percentage($elements, $sum) {
//  return ceil($elements / $sum / 0.1) * 0.1;
  return round($elements / $sum, 1) * 100;
}

/**
 * Output for letters links.
 */
function bookcrossing_catalog_letters($letters) {
  $output = '';

  $rus_charset = drupal_map_assoc(array('а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'Ъ', 'ы', 'ь', 'э', 'ю', 'я'));
  $eng_charset = drupal_map_assoc(array('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'));
  $nums = range(0, 9);

  $font_sizes = bookcrossing_catalog_get_letters_levels();

//  $rus_letters_sum = 0;
//  $eng_letters_sum = 0;
  $total = 0;
  foreach ($letters as $letter => $elements) {
//    if (isset($rus_charset[$letter])) {
//      $rus_letters_sum += $elements;
//    }
//    elseif (isset($eng_charset[$letter])) {
//      $eng_letters_sum += $elements;
//    }
    $total += $elements;
  }

  $output = '<div class="letters-wrapper">';
  $output .= '<div class="letters-container">';
  foreach ($rus_charset as $letter) {
    $attributes = array();
    if (isset($letters[$letter]) && $total) {
      $percentage = bookcrossing_catalog_letter_percentage($letters[$letter], $total);
      $attributes['style'] = array('font-size: ' . $font_sizes[$percentage] . 'px');
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
    if (isset($letters[$letter]) && $total) {
      $percentage = bookcrossing_catalog_letter_percentage($letters[$letter], $total);
      $attributes['style'] = array('font-size: ' . $font_sizes[$percentage] . 'px');
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
  foreach ($nums as $num) {
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
    $percentage = bookcrossing_catalog_letter_percentage($letters_sum, $total);
  }

  $output .= $names_with_numbers ? l('123', $_GET['q'], array('query' => $query, 'attributes' => array('style' => 'font-size: ' . $font_sizes[$percentage] . 'px'))) : '<span>123</span>';
  $output .= '</div>';

  $output .= '</div>';

  return $output;
}

/**
 * Links for catalog sorting types.
 */
function bookcrossing_catalog_sort_types($mode = 'tile') {
  $sort_types = bookcrossing_catalog_get_sort_types();
  $view_types = bookcrossing_catalog_get_view_types();

  $output = '';
  foreach ($sort_types as $id => $type) {
    $sort_path = !empty($type['path']) ? '/' . $type['path'] : '';
    $view_path = !empty($view_types[$mode]['path']) ? '/' . $view_types[$mode]['path'] : '';

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
 * Helper function: get list of sorting types.
 * @todo filter callback
 */
function bookcrossing_catalog_get_sort_types() {
  $types = array(
    'title' => array(
      'title' => t('title'),
      'path' => '',
      'grouping_callback' => 'bookcrossing_title_first_letter',
      'callback' => 'bookcrossing_catalog_sort_by_title',
      'filter_callback' => 'bookcrossing_catalog_filter_by_title',
      'letters_callback' => 'bookcrossing_catalog_title_letters',
    ),
    'author' => array(
      'title' => t('author'),
      'path' => 'author',
      'grouping_callback' => 'bookcrossing_get_author',
      'callback' => 'bookcrossing_catalog_sort_by_author',
      'filter_callback' => 'bookcrossing_catalog_filter_by_author',
      'letters_callback' => 'bookcrossing_catalog_author_letters',
    ),
    'genre' => array(
      'title' => t('genre'),
      'path' => 'genre',
      'grouping_callback' => 'bookcrossing_get_genre',
      'callback' => 'bookcrossing_catalog_sort_by_genre',
      'filter_callback' => 'bookcrossing_catalog_filter_by_genre',
      'letters_callback' => 'bookcrossing_catalog_genre_letters',
    ),
    'place' => array(
      'title' => t('place'),
      'path' => 'place',
      'grouping_callback' => 'bookcrossing_get_city',
      'callback' => 'bookcrossing_catalog_sort_by_place',
      'filter_callback' => 'bookcrossing_catalog_filter_by_place',
      'letters_callback' => 'bookcrossing_catalog_place_letters',
      'sort_form' => 'bookcrossing_catalog_select_country',
    ),
  );

  return $types;
}

/**
 * Form for country select.
 */
function bookcrossing_catalog_select_country($form, $form_state) {
  $form = array();

  $form['descr'] = array(
    '#markup' => t('Choose the country'),
  );

  $terms = taxonomy_get_tree(3, 0, 1);
  $options['all'] = t('All countries');
  foreach ($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  $form['country'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($_SESSION['country']) ? $_SESSION['country'] : 'all',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('apply filter'),
  );

  return $form;
}

/**
 * Submit for selecting country.
 */
function bookcrossing_catalog_select_country_submit($form, $form_state) {
  if ($form_state['values']['country'] != 'all') {
    $_SESSION['country'] = $form_state['values']['country'];
  }
  else {
    unset($_SESSION['country']);
  }
  //$url = url($_GET['q'], array('query' => drupal_get_query_parameters(), 'absolute' => TRUE));
  //drupal_goto($url);
}

/**
 * Get letters for titles
 */
function bookcrossing_catalog_title_letters() {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('node', 'n', 'b.nid = n.nid');
  $query->addExpression('LOWER(LEFT(n.title, 1))');
  $query->addExpression('COUNT(*)');
  
  if (isset($_GET['find']) && !empty($_GET['find'])) {   
    $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
    $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');
    $query->innerJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
    $query->where('(n.title REGEXP :letter OR i.field_bookcrossing_isbn_value REGEXP :letter OR t.name REGEXP :letter)', array(':letter' => $_GET['find']));
  }

  $letters = $query
      ->distinct()
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->groupBy('LOWER(LEFT(n.title, 1))')
      ->execute()
      ->fetchAllKeyed();

  return $letters;
}

/**
 * Get letters for authors
 */
function bookcrossing_catalog_author_letters() {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');
  $query->addExpression('LOWER(LEFT(t.name, 1))');
  $query->addExpression('COUNT(*)');

  if (isset($_GET['find']) && !empty($_GET['find'])) {   
    $query->innerJoin('node', 'n', 'b.nid = n.nid');
    $query->innerJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
    $query->where('(n.title REGEXP :letter OR i.field_bookcrossing_isbn_value REGEXP :letter OR t.name REGEXP :letter)', array(':letter' => $_GET['find']));
  }
  
  $letters = $query
      ->distinct()
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->groupBy('LOWER(LEFT(t.name, 1))')
      ->execute()
      ->fetchAllKeyed();

  return $letters;
}

/**
 * Get letters for books genres.
 */
function bookcrossing_catalog_genre_letters() {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_genre', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_genre_tid = t.tid');
  $query->addExpression('LOWER(LEFT(t.name, 1))');
  $query->addExpression('COUNT(*)');
  
  if (isset($_GET['find']) && !empty($_GET['find'])) {   
    $query->innerJoin('node', 'n', 'b.nid = n.nid');
    $query->innerJoin('field_data_field_bookcrossing_author', 'f', 'b.nid = f.entity_id');
    $query->innerJoin('taxonomy_term_data', 'p', 'f.field_bookcrossing_author_tid = p.tid');
    $query->innerJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
    $query->where('(n.title REGEXP :letter OR i.field_bookcrossing_isbn_value REGEXP :letter OR p.name REGEXP :letter)', array(':letter' => $_GET['find']));
  }

  $letters = $query
      ->distinct()
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->groupBy('LOWER(LEFT(t.name, 1))')
      ->execute()
      ->fetchAllKeyed();

  return $letters;
}

/**
 * Get letters for book places.
 */
function bookcrossing_catalog_place_letters() {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('taxonomy_term_hierarchy', 'h', 'b.current_place = h.tid');
  $query->innerJoin('taxonomy_term_data', 't', 'h.parent = t.tid');
  $query->addExpression('LOWER(LEFT(t.name, 1))');
  $query->addExpression('COUNT(*)');

  if (isset($_GET['find']) && !empty($_GET['find'])) {   
    $query->innerJoin('node', 'n', 'b.nid = n.nid');
    $query->innerJoin('field_data_field_bookcrossing_author', 'f', 'b.nid = f.entity_id');
    $query->innerJoin('taxonomy_term_data', 'p', 'f.field_bookcrossing_author_tid = p.tid');
    $query->innerJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
    $query->where('(n.title REGEXP :letter OR i.field_bookcrossing_isbn_value REGEXP :letter OR p.name REGEXP :letter)', array(':letter' => $_GET['find']));
  }
  
  $letters = $query
      ->distinct()
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->groupBy('LOWER(LEFT(t.name, 1))')
      ->execute()
      ->fetchAllKeyed();

  return $letters;
}

/**
 * Links for catalog view types.
 */
function bookcrossing_catalog_view_types($sort_type = 'title') {
  $sort_types = bookcrossing_catalog_get_sort_types();
  $view_types = bookcrossing_catalog_get_view_types();

  $output = '';
  foreach ($view_types as $id => $type) {
    $sort_path = !empty($sort_types[$sort_type]['path']) ? '/' . $sort_types[$sort_type]['path'] : '';
    $view_path = !empty($type['path']) ? '/' . $type['path'] : '';

    $path = $sort_path . $view_path;

    $parameters = drupal_get_query_parameters();
    $sort = array();
    if (isset($parameters['sort'])) {
      $sort['sort'] = $parameters['sort'];
    }
    if (isset($parameters['find'])) {
      $sort['find'] = $parameters['find'];
    }

    $output .= l($type['title'], 'books' . $path, array('query' => $sort, 'attributes' => array('class' => array('catalog-view-' . $id))));
  }

  return $output;
}

/**
 * Returns books with title starts with $letter
 */
function bookcrossing_catalog_filter_by_title($letter, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('node', 'n', 'b.nid = n.nid');
  $query = $query->extend('PagerDefault');
  $query->limit($limit);

  $args = array(':letter' => $letter);
  $condition = ' = :letter';
  if ($letter == 'num') {
    $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
    $args = array();
  }

  $bids = $query
      ->fields('b', array('bid'))
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->where('LEFT(n.title, 1) ' . $condition, $args)
      ->execute()
      ->fetchCol();

  $books = bookcrossing_load_multiple($bids);

  return $books;
}

/**
 * Returns books with title starts with $letter
 */
function bookcrossing_catalog_filter_by_author($letter, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');

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
      ->fields('b', array('bid'))
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->where('LEFT(t.name, ' . drupal_strlen($letter) . ') ' . $condition, $args)
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
 */
function bookcrossing_catalog_filter_by_genre($letter, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_genre', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_genre_tid = t.tid');
  $query = $query->extend('PagerDefault');
  $query->limit($limit);

  $args = array(':letter' => $letter);
  $condition = ' = :letter';
  if ($letter == 'num') {
    $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
    $args = array();
  }

  $bids = $query
      ->fields('b', array('bid'))
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->where('LEFT(t.name, ' . drupal_strlen($letter) . ') ' . $condition, $args)
      ->execute()
      ->fetchCol();

  $books = array();
  foreach ($bids as $bid) {
    $books[] = bookcrossing_load($bid);
  }

  return $books;
}

function taxonomy_get_children_all($tid, $vid = 0, $key = 'tid'){
	$c = taxonomy_get_children($tid, $vid, $key);
	$result = array();
	foreach ($c as $t => $d){
		$result[$t] = $d;
		$below = taxonomy_get_children_all($t, $vid, $key);
		if (!empty($below)) {
			foreach ($below as $nt => $nd){
				$result[$nt] = $nd;
			}
		}
	}
	return $result;
}

/**
 * Returns books with title starts with $letter
 */
function bookcrossing_catalog_filter_by_place($letter = '', $limit = 30) {
  $tids = array();

  if (isset($_GET['country'])) {
    
    $_SESSION['country'] = db_select('taxonomy_term_data', 't')->fields('t', array('tid'))->condition('t.name', $_GET['country'], '=')->execute()->fetchField();
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
    $query = db_select('bookcrossing_books', 'b');
    $query->innerJoin('taxonomy_term_data', 't', 'b.current_place = t.tid');
  }
  else {
    $query = db_select('bookcrossing_books', 'b');
    $query->innerJoin('taxonomy_term_hierarchy', 'h', 'b.current_place = h.tid');
    $query->innerJoin('taxonomy_term_data', 't', 'h.parent = t.tid');
  }

  $args = array(':letter' => $letter);
  $condition = ' = :letter';
  if ($letter == 'num') {
    $condition = " IN('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')";
    $args = array();
  }

  $query->fields('b', array('bid'));
  $query->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

  if (!empty($tids)) {
    $query->condition('t.tid', $tids, 'IN');
  }

  $query->where('LEFT(t.name, ' . drupal_strlen($letter) . ') ' . $condition, $args);
  $query = $query->extend('PagerDefault');
  $query->limit($limit);
  $query->orderBy('t.name', 'ASC');
  $bids = $query->execute()->fetchCol();

  $books = array();
  foreach ($bids as $bid) {
    $books[] = bookcrossing_load($bid);
  }

  return $books;
}

/**
 * Helper function: returns catalog view types
 */
function bookcrossing_catalog_get_view_types() {
  $types = array(
    'tile' => array(
      'title' => t('tile'),
      'path' => '',
    ),
    'covers' => array(
      'title' => t('covers'),
      'path' => 'covers',
    ),
  );

  return $types;
}

/**
 * Sorts books by title.
 */
function bookcrossing_catalog_sort_by_title($mode, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('node', 'n', 'b.nid = n.nid');
  $query->fields('b', array('bid'));
  $query->distinct();
  $query->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

  $query = $query->extend('PagerDefault');
  if ($limit !== FALSE) {
    $query = $query->limit($limit);
  }

  $query->orderBy("if(n.title regexp '^[а-яА-Я]', concat('1', n.title), n.title)");
  $bids = $query->execute()->fetchCol();

//  $books = bookcrossing_load_multiple($bids);
  $books = array();
  foreach ($bids as $bid) {
    $books[] = bookcrossing_load($bid);
  }

  return $books;
}

/**
 * Sorts by books author.
 */
function bookcrossing_catalog_sort_by_author($mode, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');

  if ($mode == 'covers') {
    $query->distinct();
  }

  $query->fields('b', array('bid'));
  $query->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
  $query->addExpression('LOWER(LEFT(t.name, 1))', 'letter');
  
  $query = $query->extend('PagerDefault');
  if ($limit !== FALSE) {
    $query = $query->limit($limit);
  }

//if ($mode == 'tile') {
//  $query->groupBy('concat(b.bid, t.name)');
//}
 
  $query->orderBy("if(t.name regexp '^[а-яА-Я]', concat('1', t.name), t.name)");
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
 * Sorts by books genre.
 */
function bookcrossing_catalog_sort_by_genre($mode, $limit = 30) {
  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('field_data_field_bookcrossing_genre', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_genre_tid = t.tid');
  
  if ($mode == 'covers') {
    $query->distinct();
  }

  $query->fields('b', array('bid'));
  $query->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
  $query->addExpression('LOWER(LEFT(t.name, 1))', 'letter');

  $query = $query->extend('PagerDefault');
  if ($limit !== FALSE) {
    $query = $query->limit($limit);
  }

//if ($mode == 'tile') {
  //$query->groupBy('concat(b.bid, t.name)');
//}

  $query->orderBy("if(t.name regexp '^[а-яА-Я]', concat('1', t.name), t.name)");
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
 * Sorts by books place.
 * @todo
 */
function bookcrossing_catalog_sort_by_place($mode, $limit = 30) {
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

  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('taxonomy_term_hierarchy', 'h', 'b.current_place = h.tid');
  $query->innerJoin('taxonomy_term_data', 't', 'h.parent = t.tid');
  $query->fields('b', array('bid'));
  $query->distinct();
  $query->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');

  if (!empty($tids)) {
    $query->condition('t.tid', $tids, 'IN');
  }

  $query = $query->extend('PagerDefault');
  if ($limit !== FALSE) {
    $query = $query->limit($limit);
  }

  $query->orderBy("if(t.name regexp '^[а-яА-Я]', concat('1', t.name), t.name)");
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
 */
function bookcrossing_title_first_letter($book) {
  return drupal_ucfirst(drupal_substr($book['node']->title, 0, 1));
}

/**
 * Helper function: returns author name.
 */
function bookcrossing_get_author($book) {
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
//      $name = bookcrossing_get_genre($book);
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
  }
  else {
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
  }
  else {
    $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
  }
  
  return $name;
}

/**
 * Helper function: returns books genre.
 */
function bookcrossing_get_genre($book) {
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
  }
  else {
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
//      //$name = bookcrossing_get_genre($book);
//    }
//  }
//  else {
//    $name = $term->name;
//  }

  if (isset($_GET['sort'])) {
    $name = $term->name;
  }
  else {
    $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
  }
  
  return $name;
}

/**
 * Helper function: returns books city.
 */
function bookcrossing_get_city($book, $trim = FALSE) {
  if (isset($_GET['place'])) {
    $term =  $book['status'] ?$book['place'] : taxonomy_term_load($book['place_id_left']);
  }
  else {
    $parents = $book['status'] ? taxonomy_get_parents($book['place']->tid) : taxonomy_get_parents($book['place_id_left']);
    $term = reset($parents);
  }

  if (isset($_GET['sort']) || isset($_GET['place']) || $trim) {
    $name = $term->name;
  }
  elseif (!$trim) {
    $name = drupal_ucfirst(drupal_substr($term->name, 0, 1));
  }
  
  return $name;
}

/**
 * Output for catalog sorting with a tile view mode.
 */
function bookcrossing_catalog_sort_tile($sort = 'title', $books = array()) {
  $sort_types = bookcrossing_catalog_get_sort_types();
  $function = $sort_types[$sort]['grouping_callback'];
  if (!function_exists($function)) {
    return '';
  }

  $letters = array();
  foreach ($books as $book) {
    $letters[$function($book)][] = $book;
  }

  uksort($letters, 'bookcrossingRusFirst');

  $output = '';
  $l = '';
  foreach ($letters as $group_name => $books) {
    $group_descr = '';
    if ($sort != 'title' && $l != $group_name && drupal_strlen($group_name) == 1) {
      if ($sort == 'place') {
        $names = db_select('taxonomy_term_data', 't')
          ->fields('t', array('name'))
          ->where('LOWER(LEFT(t.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
          ->condition('t.vid', BOOKCROSSING_PLACES_VID, '=')
          ->orderBy('t.name')
          ->execute()
          ->fetchCol();

        $start = $names[0];
        $end = $names[count($names) - 1];
      }
      elseif ($sort == 'genre') {
        $names = db_select('taxonomy_term_data', 't')
          ->fields('t', array('name'))
          ->where('LOWER(LEFT(t.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
          ->condition('t.vid', 1, '=')
          ->orderBy('t.name')
          ->execute()
          ->fetchCol();

        $start = $names[0];
        $end = $names[count($names) - 1];
      }
      elseif ($sort == 'author') {
        $names = db_select('taxonomy_term_data', 't')
          ->fields('t', array('name'))
          ->where('LOWER(LEFT(t.name, 1)) = :letter', array(':letter' => drupal_strtolower($group_name)))
          ->condition('t.vid', 2, '=')
          ->orderBy('t.name')
          ->execute()
          ->fetchCol();

        $start = $names[0];
        $end = $names[count($names) - 1];
      }
      
      if ($start != $end) {
        $group_descr = ' <span>(' . $start . ' - ' . $end . ')</span>';
      }
      else {
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

      $status_string = $book['status'] ? t('Found') : t('Free book');
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

function bookcrossingRusFirst($a, $b) {
  $rus = drupal_map_assoc(array('а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'Ъ', 'ы', 'ь', 'э', 'ю', 'я'));
  $eng = drupal_map_assoc(array('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'));
  $num = range(0, 9);

  $a = drupal_strtolower($a);
  $b = drupal_strtolower($b); 

  if (isset($rus[$a]) && isset($rus[$b])) {
    return strcmp($rus[$a], $rus[$b]);
  }
  
  if (isset($rus[$a]) && isset($eng[$b])) {
    return -1;
  }

  if (isset($rus[$b]) && isset($eng[$a])) {
    return 1;
  }

  if (isset($eng[$a]) && isset($eng[$b])) {
    return strcmp($eng[$a], $eng[$b]);
  }

  if (isset($num[$a]) && isset($num[$b])) {
    if ($a < $b) {
      return -1;
    }
    else {
      return 1;
    }
  }

  if (isset($num[$a]) && !isset($num[$b])) {
    return 1;
  }

  if (!isset($num[$a]) && isset($num[$b])) {
    return -1;
  }
}

/**
 * Output for catalog sorting with a covers view mode.
 */
function bookcrossing_catalog_sort_covers($sort = 'title', $books = array()) {
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
 * Finds books
 */
function bookcrossing_catalog_find_books($type = 'title', $mode = 'tile', $limit = 30) {
  if (!isset($_GET['find']) || empty($_GET['find'])) {
    return array();
  }

  $query = db_select('bookcrossing_books', 'b');
  $query->innerJoin('node', 'n', 'b.nid = n.nid');
  $query->innerJoin('field_data_field_bookcrossing_author', 'a', 'b.nid = a.entity_id');
  $query->innerJoin('taxonomy_term_data', 't', 'a.field_bookcrossing_author_tid = t.tid');
  $query->leftJoin('field_data_field_bookcrossing_isbn', 'i', 'b.nid = i.entity_id');
  
  if ($mode == 'covers') {
    $query->distinct();
  }

  $left = '';
  if ($type == 'title') {
    $query->orderBy("if(n.title regexp '^[а-яА-Я]', concat('a', n.title), n.title)");
    
    if (isset($_GET['sort'])) {
      $left = ' AND LEFT(n.title, ' . drupal_strlen($_GET['sort']) . ') = :sort';
    }
  }
  elseif ($type == 'author') {
    $query->orderBy("if(LEFT(t.name, 1) regexp '^[а-яА-Я]', concat('a', LEFT(t.name, 1)), LEFT(t.name, 1))");
    
    if (isset($_GET['sort'])) {
      $left = ' AND LEFT(t.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
    }
    else {
      $query->addExpression('LEFT(t.name, 1)', 'letter');
    }
  }
  elseif ($type == 'genre') {
    $query->innerJoin('field_data_field_bookcrossing_genre', 'g', 'b.nid = g.entity_id');
    $query->innerJoin('taxonomy_term_data', 'p', 'g.field_bookcrossing_genre_tid = p.tid');
    $query->orderBy("if(p.name regexp '^[а-яА-Я]', concat('a', p.name), p.name)");
    
    if (isset($_GET['sort'])) {
      $left = ' AND LEFT(p.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
    }
    else {
      $query->addExpression('LEFT(p.name, 1)', 'letter');
    }
  }
  elseif ($type == 'place') {
    $query->innerJoin('taxonomy_term_hierarchy', 'h', 'b.current_place = h.tid');
    $query->innerJoin('taxonomy_term_data', 'p', 'h.parent = p.tid');
    $query->orderBy('p.name', 'ASC');
    
    if (isset($_GET['sort'])) {
      $left = ' AND LEFT(p.name, ' . drupal_strlen($_GET['sort']) . ') = :sort';
    }
  }
  
  $query->distinct();
  
  $query = $query->extend('PagerDefault');
  $query->limit($limit);
  
  $info = $query
      ->fields('b', array('bid'))
      ->condition('b.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>')
      ->where('(LOWER(n.title) REGEXP LOWER(:letter) OR LOWER(i.field_bookcrossing_isbn_value) REGEXP LOWER(:letter) OR LOWER(t.name) REGEXP LOWER(:letter))' . $left, array(':letter' => $_GET['find'], ':sort' => isset($_GET['sort']) ? $_GET['sort'] : ''))
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
