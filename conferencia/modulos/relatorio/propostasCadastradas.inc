<?php

$where = array();
extract($_REQUEST);

switch ($_REQUEST['act']){
    case 'filtraSubEixo':
        if ($_REQUEST['etmid']) {
            $habilitado = 'S';
            $dados = "SELECT setid AS codigo, setnome AS descricao
                                       FROM
                                       conferencia.subeixotematico
                                       WHERE
                                       etmid = '{$_REQUEST['etmid']}' AND setstatus = 'A'";
        } elseif ($_REQUEST['etmid'] == '') {
            $habilitado = 'N';
            $dados      = array(array('codigo'  => 0, 'descricao' => 'Selecione um Sub Eixo Temático', array("etmstatus='A'")));
        }
        $db->monta_combo("setid", $dados, $habilitado, 'Selecione...', '', '', '', 300, 'N', "setid");
        die();
    break;
    case 'pesquisar':
        if($_REQUEST['mcoid']){
            $where[] = "mco.mcoid = {$_REQUEST['mcoid']}";
        }
        if($_REQUEST['tpcid']){
            $where[] = "tpc.tpcid = {$_REQUEST['tpcid']}";
        }
        if(!empty($_REQUEST['pcntitulo'])){
            $where[] = "pcn.pcntitulo ILIKE '%{$_REQUEST['pcntitulo']}%'";
        }
        if($_REQUEST['esdid']){
            $where[] = "esd.esdid = {$_REQUEST['esdid']}";
        }
        if(!empty($_REQUEST['pcfdsc'])){
            $where[] = "pcf.pcfdsc ILIKE '%{$_REQUEST['pcfdsc']}%'";
        }
        if($_REQUEST['etmid']){
            $where[] = "etm.etmid = {$_REQUEST['etmid']}";
        }
        if($_REQUEST['setid']){
            $where[] = "set.setid = {$_REQUEST['setid']}";
        }
    break;
    case 'importarxls':
        header("Content-Type: application/vnd.ms-excel");
        header("Expires: 0");
        header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
        header("content-disposition: attachment;filename=propostas_cadastradas.xls");

        if($_REQUEST['mcoid']){
            $where[] = "mco.mcoid = {$_REQUEST['mcoid']}";
        }
        if($_REQUEST['tpcid']){
            $where[] = "tpc.tpcid = {$_REQUEST['tpcid']}";
        }
        if(!empty($_REQUEST['pcntitulo'])){
            $where[] = "pcn.pcntitulo ILIKE '%{$_REQUEST['pcntitulo']}%'";
        }
        if($_REQUEST['esdid']){
            $where[] = "esd.esdid = {$_REQUEST['esdid']}";
        }
        if(!empty($_REQUEST['pcfdsc'])){
            $where[] = "pcf.pcfdsc ILIKE '%{$_REQUEST['pcfdsc']}%'";
        }
        if($_REQUEST['etmid']){
            $where[] = "etm.etmid = {$_REQUEST['etmid']}";
        }
        if($_REQUEST['setid']){
            $where[] = "set.setid = {$_REQUEST['setid']}";
        }

        $modelo = new PropostaConferencia();
        $lista  = $modelo->getListaRelatorio($where,false);

        $arCabecalho = array("ID da proposta",
                             "Edição da Conferência",
                             "Tipo da Conferência",
                             "Titulo da Conferência",
                             "Situação da Conferência",
                             "Código da proposta",
                             "Proposta",
                             "Eixo Temático",
                             "Sub-Eixo Temático");

        // ARRAY de parametros de configuração da tabela
        $arConfig = array("style"         => "width:100%;",
                                      "totalLinha"    => false,
                                      "totalRegistro" => true);

        $arParamCol[0]  = array("type" => Lista::TYPESTRING);
        $arParamCol[1]  = array("type" => Lista::TYPESTRING);
        $arParamCol[2]  = array("type" => Lista::TYPESTRING);
        $arParamCol[3]  = array("type" => Lista::TYPESTRING);
        $arParamCol[4]  = array("type" => Lista::TYPESTRING);
        $arParamCol[5]  = array("type" => Lista::TYPESTRING);
        $arParamCol[6]  = array("type" => Lista::TYPESTRING);

        $oLista = new Lista($arConfig);
        $oLista->setCabecalho( $arCabecalho );
        $oLista->setCorpo( $lista, $arParamCol );
        $oLista->show();
        die();
    break;
}
require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo( $titulo_modulo, 'Selecione os filtros e clique no botão Pesquisar');
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"> </script>
<script type="text/javascript">

    function geraXLS(){
        $('#act').val('importarxls');
        $('#formulario').submit();
    }

    function filtraSubEixo(etmid) {
        $('#spanSubEixo').html('carregando...');
        $('#spanSubEixo').load("?modulo=relatorio/propostasCadastradas&acao=A",
                {'act': 'filtraSubEixo',
                    'etmid': etmid,
                    'pcnid': $('#pcnid').val()
        });

        $('#act').val('');
    }

    function limpar(){
        window.location = "conferencia.php?modulo=relatorio/propostasCadastradas&acao=A";
    }

    function pesquisar(){
        $('#act').val('pesquisar');
        $('#formulario').submit();
    }

 </script>



