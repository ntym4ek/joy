<?php
  $categories = trim(strip_tags(views_embed_view('yml_categories', 'views_data_export_2')));
  $categories = explode(',', $categories);
//  $categories = array_flip($categories)
?>
<?php foreach ($themed_rows as $count => $row):
  if ($inter = array_intersect(explode(',', str_replace(' ', '', $row['field_pd_category'])), $categories)) {
  ?>
  <offer id="<?php echo $row['product_id']; ?>" available="true">
    <url><?php echo url('node/' . $row['nid'], ['absolute' => TRUE]) . '?id=' . $row['product_id']; ?></url>
    <price><?php echo $row['commerce_price'] / 100; ?></price>
    <currencyId>RUR</currencyId>
    <categoryId><?php echo array_shift($inter); ?></categoryId>
    <?php if (!empty($row['field_p_image'])): ?>
      <picture><?php echo $row['field_p_image']; ?></picture>
    <?php endif; ?>
    <name><?php echo $row['title_1']; ?></name>
    <description><?php echo htmlspecialchars($row['body']); ?></description>
    <?php if (!empty($row['field_p_tare'])) {
      echo '<param name="Упаковка">' . $row['field_p_tare'] . '</param>';
    } ?>
    <?php if (!empty($row['field_attribute_color'])) {
      echo '<param name="Цвет">' . $row['field_attribute_color'] . '</param>';
    } ?>
    <?php if (!empty($row['field_option_size'])) {
      foreach (explode(',', $row['field_option_size']) as $size) {
        echo '<param name="Размер">' . trim($size) . '</param>';
      }
    } ?>
    <?php if (!empty($row['field_option_height'])) {
      foreach (explode(',', $row['field_option_height']) as $height) {
        echo '<param name="Ростовка">' . trim($height) . '</param>';
      }
    } ?>
  </offer>
  <?php
  }
endforeach; ?>
