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

      // -------------------------------- Запуск Событий -----------------------
      onPageLoadEvent();

      $(document).scroll(function(){
        onScrollEvent();
      });

      $(window).resize(function(){
        onResizeEvent();
      });

      // событие, срабатывающее на добавление в корзину
      $('a.btn-add-to-cart, .commerce-add-to-cart .form-submit').click(function() {
        onAddToCartEvent();
      });
      // событие, срабатывающее на добавление в вишлист
      $('a.add-to-wishlist').click(function() {
        onAddToWishlistEvent();
      });


      // -------------------------------- Обработка Событий --------------------
      function onPageLoadEvent() {
        GTMcheckCardsImpressions();
      }
      function onScrollEvent() {
        GTMcheckCardsImpressions();
      }
      function onResizeEvent() {
        GTMcheckCardsImpressions();
      }
      function onAddToCartEvent() {
        // сообщить FB
        fbq('track', 'AddToCart');
      }
      function onAddToWishlistEvent() {
        // сообщить FB
        fbq('track', 'AddToWishlist');
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
      // ------------------------- Card Impressions
      function GTMcheckCardsImpressions() {
        $(".product.teaser").each(function(index) {
          if (!GTMcheckCardPositionAndSendDataToGTM(this)) {
            return false;
          }
        });
      }

      function GTMcheckCardPositionAndSendDataToGTM(el) {
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
            // todo послать информацию
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
