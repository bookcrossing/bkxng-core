jQuery(document).ready(function () { 
        function header_text_type(){
            jQuery("#logo").hide();
            jQuery("#resize").hide();
            jQuery("#align").hide();
            jQuery("#mobilizerTitle").show();
        }  
        function header_image_type(){
            jQuery("#logo").show();
            jQuery("#resize").show();
            jQuery("#align").show();
            jQuery("#mobilizerTitle").hide();
        }
        jQuery("#edit-header-type").change(function() {

        var chk = jQuery(this).val();
        
        if(chk == "header_text_type"){
             header_text_type(); 
        }
        if(chk == "header_image_type"){
             header_image_type();
        }
        
        });
        
        default_type = jQuery("#edit-header-type").val();
         
        if(default_type == "header_text_type"){
             header_text_type(); 
        }
        if(default_type == "header_image_type"){
             header_image_type();
        }
        
     jQuery('#edit-header-image-upload-button').click(function(){
         jQuery('#deflogo').hide();
     });
        
});

/*
 *hide default mobilizer logo when new mobilizer logo chnage 
 *
 */
Drupal.behaviors.fileUpload = {
    attach: function(context, settings) {
        jQuery("body").ajaxComplete(function(event,request, settings){
                if (form_build_id = settings.url.match(/file\/ajax\/header_image\d*\/(.*)$/)) {                    
                    jQuery('#default_logo').hide();
                }
        });

    }
}
