<?php

/**
 * @file
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * Use render($user_profile) to print all profile items, or print a subset
 * such as render($user_profile['user_picture']). Always call
 * render($user_profile) at the end in order to print all remaining items. If
 * the item is a category, it will contain all its profile items. By default,
 * $user_profile['summary'] is provided, which contains data on the user's
 * history. Other data can be included by modules. $user_profile['user_picture']
 * is available for showing the account picture.
 *
 * Available variables:
 *   - $user_profile: An array of profile items. Use render() to print them.
 *   - Field variables: for each field instance attached to the user a
 *     corresponding variable is defined; e.g., $account->field_example has a
 *     variable $field_example defined. When needing to access a field's raw
 *     values, developers/themers are strongly encouraged to use these
 *     variables. Otherwise they will have to explicitly specify the desired
 *     field language, e.g. $account->field_example['en'], thus overriding any
 *     language negotiation rule that was previously applied.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 *
 * @ingroup themeable
 */

hide($user_profile['field_phone']);
hide($user_profile['field_surname']);
hide($user_profile['field_name_first']);
hide($user_profile['field_name_second']);
hide($user_profile['userpoints']);
hide($user_profile['wishlist']);
hide($user_profile['orders']);
?>

<div class="profile"<?php print $attributes; ?>>
  <div class="row">
    <div class="col-md-6">
      <div class="user-info-block media">
        <div class="media-left">
          <? print render($user_profile["user_picture"]); ?>
        </div>
        <div class="media-body">
          <h4><? print $elements["#account"]->realname; ?></h4>
          <? print ($elements["#account"]->mail ? '<div><span class="text-muted">E-Mail:</span> ' . $elements["#account"]->mail . '</div>' : ''); ?>
          <? print empty($elements["#account"]->field_phone["und"][0]["safe_value"]) ? '' : '<div><span class="text-muted">Телефон:</span> ' . $elements["#account"]->field_phone["und"][0]["safe_value"] . '</div>'; ?>
          <? if ($logged_in): ?>
          <ul class="links">
            <li><a href="/user/<? print $elements["#account"]->uid;  ?>/edit">Редактировать</a></li>
          </ul>
          <? endif; ?>
        </div>
      </div>
    </div>

    <div class="col-md-6">
      <div class="user-info-block media points">
        <div class="media-left media-bottom">
          <i class="fas fa-coins"></i>
        </div>
        <div class="media-body">
          <h4><? print render($elements["userpoints"]["title"]); ?></h4>
          <div class="counter"><? print render($elements["userpoints"]["list"]); ?></div>
          <? print render($elements["userpoints"]["actions"]); ?>
        </div>
      </div>
    </div>
  </div>

  <div class="row">
    <div class="col-md-6">
      <div class="user-info-block media orders">
        <div class="media-left media-bottom">
          <i class="fas fa-bags-shopping"></i>
        </div>
        <div class="media-body">
          <h4><? print render($elements["orders"]["title"]); ?></h4>
          <div class="counter"><? print render($elements["orders"]["counter"]); ?></div>
          <? print render($elements["orders"]["actions"]); ?>
        </div>
      </div>
    </div>

    <div class="col-md-6">
      <div class="user-info-block media wishlist">
        <div class="media-left media-bottom">
          <i class="fas fa-heart"></i>
        </div>
        <div class="media-body">
          <h4><? print render($elements["wishlist"]["title"]); ?></h4>
          <div class="counter"><? print render($elements["wishlist"]["counter"]); ?></div>
          <? print render($elements["wishlist"]["actions"]); ?>
        </div>
      </div>
    </div>

  </div>

  <?php print render($user_profile); ?>
</div>
