<?php
 function ds_bc_2col() {
    return array(
      'label' => t('BookCrossing: Catlog layout'),
      'regions' => array(
        // The key of this region name is also the variable used in
        // the template to print the content of that region.
        'bc_left' => t('Left'),
        'bc_right' => t('Right'),
      ),
      // Add this if there is a default css file.
      'css' => TRUE,
    );
  }