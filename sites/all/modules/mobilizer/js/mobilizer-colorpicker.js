/**
 * Implementation of hook_elements.
 *
 * Much of the colorpicker code was adapted from the Colorpicker module.
 * That module has no stable release yet nor any D6 branch.
 */
/*
 *  Bind the colorpicker event to the form element
 */
(function ($) {
  Drupal.behaviors.field_example_colorpicker = {
    attach: function(context) {
      $(".edit-palette-colorpicker").live("focus", function(event) {
        var edit_field = this;
        //alert(edit_field.id);
        var picker = $(this).closest('div').parent().find(".palette-colorpicker");
        console.log(picker);
        // Hide all color pickers except this one.
        $(".palette-colorpicker").hide();
        $(picker).show();
        $.farbtastic(picker, function(color) {
          edit_field.value = color;
          edit_field.style.backgroundColor = color; // this is add for background color in text box (pankaj chahuan).
          // set palette preview color on the fly
          $("#list_color_paletteno ."+edit_field.id).css("background-color", color);
        }).setColor(edit_field.value);
      });
    }
  }
   $(".edit-palette-colorpicker").live('blur',function (event){
      var edit_field = this;
      var picker = $(this).closest('div').parent().find(".palette-colorpicker");
        $(picker).hide();          
      });
})(jQuery);

