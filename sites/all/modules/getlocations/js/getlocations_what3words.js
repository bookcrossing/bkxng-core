
/**
 * @file
 * getlocations_what3words.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations module
*/
(function ($) {

  Drupal.behaviors.getlocations_what3words = {
    attach: function() {

      // work over all class 'getlocations_map_canvas'
      $(".getlocations_map_canvas").once('getlocations-map-what3words-processed', function(index, element) {
        var elemID = $(element).attr('id');
        var key = elemID.replace(/^getlocations_map_canvas_/, '');
        // is there really a map?
        if ($("#getlocations_map_canvas_" + key).is('div')) {
          settings = Drupal.settings.getlocations[key];
          var w3w = '';
          var lat = false;
          var lon = false;
          var marker = false;
          if (settings.what3words_enable) {
            if (settings.what3words_search && $("#edit-getlocations-what3words-search").is('input')) {
              $("#edit-getlocations-what3words-search-submit").click( function() {
                $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_inactive').addClass('getlocations_w3w_throbber_active');
                var s = $("#edit-getlocations-what3words-search").val();
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
                          marker.setMap();
                        }
                        settings.markdone = Drupal.getlocations.getIcon(settings.what3words_map_marker);
                        marker = Drupal.getlocations.makeMarker(Drupal.getlocations_map[key], settings, lat, lon, 0, w3w, '', '', '', key);
                      }
                      if (settings.what3words_zoom == -1) {
                        Drupal.getlocations_map[key].setZoom(parseInt(settings.nodezoom));
                      }
                      else if (settings.what3words_zoom > -1) {
                        Drupal.getlocations_map[key].setZoom(parseInt(settings.what3words_zoom));
                      }
                      //edit-getlocations-what3words-show
                      if (settings.what3words_show && $("#edit-getlocations-what3words-show").is('div') && w3w) {
                        $("#edit-getlocations-what3words-show").html(w3w);
                      }
                      if (settings.what3words_center) {
                        var c = new google.maps.LatLng(parseFloat(lat), parseFloat(lon));
                        Drupal.getlocations_map[key].setCenter(c);
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
              Drupal.getlocations_map[key].addListener('click', function(e) {
                $("#getlocations_w3w_throbber_" + key).removeClass('getlocations_w3w_throbber_inactive').addClass('getlocations_w3w_throbber_active');
                var ll = e.latLng;
                var wlat = ll.lat();
                var wlon = ll.lng();
                // go get w3w for ll
                data = {'data': parseFloat(wlat) + ',' + parseFloat(wlon)};
               $.get(settings.what3words_path, data, function(r) {
                  response = JSON.parse(r);
                  if (response) {
                    w3w = response.words.join('.');
                    if (settings.what3words_marker_show && wlat && wlon ) {
                      if (marker) {
                        // kill marker
                        marker.setMap();
                      }
                      settings.markdone = Drupal.getlocations.getIcon(settings.what3words_map_marker);
                      marker = Drupal.getlocations.makeMarker(Drupal.getlocations_map[key], settings, wlat, wlon, 0, w3w, '', '', '', key);
                    }
                    if (settings.what3words_zoom == -1) {
                      Drupal.getlocations_map[key].setZoom(parseInt(settings.nodezoom));
                    }
                    else if (settings.what3words_zoom > -1) {
                      Drupal.getlocations_map[key].setZoom(parseInt(settings.what3words_zoom));
                    }
                    if (settings.what3words_show && $("#edit-getlocations-what3words-show").is('div') && w3w) {
                      $("#edit-getlocations-what3words-show").html(w3w);
                    }
                    if (settings.what3words_center) {
                      Drupal.getlocations_map[key].setCenter(ll);
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
