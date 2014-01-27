<div class="span3 text-center <?php print $classes; ?> clearfix <?php print $bc_content_classes; ?>">



    <?php if (isset($title_suffix['contextual_links'])): ?>

        <?php print render($title_suffix['contextual_links']); ?>

    <?php endif; ?>



    <?php if ($bc_content): ?>

        <?php print $bc_content; ?>

    <?php endif; ?>



</div>



<?php if (!empty($drupal_render_children)): ?>

    <?php print $drupal_render_children ?>

<?php endif; ?>