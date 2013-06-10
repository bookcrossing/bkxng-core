<?php
$title = '';
if (arg ( 1 ) == 'mobilizer' && arg ( 2 ) == '') {
  $title = 'Start';
}
if ((arg ( 2 ) == 'start-mobilizer' || arg ( 1 ) == 'mobilizer') && arg ( 2 ) == 'start-mobilizer') {
  $title = 'Start';
}
if (arg ( 2 ) == 'select-layout') {
  $title = 'Layout';
}
if ((arg ( 2 ) == 'select-menu' || arg ( 2 ) == 'select-menu-configure' || arg ( 2 ) == 'menu') && arg ( 3 ) != 'page') {
  $title = 'Menu';
}
if (arg ( 2 ) == 'select-menu-configure' && arg ( 3 ) == 'page') {
  $title = 'Pages';
}
if (arg ( 2 ) == 'featured-pages') {
  $title = 'Pages';
}
if (arg ( 3 ) == 'theme-setting-mobilizer') {
  $title = 'Style';
}
if (arg ( 3 ) == 'header') {
  $title = 'Header';
}
if (arg ( 2 ) == 'finish-page') {
  $title = 'Publish';
}
?>
<div class="outer_container clearfix">
    <?php print $top_nav; ?>
    <!--Body Container Start-->
	<div class="body_container clearfix">
        <?php print $left_nav; ?>
        <div id="mobilizer_messages"><?php print mobilizer_get_messages();?></div>
		<!--Main content Area Start-->
		<div class="main_content_area">
			<!--content heading start-->
			<div class="content_area_heading">
				<h3><?php
    if ($title) {
      print $title;
    } else {
      print drupal_get_title ();
    }
    ?>
                </h3>
				<!--Logo Start-->
				<div class="logo">Drupal Mobilizer</div>
				<!--Logo Close-->
			</div>
			<!--content heading close-->
			<!--content Area-->
			<div class="content_area_inside">
                <?php print $form_output; ?>
            </div>
			<!--content Area Close-->
		</div>
		<!--Main content Area Close-->
		<!--Mobile Container Start-->
        <?php if (!empty($right_nav)) { ?>
            <div class="mobile_phone_container">
			<!-- *****************View website in this box***********-->
			<div class="mobile_site_view"><?php print $right_nav; ?></div>
			<!-- *****************View website in this box Close***********-->
		</div>
        <?php } ?>
        <!--Mobile Container Close-->
	</div>
	<!--Body Container Close-->
</div>
