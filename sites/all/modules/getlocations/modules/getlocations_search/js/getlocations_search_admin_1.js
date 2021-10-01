/**
 * @file
 * getlocations_search_admin.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations_search admin
*/
(function ($) {

  Drupal.behaviors.getlocations_search_admin = {
    attach: function() {

      if ($("#edit-getlocations-search-defaults-method, #edit-getlocations-search-block-defaults-method").val() == 'vocab') {
        $("#getlocations_search_defaults_vocab").show();
      }
      else {
        $("#getlocations_search_defaults_vocab").hide();
      }
      $("#edit-getlocations-search-defaults-method, #edit-getlocations-search-block-defaults-method").change( function() {
        if ($(this).val() == 'vocab') {
          $("#getlocations_search_defaults_vocab").show();
        }
        else {
          $("#getlocations_search_defaults_vocab").hide();
        }
      });

      if ($("#edit-getlocations-search-defaults-restrict-by-country, #edit-getlocations-search-block-defaults-restrict-by-country").is('input')) {
        if ($("#edit-getlocations-search-defaults-restrict-by-country, #edit-getlocations-search-block-defaults-restrict-by-country").prop('checked')) {
          $("#getlocations_search_country").show();
        }
        else {
          $("#getlocations_search_country").hide();
        }
        $("#edit-getlocations-search-defaults-restrict-by-country, #edit-getlocations-search-block-defaults-restrict-by-country").change( function() {
          if ($("#edit-getlocations-search-defaults-restrict-by-country, #edit-getlocations-search-block-defaults-restrict-by-country").prop('checked')) {
            $("#getlocations_search_country").show();
          }
          else {
            $("#getlocations_search_country").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-markermanagertype, #edit-getlocations-search-block-defaults-markermanagertype").val() == 1) {
        // markermanager
        $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").show();
        $("#wrap-getlocations-clusteropts").hide();
        $("#wrap-getlocations-markeropts").show();
      }
      else if ($("#edit-getlocations-search-defaults-markermanagertype, #edit-getlocations-search-block-defaults-markermanagertype").val() == 2) {
        // markerclusterer
        $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").hide();
        $("#wrap-getlocations-clusteropts").show();
        $("#wrap-getlocations-markeropts").hide();
      }
      else {
        // none
        $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").hide();
        $("#wrap-getlocations-clusteropts").hide();
        $("#wrap-getlocations-markeropts").hide();
      }
      $("#edit-getlocations-search-defaults-markermanagertype, #edit-getlocations-search-block-defaults-markermanagertype").change(function() {
        if ($(this).val() == 1) {
          // markermanager
          $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").show();
          $("#wrap-getlocations-clusteropts").hide();
          $("#wrap-getlocations-markeropts").show();
        }
        else if ($(this).val() == 2) {
          // markerclusterer
          $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").hide();
          $("#wrap-getlocations-clusteropts").show();
          $("#wrap-getlocations-markeropts").hide();
        }
        else {
          // none
          $(".form-item-getlocations-search-defaults-usemarkermanager, .form-item-getlocations-search-block-defaults-usemarkermanager").hide();
          $("#wrap-getlocations-clusteropts").hide();
          $("#wrap-getlocations-markeropts").hide();
        }
      });

      if ($("#edit-getlocations-search-defaults-trafficinfo, #edit-getlocations-search-block-defaults-trafficinfo").is('input')) {
        if ($("#edit-getlocations-search-defaults-trafficinfo, #edit-getlocations-search-block-defaults-trafficinfo").prop('checked')) {
          $("#wrap-getlocations-trafficinfo").show();
        }
        else {
          $("#wrap-getlocations-trafficinfo").hide();
        }
        $("#edit-getlocations-search-defaults-trafficinfo, #edit-getlocations-search-block-defaults-trafficinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-trafficinfo").show();
          }
          else {
            $("#wrap-getlocations-trafficinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-bicycleinfo, #edit-getlocations-search-block-defaults-bicycleinfo").is('input')) {
        if ($("#edit-getlocations-search-defaults-bicycleinfo, #edit-getlocations-search-block-defaults-bicycleinfo").prop('checked')) {
          $("#wrap-getlocations-bicycleinfo").show();
        }
        else {
          $("#wrap-getlocations-bicycleinfo").hide();
        }
        $("#edit-getlocations-search-defaults-bicycleinfo, #edit-getlocations-search-block-defaults-bicycleinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-bicycleinfo").show();
          }
          else {
            $("#wrap-getlocations-bicycleinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-transitinfo, #edit-getlocations-search-block-defaults-transitinfo").is('input')) {
        if ($("#edit-getlocations-search-defaults-transitinfo, #edit-getlocations-search-block-defaults-transitinfo").prop('checked')) {
          $("#wrap-getlocations-transitinfo").show();
        }
        else {
          $("#wrap-getlocations-transitinfo").hide();
        }
        $("#edit-getlocations-search-defaults-transitinfo, #edit-getlocations-search-block-defaults-transitinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-transitinfo").show();
          }
          else {
            $("#wrap-getlocations-transitinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-polygons-enable, #edit-getlocations-search-block-defaults-polygons-enable").prop('checked')) {
        $("#wrap-getlocations-polygons").show();
      }
      else {
        $("#wrap-getlocations-polygons").hide();
      }
      $("#edit-getlocations-search-defaults-polygons-enable, #edit-getlocations-search-block-defaults-polygons-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-polygons").show();
        }
        else {
          $("#wrap-getlocations-polygons").hide();
        }
      });

      if ($("#edit-getlocations-search-defaults-rectangles-enable, #edit-getlocations-search-block-defaults-rectangles-enable").prop('checked')) {
        $("#wrap-getlocations-rectangles").show();
      }
      else {
        $("#wrap-getlocations-rectangles").hide();
      }
      $("#edit-getlocations-search-defaults-rectangles-enable, #edit-getlocations-search-block-defaults-rectangles-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-rectangles").show();
        }
        else {
          $("#wrap-getlocations-rectangles").hide();
        }
      });

      if ($("#edit-getlocations-search-defaults-circles-enable, #edit-getlocations-search-block-defaults-circles-enable").prop('checked')) {
        $("#wrap-getlocations-circles").show();
      }
      else {
        $("#wrap-getlocations-circles").hide();
      }
      $("#edit-getlocations-search-defaults-circles-enable, #edit-getlocations-search-block-defaults-circles-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-circles").show();
        }
        else {
          $("#wrap-getlocations-circles").hide();
        }
      });

      if ($("#edit-getlocations-search-defaults-polylines-enable, #edit-getlocations-search-block-defaults-polylines-enable").prop('checked')) {
        $("#wrap-getlocations-polylines").show();
      }
      else {
        $("#wrap-getlocations-polylines").hide();
      }
      $("#edit-getlocations-search-defaults-polylines-enable, #edit-getlocations-search-block-defaults-polylines-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-polylines").show();
        }
        else {
          $("#wrap-getlocations-polylines").hide();
        }
      });

      // search_places
      if ($("#edit-getlocations-search-defaults-search-places, #edit-getlocations-search-block-defaults-search-places").prop('checked')) {
        $("#wrap-getlocations-search-places").show();
      }
      else {
        $("#wrap-getlocations-search-places").hide();
      }
      $("#edit-getlocations-search-defaults-search-places, #edit-getlocations-search-block-defaults-search-places").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-search-places").show();
        }
        else {
          $("#wrap-getlocations-search-places").hide();
        }
      });

      // search marker
      if ($("#edit-getlocations-search-defaults-do-search-marker, #edit-getlocations-search-block-defaults-do-search-marker").is('input')) {
        if ($("#edit-getlocations-search-defaults-do-search-marker, #edit-getlocations-search-block-defaults-do-search-marker").prop('checked')) {
          $("#wrap-getlocations-search-marker").show();
        }
        else {
          $("#wrap-getlocations-search-marker").hide();
        }
        $("#edit-getlocations-search-defaults-do-search-marker, #edit-getlocations-search-block-defaults-do-search-marker").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-search-marker").show();
          }
          else {
            $("#wrap-getlocations-search-marker").hide();
          }
        });
      }

      // search area shape
      if ($("#edit-getlocations-search-defaults-search-radshape-enable, #edit-getlocations-search-block-defaults-search-radshape-enable").is('input')) {
        if ($("#edit-getlocations-search-defaults-search-radshape-enable, #edit-getlocations-search-block-defaults-search-radshape-enable").prop('checked')) {
          $("#wrap-getlocations-search-radshape").show();
        }
        else {
          $("#wrap-getlocations-search-radshape").hide();
        }
        $("#edit-getlocations-search-defaults-search-radshape-enable, #edit-getlocations-search-block-defaults-search-radshape-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-search-radshape").show();
          }
          else {
            $("#wrap-getlocations-search-radshape").hide();
          }
        });
      }

      // geojson
      if ($("#edit-getlocations-search-defaults-geojson-enable, #edit-getlocations-search-block-defaults-geojson-enable").is('input')) {
        if ($("#edit-getlocations-search-defaults-geojson-enable, #edit-getlocations-search-block-defaults-geojson-enable").prop('checked')) {
          $("#wrap-getlocations-geojson-enable").show();
        }
        else {
          $("#wrap-getlocations-geojson-enable").hide();
        }
        $("#edit-getlocations-search-defaults-geojson-enable, #edit-getlocations-search-block-defaults-geojson-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-geojson-enable").show();
          }
          else {
            $("#wrap-getlocations-geojson-enable").hide();
          }
        });
      }

      // zoom on single use
      if ($("#edit-getlocations-search-defaults-zoom-on-single-use, #edit-getlocations-search-block-defaults-zoom-on-single-use").is('input')) {
        if ($("#edit-getlocations-search-defaults-zoom-on-single-use, #edit-getlocations-search-block-defaults-zoom-on-single-use").prop('checked')) {
          $("#wrap-zoom-on-single-use").show();
        }
        else {
          $("#wrap-zoom-on-single-use").hide();
        }
        $("#edit-getlocations-search-defaults-zoom-on-single-use, #edit-getlocations-search-block-defaults-zoom-on-single-use").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-zoom-on-single-use").show();
          }
          else {
            $("#wrap-zoom-on-single-use").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-pancontrol, #edit-getlocations-search-block-defaults-pancontrol").is('input')) {
        if ($("#edit-getlocations-search-defaults-pancontrol, #edit-getlocations-search-block-defaults-pancontrol").prop('checked')) {
          $("#wrap-getlocations-pancontrol").show();
        }
        else {
          $("#wrap-getlocations-pancontrol").hide();
        }
        $("#edit-getlocations-search-defaults-pancontrol, #edit-getlocations-search-block-defaults-pancontrol").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-pancontrol").show();
          }
          else {
            $("#wrap-getlocations-pancontrol").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-controltype, #edit-getlocations-search-block-defaults-controltype").is('select')) {
        if ($("#edit-getlocations-search-defaults-controltype, #edit-getlocations-search-block-defaults-controltype").val() == 'none') {
          $("#wrap-getlocations-zoomcontrol").hide();
        }
        else {
          $("#wrap-getlocations-zoomcontrol").show();
        }
        $("#edit-getlocations-search-defaults-controltype, #edit-getlocations-search-block-defaults-controltype").change(function() {
          if ($(this).val() == 'none') {
            $("#wrap-getlocations-zoomcontrol").hide();
          }
          else {
            $("#wrap-getlocations-zoomcontrol").show();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-mtc, #edit-getlocations-search-block-defaults-mtc").is('select')) {
        if ($("#edit-getlocations-search-defaults-mtc, #edit-getlocations-search-block-defaults-mtc").val() == 'none') {
          $("#wrap-getlocations-mapcontrol").hide();
        }
        else {
          $("#wrap-getlocations-mapcontrol").show();
        }
        $("#edit-getlocations-search-defaults-mtc, #edit-getlocations-search-block-defaults-mtc").change(function() {
          if ($(this).val() == 'none') {
            $("#wrap-getlocations-mapcontrol").hide();
          }
          else {
            $("#wrap-getlocations-mapcontrol").show();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-scale, #edit-getlocations-search-block-defaults-scale").is('input')) {
        if ($("#edit-getlocations-search-defaults-scale, #edit-getlocations-search-block-defaults-scale").prop('checked')) {
          $("#wrap-getlocations-scale").show();
        }
        else {
          $("#wrap-getlocations-scale").hide();
        }
        $("#edit-getlocations-search-defaults-scale, #edit-getlocations-search-block-defaults-scale").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-scale").show();
          }
          else {
            $("#wrap-getlocations-scale").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-overview, #edit-getlocations-search-block-defaults-overview").is('input')) {
        if ($("#edit-getlocations-search-defaults-overview, #edit-getlocations-search-block-defaults-overview").prop('checked')) {
          $("#wrap-getlocations-overview").show();
        }
        else {
          $("#wrap-getlocations-overview").hide();
        }
        $("#edit-getlocations-search-defaults-overview, #edit-getlocations-search-block-defaults-overview").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-overview").show();
          }
          else {
            $("#wrap-getlocations-overview").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-sv-show, #edit-getlocations-search-block-defaults-sv-show").is('input')) {
        if ($("#edit-getlocations-search-defaults-sv-show, #edit-getlocations-search-block-defaults-sv-show").prop('checked')) {
          $("#wrap-getlocations-sv-show").show();
        }
        else {
          $("#wrap-getlocations-sv-show").hide();
        }
        $("#edit-getlocations-search-defaults-sv-show, #edit-getlocations-search-block-defaults-sv-show").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-sv-show").show();
          }
          else {
            $("#wrap-getlocations-sv-show").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-fullscreen, #edit-getlocations-search-block-defaults-fullscreen").is('input')) {
        if ($("#edit-getlocations-search-defaults-fullscreen, #edit-getlocations-search-block-defaults-fullscreen").prop('checked')) {
          $("#wrap-getlocations-fs-show").show();
        }
        else {
          $("#wrap-getlocations-fs-show").hide();
        }
        $("#edit-getlocations-search-defaults-fullscreen, #edit-getlocations-search-block-defaults-fullscreen").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-fs-show").show();
          }
          else {
            $("#wrap-getlocations-fs-show").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-highlight-enable, #edit-getlocations-search-block-defaults-highlight-enable").is('input')) {
        if ($("#edit-getlocations-search-defaults-highlight-enable, #edit-getlocations-search-block-defaults-highlight-enable").prop('checked')) {
          $("#wrap-getlocations-highlight").show();
        }
        else {
          $("#wrap-getlocations-highlight").hide();
        }
        $("#edit-getlocations-search-defaults-highlight-enable, #edit-getlocations-search-block-defaults-highlight-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-highlight").show();
          }
          else {
            $("#wrap-getlocations-highlight").hide();
          }
        });
      }

      if ($("#edit-getlocations-search-defaults-show-maplinks, #edit-getlocations-search-block-defaults-show-maplinks").is('input')) {
        if ($("#edit-getlocations-search-defaults-show-maplinks, #edit-getlocations-search-block-defaults-show-maplinks").prop('checked')) {
          $("#wrap-getlocations-maplinks").show();
        }
        else {
          $("#wrap-getlocations-maplinks").hide();
        }
        $("#edit-getlocations-search-defaults-show-maplinks, #edit-getlocations-search-block-defaults-show-maplinks").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-maplinks").show();
          }
          else {
            $("#wrap-getlocations-maplinks").hide();
          }
        });
      }


    }
  };
})(jQuery);
