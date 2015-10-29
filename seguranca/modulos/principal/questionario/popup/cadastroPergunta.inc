<?php


//retirando as barras que são adicionadas pelo 'magic_quotes_gpc'
$_REQUEST['pertitulo'] = stripcslashes( $_REQUEST['pertitulo'] );
$_REQUEST['perajuda']  = stripcslashes( $_REQUEST['perajuda'] );

//adicionando o 'htmlspecialchars' nos campos
// $_REQUEST['pertitulo'] = htmlspecialchars( $_REQUEST['pertitulo'], ENT_QUOTES);
// $_REQUEST['perajuda']  = htmlspecialchars( $_REQUEST['perajuda'], ENT_QUOTES);

$tabela = new TabelaTabela();
switch($_REQUEST['op']){
	case 'telatabela':
		$tabela->tabelaTela();
		die;
	case 'telagrupocoluna':
		$tabela->grupoColunaTela();
		die;
	case 'telacoluna':
		$tabela->colunaTela();
		die;
	case 'telagrupolinha':
		$tabela->grupoLinhaTela();
		die;
	case 'telalinha':
		$tabela->linhaTela();
		die;
//	case 'salvartabela':
//		$tabela->salvar($_POST['data']);
//		die;
}

if ($_GET['ajax']){
	switch (true) {
		case $_REQUEST['perid2'] :
			$dados = new QPergunta();
			$where = array("perid = '".$_REQUEST['perid2']."'");
			if ($_REQUEST['pertipo'])
				array_push($where, "pertipo='{$_REQUEST['pertipo']}'");

			$rs = $dados->recuperarUm('perid' , $where);

			if ( $rs ){
				$rs = Hierarquia::buscaPergunta($_REQUEST['queid'], Hierarquia::QUESTIONARIO,$_REQUEST['perid2']);
			}

			die ($rs);
		break;
		case $_REQUEST['ajudaMascara'] :
				monta_titulo( ("Ajuda"), '');

				echo "<table class='tabela' style='margin-left:6px' width='95%' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
						<tr>
							<td class='subtituloesquerda' colspan='3' >
								".('A máscara tem como finalidade definir e limitar o formato de um campo de texto. Ao digitar em um campo com máscara definida, o carácter "#" será substituído somente por números digitados.')."
							</td>
						</tr>
						<tr>
							<td class='subtituloesquerda'>
								".("Exemplo")."
							</td>
							<td class='subtituloesquerda'>
								".("Máscara")."
							</td>
							<td class='subtituloesquerda'>
								".("Resultado")."
							</td>
						</tr>
						<tr>
							<td>
								".("CPF")."
							</td>
							<td>
								###<b>.</b>###<b>.</b>###<b>-</b>##
							</td>
							<td>
								555<b>.</b>555<b>.</b>555<b>-</b>55
							</td>
						</tr>
						<tr>
							<td>
								".("Data")."
							</td>
							<td>
								##<b>/</b>##<b>/</b>####
							</td>
							<td>
								05<b>/</b>05<b>/</b>2005
							</td>
						</tr>
						<tr>
							<td>
								".("Número")."
							</td>
							<td>
								############
							</td>
							<td>
								123456789102
							</td>
						</tr>
						<tr>
							<td>
								".("Monetário")."
							</td>
							<td>
								###<b>.</b>###<b>.</b>###<b>,</b>##
							</td>
							<td>
								9<b>.</b>123<b>,</b>45
							</td>
						</tr>
					  </table>";
			break;

			case $_REQUEST['ajudaRegra'] :
				monta_titulo( ("Ajuda"), '');

				echo "<table class='tabela' style='margin-left:6px' width='95%' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
						<tr>
							<td><b>
								<p>
									".('A regra tem como finalidade limitar a resposta de uma determinada pergunta utilizando como referência um operador e a resposta de
									outra pergunta.')."
								</p>
								<p>
									".('Para que a regra seja definida em uma pergunta é necessario definir um operador dentre os disponiveis (Menor, Menor ou Igual, Igual, Maior ou Igual, Maior)
									 e o código da pergunta que será relacionada.')."
								</p>
								<p>
									".('o código pode ser obtido posicionando o mouse sobre a pergunta desejada (sem clicar) na Árvore de Questionários.')."
								</p></b>
							</td>
						</tr>
						<tr>
							<td align='center' bgcolor='#dcdcdc'>
								<b>".('Campos Tipo Texto')."</b>
							</td>
						</tr>
						<tr align='center'>
							<td><br>";
								echo $db->monta_combo("operadorajuda1", array(), 'N', ('Menor ou Igual'), '', '', '', '130', 'N', 'operadorajuda1', '', '', ('Operador') );
								echo " &nbsp; ";
								$peridajuda1 = 6178;
								echo campo_texto('peridajuda1', 'N', 'N', '', 10, 10,'########', '', '', '', 0, 'id="peridajuda1"' );
				echo		"</td>
						</tr>
						<tr>
							<td>
								<b>".('No exemplo acima a resposta da pergunta onde a regra está sendo implementada deverá ser Menor ou Igual a resposta
									 da pergunta de código 6178.')."</b><br>
							</td>
						</tr>
						<tr>
							<td align='center' bgcolor='#dcdcdc'>
								<b>".('Campos Tipo Tabela')."</b>
							</td>
						</tr>
						<tr>
							<td><b>
								<p>
									".('Para relacionar em uma regra uma pergunta a algum campo de uma tabela deve-se utilizar as notações da tabela abaixo:')."
								</p></b>
							</td>
						</tr>
						<tr align='center'>
							<td>
								<table border='1' style='margin-left:3px' width='95%' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
									<tr>
										<td width='60%'>";
												echo $db->monta_combo("operadorajuda2", array(), 'N', ('Menor ou Igual'), '', '', '', '125', 'N', 'operadorajuda2', '', '', ('Operador') );
												echo " &nbsp; ";
												$peridajuda2 = 6178;
												echo campo_texto('peridajuda2', 'N', 'N', '', 15, 15,'########', '', '', '', 0, 'id="peridajuda2"' );
						echo "			</td>
										<td>
											".('A resposta deverá ser Menor ou Igual a soma da resposta de todos os campos da Tabela.')."
										</td>
									</tr>
									<tr>
										<td>";
												echo $db->monta_combo("operadorajuda3", array(), 'N', ('Menor ou Igual'), '', '', '', '125', 'N', 'operadorajuda3', '', '', ('Operador') );
												echo " &nbsp; ";
												$peridajuda3 = "6178:total";
												echo campo_texto('peridajuda3', 'N', 'N', '', 15, 15,'########', '', '', '', 0, 'id="peridajuda3"' );
						echo "			</td>
										<td>
											".('Tem o mesmo resultado do caso acima.')."
										</td>
									</tr>
									<tr>
										<td>";
												echo $db->monta_combo("operadorajuda4", array(), 'N', ('Menor ou Igual'), '', '', '', '125', 'N', 'operadorajuda4', '', '', ('Operador') );
												echo " &nbsp; ";
												$peridajuda4 = "6178:total:1";
												echo campo_texto('peridajuda4', 'N', 'N', '', 15, 15,'########', '', '', '', 0, 'id="peridajuda4"' );
						echo "			</td>
										<td>
											".('A resposta deverá ser Menor ou Igual a soma da resposta de todos os campos da primeira linha da Tabela.')."
										</td>
									</tr>
									<tr>
										<td>";
												echo $db->monta_combo("operadorajuda5", array(), 'N', ('Menor ou Igual'), '', '', '', '125', 'N', 'operadorajuda5', '', '', ('Operador') );
												echo " &nbsp; ";
												$peridajuda5 = "6178:total:a";
												echo campo_texto('peridajuda5', 'N', 'N', '', 15, 15,'########', '', '', '', 0, 'id="peridajuda5"' );
						echo "			</td>
										<td>
											".('A resposta deverá ser Menor ou Igual a soma da resposta de todos os campos da primeira coluna da Tabela.')."
										</td>
									</tr>
									<tr>
										<td>";
												echo $db->monta_combo("operadorajuda6", array(), 'N', ('Menor ou Igual'), '', '', '', '125', 'N', 'operadorajuda6', '', '', ('Operador') );
												echo " &nbsp; ";
												$peridajuda6 = "6178:total:b:2";
												echo campo_texto('peridajuda6', 'N', 'N', '', 15, 15,'########', '', '', '', 0, 'id="peridajuda6"' );
						echo "			</td>
										<td>
											".('A resposta deverá ser Menor ou Igual a resposta do campo correspondente a célula b2 (segunda coluna e segunda linha) da Tabela.')."
										</td>
									</tr>
								<table>
							</td>
						</tr>
					 </table>";
			break;
		default:
			die(('Falta parâmetro na requisição.'));
	}
	die;
}



