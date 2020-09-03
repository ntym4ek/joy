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
          if (url.searchParams.has('utm_content')) user_marketing.utm[key]['utm_term'] = url.searchParams.get('utm_content');

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

      // product click
      $('.product.teaser .p-title, .product.teaser .p-image').click(function() {
        var el = $(this).closest('.product.teaser');
        onCardClickEvent(el);
      });
      // add to cart
      $('a.btn-add-to-cart, .commerce-add-to-cart .form-submit').bind('mousedown touchstart', function() {
        var el = $(this).closest('.product');
        onAddToCartEvent(el);
      });
      // add to wishlist
      $('a.add-to-wishlist').click(function() {
        onAddToWishlistEvent();
      });

      $('body').once(function() {
        onPageLoadEvent();

        // checkout page open
        if ($('body').is('.page-checkout-checkout')) {
          onCheckoutInitEvent();
        }
        // order completed
        if ($('body').is('.page-checkout-complete')) {
          onCheckoutCompleteEvent();
        }
      });

      $('.commerce-checkout-form-checkout').once(function() {
        // payment method choose
        $('.commerce_payment label.control-label').one('click', function() {
          onPaymentClickEvent(this);
        });
        // shipping method choose
        $('.commerce_shipping label.control-label').one('click', function() {
          onDeliveryClickEvent(this);
        });
      });




      // -------------------------------- Events Processing --------------------
      function onPageLoadEvent() {
        // GTMcheckCardsImpressions();
        GTMshowFullCardSendData();
      }
      function onScrollEvent() {
        // GTMcheckCardsImpressions();
      }
      function onResizeEvent() {
        // GTMcheckCardsImpressions();
      }
      function onAddToCartEvent(el) {
        // FB
        fbq('track', 'AddToCart');
        // GTM
        GTMaddToCartSendData(el);
      }
      function onCardClickEvent(el) {
        GTMclickCardSendData(el);
      }
      function onAddToWishlistEvent() {
        // FB
        fbq('track', 'AddToWishlist');
      }
      function onCheckoutInitEvent() {
        GTMCheckoutSendData();
      }
      function onDeliveryClickEvent(el) {
        var method = $(el).find('.carrier').text();
        GTMDeliveryClickSendData(method);
      }
      function onPaymentClickEvent(el) {
        var method = $(el).find('.carrier').text();
        GTMDPaymentClickSendData(method);
      }
      function onCheckoutCompleteEvent() {
        var paid = $('.order-complete .oc-total').data('paid');
        var total = $('.order-complete .oc-total').data('total');
        if (paid) {
          GTMCheckoutCompleteEvent(total);
        }
      }


      // // FB начало оформления
      // $('.commerce-cart-form-checkout .checkout-continue').click(function() {
      //   fbq('track', 'InitiateCheckout');
      // });
      // // FB завершение оформления (со страницы )
      //
      // $('.commerce-checkout-form-checkout .checkout-continue').click(function() {
      //   var total = $('.checkout-summary .cs-total').data('cs-total');
      //   fbq('track', 'Purchase', {value: total, currency: 'RUB'});
      // });

      // ------------------------- GTM -----------------------------------------
      // ------------------------- Checkout Complete
      function GTMCheckoutCompleteEvent(total) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'orderPaid',
          'ecommerce': {
            'currencyCode': 'RUB',
            'checkout': {
              'actionField': {revenue: total},
            }
          },
        });
      }
      // ------------------------- Choose Payment Method
      function GTMDPaymentClickSendData(method) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 2, 'option': method},
            }
          },
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
            }
          },
        });
      }
      // ------------------------- Checkout page open
      function GTMCheckoutSendData() {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'event': 'checkout',
          'ecommerce': {
            'checkout': {
              'actionField': {'step': 1},
            }
          },
        });
      }
      // ------------------------- Add to  Cart
      function GTMaddToCartSendData(el) {
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'ecommerce': {
            'currencyCode': 'RUB',
            'add': {
              'products': [{
                'name':  $(el).data('title'),
                'price': $(el).data('price'),
                'variant': $(el).data('variant'),
                'quantity': 1
              }]
            }
          },
        });
      }

      // ------------------------- Show Full Card
      function GTMshowFullCardSendData() {
        if ($(".product").is(".full")) {
          var el = $(".product.full");
          var price = $(el).data('price');
          if (price) {
            window.dataLayer = window.dataLayer || [];
            dataLayer.push({
              'ecommerce': {
                'currencyCode': 'RUB',
                'detail': {
                  'actionField': {'list': ''},
                  'products': [{
                    'name':  $(el).data('title'),
                    'price': $(el).data('price'),
                    'variant': $(el).data('variant'),
                  }]
                }
              },
            });
          }
        }
      }

      // ------------------------- Card Click
      function GTMclickCardSendData(el) {
        console.log({
          'ecommerce': {
            'currencyCode': 'RUB',
            'click': {
              'actionField': {'list': ''},
              'products': [{
                'name':  $(el).data('title'),
                'price': $(el).data('price'),
                'variant': $(el).data('variant'),
              }]
            }
          },
        });
        window.dataLayer = window.dataLayer || [];
        dataLayer.push({
          'ecommerce': {
            'currencyCode': 'RUB',
            'click': {
              'actionField': {'list': ''},
              'products': [{
                'name':  $(el).data('title'),
                'price': $(el).data('price'),
                'variant': $(el).data('variant'),
              }]
            }
          },
        });
      }
      // ------------------------- Card Impressions
      function GTMcheckCardsImpressions() {
        $(".product.teaser").each(function(index) {
          if (!GTMcheckCardPositionAndSendData(this)) {
            return false;
          }
        });
      }
      function GTMcheckCardPositionAndSendData(el) {
        var div_position = $(el).offset();

        var see_x1 = $(document).scrollLeft();
        var see_x2 = $(window).width() + $(document).scrollLeft();
        var see_y1 = $(document).scrollTop();
        var see_y2 = $(window).height() + $(document).scrollTop();

        var div_x1 = div_position.left;
        var div_x2 = div_position.left + $(el).width();
        var div_y1 = div_position.top;
        var div_y2 = div_position.top + $(el).height();

        if( div_x1 >= see_x1 && div_x2 <= see_x2 && div_y1 >= see_y1 && div_y2 <= see_y2 ){
          // если элемент в поле видимости и данные ещё не отправлялись, отправить
          if ($(el).data('was-impressed') !== true) {
            $(el).data('was-impressed', true);
            // послать информацию
            window.dataLayer = window.dataLayer || [];
            dataLayer.push({
              'event': 'ProductImpressions',
              'ecommerce': {
                'currencyCode': 'RUB',
                'impressions': [
                  {
                    'name': $(el).data('title'),
                    'price': $(el).data('price'),
                    'variant': $(el).data('variant'),
                  }]
              },
            });
          }
        } else {
          // если элемент ещё не был показан и не виден, то все следующие также не видны,
          // по возврату из функции выйти из цикла
          if ($(el).data('was-impressed') !== true) {
            return false;
          }
        }
        return true;
      }

    }
  };
})(jQuery);
