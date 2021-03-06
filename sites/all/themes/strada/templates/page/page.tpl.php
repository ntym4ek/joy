<?php

/**
 * @file
 * Default theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template in this directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/bartik.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['highlighted']: Items for the highlighted content region.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar.
 * - $page['sidebar_second']: Items for the second sidebar.
 * - $page['header']: Items for the header region.
 * - $page['footer']: Items for the footer region.
 *
 * @see bootstrap_preprocess_page()
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see bootstrap_process_page()
 * @see template_process()
 * @see html.tpl.php
 *
 * @ingroup templates
 */
?>
<header id="navbar" role="banner" class="<?php print $navbar_classes; ?>">

    <div class="line-1 row">
        <?php if (empty($tsss)): ?>
        <div class="<?php print $container_class; ?>">
            <div class="topbar">
                <div class="topbar-region hidden-xs"><i class="far fa-location-arrow"></i> <a href="#" id="user_region" class="select-region"></a></div>

<!--                <div class="topbar-phone hidden-xs">-->
<!--                    Есть вопросы? Пишите WhatsApp:&nbsp;<a href="https://wa.me/--><?// print ext_user_normalize_phone(variable_get('manager_whatsapp', '8-922-910-99-40')); ?><!--" tabindex="0" role="button" target="_blank">--><?// print variable_get('manager_whatsapp', '8-922-910-99-40'); ?><!--</a>-->
<!--                </div>-->

<!--                <div class="topbar-shipping"><a id="free_shipping" href="/info/delivery">Доставка</a></div>-->
                <div class="topbar-menu hidden-xs hidden-sm">
                    <ul class="dry-list">
                        <li><a href="/info/delivery">Доставка</a></li>
                        <li><a href="/info/payment">Оплата</a></li>
                        <li><a href="/info/return">Возврат</a></li>
                        <li><a href="/info/how-to-buy">Как купить</a></li>
                        <li><a href="/info/contacts">Контакты</a></li>
                    </ul>
                </div>
            </div>
        </div>
        <?php endif; ?>
    </div>

    <div class="line-2">
        <div class="<?php print $container_class; ?>">

            <div class="navbar-header">
                <div class="navbar-logo">
                    <a class="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                      <img class="logo-img" src="/sites/all/themes/strada/logo-img.png" alt="<?php print $site_name; ?>" />
                      <img class="logo-words" src="/sites/all/themes/strada/logo_words2.svg" alt="<?php print $site_name; ?>" />
                    </a>
                </div>

                <?php if (empty($tsss)): ?>
                <div class="navbar-menu hidden-sm hidden-md hidden-lg" role="navigation">
                    <?php print $main_menu_mobile_html; ?>
                </div>
                <?php endif; ?>

<!--              <div class="navbar-phone">-->
<!--                <a href="tel:88005502885" tabindex="0" role="button" class="btn btn-brand">8-800-550-28-85<br /><div>круглосуточно</div></a>-->
<!--              </div>-->

              <?php if (empty($tsss)): ?>
                <div class="navbar-search">
                        <div class="navbar-search-mobile"><a class="btn btn-link"><i class="far fa-search"></i></a></div>
                        <div class="navbar-search-pane">
                            <div class="navbar-search-pane-close"><a class="btn btn-link"><i class="fal fa-times"></i></a></div>
                        <?php
                            $search_page = entity_load_single('search_api_page', 1);
                            $search_form = drupal_get_form('search_api_page_search_form', $search_page);
                            print drupal_render($search_form);
                        ?>
                        </div>
                </div>
                <?php endif; ?>

                <?php if (empty($tsss)): ?>
                <div class="navbar-user" role="navigation">
                    <?php print render($user_menu); ?>
                </div>
                <?php endif; ?>

                <?php if (empty($tsss)): ?>
                <div class="navbar-wishlist" role="navigation">
                    <?php print theme('commerce_wishlist_menu_wishlist_link', []); ?>
                </div>
                <?php endif; ?>

                <?php if (empty($tsss)): ?>
                <div id="cart" class="navbar-cart" role="navigation">
                    <?php print ext_order_cart_info_block(); ?>
                </div>
                <?php endif; ?>

            </div>
        </div>
    </div>

    <div class="line-2-1 hidden-sm hidden-md hidden-lg">
        <div class="topbar-phone">
            <div><a href="tel:88005502885">8-800-550-28-85</a></div>
            <div>WhatsApp:&nbsp;<a href="https://wa.me/<? print str_replace('-', '', variable_get('manager_whatsapp', '8-922-910-99-40')); ?>" tabindex="0" role="button" target="_blank"><? print variable_get('manager_whatsapp', '8-922-910-99-40'); ?></a></div>
        </div>
    </div>

    <?php if (empty($tsss)): ?>
    <div class="line-3 row hidden-xs">
        <div class="<?php print $container_class; ?>">
          <div class="menu-horizont-wrapper">
            <div class="navbar-menu" role="navigation">
              <?php print $main_menu_mobile_html; ?>
            </div>