$obPergunta = new QPergunta( $_REQUEST['perid'] );

//verifica se a pergunta já foi respondida, se sim, não poderá ser alterada
if($_REQUEST['perid']){
	$respondida = count($obPergunta->perguntaRespondida()) > 0;
	if($respondida)
		die ("<script>
			 	alert('".('Não é possível alterar esta pergunta pois ela já foi respondida.')."');
				window.close();
			  </script>");
}


if($_POST['submeter'] == 'salvar'){
	if($_REQUEST['tp'] == 'acordo'){
		$arCampos = array( "perid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao" ,"pervalidacaoonblur" );
		$where = "queid = {$_POST['queid']}";
	} else if($_POST['queid'] <> ''){
		$arCampos = array( "perid", "queid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao","pervalidacaoonblur"  );
		$where = "queid = {$_POST['queid']}";
	} else if ($_POST['grpid'] <> ''){
		$arCampos = array( "perid", "grpid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao" ,"pervalidacaoonblur" );
		$where = "grpid = {$_POST['grpid']}";
	} else if ($_POST['itpid'] <> ''){
		$arCampos = array( "perid", "itpid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao","pervalidacaoonblur"  );
		$where = "itpid = {$_POST['itpid']}";
	} else if ($_POST['grpid'] <> '' && $_POST['queid'] <> ''){
		$arCampos = array( "perid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao","pervalidacaoonblur"  );
	} else {
		$arCampos = array( "perid", "pertipo", "pertitulo", "perajuda", "perobrigatorio", "permascara", "permultipla", "pertipografico", "pertabela", "perregra", "queid_atual", "percondicao","pervalidacaoonblur" );
	}

	if ($_REQUEST['tp'] == 'acordo'){
		array_push($arCampos, 'persugerido');
		$_REQUEST['persugerido'] = true;
		array_push($arCampos, 'grpid');

		//cria grupo de sugestões
		$qtd = $obPergunta->getCountPerSugeridoPorQuestionario($_REQUEST['queid']);
		if ($qtd == 0){
			$obGrupo = new QGrupo();
			$ordem = ($obGrupo->recuperarUm("MAX(grpordem)", array("queid = {$_REQUEST['queid']}")))+1;
			$arCamposGrupo = array("queid", "grptitulo", "grpordem", "grpstatus", "grpajuda", "queid_atual");
			$arDadosGrupo  = array("queid"=>$_REQUEST['queid'], "grptitulo"=>"Perguntas Sugeridas", "grpordem"=>$ordem,"grpstatus"=>'A', "grpajuda"=>"Grupo de Perguntas Sugeridas", "queid_atual" => $_REQUEST['queval'] );
			$obGrupo->popularObjeto($arCamposGrupo, $arDadosGrupo);
			$_REQUEST['grpid'] = $obGrupo->salvar();
			$obGrupo->commit();
		}else{
			$sql = "SELECT
						per.grpid
					FROM
						questionario.legendapergunta leg
				 	INNER JOIN
					 	questionario.pergunta per ON per.perid = leg.perid AND per.perstatus = 'A'
				 	WHERE
				 		per.persugerido = TRUE AND leg.queid = {$_REQUEST['queid']}";
			$_REQUEST['grpid'] = $db->pegaUm($sql);
		}

	}

	// Processa as extensões
	if ($_POST['pertipo'] == 'ARQ'){
        if($_POST['perextensao'] != ''){
            $extensoes               = split(',', $_POST['perextensao']);
            $extensoes               = array_map('trim', $extensoes);
            $extensoes               = array_map('strtolower', $extensoes);
            $extensoes               = join(", ", $extensoes);
            $_REQUEST['perextensao'] = $extensoes;
        }

        array_push($arCampos, 'perextensao');
        array_push($arCampos, 'perdescricao');
        array_push($arCampos, 'perqtdchardescricao');
        array_push($arCampos, 'perdescricaoobrigatoria');
	}

	if ($_POST['pertipo'] == 'END'){
        array_push($arCampos, 'perexibemapa');
        $_REQUEST['perexibemapa'] = $_REQUEST['perexibemapa'];
	}

	if($_POST['pertipo'] == 'DT'){
        array_push($arCampos, 'permascara');
        $_REQUEST['permascara'] = $_REQUEST['mascaradata'];
	}


	$obPergunta->popularObjeto( $arCampos, $arDados );
	$perid = $obPergunta->salvar();

	if ( $obPergunta->perid )
		$perid = $obPergunta->perid;

	if ($perid && $_POST['pertipo'] == 'EXT'){
		$tabela->salvar($_POST['tabela_json'], $perid);
	}


	if ( !empty($_REQUEST['operador']) && !empty($_REQUEST['perid2']) ){
		$_REQUEST['perregra'] = $perid.";".$_REQUEST['operador'].";".$_REQUEST['perid2'];
	}else{
		$_REQUEST['perregra'] = '';
	}

	$obPergunta->popularDadosObjeto( array("perregra" => $_REQUEST['perregra'],"queid_atual" => $_REQUEST['queval']) )->salvar();

	$objQuestionario = new QQuestionario();
	$objQuestionario->ordenaNivelDefault($obPergunta, $_REQUEST['perordem']);

	$obPergunta->commit();

	if($_POST['perordem'] > $_POST['ordemi']){
		$sit = '>';
	} else {
		$sit = '>=';
	}



	if ($_REQUEST['tp'] == 'acordo'){

		$arCamposSugerido = array("perid", "lgptitulo", "lgpsituacao", "usucpfsugestao", "queid");
		$arDadosSugerido =  array('perid'=>$perid , "lgptitulo"=>$_REQUEST['pertitulo'], "lgpsituacao"=>"V", "usucpfsugestao"=>$_SESSION['usucpf'], "queid"=>$_REQUEST['queid']);
		$obLegenda = new QLegendaPergunta();
		$obLegenda->popularObjeto($arCamposSugerido, $arDadosSugerido);
		$obLegenda->salvar();
		$obLegenda->commit();
	}


	if( $perid ){
		$sql = "SELECT
					ip.itpid
				FROM
					questionario.itempergunta ip
				INNER JOIN questionario.pergunta p ON ip.itpid = p.itpid AND
                                                      p.perstatus = 'A'
				WHERE ip.perid = $perid AND
                      ip.itpstatus = 'A'";

		$arrItpid = (array) $db->carregarColuna($sql);
		if ($arrItpid[0]){
			$where1 = "AND itpid NOT IN (" . implode(", ", $arrItpid) . ")";
		}
		$sql = "SELECT
					ip.itpid
				FROM
					questionario.itempergunta ip
				INNER JOIN questionario.resposta r ON r.itpid = ip.itpid
				WHERE ip.perid = $perid AND
                      ip.itpstatus = 'A'";

		$arrItpid = (array) $db->carregarColuna($sql);
		if ($arrItpid[0]){
			$where2 = "AND itpid NOT IN (" . implode(", ", $arrItpid) . ")";
		}

		// Deleta os questionarioordem
		$sql = "DELETE FROM questionario.questionarioordem WHERE itpid IN ( SELECT itpid FROM questionario.itempergunta WHERE itpstatus = 'A' AND perid = {$perid} {$where1} {$where2} )";
		$db->executar($sql);

		// Deleta os não respondidos
		$sql = "DELETE FROM questionario.itempergunta WHERE perid = {$perid} {$where1} {$where2}";
		$db->executar($sql);
		if(in_array($_REQUEST['pertipo'], array('CK', 'RA', 'CM', 'CB', 'BM'))){
			if ( is_array($_POST['descricao'])){
				foreach($arrItpid as $k => $itpid){
				    $ordem = $k + 1;

					$obItemPergunta = new QItemPergunta($itpid);
					$arCamposItem = array("perid", "itptitulo", "itpordem");
					$arDadosItem  = array("perid" => $perid, "itptitulo" => $_POST['descricao'][$itpid], "itpordem" => $ordem);

					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
				//	$_POST['descricao'] = array_diff_key($_POST['descricao'], array($itpid));
					$obItemPergunta->salvar();
					$obItemPergunta->commit();

				}

				foreach($_POST['descricao'] as $k => $itptitulo):
				    $ordem = $k + 1;

					$obItemPergunta = new QItemPergunta();
					$arCamposItem = array("perid", "itptitulo","queid_atual", "itpordem");
					$arDadosItem  = array("perid" => $perid, "itptitulo" => $itptitulo, "queid_atual" => $_REQUEST['queval'], "itpordem" => $ordem);

					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
					$obItemPergunta->salvar();
					$obItemPergunta->commit();

				endforeach;

			}
			//SE EU DELETAR TODOS OS LINKS DAQUI ELE NÃO VAI ENTRAR E CONSEQUENTEMENTE NÃO VAI DELETAR!
			if($_POST['descricaocarregado']){
				foreach($_POST['descricaocarregado'] as $itpid => $itptitulo){
					$obItemPergunta = new QItemPergunta($itpid);
					$arCamposItem = array("itptitulo" );
					$arDadosItem  = array("itptitulo" => $itptitulo);

					$obItemPergunta->popularObjeto($arCamposItem, $arDadosItem);
					$obItemPergunta->salvar();
					$obItemPergunta->commit();
				}
			}
		}

        $objOrdem = new QQuestionarioOrdem();
        $objOrdem->ordenarQuestionario($obPergunta->getObjPai());

		$obPergunta->commit();


		die ("<script type=\"text/javascript\" src=\"/includes/funcoes.js\"></script>
			  <script type='text/javascript' src='../includes/JQuery/jquery-1.4.2.js'></script>
			  <script>
			 	alert('".('Operação realizada com sucesso!')."');
// 				if ( window.opener.$('#sisfiltro').length == 0 ){
					window.opener.location.replace(window.opener.location);
// 				}else{
// 					window.resizeTo (0, 0);
// 					window.opener.focus();
// 					window.blur();
// 					window.opener.montarA(window.opener.$('#queid').val(), window.opener.$('#sisfiltro').val());
//					window.opener.location.replace('?modulo=sistema/questionario/arvore&acao=A&sisidFiltro='+window.opener.$('#sisfiltro').val()+'&queidFiltro='+window.opener.$('#quefiltro').val()+'&acoidFiltro='+window.opener.$('#queid').val());
// 				}
				window.close();
			  </script>");
	}
	else{
		$obPergunta->rollback();
		$obPergunta->insucesso(('Falha na operação'));
	}
}


if ($_REQUEST['tp'] == 'acordo'){
	$qtd = $obPergunta->getCountPerSugeridoPorQuestionario($_REQUEST['queid']);

	if ($qtd >= LIMITE_PERGUNTAS_SUGERIDAS ){
		echo "<script>
				alert('".("O limite de")." " . LIMITE_PERGUNTAS_SUGERIDAS . " ".("perguntas sugeridas já foi atingido").".');
				window.close();
			  </script>";
	}
}



// Extrai dados para Atualização
$perid			         = ($obPergunta->perid ? $obPergunta->perid : $_GET['perid']);
$grpid			         = ($obPergunta->grpid ? $obPergunta->grpid : $_GET['grpid']);
$itpid			         = ($obPergunta->itpid ? $obPergunta->itpid : $_GET['itpid']);
$queid			         = ($obPergunta->queid ? $obPergunta->queid : $_GET['queid']);
$pertipo		         = $obPergunta->pertipo;
$pervalidacaoonblur		 = $obPergunta->pervalidacaoonblur;
$pertitulo		         = $obPergunta->pertitulo;
$perordem		         = $obPergunta->perordem;
$ordemi	    	         = $obPergunta->perordem;
$perajuda		         = $obPergunta->perajuda;
$perobrigatorio	         = $obPergunta->perobrigatorio;
$permascara		         = $obPergunta->permascara;
$permultipla	         = $obPergunta->permultipla;
$pertipografico	         = $obPergunta->pertipografico;
$pertabela	 	         = $obPergunta->pertabela;
$perregra		         = $obPergunta->perregra;
$perextensao		     = $obPergunta->perextensao;
$perdescricao		     = $obPergunta->perdescricao;
$perqtdchardescricao	 = $obPergunta->perqtdchardescricao;
$perdescricaoobrigatoria = $obPergunta->perdescricaoobrigatoria;
$percondicao             = $obPergunta->percondicao;
$perexibemapa            = $obPergunta->perexibemapa;

if ($pertipo == 'EXT'){
	// Dados da Tabela
	$dadosTabela = new Tabela();
	$dadosTabela->carregarPorPergunta($perid);
	extract($dadosTabela->getDados());
}

if($_REQUEST['queid'] && !$_REQUEST['perid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE queid = $queid AND perstatus = 'A'";
	$ordem = ($db->pegaUm($sql))+1;
} else if($_REQUEST['grpid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE grpid = $grpid AND perstatus = 'A'";
	$ordem = ($db->pegaUm($sql))+1;
} else if ($_REQUEST['itpid']){
	$sql = "SELECT MAX(perordem) FROM questionario.pergunta WHERE itpid = $itpid AND perstatus = 'A'";
	$ordem = ($db->pegaUm($sql))+1;
}

monta_titulo( ("Cadastro de Perguntas"), obrigatorio().' '.("Indica Campo Obrigatório"));

if($obPergunta->perid){
	$sql = "SELECT
				ip.itpid, itptitulo, r.resid, g.grpid, p.perid
			FROM
				questionario.itempergunta ip
			LEFT JOIN questionario.resposta r using(itpid)
			LEFT JOIN questionario.grupopergunta g using(itpid)
			LEFT JOIN questionario.pergunta p using(itpid)
			WHERE
				ip.perid = ". $obPergunta->perid." AND
                ip.itpstatus = 'A' order by 1";
	$arrItens = (array) $db->carregar($sql);
}
?>


<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery.json.js"></script>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript">

		var d = document;
		function salvarPergunta(){

			if ($("#pertipo").val() == "EXT"){
				TableManager.montarJsonCampo();
			}else{
				TableManager.montarJsonCampoClear();
			}

			var camposObri 		= "#pertipo#pertitulo#perordem";
			var tiposCamposObri	= '#select#textarea#texto';

			if(!validaForm('formulario',camposObri,tiposCamposObri,false))
					return false;

			if(!validaForm('formulario',camposObri,tiposCamposObri,false))
				return false;

			if ( d.getElementById('operador').value == '' ^ d.getElementById('perid2').value == '' ){
				if ( d.getElementById('operador').value == '')
					alert('<?= ('O campo obrigatório: Operador!'); ?>');
				if ( d.getElementById('perid2').value == '')
					alert('<?= ('O campo obrigatório: Identificador 2!'); ?>');
				return;
			}

			if ( d.getElementById('operador').value != '' &&  d.getElementById('perid2').value != '' ){
				if ( !verificaPergunta(d.getElementById('perid2').value,d.getElementById('queval').value ) ){
						var perg = d.getElementById('perid2').value.split(':');
						if (perg.length > 1){
							alert('<?= ('A pergunta informada não existe ou não pertence a esse questionário Ou o erro está na notação ":" de uso exclusivo em perguntas do tipo tabela'); ?>');
						}else{
							alert('<?= ('A pergunta informada não existe ou não pertence a esse questionário!'); ?>');
						}
						d.getElementById('perid2').value = '';
						return;
				}
			}

			if ($("#pertipo").val() == "ARQ"){
			    if($('#perextensao').val() != ''){
			    	var er = RegExp('^[a-zA-Z0-9, ]*$'); // Só permite letras, números, vírgula e um eventual espaço em branco
			    	if(!er.test($('#perextensao').val())){
						alert('<?= ('As extensões informadas são inválidas, elas devem conter apenas letras e números e devem ser separadas por vírgula.'); ?>');
						return false;
			    	}
			    }
			    if($('[name=perdescricao][value=t]:checked').length > 0 && $('#perqtdchardescricao').val() == ''){
					alert('<?= ('Informe o tamanho do campo de descrição do arquivo.'); ?>');
					return false;
			    }
			}

			if ($("#pertipo").val() == "DT"){
    			if ($("[name=mascaradata]").val() == ""){
					alert('<?= ('Informe o formato da data.'); ?>');
					return false;
    			}
			}

            d.getElementById('submeter').value = 'salvar';
			d.getElementById('formulario').submit();
		}

		function gerenciaTotalizador(obj){
			var idtr   = ($(obj).attr('name') == 'tbltotalvertical' ? '#tr1_textotabela' : '#tr2_textotabela');
			var idtext = ($(obj).attr('name') == 'tbltotalvertical' ? '#tbltotalverticaltxt' : '#tbltotalhorizontaltxt');
			if ($(obj).val() == 't'){
				$(idtr).show();
			}else{
				$(idtr).hide();
				$(idtext).val("");
			}
		}

		function tipoPergunta(tipo){
			var arrTipo = Array('CK', 'RA', 'CM', 'CB', 'BM');

//			if ( arrTipo.indexOf(tipo) > -1 ){
			if ( $.inArray(tipo, arrTipo) > -1 ){
				$('#itens').css("display", "");
			}else{
				$('#itens').css("display", "none");
			}

			if (tipo == 'TX'){
				$('#ajudaRegra').css("display","");
				$('#ajudaMascara').css("display","");
				$('#exibirValidacaoEspecial').show();
			}else{
				$('#exibirValidacaoEspecial').hide();

				$('#ajudaRegra').css("display","none");
				$('#ajudaMascara').css("display","none");

				$('#pervalidacaoonblur').val("");
				$('#operador').val("");
				$('#perid2').val("");
				$('#permascara').val("");
			}

			if(tipo == 'DT'){
				$('#mascaraData').css("display","");
			}else{
				$('#mascaraData').css("display","none");
			}

			if (tipo == 'EXT'){
				$('[id*=_tabela]').css("display", "");
				window.resizeTo (900, 770);
			}else{
				$('[id*=_tabela], [id*=_textotabela]').css("display", "none");
				$('[name*=tbltotal][value=f]').attr('checked', 'checked');
				$('[id*=tbltotal]:input').val('');
				window.resizeTo (600, 770);
			}

			if (tipo == 'ARQ'){
		        $('#extensaoArquivo').show();
		        $('#dscArquivo').show();
		        $('#dscArquivoObrigatoria').show();
		        $('#dscArquivoQuantidade').show();

		        exibirCamposDescArq();
			}else{
				$('#extensaoArquivo').hide();
		        $('#dscArquivo').hide();
		        $('#dscArquivoObrigatoria').hide();
		        $('#dscArquivoQuantidade').hide();

				$('#perqtdchardescricao').val(250);
				$('#perextensao').val('');
				$('[name=perdescricao][value=t]').attr('checked', 'checked');
				$('[name=perdescricaoobrigatoria][value=t]').attr('checked', 'checked');
			}

			if (tipo == 'END'){
				$('#exibirMapa').show();
			}else{
				$('#exibirMapa').hide();
			}

		}

		function addItem(linhaDados){
			var table, totRow, metadeRow, color, img, resid, itpid, itptitulo = '' , grpid, perid;
			if(linhaDados){
				resid 	  = linhaDados[0];
				itpid 	  = linhaDados[1];
				itptitulo = linhaDados[2];
				grpid	  = linhaDados[3];
				perid	  = linhaDados[4];
			}

			if (resid || grpid || perid){
				name = "descricaocarregado[" + itpid + "]";
				img	 = '<img src="/imagens/excluir_01.gif" onclick="alert(\''+ '<?= ("Esse item já possui um registro vinculado.") ?>'+'\');" >';
			}else{
				name = "descricao[]";
				img	 = '<img src="/imagens/excluir.gif" onclick="$(\'#tableItem\')[0].deleteRow(this.parentNode.parentNode.parentNode.rowIndex);">';
			}

			table = $('#tableItem');

//			totRow = table[0].rows.length;
//			metadeRow  = (totRow / 2).toFixed(0);

			var campo1 = '<input type="text" name="' + name + '" value="' + itptitulo + '" style="width:360px;">';
//			var campo2 = '<input type="text" nome="valor[]" style="width:140px;">';
//			table.append("<tr><td><center>" + img + "</center></td><td>" + campo1 + "</td><td>" + campo2 + "</td><tr>");

			table.append("<tr><td><center>" + img + "</center></td><td>" + campo1 + "</td></tr>");



		}

		function verificaPergunta( perid2 , queid ){
			var pertipoURL = '';
			var idNum = new Number(perid2);
			if (isNaN(idNum) == true){
				var arNum = perid2.split(':');
				if (arNum.length <= 1){
					return false;
				}else{
					perid2 = arNum[0];
					pertipoURL = '&pertipo=EXT';
				}
			}

			if( perid2 != ''){
					var url = location.href;
					var temp = '';
					url += '&ajax=1&perid2='+perid2+'&queid='+queid+pertipoURL;

					$.ajax({url: url,
						    type: "POST",
						    async:false,
						    success:function(data){
										temp = data;
									}});
					return temp;
			}

		}

		function exibirCamposDescArq(){
		    if($('[name=perdescricao][value=t]:checked').length > 0){
		        $('#dscArquivoObrigatoria').show();
		        $('#dscArquivoQuantidade').show();
		    }else{
		        $('#dscArquivoObrigatoria').hide();
		        $('#dscArquivoQuantidade').hide();

				$('#perqtdchardescricao').val(250);
				$('[name=perdescricaoobrigatoria][value=t]').attr('checked', 'checked');
		    }
		}

		</script>

	</head>
<body topmargin="0" marginwidth="0">
	<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
		<input type="hidden" id="submeter" name="submeter" value="">
		<input type="hidden" id="grpid" name="grpid" value="<?=$grpid; ?>">
		<input type="hidden" id="itpid" name="itpid" value="<?=$itpid; ?>">
		<input type="hidden" id="perid" name="perid" value="<?=$perid; ?>">
		<input type="hidden" id="queid" name="queid" value="<?=$queid; ?>">
		<input type="hidden" id="queval" name="queval" value="<?= $_GET['queval'] ? $_GET['queval'] : $_GET['queid']; ?>">
		<input type="hidden" id="ordemi" name="ordemi" value="<?=$ordemi; ?>">
		<input type="hidden" id="permultipla" name="permultipla" value="f">

<!--		<div id="div_conteudo">-->
			<table class='tabela' width='95%' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
				<tr>
					<td class='subtituloesquerda' style='width: 20%; text-align:center; text-decoration: underline;'>
						<?= ("Hierarquia"); ?>
					</td>
				</tr>
				<tr>
					<td class='subtituloesquerda' style='width: 20%;'>
							<?php
								$map = array("queid" => Hierarquia::QUESTIONARIO,
											 "grpid" => Hierarquia::GRUPO,
											 "perid" => Hierarquia::PERGUNTA,
											 "itpid" => Hierarquia::ITEMPERGUNTA);
								foreach ($map as $index => $value){
									if (isset($_REQUEST[$index])){
										$hierarquia = new Hierarquia($_REQUEST[$index], $value);
										break;
									}
								}
								echo $hierarquia->getHierarquia();
							?>
					</td>
				</tr>
			</table>

			<table class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
				<tr>
					<td class="subtitulodireita" style="width: 20%;"><b><?= ("Tipos de Pergunta:"); ?></b></td>
					<td><?
						$array = array(
										array("codigo" => "CK", "descricao" => "Checkbox"),
										array("codigo" => "RA", "descricao" => "Radio")
									  );

						if(empty($pertipo) || $pertipo == "TX")
							$array[] = array("codigo" => "TX", "descricao" => "Texto");

						if(empty($pertipo) || $pertipo == "CB")
							$array[] = array("codigo" => "CB", "descricao" => "Combo");

						if(empty($pertipo) || $pertipo == "CM")
							$array[] = array("codigo" => "CM", "descricao" => "Combo Múltipla");

						if(empty($pertipo) || $pertipo == "DT")
							$array[] = array("codigo" => "DT", "descricao" => "Data");

						if(empty($pertipo) || $pertipo == "TA")
							$array[] = array("codigo" => "TA", "descricao" => "Textarea");

//						if((empty($pertipo) || $pertipo == "EXF") && $db->testa_superuser())
//							$array[] = array("codigo" => "EXF", "descricao" => "Externo");

						if(empty($pertipo) || $pertipo == "EXT")
							$array[] = array("codigo" => "EXT", "descricao" => "Tabela");

						$habilitado = empty($pertipo) || $pertipo == 'CK' || $pertipo == 'RA' ? 'S' : 'N';

						if(empty($pertipo) || $pertipo == "ARQ")
							$array[] = array("codigo" => "ARQ", "descricao" => "Arquivo");

						if(empty($pertipo) || $pertipo == "MAP")
							$array[] = array("codigo" => "MAP", "descricao" => "Mapa");

						if(empty($pertipo) || $pertipo == "END")
							$array[] = array("codigo" => "END", "descricao" => "Endereço");

						$db->monta_combo("pertipo", $array, $habilitado, ('Selecione...'), 'tipoPergunta', '', '', '265', 'S', 'pertipo', '', $pertipo, ('Tipos de Pergunta') );
					 ?></td>
				</tr>
				<tr id="exibirValidacaoEspecial" style="display: <?= $pertipo == 'TX' ? 'block' : 'none'; ?>;">
					<td class="subtitulodireita" style="width: 20%;"><b><?= ("Validação Especial:"); ?></b></td>
					<td><?
						$array = array(
										array("codigo" => "EMAIL", "descricao" => "Email")
									  );


						$db->monta_combo("pervalidacaoonblur", $array, 'S', ('Selecione...'), '', '', '', '265', 'N', 'pervalidacaoonblur', '', $pervalidacaoonblur, ('Validação Especial') );
					 ?></td>
				</tr>

				<tr id="tr1_tabela" style="display:none ;">
					<td class="subtitulodireita" style="width: 20%;">
						<b><?= ("Totalizador Vertical (por coluna):"); ?></b>
					</td>
					<td>
						<label>
							<input name="tbltotalvertical" type="radio" value="t" onclick="gerenciaTotalizador(this);" <?php echo $tbltotalvertical == 't' ? 'checked="checked"' : ""; ?>>
							<?= ("Sim") ?>
						</label>
						<label>
							<input name="tbltotalvertical" type="radio" value="f" onclick="gerenciaTotalizador(this);" <?php echo $tbltotalvertical == 'f' ? 'checked="checked"' : ""; ?>>
							<?= ("Não") ?>
						</label>
					</td>
				</tr>
				<tr id="tr1_textotabela" style="display:<?php echo $tbltotalvertical == 't' ? '' : 'none'; ?>;">
					<td class="subtitulodireita" style="width: 20%;">
						<b><?= ("Texto do Totalizador:"); ?></b>
					</td>
					<td>
						<?=campo_texto('tbltotalverticaltxt', 'N', 'S', 'Totalizador Vertical', 50, 30, '', '', '', '', 0, 'id="tbltotalverticaltxt"' ); ?>
					</td>
				</tr>
				<tr id="tr2_tabela" style="display:none ;">
					<td class="subtitulodireita" style="width: 20%;">
						<b><?= ("Totalizador Horizontal (por linha):"); ?></b>
					</td>
					<td>
						<label>
							<input name="tbltotalhorizontal" type="radio" value="t" onclick="gerenciaTotalizador(this);" <?php echo $tbltotalhorizontal == 't' ? 'checked="checked"' : ''; ?>>
							<?= ("Sim") ?>
						</label>
						<label>
							<input name="tbltotalhorizontal" type="radio" value="f" onclick="gerenciaTotalizador(this);" <?php echo $tbltotalhorizontal == 'f' ? 'checked="checked"' : ''; ?>>
							<?= ("Não") ?>
						</label>
					</td>
				</tr>
				<tr id="tr2_textotabela" style="display:<?php echo $tbltotalhorizontal == 't' ? '' : 'none'; ?>;">
					<td class="subtitulodireita" style="width: 20%;">
						<b><?= ("Texto do Totalizador:"); ?></b>
					</td>
					<td>
						<?=campo_texto('tbltotalhorizontaltxt', 'N', 'S', 'Totalizador Horizontal', 50, 30, '', '', '', '', 0, 'id="tbltotalhorizontaltxt"' ); ?>
					</td>
				</tr>
				<tr id="estrutura_tabela" style="display:none ;">
					<td colspan="2" align="center">
						<fieldset style="background: #FFF;">
							<legend><?= ("Tabela"); ?></legend>
							<div style="overflow: auto; height: 200px;">
							<?php
							$tabela->matrizTela($perid);
							?>
							</div>
						</fieldset>
					</td>
				</tr>

				<tr>
					<td class="subtitulodireita" width="20px;"><b><?= ("Item:"); ?></b></td>
					<td><?=campo_textarea('pertitulo', 'S', 'S', ('Descrição da pergunta'), 73, 5, 300, '', '', '', '', ('Descrição da pergunta')); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="20px;"><b><?= ("Texto de ajuda:"); ?></b></td>
					<td><?=campo_textarea('perajuda', 'N', 'S', ('Texto de ajuda'), 73, 5, 1000, '', '', '', '', ('Texto de ajuda')); ?></td>
				</tr>
				<tr id="ajudaMascara" style="display:none;">
					<td class="subtitulodireita" style="width: 20%;"><b><?= ("Máscara:"); ?></b></td>
					<td ><?=campo_texto('permascara', 'N', 'S', 'Máscara', 50, 30, '', '', '', '', 0, 'id="permascara"' ); ?>
						<img src='/imagens/ajuda.gif' title="<?= ("Clique aqui para obter ajuda com a máscara.")?>" style="cursor:pointer" onclick="ajudaMascara.open(true);">
					</td>
				</tr>
				<tr>
					<td class="subtitulodireita" style="width: 20%;"><b><?= ("Ordem:"); ?></b></td>
					<td><?=campo_texto('perordem', 'S', 'S', 'Ordem de Visualização', 15, 5, '[#]', '', '', '', 0, 'id="perordem"', '', $ordem ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita" width="20px;"><b><?= ("Obrigatório:"); ?></b></td>
					<td><input type="radio" name="perobrigatorio" id="perobrigatorio" value="1" <?=($perobrigatorio == 't' || $perobrigatorio == '' ? "checked='checked'" : '') ?> > <?= ("Sim"); ?> &nbsp;
						<input type="radio" name="perobrigatorio" id="perobrigatorio" value="0" <?=($perobrigatorio == 'f' ? "checked='checked'" : '') ?> > <?= ("Não"); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita" style="width: 20%;"><b><?= ("Tipo de Gráfico:"); ?></b></td>
					<td><?
						$array = array(
									array("codigo" => "P", "descricao" => ("Pizza")),
									array("codigo" => "L", "descricao" => ("Linha")),
									array("codigo" => "B", "descricao" => ("Barra")),
									);
						$db->monta_combo("pertipografico", $array, 'S', ('Selecione...'), '', '', '', '265', 'N', 'pertipografico', '', '', ('Tipo de Gráfico') );
					 ?></td>
				</tr>
				<tr style="">
            		<td class="subtitulodireita" style="width: 20%;"><b><?= ("Condição:"); ?></b></td>
            		<td><?=campo_texto('percondicao', 'N', 'S', ('Condição'), 50, 500, '', '', '', '', 0, 'id="percondicao"' ); ?></td>
            	</tr>
				<tr style="display: none;">
					<td class="subtitulodireita" width="20px;"><b><?= ("Saída em Tabela:"); ?></b></td>
					<td><input type="radio" name="pertabela" id="pertabela" value="1" <?=($pertabela == 't' || $pertabela == '' ? "checked='checked'" : '') ?> > <?= ("Sim"); ?> &nbsp;
						<input type="radio" name="pertabela" id="pertabela" value="0" <?=($pertabela == 'f'  ? "checked='checked'" : '') ?> > <?= ("Não"); ?></td>
				</tr>
				<tr id="ajudaRegra" style="display:none;">
					<td class="subtitulodireita" width="20px;"><b><?= ("Regra:"); ?></b></td>
					<td>
						<?
							$perregra = explode(';', $perregra);
							$operador = $perregra[1];
							$perid2 = $perregra[2];

							if ( $_REQUEST['perid']){
								echo campo_texto('perid', 'N', 'N', '', 10, 10,'########', '', '', '', 0, 'id="perid"' );
							}
							echo " &nbsp; ";
							$array = array(
											array("codigo" => "<", "descricao" => ("Menor")),
											array("codigo" => "<=", "descricao" => ("Menor ou Igual")),
											array("codigo" => "==", "descricao" => ("Igual")),
											array("codigo" => ">=", "descricao" => ("Maior ou Igual")),
											array("codigo" => ">", "descricao" => ("Maior"))
										);
							echo $db->monta_combo("operador", $array, 'S', ('Selecione...'), '', '', '', '130', 'N', 'operador', '', '', ('Operador') );
							echo " &nbsp; ";
							echo campo_texto('perid2', 'N', 'S', '', 10, 10,'', '', '', '', 0, 'id="perid2"' );
						?>
						<img src='/imagens/ajuda.gif' title="<?= ("Clique aqui para obter ajuda com a regra.")?>" style="cursor:pointer" onclick="ajudaRegra.open(true);">
					</td>
				</tr>

				<tr id="exibirMapa" style="display: none;">
					<td class="subtitulodireita" width="20px;">
					   <b><?= ("Exibir mapa?"); ?></b>
				    </td>
					<td>
					   <input type="radio" name="perexibemapa" id="perexibemapa" value="t" <?=($perexibemapa == 't' || $perexibemapa == '' ? "checked='checked'" : '') ?> > <?= ("Sim"); ?> &nbsp;
					   <input type="radio" name="perexibemapa" id="perexibemapa" value="f" <?=($perexibemapa == 'f'  ? "checked='checked'" : '') ?> > <?= ("Não"); ?>
				   </td>
				</tr>

			<!--	<tr>-->
			<!--		<td class="subtitulodireita" width="20px;"><b>Múltiplo:</b></td>-->
			<!--		<td><input type="radio" name="permultipla" id="permultipla" value="1" <?=($permultipla == 't' ? "checked='checked'" : '') ?> > Sim &nbsp;-->
			<!--			<input type="radio" name="permultipla" id="permultipla" value="0" <?=($permultipla == 'f' ? "checked='checked'" : '') ?> > Não</td>-->
			<!--	</tr>-->

                <tr id="extensaoArquivo">
					<td class="subtitulodireita" style="width: 20%;">
					   <b><?= ("Extensões:"); ?></b>
				    </td>
					<td ><?=campo_texto('perextensao', 'N', 'S', 'Extensões permitidas para upload', 50, 250, '', '', '', '', 0, 'id="perextensao"' ); ?>
						<img src='/imagens/ajuda.gif' title="<?= ("Informe as extensões permitidas para upload separadas por vírgula, exemplo: pdf, doc, jpg \nPara não restringir as extensões deixe o campo em branco.")?>" style="cursor:pointer">
					</td>
				</tr>
				<tr id="dscArquivo" style="display: none;">
					<td class="subtitulodireita" width="20px;">
					   <b><?= ("Descrição para o arquivo:"); ?></b>
				    </td>
					<td>
					   <input type="radio" name="perdescricao" id="perdescricao" value="t" onclick="exibirCamposDescArq();" <?=($perdescricao == 't' || $perdescricao == '' ? "checked='checked'" : '') ?> > <?= ("Sim"); ?> &nbsp;
					   <input type="radio" name="perdescricao" id="perdescricao" value="f" onclick="exibirCamposDescArq();" <?=($perdescricao == 'f'  ? "checked='checked'" : '') ?> > <?= ("Não"); ?>
				   </td>
				</tr>

				<tr id="dscArquivoQuantidade" style="display: none;">
					<td class="subtitulodireita" style="width: 20%;">
					   <b><?= ("Tamanho da Descrição:"); ?></b>
				    </td>
					<td >
					   <?php
					       $perqtdchardescricao = $perqtdchardescricao ? $perqtdchardescricao : 250;
					       echo campo_texto('perqtdchardescricao', 'S', 'S', 'Quantidade máxima de caracteres permitidos na descrição', 15, 5, '[#]', '', '', '', 0, 'id="perqtdchardescricao"', '', $perqtdchardescricao );
				       ?>
					</td>
				</tr>

				<tr id="dscArquivoObrigatoria" style="display: none;">
					<td class="subtitulodireita" width="20px;">
					   <b><?= ("Descrição obrigatória:"); ?></b>
				    </td>
					<td>
					   <input type="radio" name="perdescricaoobrigatoria" id="perdescricaoobrigatoria" value="t" <?=($perdescricaoobrigatoria == 't' || $perdescricaoobrigatoria == '' ? "checked='checked'" : '') ?> > <?= ("Sim"); ?> &nbsp;
					   <input type="radio" name="perdescricaoobrigatoria" id="perdescricaoobrigatoria" value="f" <?=($perdescricaoobrigatoria == 'f'  ? "checked='checked'" : '') ?> > <?= ("Não"); ?>
				   </td>
				</tr>

				<tr id="itens" style="display:none;">
					<td colspan="2" align="center">
						<div style="width: 95%; text-align: right;"><a style="cursor:pointer" onclick="addItem()"><?= ("Novo Item"); ?></a></div>
						<fieldset style="background: #FFF;">
							<legend><?= ("Itens"); ?></legend>
							<div style="overflow: auto; height: 100px;">
							<table class="tabela" id="tableItem">
								<tr>
									<th width="10%"><?= ("Ação"); ?></th>
									<th><?= ("Descrição"); ?></th>
								</tr>
							</table>
							</div>
						</fieldset>
					</td>
				</tr>

				<tr id="mascaraData" style="display:none;">
					<td class="subtitulodireita" style="width: 20%;">
					   <b><?= ("Formato Data"); ?>:</b>
				   </td>
					<td >
					    <?php
					        $mascaradata = $permascara;
                            $dados = array(array("codigo" => "dd/mm/yyyy", "descricao" => "dd/mm/yyyy"),
                                           array("codigo" => "mm/dd/yyyy", "descricao" => "mm/dd/yyyy"),);
                            $db->monta_combo("mascaradata", $dados, 'S', ('Selecione...'), '', '', '', '130', 'S', '', '', $mascaradata, '' );
					    ?>
					</td>
				</tr>

				<tr>
					<th colspan="2">
						<input type="button" value="<?= ("Salvar"); ?>" name="btnSalvar" id="btnSalvar" onclick="salvarPergunta();">&nbsp;
						<input type="button" value="<?= ("Voltar"); ?>" name="btnVoltar" id="btnVoltar" onclick="window.close();">
					</th>
				</tr>
			</table>
<!--		</div>-->
	</form>
<script type="text/javascript">
<?php
if($obPergunta->perid){

	foreach( $arrItens as $item ):
		echo "addItem(['{$item['resid']}', '{$item['itpid']}', '{$item['itptitulo']}', '{$item['grpid']}', '{$item['perid']}']);";
	endforeach;
}
?>

var ajudaMascara = new ToolTip();
	ajudaMascara.config({url	: '?modulo=sistema/questionario/popup/cadastroPergunta&acao=A&ajudaMascara=1&ajax=1',
						id    	: 'ajudaMascara',
						cache	: false,
						css		: {width   : '500px',
									height : '200px'}});

var ajudaRegra = new ToolTip();
	ajudaRegra.config({url   	: '?modulo=sistema/questionario/popup/cadastroPergunta&acao=A&ajudaRegra=1&ajax=1',
					id    	: 'ajudaRegra',
					cache	: false,
					css   	: {width   : '500px',
							 	height : '665px'}});

$(document).ready(function (){ tipoPergunta('<?=$obPergunta->pertipo?>'); });
</script>
</body>
</html>