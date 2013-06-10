<?php
$step2_enable_disable = 'disable-link';
$step3_enable_disable = 'disable-link';
$step4_enable_disable = 'disable-link';
$step5_enable_disable = 'disable-link';
$step6_enable_disable = 'disable-link';
$step7_enable_disable = 'disable-link';

$if_layout_process = variable_get ( 'mobilizer_layout_process', 0 );
if ($if_layout_process) {
  $step2_enable_disable = '';
}
$if_menu_process = variable_get ( 'mobilizer_menu_process', 0 );
if ($if_menu_process) {
  $step3_enable_disable = '';
}
$if_page_process = variable_get ( 'mobilizer_page_process', 0 );
if ($if_page_process) {
  $step4_enable_disable = '';
}
$if_style_process = variable_get ( 'mobilizer_style_process', 0 );
if ($if_style_process) {
  $step5_enable_disable = '';
}
$if_header_process = variable_get ( 'mobilizer_header_process', 0 );
if ($if_header_process) {
  $step6_enable_disable = '';
}
$if_publish_process = variable_get ( 'mobilizer_publish_process', 0 );
if ($if_publish_process) {
  $step7_enable_disable = '';
}

if ((arg ( 2 ) == '' && arg ( 1 ) == 'mobilizer') || (arg ( 2 ) == 'start-mobilizer')) {
  $step1 = 'active';
} else {
  $step1 = '';
}
if (arg ( 2 ) == 'select-layout') {
  $step2 = 'active';
} else {
  $step2 = '';
}
if ((arg ( 2 ) == 'select-menu' || arg ( 2 ) == 'select-menu-configure' || arg ( 2 ) == 'menu') && arg ( 3 ) != 'page') {
  $step3 = 'active';
} else {
  $step3 = '';
}
if ((arg ( 2 ) == 'featured-pages') || (arg ( 2 ) == 'featured-mobilizer') || (arg ( 2 ) == 'select-menu-configure' && arg ( 3 ) == 'page')) {
  $step4 = 'active';
} else {
  $step4 = '';
}
if (arg ( 3 ) == 'theme-setting-mobilizer') {
  $step5 = 'active';
} else {
  $step5 = '';
}
if (arg ( 3 ) == 'header') {
  $step6 = 'active';
} else {
  $step6 = '';
}
if (arg ( 2 ) == 'finish-page') {
  $step7 = 'active';
} else {
  $step7 = '';
}
?>
<!--Top Header start-->
<div class="top_header">
	<!--Breadcrumb Start-->
	<div class="pagebreadcrumb">
		<ul>
			<li class="<?php print $step1;?>"><span> 1</span><?php print l(t('Start'),'admin/mobilizer/start-mobilizer',array('attributes' => array('class' => array($step1))) ); ?></li>
			<li
				class="<?php print $step2;?>  <?php print $step2_enable_disable; ?>"><span>
					2</span><?php print l(t('Layout'),'admin/mobilizer/select-layout',array('attributes' => array('class' => array($step2)))); ?></li>
			<li
				class="<?php print $step3;?>  <?php print $step3_enable_disable; ?>"><span>
					3</span><?php print l(t('Menu'),'admin/mobilizer/select-menu-configure',array('attributes' => array('class' => array($step3)))); ?></li>
			<li
				class="<?php print $step4;?>  <?php print $step4_enable_disable; ?>"><span>
					4</span><?php print l(t('Pages'),'admin/mobilizer/featured-pages',array('attributes' => array('class' => array($step4)))); ?></li>
			<li
				class="<?php print $step5;?>  <?php print $step5_enable_disable; ?>"><span>
					5</span><?php print l(t('Style'),'admin/mobilizer/config/theme-setting-mobilizer',array('attributes' => array('class' => array($step5)))); ?></li>
			<li
				class="<?php print $step6;?>  <?php print $step6_enable_disable; ?>"><span>
					6</span><?php print l(t('Header'),'admin/mobilizer/config/header',array('attributes' => array('class' => array($step6)))); ?></li>
			<li
				class="<?php print $step7;?>  <?php print $step7_enable_disable; ?>"><span>
					7</span><?php print l(t('Publish'),'admin/mobilizer/finish-page',array('attributes' => array('class' => array($step7)))); ?></li>
		</ul>
	</div>
	<!--Breadcrumb Close-->

	<!--Logo Start-->
	<!--  <div class="logo">Drupal Mobilizer</div> -->
	<!--Logo Close-->

	<!--Right Menu Start-->
	<div class="right_menu">
		<ul>
			<li class="log"><a href="#">Log</a></li>
			<li class="about"><a href="javascript:void(0);">About</a></li>
			<li class="settings"><?php print l('Configuration','admin/mobilizer/config/module-config')?></li>
		</ul>
		<div id="about_container" style="display: none;"
			class="about_container_overlay">
			<div class="about-content-close">
				<span id="about-close">Close</span>
			</div>
			<h1>It's Amazing, Quick and Easy!</h1>
			<p>OSSCube Mobilizer gives you the ability to create a mobile version
				of your existing Drupal 7 website in minutes without any complex
				coding! Using the intuitive GUI, you have the flexibility to
				customize the layout, add color scheme and prioritize content for a
				more appealing mobile experience. So start now and create a
				fantastic website for any mobile device on the market without the
				expense and time associated with custom coding.</p>
			<h2>Features</h2>
			<ul class="about-detail">
				<li>Go mobile in just minutes – with NO custom coding!</li>
				<li>Customize the way you want. No coding skills required</li>
				<li>Google Map, Click-to-Call and Social Channels integration</li>
				<li>Achieve your branding goals with one URL</li>
				<li>Compatibility with all mobile devices</li>
			</ul>
		</div>
	</div>
	<!--Right Menu Close-->

</div>
<!--Top Header Close-->
