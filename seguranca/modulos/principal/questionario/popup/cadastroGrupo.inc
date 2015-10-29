<?php
include_once APPRAIZ.'includes/classes/modelo/questionario/QGrupo.class.inc';

$obGrupo = new QGrupo( $_REQUEST['grpid'] );

if( $_REQUEST['grpid'] && $obGrupo->grpid_pai ){
    $multiplosHabilitados = 'N';
}else{
    if($_REQUEST['grpid']){
        $multiplosHabilitados  = $obGrupo->verificaMultiploHabilitado( $_REQUEST['grpid'] );
    }else{
        $multiplosHabilitados  = 'S';
    }
}

if($_POST['submeter'] == 'salvar'){
    $arCamposNulo = array();

    $grpcondicaoOriginal = $obGrupo->grpcondicao;

    //retirando as barras que são adicionadas pelo 'magic_quotes_gpc'
    $_REQUEST['grptitulo'] = stripcslashes( $_REQUEST['grptitulo'] );
    $_REQUEST['grpajuda']  = stripcslashes( $_REQUEST['grpajuda'] );

    //adicionando o 'htmlspecialchars' nos campos
//     $_REQUEST['grptitulo'] = htmlspecialchars( $_REQUEST['grptitulo'], ENT_QUOTES);
//     $_REQUEST['grpajuda']  = htmlspecialchars( $_REQUEST['grpajuda'], ENT_QUOTES);

	$_REQUEST['queid_atual'] = $_REQUEST['queval'] ? $_REQUEST['queval'] : $_REQUEST['queid'] ;


    if( $_REQUEST['grpqtdmaximo'] == '' || $_REQUEST['grppermitemultiplo'] == '0' ){
        $_REQUEST['grpqtdmaximo'] = null;
        $arCamposNulo[]           = 'grpqtdmaximo';
    }

	if($_POST['queid'] <> ''){
		$arCampos = array( "grpid", "queid", "grptitulo", "grpstatus", "grpajuda", "queid_atual","grppermitemultiplo",'grpqtdmaximo', 'grpcondicao');
		$where = "queid = {$_POST['queid']}";
	} else if($_POST['gru_grpid'] <> ''){
		$arCampos = array( "grpid", "gru_grpid", "grptitulo", "grpstatus", "grpajuda", "queid_atual","grppermitemultiplo",'grpqtdmaximo', 'grpcondicao');
		$where = "gru_grpid = {$_POST['gru_grpid']}";
	} else if($_POST['itpid'] <> ''){
		$arCampos = array( "grpid", "itpid", "grptitulo", "grpstatus", "grpajuda", "queid_atual","grppermitemultiplo",'grpqtdmaximo', 'grpcondicao');
		$where = "itpid = {$_POST['itpid']}";
	}
	$obGrupo->popularObjeto( $arCampos );

	$grpid = $obGrupo->salvar(true, true, $arCamposNulo );
	if ( $obGrupo->grpid )
		$grpid = $obGrupo->grpid;

	$objQuestionario = new QQuestionario();
	$objQuestionario->ordenaNivelDefault($obGrupo, $_REQUEST['grpordem']);

	$objOrdem = new QQuestionarioOrdem();
	$objOrdem->ordenarQuestionario($obGrupo->getObjPai());

	$arGruposFilhos    = $obGrupo->recuperarGruposFilhosByGrupo();
	$arPerguntasFilhas = $obGrupo->recuperarPerguntasFilhasByGrupo();

	$mGrupo = new QGrupo();
	foreach($arGruposFilhos as $grupoFilho){
	    if($grpcondicaoOriginal == $grupoFilho['grpcondicao'] || $grupoFilho['grpcondicao'] == ''){
	        $mGrupo->carregarPorId($grupoFilho['grpid']);
	        $mGrupo->grpcondicao = $_REQUEST['grpcondicao'];
            $mGrupo->salvar();
            $mGrupo->setDadosNull();
	    }
	}

	$mPergunta = new QPergunta();
	foreach($arPerguntasFilhas as $perguntaFilha){
	    if($grpcondicaoOriginal == $perguntaFilha['percondicao'] || $perguntaFilha['percondicao'] == ''){
	        $mPergunta->carregarPorId($perguntaFilha['perid']);
    	    $mPergunta->percondicao = $_REQUEST['grpcondicao'];
            $mPergunta->salvar();
            $mPergunta->setDadosNull();
	    }
	}

	$obGrupo->commit();
	echo "<script>
				alert('Operação realizada com sucesso');
				window.close();
				window.opener.location.replace(window.opener.location);
			  </script>";

// 	}else{
// 		$obGrupo->rollback();
// 		$obGrupo->insucesso('Falha na operação');
// 	}

    /*
	$obGrupo->commit();

			die ("<script type=\"text/javascript\" src=\"/includes/funcoes.js\"></script>
			  <script type='text/javascript' src='../includes/JQuery/jquery-1.4.2.js'></script>
			  <script>
			 	alert('".('Operação realizada com sucesso!')."');
					window.resizeTo (0, 0);
					window.opener.focus();
					window.blur();
					window.opener.montarA(window.opener.$('#queid').val(), window.opener.$('#sisfiltro').val());
//					window.opener.location.replace('?modulo=sistema/questionario/arvore&acao=A&sisidFiltro='+window.opener.$('#sisfiltro').val()+'&queidFiltro='+window.opener.$('#quefiltro').val()+'&acoidFiltro='+window.opener.$('#queid').val());
				window.close();
			  </script>");
     */

}

