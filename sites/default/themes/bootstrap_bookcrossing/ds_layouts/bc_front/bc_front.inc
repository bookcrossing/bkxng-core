<?php
function ds_bc_front()
{
    return array(
        'label' => t('BookCrossing: Front page layout'),
        'regions' => array(
            // The key of this region name is also the variable used in
            // the template to print the content of that region.
            'bc_content' => t('Content'),
        ),
        // Add this if there is a default css file.
        'css' => TRUE,
    );
}