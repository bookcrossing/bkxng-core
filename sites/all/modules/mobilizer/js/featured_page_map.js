jQuery(document).ready(function (){
	jQuery('#gmap').hide();
	jQuery('#lat').hide();
	jQuery('#lon').hide();
	jQuery('#zoom_level').hide();
	jQuery('#close_map').hide();
	jQuery('#map_link').click(function(){
		initialize();

});
function initialize(){
	//jQuery('#gmap').show();
	//jQuery('#lat').show();
	jQuery('#lon').show();
	jQuery('#zoom_level').show();
	jQuery('#close_map').show();
var myLatlng = jQuery('#map_link').attr('name');
var arr = myLatlng.split(',');
var myLatlng = new google.maps.LatLng(arr[0],arr[1]); 
var myOptions = {
zoom: 16,
center: myLatlng,
mapTypeId: google.maps.MapTypeId.ROADMAP
}
map = new google.maps.Map(document.getElementById("gmap"), myOptions);
marker = new google.maps.Marker({
position: myLatlng,
map: map
});

google.maps.event.addListener(map, 'center_changed', function() {
var location = map.getCenter();
document.getElementById("lat").innerHTML = location.lat();
document.getElementById("lon").innerHTML = location.lng();

placeMarker(location);
});

google.maps.event.addListener(map, 'zoom_changed', function() {
zoomLevel = map.getZoom();
document.getElementById("zoom_level").innerHTML = zoomLevel;
});

google.maps.event.addListener(marker, 'dblclick', function() {
zoomLevel = map.getZoom()+1;
if (zoomLevel == 20) {
zoomLevel = 10;
}
document.getElementById("zoom_level").innerHTML = zoomLevel;
map.setZoom(zoomLevel);


});
}
function placeMarker(location) {
var clickedLocation = new google.maps.LatLng(location);
marker.setPosition(location);
}
jQuery('#close_map').click(function(){
	jQuery('#gmap').hide();
	jQuery('#lat').hide();
	jQuery('#lon').hide();
	jQuery('#zoom_level').hide();
	jQuery('#close_map').hide();
});

});