// Extrai dados para Atualização
$grptitulo          = $obGrupo->grptitulo;
$grpajuda           = $obGrupo->grpajuda;
$grpstatus          = $obGrupo->grpstatus;
$grppermitemultiplo = $obGrupo->grppermitemultiplo;
$grpqtdmaximo       = $obGrupo->grpqtdmaximo;
$grpordem           = $obGrupo->grpordem;
$queid              = $obGrupo->queid ? $obGrupo->queid : $_GET['queid'];
$gru_grpid          = $obGrupo->gru_grpid ? $obGrupo->gru_grpid : $_GET['gru_grpid'];
$grpid              = $obGrupo->grpid ? $obGrupo->grpid : $_GET['grpid'];
$itpid              = $obGrupo->itpid ? $obGrupo->itpid : $_GET['itpid'];
$ordemi             = $obGrupo->grpordem;
$grpcondicao        = $obGrupo->grpcondicao;

monta_titulo( ('Cadastro de Grupos'), obrigatorio().' '.("Indica Campo Obrigatório"));
$ordem = $grpordem;
if(!$grpordem){
	if($_REQUEST['queid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE queid = $queid AND grpstatus = 'A' ";
		$ordem = ($db->pegaUm($sql))+1;
	} else if ($_REQUEST['itpid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE itpid = $itpid AND grpstatus = 'A' ";
		$ordem = ($db->pegaUm($sql))+1;
	}else if ($_REQUEST['gru_grpid']){
		$sql = "SELECT MAX(grpordem) FROM questionario.grupopergunta WHERE gru_grpid = $gru_grpid AND grpstatus = 'A' ";
		$ordem = ($db->pegaUm($sql))+1;
	}
}
?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type='text/javascript' src='../includes/JQuery/jquery-1.4.2.js'></script>
		<script type="text/javascript">

            function alteraCampoMaximo(opcao){
                if( opcao == 1){
                    $('#tr_qtdmaxima').show();
                    $('#grpqtdmaximo').val('');
                }else{
                    $('#tr_qtdmaxima').hide();
                    $('#grpqtdmaximo').val('');
                }
            }

			function salvarGrupos(){
				var camposObri 		= "#grptitulo#grpordem#grpstatus";
				var tiposCamposObri	= '#texto#texto#radio';

				if(!validaForm('formulario',camposObri,tiposCamposObri,false))
						return false;

				document.getElementById('submeter').value = 'salvar';
				document.getElementById('formulario').submit();
			}
		</script>
	</head>
<body topmargin="0" marginwidth="0">
<form action="" method="POST" id="formulario" name="formulario">
<input type="hidden" id="submeter" name="submeter" value="">
<input type="hidden" id="grpid" name="grpid" value="<?=$grpid; ?>">
<input type="hidden" id="queid" name="queid" value="<?=$queid; ?>">
<input type="hidden" id="gru_grpid" name="gru_grpid" value="<?=$gru_grpid; ?>">
<input type="hidden" id="itpid" name="itpid" value="<?=$itpid; ?>">
<input type="hidden" id="ordemi" name="ordemi" value="<?=$ordemi; ?>">
<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Descrição:"); ?></b></td>
		<td><?=campo_texto('grptitulo', 'S', 'S', ('Título do Grupo'), 50, 450, '', '', '', '', 0, 'id="grptitulo"' ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;"><b><?= ("Texto de ajuda:"); ?></b></td>
		<td><?=campo_textarea('grpajuda', 'N', 'S', ('Texto de ajuda'), 53, 5, 2000, '', '', '', '', ('Texto de ajuda')); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Ordem:"); ?></b></td>
		<td><?=campo_texto('grpordem', 'S', 'S', ('Ordem de Visualização'), 10, 2, '[#]', '', '', '', 0, 'id="grpordem"', '', $ordem ); ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita" width="20px;" style="display:none"><b><?= ("Status:"); ?></b></td>
		<td style="display:none"><input type="radio" name="grpstatus" id="grpstatus" value="A" <?=($grpstatus == 'A' || $grpstatus == '' ? "checked='checked'" : '') ?> > <?= ("Ativo"); ?> &nbsp;
			<input type="radio" name="grpstatus" id="grpstatus" value="I" <?=($grpstatus == 'I' ? "checked='checked'" : '') ?> > <?= ("Inativo"); ?></td>
	</tr>
    <tr>
        <td class="subtitulodireita" width="20px;"><b><?= ("Permitir Multiplas respostas:"); ?></b></td>
        <td><input type="radio" name="grppermitemultiplo" <?= $multiplosHabilitados == 'N' ? 'disabled="disabled"' : '' ?> onclick="alteraCampoMaximo(1)" id="grppermitemultiplo" value="1" <?=($grppermitemultiplo == 't' ? "checked='checked'" : ''  ) ?> > <?= ("Sim"); ?> &nbsp;
            <input type="radio" name="grppermitemultiplo" <?= $multiplosHabilitados == 'N' ? 'disabled="disabled"' : '' ?> onclick="alteraCampoMaximo(0)" id="grppermitemultiplo" value="0" <?=($grppermitemultiplo == 'f' || $grppermitemultiplo == '' ? "checked='checked'" : '') ?> > <?= ("Não"); echo obrigatorio(); ?></td>
    </tr>
	<tr id="tr_qtdmaxima" style="<?=($grppermitemultiplo == 't' ? "" : "display:none;") ?>">
		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Quantidade Máxima:"); ?></b></td>
		<td><?=campo_texto('grpqtdmaximo', 'N', $multiplosHabilitados, ('Quantidade Máxima'), 14, 50, '', '', '', '', 0, 'id="grpqtdmaximo"' ); ?></td>
	</tr>
	<tr style="">
		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Condição:"); ?></b></td>
		<td><?=campo_texto('grpcondicao', 'N', 'S', ('Condição'), 50, 500, '', '', '', '', 0, 'id="grpcondicao"' ); ?></td>
	</tr>
	<tr>
		<th colspan="2">
			<input type="button" value="<?= ("Salvar"); ?>" name="btnSalvar" id="btnSalvar" onclick="salvarGrupos();">&nbsp;
			<input type="button" value="<?= ("Voltar"); ?>" name="btnVoltar" id="btnVoltar" onclick="window.close();">
		</th>
	</tr>
</table>
</form>
</body>
</html>