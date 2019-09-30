<?php
/**
 * @file
 * Default theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 *
 * @ingroup templates
 */
?>
<article id="node-<?php print $node->nid; ?>" class="product full <?php print $classes; ?>"<?php print $attributes; ?>>
    <div class="row">

        <div class="col-sm-7 product-image">
            <?php print render($content['product:field_p_image']);?>
        </div>

        <div class="clearfix visible-xs-block"></div>

        <div class="col-sm-5 product-intro">
            <div class="product-title">
                <h2<?php print $title_attributes; ?>><?php print $title; ?></h2>
                <div class="product-summary">
                    <?php print str_replace('.', '', $body[0]['safe_summary']); ?>
                </div>
            </div>

            <div class="product-price">
                <?php print render($content['product:commerce_price']);?>
                <?php print render($content['product:field_p_out_of_stock']);?>
            </div>

            <?php print render($content['field_product']); ?>

            <?php if($bought): ?>
            <div class="product-bought">
                <h4>Этот товар любят!</h4>
                <div class="pb-wrapper">
                    <div class="pb-image">
                        <img src="/sites/all/themes/strada/images/bags.png" />
                    </div>
                    <div class="pb-count">
                        <?php print $bought; ?> его приобрели и уже получили
                    </div>
                </div>
            </div>
            <? endif; ?>
        </div>
    </div>

    <div class="row">
        <div class="col-xs-12">
            <div class="product-description">
                <div class="row">
                    <h2 class="col-xs-12">Характеристики и описание</h2>
                    <div class="col-sm-6">
                        <h3 class="product-purpose-title">Назначение</h3>
                        <div class="product-purpose">
                            <?php print render($content['field_pd_target']); ?>
                            <?php print render($content['body']); ?>
                            <?php print render($content['field_pd_features']); ?>
                        </div>
                    </div>

                    <div class="col-sm-6">
                        <h3 class="product-app-title">Применение</h3>
                        <div class="product-app">
                            <?php print render($content['field_pd_rates_image']); ?>
                            <?php print render($content['field_pd_rates']); ?>
                            <?php print render($content['field_pd_measures']); ?>
                            <?php print render($content['field_pd_period']); ?>
                            <?php print render($content['field_pd_expiration']); ?>
                        </div>
                    </div>
                </div>
            </div>

            <?php if(!empty($content['field_pd_info'])): ?>
            <div class="row">
                <div class="col-sm-12">
                    <h3 class="product-info-title">Полезная информация</h3>
                    <div class="product-info">
                        <?php print render($content['field_pd_info']); ?>
                    </div>
                </div>
            </div>
            <?php endif; ?>

            <?php if(!empty($content['field_pd_related'])): ?>
            <div class="row">
                <div class="col-sm-12">
                    <h3 class="product-related-title">С этим товаром покупают</h3>
                    <div class="product-related">
                        <?php print render($content['field_pd_related']); ?>
                    </div>

                </div>
            </div>
            <?php endif; ?>

        </div>
    </div>
</article>

