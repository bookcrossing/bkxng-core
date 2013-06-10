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

if (arg ( 2 ) == 'start-mobilizer') {
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
<!--Left Navigation Start-->
<div class="admin_main_menu">
	<ul>
		<li
			class="layout <?php print $step2;?> <?php print $step2_enable_disable; ?>"><?php print l(t('Layout'),'admin/mobilizer/select-layout'); ?></li>
		<li
			class="menu <?php print $step3;?> <?php print $step3_enable_disable; ?>"><?php print l(t('Menu'),'admin/mobilizer/select-menu-configure'); ?></li>
		<li
			class="pages <?php print $step4;?> <?php print $step4_enable_disable; ?>"><?php print l(t('Pages'),'admin/mobilizer/featured-pages'); ?></li>
		<li
			class="style <?php print $step5;?> <?php print $step5_enable_disable; ?>"><?php print l(t('Style'),'admin/mobilizer/config/theme-setting-mobilizer'); ?></li>
		<li
			class="header <?php print $step6;?> <?php print $step6_enable_disable; ?>"><?php print l(t('Header'),'admin/mobilizer/config/header'); ?></li>
		<li
			class="publish <?php print $step7;?> <?php print $step7_enable_disable; ?>"><?php print l(t('Publish'),'admin/mobilizer/finish-page'); ?></li>
	</ul>
</div>



<!--Left Navigation Close-->
