<?php

 function ds_bc_bookdetails() {

    return array(

      'label' => t('BookCrossing: Book Details layout'),

      'regions' => array(

        // The key of this region name is also the variable used in

        // the template to print the content of that region.

        'bc_bookcover' => t('Book Cover'),

        'bc_bookdetails' => t('Book Details'),

      ),

      // Add this if there is a default css file.

      'css' => TRUE,

    );

  }