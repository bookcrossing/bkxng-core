
/**
 * @file
 * getlocations_what3words.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations_leaflet module
*/
(function ($) {

  Drupal.behaviors.getlocations_leaflet_what3words = {
    attach: function(context) {

      // work over all class 'getlocations_leaflet_canvas'
      $(".getlocations_leaflet_canvas", context).once('getlocations-leaflet-what3words-processed', function(index, element) {
        var elemID = $(element).attr('id');
        var key = elemID.replace(/^getlocations_leaflet_canvas_/, '');
        // is there really a map?
        if ( $("#getlocations_leaflet_canvas_" + key).is('div') ) {
          settings = Drupal.settings.getlocations_leaflet[key].map_settings;
          var w3w = '';
          var lat = false;
          var lon = false;
          var marker = false;
          if (settings.what3words_enable) {
            if (settings.what3words_search && $("#edit-getlocations-leaflet-what3words-search").is('input')) {
              $("#edit-getlocations-leaflet-what3words-search-submit").click( function() {
                $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_inactive').addClass('getlocations_w3w_throbber_active');
                var s = $("#edit-getlocations-leaflet-what3words-search").val();
                // validate the string, 3 words sep by spaces or dots
                // first replace spaces with dots
                s = s.replace(/\s+/g, '.');
                var arr = s.split(/\./g);
                // then find out if we have enough words
                if (arr.length == 3) {
                  // go get the latlon
                  w3w = s;
                  data = {'data': w3w};
                  $.get(settings.what3words_path, data, function(r) {
                    response = JSON.parse(r);
                    if (response) {
                      var lat = response.position[0];
                      var lon = response.position[1];
                      if (settings.what3words_marker_show && lat && lon ) {
                        if (marker) {
                          Drupal.getlocations_leaflet_map[key].removeLayer(marker);
                        }
                        marker = Drupal.getlocations_leaflet.makeMarker(settings, lat, lon, '', 0, 0, w3w, settings.what3words_map_marker, '', '', '', key);
                        Drupal.getlocations_leaflet_map[key].addLayer(marker);
                      }
                      if (settings.what3words_zoom == -1) {
                        Drupal.getlocations_leaflet_map[key].setZoom(parseInt(settings.nodezoom));
                      }
                      else if (settings.what3words_zoom > -1) {
                        Drupal.getlocations_leaflet_map[key].setZoom(parseInt(settings.what3words_zoom));
                      }
                      if (settings.what3words_show && $("#edit-getlocations-leaflet-what3words-show").is('div') && w3w) {
                        $("#edit-getlocations-leaflet-what3words-show").html(w3w);
                      }
                      if (settings.what3words_center) {
                        var lla = {lat: parseFloat(lat), lng: parseFloat(lon)};
                        Drupal.getlocations_leaflet_map[key].panTo(lla);
                      }
                    }
                    $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_active').addClass('getlocations_w3w_throbber_inactive');
                  });
                }
                else {
                  $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_active').addClass('getlocations_w3w_throbber_inactive');
                }
              });
            }

            if (settings.what3words_click) {
              Drupal.getlocations_leaflet_map[key].on('click', function(e) {
                $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_inactive').addClass('getlocations_w3w_throbber_active');
                var ll = e.latlng;
                var wlat = ll.lat;
                var wlon = ll.lng;
                // go get w3w for ll
                data = {'data': parseFloat(wlat) + ',' + parseFloat(wlon)};
                $.get(settings.what3words_path, data, function(r) {
                  response = JSON.parse(r);
                  if (response) {
                    w3w = response.words.join('.');
                    if (settings.what3words_marker_show && wlat && wlon ) {
                      if (marker) {
                        // kill marker
                         Drupal.getlocations_leaflet_map[key].removeLayer(marker);
                      }
                      marker = Drupal.getlocations_leaflet.makeMarker(settings, wlat, wlon, '', 0, 0, w3w, settings.what3words_map_marker, '', '', '', key);
                      Drupal.getlocations_leaflet_map[key].addLayer(marker);
                    }

                    if (settings.what3words_zoom == -1) {
                      Drupal.getlocations_leaflet_map[key].setZoom(parseInt(settings.nodezoom));
                    }
                    else if (settings.what3words_zoom > -1) {
                      Drupal.getlocations_leaflet_map[key].setZoom(parseInt(settings.what3words_zoom));
                    }

                    if (settings.what3words_show && $("#edit-getlocations-leaflet-what3words-show").is('div') && w3w) {
                      $("#edit-getlocations-leaflet-what3words-show").html(w3w);
                    }
                    if (settings.what3words_center) {
                      var wlla = {lat: parseFloat(wlat), lng: parseFloat(wlon)};
                      Drupal.getlocations_leaflet_map[key].panTo(wlla);
                    }
                  }
                  $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_active').addClass('getlocations_w3w_throbber_inactive');
                });
              }); // end map click listener
            }

          }

        } // end is there really a map?
      }); // end once
    }
  };

})(jQuery);
