<?php
function pegaLegenda(){
	global $db;
	global $legenda;
	global $lgptitulo;
	$sql = "SELECT lgptitulo, lgpid FROM questionario.legendapergunta WHERE perid = {$_REQUEST['perid']} AND lgpsituacao = 'V'";
	$legenda = $db->pegaLinha( $sql );
	$lgptitulo = $legenda['lgptitulo'];
}

function pegaPergunta(){
	global $db;
	global $pertitulo;
	$sql = "SELECT pertitulo FROM questionario.pergunta WHERE perid = {$_REQUEST['perid']}";
	$pertitulo = $db->pegaUm( $sql );
}

function fecharPopUp(){
	echo '<script type="text/javascript">window.opener.location.reload();window.close();</script>';
}

if ( $_REQUEST['perid'] )
{
	pegaPergunta();
	pegaLegenda();
}

if ($_REQUEST['act'] == 'inserir')
{
	if ( empty($lgptitulo) ){
		$sql = "INSERT INTO questionario.legendapergunta(
		            leg_lgpid,
		            perid,
		            lgptitulo,
		            lgpsituacao,
		            usucpfsugestao,
		            usucpfavaliador,
		            queid)
	    		VALUES (
	    			NULL,
	    			'{$_REQUEST['perid']}',
	    			'{$_REQUEST['lgptitulo']}',
	    			'V',
	    			'{$_SESSION['usucpf']}',
	    			NULL,
    				'{$_REQUEST['queid']}'
	    			)";

	}else{
		$sql = "UPDATE questionario.legendapergunta SET lgpsituacao = 'R' WHERE perid = {$_REQUEST['perid']} AND lgpsituacao = 'V'";
		$db->executar( $sql );
		$sql = "INSERT INTO questionario.legendapergunta(
		            leg_lgpid,
		            perid,
		            lgptitulo,
		            lgpsituacao,
		            usucpfsugestao,
		            usucpfavaliador,
		            queid)
	    		VALUES (
					'{$legenda['lgpid']}',
	    			'{$_REQUEST['perid']}',
	    			'{$_REQUEST['lgptitulonovo']}',
	    			'V',
	    			'{$_SESSION['usucpf']}',
	    			NULL,
    				'{$_REQUEST['queid']}'
	    			)";
	}
	$db->pegaUm($sql);
	$db->commit();
	alert('Operação realizada com sucesso');
	fecharPopUp();

}
elseif ($_REQUEST['act'] == 'aceitar')
{
	$sql = "UPDATE questionario.legendapergunta SET lgpsituacao = 'A' WHERE perid = {$_REQUEST['perid']} AND lgpsituacao = 'V'";
	$db->executar( $sql );
	$sql = "UPDATE questionario.pergunta SET pertitulo = '{$_REQUEST['lgptitulo']}' WHERE perid = {$_REQUEST['perid']}";
	$db->executar( $sql );
	$db->commit();
	alert('Operação realizada com sucesso');
	fecharPopUp();
}
elseif ($_REQUEST['act'] == 'sugerir/aceitar')
{

	$sql = "UPDATE questionario.legendapergunta SET lgpsituacao = 'R' WHERE perid = {$_REQUEST['perid']} AND lgpsituacao = 'V'";
	$db->executar( $sql );
	$sql = "INSERT INTO questionario.legendapergunta(
	            leg_lgpid,
	            perid,
	            lgptitulo,
	            lgpsituacao,
	            usucpfsugestao,
	            usucpfavaliador,
	            queid)
    		VALUES (
				'{$legenda['lgpid']}',
    			'{$_REQUEST['perid']}',
    			'{$_REQUEST['lgptitulonovo']}',
    			'A',
    			'{$_SESSION['usucpf']}',
    			NULL,
    			'{$_REQUEST['queid']}'
    			)";

	$db->pegaUm($sql);
	$db->commit();

	$sql = "UPDATE questionario.pergunta SET pertitulo = '{$_REQUEST['lgptitulonovo']}' WHERE perid = {$_REQUEST['perid']}";
	$db->executar( $sql );
	$db->commit();

	alert(('Operação realizada com sucesso'));
	fecharPopUp();
}

