<?php //dsm('1'); ?>
<div class="invoice" style="background: #fff; padding: 30px 60px; min-height: 1000px; width: 1000px;">
    <div class="do-not-print" style="text-align: right; margin-bottom: 15px;"><a href="javascript:window.print()" class="btn btn-info">Распечатать</a></div>
    <table class="table-condensed" style="color:#000; margin-bottom:20px;">
        <tr height="30"><td rowspan="4" width="150"><img src="<?php print $invoice['payment_method']['settings']['rcpt_logo']; ?>"></td><td style="font-size:15px;"><strong><?php print $invoice['payment_method']['settings']['rcpt_name']; ?></strong></td></tr>
        <tr height="30"><td><?php print $invoice['payment_method']['settings']['rcpt_address']; ?></td></tr>
        <tr><td></td></tr>
    </table>

    <table class="table table-bordered table-condensed" style="color:#000; margin-bottom:100px;">
        <tr><td>ИНН <?php print $invoice['payment_method']['settings']['rcpt_inn']; ?></td><td>КПП <?php print $invoice['payment_method']['settings']['rcpt_kpp']; ?></td><td rowspan="2" width="60">Сч.№</td><td rowspan="2" width="25%"><?php print $invoice['payment_method']['settings']['rcpt_acc']; ?></td></tr>
        <tr><td colspan="2">Получатель<br/><?php print $invoice['payment_method']['settings']['rcpt_name'].', '.$invoice['payment_method']['settings']['rcpt_address']; ?></td></tr>
        <tr><td colspan="2" rowspan="2">Банк получателя<br/><?php print $invoice['payment_method']['settings']['rcpt_bank']; ?></td><td>БИК</td><td><?php print $invoice['payment_method']['settings']['rcpt_bik']; ?></td></tr>
        <tr><td>Сч.№</td><td><?php print $invoice['payment_method']['settings']['rcpt_k_acc']; ?></td></tr>
    </table>

    <table class="table-condensed" style="color:#000; margin-bottom:10px;">
        <tr><td colspan="2" style="font: 18px/20px open_sansbold; text-align:center;">СЧЁТ №: <?php print $invoice['order_id']; ?> от <?php print date("d.m.Y");; ?></td></tr>
        <tr><td width="15%">Плательщик:</td><td></td></tr>
        <tr><td width="15%">Грузополучатель:</td><td></td></tr>
    </table>

    <table class="table table-bordered table-condensed" style="color:#000; margin-bottom:30px;">
        <tr><th width="35">№</th><th width="60%">Наименование</th><th width="7%">Кол-во</th><th width="15%">Цена, руб.</td><th width="20%">Сумма, руб.</th></tr>
        <?php foreach($invoice['line_items'] as $key=>$line_item): ?>
        <tr>
            <td><?php print ($key+1); ?><td align="left"><?php print $line_item['title']; ?></td><td align="right"><?php print $line_item['quantity']; ?></td>
            <td align="right"><?php print $line_item['price']; ?></td><td align="right"><?php print $line_item['amount']; ?></td>
        </tr>
        <?php endforeach; ?>
        <tr><td colspan="4" align="right" style="border: none;">В том числе НДС:</td><td align="right"><?php print $invoice['total_nds']; ?></td></tr>
        <tr><td colspan="4" align="right" style="border: none;">Итого:</td><td align="right"><?php print $invoice['total_amount']; ?></td></tr>
    </table>

    <table class="table-condensed" style="color:#000; margin-bottom:50px;">
        <tr><td>Всего наименований <?php print $invoice['total_qty']; ?>, на сумму <?php print $invoice['total_amount']; ?> руб.</td></tr>
        <tr><td><strong><?php print $invoice['total_amount_str']; ?></strong></td></tr>
    </table>

    <table class="table-condensed" style="color:#000; margin-bottom:10px;line-height:30px;">
        <tr><td style="line-height:30px;" width="20%">Руководитель </td><td style="border-bottom:1px solid #000;" width="30%"><?php print $invoice['payment_method']['settings']['rcpt_boss']; ?></td><td colspan="2" align="center" valign="middle">М.П.</td></tr>
        <tr><td style="line-height:30px;">Гл. бухгалтер </td><td  style="border-bottom:1px solid #000;"><?php print $invoice['payment_method']['settings']['rcpt_buh']; ?></td></tr>
    </table>
  <div class="do-not-print" style="text-align: right; margin-top: 15px;"><a href="javascript:window.print()" class="btn btn-info">Распечатать</a></div>
</div>
