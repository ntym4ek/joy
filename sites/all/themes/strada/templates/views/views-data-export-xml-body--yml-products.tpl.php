<?php foreach ($themed_rows as $count => $row): ?>
  <offer id="<?php echo $row['product_id']; ?>" available="true">
    <url><?php echo url('node/' . $row['nid'], ['absolute' => true]) . '?id=' . $row['product_id']; ?></url>
    <price><?php echo $row['commerce_price']/100; ?></price>
    <currencyId>RUR</currencyId>
    <categoryId><?php echo explode(',', $row['field_pd_category'])[0]; ?></categoryId>
    <?php if (!empty($row['field_p_image'])): ?>
      <picture>
        <?php  echo $row['field_p_image']; ?>
      </picture>
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
      foreach(explode(',', $row['field_option_size']) as $size) {
        echo '<param name="Размер">' . trim($size) . '</param>';
      }
    } ?>
    <?php if (!empty($row['field_option_height'])) {
      foreach(explode(',', $row['field_option_height']) as $height) {
        echo '<param name="Ростовка">' . trim($height) . '</param>';
      }
    } ?>

    <delivery>true</delivery>
    <pickup>true</pickup>
    <store>false</store>
  </offer>
<?php endforeach; ?>
