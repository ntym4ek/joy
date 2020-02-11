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
<div class="view-items products-list">
    <?php
    $cnt = count($rows);
    foreach ($rows as $id => $row) {
        if (($cnt > 5 && $id == 5)) {
            print '<div class="collapse-link"><a class="btn btn-link" role="button" data-toggle="collapse" href="#collapseItems" aria-expanded="false" aria-controls="collapseItems">' .
                      'и ещё ' . format_plural($cnt-5, '@count item', '@count items', [],  ['context' => 'commerce order']) .
                   '</a></div><div class="collapse" id="collapseItems">';
        }
        print '<div class="' . $classes_array[$id] . '">' . $row . '</div>';
    }
    if ($cnt > 5) print '</div>';
    ?>
</div>