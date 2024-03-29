
/**
 * @file
 * getlocations_admin.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations module admin
 * jquery stuff
*/
(function ($) {

  Drupal.behaviors.getlocations_admin = {
    attach: function() {

      if ($("#edit-getlocations-default-returnlink-page-enable").prop('checked')) {
        $("#wrap-page-link").show();
      }
      else {
        $("#wrap-page-link").hide();
      }
      $("#edit-getlocations-default-returnlink-page-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-page-link").show();
        }
        else {
          $("#wrap-page-link").hide();
        }
      });

      if ($("#edit-getlocations-default-returnlink-user-enable").prop('checked')) {
        $("#wrap-user-link").show();
      }
      else {
        $("#wrap-user-link").hide();
      }
      $("#edit-getlocations-default-returnlink-user-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-user-link").show();
        }
        else {
          $("#wrap-user-link").hide();
        }
      });

      if ($("#edit-getlocations-default-returnlink-term-enable").prop('checked')) {
        $("#wrap-term-link").show();
      }
      else {
        $("#wrap-term-link").hide();
      }
      $("#edit-getlocations-default-returnlink-term-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-term-link").show();
        }
        else {
          $("#wrap-term-link").hide();
        }
      });

      if ($("#edit-getlocations-default-returnlink-comment-enable").prop('checked')) {
        $("#wrap-comment-link").show();
      }
      else {
        $("#wrap-comment-link").hide();
      }
      $("#edit-getlocations-default-returnlink-comment-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-comment-link").show();
        }
        else {
          $("#wrap-comment-link").hide();
        }
      });

      if ($("#edit-getlocations-colorbox-enable").prop('checked')) {
        $("#wrap-getlocations-colorbox").show();
      }
      else {
        $("#wrap-getlocations-colorbox").hide();
      }
      $("#edit-getlocations-colorbox-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-colorbox").show();
          var cg = $(this);
          $("#wrap-getlocations-colorbox").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-colorbox").hide();
        }
      });

      if ($("#edit-getlocations-colorbox-marker-enable").prop('checked')) {
        $("#wrap-getlocations-marker-colorbox").show();
      }
      else {
        $("#wrap-getlocations-marker-colorbox").hide();
      }
      $("#edit-getlocations-colorbox-marker-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-marker-colorbox").show();
          var cg = $(this);
          $("#wrap-getlocations-marker-colorbox").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-marker-colorbox").hide();
        }
      });

      if ($("#edit-getlocations-node-marker-enable").prop('checked')) {
        $("#wrap-getlocations-node-markers").show();
      }
      else {
        $("#wrap-getlocations-node-markers").hide();
      }
      $("#edit-getlocations-node-marker-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-node-markers").show();
        }
        else {
          $("#wrap-getlocations-node-markers").hide();
        }
      });

      if ($("#edit-getlocations-vocabulary-marker-enable").prop('checked')) {
        $("#wrap-getlocations-vocabulary-markers").show();
      }
      else {
        $("#wrap-getlocations-vocabulary-markers").hide();
      }
      $("#edit-getlocations-vocabulary-marker-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-vocabulary-markers").show();
        }
        else {
          $("#wrap-getlocations-vocabulary-markers").hide();
        }
      });

      if ($("#edit-getlocations-term-marker-enable").prop('checked')) {
        $("#wrap-getlocations-term-markers").show();
      }
      else {
        $("#wrap-getlocations-term-markers").hide();
      }
      $("#edit-getlocations-term-marker-enable").change(function() {
        if ($(this).prop('checked')) {
          $("#wrap-getlocations-term-markers").show();
        }
        else {
          $("#wrap-getlocations-term-markers").hide();
        }
      });

      if ($("#edit-getlocations-default-markermanagertype").val() == 1) {
        // markermanager
        $(".form-item-getlocations-default-usemarkermanager").show();
        $("#wrap-getlocations-clusteropts").hide();
        $("#wrap-getlocations-markeropts").show();
      }
      else if ($("#edit-getlocations-default-markermanagertype").val() == 2) {
        // markerclusterer
        $(".form-item-getlocations-default-usemarkermanager").hide();
        $("#wrap-getlocations-clusteropts").show();
        $("#wrap-getlocations-markeropts").hide();
      }
      else {
        // none
        $(".form-item-getlocations-default-usemarkermanager").hide();
        $("#wrap-getlocations-clusteropts").hide();
        $("#wrap-getlocations-markeropts").hide();
      }
      $("#edit-getlocations-default-markermanagertype").change(function() {
        if ($(this).val() == 1) {
          // markermanager
          $(".form-item-getlocations-default-usemarkermanager").show();
          $("#wrap-getlocations-clusteropts").hide();
          $("#wrap-getlocations-markeropts").show();
        }
        else if ($(this).val() == 2) {
          // markerclusterer
          $(".form-item-getlocations-default-usemarkermanager").hide();
          $("#wrap-getlocations-clusteropts").show();
          $("#wrap-getlocations-markeropts").hide();
        }
        else {
          // none
          $(".form-item-getlocations-default-usemarkermanager").hide();
          $("#wrap-getlocations-clusteropts").hide();
          $("#wrap-getlocations-markeropts").hide();
        }
      });

      if ($("#edit-getlocations-default-trafficinfo").is('input')) {
        if ($("#edit-getlocations-default-trafficinfo").prop('checked')) {
          $("#wrap-getlocations-trafficinfo").show();
        }
        else {
          $("#wrap-getlocations-trafficinfo").hide();
        }
        $("#edit-getlocations-default-trafficinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-trafficinfo").show();
          }
          else {
            $("#wrap-getlocations-trafficinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-bicycleinfo").is('input')) {
        if ($("#edit-getlocations-default-bicycleinfo").prop('checked')) {
          $("#wrap-getlocations-bicycleinfo").show();
        }
        else {
          $("#wrap-getlocations-bicycleinfo").hide();
        }
        $("#edit-getlocations-default-bicycleinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-bicycleinfo").show();
          }
          else {
            $("#wrap-getlocations-bicycleinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-transitinfo").is('input')) {
        if ($("#edit-getlocations-default-transitinfo").prop('checked')) {
          $("#wrap-getlocations-transitinfo").show();
        }
        else {
          $("#wrap-getlocations-transitinfo").hide();
        }
        $("#edit-getlocations-default-transitinfo").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-transitinfo").show();
          }
          else {
            $("#wrap-getlocations-transitinfo").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-polygons-enable").prop('checked')) {
        $("#wrap-getlocations-polygons").show();
      }
      else {
        $("#wrap-getlocations-polygons").hide();
      }
      $("#edit-getlocations-default-polygons-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-polygons").show();
          var cg = $(this);
          $("#wrap-getlocations-polygons").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-polygons").hide();
        }
      });
      if ($("#edit-getlocations-default-rectangles-enable").prop('checked')) {
        $("#wrap-getlocations-rectangles").show();
      }
      else {
        $("#wrap-getlocations-rectangles").hide();
      }
      $("#edit-getlocations-default-rectangles-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-rectangles").show();
          var cg = $(this);
          $("#wrap-getlocations-rectangles").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });

        }
        else {
          $("#wrap-getlocations-rectangles").hide();
        }
      });

      if ($("#edit-getlocations-default-circles-enable").prop('checked')) {
        $("#wrap-getlocations-circles").show();
      }
      else {
        $("#wrap-getlocations-circles").hide();
      }
      $("#edit-getlocations-default-circles-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-circles").show();
          var cg = $(this);
          $("#wrap-getlocations-circles").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-circles").hide();
        }
      });

      if ($("#edit-getlocations-default-polylines-enable").prop('checked')) {
        $("#wrap-getlocations-polylines").show();
      }
      else {
        $("#wrap-getlocations-polylines").hide();
      }
      $("#edit-getlocations-default-polylines-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-polylines").show();
          var cg = $(this);
          $("#wrap-getlocations-polylines").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });

        }
        else {
          $("#wrap-getlocations-polylines").hide();
        }
      });
      // search_places
      if ($("#edit-getlocations-default-search-places").prop('checked')) {
        $("#wrap-getlocations-search-places").show();
      }
      else {
        $("#wrap-getlocations-search-places").hide();
      }
      $("#edit-getlocations-default-search-places").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-search-places").show();
          var cg = $(this);
          $("#wrap-getlocations-search-places").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-search-places").hide();
        }
      });

      // geojson
      if ($("#edit-getlocations-default-geojson-enable").prop('checked')) {
        $("#wrap-getlocations-geojson-enable").show();
      }
      else {
        $("#wrap-getlocations-geojson-enable").hide();
      }
      $("#edit-getlocations-default-geojson-enable").change(function() {
        if ($(this).prop('checked')) {
          //$("#wrap-getlocations-geojson-enable").show();
          var cg = $(this);
          $("#wrap-getlocations-geojson-enable").slideDown({
            duration: 'fast',
            easing: 'linear',
            complete: function () {
              Drupal.getlocations.divScrollIntoView(cg);
              cg.animating = false;
            }
          });
        }
        else {
          $("#wrap-getlocations-geojson-enable").hide();
        }
      });

      if ($("#edit-getlocations-default-pancontrol").is('input')) {
        if ($("#edit-getlocations-default-pancontrol").prop('checked')) {
          $("#wrap-getlocations-pancontrol").show();
        }
        else {
          $("#wrap-getlocations-pancontrol").hide();
        }
        $("#edit-getlocations-default-pancontrol").change(function() {
          if ($(this).prop('checked')) {
            //$("#wrap-getlocations-pancontrol").show();
            var cg = $(this);
            $("#wrap-getlocations-pancontrol").slideDown({
              duration: 'fast',
              easing: 'linear',
              complete: function () {
                Drupal.getlocations.divScrollIntoView(cg);
                cg.animating = false;
              }
            });
          }
          else {
            $("#wrap-getlocations-pancontrol").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-controltype").is('select')) {
        if ($("#edit-getlocations-default-controltype").val() == 'none') {
          $("#wrap-getlocations-zoomcontrol").hide();
        }
        else {
          $("#wrap-getlocations-zoomcontrol").show();
        }
        $("#edit-getlocations-default-controltype").change(function() {
          if ($(this).val() == 'none') {
            $("#wrap-getlocations-zoomcontrol").hide();
          }
          else {
            $("#wrap-getlocations-zoomcontrol").show();
          }
        });
      }

      if ($("#edit-getlocations-default-mtc").is('select')) {
        if ($("#edit-getlocations-default-mtc").val() == 'none') {
          $("#wrap-getlocations-mapcontrol").hide();
        }
        else {
          $("#wrap-getlocations-mapcontrol").show();
        }
        $("#edit-getlocations-default-mtc").change(function() {
          if ($(this).val() == 'none') {
            $("#wrap-getlocations-mapcontrol").hide();
          }
          else {
            $("#wrap-getlocations-mapcontrol").show();
          }
        });
      }

      if ($("#edit-getlocations-default-scale").is('input')) {
        if ($("#edit-getlocations-default-scale").prop('checked')) {
          $("#wrap-getlocations-scale").show();
        }
        else {
          $("#wrap-getlocations-scale").hide();
        }
        $("#edit-getlocations-default-scale").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-scale").show();
          }
          else {
            $("#wrap-getlocations-scale").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-overview").is('input')) {
        if ($("#edit-getlocations-default-overview").prop('checked')) {
          $("#wrap-getlocations-overview").show();
        }
        else {
          $("#wrap-getlocations-overview").hide();
        }
        $("#edit-getlocations-default-overview").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-overview").show();
          }
          else {
            $("#wrap-getlocations-overview").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-sv-show").is('input')) {
        if ($("#edit-getlocations-default-sv-show").prop('checked')) {
          $("#wrap-getlocations-sv-show").show();
        }
        else {
          $("#wrap-getlocations-sv-show").hide();
        }
        $("#edit-getlocations-default-sv-show").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-sv-show").show();
          }
          else {
            $("#wrap-getlocations-sv-show").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-fullscreen").is('input')) {
        if ($("#edit-getlocations-default-fullscreen").prop('checked')) {
          $("#wrap-getlocations-fs-show").show();
        }
        else {
          $("#wrap-getlocations-fs-show").hide();
        }
        $("#edit-getlocations-default-fullscreen").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-fs-show").show();
          }
          else {
            $("#wrap-getlocations-fs-show").hide();
          }
        });
      }

      if ($("#edit-getlocations-default-highlight-enable").is('input')) {
        if ($("#edit-getlocations-default-highlight-enable").prop('checked')) {
          $("#wrap-getlocations-highlight").show();
        }
        else {
          $("#wrap-getlocations-highlight").hide();
        }
        $("#edit-getlocations-default-highlight-enable").change(function() {
          if ($(this).prop('checked')) {
            //$("#wrap-getlocations-highlight").show();
            var cg = $(this);
            $("#wrap-getlocations-highlight").slideDown({
              duration: 'fast',
              easing: 'linear',
              complete: function () {
                Drupal.getlocations.divScrollIntoView(cg);
                cg.animating = false;
              }
            });
          }
          else {
            $("#wrap-getlocations-highlight").hide();
          }
        });
      }

      // search marker
      if ($("#edit-getlocations-default-views-search-marker-enable").is('input')) {
        if ($("#edit-getlocations-default-views-search-marker-enable").prop('checked')) {
          $("#wrap-getlocations-views-search-marker").show();
        }
        else {
          $("#wrap-getlocations-views-search-marker").hide();
        }
        $("#edit-getlocations-default-views-search-marker-enable").change(function() {
          if ($(this).prop('checked')) {
            //$("#wrap-getlocations-views-search-marker").show();
            var cg = $(this);
            $("#wrap-getlocations-views-search-marker").slideDown({
              duration: 'fast',
              easing: 'linear',
              complete: function () {
                Drupal.getlocations.divScrollIntoView(cg);
                cg.animating = false;
              }
            });

          }
          else {
            $("#wrap-getlocations-views-search-marker").hide();
          }
        });
      }

      // search area shape
      if ($("#edit-getlocations-default-views-search-radshape-enable").is('input')) {
        if ($("#edit-getlocations-default-views-search-radshape-enable").prop('checked')) {
          $("#wrap-getlocations-views-search-radshape").show();
        }
        else {
          $("#wrap-getlocations-views-search-radshape").hide();
        }
        $("#edit-getlocations-default-views-search-radshape-enable").change(function() {
          if ($(this).prop('checked')) {
            //$("#wrap-getlocations-views-search-radshape").show();
            var cg = $(this);
            $("#wrap-getlocations-views-search-radshape").slideDown({
              duration: 'fast',
              easing: 'linear',
              complete: function () {
                Drupal.getlocations.divScrollIntoView(cg);
                cg.animating = false;
              }
            });
          }
          else {
            $("#wrap-getlocations-views-search-radshape").hide();
          }
        });
      }

      //edit-getlocations-default-show-maplinks-viewport
      if ($("#edit-getlocations-default-show-maplinks").is('input')) {
        if ($("#edit-getlocations-default-show-maplinks").prop('checked')) {
          $("#wrap-getlocations-maplinks").show();
        }
        else {
          $("#wrap-getlocations-maplinks").hide();
        }
        $("#edit-getlocations-default-show-maplinks").change(function() {
          if ($(this).prop('checked')) {
            $("#wrap-getlocations-maplinks").show();
          }
          else {
            $("#wrap-getlocations-maplinks").hide();
          }
        });
      }

      // What3Words #edit-getlocations-default-what3words-enable
      if ($("#edit-getlocations-default-what3words-enable").is('input')) {
        if ($("#edit-getlocations-default-what3words-enable").prop('checked')) {
          $("#wrap-getlocations-what3words").show();
        }
        else {
          $("#wrap-getlocations-what3words").hide();
        }
        $("#edit-getlocations-default-what3words-enable").change(function() {
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
