(function ($, Drupal, window, document, undefined) {

    Drupal.behaviors.cdek = {
        attach: function (context, settings) {

            $('#cdek_link').once(function () {
                $('#cdek_link').bind('click', function () {
                    // инициализация диалога
                    var containerId = 'forpvz';
                    var html = '<div id="' + containerId + '" style="height:500px;"></div>';

                    if (!$('#forpvz').is('div')) {
                        var $dialog = $(html).appendTo('body');
                    } else {
                        $dialog = $('#forpvz');
                    }
                    var width = $(window).width();
                    if (width > 1000) width = $(window).width() * .8;
                    $dialog.dialog({
                        title: 'Выберите пункт выдачи',
                        autoOpen: false,
                        width: width,
                        height: 550,
                        modal: true,
                        open: function() {
                            // Close dialog when prooer target clicked
                            $('.ui-widget-overlay').bind('click', function (event) {
                                if ($(event.target).hasClass('ui-widget-overlay')) {
                                    $dialog.dialog('close');
                                    // $dialog.remove();
                                }
                            });
                        }
                    });

                    // инициализация виджета СДЭК
                    var city = $("body").attr("data-locality");
                    var location = JSON.parse($.cookie('user_region'));
                    if (location && location.data.city) {
                        city = location.data.city;
                    }

                    var order_amount = $(".cdek-order-amount").val();
                    var order_weight = $(".cdek-order-weight").val();
                    var widjet = new ISDEKWidjet({
                        showWarns: true,
                        showErrors: true,
                        showLogs: true,
                        hideMessages: false,
                        choose: true,
                        path: Drupal.settings.basePath + 'sites/all/modules/_/commerce_cdek/widget/scripts/',
                        country: 'Россия',
                        defaultCity: city,
                        cityFrom: 'Киров',
                        link: containerId,
                        hidedress: true,
                        hidecash: true,
                        // hidedelt: true,
                        goods: [{
                            length: 1,
                            width: 1,
                            height: 1,
                            weight: order_weight
                        }],
                        onChoose: onChoose,
                        onChooseProfile: onChooseProfile,
                        onCalculate: onCalculate
                    });

                    Drupal.attachBehaviors($dialog);
                    $dialog.dialog('open');
                });
            });


            function onCalculate(result) {
                // при выборе нового города, сохранить новый параметры расчёта
                $(".field-name-field-data textarea").val(JSON.stringify(result));
            }

            function onChoose(result) {
                $(".cdek_addr").html(result.cityName + ', ' + result.PVZ.Address + " (id " + result.id + ")");

                var data = {};
                try { data = JSON.parse($(".field-name-field-data textarea").val()); }
                catch(error) {}
                data.cdek = {
                        city: result.city,
                        cityName: result.cityName,
                        pvz: {address: result.PVZ.Address},
                        id: result.id
                    };
                $(".field-name-field-data textarea").val(JSON.stringify(data));

                $('#forpvz').dialog('close');
                $("[id^='edit-commerce-shipping-recalc']").click();
            }

            function onChooseProfile(result) {
                // при выборе курьера
                $(".cdek_addr_show").html('Курьером до двери');
                var storage = JSON.parse($(".field-name-field-data textarea").val());
                $.extend(storage, {
                    courier: true,
                });
                $(".field-name-field-data textarea").val(JSON.stringify(storage));
                $('#forpvz').dialog('close');
                $("[id^='edit-commerce-shipping-recalc']").click();
            }



        }
    }

})(jQuery, Drupal, this, this.document);
