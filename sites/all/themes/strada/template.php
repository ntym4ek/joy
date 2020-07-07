<?php

/**
 * @file
 * The primary PHP file for this theme.
 */

function strada_html_head_alter(&$head_elements)
{
    $head_elements['device_width'] = array(
        '#type' => 'html_tag',
        '#tag' => 'link',
        '#attributes' => array('rel' => 'apple-touch-icon',  'href' => '/' . drupal_get_path('theme', 'strada') . '/images/icon.png'),
    );
}

/**
 * Реализация hook_theme()
 */
function strada_theme() {
    return array(
        'review_node_form' => array(
            'template' => 'review-node-form',
            'render element' => 'form',
        ),
        'commerce_checkout_form_checkout' => array(
            'template' => 'commerce-checkout-form-checkout',
            'render element' => 'form',
        ),
        'commerce_wishlist_menu_wishlist_link' => array(
            'variables' => [],
          ),
    );
}

/**
 * переопределение theme_commerce_cart_empty_page()
 */
function strada_commerce_cart_empty_page()
{
    return '<div class="cart-empty">' .
                '<h4>Вы ещё не выбрали чем порадовать Ваши растения</h4>' .
                '<img src="/sites/all/themes/strada/images/sad.png" />' .
            '</div>';
}

/**
 * Add to wishlist theme callback.
 */
function strada_commerce_wishlist_product_add_link($variables)
{
    $user = $variables['user'];
    $product_id = $variables['product_id'];

    $url = 'user/' . $user->uid . '/wishlist/nojs/add/' . $product_id;
    $params = array(
        'attributes' => array(
            'class' => array('ajax' => 'use-ajax', 'add-to-wishlist'),
            'id' => 'add-wishlist-' . $product_id,
        ),
        'query' => array(
            'destination' => $_GET['q'],
            'token' => drupal_get_token(),
        ),
        'html' => true,
    );

    // If the current user is not logged in, build a different link that
    // points to the login page and lists all other relevant details
    // (product ID, node ID and original URL) in query string.
    if ($user->uid == 0) {
        unset($params['attributes']['class']['ajax'], $params['query']);
        $params['query']['wishlist_product'] = $product_id;
        $params['query']['destination'] = $_GET['q'];
        $url = 'user/login';
    }
    return l('<i class="far fa-heart"></i><span>' . t('To wishlist') . '</span>', $url, $params);
}

/**
 * Added to cart link theme callback.
 */
function strada_commerce_wishlist_already_in_wishlist_link(&$variables) {
    return '<a class="in-wishlist" href="' . url('user/' . $variables['user_id'] . '/wishlist') . '"><i class="fas fa-heart"></i><span>' . t('In your wish list') . '</span></a>';
}

/**
 * Added to cart link theme callback.
 */
function strada_commerce_wishlist_added_to_wishlist_link(&$variables)
{
  $url = '/user/login';
  if (!empty($variables['user_id'])) {
    $url = url('user/' . $variables['user_id'] . '/wishlist');
  }
  return '<a class="in-wishlist" href="' . $url . '"><i class="fas fa-heart"></i><span>' . t('In your wish list') . '</span></a>';
}

/**
 * Added to cart link theme callback.
 */
function strada_commerce_wishlist_menu_wishlist_link(&$variables)
{
  $url = '/user/login';
  if (!empty($GLOBALS['user']->uid)) {
    $url = url('user/' . $GLOBALS['user']->uid . '/wishlist');
  }
  return '<a id="wishlist" href="' . $url . '" class="btn btn-link" title="Отложенные товары">' . commerce_helper_wishlist_info_block() . '</a>';
}
