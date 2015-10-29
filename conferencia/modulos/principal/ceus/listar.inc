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
    if($_REQUEST["ceucodigo"]){
        $arWhere[] = "ceu.ceucodigo = '{$_REQUEST["ceucodigo"]}'";
    }
    if($_REQUEST["ceuid"]){
        $arWhere[] = "ceu.ceuid = '{$_REQUEST["ceuid"]}'";
    }
    if($_REQUEST["vceid_pesquisa"]){
        $arWhere[] = "vce.vceid = '{$_REQUEST["vceid_pesquisa"]}'";
    }
    if($_REQUEST["esdid"]){
        $arWhere[] = "esd.esdid = '{$_REQUEST["esdid"]}'";
    }
    if($_REQUEST["vceativo"]){
        if($_REQUEST["vceativo"] == 't'){
            $arWhere[] = "vce.vceativo IS TRUE";
        }elseif($_REQUEST["vceativo"] == 'f'){
            $arWhere[] = "vce.vceativo IS FALSE";
        }
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
        $arCabecalho = array("Identificador", "Contrato", "Modelo", "UF", "Município", "Código IBGE", "Situação", "Possui alteração no endereço?","Ativo", "Cadastro por", "Data de Cadastro");

        $arParamCol[0]  = array('type' => Lista::TYPESTRING);
        $arParamCol[1]  = array('type' => Lista::TYPESTRING);

        $arWhere[] = getWhereResponsabilidadeVinculoCeu();

        $colunas = "vce.vceid,
			    	ceu.ceucodigo,
                    ceu.ceunome AS ceunome,
			    	mun.estuf,
			    	mun.mundescricao,
			    	mun.muncod,
			    	esd.esddsc,
			    	CASE
			    	    WHEN div.edcid IS NULL THEN
		    	            'Não'
		    	        ELSE
			    	        'Sim'
    	            END AS enderecoalterado,
			    	CASE
			    	    WHEN vceativo IS TRUE THEN
		    	            'Sim'
		    	        ELSE
			    	        'Não'
    	            END AS ativo,
			    	usu.usunome,
			    	TO_CHAR(vce.vcedtcadastro, 'DD/MM/YYYY as HH:MI') AS vcedtcadastro";

        $rs = $mVinculoCeu->listar( $arWhere, false, $colunas );
        $oLista = new Lista($arConfig);
        $oLista->setCabecalho( $arCabecalho );
        $oLista->setCorpo( $rs, $arParamCol );
        $oLista->show();


        die();
    }
}elseif($_REQUEST["act"] == "excluir"){
	$mVinculoCeu->carregarPorId($_REQUEST["vceid"]);
	$mVinculoCeu->vcestatus = 'I';
	$mVinculoCeu->salvar();
	$mVinculoCeu->commit();

	$db->sucesso($_REQUEST['modulo']);
}elseif($_REQUEST["act"] == "alterar"){
    $mVinculoCeu->setVinculoSession($_REQUEST["vceid"]);
    header("Location: ?modulo=principal/ceus/incluir&acao=A");
}elseif($_REQUEST["act"] == "listar_atividades"){
    $mVinculoCeu->setVinculoSession($_REQUEST["vceid"]);
    header("Location: ?modulo=principal/ceus/listarAtividades&acao=A");
}elseif($_REQUEST["act"] == "abrir_calendario"){
    header("Location: ?modulo=principal/ceus/calendario&acao=A&vceid=" . $_REQUEST["vceid"] );
}
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( $titulo_modulo, '' );

?>
<form action="" method="post" name="formulario">
    <input type="hidden" name="act" />
    <input type="hidden" name="vceid" />
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <?php if(!possui_perfil(CONFERENCIA_PERFIL_CADASTRO) || $db->testa_superuser()): ?>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Identificador:
            </td>
            <td>
                <?= campo_texto("vceid_pesquisa", 'N', "S", "", "10", "7", "#######", ""); ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita">
                Nº Contrato CEU:
            </td>
            <td>
                <?= campo_texto('ceucodigo','N','','',10,8,'########',''); ?>
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
                Situação:
            </td>
            <td>
                <?php
                $sql  = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE esdstatus= 'A' AND tpdid ='".WF_PRECADASTRO_CONFERENCIA."' ORDER BY descricao";
                $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', '250', 'N', "esdid");
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Ativo:
            </td>
            <td>
                <input type="radio" name="vceativo" id="vceativo-t" <?= ($vceativo == 't' ? ' checked="checked" ' : "")  ?> value="t" />
				<label for="vceativo-t" style="cursor: pointer;"> Sim</label>
                &nbsp;
				<input type="radio" name="vceativo" id="vceativo-f" <?= ($vceativo == 'f' ? ' checked="checked" ' : "") ?> value="f" />
				<label for="vceativo-f" style="cursor: pointer;"> Não</label>
                &nbsp;
				<input type="radio" name="vceativo" id="vceativo-a" <?= ($vceativo == 'a'  || empty($vceativo) ? ' checked="checked" ' : "") ?> value="a" />
				<label for="vceativo-a" style="cursor: pointer;"> Todos</label>
            </td>
        </tr>
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
        <?php endif; ?>
        <tr>
            <td colspan="2" style="text-align: left;">
                 <a style="cursor: pointer;" onclick="javascript:document.location='?modulo=principal/ceus/incluir&acao=A';" title="Clique para vincular um CEU">
    			     <img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Vincular</b>
    			 </a>
            </td>
        </tr>
    </table>
</form>

<?php
    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ação", "Identificador", "Contrato", "Modelo", "UF", "Município", "Código IBGE", "Situação", "Possui alteração no endereço?","Ativo", "Cadastro por", "Data de Cadastro");


    $arParamCol[0]  = array('type' => Lista::TYPESTRING);
    $arParamCol[1]  = array('type' => Lista::TYPESTRING);

    $acao = '<div style="text-align: center; width: 80px; margin: auto;">
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {vceid} )">
            	<img src="/imagens/excluir.gif" style="cursor:pointer" title="Excluir" onclick="excluir({vceid});">
    			<!-- <img src=\'/imagens/ico_config.gif\' style="cursor:pointer" title="Atividades" onclick="listarAtividades( {vceid} )">
    			<img src=\'/imagens/calendario.gif\' style="cursor:pointer" title="Calendário" onclick="abrirCalendario( {vceid} )"> -->
             </div>';

    $arWhere[] = getWhereResponsabilidadeVinculoCeu();

    $rs = $mVinculoCeu->listar( $arWhere, true );
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
        $('#spanMunicipio').load('?modulo=principal/ceus/listar&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function filtraCeu(muncod, ceuid){
    	ceuid = ceuid ? ceuid : "";
        $('#spanCeu').html('carregando...');
        $('#spanCeu').load('?modulo=principal/ceus/listar&acao=A',
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

    function excluir(cod){
    	if ( confirm('Tem certeza que deseja apagar este registro?') ){
            document.formulario.act.value = 'excluir';
            document.formulario.vceid.value = cod;

            document.formulario.submit();
    	}
	}

    function editar(cod){
        document.formulario.act.value = 'alterar';
        document.formulario.vceid.value = cod;

        document.formulario.submit();
    }

    function listarAtividades(cod){
        document.formulario.act.value = 'listar_atividades';
        document.formulario.vceid.value = cod;

        document.formulario.submit();
    }
    
    function abrirCalendario(cod){
        document.formulario.act.value = 'abrir_calendario';
        document.formulario.vceid.value = cod;

        document.formulario.submit();
    }
</script>