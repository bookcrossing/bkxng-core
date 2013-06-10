jQuery(document).ready(function (){
 //alert('---->');
 // if (window!=window.top) { 
       var path = location.href;
       var anchors = document.getElementsByTagName("a");
      for(var i = 0; i < anchors.length; i++) {
	if(window.location.search) {
	var mobilizer_set_demo_device  = Drupal.settings.mobilizer_set_demo_device;
	anchors[i].href = anchors[i].href + '/?mobilizer='+mobilizer_set_demo_device;
      }
    }
if(jQuery('.callus-class a').attr('href')){
    var href_skpe = jQuery('.callus-class a').attr('href').replace('/?mobilizer='+mobilizer_set_demo_device, " ");
    if(href_skpe)jQuery('.callus-class a').attr('href',href_skpe);	
}
if(jQuery('.copyright a').attr('href')){
    var copyright = jQuery('.copyright a').attr('href').replace('/?mobilizer='+mobilizer_set_demo_device, " ");
    if(copyright)jQuery('.copyright a').attr('href', copyright);	
}


  //}
  jQuery('.about a').click(function (){
	  jQuery('#about_container').toggle();
  });
  jQuery('#about-close').click(function (){
	  jQuery('#about_container').css('display','none');
  });
  jQuery('.share-trigger').click(function (){
	  jQuery('#share-box-container').toggle();
  });
  jQuery('#share-close').click(function (){
	  jQuery('#share-box-container').css('display','none');
  });
jQuery('[id^=feed-link]').click(function(){
      var div_id = this.id;
      var key = div_id.split('feed-link-');
      var display = jQuery('#feed-'+key[1]).css('display');
      if(display == 'none') {
	jQuery('#feed-'+key[1]).css('display','block');
      }
      if(display == 'block') {
	jQuery('#feed-'+key[1]).css('display','none');
      }
      
}); 
});
