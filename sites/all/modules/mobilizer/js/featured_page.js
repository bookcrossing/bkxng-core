
jQuery(document).ready(function (){
	jQuery('#close_map').hide();
	jQuery('#map_link').click(function(){
var latlon = jQuery('#map_link').attr('name');
var img_url="http://maps.googleapis.com/maps/api/staticmap?center="
+latlon+"&zoom=14&size=400x300&sensor=false";
document.getElementById("map").innerHTML="<img src='"+img_url+"'>";
jQuery('#close_map').show();
});
jQuery('#close_map').click(function(){
	jQuery('#map').html("");
	jQuery('#close_map').hide();
});
});
