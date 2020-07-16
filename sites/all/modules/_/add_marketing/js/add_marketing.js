(function ($) {
  Drupal.behaviors.add_marketing = {
    attach: function (context, settings) {

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
    }
  };
})(jQuery);
