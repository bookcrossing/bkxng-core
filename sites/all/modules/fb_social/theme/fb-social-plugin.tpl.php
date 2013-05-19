<?php
/**
 * @file fb-social-plugin.tpl.php
 * Theme the more link
 *
 * - $plugin_type: the type of this plugin
 * - $tag_name : the tagname of this plugin
 */
?>
<div class="fb-social-<?php print $plugin_type?>-plugin">
  <<?php print $tag_name ?> <?php print drupal_attributes($options)?>></<?php print $tag_name?>>
</div>

