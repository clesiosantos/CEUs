<?php
$ceuid = $_REQUEST['ceuid'];
$mCeu = new Ceu($ceuid);

if($_REQUEST['act'] == 'popUpEnderecos'){
	include 'popUpEnderecos.inc';
	die();
}

$arWhere = array();
//ajax que carrega os municípios
if( $_REQUEST['act'] == 'filtraMunicipio' ){
	if( $_REQUEST['estuf'] ){
		$sql = "SELECT
		muncod AS codigo,
		mundescricao AS descricao
		FROM
		territorios.municipio
		WHERE
		estuf = '{$_REQUEST['estuf']}'
		ORDER BY
		2";
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
    		$mCeu           = new Ceu();
    		$dadosCeu = $mCeu->recuperarTodos("ceuid as codigo, ceucodigo || ' - ' || ceunome as descricao", array("ceustatus = 'A'", "muncod = '{$muncod}'"), 2);
    }else{
    	$dadosCeu       = array(array('codigo' => 0, 'descricao' => 'Escolha o Município'));
    	$habilitado_ceu = 'N';
    }

    $db->monta_combo("ceuid", $dadosCeu, $habilitado_ceu, 'Selecione...', '', '', '', 250, 'N', "ceuid");
    die();
}elseif($_REQUEST["act"] == "alterarvinculo"){
	$mVinculoCeu = new VinculoCeu();
    $mVinculoCeu->setVinculoSession($_REQUEST["vceid"]);
    header("Location: ?modulo=principal/ceus/incluir&acao=A");
}



//exluir um uso
if ($_POST['act'] == 'excluir'){
	$dado = array('ceustatus' => 'I');
	$mCeu->popularDadosObjeto($dado);
	$mCeu->salvar();
	$mCeu->commit();

	$db->sucesso($_REQUEST['modulo']);
}



//botao pesquisar
$where = array();
if($_POST['act'] == 'pesquisar'){
	if($_POST['ceucodigo'])
		$where[] = " ceu.ceucodigo ILIKE '%{$_POST['ceucodigo']}%' ";
	if($_POST['ceunome'])
		$where[] = " ceu.ceunome ILIKE '%{$_POST['ceunome']}%' ";

	if($_POST['muncod'])
		$where[] = " mun.muncod = '{$_POST['muncod']}' ";
	if($_POST['estuf'])
		$where[] = " mun.estuf = '{$_POST['estuf']}' ";
	if($_POST['enderrado'] == 't' || $_POST['enderrado'] == 'f'){

		if($_POST['enderrado'] == 't'){
			$where[] = " (SELECT
    						count(vce.*)
    					 FROM
    					 	conferencia.vinculoceu vce
    					 JOIN
    					 	conferencia.dadosiniciaisvinculoceu div ON div.vceid = vce.vceid AND
    					 											   div.divstatus = 'A' AND
    					 											   div.edcid IS NOT NULL
    					 WHERE
    					 	vce.ceuid = ceu.ceuid AND
    					 	vce.vcestatus = 'A' ) > 0 ";
		}else{
			$where[] = " (SELECT
    						count(vce.*)
    					 FROM
    					 	conferencia.vinculoceu vce
    					 JOIN
    					 	conferencia.dadosiniciaisvinculoceu div ON div.vceid = vce.vceid AND
    					 											   div.divstatus = 'A' AND
    					 											   div.edcid IS NOT NULL
    					 WHERE
    					 	vce.ceuid = ceu.ceuid AND
    					 	vce.vcestatus = 'A' ) = 0 ";
		}

	}


	extract($_POST);
}




include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( $titulo_modulo, '' );
?>



