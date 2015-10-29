<?php
$cleid = $_REQUEST['cleid'];

$configuracaoLiberacaoExcecao = new ConfiguracaoLiberacaoExcecao($cleid); //instancia da modelo
$mLiberacaoExcecaoMenu = new LiberacaoExcecaoMenu();

//botao pesquisar
$where = array();
if($_POST['act'] == 'pesquisar'){


	if($_POST['mnuid'])
		$where[] = " mnu.mnuid = {$_POST['mnuid']} ";

	if($_POST['ceuid'])
		$where[] = " {$_POST['ceuid']} IN ((SELECT
												ceu.ceuid
											 FROM conferencia.ceu
											 INNER JOIN
											 	 conferencia.liberacaoexcecaomenu lem ON lem.ceuid = ceu.ceuid AND lemstatus = 'A'
											 WHERE
											 	 lem.cleid = cle.cleid
						))";

		extract($_POST);
}


//botão excluir
if ($_POST['act'] == 'excluir'){
	$cleid = $_REQUEST["cleid"];

	$configuracaoLiberacaoExcecao->carregarPorId($cleid);
	$dado = array('clestatus' => 'I');
	$configuracaoLiberacaoExcecao->popularDadosObjeto($dado);
	$configuracaoLiberacaoExcecao->salvar();

	$mLiberacaoExcecaoMenu->desabilitaPorConfiguracaoLiberacaoExcecao($cleid);

	$configuracaoLiberacaoExcecao->commit();
	$db->sucesso($_REQUEST['modulo']);
}


//monta o cabeçalho
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( $titulo_modulo, '' );
?>



<!-- ----------------------------------------------------- -->
<!-- inicio do formulario -->
<form method="POST" name="formulario" action="">
<input type="hidden" name="cleid" value="<?php echo $cleid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

     <tr>
	     <td class="SubTituloDireita">
	                 Funcionalidade:
	     </td>
	     <td>
	                <?php
	                    $mConfiguracaoLiberacao	= new ConfiguracaoLiberacao();
	                    $dadosMenus  			= $mConfiguracaoLiberacao->getMenusCeu();
	                    $db->monta_combo("mnuid", $dadosMenus, 'S', 'Selecione...', '', '', '', '325', 'N', 'mnuid');

	                ?>
	    </td>
    </tr>

     <tr>
	     <td class="SubTituloDireita">
                 CEU:
	     </td>
	     <td>
      			<?php
                    $mCeu	= new Ceu();
                    $dadosCeu = $mCeu->listar(array());
                    $db->monta_combo("ceuid", $dadosCeu, 'S', 'Selecione...', '', '', '', '325', 'N', 'ceuid');
                ?>
	     </td>
    </tr>


    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/apoio/listarConfiguracaoLiberacaoExcecao&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/apoio/incluirConfiguracaoLiberacaoExcecao&acao=A';" title="Clique para incluir um novo Uso">
				<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Exceção de Liberação</b>
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
    $arCabecalho = array("Ação", "Funcionalidade", "Data Limite", "CEU(s)", "Data de Cadastro", "Cadastrado por");


    $arParamCol[0]  =  array();
    $arParamCol[1]  = array('type' => Lista::TYPESTRING);

     $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {cleid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {cleid} )">
             </center>';



    $rs = $configuracaoLiberacaoExcecao->listar( $where, true );
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

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.cleid.value = cod;

        document.formulario.submit();
	}
}

function editar(cod){

 	window.location = '?modulo=sistema/apoio/incluirConfiguracaoLiberacaoExcecao&acao=A&cleid='+cod;
}


</script>

