jQuery(document).ready(function (){
	var mobilizer_flag_current_palette = "";
	var mobilizer_flag_change_confirm_palette = "";
	var mobilizer_flag_previous_palette = "";

	for(var customize_count = 1; customize_count <= 5; customize_count++){
		jQuery('.customize'+customize_count).hide();
	}
	
	for(var customize_count_show = 1; customize_count_show <= 5; customize_count_show++){
		jQuery('.color_palette'+customize_count_show).click(function(){		
		for(var customize_count = 1; customize_count <= 5; customize_count++){			
			if(this.value == "color_palette"+customize_count){
				if(mobilizer_flag_change_confirm_palette == "confirm"){
					if(mobilizer_flag_previous_palette != this.value){
	            	     var r=confirm("Selecting a different color palette will delete your customized one. Are you sure?");         
		               	 if (r==true){
			            	  set_previous_color_palette_style(mobilizer_flag_previous_palette);
			              }else{
			            	  jQuery('.'+mobilizer_flag_previous_palette).attr('checked', 'checked'); 
			            	 return false;
			              }
					}
	            }
				 set_color_palette_style(this.value);
				jQuery('.customize'+customize_count).show();
			}else{
				jQuery('.customize'+customize_count).hide();
			}
		}

	  });
	
		jQuery('.customize'+customize_count_show).click(function(){
			/*before_open  this field the default color setting*/ 
			jQuery('.customize_container_main_cs').show();
			jQuery('.customize_color_palette_container').show();
			return false;
	    });
	}
	
	jQuery('.customize_done_btn').click(function(){
		 var color_palette_column = mobilizer_color_palette_style();
		 for(var col=0; col < color_palette_column.length; col++){
			 col_temp = color_palette_column[col].replace(" ","_");
				 jQuery('.'+mobilizer_flag_current_palette+'_'+col_temp+'_cs').attr('style', 'background-color: ' + jQuery('.'+col_temp+'_cs').val());
	    }
		
		 jQuery("#edit-text").val(jQuery("#edit-text-color-txt").val());
		 jQuery("#edit-link").val(jQuery("#edit-link-txt").val());
		 
		 jQuery("#edit-text").attr('style', 'background-color: ' + jQuery("#edit-text-color-txt").val());
		 jQuery("#edit-link").attr('style', 'background-color: ' + jQuery("#edit-link-txt").val());
			
		 mobilizer_flag_previous_palette = mobilizer_flag_current_palette;
		 mobilizer_flag_change_confirm_palette = "confirm";
		 
		 jQuery('.palette_border').removeClass("palette_border_active");
   	  	 jQuery('#list_'+mobilizer_flag_current_palette).addClass("palette_border_active");
		
		jQuery('.customize_color_palette_container').hide();
		jQuery('.customize_container_main_cs').hide();
		return false;
    });

	/**
	 * color_palette all palette style
	 */
	function  mobilizer_color_palette_style(){
		var color_palette_column = new Array('header background','text color','navigation','link','site background','navigation label');
		return color_palette_column;
	}
	
    /* for setting the default and current color */
	function set_color_palette_style(color_palette){
		 var mobilizer_color_palette  = Drupal.settings.mobilizer_color_palette;
		 var col_temp = "";
		 mobilizer_flag_current_palette = color_palette;
		 var mobilizer_style  = Drupal.settings.mobilizer_style;
		 
		 var color_palette_column = mobilizer_color_palette_style();
		 for(var col=0; col < color_palette_column.length; col++){
			 col_temp = color_palette_column[col].replace(" ","_");
			 
			 if(mobilizer_style && mobilizer_style['colorpalatte']['palette_id'] == color_palette){
				 jQuery('.'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_style['colorpalatte'][col_temp]);
				 jQuery('.'+col_temp+'_cs').val(mobilizer_style['colorpalatte'][col_temp]);
				 jQuery('.selected_palette_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_style['colorpalatte'][col_temp]);	 
			 }else{
			 jQuery('.'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_color_palette[color_palette][col_temp]);
			 jQuery('.'+col_temp+'_cs').val(mobilizer_color_palette[color_palette][col_temp]);
			 jQuery('.selected_palette_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_color_palette[color_palette][col_temp]);
		   }
	    }
	}
	

	function set_previous_color_palette_style(color_palette){
		 var mobilizer_color_palette  = Drupal.settings.mobilizer_color_palette;
		 var col_temp = "";
		 mobilizer_flag_current_palette = color_palette;
		 var mobilizer_style  = Drupal.settings.mobilizer_style;
		
		 var color_palette_column = mobilizer_color_palette_style();
		 for(var col=0; col < color_palette_column.length; col++){
			 col_temp = color_palette_column[col].replace(" ","_");
				if(mobilizer_style && mobilizer_style['colorpalatte']['palette_id'] == color_palette){			
					jQuery('.'+color_palette+'_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_style['colorpalatte'][col_temp]);
				 }else{

					 jQuery('.'+color_palette+'_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_color_palette[color_palette][col_temp]);
				 }
		 }
	}

	// function for Text & Links, Headings
	jQuery('.text_links_cs').click(function(){
		jQuery('.text_style_container_main_cs').show();
		jQuery('#text_style_fs').show();
		return false;
    });
	// closing trigger
	jQuery('.customize_done_btn_text').click(function(){
		jQuery("#edit-text-color-txt").val(jQuery("#edit-text").val());
		jQuery("#edit-link-txt").val(jQuery("#edit-link").val());
		jQuery("#edit-text-color-txt").attr('style', 'background-color: ' + jQuery("#edit-text").val());
		jQuery("#edit-link-txt").attr('style', 'background-color: ' + jQuery("#edit-link").val());
		
		jQuery('.'+mobilizer_flag_current_palette+'_text_color_cs').attr('style', 'background-color: ' + jQuery("#edit-text").val());
		jQuery('.'+mobilizer_flag_current_palette+'_link_cs').attr('style', 'background-color: ' + jQuery("#edit-link").val());
		
		jQuery('#text_style_fs').hide();
		jQuery('.text_style_container_main_cs').hide();
		return false;
    });
	jQuery('.header_cs').click(function(){
		jQuery('.text_heading_container_main_cs').show();
		jQuery('#headings_fs').show();
		return false;
    });
	// closing trigger
	jQuery('.customize_done_btn_heading').click(function(){					
		jQuery('#headings_fs').hide();
		jQuery('.text_heading_container_main_cs').hide();
		return false;
    });
	// END function for Text & Links, Headings
});

function set_default_color_for_style(){	
	var color_palette_column = new Array('header background','text color','navigation','link','site background','navigation label');	
	var mobilizer_style  = Drupal.settings.mobilizer_style;
	var mobilizer_color_palette  = Drupal.settings.mobilizer_color_palette;

	if(mobilizer_style){
		var color_platte_name = mobilizer_style['colorpalatte']['palette_id'];
		jQuery('#list_'+color_platte_name).addClass("palette_border_active");
		//var customize_id = color_platte_name.replace("color_palette","");
		//jQuery('.customize'+customize_id).show();
		
		 for(var palette =1; palette <=5; palette++){
			if(color_platte_name == "color_palette"+palette){
				 for(var col=0; col < color_palette_column.length; col++){
					 col_temp = color_palette_column[col].replace(" ","_");	
					 jQuery('.'+color_platte_name+'_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_style['colorpalatte'][col_temp]);
				 }			 
			}else{
				for(var col=0; col < color_palette_column.length; col++){
					 col_temp = color_palette_column[col].replace(" ","_");
					 jQuery('.color_palette'+palette+'_'+col_temp+'_cs').attr('style', 'background-color: ' + mobilizer_color_palette['color_palette'+palette][col_temp]);
				 }
				
			}
		 }
	}
}
window.onload = set_default_color_for_style;
