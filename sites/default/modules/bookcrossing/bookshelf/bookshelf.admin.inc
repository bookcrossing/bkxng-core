<?php

include_once DRUPAL_ROOT . "/modules/taxonomy/taxonomy.admin.inc";

function bookshelf_safezone_manage($term)
{
    $vocabulary = taxonomy_vocabulary_load(BOOKCROSSING_SAFEZONE_VID, FALSE);
    if (!is_null($term)) {
        $term = taxonomy_term_load($term->tid);
        //$term['parent'] = (array)$parent;
    }
    //module_load_include('module', 'bookcrossing', 'bookcrossing');
    //module_load_include('inc', 'taxonomy', 'taxonomy.admin');
    drupal_add_library('system', 'drupal.ajax');
    drupal_add_library('system', 'jquery.form');

    //For redirect after submit form
    $vocabulary->page = BOOKCROSSING_SAFEZONE_NAME;
    $form = drupal_get_form('taxonomy_form_term', $term, $vocabulary);

    return drupal_render($form);
}
