<?php
//$queid = is_numeric($_REQUEST['queid']) ? $_REQUEST['queid'] : "";
//$escid = is_numeric($_GET['escid']) ? $_GET['escid'] : "";
// Buscas as escolas vinculadas o usuario, atribuídas a esse questionário (acordo)
//$escola = new QEscola();
//$escolaVinculada = $escola->buscarEscolasPorUsuario(null, $queid);
//
//$permissaoSuperUsuario = possui_perfil(PERFIL_SUPER_USUARIO);
//
//if (($escid && !$permissaoSuperUsuario && !in_array($escid, $escolaVinculada)) || (!$queid)){
//	die("<script>
//			alert('".("Usuário não tem permissão de acesso!", true)."');
//			window.close();
//		 </script>");
//}

echo "<div align='center' width='90%'>";
if(!isset($_REQUEST['ajax'])){
	monta_titulo( ('Pré-visualizar Questionário'),  '<img src="/imagens/valida2.gif"/>&nbsp;<label style="color:red;font-weight:bold;">'.('ATENÇÃO!').'</label><br><label style="font-weight:bold;">'.('Esta é apenas uma visualização, nenhuma informação será armazenada').'.</label>');
}
echo "</div>";

?>

<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript">
			//script...
		</script>
	</head>
	<body topmargin="0" marginwidth="0">
		<table bgcolor="#f5f5f5" align="center" class="tabela" >
			<tr>
				<td width="90%" >
					<fieldset style="background: #fff;">
						<legend><?= ("Questionário"); ?></legend>
						<?php
							$tela = new Tela( array("preview" => true, "queid" => $_GET['queid'], 'tamDivPx' => 355, 'relatorio' => 'modulo=principal/relatorioQuestionario&acao=A') );
						?>
					</fieldset>
				</td>
			</tr>
		</table>
	</body>
</html>