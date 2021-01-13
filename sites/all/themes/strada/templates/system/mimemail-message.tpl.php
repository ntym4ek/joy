<?php
/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[module]--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */

/** сформировать переменные
* message - текст сообщения на языке письма
* sign - подпись на языке письма
* auto - текст сообщения о том, что письмо сформировано автоматически
*/
$sign = empty($message['params']['context']['sign']) ? 'Интернет-магазин joy-magazin.ru<br />Мы будем рады ответить на Ваши вопросы по электронной почте help@joy-magazin.ru ' : $message['params']['context']['sign'];
$auto = !isset($message['params']['context']['auto']) ? 'Письмо сформировано автоматически' : '';
$a = url('unsubscribe', ['absolute' => true, 'query' => ['mail' => $message["to"]]]);
?>

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <?php if ($css): ?>
            <style type="text/css">
                <!--
                <?php print $css ?>
                -->
            </style>
        <?php endif; ?>
    </head>
    <body id="mimemail-body">
        <table width="100%" style="font-family: ubuntu,Lucida Grande,Lucida Sans,Lucida Sans Unicode,Arial,Helvetica,Verdana,sans-serif; font-size: 14px;">
            <tr>
                <td align="center">
                    <table width="800px">
                        <tr>
                            <td style="border-bottom: 1px solid #ccc;">
                                <div style="width: 110px; margin-bottom: 10px;">
                                  <a href="https://joy-magazin.ru" target="_blank" title="JOY-MAGAZIN - ДЛЯ ДОМА И САДА"><img src="https://joy-magazin.ru/sites/all/themes/strada/logo_mail.png" style="width: 100%;"></a>
                                </div>
                            </td>
                        </tr>
                        <tr>
                            <td style="border-bottom: 1px solid #ccc; padding: 20px; height:300px; vertical-align: top;">
                                <? print $body; ?>
                            </td>
                        </tr>
                        <tr>
                            <td style="padding: 20px;">
                                <? print $sign; ?><br />
                                <? if ($auto): ?>
                                    <span style="color: #bbb; font-size: .8em;">
                                      <? print $auto; ?><br />
                                      <a href="<? print url('unsubscribe', ['absolute' => true, 'query' => ['mail' => $message["to"]]]);  ?>">Отписаться</a> от рассылки
                                    </span>
                                <? endif; ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
        </table>
    </body>
</html>
