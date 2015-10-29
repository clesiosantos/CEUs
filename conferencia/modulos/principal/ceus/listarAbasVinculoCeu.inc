<?php

$mVinculoCeu = new VinculoCeu();
$mVinculoCeu->limparVinculoSession();

$mCeu = new Ceu();

$arWhere = array();
//ajax que carrega os municípios
if( $_REQUEST['act'] == 'filtraMunicipio' ){
    if( $_REQUEST['estuf'] ){
        $sql = $mCeu->getSqlMunicipioCeus("mun.muncod AS codigo, mun.mundescricao AS descricao", array("mun.estuf = '{$_REQUEST['estuf']}'"), "descricao");
        $habilitado_municipio = 'S';
    }else{
        $sql = array(array('codigo' => 0, 'descricao' => 'Escolha a UF'));
        $habilitado_municipio = 'N';
    }

    $muncod  = $_REQUEST['muncod'];
    $db->monta_combo("muncod", $sql, $habilitado_municipio, 'Selecione...', 'filtraCeu', '', '', 250, 'N', "muncod");
    die();
}elseif( $_REQUEST['act'] == 'filtraCeu' ){
	$muncod = $_REQUEST["muncod"];
	$ceuid  = $_REQUEST["ceuid"];
    if( $muncod ){
    	$habilitado_ceu = 'S';

        $dadosCeu = $mCeu->recuperarTodos("ceuid as codigo, ceucodigo || ' - ' || ceunome as descricao", array("ceustatus = 'A'", "mun.muncod = '{$muncod}'"), 2);
    }else{
    	$dadosCeu       = array(array('codigo' => 0, 'descricao' => 'Escolha o Município'));
        $habilitado_ceu = 'N';
    }

    $db->monta_combo("ceuid", $dadosCeu, $habilitado_ceu, 'Selecione...', '', '', '', 250, 'N', "ceuid");
    die();
}elseif($_POST["act"] == "pesquisar" || $_POST["act"] == "exportar_xls" ){
    if($_REQUEST["estuf"]){
        $arWhere[] = "mun.estuf = '{$_REQUEST["estuf"]}'";
    }
    if($_REQUEST["muncod"]){
        $arWhere[] = "mun.muncod = '{$_REQUEST["muncod"]}'";
    }
    if($_REQUEST["ceuid"]){
        $arWhere[] = "ceu.ceuid = '{$_REQUEST["ceuid"]}'";
    }
    if($_REQUEST["vceid_pesquisa"]){
        $arWhere[] = "vce.vceid = '{$_REQUEST["vceid_pesquisa"]}'";
    }
    if($_REQUEST["esdid"]){
        $arWhere['estado'] = "esd.esdid = '{$_REQUEST["esdid"]}'";
    }
    if($_REQUEST["mnuidfiltro"]){
        $arWhere[] = "mnu.mnuid = '{$_REQUEST["mnuidfiltro"]}'";
    }

    extract($_POST);

    if($_POST["act"] == "exportar_xls"){
        header( 'Content-Type: application/vnd.ms-excel' );
        header( 'Expires: 0' );
        header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
        header( 'Content-Disposition: attachment; filename="vinculos_ceu_'.date('Ymd').'.xls"' );

        $arConfig = array("style"           => "width: 100%;",
                          "totalLinha"      => false,
                          "totalRegistro"   => false);
        $arCabecalho = array("Identificador", "Contrato", "Tipo", "UF", "Município", "Código IBGE", "Aba","% de Preenchimento", "Situação do workflow","Quem fez a última atualização", "Quando foi a última ação", "Situação da aba");


        $arParamCol[0]  = array('type' => Lista::TYPESTRING);

        $arParamCol[7]  = array('html' => '<php>
    												if("{qrpid}" != "" && "{queid}" != "" && "{vceid}" != ""){
    										    		return formata_valor("{qrppreenchimento}", 0) . "%" ;
    												}else{
    													if("{mnuid}" == "' . MENU_CEU_DADOS_INICIAIS . '"){
    														if("{divid}" != ""){
    															return "100%";
    														}else{
    															return "0%";
    														}
    													}else{
    														return "-";
    													}
    												}
    									   </php>',
    							'align' => 'center');

    	$arParamCol[10]  = array('type' => Lista::TYPEDATE);
    	$arParamCol[11]  = array('align' => 'center');

        $rs = $mVinculoCeu->listarAbasVinculoCeu( $arWhere, false );
        $oLista = new Lista($arConfig);
        $oLista->setCabecalho( $arCabecalho );
        $oLista->setCorpo( $rs, $arParamCol );
        $oLista->show();

        die();
    }

}elseif($_REQUEST["act"] == "visualizar"){
    $mVinculoCeu->setVinculoSession($_REQUEST["vceid"]);

    $menu 		= new Menu($_REQUEST['mnuid']);
    $dadosMenu	= $menu->getDados();

	$link = $dadosMenu['mnulink'] != "" ? $dadosMenu['mnulink'] : "?modulo=principal/ceus/incluir&acao=A";
    header("Location: {$link}");
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

monta_titulo( $titulo_modulo, '' );
?>

<form action="" method="post" name="formulario">
    <input type="hidden" name="act" />
    <input type="hidden" name="vceid" />
    <input type="hidden" name="mnuid" />
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Identificador:
            </td>
            <td>
                <?= campo_texto("vceid_pesquisa", 'N', "S", "", "10", "7", "#######", ""); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                UF:
            </td>
            <td>
                <?php
    	            $dados  = $mCeu->getSqlUfCeus("est.estuf as codigo, est.estuf as descricao", array(), "descricao");
    	            $db->monta_combo("estuf", $dados, 'S', 'Selecione...', 'filtraMunicipio', '', '', '100', 'N', 'estuf');
    	        ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Município:
            </td>
            <td>
                <span id="spanMunicipio">
    	            <?php
    	                if( $estuf ){
    	                    $sql = $mCeu->getSqlMunicipioCeus("mun.muncod AS codigo, mun.mundescricao AS descricao", array("mun.estuf = '{$estuf}'"), "descricao");
    	                    $habilitado_municipio = 'S';
    	                }else{
    	                    $sql        = array(array('codigo' => 0, 'descricao' => 'Escolha a UF'));
    	                    $habilitado_municipio = 'N';
    	                }

    	                $db->monta_combo("muncod", $sql, $habilitado_municipio, 'Selecione...', 'filtraCeu', '', '', 250, 'N', "muncod");
    	            ?>
    	        </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                CEU:
            </td>
            <td>
                <span id="spanCeu">
    	            <?php
    	                if( $muncod ){
    	                    $habilitado_ceu = 'S';
                            $dadosCeu = $mCeu->recuperarTodos("ceu.ceuid as codigo, ceu.ceucodigo || ' - ' || ceu.ceunome as descricao", array("ceu.ceustatus = 'A'", "mun.muncod = '{$muncod}'"), 2);
    	                }else{
    	                    $dadosCeu       = array(array('codigo' => 0, 'descricao' => 'Escolha o Município'));
    	                    $habilitado_ceu = 'N';
    	                }

    	                $db->monta_combo("ceuid", $dadosCeu, $habilitado_ceu, 'Selecione...', '', '', '', 250, 'N', "ceuid");
    	            ?>
    	        </span>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:15%;">
                Situação do workflow:
            </td>
            <td>
                <?php
                $sql  = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE esdstatus= 'A' AND tpdid ='".WF_QUESTIONARIO."' ORDER BY descricao";
                $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', '250', 'N', "esdid");
                ?>
            </td>
        </tr>
	    <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Aba:
            </td>
            <td>
                <?php
                    $mConfiguracaoLiberacao	= new ConfiguracaoLiberacao();
                    $dadosMenus  			= $mConfiguracaoLiberacao->getMenusCeu();
                    $db->monta_combo("mnuidfiltro", $dadosMenus, 'S', 'Selecione...', '', '', '', '250', 'N', 'mnuidfiltro');
                ?>
            </td>
        </tr>
<!-- 	    <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                 Situação da aba:
             </td>
            <td>
				<input type="radio" name="situacaoaba" id="situacaoaba-t" <?php //echo ($situacaoaba == 't' ) ? ' checked="checked" ' : "" ?> value="t" /> <label for="situacaoaba-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" name="situacaoaba" id="situacaoaba-f" <?php //echo $situacaoaba == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="situacaoaba-f" style="cursor:pointer;"> Não</label>
				<input type="radio" name="situacaoaba" id="situacaoaba-a" <?php// echo ($situacaoaba == 'a' || empty($situacaoaba)) ? ' checked="checked" ' : "" ?> value="a" /> <label for="situacaoaba-a" style="cursor:pointer;"> Todos</label>
             </td>
         </tr> -->
        <tr style="background-color: #cccccc">
            <td align="right" style="width:25%;">
                &nbsp;
            </td>
            <td>
                <input type="submit" name="btnPesquisar" value="Pesquisar" onclick="pesquisar();" />
                <input type="button" name="btnExportarXls" value="Exportar XLS" onclick="exportarXls();" />
                <input type="button" name="btnLimpar" value="Limpar" onclick="limpar();"/>
            </td>
        </tr>
    </table>
</form>

<?php

    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ação", "Identificador", "Contrato", "Modelo", "UF", "Município","Código IBGE", "Aba","% de Preenchimento", "Situação do workflow","Quem fez a ultima atualização", "Quando foi a ultima ação", "Situação da aba");


    $arParamCol[0]  = array('type' => Lista::TYPESTRING);
    
    $arParamCol[1]  = array('type' => Lista::TYPESTRING);

    $arParamCol[7]  = array('html' => '<php>
												if("{qrpid}" != "" && "{queid}" != "" && "{vceid}" != ""){
										    		return barraProgresso("{qrppreenchimento}", 100, true,true,false);
												}else{
													if("{mnuid}" == "' . MENU_CEU_DADOS_INICIAIS . '"){
														if("{divid}" != ""){
															return barraProgresso(1, 1, true,true,false);
														}else{
															return barraProgresso(0, 1, true,true,false);
														}
													}else{
														return "-";
													}
												}
									   </php>',
							'align' => 'center');

	$arParamCol[10]  = array('type' => Lista::TYPEDATE);
	$arParamCol[11]  = array('align' => 'center');

    $acao = '<center>
    			<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Visualizar Aba" onclick="visualizar( {mnuid}, {vceid} )">&nbsp;
             </center>';
    $rs = $mVinculoCeu->listarAbasVinculoCeu( $arWhere, true );
    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs, $arParamCol );
    $oLista->setAcao( $acao );
    $oLista->show();
?>



<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script language="javascript" type="text/javascript" >

    function filtraMunicipio(estuf, muncod){
        $('#spanMunicipio').html('carregando...');
        $('#spanMunicipio').load('?modulo=principal/ceus/listarAbasVinculoCeu&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function filtraCeu(muncod, ceuid){
    	ceuid = ceuid ? ceuid : "";
        $('#spanCeu').html('carregando...');
        $('#spanCeu').load('?modulo=principal/ceus/listarAbasVinculoCeu&acao=A',
                          {'act'    : 'filtraCeu',
                           'ceuid'  : ceuid,
                           'muncod' : muncod});
    }

    function limpar(){
        window.location = window.location;
    }

    function pesquisar(){
        window.formulario.act.value = "pesquisar";
        document.formulario.submit();
    }

    function exportarXls(){
        window.formulario.act.value = "exportar_xls";
        document.formulario.submit();
    }

    function visualizar(mnuid, vceid){
        document.formulario.act.value = 'visualizar';
        document.formulario.mnuid.value = mnuid;
        document.formulario.vceid.value = vceid;

        document.formulario.submit();
    }


</script>