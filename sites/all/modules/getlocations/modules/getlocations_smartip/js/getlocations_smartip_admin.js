/**
 * @file
 * getlocations_smartip.admin.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations_smartip module admin
 * jquery stuff
 */
(function ($) {
  Drupal.behaviors.getlocations_admin_smartip = {
    attach: function() {
      // smartip button
      if ($("input[id$=smartip-button]").is('input')) {
        if ($("input[id$=smartip-button]").attr('checked')) {
          $("#wrap-getlocations-smartip-button").show();
        }
        else {
          $("#wrap-getlocations-smartip-button").hide();
        }
        $("input[id$=smartip-button]").change(function() {
          if ($(this).attr('checked')) {
            $("#wrap-getlocations-smartip-button").show();
          }
          else {
            $("#wrap-getlocations-smartip-button").hide();
          }
        });
      }
    }
  };
}(jQuery));
