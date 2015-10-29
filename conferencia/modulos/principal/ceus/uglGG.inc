<?php
testarResponsabilidadeVinculoCeuTela(true);

$mVinculoCeu = new VinculoCeu();
$vceid       = $mVinculoCeu->getVinculoSession();
$queid       = TIPO_QUESTIONARIO_UGL_GG;
$subTitulo   = obrigatorio() . 'Indica Campo Obrigatório.';

$mDadosIniciais = new DadosIniciais();
$mDadosIniciais->verificarSeExistePorVinculo($vceid, true);

$mConfiguracao     = new ConfiguracaoLiberacao();
$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $mVinculoCeu->ceuid);
$barrasHabilitadas = $habilitado;

$mVinculoQuestionario  = new VinculoCeuQuestionario();
$arDadosQQResposta     = $mVinculoQuestionario->getDadosQuestionarioRespostaAtualPorVinculoQuestionario($vceid, $queid, true);

$arDadosEstadoWf = wf_pegarEstadoAtual($arDadosQQResposta['docid']);
$habilitado = in_array($arDadosEstadoWf['esdid'], array(WF_QUESTIONARIO_PREENCHIMENTO, WF_QUESTIONARIO_CORRECAO)) ? $habilitado : 'N';

//adicionando as perguntas que são de grupos
$grupo = new QGrupo();
$_SESSION['questionario']['grupoMultiplo'] = $grupo->getGruposMultiplosByQuestionario($queid, $arDadosQQResposta['qrpid']);

if($_REQUEST["act"] == "novoQuestionario"){
    $mVinculoQuestionario->criarQuetionarioRespostaPorVinculoQuestionario($vceid, $queid);
    $mVinculoQuestionario->commit();
    $db->sucesso($_REQUEST["modulo"]);
}elseif($_REQUEST["act"] == "historicoQuestionario"){
    include_once APPRAIZ . "conferencia/modulos/principal/ceus/historicoQuestionario.inc";
    die();
}elseif($_REQUEST["act"] == "validaTramitacao"){
	$vcqid = $_REQUEST['vcqid'];
	$queid = $_REQUEST['queid'];

	$mVinculoQuestionario  = new VinculoCeuQuestionario($vcqid);

	//Validando o total de respondido
	$modeloQuestionario = new GerenciaQuestionario(array("qrpid" => $mVinculoQuestionario->qrpid , "queid" => $queid, "vceid" => $vceid, "dadoscondicao" => array("vceid" => $vceid)));
	$dados = $modeloQuestionario->buscaTotalRespostas();

	//Validando a quantidade de grupos multiplos
	$modeloResposta = new QResposta();
	$mensagem		= "";

	$qtdMembrosSociedade 		= $modeloResposta->pegaResposta(PERGUNTA_UGL_GG_NUMERO_MEMBROS_SOCIEDADE,$mVinculoQuestionario->qrpid);
	$qtdGrupoMembrosSociedade 	= calcularQuantidadeGruposGGTipoMembro($mVinculoQuestionario->qrpid,ITEM_PERGUNTA_UGL_GG_MEMBRO_SOCIEDADE);

	if($qtdMembrosSociedade != '' && ($qtdMembrosSociedade != $qtdGrupoMembrosSociedade)){
		$mensagem = 'A quantidade de membros da sociedade definida está diferente da quantidade cadastrada';
	}else{
		$qtdMembrosEntorno 		= $modeloResposta->pegaResposta(PERGUNTA_UGL_GG_NUMERO_MEMBROS_ENTORNO,$mVinculoQuestionario->qrpid);
		$qtdGrupoMembrosEntorno = calcularQuantidadeGruposGGTipoMembro($mVinculoQuestionario->qrpid,ITEM_PERGUNTA_UGL_GG_MEMBRO_ENTORNO);

		if($qtdMembrosEntorno != $qtdGrupoMembrosEntorno){
			$mensagem = 'A quantidade de membros do entorno definida está diferente da quantidade cadastrada';
		}else{
			$qtdMembrosPoderPublico 		= $modeloResposta->pegaResposta(PERGUNTA_UGL_GG_NUMERO_MEMBROS_PODER_PUBLICO,$mVinculoQuestionario->qrpid);
			$qtdGrupoMembrosPoderPublico 	= calcularQuantidadeGruposGGTipoMembro($mVinculoQuestionario->qrpid,ITEM_PERGUNTA_UGL_GG_MEMBRO_PODER_PUBLICO);

			if($qtdMembrosPoderPublico != $qtdGrupoMembrosPoderPublico){
				$mensagem = 'A quantidade de membros do poder público definida está diferente da quantidade cadastrada';
			}
		}
	}

    die( json_encode( array("respondido" => $dados['respondidas'] == $dados['total'] ? 'true' : 'false', 'quantidade' => utf8_encode($mensagem))) );

}elseif($_REQUEST["act"] == "duplicarQuestionario"){
    $vcqidDuplicar = $_REQUEST["vcqid"];

    $mVinculoQuestionario->duplicarQuestionario($vcqidDuplicar);
    $mVinculoQuestionario->commit();

    $db->sucesso($_REQUEST["modulo"]);
}elseif(!isset($_REQUEST['ajax'])) {
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br />";

    echo $db->cria_aba($abacod_tela, $url, $parametros);
    echo $mVinculoCeu->desenharCabecalho(true);
    monta_titulo( $titulo_modulo, $subTitulo );
}

