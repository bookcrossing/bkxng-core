<?php

/**
 * @file page.tpl.php
 * Default theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>

<div id="page-wrap">

    <section class="container preheader">
        <div class="row-fluid">
            <div class="span4">
                <!--this is the login for the user-->
                <?php if ($logged_in) : ?>
                    <nav class="user clearfix"><i
                            class="icon-user"></i> <?php print l($user->name, 'user/' . $user->uid); ?> <?php print l(t('(log out)'), 'user/logout', array('attributes' => array('class' => array('logout-link')))); ?>
                    </nav>
                <?php else : ?>
                    <nav class="user clearfix">
                        <i class="icon-user"></i> <a href="/login" role="button" rel="nofollow"><?php print t('Log in'); ?></a>
                    </nav>
                <?php endif; ?>
                <!--close user nav-->
            </div>
            <div class="span4 offset4 pull-right visible-desktop">
                <?php print render($page['social_buttons']); ?>
<!-- 
                <ul class="social hidden-phone">
                    <li id="fb-wrapper">
                        <div class="fb-like" data-href="http://www.facebook.com/bookcrossing.by" data-send="false"
                             data-layout="button_count" data-width="450" data-show-faces="false" data-font="tahoma"></div>
                    </li>
                    <li id="github-wrapper">
                        <iframe src="/github-btn.html?user=bookcrossing&repo=core&type=fork&count=true" allowtransparency="true"
                                frameborder="0" scrolling="0" width="130" height="20"></iframe>
                    </li>
                </ul>
 -->
            </div>
        </div>
    </section>

    <!-- begin .header-->
    <header class="header clearfix">

        <div class="navbar navbar-inverse">
            <div class="navbar-inner">
                <div class="container">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-responsive-collapse">
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </a>
                    <?php if ($logo || $site_name): ?>
                        <a class="brand" href="<?php print $front_page; ?>" title="<?php print t('BookCrossing'); ?>" rel="home" id="logo">
                            <?php if ($logo): ?>
                                <div class="logo">
                                    <div class="logo-title"><?php print t('BookCrossing'); ?></div><span class="logo-country"><?php print t('Belarus'); ?></span>
                                </div>
                            <?php else: ?>
                                <?php print $site_name; ?>
                            <?php endif; ?>
                        </a>
                    <?php endif; ?>

                    <div class="nav-collapse collapse navbar-responsive-collapse">

                        <!-- begin #main_menu -->
                        <?php print theme('bookcrossing_main_menu', array('visible' => theme_get_setting('visible_menu_items'), 'links' => $main_menu, 'attributes' => array('id' => 'main-menu', 'class' => array('links', 'clearfix')), 'heading' => '')); ?>
                        <!-- close #main_menu -->

                        <ul class="nav pull-right">
                            <?php if ($logged_in) : ?>
                                <li><a href="<?php print $base_path; ?>release-book"><span
                                            class="btn btn-success btn-large"><?php print t('Release book'); ?></span></a>
                                </li>
                            <?php else: ?>
                                <li>
                                    <a href="/login" role="button" rel="nofollow"><span class="btn btn-success btn-large"><?php print t('Release book'); ?></span></a>
                                </li>
                            <?php endif; ?>
                            <li class="divider-vertical"></li>
                            <li class="dropdown">
                                <a href="#" class="dropdown-toggle" data-toggle="dropdown"><?php print t('Help'); ?> <b
                                        class="caret"></b></a>
                                <ul class="dropdown-menu">
                                    <li><?php print l(t('Site help'), variable_get('bookcrossing_help_link', '')); ?></li>
                                    <li><?php print l(t('Frequently asked questions'), variable_get('bookcrossing_faq_link', '')); ?></li>
                                    <li class="divider"></li>
                                    <li><?php print l(t('Feedback'), 'http://meta.bookcrossing.by/'); ?></li>
                                </ul>
                            </li>
                        </ul>

                    </div>
                    <!-- close .nav-collapse -->
                </div>
            </div>
            <!-- close .navbar-inner -->
        </div>
        <!-- close .navbar -->

        <div class="container clearfix">
            <div class="row">
                <div class="span4">
                    <?php print render($page['header_sidebar_first']); ?>
                </div>
                <div class="span8">
                    <?php print render($page['header_sidebar_second']); ?>
                </div>
            </div>
        </div>

    </header>
    <!-- close .header -->

    <!-- begin page - the container for everything but header -->
    <div class="container clearfix">
        <?php print $messages; ?>
    </div>

    <div class="container clearfix">
        <div class="row">
            <div class="span12">
                <?php print render($page['before_content']); ?>
            </div>
        </div>

        <div class="row">
            <?php $content_span = 12;
            if ($page['sidebar_first']) $content_span -= 2;
            if ($page['sidebar_second']) $content_span -= 2;
            ?>
            <?php if ($page['sidebar_first']): ?>
                <div class="span2">
                    <?php print render($page['sidebar_first']); ?>
                </div>
            <?php endif; ?>

            <div class="span<?php print $content_span; ?>">
                <?php print render($page['content']); ?>
            </div>
            <?php if ($page['sidebar_second']): ?>
                <div class="span2">
                    <?php print render($page['sidebar_second']); ?>
                </div>
            <?php endif; ?>
        </div>

        <div class="row">
            <div class="span12">
                <?php print render($page['after_content']); ?>
            </div>
        </div>

    </div>
    <!-- close page-->

</div><!-- close #page-wrap-->

<footer id="footer-wrap">
    <div class="container clearfix">
        <div class="row">
            <div class="span10 offset2">
                <?php print render($page['footer_menu']); ?>
            </div>
        </div>
        <div class="row">
            <div class="span12">
                <div class="credit muted">
                    <?php print render($page['footer_text']); ?>
                </div>
            </div>
        </div>
    </div>
</footer>

<!-- Back to top -->
<div id="back-top" style="display:none;">
  <script type="text/javascript">
		/*<![CDATA[*/
    var scrollPosition = 0;
    (function ($) {
			$(document).ready(function(){
				$("#back-top").hide();
				$(function () {
					$(window).scroll(function () {
                        var scrollCurrent = $(this).scrollTop();
						if (scrollCurrent > 100 && scrollPosition > scrollCurrent) {
							$('#back-top').fadeIn();
						} else {
                            $('#back-top').fadeOut();
						}
                        scrollPosition = $(this).scrollTop();
					});
					$('#back-top a').click(function () {
						$('body,html').animate({
							scrollTop: 0
						}, 300);
						return false;
					});
				});
			});
    })(jQuery);
		/*]]>*/
		</script>
  <a href="#"><?php print t('back to top'); ?></a>
