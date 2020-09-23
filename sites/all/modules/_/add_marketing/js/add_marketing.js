(function ($) {
  Drupal.behaviors.add_marketing = {
    attach: function (context, settings) {

      // -------------------------------- UTM ----------------------------------
      // проверить наличие UTM в ссылке
      var url = new URL(location.href);
      if (url.searchParams.has('utm_source') && url.searchParams.has('utm_medium') && url.searchParams.has('utm_campaign')) {
        var user_marketing = JSON.parse($.cookie('user_marketing'));
        if (!user_marketing) {
          user_marketing = {};
        }
        var key = url.searchParams.get('utm_source') + '_' + url.searchParams.get('utm_medium') + '_' + url.searchParams.get('utm_campaign');
        if (user_marketing.utm === undefined) {
          user_marketing.utm = {};
        }
        // сохранить при отсутствии в маркетинговой куке
        if (user_marketing.utm[key] === undefined) {
          user_marketing.utm[key] = {
            timestamp:    Date.now(),
            utm_source:   url.searchParams.get('utm_source'),
            utm_medium:   url.searchParams.get('utm_medium'),
            utm_campaign: url.searchParams.get('utm_campaign'),
          };
          if (url.searchParams.has('utm_term')) user_marketing.utm[key]['utm_term'] = url.searchParams.get('utm_term');
          if (url.searchParams.has('utm_content')) user_marketing.utm[key]['utm_content'] = url.searchParams.get('utm_content');
          if (url.searchParams.has('utm_position')) user_marketing.utm[key]['utm_position'] = url.searchParams.get('utm_position');
          if (url.searchParams.has('utm_group')) user_marketing.utm[key]['utm_group'] = url.searchParams.get('utm_group');
          if (url.searchParams.has('utm_device')) user_marketing.utm[key]['utm_device'] = url.searchParams.get('utm_device');

          $.cookie('user_marketing', JSON.stringify(user_marketing), {path: "/"});
        }
      }

      // -------------------------------- Events Fire -----------------------
      $(document).scroll(function(){
        onScrollEvent();
      });

      $(window).resize(function(){
        onResizeEvent();
      });

      // отследить событие авторизации и регистрации --
      $(".user-form-container .form-submit").click(function() {
        // отследить нажатие сабмита
        localStorage.login = $(".form-item-name input").val();
      });
      if (localStorage.login != undefined && Drupal.settings.user.uid) {
        // если с момента создания пользователя до открытия страницы не более 5 минут,
        // то он новорег
        if (Date.now() - Drupal.settings.user.created*1000 < 300000) {
          onUserRegistration(localStorage.login);
        } else {
          onUserAuthorization(localStorage.login);
        }
        localStorage.removeItem("login");
      }

      // product click --
      $('.product.teaser .p-title, .product.teaser .p-image').once(function() {
        $(this).click(function() {
          var el = $(this).closest('.product.teaser');
          onCardClickEvent(getPageListing(), {
            "id": $(el).data("id"),
            "name": $(el).data('title'),
            "variant": $(el).data('variant'),
            "price": $(el).data('price'),
            "list": getProductsListing(el),
            'position': $(el).parent().index()+1
          });
          localStorage.fromListing = getPageListing();
        });
      });

      // Добавить в корзину из каталога
      $('a.btn-add-to-cart').once(function() {
        $(this).bind('mousedown touchstart', function() {
          var el = $(this).closest('.product');
          onAddToCartEvent(getPageListing(), "Нажали «Купить» из каталога", {
            "id": $(el).data("id"),
            "name": $(el).data('title'),
            "variant": $(el).data('variant'),
            "price": $(el).data('price'),
            "quantity": 1,
            "list": getProductsListing(el),
          });
        });
      });
      // Добавить в корзину из описания
      $('.commerce-add-to-cart').once(function() {
        $('.commerce-add-to-cart .form-submit').bind('mousedown touchstart', function() {
          var el = $(this).closest('.product');
          onAddToCartEvent("Карточка товара", "Нажали «Купить» из карточки товара", {
            "id": $("[name=product_id]").val(),
            "name": $(el).data('title'),
            "variant": $(".attribute-widgets > .form-item > label").text() + ": " + $(".attribute-widgets  .active.form-item > label").text().trim(),
            "price": $(".product-breaf .price-amount").text().replace("₽", "").trim(),
            "quantity": 1,
          });
        });
      });
      // Добавление и Удаление из на странице корзины
      $('.ccf-item').once(function() {
        var item = $(this);
        item.find(".commerce-quantity-plusminus-link-increase a").bind('mousedown touchstart', function() {
          onAddToCartEvent("Корзина", "Нажали плюс в корзине", {
            "id": item.find(".ccf-product").data("id"),
            "name": item.find(".ccf-title").text(),
            "variant": item.find(".ccf-options").text().trim(),
            "price": item.find(".ccf-product").data("price"),
          });
        });
        item.find(".delete-line-item, .commerce-quantity-plusminus-link-decrease a").bind('mousedown touchstart', function() {
          onDeleteFromCartEvent({
            "id": item.find(".ccf-product").data("id"),
            "name": item.find(".ccf-title").text(),
            "variant": item.find(".ccf-options").text().trim(),
            "price": item.find(".ccf-product").data("price"),
          });
        });
      });

      $('.commerce-add-to-cart').once(function() {
        $('.commerce-add-to-cart .form-submit').bind('mousedown touchstart', function() {
          var el = $(this).closest('.product');
          onAddToCartEvent("Карточка товара", "Нажали «Купить» из карточки товара", {
            "id": $("[name=product_id]").val(),
            "name": $(el).data('title'),
            "variant": $(".attribute-widgets > .form-item > label").text() + ": " + $(".attribute-widgets  .active.form-item > label").text().trim(),
            "price": $(".product-breaf .price-amount").text().replace("₽", "").trim(),
            "quantity": 1,
          });
        });
      });

      // add to wishlist
      $('a.add-to-wishlist').click(function() {
        onAddToWishlistEvent();
      });

      $('body').once(function() {
        onPageLoadEvent();

        // cart page open
        if ($('body').is('.page-cart')) {
          onCartOpenEvent();
        }

        // клик по кнопке "Перейти к оформлению"
        $('.page-cart .checkout-continue').bind('mousedown touchstart', function() {
          onCheckoutClickEvent();
        });

        // checkout page open
        if ($('body').is('.page-checkout-checkout')) {
          onCheckoutInitEvent();
        }
        // order completed
        if ($('body').is('.page-checkout-complete')) {
          onCheckoutCompleteEvent();
        }

        // обработчик Flexslider carousel кнопки Next (тк js, то кнопка появляется не сразу)
        setTimeout(function() {
          $(".flex-direction-nav .flex-next").click(function(){
            onFlexCarouselNextClickEvent();
          });
        }, 1000);

        // обработчик Bootstrap carousel after slide
        $('#views-bootstrap-carousel-1').on('slid.bs.carousel', function () {
          onBStrapCarouselSlideEvent();
        });
      });


      $('.commerce-checkout-form-checkout').once(function() {
        // отследить применение промокода -------------------------------------
        $(".commerce_coupon [name=coupon_add]").bind('mousedown touchstart', function() {
          // отследить нажатие сабмита
          localStorage.promocode = $(".form-item-commerce-coupon-coupon-code input").val();
        });
        if (localStorage.promocode != undefined) {
          var promocode_redeemed = $(".commerce_coupon .views-field-code").text();
          onUserPromocodeUse(localStorage.promocode.trim(), promocode_redeemed.trim());
          localStorage.removeItem("promocode");
        }

        // отследить событие списания баллов ----------------------------------
        $("#commerce-userpoints-discount [name=cup_submit]").bind('mousedown touchstart', function() {
          // отследить нажатие сабмита
          localStorage.userpoints = $(".form-item-commerce-userpoints-discount-custom-amount input").val();
        });
        if (localStorage.userpoints != undefined) {
          var points_redeemed = $(".commerce-userpoints-info").data("points-redeemed");
          onUserPointsUse(localStorage.userpoints, points_redeemed);
          localStorage.removeItem("userpoints");
        }



        // payment method choose ----------------------------------------------
        // отправить при загрузке страницы
        onPaymentClickEvent($('.commerce_payment .active label.control-label'));
        // отправить при выборе
        $('.commerce_payment label.control-label').on('click', function() {
          onPaymentClickEvent(this);
        });
        // shipping method choose ---------------------------------------------
        // отправить при загрузке страницы
        onDeliveryClickEvent($('.commerce_shipping .active label.control-label'));
        // отправить при выборе
        $('.commerce_shipping label.control-label').on('click', function() {
          onDeliveryClickEvent(this);
        });
      });




      // -------------------------------- Events Processing --------------------
      function onPageLoadEvent() {
        // просмотр описания товара
        if ($("body").is(".node-type-product")) {
          var el = $(".product.full");
          GTMshowFullCardSendData({
            "id": $("[name=product_id]").val(),
            "name": $(el).data('title'),
            "variant": $(".attribute-widgets > .form-item > label").text() + ": " + $(".attribute-widgets  .active.form-item > label").text().trim(),
            "price": $(".product-breaf .price-amount").text().replace("₽", "").trim(),
          });
        }
        setTimeout(function() {
          GTMcheckCardsImpressions();
          GTMcheckBannersImpressions();
        }, 1000);

      }
      function onScrollEvent() {
        GTMcheckCardsImpressions();
        GTMcheckBannersImpressions();
      }
      function onResizeEvent() {
        GTMcheckCardsImpressions();
        GTMcheckBannersImpressions();
      }
      function onFlexCarouselNextClickEvent() {
        setTimeout(GTMcheckCardsImpressions, 1000);
      }
      function onBStrapCarouselSlideEvent() {
        GTMcheckBannersImpressions();
      }
      function onUserAuthorization(login) {
        GTMuserAuthorization(login);
      }
      function onUserRegistration(login) {
        GTMuserRegistration(login);
      }
      function onAddToCartEvent(list, action, item) {
        fbq('track', 'AddToCart');
        GTMaddToCartSendData(list, action, item);
      }
      function onDeleteFromCartEvent(item) {
        GTMdeleteFromCartSendData(item);
      }
      function onCardClickEvent(list, item) {
        GTMclickCardSendData(list, item);
      }
      function onAddToWishlistEvent() {
        fbq('track', 'AddToWishlist');
      }
      function onCartOpenEvent() {
        saveCartContentToLocalStorage();
        GTMcartOpenSendData();
      }

      function onCheckoutClickEvent() {
        saveCartContentToLocalStorage();
      }

      function onCheckoutInitEvent() {
        GTMCheckoutSendData();
      }
      function onUserPromocodeUse(promocode_used, promocode_redeemed) {
        GTMuserPromocodeUse(promocode_used, promocode_redeemed);
      }
      function onUserPointsUse(points_used, points_redeemed) {
        GTMuserPointsUse(points_used, points_redeemed);
      }
      function onDeliveryClickEvent(el) {
        var method = $(el).find(".carrier").text();
        if (method) {
          GTMDeliveryClickSendData(method);
        }
      }
      function onPaymentClickEvent(el) {
        var method = $(el).find(".carrier").text();
        if (method) {
          GTMDPaymentClickSendData(method);
          fbq("track", "AddPaymentInfo");
        }
      }
      function onCheckoutCompleteEvent() {
        // var paid = $('.order-complete .oc-total').data('paid');
        var total = $('.order-complete .oc-total').data('total');
        GTMCheckoutCompleteEvent(total);
      }



      // ------------------------- GTM -----------------------------------------
      // ------------------------- Authorization
      function GTMuserAuthorization(login) {
        var type = login.indexOf("@") !== -1 ? "При помощи e-mail" : "При помощи телефона";
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'UA event',
          'eventCategory': 'Авторизация',
          'eventAction': type,
          'virtualPage': '/virtual/authorization',
          'virtualPageTitle': 'Авторизация',
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Registration
      function GTMuserRegistration(login) {
        var type = login.indexOf("@") !== -1 ? "При помощи e-mail" : "При помощи телефона";
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'UA event',
          'eventCategory': 'Регистрация',
          'eventAction': type,
          'virtualPage': '/virtual/registration',
          'virtualPageTitle': 'Регистрация',
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Promocode use
      function GTMuserPromocodeUse(promocode_used, promocode_redeemed) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'UA event',
          'eventCategory': 'Активация промокода',
          'eventAction': promocode_used,
          'eventLabel': (promocode_used && (promocode_used === promocode_redeemed)) ? "Действителен" : "Не действителен",
          'virtualPage': '/virtual/promo_code_activation',
          'virtualPageTitle': 'Активация промо-кода',
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Userpoints use
      function GTMuserPointsUse(points_used, points_redeemed) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'UA event',
          'eventCategory': 'Использование баллов',
          'eventAction': points_used,
          'eventLabel': points_redeemed === 0 ? 'Не списано' : 'Списано',
          'virtualPage': '/virtual/userpoints_use',
          'virtualPageTitle': 'Использование баллов',
          'userId': Drupal.settings.user.uid,
        });
      }


      // ------------------------- Cart page open
      function GTMcartOpenSendData() {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 1, 'action': 'checkout'},
              'products' : getCartContentFromLocalStorage()
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Checkout page open
      function GTMCheckoutSendData() {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 2, 'action': 'checkout'},
              'products' : getCartContentFromLocalStorage()
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Choose Delivery Method
      function GTMDeliveryClickSendData(method) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 3, 'option': method},
              'products' : getCartContentFromLocalStorage()
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Choose Payment Method
      function GTMDPaymentClickSendData(method) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 5, 'option': method},
              'products' : getCartContentFromLocalStorage()
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }

      // ------------------------- Checkout Complete
      function GTMCheckoutCompleteEvent(total) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 6, 'action': 'checkout'},
              'products' : getCartContentFromLocalStorage()
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }


      // ------------------------- Add to  Cart
      function GTMaddToCartSendData(list, action, item) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'addToCart',
          'eventCategory': 'Добавление в корзину',
          'eventAction': action,
          'eventLabel': item.name,
          'ecommerce': {
            'add': {
              'actionField': {'list': list},
              'products': [item]
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }

      // ------------------------- Delete From Cart
      function GTMdeleteFromCartSendData(item) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'removeFromCart',
          'eventCategory': 'ecommerce',
          'eventAction': 'removeFromCart',
          'eventLabel': item.name,
          'ecommerce': {
            'remove': {
              'products': [item]
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }

      // ------------------------- Show Full Card
      function GTMshowFullCardSendData(item) {
        var list = !!localStorage.fromListing ? localStorage.fromListing : "";
        localStorage.removeItem("fromListing");

        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'productDetails',
          'eventCategory': 'ecommerce',
          'eventAction': 'productDetails',
          'ecommerce': {
            'detail': {
              'actionField': {'list': list},
              'products': [item]
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }

      // ------------------------- Card Click
      function GTMclickCardSendData(list, item) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'productClick',
          'eventCategory': 'ecommerce',
          'eventAction': 'productClick',
          'ecommerce': {
            'click': {
              'actionField': {'list': list},
              'products': [item]
            }
          },
          'userId': Drupal.settings.user.uid,
        });
      }
      // ------------------------- Card Impressions
      function GTMcheckCardsImpressions() {
        $(".view-carousel .product.teaser").each(function(index) {
          GTMCardImpressionSendData(this, $(this).closest(".view-carousel"));
        });
        $(".view:not(.view-carousel) .product.teaser, .field .product.teaser").each(function(index) {
          GTMCardImpressionSendData(this);
        });
      }
      function GTMCardImpressionSendData(el, box = null) {
        if (checkDivPosition(el, box)) {
          // если элемент в поле видимости и данные ещё не отправлялись, отправить
          if ($(el).data('was-impressed') !== true) {
            $(el).addClass('was-impressed').data('was-impressed', true);
            // послать информацию
            window.dataLayer = window.dataLayer || [];
            dataLayer.push({
              'event': 'productImpression',
              'eventCategory': 'ecommerce',
              'eventAction': 'productImpression',
              'ecommerce': {
                'impressions': [{
                  'id': $(el).data('id'),
                  'name': $(el).data('title'),
                  'price': $(el).data('price'),
                  'variant': $(el).data('variant'),
                  'position': $(el).parent().index()+1
                }]
              },
            });
          }
        }
      }


      // ------------------------- Banner Impressions
      function GTMcheckBannersImpressions() {
        $(".view-sliders .carousel-inner .item.active").each(function(index) {
          GTMBannerImpressionSendData(this);
        });
      }
      function GTMBannerImpressionSendData(el, box = null) {
        if (checkDivPosition(el, box)) {
          // если элемент в поле видимости и данные ещё не отправлялись, отправить
          if ($(el).data('was-impressed') !== true) {
            $(el).addClass('was-impressed').data('was-impressed', true);
            // послать информацию
            window.dataLayer = window.dataLayer || [];
            dataLayer.push({
              'event': 'promotionView',
              'eventCategory': 'ecommerce',
              'eventAction': 'promotionView',
              'ecommerce': {
                'promoView': {
                  'promotions': [{
                    'id': $(el).find("img").attr("alt"),
                    'name': $(el).find("img").attr("alt"),
                    'creative': 'Слайдер на главной',
                    'position': $(el).index()+1
                  }]
                },
              },
            });
          }
        }
      }

      // проверить видимость элемента в окне браузера, опционально в пределах элемента
      function checkDivPosition(el, box = null) {
        var el_pos = $(el).offset();

        var see_x1 = $(document).scrollLeft();
        var see_x2 = $(window).width() + $(document).scrollLeft();
        var see_y1 = $(document).scrollTop();
        var see_y2 = $(window).height() + $(document).scrollTop();
        if (box)  {
          var box_pos = $(box).offset();
          var box_x1 = box_pos.left;
          var box_x2 = box_pos.left + $(box).width();
          var box_y1 = box_pos.top;
          var box_y2 = box_pos.top + $(box).height();
        }

        var el_x1 = el_pos.left;
        var el_x2 = el_pos.left + $(el).width();
        var el_y1 = el_pos.top;
        var el_y2 = el_pos.top + $(el).height()/2;

        if (( el_x1 > 0 && el_x1 >= see_x1 && el_x2 <= see_x2 && el_y1 > 0 && el_y1 >= see_y1 && el_y2 <= see_y2 )
          && ((!box) || (box && el_x1 >= box_x1 && el_x2 <= box_x2 && el_y1 >= box_y1 && el_y2 <= box_y2))) {
          return true;
        }
        return false;
      }

      // сохранить корзину в localStorage
      function saveCartContentToLocalStorage() {
        if ($('body').is('.page-cart')) {
          var items = [];
          $(".view-items .view-item").each(function () {
            items.push({
              "id": $(this).find(".ccf-product").data("id"),
              "name": $(this).find(".ccf-title").text(),
              "variant": $(this).find(".ccf-options").text().trim(),
              "price": $(this).find(".ccf-product").data("price"),
              "quantity": $(this).find(".ccf-product").data("quantity"),
            });
          });
          localStorage.cartProducts = JSON.stringify(items);
        }
      }
      function getCartContentFromLocalStorage() {
        return localStorage.cartProducts !== undefined ? JSON.parse(localStorage.cartProducts) : false;
      }

      // определить листинг страницы
      function getPageListing() {
        var list = "";
        if ($("body").is(".front")) {
          list = "Главная страница";
        } else if ($("body").is(".page-node")) {
          list = "Карточка товара";
        } else if ($("body").is(".page-taxonomy")) {
          list = $(".page-header h1").text().trim();
        }
        return list;
      }

      // определить листинг карточки товара
      function getProductsListing(product) {
        var productList = $(product).closest(".row").children(".product-related-title").text().trim(); // products related
        if (productList === "") {
          productList = $(product).closest(".block").find("h2").text().trim(); // blocks
        }
        if (productList === "") {
          productList = $(".page-header h1").text().trim(); // catalog
        }
        return productList;
      }

    }
  };
})(jQuery);
