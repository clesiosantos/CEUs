<?php
$colid = $_REQUEST['colid'];
$configuracaoLiberacao = new ConfiguracaoLiberacao($colid); //instanciando a modelo

if($_REQUEST["act"] == "salvar"){

	$arDados = array(
			'colid'        => $configuracaoLiberacao->getIdByMenu($_REQUEST["mnuid"]),
			'mnuid'        => $_REQUEST["mnuid"],
			'colliberado'  => $_REQUEST["colliberado"],
			'usucpf'  		=> $_SESSION["usucpf"],
			'colstatus'    => 'A'
	);
	$configuracaoLiberacao->popularDadosObjeto($arDados)->salvar();
	$configuracaoLiberacao->commit();

	 $db->sucesso("sistema/apoio/listarConfiguracaoLiberacao", "&colid={$_REQUEST["colid"]}");
}

if($colid){
	$dadosConfig = $configuracaoLiberacao->getDados();
	$mnuidConfig = $dadosConfig['mnuid'];
	$colliberado = $dadosConfig['colliberado'];
}

//incluindo o cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = (empty($colid) ? 'Incluir Configuração de Liberação' : 'Alterar Configuração de Liberação');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );

?>

<!-- Inicio do formulario da página -->

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
                    $db->monta_combo("mnuid", $dadosMenus, 'S', 'Selecione...', '', '', '', '325', 'S', 'mnuid', null, $mnuidConfig);
                ?>
            </td>
    <tr>

    <tr>
        <td align='right' class="SubTituloDireita">Liberado?</td>
        <td>
			<input type="radio" name="colliberado" id="colliberado-t" <?php echo ($colliberado == 't' || empty($colliberado)) ? ' checked="checked" ' : "" ?> value="t" /> <label for="colliberado-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="colliberado" id="colliberado-f" <?php echo $colliberado == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="colliberado-f" style="cursor:pointer;"> Não</label>

			<?= obrigatorio(); ?>
		</td>
    </tr>





    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>

        <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
        <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/apoio/listarConfiguracaoLiberacao&acao=A'"/>
    </td>
    </tr>
	<tr>

</table>
</form>


<!-- funcoes javascript para o botao salvar -->

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
	if (!validaBranco(document.formulario.mnuid, 'Funcionalidade')) return;
    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}


</script>



