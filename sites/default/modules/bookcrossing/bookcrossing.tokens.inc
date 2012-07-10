<?php

function bookcrossing_token_info() {
  $type = array(
    'name' => t('Books'),
    'description' => t('Tokens related to individual content items, or "books".'),
    'needs-data' => 'book',
  );  

  $book['bid'] = array(
    'name' => t("Book ID"),
    'description' => t('The unique ID(not BCID) of the content item, or "book".'),
  );

  $book['title'] = array(
    'name' => t("Title"),
    'description' => t("The title of the book."),
  );

  return array(
    'types' => array('book' => $type),
    'tokens' => array('book' => $book),
  );
}

/**
 * Implements hook_tokens().
 */
function bookcrossing_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'book') {
    $book = $data['book'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the node.
        case 'bid':
          $replacements[$original] = $book['bid'];
          break;

        case 'title':
          $replacements[$original] = check_plain($book['node']->title);
          break;
      }
    }
  }

  return $replacements;
}