<?php
$tinid        = $_REQUEST['tinid'];
$mTelaInicial = new TelaInicial($tinid);

if($_REQUEST['act'] == 'salvar'){
    $_REQUEST['usucpf']        = $_SESSION['usucpf'];
    $_REQUEST['tindtcadastro'] = 'now()';

    $_REQUEST['tinhtml'] = stripcslashes($_REQUEST['tinhtml']);

    $mTelaInicial->popularDadosObjeto()->salvar();
    $mTelaInicial->commit();

    $db->sucesso($_REQUEST['modulo']);
}

extract($mTelaInicial->getTelaInicial());

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );
?>

<form method="POST" name="formulario" action="">
    <input type=hidden name="act" value="salvar"/>
    <input type=hidden name="tinid" value="<?= $tinid; ?>"/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita" width="5%">
                HTML:
            </td>
            <td>
            <?
                echo campo_textarea('tinhtml', 'N', 'S', '', 200, 40, 10000);
            ?>
            </td>
        </tr>

        <tr style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%">
                &nbsp;
            </td>
            <td>
                <input type="submit" name="alterar" value="Salvar"/>
            </td>
        </tr>
    </table>
</form>