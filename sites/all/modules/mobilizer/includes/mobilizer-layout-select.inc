<?php
/**
 * @file 
 * Containg function for layout and finish page.
 */
/**
 * Function to render layout item listing. 
 * @return themed html of list of layouts.
 */
function mobilizer_select_theme_layout_page() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_select_theme_layout_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html( $base_url ));
}
/**
 * Function create layout form.
 * @return form
 */
function mobilizer_select_theme_layout_form($form, &$form_state) {
  global $base_url;
  $default_value = variable_get('mobilizer_nav_style');
  $image_base_path = $base_url . '/' . drupal_get_path('module', 'mobilizer');
  $options = array(
      'menu-horizontal' => '<h3>List</h3>' . '<img src = ' . $image_base_path . '/images/layout_1.png />',
      'menu-box' => '<h3>Matrix</h3>' . '<img src =' . $image_base_path . '/images/layout_2.png />',
      'menu-dropdown' => '<h3>Expandable</h3>' . '<img src =' . $image_base_path . '/images/layout_3.png />' 
  );
  $form['layout'] = array(
      '#prefix' => '<div class = "layout_confi">',
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => isset($default_value) ? $default_value : 'list',
      '#ajax' => array(
          'event' => 'click',
          'callback' => 'mobilizer_ajax_callback',
          'wrapper' => 'iframe_div',
          'method' => 'replace',
          'effect' => 'fade' 
      ) 
  );
  $form['submit'] = array(
      '#suffix' => '</div>',
      '#type' => 'submit',
      '#value' => t('Proceed'),
  );
  return $form;
}
/**
 * Layout form submit.
 */
function mobilizer_select_theme_layout_form_submit($form, &$form_state) {
  $layout = $form_state ['values'] ['layout'];
  $form ['layout'] ['#value'] = $layout;
  variable_set( 'mobilizer_nav_style', $layout);
  variable_set('mobilizer_layout_process', 1);
  drupal_goto('admin/mobilizer/select-menu');
}
/**
 * Function to render finish page. 
 * @return themed html of list of layouts.
 */
function mobilizer_finish_page() {
  global $base_url;
  $render_array = drupal_get_form("mobilizer_finish_page_form");
  $output = drupal_render($render_array);
  return _mobilizer_themer($output, _mobilizer_iframe_with_html($base_url));
}
/**
 * Function to create finish form.
 * @return form.
 */
function mobilizer_finish_page_form($form, &$form_state) {
  global $base_url;
  $image_base_path = $base_url . '/' . drupal_get_path('module', 'mobilizer');
  $mobilizer_site = variable_get('mobilizer_site_status');
  $site_status = variable_get('mobilizer_publish_process');
  
  $mobilizer_preview_html = '<div class="preview_url_main_cs"><div class="preview_content_cs"> Mobile site preview link: </div><div class="preview_url_cs">' . $base_url . "/?mobilizer=" . md5( $base_url . "_mobilizer" ) . '</div><div id="share-trigger-container" class="share-trigger-container">' . l(t('Share'), '#', array(
      'attributes' => array(
          'class' => 'share-trigger',
          'onclick' => 'return false;' 
      ) 
  ) ) . '</div></div>';
  
  $finish_message = '';
  if (! isset ( $mobilizer_site )) {
    $finish_message = '<h1 class = "title">Congratulation !</h1><br/>
      <p class = "congratulations">Your mobile site configuration has been successfully saved.
      Please open your mobile site preview url to check your mobile site.</p>' . $mobilizer_preview_html;
  }
  
  if ($mobilizer_site == '1') {
    $finish_message = '<h1 class = "title">Congratulation !</h1><br/>
      <p class = "congratulations">Your mobile site configuration has been published.
      Please open your mobile site domain url.</p>' . $mobilizer_preview_html;
  }
  if ($mobilizer_site == '0') {
    $finish_message = '<br/>
      <p class = "congratulations">Your mobile site configuration has been saved as draft.
      Please click on Go Live to publish it.</p>' . $mobilizer_preview_html;
  }
  $form['final_message'] = array(
      '#markup' => $finish_message 
  );
  if ($mobilizer_site == '1' && isset ( $site_status )) {
    
    $form['unpublish'] = array(
        '#type' => 'submit',
        '#value' => t('Unpublish'),
        '#attributes' => array(
            'class' => array(
                'btn btn-primary' 
            ) 
        ),
        '#submit' => array(
            'mobilizer_unpublish_form_submit' 
        ) 
    );
  }
  else {
    $form['submit_live'] = array(
        '#type' => 'submit',
        '#value' => t('Go Live'),
        '#attributes' => array(
            'class' => array(
                'btn btn-primary' 
            ) 
        ),
        '#submit' => array(
            'mobilizer_live_page_form_submit' 
        ) 
    );
    
    $form['submit_save_as_draft'] = array(
        '#type' => 'submit',
        '#value' => t('Save As Draft'),
        '#attributes' => array(
            'class' => array(
                'btn btn-primary' 
            ) 
        ),
        '#submit' => array(
            'mobilizer_save_as_draft_form_submit' 
        ) 
    );
  }
  $form['share_link_container'] = array(
      '#type' => 'fieldset',
      '#title' => t('Share Mobile Site Url'),
      '#prefix' => '<div id="share-box-container" style="display:none;" class="share_container_overlay">',
      '#suffix' => '</div>' 
  );
  $form['share_link_container']['close'] = array(
      '#markup' => '<div class="share-content-close"><span id="share-close">Close</span></div>' 
  );
  $form['share_link_container']['wrapper'] = array(
      '#prefix' => '<div id="message-wrapper">',
      '#suffix' => '</div>' 
  );
  $form['share_link_container']['to'] = array(
      '#type' => 'textarea',
      '#title' => t( 'To :' ),
      '#value' => '',
      '#rows' => '2',
      '#description' => 'Multiple email ids can be used with \',\' seperated.' 
  );
  $form['share_link_container']['link'] = array(
      '#type' => 'hidden',
      '#value' => $base_url . "/?mobilizer=" . md5($base_url . "_mobilizer") 
  );
  $form['share_link_container']['send'] = array(
      '#type' => 'submit',
      '#value' => t( 'Send' ),
      '#attributes' => array(
          'class' => array(
              'btn btn-primary' 
          ),
          'onclick' => 'javascript:jQuery("#message-wrapper").html("");' 
      ),
      '#ajax' => array(
          'event' => 'click',
          'callback' => 'mobilizer_share_link',
          'wrapper' => 'message-wrapper',
          'method' => 'replace' 
      ) 
  );
  return $form;
}
/**
 * Form submit for Unpublish mobile site.
 */
