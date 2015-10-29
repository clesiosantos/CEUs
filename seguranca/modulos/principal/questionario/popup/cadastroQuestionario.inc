<?php
include_once APPRAIZ.'includes/classes/modelo/questionario/QQuestionario.class.inc';
include_once APPRAIZ.'includes/classes/modelo/questionario/QQuestionarioResposta.class.inc';

$obQuestionario 		= new QQuestionario( $_REQUEST['queid'] );
$obQuestionarioresposta = new QQuestionarioresposta();

if($_POST['submeter'] == 'salvar'){

    //retirando as barras que são adicionadas pelo 'magic_quotes_gpc'
    $_POST['quetitulo']     = stripcslashes( $_POST['quetitulo'] );
    $_POST['quedescricao']  = stripcslashes( $_POST['quedescricao'] );

    //adicionando o 'htmlspecialchars' nos campos
    $_POST['quetitulo']     = htmlspecialchars( $_POST['quetitulo'], ENT_QUOTES);
    $_POST['quedescricao']  = htmlspecialchars( $_POST['quedescricao'], ENT_QUOTES);

	$arCampos = array( "queid", "sisid", "quetitulo", "quedescricao", "quedtinicio", "quedtfim" );

	$arDados = $_POST;
	$arDados['quedtinicio'] = formata_data_sql( $arDados['quedtinicio'] );
	$arDados['quedtfim'] 	= formata_data_sql( $arDados['quedtfim'] );
	$arDados['sisid'] 	    = $arDados['sisid'];

	$obQuestionario->popularObjeto( $arCampos, $arDados );

	if( $queid = $obQuestionario->salvar() ){
		$obQuestionario->commit();

		/*    //SALVA O QUEID NA TABELA QUESTIONARIORESPOSTA!!
		 *
		$arCampos = array( "queid", "qrptitulo", "qrpdata" );
		$arDados = array( "queid" => $queid, "qrptitulo" => 'teste', "qrpdata" => 'now()' );

		$obQuestionarioresposta->popularObjeto( $arCampos, $arDados );

		$obQuestionarioresposta->salvar();
		$obQuestionarioresposta->commit();
		*/


//		echo "<script>
//				alert('".("Operação realizada com sucesso", true)."');
//				window.close();
////				window.opener.location.replace(window.opener.location);
//				window.opener.location.replace('?modulo=sistema/questionario/arvore&acao=A&sisidFiltro={$_GET['sisid']}&queidFiltro={$queid}');
//			  </script>";

			die ("
				<script type=\"text/javascript\" src=\"/includes/funcoes.js\"></script>
		  		<script type='text/javascript' src='../includes/JQuery/jquery-1.4.2.js'></script>
		  		<script>
		 			alert('".('Operação realizada com sucesso!')."');
// 		  			window.opener.filtraQuestionario( ".$_SESSION['sisid']." );
//		  			window.opener.$('#sisfiltro').change();
// 		  			window.opener.$('#quefiltro').val('".$queid."');
// 		  			window.opener.$('#quefiltro').change();
// 					window.resizeTo (0, 0);
// 					window.opener.focus();
// 					window.blur();
// 					window.opener.montarA(".$queid.", window.opener.$('#sisfiltro').val());
			        window.opener.location.href = window.opener.location.href;
					window.close();
		  		</script>");

	}else{
		$obQuestionario->rollback();
		$obQuestionario->insucesso(('Falha na operação'));
	}
}
// Extrai dados para Atualização
$quetitulo 	  = $obQuestionario->quetitulo;
$quedescricao = $obQuestionario->quedescricao;
list($quedtinicio) = explode(' ', $obQuestionario->quedtinicio);
list($quedtfim) = explode(' ', $obQuestionario->quedtfim);
$sisid		  = $obQuestionario->sisid ? $obQuestionario->sisid : $_GET['sisid'];

monta_titulo( ('Cadastro de Questionários'), obrigatorio().' '.('Indica Campo Obrigatório'));
?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
		<script type="text/javascript">
		function salvarQuestionario(){
// 			var camposObri 		= "#quetitulo#quedescricao#quedtinicio#quedtfim";
// 			var tiposCamposObri	= '#texto#textarea#data#data';

// 			if(!validaForm('formulario',camposObri,tiposCamposObri,false))
// 					return false;

            if($('#quetitulo').val() == ''){
                alert("<?= ("Obrigatório: ") . ' ' . ("Título") ?>");
                return false;
            }

            if($('#quedescricao').val() == ''){
                alert("<?= ("Obrigatório: ")  . ' ' . ("Descrição") ?>");
                return false;
            }

            if($('#quedtinicio').val() == '' || $('#quedtfim').val() == ''){
                alert("<?= ("Obrigatório: ")  . ' ' . ("Período") ?>");
                return false;
            }

			document.getElementById('submeter').value = 'salvar';
			document.getElementById('formulario').submit();
		}
		</script>
	</head>
<body topmargin="0" marginwidth="0">
<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="submeter" name="submeter" value="">
<input type="hidden" id="queid" name="queid" value="<?=$_REQUEST['queid']; ?>">
<input type="hidden" id="sisid" name="sisid" value="<?=$sisid; ?>">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Título:"); ?></b></td>
		<td><?=campo_texto('quetitulo', 'S', 'S', '', 80, 100, '', '', '', '', 0, 'id="quetitulo"' ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b><?= ("Descrição:"); ?></b></td>
		<td><?=campo_textarea('quedescricao', 'S', 'S', ('Descrição do quetionário'), 83, 10, 2000, '', '', '', '', ''); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b><?= ("Período:"); ?></b></td>
		<td><?= ("De"); ?> <?=campo_data2('quedtinicio', 'N','S',('Data Inicial'),'S','',''); ?> <?= ("a"); ?> <?=campo_data2('quedtfim', 'N','S',('Data Final'),'S','',''); ?> (dd/mm/yyyy) </td>
	</tr>
	<tr>
		<th colspan="2">
			<input type="button" value="<?= ("Salvar"); ?>" name="btnSalvar" id="btnSalvar" onclick="salvarQuestionario();">&nbsp;
			<input type="button" value="<?= ("Voltar"); ?>" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
</body>
</html>