<!--          --><?php //print render($main_menu); ?>
            <div class="menu-horizont">
              <ul class="">
                <li><a href="/catalog/udobreniya">Удобрения</a></li>
                <li><a href="/catalog/dom">Для дома</a></li>
                <li><a href="/catalog/dacha">Для дачи</a></li>
                <li><a href="/catalog/banya">Баня</a></li>
                <li><a href="/catalog/otdyh-i-sport">Отдых и спорт</a></li>
                <li><a href="/catalog/bytovaya-himiya">Бытовая химия</a></li>
                <li><a href="/catalog/ot-pozhara">От пожара</a></li>
                <li><a href="/catalog/avtohimiya">Автохимия</a></li>
                <li><a href="/catalog/hobbi">Хобби</a></li>
                <li><a href="/catalog/zootovary">Зоотовары</a></li>
              </ul>
            </div>
          </div>
        </div>
    </div>
    <?php endif; ?>

</header>

<div class="main-container <?php print $container_class; ?>">

    <header role="banner" id="page-header">
        <?php print render($page['header']); ?>
    </header> <!-- /#page-header -->

    <?php if (!empty($page['highlighted'])): ?>
        <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
    <?php endif; ?>

    <div class="row">

        <?php if (!empty($breadcrumb) || !empty($title) || !empty($tabs['#primary']) || !empty($tabs['#secondary']) || !empty($page['help']) || !empty($action_links)): ?>
        <section class="col-sm-12 content-header">
            <?php if (!empty($breadcrumb) && (empty($tsss))): print $breadcrumb; endif;?>

            <a id="main-content"></a>

            <?php if (!empty($title) || !empty($page['navigation'])): ?>
            <div class="page-header">
                <?php print render($title_prefix); ?>
                <h1><?php if (!empty($title)) print $title; ?></h1>
                <?php print render($title_suffix); ?>

                <?php if (!empty($page['navigation'])) { print render($page['navigation']); } ?>
            </div>
            <?php endif; ?>

            <?php print $messages; ?>

            <?php if (!empty($tabs)): ?>
                <?php print render($tabs); ?>
            <?php endif; ?>

            <?php if (!empty($page['help'])): ?>
                <?php print render($page['help']); ?>
            <?php endif; ?>
        </section>
        <?php endif; ?>

        <?php if (!empty($page['sidebar_first'])): ?>
            <aside class="col-sm-3" role="complementary">
                <?php print render($page['sidebar_first']); ?>
            </aside>  <!-- /#sidebar-first -->
        <?php endif; ?>

        <section class="<?php print implode(' ', $content_column_class); ?>">
            <?php if (!empty($content_title)) print '<div class="content-title"><h1>' . $content_title . '</h1></div>'; ?>
            <?php print render($page['content']); ?>
        </section>

        <?php if (!empty($page['sidebar_second']) || !empty($action_links)): ?>
            <aside class="col-sm-3" role="complementary">
                <?php if (!empty($action_links)): ?>
                    <?php print render($action_links); ?>
                <?php endif; ?>

                <?php print render($page['sidebar_second']); ?>
            </aside>  <!-- /#sidebar-second -->
        <?php endif; ?>

    </div>
</div>

<footer class="footer">
    <div class="f-main <?php print $container_class; ?>">
        <div class="f-logo">
            <a class="logo" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                <img class="img-responsive" src="/sites/all/themes/strada/logo3.png" alt="<?php print t('Home'); ?>" />
            </a>
            <div class="f-info">
                <div class="f-company"><? print date("Y"); ?> © joy-magazin.ru — интернет-магазин товаров для сада и дома. Все права защищены. Доставка по всей России!</div>
                <div class="f-terms"><a href="/info/rules">Пользовательское соглашение</a></div>
              <?php if ($is_front): ?>
                <h1 class="f-slogan"><?php print $site_slogan; ?></h1>
              <?php endif; ?>
            </div>
        </div>
        <div class="f-menu">
            <h5>Меню</h5>
            <ul class="dry-list">
                <li><a href="/info/contacts">Контакты</a></li>
                <li><a href="<?php print $GLOBALS['user']->uid ? '/user/' . $GLOBALS['user']->uid . '/orders' : '/user/login'; ?>">Мои заказы</a></li>
                <li><a href="/info/about">О компании</a></li>
                <li><a href="/info/resellers">Оптовым покупателям</a></li>
                <li><a href="/info/suppliers">Поставщкам</a></li>
                <li><a href="/info/how-to-buy">Помощь</a></li>
            </ul>
        </div>
        <div class="f-how-to">
            <h5>Помощь</h5>
            <ul class="dry-list">
                <li><a href="/info/how-to-buy">Как купить</a></li>
                <li><a href="/info/delivery">Доставка</a></li>
                <li><a href="/info/payment">Оплата</a></li>
                <li><a href="/info/promo">Акции</a></li>
            </ul>
        </div>
        <div class="f-social">
            <h5>Оцените<br />нас</h5>
            <div class="social-links">
                <a class="vk" href="https://vk.com/joy_garden" rel="nofollow noopener" target="_blank" title="ВКонтакте"><i class="fab fa-vk" aria-hidden="true"></i></a>
                <a class="fb" href="https://www.facebook.com/joy.garden.43/" rel="nofollow noopener" target="_blank" title="Facebook"><i class="fab fa-facebook" aria-hidden="true"></i></a>
                <a class="in" href="https://www.instagram.com/joy_info" rel="nofollow noopener" target="_blank" title="Instagram"><i class="fab fa-instagram" aria-hidden="true"></i></a>
            </div>
            <div>Горячая линия Интернет-магазина</div>
            <div class="support-phone"><a href="tel:88005502885">8-800-550-28-85</a></div>
        </div>
    </div>
    <div class="f-scripts">
        <?php print render($page['footer']); ?>
    </div>
</footer>

<div id="modalBackdrop"></div>
