<?php
$cleid = $_REQUEST['cleid'];

$mConfiguracaoLiberacaoExcecao = new ConfiguracaoLiberacaoExcecao ($cleid);
$mLiberacaoExcecaoMenu = new LiberacaoExcecaoMenu();
$mCeu = new Ceu();

if ($_REQUEST['act'] == 'salvar'){

	$dadosConfiguracaoLiberacaoExcecao = array(
	 	'cleid' => $_REQUEST['cleid'],
		'mnuid' => $_REQUEST['mnuid'],
		'cledatalimite' => $_REQUEST['cledatalimite'],
	);

	if(empty($_REQUEST['cleid'])){
		$dadosConfiguracaoLiberacaoExcecao['usucpf'] = $_SESSION['usucpf'];
	}

	$cleid = $mConfiguracaoLiberacaoExcecao->popularDadosObjeto($dadosConfiguracaoLiberacaoExcecao)->salvar();

	$mLiberacaoExcecaoMenu->desabilitaPorConfiguracaoLiberacaoExcecao($cleid);
	foreach ($_REQUEST['ceuid'] as $ceuid){
		$dadosLiberacaoExcecaoMenu = array(
			'lemid'     => $mLiberacaoExcecaoMenu->getIdByFks($cleid, $ceuid),
			'ceuid'     => $ceuid,
			'cleid'     => $cleid,
			'lemstatus'	=> 'A'
		);

		$mLiberacaoExcecaoMenu->popularDadosObjeto($dadosLiberacaoExcecaoMenu)->salvar();
	}
	$mConfiguracaoLiberacaoExcecao->commit();
	$db->sucesso("sistema/apoio/listarConfiguracaoLiberacaoExcecao");
}

// monta o cabeçalho
include APPRAIZ . "includes/cabecalho.inc";
echo "<br />";
extract($mConfiguracaoLiberacaoExcecao->getDados());
if($cleid){
	monta_titulo ( 'Alterar Exceção de Liberação', '' );
} else {
	monta_titulo ( $titulo_modulo, '' );
}
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form action="" method="post" enctype="multipart/form-data" name="formulario" onsubmit="return enviarFormulario();">
	<input type="hidden" name="act" />
	<input type="hidden" name="cleid" value="<?= $cleid; ?>" />
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<td class="SubTituloDireita">
                 Funcionalidade:
     	</td>
    	<td>
            <?php
                $mConfiguracaoLiberacao	= new ConfiguracaoLiberacao();
                $dadosMenus  			= $mConfiguracaoLiberacao->getMenusCeu();
                $db->monta_combo("mnuid", $dadosMenus, 'S', 'Selecione...', '', '', '', '325', 'S', 'mnuid');
            ?>
        </td>
		<tr>
			<td class="SubTituloDireita">Data Limite:</td>
			<td>
	 		    <?= campo_data2( 'cledatalimite','S', 'S', '', 'S' ); ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">CEU(s):</td>
			<td>
			<?php
			 $sqlCeu = $mCeu->listar(array(), false, true);
			 $nome = 'ceuid';
			 if($cleid){
			 	$$nome = $mLiberacaoExcecaoMenu->getCeusByConfiguracaoLiberacaoExcecao($cleid);
			 }else{
			 	$$nome = array();
			 }
	 		 echo  combo_popup( $nome, $sqlCeu, 'Selecione o(s) CEU(s)', '360x460') . obrigatorio();
			?>
            </td>
		</tr>

		<tr style="background-color: #cccccc">
			<td align="right" style="width: 25%;">&nbsp;</td>
			<td colspan="2">
				<input type="submit" name="btnGravar" value="Salvar" onsubmit="return enviarFormulario();" />
				<input type="button" name="Voltar" value="Voltar" onclick="cancelar()"/>
			</td>
		</tr>
	</table>
</form>
<script language="javascript" type="text/javascript" >

/**
 * Varre formulário em busca de campos que precisam de tratamento
 * especial. O campo do tipo 'combo_popup' tem seus itens todos
 * selecionados.
 *
 * @return void
 */
function prepara_formulario()
{
	var quantidade = document.forms.length;
	var quantidade_elementos = 0;
	var elemento = null;
	var j = 0;
	for ( var i = 0; i < quantidade; i++  )
	{
		quantidade_elementos = document.forms[i].elements.length;
		for ( j = 0; j < quantidade_elementos; j++ )
		{
			elemento = document.forms[i].elements[j];
			if ( elemento.getAttribute( 'tipo' ) == 'combo_popup' )
			{
				selectAllOptions( elemento );
			}
		}
	}
}

function enviarFormulario(){
	prepara_formulario();
	var d = document;
    var f = d.formulario;
    var msg = '';
	var countCeu = 0;

    $(" #ceuid option:selected" ).each(function() {
        if ($( this ).val() != ''){
			countCeu++;
        }
    });

	if (f.mnuid.value == '') {
        msg += '\n\tFuncionalidade';
    }

	if (f.cledatalimite.value == '') {
        msg += '\n\tData Limite';
    }

	if (countCeu == 0) {
        msg += '\n\tCEU(s)';
    }


    if (msg != '') {
        alert('Os campos listados são obrigatórios e devem ser preenchidos:\n' + msg);
        return false;
    }

    f.act.value = "salvar";
    return true;
}

function cancelar(){
        window.location = "?modulo=sistema/apoio/listarConfiguracaoLiberacaoExcecao&acao=A";
}


</script>