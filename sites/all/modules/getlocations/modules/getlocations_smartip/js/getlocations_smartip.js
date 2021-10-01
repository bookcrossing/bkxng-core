
/**
 * @file
 * getlocations_smartip.js
 * @author Bob Hutchinson http://drupal.org/user/52366
 * @copyright GNU GPL
 *
 * Javascript functions for getlocations_smartip module for Drupal 7
 */

(function ($) {
  Drupal.getlocations_smartip = {};


  Drupal.getlocations_smartip.dolocation = function(key, settings) {

    active_throbber(key);
    // remove any old markers
    $("#getlocations_smartip_lat_" + key).html('');
    $("#getlocations_smartip_lon_" + key).html('');
    // for distance views
    if ($("#edit-distance-latitude").is('input')) {
      $("#edit-distance-latitude").val('');
      $("#edit-distance-longitude").val('');
    }
    var smartip_marker = Drupal.getlocations.getIcon(settings.smartip_marker);
    var smartip_marker_title = settings.smartip_marker_title;
    var smartip_bubble = settings.smartip_bubble;
    var smartip_center = settings.smartip_center;
    var smartip_zoom = settings.smartip_zoom;
    var smartip_callback = settings.smartip_callback;
    var gs = Drupal.getlocations_settings[key];
    gs.show_maplinks = false;
    Drupal.getlocations_smartip.marker = Drupal.getlocations_smartip.marker || [];
    smartip_marker_kill();
    gs.markdone = smartip_marker;
    var result = [];
    result['lat'] = '';
    result['lon'] = '';
    smartip_in_dom(key, '', '');
    result['formatted_address'] = '';

    // fetch smartip data from server
    $.get(smartip_callback, {}, function (loc) {
      if (loc) {
        result['lat'] = loc.latitude;
        result['lon'] = loc.longitude;
        if (result['lat'] && result['lon']) {
          $("#getlocations_smartip_lat_" + key).html(result['lat']);
          $("#getlocations_smartip_lon_" + key).html(result['lon']);
          address = [];
          if (loc.city) {
            address.push(loc.city);
          }
          if (loc.region) {
            address.push(loc.region);
          }
          if (loc.zip && loc.zip != '-') {
            address.push(loc.zip);
          }
          if (loc.country) {
            address.push(loc.country);
          }
          else if (loc.country_code) {
            address.push(loc.country_code);
          }
          if (address.length) {
            result['formatted_address'] = address.join(',');
          }
          var ll = new google.maps.LatLng(parseFloat(result['lat']), parseFloat(result['lon']));
          smartip_in_dom(key, result['lat'], result['lon']);
          var customContent = '';

          if (smartip_bubble && result['formatted_address']) {
            customContent = '<div class="location vcard">';
            customContent += '<h4>' + smartip_marker_title + '</h4>';
            customContent += '<div class="adr">' + result['formatted_address'].replace(/[,]/g, ',<br />');
            customContent += '</div></div>';
            gs.useCustomContent = true;
            gs.useInfoBubble = (Drupal.settings.getlocations[key].markeraction == 2 ? true : false);
            gs.markeraction = (Drupal.settings.getlocations[key].markeraction == 2 ? 2 : 1);
          }
          else {
            gs.useCustomContent = false;
          }

          var m = Drupal.getlocations.makeMarker(Drupal.getlocations_map[key], gs, result['lat'], result['lon'], 0, smartip_marker_title, '', customContent, '', key);
          Drupal.getlocations_smartip.marker[key].push(m);
          if (smartip_center) {
            Drupal.getlocations_map[key].setCenter(ll);
          }
          if (smartip_zoom > -1) {
            Drupal.getlocations_map[key].setZoom(parseInt(smartip_zoom));
          }
          deactive_throbber(key);
        }
      }
      else {
        deactive_throbber(key);
      }
    });

    function deactive_throbber(k) {
      $("#getlocations_smartip_throbber_" + k).removeClass('getlocations_smartip_throbber_active').addClass('getlocations_smartip_throbber_inactive');
    }

    function active_throbber(k) {
      $("#getlocations_smartip_throbber_" + k).removeClass('getlocations_smartip_throbber_inactive').addClass('getlocations_smartip_throbber_active');
    }

    function smartip_in_dom(k, lat, lon) {
      if (! $("#getlocations_smartip_lat_" + k).is('div')) {
        var lladd = '<div class="js-hide" id="getlocations_smartip_lat_' + k + '"></div><div class="js-hide" id="getlocations_smartip_lon_' + k + '"></div>';
        $("#getlocations_map_wrapper_" + k).append(lladd);
      }
      if (lat && lon) {
        $("#getlocations_smartip_lat_" + k).html(lat);
        $("#getlocations_smartip_lon_" + k).html(lon);
        if ($("#edit-distance-latitude").is('input')) {
          $("#edit-distance-latitude").val(lat);
          $("#edit-distance-longitude").val(lon);
        }
        //if ($("#getlocations_search_slat_" +k).is('div')) {
        //  $("#getlocations_search_slat_" + k).html(lat);
        //  $("#getlocations_search_slon_" + k).html(lon);
        //    if ($("#getlocations_search_geocode_button_wrapper_" + k).is('div')) {
        //      $("#getlocations_search_geocode_button_" + k).show();
        //    }
        //}
      }
    }

    function smartip_marker_kill() {
      // remove any old markers
      if (Drupal.getlocations_smartip.marker[key] !== undefined ) {
        for (var mct = 0; mct < Drupal.getlocations_smartip.marker[key].length; mct++) {
          Drupal.getlocations_smartip.marker[key][mct].setMap();
        }
      }
      else {
        Drupal.getlocations_smartip.marker = [];
        Drupal.getlocations_smartip.marker[key] = [];
      }
    }
  };

  Drupal.behaviors.getlocations_smartip = {
    attach: function (context, settings) {

      // doh
      if (settings.getlocations_smartip == undefined) {
        return;
      }

      $(".getlocations_map_canvas", context).once('getlocations-smartip-map-processed', function(index, element) {
        var elemID = $(element).attr('id');
        var key = elemID.replace(/^getlocations_map_canvas_/, '');
        // is there really a map?
        if ($("#getlocations_map_canvas_" + key).is('div') && settings.getlocations_smartip[key] !== undefined ) {
          var lladd = '<div class="js-hide" id="getlocations_smartip_lat_' + key + '"></div><div class="js-hide" id="getlocations_smartip_lon_' + key + '"></div>';
          $("#getlocations_map_wrapper_" + key).append(lladd);
          // smartip button
          $("#getlocations_smartip_show_" + key).click( function() {
            Drupal.getlocations_smartip.dolocation(key, settings.getlocations_smartip[key]);
          }); // end button click
        } //  end is there really a map?
      }); // end once
    } // end attach
  }; // end behaviors

})(jQuery);
