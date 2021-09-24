<?php

/**
 * Implements hook_menu().
 */
function bookcrossing_menu()
{
    $paths = array();

    /**
     * Admin menu
     *
     */
    $paths['admin/config/bookcrossing'] = array(
        'title' => 'BookCrossing',
        'description' => 'My Description',
        'position' => 'right',
        'weight' => -100,
        'page callback' => 'system_admin_menu_block_page',
        'access arguments' => array('access administration pages'),
        'file' => 'system.admin.inc',
        'file path' => drupal_get_path('module', 'system'),
    );

    $paths['admin/config/bookcrossing/settings'] = array(
        'title' => 'BookCrossing General Settings',
        'description' => 'BookCrossing General Settings',
        'weight' => -100,
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bookcrossing_page_settings'),
        'access arguments' => array('access administration pages'),
        'file' => 'bookcrossing.admin.inc',
    );

    $paths['admin/config/bookcrossing/books'] = array(
        'title' => t('Book Items'),
        'description' => 'Manage extended bookcrossing data (BCID, status, place) assosiated with books.',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bookcrossing_admin_show_books'),
        'access arguments' => array('administer books'),
        'file' => 'bookcrossing.admin.inc',
    );

    $paths['admin/config/bookcrossing/books/%bookcrossing/edit'] = array(
        'title' => 'Change book info',
        'description' => 'Edit books',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bookcrossing_edit_book', 3),
        'access arguments' => array('administer books'),
        'file' => 'bookcrossing.admin.inc',
        //'type' => MENU_LOCAL_TASK,
        //'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
    );

    $paths['admin/config/bookcrossing/reviews'] = array(
        'title' => t('Reviews in Books History'),
        'description' => 'Reviews left by users who read the book.',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bookcrossing_admin_show_comments'),
        'access arguments' => array('administer books'),
        'file' => 'bookcrossing.admin.inc',
    );

    $paths['admin/config/bookcrossing/merge-books'] = array(
        'title' => t('Merge Book Items'),
        'description' => 'Merge books descriptions.',
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bookcrossing_merge_books'),
        'access arguments' => array('administer books'),
        'file' => 'bookcrossing.admin.inc',
    );

    $paths['users'] = array(
        'title' => 'Users list',
        'page callback' => 'bookcrossing_users_list',
        'access arguments' => array('view users list'),
        'file' => 'bookcrossing.userslist.inc',
    );

    $paths['ajax/bookcrossing-places-autocompletion/%'] = array(
        'title' => 'Places autocompletion',
        'page callback' => 'bookcrossing_places_autocomplete_results',
        'page arguments' => array(2),
        'access callback' => TRUE,
        'type' => MENU_CALLBACK,
    );

    /*
     * Page with form, where user can select to create new book or release one of the books he is reading.
     */
    $paths['release-book'] = array(
        'title' => t('Release book'),
        'page callback' => 'bookcrossing_release_book_page',
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Reserve new bcid.
     *
     */
    $paths['reserve-bcid'] = array(
        'title' => t('Reserve BCID'),
        'page callback' => 'bookcrossing_reserve_bcid',
        'access arguments' => array('register found book'),
        'file' => 'bookcrossing.pages.inc',
    );

    /*
     * Delete reserved bcid.
     */
    $paths['delete-reserved/%'] = array(
        'title' => t('Delete reserved BCID'),
        'page callback' => 'bookcrossing_delete_reserved_bcid',
        'page arguments' => array(1),
        'access arguments' => array('register found book'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Books catalog
     *
     */
    $paths['books/%/%'] = array(
        'title' => t('Catalog of books'),
        'page callback' => 'bookcrossing_catalog_page',
        'page arguments' => array(1, 2),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.catalog.inc',
    );

    $paths['books'] = array(
        'title' => t('Catalog of books'),
        'page callback' => 'bookcrossing_catalog_page',
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.catalog.inc',
    );

    $paths['books/covers'] = array(
       'title' => t('Catalog of books'),
        'page callback' => 'bookcrossing_catalog_page',
        'page arguments' => array('title', 'covers'),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.catalog.inc',
    );

    $paths['books/%'] = array(
        'title' => t('Catalog of books'),
        'page callback' => 'bookcrossing_catalog_page',
        'page arguments' => array(1),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.catalog.inc',
    );

    $paths['add-to-bookmarks/%bookcrossing'] = array(
        'title' => t('Add to bookmarks'),
        'page callback' => 'bookcrossing_to_favourites',
        'page arguments' => array(1),
        'access arguments' => array('add to bookmarks'),
    );

    /**
     * Page with 3 random books with large covers.
     *
     */
    $paths['books-front'] = array(
        'title' => t('Front page'),
        'page callback' => 'bookcrossing_front_page',
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * User released book.
     *
     */
    $paths['release-book/%bookcrossing'] = array(
        'title' => 'Release book',
        'page callback' => 'bookcrossing_release_book',
        'page arguments' => array(1),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * User can change their comment to book.
     *
     */
    $paths['change-comment/%'] = array(
        'title' => '',
        'page callback' => 'bookcrossing_change_comment',
        'page arguments' => array(1),
        'access arguments' => array('change review'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * History of the book.
     *
     */
    $paths['view-history/%bookcrossing_history'] = array(
        'title' => t('Book history'),
//    'title callback' => 'bookcrossing_history_title',
        'page callback' => 'bookcrossing_view_history',
        'page arguments' => array(1),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Form for adding a new book.
     *
     */
    $paths['release-book/add-new'] = array(
        'title' => t('Add new book'),
        'page callback' => 'bookcrossing_add_new_book_form', //'node_add',
        //'page arguments' => array('bookcrossing'),
        'access callback' => 'node_access',
        'access arguments' => array('create', 'bookcrossing'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Page with full information about the book.
     *
     */
    $paths['book/%bookcrossing'] = array(
        'title' => t('Book'),
//    'title callback' => 'bookcrossing_title',
        'page callback' => 'bookcrossing_view_book',
        'page arguments' => array(1),
        'access arguments' => array('access content'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * User can enter a bcid and become a reader of the book
     *
     */
    $paths['found-book/%'] = array(
        'title' => t('Found the book.'),
        'page callback' => 'bookcrossing_found_book_page',
        'page arguments' => array(1),
        'access arguments' => array('register found book'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Form for adding reserved book.
     *
     */
    $paths['add-reserved-book/%'] = array(
        'title' => t('Add reserved book'),
        'page callback' => 'bookcrossing_add_reserved_book_form',
        'page arguments' => array(1),
        'access callback' => 'node_access',
        'access arguments' => array('create', 'bookcrossing'),
        'file' => 'bookcrossing.pages.inc',
    );

    /**
     * Shows bcid and information about where it should be written on the book.
     *
     */
    $paths['book-added/%'] = array(
        'title' => t('The book has been added'),
        'page callback' => 'bookcrossing_book_added',
        'page arguments' => array(1),
        'access callback' => 'node_access',
        'access arguments' => array('create', 'bookcrossing'),
        'file' => 'bookcrossing.pages.inc',
    );

    /*$paths['book/%bookcrossing/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );*/

    return $paths;
}


/**
 * Implements hook_menu_alter()
 */
function bookcrossing_menu_alter(&$paths)
{
    $paths['user/%user']['page callback'] = 'bookcrossing_users_shelf';
    $paths['user/login']['access callback'] = 'bookcrossing_permission_forms_callback';
    $paths['user/register']['access callback'] = 'bookcrossing_permission_forms_callback';
    $paths['user/password']['access callback'] = 'bookcrossing_permission_forms_callback';
    $paths['user']['access callback'] = 'bookcrossing_permission_forms_callback';
    $paths['user']['access arguments'] = array(0);
}
