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
    if($_REQUEST["tipo"]){
        $arWhere[] = "d.resposta[1] ILIKE '%{$_REQUEST["tipo"]}%'";
    }
    if($_REQUEST["nome"]){
        $arWhere[] = "d.resposta[3] ILIKE '%{$_REQUEST["nome"]}%'";
    }
    if($_REQUEST["titularidade"]){
        $arWhere[] = "d.resposta[2] ILIKE '%{$_REQUEST["titularidade"]}%'";
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
        $arCabecalho = array("Contrato", "Modelo", "UF", "Município", "Tipo", "Titularidade", "Nome", "DDD", "Telefone", "Email");

        $arParamCol[0]  = array('type' => Lista::TYPESTRING);
        $arParamCol[1]  = array('type' => Lista::TYPESTRING);
        $arParamCol[7]  = array('type' => Lista::TYPESTRING);

        $rs = $mVinculoCeu->listarMembrosGGCeus( $arWhere, false );
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
    header("Location: ?modulo=principal/ceus/uglGG&acao=A");
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
                UF:
            </td>
            <td>
                <?php
    	            $dados  = $mCeu->getSqlUfCeus("est.estuf as codigo, est.estuf as descricao", array(), "descricao");
    	            $db->monta_combo("estuf", $dados, 'S', 'Selecione...', 'filtraMunicipio', '', '', '100', 'N', 'estuf');
    	        ?>
            </td>
            <td class="SubTituloDireita" style="width: 15%;">
                Tipo:
            </td>
            <td>
    	        <?php
    	        	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(PERGUNTA_UGL_GG_GRP_MEMBROS_GG_TIPO_DE_MEMBRO);
    	            $db->monta_combo("tipo", $dadosCombo, 'S', 'Selecione...', '', '', '', '300', 'N', "tipo");
    	        ?>
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
                Titularidade:
            </td>
            <td>
    	        <?php
    	        	$dadosCombo = $mVinculoCeuQuestionario->comboItemPerguntaRhPorPergunta(PERGUNTA_UGL_GG_GRP_MEMBROS_GG_TITULARIDADE);
    	            $db->monta_combo("titularidade", $dadosCombo, 'S', 'Selecione...', '', '', '', '300', 'N', "titularidade");
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
            <td class="SubTituloDireita" style="width: 15%;" >
                Nome:
            </td>
            <td>
                <?= campo_texto("nome", 'N', "S", "", "45", "100", "", ""); ?>
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
    $arCabecalho = array("Ação", "Contrato", "Modelo", "UF", "Município", "Tipo", "Titularidade", "Nome", "DDD", "Telefone", "Email");


    $arParamCol[0]  = array('type' => Lista::TYPESTRING);
    $arParamCol[7]  = array('type' => Lista::TYPESTRING);

    $acao = '<center>
    			<img src=\'/imagens/consultar.gif\' style="cursor:pointer" title="Visualizar Membro do GG" onclick="visualizar( {ceuid}, {grpid} )">&nbsp;
             </center>';
    $rs = $mVinculoCeu->listarMembrosGGCeus( $arWhere, true );
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
        $('#spanMunicipio').load('?modulo=principal/ceus/listarMembrosUglCeu&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function filtraCeu(muncod, ceuid){
    	ceuid = ceuid ? ceuid : "";
        $('#spanCeu').html('carregando...');
        $('#spanCeu').load('?modulo=principal/ceus/listarMembrosUglCeu&acao=A',
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