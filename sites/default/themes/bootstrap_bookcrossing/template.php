<?php

/**
 * @file template.php
 */

/**
 * Implements hook_theme().
 */
function bookcrossing_theme() {
  return array(
    'bookcrossing_main_menu' => array(
      'variables' => array('links' => NULL, 'visible' => NULL),
    ),
    'book_no_image' => array(
      'variable' => array('book' => NULL),
    ),
  );
}

/**
 * Theme main menu
 */
function theme_bookcrossing_main_menu($variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  $num_links_visible = theme_get_setting('visible_menu_items');
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    $output = '';


    // Treat the heading first if it is present to prepend it to the list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }

	$output .= '<ul class="nav">';

    $num_links = count($links);
    $i = 1;

    foreach ($links as $key => $link) {
      $class = array($key);

      // Add first, last and active classes to the list of links to help out themers.
      if ($i == 1) { $class[] = 'first'; }
      if ($i == $num_links) { $class[] = 'last'; }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
          && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      if ($link['href'] == 'users') { $i--; }

      if ($i == $variables['visible']) {
        $output .= '
	        <li class="dropdown">
	        	<a href="#" class="dropdown-toggle" data-toggle="dropdown">' . t('more') . '<b class="caret"></b></a>
	            <ul class="dropdown-menu">
        ';
      }

      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';

      if (isset($link['href'])) {
        if ($link['href'] == 'books' && arg(0) == 'books') {
          $link['attributes']['class'][] = 'active';
        }

        // Pass in $link as $options, they share the same keys.
        $output .= l($link['title'], $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        // Some links are actually not links, but we wrap these in <span> for adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }

      $i++;
      $output .= "</li>";
    }

    $output .= '</ul></li></ul>';
  }

  return $output;
}

/**
 * Theme book when there is image uploaded.
 */
function theme_book_no_image($variables) {
  
}

