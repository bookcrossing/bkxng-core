
/**
 * @file
 * getlocations_leaflet_formatter.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations_leaflet module admin
 * jquery gee whizzery
*/
(function ($) {
  Drupal.behaviors.getlocations_leaflet_formatter = {
    attach: function(context, settings) {

      if ($("input[id$=scalecontrol]").prop('checked')) {
        $("#wrap-getlocations-scalecontrol").show();
      }
      else {
        $("#wrap-getlocations-scalecontrol").hide();
      }
      $("input[id$=scalecontrol]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-scalecontrol").show();
        }
        else {
          $("#wrap-getlocations-scalecontrol").hide();
        }
      });

      if ($("input[id$=zoomcontrol]").prop('checked')) {
        $("#wrap-getlocations-zoomcontrol").show();
      }
      else {
        $("#wrap-getlocations-zoomcontrol").hide();
      }
      $("input[id$=zoomcontrol]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-zoomcontrol").show();
        }
        else {
          $("#wrap-getlocations-zoomcontrol").hide();
        }
      });

      if ($("input[id$=attributioncontrol]").prop('checked')) {
        $("#wrap-getlocations-attributioncontrol").show();
      }
      else {
        $("#wrap-getlocations-attributioncontrol").hide();
      }
      $("input[id$=attributioncontrol]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-attributioncontrol").show();
        }
        else {
          $("#wrap-getlocations-attributioncontrol").hide();
        }
      });

      if ($("input[id$=layercontrol]").prop('checked')) {
        $("#wrap-getlocations-layercontrol").show();
      }
      else {
        $("#wrap-getlocations-layercontrol").hide();
      }
      $("input[id$=layercontrol]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-layercontrol").show();
        }
        else {
          $("#wrap-getlocations-layercontrol").hide();
        }
      });

      if ($("input[id*=polygons-enable]").prop('checked')) {
        $("#wrap-getlocations-polygons").show();
      }
      else {
        $("#wrap-getlocations-polygons").hide();
      }
      $("input[id*=polygons-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-polygons").show();
        }
        else {
          $("#wrap-getlocations-polygons").hide();
        }
      });

      if ($("input[id*=rectangles-enable]").prop('checked')) {
        $("#wrap-getlocations-rectangles").show();
      }
      else {
        $("#wrap-getlocations-rectangles").hide();
      }
      $("input[id*=rectangles-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-rectangles").show();
        }
        else {
          $("#wrap-getlocations-rectangles").hide();
        }
      });

      if ($("input[id*=circles-enable]").prop('checked')) {
        $("#wrap-getlocations-circles").show();
      }
      else {
        $("#wrap-getlocations-circles").hide();
      }
      $("input[id*=circles-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-circles").show();
        }
        else {
          $("#wrap-getlocations-circles").hide();
        }
      });

      if ($("input[id*=polylines-enable]").prop('checked')) {
       $("#wrap-getlocations-polylines").show();
      }
      else {
        $("#wrap-getlocations-polylines").hide();
      }
      $("input[id*=polylines-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-polylines").show();
        }
        else {
          $("#wrap-getlocations-polylines").hide();
        }
      });

      if ($("input[id$=geojson-enable]").prop('checked')) {
       $("#wrap-getlocations-geojson-enable").show();
      }
      else {
        $("#wrap-getlocations-geojson-enable").hide();
      }
      $("input[id$=geojson-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-geojson-enable").show();
        }
        else {
          $("#wrap-getlocations-geojson-enable").hide();
        }
      });

      if ($("input[id$=fullscreen]").prop('checked')) {
        $("#getlocations-fullscreen-wrap").show();
      }
      else {
        $("#getlocations-fullscreen-wrap").hide();
      }
      $("input[id$=fullscreen]").change(function() {
        if ($(this).prop('checked')) {
          $("#getlocations-fullscreen-wrap").show();
        }
        else {
          $("#getlocations-fullscreen-wrap").hide();
        }
      });

      if ($("input[id$=pancontrol]").prop('checked')) {
       $("#getlocations-pancontrol-wrap").show();
      }
      else {
        $("#getlocations-pancontrol-wrap").hide();
      }
      $("input[id$=pancontrol]").change(function() {
        if ($(this).prop('checked')) {
          $("#getlocations-pancontrol-wrap").show();
        }
        else {
          $("#getlocations-pancontrol-wrap").hide();
        }
      });

      if ($("input[id$=zoomslider]").prop('checked')) {
       $("#getlocations-zoomslider-wrap").show();
      }
      else {
        $("#getlocations-zoomslider-wrap").hide();
      }
      $("input[id$=zoomslider]").change(function() {
        if ($(this).prop('checked')) {
          $("#getlocations-zoomslider-wrap").show();
        }
        else {
          $("#getlocations-zoomslider-wrap").hide();
        }
      });

      if ($("input[id$=returnlink-page-enable]").prop('checked')) {
        $("#wrap-page-link").show();
      }
      else {
        $("#wrap-page-link").hide();
      }
      $("input[id$=returnlink-page-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-page-link").show();
        }
        else {
          $("#wrap-page-link").hide();
        }
      });

      if ($("input[id$=returnlink-user-enable]").prop('checked')) {
        $("#wrap-user-link").show();
      }
      else {
        $("#wrap-user-link").hide();
      }
      $("input[id$=returnlink-user-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-user-link").show();
        }
        else {
          $("#wrap-user-link").hide();
        }
      });

      if ($("input[id$=returnlink-term-enable]").prop('checked')) {
        $("#wrap-term-link").show();
      }
      else {
        $("#wrap-term-link").hide();
      }
      $("input[id$=returnlink-term-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-term-link").show();
        }
        else {
          $("#wrap-term-link").hide();
        }
      });

      if ($("input[id$=returnlink-comment-enable]").prop('checked')) {
        $("#wrap-comment-link").show();
      }
      else {
        $("#wrap-comment-link").hide();
      }
      $("input[id$=returnlink-comment-enable]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-comment-link").show();
        }
        else {
          $("#wrap-comment-link").hide();
        }
      });

      // categories
      if ($("#edit-style-options-category-method").val() > 0) {
        $("#wrap-category1").show();
        if ($("#edit-style-options-category-method").val() == 2) {
          $("#wrap-category2").show();
        }
        else {
          $("#wrap-category2").hide();
        }
      }
      else {
        $("#wrap-category1").hide();
      }
      $("#edit-style-options-category-method").change(function() {
        if ($("#edit-style-options-category-method").val() > 0) {
          $("#wrap-category1").show();
          if ($("#edit-style-options-category-method").val() == 2) {
            $("#wrap-category2").show();
          }
          else {
            $("#wrap-category2").hide();
          }
        }
        else {
          $("#wrap-category1").hide();
        }
      });

      if($('#edit-style-options-custom-content-enable').prop('checked')) {
        $('#wrap-custom-content-source').show();
      }
      else {
        $('#wrap-custom-content-source').hide();
      }
      $("#edit-style-options-custom-content-enable").change(function() {
        if($(this).prop('checked')) {
          $('#wrap-custom-content-source').show();
        }
        else {
          $('#wrap-custom-content-source').hide();
        }
      });

      if ($("select[id$=marker-type]").val() == 'fa') {
        $('#getlocations-awesome-wrap').show();
      }
      else {
        $('#getlocations-awesome-wrap').hide();
      }
      $("select[id$=marker-type]").change(function() {
        if ($("select[id$=marker-type]").val() == 'fa') {
          $('#getlocations-awesome-wrap').show();
        }
        else {
          $('#getlocations-awesome-wrap').hide();
        }
      });

      if ($("input[id$=graticule]").prop('checked')) {
       $("#getlocations-graticule-wrap").show();
      }
      else {
        $("#getlocations-graticule-wrap").hide();
      }
      $("input[id$=graticule]").change(function() {
        if ($(this).prop('checked')) {
          $("#getlocations-graticule-wrap").show();
        }
        else {
          $("#getlocations-graticule-wrap").hide();
        }
      });

      if ($("input[id$=mouseposition]").prop('checked')) {
       $("#getlocations-mouseposition-wrap").show();
      }
      else {
        $("#getlocations-mouseposition-wrap").hide();
      }
      $("input[id$=mouseposition]").change(function() {
        if ($(this).prop('checked')) {
          $("#getlocations-mouseposition-wrap").show();
        }
        else {
          $("#getlocations-mouseposition-wrap").hide();
        }
      });

      if ($("input[id$=geocoder]").prop('checked')) {
       $("#wrap-getlocations-geocoder").show();
      }
      else {
        $("#wrap-getlocations-geocoder").hide();
      }
      $("input[id$=geocoder]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-geocoder").show();
        }
        else {
          $("#wrap-getlocations-geocoder").hide();
        }
      });

      if ($("input[id$=usermarker]").prop('checked')) {
       $("#wrap-getlocations-usermarker").show();
      }
      else {
        $("#wrap-getlocations-usermarker").hide();
      }
      $("input[id$=usermarker]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-usermarker").show();
        }
        else {
          $("#wrap-getlocations-usermarker").hide();
        }
      });

      if ($("input[id$=bouncemarker]").prop('checked')) {
        $("#wrap-getlocations-bouncemarker").show();
      }
      else {
        $("#wrap-getlocations-bouncemarker").hide();
      }
      $("input[id$=bouncemarker]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-bouncemarker").show();
        }
        else {
          $("#wrap-getlocations-bouncemarker").hide();
        }
      });

      if ($("input[id$=magnifyingglass]").prop('checked')) {
        $("#wrap-getlocations-magnifyingglass").show();
      }
      else {
        $("#wrap-getlocations-magnifyingglass").hide();
      }
      $("input[id$=magnifyingglass]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-magnifyingglass").show();
        }
        else {
          $("#wrap-getlocations-magnifyingglass").hide();
        }
      });

      if ($("input[id$=terminator]").prop('checked')) {
        $("#wrap-getlocations-terminator").show();
      }
      else {
        $("#wrap-getlocations-terminator").hide();
      }
      $("input[id$=terminator]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-terminator").show();
        }
        else {
          $("#wrap-getlocations-terminator").hide();
        }
      });

      if ($("input[id$=mapquest-maps-use]").prop('checked')) {
        $("#wrap-getlocations-mapquest-maps-use").show();
      }
      else {
        $("#wrap-getlocations-mapquest-maps-use").hide();
      }
      $("input[id$=mapquest-maps-use]").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-mapquest-maps-use").show();
        }
        else {
          $("#wrap-getlocations-mapquest-maps-use").hide();
        }
      });

      // search marker
      if ($("#edit-getlocations-leaflet-defaults-views-search-marker-enable").is('input')) {
        if ($("#edit-getlocations-leaflet-defaults-views-search-marker-enable").prop('checked')) {
          $("#wrap-getlocations-views-search-marker").show();
        }
        else {
          $("#wrap-getlocations-views-search-marker").hide();
        }
        $("#edit-getlocations-leaflet-defaults-views-search-marker-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-views-search-marker").show();
          }
          else {
            $("#wrap-getlocations-views-search-marker").hide();
          }
        });
      }

      // search area shape
      if ($("#edit-getlocations-leaflet-defaults-views-search-radshape-enable").is('input')) {
        if ($("#edit-getlocations-leaflet-defaults-views-search-radshape-enable").prop('checked')) {
          $("#wrap-getlocations-views-search-radshape").show();
        }
        else {
          $("#wrap-getlocations-views-search-radshape").hide();
        }
        $("#edit-getlocations-leaflet-defaults-views-search-radshape-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-views-search-radshape").show();
          }
          else {
            $("#wrap-getlocations-views-search-radshape").hide();
          }
        });
      }
      //edit-getlocations-default-show-maplinks-viewport
      if ($("#edit-getlocations-leaflet-defaults-show-maplinks").is('input')) {
        if ($("#edit-getlocations-leaflet-defaults-show-maplinks").prop('checked')) {
          $("#wrap-getlocations-maplinks").show();
        }
        else {
          $("#wrap-getlocations-maplinks").hide();
        }
        $("#edit-getlocations-leaflet-defaults-show-maplinks").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-maplinks").show();
          }
          else {
            $("#wrap-getlocations-maplinks").hide();
          }
        });
      }

      // #edit-getlocations-leaflet-defaults-what3words-enable
      // What3Words #edit-getlocations-leaflet-default-what3words-enable
      if ($("#edit-getlocations-leaflet-defaults-what3words-enable").is('input')) {
        if ($("#edit-getlocations-leaflet-defaults-what3words-enable").prop('checked')) {
          $("#wrap-getlocations-what3words").show();
        }
        else {
          $("#wrap-getlocations-what3words").hide();
        }
        $("#edit-getlocations-leaflet-defaults-what3words-enable").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-what3words").show();
          }
          else {
            $("#wrap-getlocations-what3words").hide();
          }
        });
      }



    }
  };
})(jQuery);
