<?php
/**
 * @file
 * Mobilizer theme implementation to display a single mobile device page.
 *
 * Available variables:
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/garland.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['mobilizer_content']: The content for the mobilizer content.
 * - $page['mobilizer_top']: Items for the mobilizer top.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 */
?>
<?php
global $base_url;
?>

<div id="wrapper">
  <div id="page">
    <header id="header" role="banner">
      <?php if ($logo): ?>
        <div id="logo">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
            <img src="<?php print $logo; ?>"/>
          </a>
        </div>
      <?php endif; ?>
      <?php if ($site_name): ?>
        <h1 id="site-title">
          <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
        </h1>
      <?php endif; ?>
      <div id="site-description"><?php print $site_slogan; ?></div>
      <div class="clear"></div>

    </header>
    <div class="clear"></div>
    <nav id="main-menu"  role="navigation">
      <div id="dd" class="menu-navigation-container wrapper-dropdown-3" tabindex="1">
        <span class="menu-three-drop"><img src="<?php print $base_url . '/' . $theme_path; ?>/images/menu_list_ico.png" /></span>
        <ul class="menu dropdown">
          <?php
          print $mobilizer_menu_custom_drop; //print drupal_render($main_menu_tree);
          ?>
        </ul>
      </div>
    </nav>
    <div class="clear"></div>
    <!-- end main-menu -->

    <div id="container">
      <?php if ($page['mobilizer_top']): ?>
        <div id="head">
          <?php print render($page['mobilizer_top']); ?>
        </div>
        <div class="clear"></div>
      <?php endif; ?>

      <div class="content-sidebar-wrap">

        <div id="content">
          <?php if (theme_get_setting('breadcrumbs', 'mobilizer')): ?><div id="breadcrumbs"><?php if ($breadcrumb): print $breadcrumb;
          endif; ?></div><?php endif; ?>
          <section id="post-content" role="main">
            <?php print $messages; ?>
            <?php print render($title_prefix); ?>
            <?php if ($title): ?><h1 class="page-title"><?php print $title; ?></h1><?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php print render($page['help']); ?>
            <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
            <?php if ($page['mobilizer_content']) { ?>
              <?php print render($page['mobilizer_content']); ?>
            <?php
            } else {
              print render($page['content']);
            }
            ?>

          </section> <!-- /#main -->
        </div>


      </div>

      <div class="clear"></div>

    </div>   
  </div>
  <div id="footer">
    <div id="copyright">
      <p class="copyright"><?php print t('All Rights Reserved.'); ?> &copy; <?php echo date("Y"); ?> <?php print $site_name; ?> </p>
      <p class="copyright">Powered by <?php print l(t('OSSCube'), 'http://www.osscube.com') ?></p>
      <div class="clear"></div>
    </div>
  </div>
</div>