</div>
<!-- /Back to top -->

<!-- Modal login -->
<div id="loginModal" class="modal hide fade text-center" tabindex="-1" role="dialog" aria-labelledby="loginModalLabel" aria-hidden="true">
  <div class="modal-header">
    <img style="float:left; height:32px; width: 32px;" src="<?php print '/'.$directory; ?>/logo_sqr.png" />
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
    <h3 id="loginModalLabel"><?php print t('Log in using social profile'); ?></h3>
  </div>
  <div class="modal-body">
    <p><?php print t('To log in you need to create a profile or use your existing profile.'); ?></p>
    <p>
<!-- 
      <a href="<?php print $base_path; ?>hybridauth/window/Facebook?destination=books&destination_error=books-front"><span class="btn btn-social facebook btn-large"><i class="icon-facebook"></i><span><?php print t('Facebook'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Vkontakte?destination=books&destination_error=books-front"><span class="btn btn-social vkontakte btn-large"><i class="icon-vk"></i><span><?php print t('Vkontakte'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Disqus?destination=books&destination_error=books-front"><span class="btn btn-social disqus btn-large"><i class="icon-disqus"></i><span><?php print t('Disqus'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Goodreads?destination=books&destination_error=books-front"><span class="btn btn-social goodreads btn-large"><i class="icon-goodreads"></i><span><?php print t('Goodreads'); ?></span></span></a>
 -->
      <a href="<?php print $base_path; ?>hybridauth/window/Vkontakte?destination=books&destination_error=books-front"><span class="btn btn-social vkontakte btn-large"><i class="icon-vk"></i><span><?php print t('Vkontakte'); ?></span></span></a>
    </p>
  </div>
  <div class="modal-footer">
    <button class="btn" data-dismiss="modal" aria-hidden="true"><?php print t('Cancel'); ?></button>
  </div>
</div>    
<!-- /Modal login -->

<!-- Modal release book -->
<div id="releaseModal" class="modal hide fade text-center" tabindex="-1" role="dialog" aria-labelledby="releaseModalLabel" aria-hidden="true">
  <div class="modal-header">
    <img style="float:left; height:32px; width: 32px;" src="<?php print '/'.$directory; ?>/logo_sqr.png" />
    <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
    <h3 id="releaseModalLabel"><?php print t('Log into profile to release a book'); ?></h3>
  </div>
  <div class="modal-body">
    <p><?php print t('To release a book you need to create a profile or log into your existing profile.'); ?></p>
    <p>
      <a href="<?php print $base_path; ?>hybridauth/window/Facebook?destination=release-book&destination_error=books-front"><span class="btn btn-social facebook btn-large"><i class="icon-facebook"></i><span><?php print t('Facebook'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Vkontakte?destination=release-book&destination_error=books-front"><span class="btn btn-social vkontakte btn-large"><i class="icon-vk"></i><span><?php print t('Vkontakte'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Disqus?destination=release-book&destination_error=books-front"><span class="btn btn-social disqus btn-large"><i class="icon-disqus"></i><span><?php print t('Disqus'); ?></span></span></a>
      <a href="<?php print $base_path; ?>hybridauth/window/Goodreads?destination=release-book&destination_error=books-front"><span class="btn btn-social goodreads btn-large"><i class="icon-goodreads"></i><span><?php print t('Goodreads'); ?></span></span></a>
    </p>
  </div>
  <div class="modal-footer">
    <button class="btn" data-dismiss="modal" aria-hidden="true"><?php print t('Cancel'); ?></button>
  </div>
</div>    
<!-- /Modal release book -->

<?php if ($page['banners_footer']): ?>
    <section id="banners-wrapper" class="container clearfix">
        <div class="row">
            <?php print render($page['banners_footer']); ?>
        </div>
    </section>
<?php endif; ?>

<?php if ($page['debug']): ?>
    <section id="debug" class="hidden-desktop hidden-tablet hidden-phone">
        <?php print render($page['debug']); ?>
    </section>
<?php endif; ?>

<?php if ($page['scripts']): ?>
    <section id="scripts" class="hidden-desktop hidden-tablet hidden-phone">
        <?php print render($page['scripts']); ?>
    </section>
<?php endif; ?>