if($_GET["vcqid"]){
    $vcqid             = $_GET["vcqid"];
    $vcqidAtual        = $arDadosQQResposta["vcqid"];
    $arDadosQQResposta = $mVinculoQuestionario->getDadosQuestionarioRespostaPorVinculoQuestionario($vceid, $vcqid);
    $subTitulo         = "Visualização de histórico do questionário de {$arDadosQQResposta["vcqdtcadastroformatada"]}";
    $habilitado        = ($vcqid == $vcqidAtual) ? $habilitado : 'N';
}


if(!isset($_REQUEST['ajax'])) {
?>
<table bgcolor="#f5f5f5" align="center" class="tabela" >
	<tr>
		<td >
<?php
}

$tela = new Tela(array("qrpid" => $arDadosQQResposta['qrpid'], 'tamDivPx' => 355, 'habilitado'=> $habilitado, 'dadoscondicao' => array('vceid' => $vceid), 'relatorio' => 'S'));

if(!isset($_REQUEST['ajax'])) {
?>
		</td>
		<td>
		  <?
 		  	echo wf_desenhaBarraNavegacao($arDadosQQResposta["docid"], array('vcqid' => ($_GET["vcqid"] ? $_GET["vcqid"] : $arDadosQQResposta['vcqid']), 'queid' => TIPO_QUESTIONARIO_UGL_GG, 'abacod_tela' => $abacod_tela,'url' => $url), array('historico' => !$db->testa_superuser()), ( $_GET["vcqid"] ? (($vcqid == $vcqidAtual) ? 'S' : 'N') : 'S' )   );
		  ?>
		  <br />

		  <table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
				<tr style="background-color: #c9c9c9; text-align:center;">
        			<td style="font-size:7pt; text-align:center; font-weight: bold;">
        				Questionário
        			</td>
        		</tr>
        		<?php if($barrasHabilitadas == 'S') : ?>
<!--             		<tr>
    					<td style="font-size: 7pt; text-align: center; border-top-width: 2px; border-top-style: solid; border-top-color: rgb(208, 208, 208);" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
     						<a href="javascript:novoQuestionario();" title="Novo Questionário">
    						    <img style="cursor: pointer;" src="/imagens/new.gif" title="" onclick=""><br />Novo
     					    </a>
     					</td>
     				</tr>
             		<tr>
    					<td style="font-size: 7pt; text-align: center; border-top-width: 2px; border-top-style: solid; border-top-color: rgb(208, 208, 208);" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
    						<a href="javascript:duplicarQuestionario(<? //$arDadosQQResposta["vcqid"]; ?>);" title="Duplicar este">
    						    <img style="cursor: pointer;" src="/imagens/duplicar.png" title="" onclick=""><br />Duplicar
     					    </a>
     					</td>
<!--     				</tr> -->
        		<?php endif; ?>
        		<tr>
					<td style="font-size: 7pt; text-align: center; border-top-width: 2px; border-top-style: solid; border-top-color: rgb(208, 208, 208);" onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
						<a href="javascript:historicoQuestionario('<?= $barrasHabilitadas; ?>');" alt="Histórico" title="" title="Histórico">
						    <img style="cursor: pointer;" src="/imagens/ico_bp_historico.png" title="" onclick=""><br />Histórico
					    </a>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript">
    function novoQuestionario(){
        if(confirm("Tem certeza que deseja um novo questionário?")){
            window.location = "conferencia.php?modulo=principal/ceus/uglGG&acao=A&act=novoQuestionario"
        }
    }

    function historicoQuestionario(habilitado){
    	janela("conferencia.php?modulo=principal/ceus/uglGG&acao=A&act=historicoQuestionario&habilitado="+habilitado,500,500);
    }

    function duplicarQuestionario(vcqid){
        if(confirm("Tem certeza que deseja um novo questionário baseado neste?")){
            window.location = "conferencia.php?modulo=principal/ceus/uglGG&acao=A&act=duplicarQuestionario&vcqid="+vcqid
        }
    }

    function consultarQuestionario(vcqid){
        window.location = "conferencia.php?modulo=principal/ceus/uglGG&acao=A&vcqid="+vcqid;
    }

    function wfCondicaoJsValidaConcluirCorrecao(vcqid, queid){
    	return valida(vcqid, queid);
    }

    function wfCondicaoJsValidaConcluir(vcqid, queid){
    	return valida(vcqid, queid);
    }

    function valida(vcqid, queid){
        //verifica se o questionário está respondido
        var validacao = '';

        $.ajax({
    		  type     : "GET",
    		  url      : "conferencia.php?modulo=principal/ceus/uglGG&acao=A",
    		  data     : { 'act' : 'validaTramitacao' ,'vcqid': vcqid, 'queid' : queid },
    		  dataType : "json",
    		  async	   : false,
    		  success  : function (retorno){
    			  validacao = retorno;
    		  }
  		});

        if(validacao.respondido == 'false'){
			alert('Não é possível executar essa ação, o questionário não está totalmente respondido!');
			return 'stop';
        }else{
	        if(validacao.quantidade && validacao.quantidade != ' '){
				alert(validacao.quantidade);
				return 'stop';
	        }
        }

		return true;
    }

</script>
<?php
}