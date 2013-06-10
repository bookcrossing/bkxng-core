<?php
$output = '<div class="body_container clearfix">
<!--Main content Area Start-->
   
    <!--content Area-->      
      <div class="content_area_inside">
      <!--Header Configration--> 
            
         <div class="header_confi">';
$output .= '<div class="content_holder">';
$output .= render ( $form ['header_type'] );
$output .= '</div>
                    
                <!--Holder Two-->';
$output .= '<div id="mobilizerTitle" class="content_holder"><div class="header-title-block">';
$output .= '<div class="half_box">';
$output .= render ( $form ['title'] );
$output .= '</div>';
$output .= '</div></div>';

$output .= '<!--Holder Two Close-->
                               <!--Holder Two-->';
$output .= '<div id="logo" class="content_holder"><div class="header-image-block">';

if (isset ( $form ['default_logo'] )) {
  $output .= '<div class="headerimgtitle">' . render ( $form ['header_image_title'] ) . '</div>';
  $output .= '<div id="default_logo" class="half_box default-image">';
  $output .= render ( $form ['default_logo'] );
  $output .= '</div>';
  $output .= '<div class="half_box_right">';
  $output .= render ( $form ['header_image'] );
  $output .= '</div>';
} else {
  $output .= '<div class="headerimgtitle">' . render ( $form ['header_image_title'] ) . '</div>';
  $output .= '<div class="half_box">';
  $output .= render ( $form ['header_image'] );
  $output .= '</div>';
}

$output .= '</div></div>';

$output .= '<div id="resize" class="content_holder"> <div class="header-resize-block">';
$output .= '<div class="half_box">';
$output .= '<div class="resizetitle">' . render ( $form ['resize_title'] ) . '</div>';
$output .= render ( $form ['resize'] );
$output .= render ( $form ['slider'] );
$output .= '</div>';
$output .= '</div></div>';

$output .= '<div id="align" class="content_holder"><div class="header-align-block">';
$output .= '<div class="half_box">';
$output .= '<div class="aligntitle">' . render ( $form ['align_title'] ) . '</div>';
$output .= render ( $form ['align'] );
$output .= '</div>';
$output .= '</div></div>';

$output .= '<div class="content_holder"><div class="header-background-block">';
$output .= '<div class="half_box"><div data-provides="fileupload" class="fileupload fileupload-new">';

$output .= render ( $form ['hbimage'] );
$output .= '</div></div>  ';
$output .= '<div class="half_box_right">';
$output .= render ( $form ['hbcolor'] );
$output .= '</div>';
$output .= '<div class="clear"></div>';
$output .= '</div></div>';

$output .= '<div class="bottom_buttons">';
$output .= render ( $form ['preview'] );
$output .= render ( $form ['proceed'] );
$output .= '</div>';
$output .= render ( $form ['form_build_id'] );
$output .= render ( $form ['form_token'] );
$output .= render ( $form ['form_id'] );

$output .= '</div> 
                        </div>';

$output .= '</div>
        <!--Header Configration close--> 
      </div>
    <!--content Area Close--> 

</div>
<!--Main content Area close-->
';
print $output;