<!-- ----------------------------------------------------- -->
<!-- inicio do formulario -->
<form method="POST" name="formulario" action="">
<input type="hidden" name="ceuid" value="<?php echo $ceuid ?>"/>
<input type="hidden" name="vceid" id="vceid" value=""/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td align='right' class="SubTituloDireita" width="30%">Contrato:</td>
        <td>
        <?
            echo campo_texto('ceucodigo','N','','',10,8,'########','');
        ?>
        </td>
    </tr>

     <tr>
        <td align='right' class="SubTituloDireita" width="30%">Tipo:</td>
        <td>
        <?
            echo campo_texto('ceunome','N','','',46,255,'','');
        ?>
        </td>
    </tr>

    <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                UF:
            </td>
            <td>
                <?php
    	            $estado = new Estado();
    	            $dados  = $estado->recuperarTodos("estuf as codigo, estuf as descricao", null, "descricao");
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
    	                    $sql = "SELECT
    	                                muncod AS codigo,
    	                                mundescricao AS descricao
    	                            FROM
    	                                territorios.municipio
    	                            WHERE
    	                                estuf = '{$estuf}'
    	                            ORDER BY
    	                                2";
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
                Vínculo com endereço errado:
            </td>
            <td>
				<input type="radio" name="enderrado" id="enderrado-t" <?php echo ($enderrado == 't' ) ? ' checked="checked" ' : "" ?> value="t" /> <label for="enderrado-t" style="cursor:pointer;"> Sim</label>
				<input type="radio" name="enderrado" id="enderrado-f" <?php echo $enderrado == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="enderrado-f" style="cursor:pointer;"> Não</label>
				<input type="radio" name="enderrado" id="enderrado-a" <?php echo ($enderrado == 'a' || empty($enderrado)) ? ' checked="checked" ' : "" ?> value="a" /> <label for="enderrado-a" style="cursor:pointer;"> Todos</label>
            </td>
        </tr>



    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/apoio/listarCeu&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/apoio/incluirCeu&acao=A';" title="Clique para incluir um novo Uso">
				<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir novo CEU</b>
			</a>
		</td>
	</tr>
</table>
</form>





<!-- ----------------------------------------------------- -->

<?php
	//listagem

    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ação", "Contrato", "Tipo", "Descrição",  "Município", "UF","Vínculo com endereço errado", "Data de Cadastro", "Cadastrado por");


    $arParamCol[0]  =  array();
    $arParamCol[1]  = array('type' => Lista::TYPESTRING);

    $arParamCol[5]  = array('type' => Lista::TYPESTRING,
                            'align'=> 'center',
                            'html' => '<php>
											$retorno = "-";
											if( "{vinculoerrado}" > 0 ){
												$retorno = "<label onclick=\'exibePopUpVinculos({ceuid})\' title=\'Clique aqui para exibir os registros com endereços divergentes\' style=\'cursor:pointer; font-weight:bold;\'>Sim</label>";
											}else{
												$retorno = "<label>Não</label>";
											}

											return $retorno;
									   </php>');


    $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {ceuid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {ceuid} )">
             </center>';


    $rs = $mCeu->listar( $where, true );
    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs, $arParamCol );
    $oLista->setAcao( $acao );
    $oLista->show();
?>





<!-- ----------------------------------------------------- -->
<!-- funcoes javascript -->

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >


function pesquisar(cod){
    document.formulario.act.value = 'pesquisar';
    document.formulario.submit();
}

function exibePopUpVinculos(cod){
	var popup = window.open('?modulo=sistema/apoio/listarCeu&acao=A&act=popUpEnderecos&ceuid=' + cod , 'EnderecosDivergentes','height=600,width=800,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no');
	popup.focus();
}

function filtraMunicipio(estuf, muncod){
    $('#spanMunicipio').html('carregando...');
    $('#spanMunicipio').load('?modulo=principal/ceus/listar&acao=A',
                      {'act'    : 'filtraMunicipio',
                       'estuf'  : estuf,
                       'muncod' : muncod});
}

function filtraCeu(muncod, ceuid){
    $('#spanCeu').html('carregando...');
    $('#spanCeu').load('?modulo=principal/ceus/listar&acao=A',
                      {'act'    : 'filtraCeu',
                       'ceuid'  : ceuid,
                       'muncod' : muncod});
}



function editar(cod){
 	window.location = '?modulo=sistema/apoio/incluirCeu&acao=A&ceuid='+cod;
}


function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.ceuid.value = cod;

        document.formulario.submit();
	}
}


</script>
