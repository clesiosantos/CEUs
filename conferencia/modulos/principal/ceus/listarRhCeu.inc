<?php
$mVinculoCeu = new VinculoCeu();
$mVinculoCeuQuestionario = new VinculoCeuQuestionario();
$mVinculoCeu->limparVinculoSession();

$mCeu = new Ceu();

$arWhere   = array();
$arWhere[] = "(ceu.ceuid IN (SELECT ceuid FROM conferencia.vinculoceu vce WHERE vce.vcestatus = 'A' AND (".getWhereResponsabilidadeVinculoCeu() . ")))";
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
    if($_REQUEST["ceucodigo"]){
        $arWhere[] = "ceu.ceucodigo = '{$_REQUEST["ceucodigo"]}'";
    }
    if($_REQUEST["espaco"]){
        $arWhere[] = "d.resposta[1] = '{$_REQUEST["espaco"]}'";
    }
    if($_REQUEST["nome"]){
        $arWhere[] = "d.resposta[2] ILIKE '%{$_REQUEST["nome"]}%'";
    }
    if($_REQUEST["sexo"]){
        $arWhere[] = "d.resposta[3] = '{$_REQUEST["sexo"]}'";
    }
    if($_REQUEST["rg"]){
        $arWhere[] = "d.resposta[4] ILIKE '{$_REQUEST["rg"]}%'";
    }
    if($_REQUEST["escolaridade"]){
    	$arWhere[] = "d.resposta[5] = '{$_REQUEST["escolaridade"]}'";
    }
    if($_REQUEST["formacao"]){
    	$arWhere[] = "d.resposta[6] ILIKE '{$_REQUEST["formacao"]}%'";
    }
    if($_REQUEST["tipovinculo"]){
    	$arWhere[] = "d.resposta[7] = '{$_REQUEST["tipovinculo"]}'";
    }
    if($_REQUEST["funcao"]){
    	$arWhere[] = "d.resposta[8] = '{$_REQUEST["funcao"]}'";
    }
    if($_REQUEST["cargahoraria"]){
    	$arWhere[] = "d.resposta[9] = '{$_REQUEST["cargahoraria"]}'";
    }

    extract($_POST);

    if($_POST["act"] == "exportar_xls"){
        header( 'Content-Type: application/vnd.ms-excel' );
        header( 'Expires: 0' );
        header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
        header( 'Content-Disposition: attachment; filename="vinculos_ceu_'.date('Ymd').'.xls"' );

        $arConfig = array("style"           => "width:100%;",
                          "totalLinha"      => false,
                          "totalRegistro"   => false);
        $arCabecalho = array("Contrato", "Tipo", "UF", "Município", "Local de Trabalho", "Nome Completo", "Sexo", "RG", "Escolaridade", "Formação", "Tipo de vínculo", "Função", "Carga horária");

        $arParamCol[0]  = array('type' => Lista::TYPESTRING);
        $arParamCol[5]  = array('type' => Lista::TYPESTRING);

        $rs = $mVinculoCeu->listarRhCeus( $arWhere, false);
        $oLista = new Lista($arConfig);
        $oLista->setCabecalho( $arCabecalho );
        $oLista->setCorpo( $rs, $arParamCol );
        $oLista->show();
        die();
    }
}elseif($_REQUEST["act"] == "visualizar"){
    // Primeiro busca um vínculo ativo, se não encontrar busca qualquer um
    $vceid = $mVinculoCeu->recuperarUm("vceid", array("vcestatus = 'A'", "ceuid = {$_REQUEST["ceuid_pesquisa"]}", "vceativo IS TRUE"));
    if(!$vceid){
        $vceid = $mVinculoCeu->recuperarUm("vceid", array("vcestatus = 'A'", "ceuid = {$_REQUEST["ceuid_pesquisa"]}"));
    }

    $_SESSION['quetionario']['grpid_inicial'] = $_REQUEST['grpid'];

    $mVinculoCeu->setVinculoSession($vceid);
    header("Location: ?modulo=principal/ceus/rh&acao=A");
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

monta_titulo( $titulo_modulo, '' );
?>

<form action="" method="post" name="formulario">
    <input type="hidden" name="act" value="" />
    <input type="hidden" name="ceuid_pesquisa" value="" />
    <input type="hidden" name="grpid" value="" />

    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">
                Contrato:
            </td>
            <td>
                <?= campo_texto("ceucodigo", 'N', "S", "", "10", "8", "########", ""); ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                Sexo:
            </td>
            <td>
    	            <?php
    	            	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_SEXO);
    	                $db->monta_combo("sexo", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "sexo");
    	            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">
                UF:
            </td>
            <td>
                <?php
    	            $dados  = $mCeu->getSqlUfCeus("est.estuf as codigo, est.estuf as descricao", array(), "descricao");
    	            $db->monta_combo("estuf", $dados, 'S', 'Selecione...', 'filtraMunicipio', '', '', '100', 'N', 'estuf');
    	        ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                RG:
            </td>
            <td>
                <?= campo_texto("rg", 'N', "S", "", "45", "100", "", ""); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;" >
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
            <td class="SubTituloDireita" style="width: 15%;">
                Escolaridade:
            </td>
            <td>
    	            <?php
    	            	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_ESCOLARIDADE);
    	                $db->monta_combo("escolaridade", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "escolaridade");
    	            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">
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
            <td class="SubTituloDireita" style="width: 15%;">
                Formação:
            </td>
            <td>
                <?= campo_texto("formacao", 'N', "S", "", "45", "100", "", ""); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">
                Local de Trabalho:
            </td>
            <td>
    	        <?php
    	        	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_ESPACO);
    	            $db->monta_combo("espaco", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "espaco");
    	        ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                Tipo de Vínculo:
            </td>
            <td>
    	            <?php
    	            	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_TIPO_DE_VINCULO);
    	                $db->monta_combo("tipovinculo", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "tipovinculo");
    	            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;" >
                Nome Completo:
            </td>
            <td>
                <?= campo_texto("nome", 'N', "S", "", "45", "100", "", ""); ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                Carga Horária:
            </td>
            <td>
    	            <?php
    	            	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_CARGA_HORARIA);
    	                $db->monta_combo("cargahoraria", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "cargahoraria");
    	            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">
                Função:
            </td>
            <td>
    	            <?php
    	            	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(ITEM_PERGUNTA_RH_FUNCAO);
    	                $db->monta_combo("funcao", $dadosCombo, 'S', 'Selecione...', '', '', '', 250, 'N', "funcao");
    	            ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                &nbsp;
            </td>
            <td>
                &nbsp;
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td colspan="4" align="center">
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
    $arCabecalho = array("Ação", "Contrato", "Modelo", "UF", "Município", "Local de Trabalho", "Nome Completo", "Sexo", "RG", "Escolaridade", "Formação", "Tipo de vínculo", "Função", "Carga horária");

    $arParamCol[0]  = array('type' => Lista::TYPESTRING);
    $arParamCol[5]  = array('type' => Lista::TYPESTRING);

    $acao = '<center>
    			<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Visualizar Recurso Humano" onclick="visualizar( {ceuid}, {grpid} )">&nbsp;
             </center>';
    $rs = $mVinculoCeu->listarRhCeus( $arWhere, true );
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
        $('#spanMunicipio').load('?modulo=principal/ceus/listarRhCeu&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function filtraCeu(muncod, ceuid){
    	ceuid = ceuid ? ceuid : "";
        $('#spanCeu').html('carregando...');
        $('#spanCeu').load('?modulo=principal/ceus/listarRhCeu&acao=A',
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

    function visualizar(ceuid, grpid){
        document.formulario.act.value   = 'visualizar';
        document.formulario.ceuid_pesquisa.value = ceuid;
        document.formulario.grpid.value = grpid;

        document.formulario.submit();
    }
</script>