function mobilizer_unpublish_form_submit($form, &$form_state) {
  variable_set('mobilizer_site_status', 0 );
  variable_set('mobilizer_publish_process', 1);
}
/**
 * Form submit for Go live mobile site.
 */
function mobilizer_live_page_form_submit($form, &$form_state) {
  variable_set('mobilizer_site_status', 1);
  variable_set('mobilizer_publish_process', 1);
}
/**
 * Form submit for Save Draft mobile site settings.
 */
function mobilizer_save_as_draft_form_submit($form, &$form_state) {
  variable_set('mobilizer_site_status', 0);
  variable_set('mobilizer_publish_process', 1);
}
/**
 * Form submit for share mobile site link.
 */
function mobilizer_share_link($form, &$form_state) {
  $flag = False;
  $from = NULL;
  
  $to = $form_state ['input'] ['to'];
  $recipients = explode(',', $to );
  $link = $form_state ['input'] ['link'];
  
  foreach ( $recipients as $key => $value ) {
    if (!valid_email_address(trim($value ))) {
      return array(
          '#markup' => '<div id="message-wrapper"><div class="messages error">' . 'Please provide valid email id(s).' . '</div></div>' 
      );
      $flag = True;
    }
    else {
      $flag = False;
    }
  }
  if ($flag == False) {
// These value can remain empty.
    $my_module = 'mobilizer';
    $my_mail_token = 'share_mobile_url';
    
    $from = variable_get('system_mail', $to);
    $message = array(
        'id' => $my_module . '_' . $my_mail_token,
        'to' => $to,
        'subject' => 'Mobile Site Link',
        'body' => array(
            $link 
        ),
        'headers' => array(
            'From' => $from,
            'Sender' => $from,
            'Return-Path' => $from,
            'MIME-Version' => '1.0',
            'Content-Type' => 'text/html; charset=UTF-8; format=flowed',
            'Content-Transfer-Encoding' => '8Bit',
            'X-Mailer' => 'Drupal' 
        ) 
    );
    $system = drupal_mail_system($my_module, $my_mail_token);
// The format function must be called before calling the mail function.
    $message = $system->format ( $message );
    if ($system->mail ( $message )) {
      return array(
          '#markup' => '<div id="message-wrapper"><div class="mob_success">' . 'Link has been shared with the reciepients.' . '</div></div>' 
      );
    }
    else {
      return array(
          '#markup' => '<div id="message-wrapper"><div class="messages error">' . 'There is some problem sending mail, Please try after some time.' . '</div></div>' 
      );
    }
  }
}
