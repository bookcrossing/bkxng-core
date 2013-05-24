<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page while offline.
 *
 * All the available variables are mirrored in html.tpl.php and page.tpl.php.
 * Some may be blank but they are provided for consistency.
 *
 * @see template_preprocess()
 * @see template_preprocess_maintenance_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>

</head>
<body class="<?php print $classes; ?>">
  <div id="fb-root"></div>
  <script>(function(d, s, id) {
    var js, fjs = d.getElementsByTagName(s)[0];
    if (d.getElementById(id)) return;
    js = d.createElement(s); js.id = id;
    js.src = "//connect.facebook.net/ru_RU/all.js#xfbml=1&appId=<?php print variable_get('fboauth_id', ''); ?>";
    fjs.parentNode.insertBefore(js, fjs);
  }(document, 'script', 'facebook-jssdk'));</script>
 
   <div id="page-wrapper">
    <div id="header">
      <div class="fb-like" data-href="http://www.facebook.com/bookcrossing.by" data-send="false" data-width="450" data-show-faces="true" data-font="tahoma"></div>
 
      <?php if (!empty($logo)): ?>
      <div id="logo-wrapper">
        <a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
          <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
        </a>
      </div>
      <?php endif; ?>

      <div id="name">
        <?php if (!empty($site_name)): ?>
          <h1 id="site-name">
            <a class="element-invisible" href="<?php print $base_path ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
          </h1>
        <?php endif; ?>
      </div>
    </div> <!-- /header -->

    <div id="container" class="clearfix">
      <div id="content-wrapper">
        <?php if (!empty($messages)): print $messages; endif; ?>
        <div id="content">
          <?php print $content; ?>
        </div> <!-- /content -->
      </div> <!-- /content-wrapper -->
    </div> <!-- /container -->

    <div class="twitter-wrapper">
      <p><?php print t('You may follow maintenance status on our twitter timeline'); ?></p>
      <div style="text-align:center;">
        <a class="twitter-timeline" href="https://twitter.com/search?q=from%3Abookcrossing_by+OR+to%3Abookcrossing_by" data-widget-id="335417286196346882" width="920" height="590" data-chrome="noheader noborders noscrollbar transparent"><?php echo t('Tweets around Bookcrossing Belarus'); ?></a>
        <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>
<!--
        <a class="twitter-timeline" href="https://twitter.com/bookcrossing_by" data-widget-id="332080748033085440" width="920" height="590" data-chrome="noheader noborders noscrollbar transparent">Твиты пользователя @bookcrossing_by</a>
        <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>
-->        
      </div>
    </div> <!-- /twitter-wrapper -->

<!--
    <?php
      $fb_image = theme('image', array('path' => path_to_theme() . '/images/fb.png', 'alt' => t('Bookcrossing on facebook')));
      $tw_image = theme('image', array('path' => path_to_theme() . '/images/tw.png', 'alt' => t('Bookcrossing on twitter')));

      $l = l($fb_image, variable_get('bookcrossing_facebook_link', ''), array('html' => TRUE, 'attributes' => array('target'=>'_blank')));
      $l .= l($tw_image, variable_get('bookcrossing_twitter_link', ''), array('html' => TRUE, 'attributes' => array('target'=>'_blank')));
      print '<div id="links">' . $l . '</div>';
    ?>
-->

  </div> <!-- /page -->
</body>
</html>
