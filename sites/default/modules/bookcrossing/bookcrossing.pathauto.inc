<?php

/**

 * @file

 * Pathauto

 */

function bookcrossing_path_alias_types() {
  return array('book/' => t('Book'));
}


/**
 * Implements hook_pathauto().
 */
function bookcrossing_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'bookcrossing';
      $settings['token_type'] = 'book';
      $settings['groupheader'] = t('Books paths');
      $settings['patterndescr'] = t('Default path pattern (applies to all content types with blank patterns below)');
      $settings['patterndefault'] = 'book/[book:bid]-[book:title]';
      $settings['batch_update_callback'] = 'bookcrossing_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'bookcrossing') . '/bookcrossing.pathauto.inc';

      return (object) $settings;

    default:
      break;
  }
}


/**
 * Batch processing callback; Generate aliases for nodes.
 */
function bookcrossing_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('bookcrossing_books', 'books');
  $query->leftJoin('url_alias', 'url', "CONCAT('book/', book.bid) = url.source");
  $query->addField('book', 'bid');
  $query->isNull('url.source');
  $query->condition('book.bid', $context['sandbox']['current'], '>');
  $query->condition('book.status', BOOKCROSSING_BOOK_STATUS_RESERVED, '<>');
  $query->orderBy('book.bid');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $bids = $query->execute()->fetchCol();

  bookcrossing_books_update_alias_multiple($bids, 'bulkupdate');
  $context['sandbox']['count'] += count($bids);
  $context['sandbox']['current'] = max($bids);
  $context['message'] = t('Updated alias for book @nid.', array('@nid' => end($bids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}


function bookcrossing_books_update_alias_multiple(array $bids, $op, array $options = array())
{
  $options += array('message' => FALSE);

  $books = bookcrossing_load_multiple($bids);

  foreach ($books as $book) {
    bookcrossing_update_alias($book, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($bids), 'Updated URL alias for 1 book.', 'Updated URL aliases for @count books.'));
  }
}


function bookcrossing_update_alias($book, $op, array $options = array()) {
  module_load_include('inc', 'pathauto', 'pathauto');
  pathauto_create_alias('bookcrossing', $op, 'book/' . $book['bid'], array('book' => $book));
}
