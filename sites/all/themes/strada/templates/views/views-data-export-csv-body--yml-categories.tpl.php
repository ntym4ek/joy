<?php
$items = [];
// Print out exported items.
foreach ($themed_rows as $count => $item_row):
  $items[] = current($item_row);
endforeach;
print implode($separator, $items);
