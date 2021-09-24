<?php

/**
 * @file
 * Display Suite BookCrossing: Book details template.
 */
?>
<div class="bc-bookdetails <?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <?php if ($bc_bookcover): ?>
    <div class="span3">
      <?php print $bc_bookcover; ?>
    </div>
  <?php endif; ?>

  <?php if ($bc_bookdetails): ?>
    <div class="span8">
      <?php print $bc_bookdetails; ?>
    </div>
  <?php endif; ?>

</div>