monta_titulo( ($titulo_modulo), obrigatorio().' '. ('Indica Campo Obrigatório') );
?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript">

			function validar_cadastro(cod) {
				if (cod != 'A' && cod != 'IA' )
				{
					if (!validaBranco(document.formulario.lgptitulo, '<?= ("Sugestão do item"); ?>')) return;
					<?php if ( !empty($lgptitulo) ) { ?>
					if (!validaBranco(document.formulario.lgptitulonovo, '<?= ("Nova Sugestão de Mudança"); ?>')) return;
					<?php } ?>
				}
				if (cod == 'I') document.formulario.act.value = 'inserir';

				if (cod == 'A')
				{
					if (confirm( '<?= ("Deseja aceitar esta sugestão?"); ?>' )){
						document.getElementById('lgptitulo').readonly = '';
						document.getElementById('lgptitulo').disabled = '';
						document.formulario.act.value = 'aceitar';
					}
				}

				if (cod == 'IA')
				{
					if (confirm( '<?= ("Deseja aceitar sua sugestão?"); ?>' )){
						document.getElementById('lgptitulo').readonly = '';
						document.getElementById('lgptitulo').disabled = '';
						document.formulario.act.value = 'sugerir/aceitar';
					}
				}
				document.formulario.submit();

			 }

		//-->
		</script>
	</head>
	<body topmargin="0" marginwidth="0">
		<form method="POST" name="formulario" >
			<input type="hidden" name="evento" value=""/>
			<input type=hidden name="act" value="0"/>

			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><b><?= ("Item Atual:"); ?></b></td>
					<td><?= campo_textarea( 'pertitulo', 'N', 'N', '', 70, 2, 300 ); ?></td>
				</tr>
				<tr>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><b><?= ("Sugestão do Item:"); ?></b></td>
					<?php  $altera = empty($lgptitulo)? 'S' : 'N'; ?>
					<td><?= campo_textarea( 'lgptitulo', 'S', $altera, '', 70, 2, 300 ); ?></td>
				</tr>
				<tr>
					<?php if ( !empty($lgptitulo) ){ ?>
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%"><b><?= ("Nova Sugestão de Mudança:"); ?></b></td>
					<td><?= campo_textarea( 'lgptitulonovo', 'S', 'S', '', 70, 2, 300 ); ?></td>
					<?php } ?>
				</tr>
			</table>
			<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr style="background-color: #cccccc">

					<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
					<td>
						<?php if ( empty($lgptitulo) ) {?>
						<input type="button" name="botao" value="<?= ("Salvar"); ?>" onclick="validar_cadastro('I');"/>
						<?php }else{?>
						<input type="button" style="width:160px" name="botao" value="<?= ("Aceitar sugestão do item"); ?>" onclick="validar_cadastro('A');"/>
						<input type="button" style="width:160px" name="botao" value="<?= ("Sugerir novo item"); ?>" onclick="validar_cadastro('I');"/>
						<?php 	if(possui_perfil(PERFIL_ADMINISTRADOR)){?>
						<input type="button" style="width:160px" name="botao" value="<?= ("Sugerir e Aceitar novo item"); ?>" onclick="validar_cadastro('IA');"/>
						<?php 	}
						      } ?>
				    </td>
				</tr>

				<tr>
					<td bgcolor="#DCDCDC" colspan="2"><center><b><?= ("Historico de Alterações"); ?></b></center> </td>
			</tr>
			</table>
		</form>
		<div style="height:100px; overflow:auto;">
		<?
			$sqlBusca = "SELECT
							a.lgptitulo,
							b.usunome,
							CASE when a.lgpsituacao = 'R'
								then '<FONT COLOR=\'#FF0000\'> ". ("Reprovado") ." </FONT>' when a.lgpsituacao = 'A'
								then '<FONT COLOR=\'#00FF00\'> ". ("Aprovado") ." </FONT>' when a.lgpsituacao = 'V'
								then '<FONT COLOR=\'#B8860B\'> ". ("Aguardando Aprovação") ." </FONT>'
							end  AS situacao
						FROM
							questionario.legendapergunta a
							LEFT JOIN seguranca.usuario b ON b.usucpf = a.usucpfsugestao
						WHERE
							perid = {$_REQUEST['perid']}";

			// exibe o resultado da consulta

			$db->monta_lista($sqlBusca, array(('Item'),
											  ('Autor'),
											  ('Situação')), 20, 5, 'N', '', '');
		?>
		</div>
	</body>
</html>