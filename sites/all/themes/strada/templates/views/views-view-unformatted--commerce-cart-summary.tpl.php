<?php

/**
 * @file
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if (!empty($title)): ?>
    <h3><?php print $title; ?></h3>
<?php endif; ?>
<div class="view-items">
    <?php $counter = 0; ?>
    <?php foreach ($rows as $id => $row): ?>
        <?php if ($counter++ == 4): ?>
        <div class="folder">
            <a class="btn btn-link" role="button" data-toggle="collapse" href="#collapseCartList" aria-expanded="false" aria-controls="collapseCartList">
                <span class="unfold">и ещё <?php print format_plural(count($rows)-4, '@count item', '@count items', [], ['context' => 'checkout order content']); ?></span>
                <span class="fold">свернуть</span>
            </a>
        </div>
        <div class="collapse" id="collapseCartList">
        <?php endif; ?>

        <div<?php if ($classes_array[$id]): ?> class="<?php print $classes_array[$id]; ?>"<?php endif; ?>>
            <?php print $row; ?>
        </div>
    <?php endforeach; ?>

    <?php if (count($rows) > 4): ?></div><?php endif; ?>
</div>
