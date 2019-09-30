<div class="r-wrapper">
    <div class="r-info">
        <div class="r-image">
            <?php print render($form['product_image']); ?>
        </div>
        <div class="r-intro">
            <p>Расскажите о своём опыте использования товара — это поможет другим покупателям определиться с выбором. Обратите внимание на качество, удобство, соответствие заявленным характеристикам.</p>
            <p>Мы не публикуем отзывы, которые написаны большими буквами или содержат ненормативную лексику и оскорбления.</p>
        </div>
    </div>
    <div class="r-form">
        <?php print render($form['field_pr_mark']); ?>
        <?php print render($form['field_pr_recommend']); ?>
        <?php print render($form['field_pr_merits']); ?>
        <?php print render($form['field_pr_disadvantages']); ?>
        <?php print render($form['field_pr_comment']); ?>
        <?php print render($form['actions']); ?>

        <?php hide($form['product_info']); ?>
    </div>
</div>
<?php echo drupal_render_children($form); ?>