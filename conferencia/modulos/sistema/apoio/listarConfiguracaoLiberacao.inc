<?php
$colid = $_REQUEST['colid'];

$configuracaoLiberacao = new ConfiguracaoLiberacao($colid); //instancia da modelo
// dbg($configuracaoLiberacao);

//botao pesquisar
$where = array();
if($_POST['act'] == 'pesquisar'){

	if($_POST['mnuid'])
		$where[] = " mun.mnuid = '{$_POST['mnuid']}' ";
	if($_POST['colliberado'])
		$where[] = " col.colliberado = '{$_POST['colliberado']}' ";

		extract($_POST);
}


//botão excluir
if ($_POST['act'] == 'excluir'){
	$configuracaoLiberacao->carregarPorId($_REQUEST["colid"]);
	$dado = array('colstatus' => 'I');
	$configuracaoLiberacao->popularDadosObjeto($dado);
	$configuracaoLiberacao->salvar();
	$configuracaoLiberacao->commit();

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
<input type="hidden" name="colid" value="<?php echo $colid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">


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
    <tr>
        <td align='right' class="SubTituloDireita">Liberado?</td>
        <td>
			<input type="radio" name="colliberado" id="colliberado-t" <?php echo ($colliberado == 't' || empty($colliberado)) ? ' checked="checked" ' : "" ?> value="t" /> <label for="colliberado-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="colliberado" id="colliberado-f" <?php echo $colliberado == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="colliberado-f" style="cursor:pointer;"> Não</label>
		</td>
    </tr>



    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/apoio/listarConfiguracaoLiberacao&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/apoio/incluirConfiguracaoLiberacao&acao=A';" title="Clique para incluir um novo Uso">
				<img src="../imagens/obras/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Configuração de Liberação</b>
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
    $arCabecalho = array("Ação", "Aba", "Liberado", "Data");


    $arParamCol[0]  =  array();
    $arParamCol[1]  = array('type' => Lista::TYPESTRING);

     $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {colid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {colid} )">
             </center>';



    $rs = $configuracaoLiberacao->listar( $where, true );
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




function editar(cod){


	window.location = '?modulo=sistema/apoio/incluirConfiguracaoLiberacao&acao=A&colid='+cod;
}


function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.colid.value = cod;
        document.formulario.submit();
	}
}





</script>


