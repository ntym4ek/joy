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
<?php foreach ($rows as $id => $row): ?>
    <div<?php if ($classes_array[$id]): ?> class="<?php print $classes_array[$id]; ?>"<?php endif; ?>>
        <?php print $row; ?>
    </div>
    <?php
        // разделитель
        $class = ['visible-xs'];
        if (($id+1) % 2 == 0) $class[] = 'visible-sm';
        if (($id+1) % 3 == 0) $class[] = 'visible-md';
        if (($id+1) % 4 == 0) $class[] = 'visible-lg';
//        print '<div class="divider ' . implode(' ', $class) . '"></div>';
    ?>
<?php endforeach; ?>