<form id="formulario" name="formulario" method="post">
	<input type="hidden" name="act" id="act" value=""/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <td align='right' class="SubTituloDireita" style="width: 30%;">
                Edição da Conferência:
            </td>
            <td>
                <?php
                $tipoConferencia = new MomentoConferencia();
                $dados           = $tipoConferencia->recuperarTodos("mcoid AS codigo,mconome AS descricao", array("mcostatus = 'A'"), "descricao");
                $db->monta_combo('mcoid', $dados, 'S', 'Selecione...', '', '', '', '300', 'N', 'mcoid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width: 30%;">
                Tipo da Conferência:
            </td>
            <td>
                <?php
                $tipoConferencia = new TipoConferencia();
                $dados           = $tipoConferencia->recuperarTodos("tpcid AS codigo,tpcsigla || ' - ' || tpcnome AS descricao", array("tpcstatus = 'A'"), "descricao");
                $db->monta_combo('tpcid', $dados, 'S', 'Selecione...', '', '', '', '300', 'N', 'tpcid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width: 30%;">
                Título da Conferência:
            </td>
            <td>
                <?= campo_texto('pcntitulo', 'N', 'S', '', 40, 255, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:30%;">
                Situação da Conferência:
            </td>
            <td>
                <?php
                $estado = new EstadoWorkflow();
                $dados = $estado->recuperarTodos("esdid as codigo, esddsc as descricao", array("esdstatus= 'A'", "tpdid ='".WF_PRECADASTRO_CONFERENCIA."'"), "descricao");
                $db->monta_combo("esdid", $dados, 'S', 'Selecione...', '', '', '', '300', 'N', "esdid");
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width: 30%;">
                Proposta:
            </td>
            <td>
                <?= campo_texto('pcfdsc', 'N', 'S', '', 40, 255, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                Eixo Temático:
            </td>
            <td>
                <?php
                $modelo = new EixoTematico();
                $dados = $modelo->recuperarTodos("etmid as codigo, etmnome as descricao", array("etmstatus= 'A'"), "descricao");
                $db->monta_combo("etmid", $dados, 'S', 'Selecione...', 'filtraSubEixo', '', '', '300', 'N', 'etmid');

                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:25%;">
                Sub-Eixo Temático:
            </td>
            <td>
                <span id="spanSubEixo">
                    <?php
                    if ($etmid) {
                        $habilitado = 'S';
                        $dados = "SELECT setid AS codigo, setnome AS descricao
                                   FROM
                                   conferencia.subeixotematico
                                   WHERE
                                   etmid = '{$etmid}' AND setstatus = 'A'";

                    } else{
                        $dados      = array(array('codigo'    => 0, 'descricao' => 'Selecione um Sub Eixo Temático'));
                        $habilitado = 'N';
                    }
                    $db->monta_combo("setid", $dados, $habilitado, 'Selecione...', '', '', '', '300', 'N', "setid");
                    ?>
                </span>
            </td>
        </tr>
       <tr style="background-color: #cccccc">
            <td align='right'style="vertical-align:top; width:30%"> </td>
            <td>
                <input type="button" value="Pesquisar" style="cursor: pointer" onclick="pesquisar()"/>
                <input type="button" value="Limpar" style="cursor: pointer" onclick="limpar()"/>
                <input type="button" value="Gerar Relatório" style="cursor: pointer" onclick="geraXLS()"/>
            </td>
        </tr>
        <tr>
            <td colspan="2">
                <?php

                    $modelo = new PropostaConferencia();
                    $lista  = $modelo->getListaRelatorio($where,true);

                    $arCabecalho = array("ID da proposta",
                                         "Edição da Conferência",
                                         "Tipo da Conferência",
                                         "Titulo da Conferência",
                                         "Situação da Conferência",
                                         "Código da proposta",
                                         "Proposta",
                                         "Eixo Temático",
                                         "Sub-Eixo Temático");

                    // ARRAY de parametros de configuração da tabela
                    $arConfig = array("style"         => "width:100%;",
                                      "totalLinha"    => false,
                                      "totalRegistro" => true);

                    $arParamCol[0]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[1]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[2]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[3]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[4]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[5]  = array("type" => Lista::TYPESTRING);
                    $arParamCol[6]  = array("type" => Lista::TYPESTRING);

                    $oLista = new Lista($arConfig);
                    $oLista->setCabecalho( $arCabecalho );
                    $oLista->setCorpo( $lista, $arParamCol );
                    $oLista->show();

                ?>
            </td>
	</table>
</form>