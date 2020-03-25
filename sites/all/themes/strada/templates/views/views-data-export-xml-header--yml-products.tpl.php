<?php print '<?xml version="1.0" encoding="UTF-8" ?>'; ?>

<!DOCTYPE yml_catalog SYSTEM "shops.dtd">
<yml_catalog date="<?php echo date('Y-m-d h:i'); ?>">
  <shop>
    <name>JOY-MAGAZIN - <?php echo variable_get('site_name', 'Drupal'); ?></name>
    <company>ООО "СТРАДА"</company>
    <url><?php echo $GLOBALS['base_url']; ?></url>
    <currencies>
      <currency id="RUR" rate="1"/></currencies>

    <?php print views_embed_view('yml_categories', 'views_data_export_1'); ?>

    <offers>
