jQuery(document).ready(function(jQuery){
  jQuery('#edit-feed-type').change(function(){
  var type = jQuery('#edit-feed-type').val();
  var value = '';
  if(type == 'search') {
    value = 'Search Item';
    jQuery('.form-item-channel label').html('Search Item');
  }
  if(type == 'playlist' || type == 'favorites') {
    value = 'Username';
    jQuery('.form-item-channel label').html('Username');
  
  }
});
});
