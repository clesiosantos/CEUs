<?php

/*
 * INICIO - Funções workflow questionários
 */

	// Preenchimento - Concluir
	// Condição: todos os campos preenchidos e aba não bloqueada + adicionais da aba.
	function wfCondicaoConcluir($vcqid,$queid,$abacod_tela,$url){
		//validação em javascript se a todos os campos estão preenchidos
		$habilitado	= "";

		$modelo 		   = new VinculoCeuQuestionario($vcqid);
		$mConfiguracao     = new ConfiguracaoLiberacao();
		$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $modelo->ceuid);

		if($habilitado == 'S'){
			return true;
		}else{
			return 'Não é possível executar essa ação, a aba se encontra bloqueada';
		}
	}

	function wfPosAcaoConcluir($vcqid){
		//não possui
		return  true;
	}

	//Concluído - Retornar para correção
	// Condição: aba não bloqueada
	// Pós Ação: Cópiar questionário
	function wfCondicaoRetornarParaCorrecao($vcqid,$queid,$abacod_tela,$url){
		$habilitado	= "";

		$modelo 		   = new VinculoCeuQuestionario($vcqid);
		$mConfiguracao     = new ConfiguracaoLiberacao();
		$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $modelo->ceuid);

		if($habilitado == 'S'){
			return true;
		}else{
			return 'Não é possível executar essa ação, a aba se encontra bloqueada';
		}
	}

	function wfPosAcaoRetornarParaCorrecao($vcqid,$queid){

		if($vcqid){
			$mVinculoQuestionario  = new VinculoCeuQuestionario();
			$mVinculoQuestionario->duplicarQuestionario($vcqid);
			$mVinculoQuestionario->commit();
		}else{
			return 'Algumas das variáveis necessárias estão faltando!';
		}

		return  true;
	}

	// Concluído - Iniciar com dados anteriores
	// Condição: aba não bloqueada
	// Pós Ação: Cópiar questionário
	function wfCondicaoIniciarDadosAnteriores($vcqid,$queid,$abacod_tela,$url){
		$habilitado	= "";

		$modelo 		   = new VinculoCeuQuestionario($vcqid);
		$mConfiguracao     = new ConfiguracaoLiberacao();
		$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $modelo->ceuid);

		if($habilitado == 'S'){
			return true;
		}else{
			return 'Não é possível executar essa ação, a aba se encontra bloqueada';
		}
	}

	function wfPosAcaoIniciarDadosAnteriores($vcqid,$queid){

		if($vcqid){
			$mVinculoQuestionario  = new VinculoCeuQuestionario();
			$mVinculoQuestionario->duplicarQuestionario($vcqid);
			$mVinculoQuestionario->commit();
		}else{
			return 'Algumas das variáveis necessárias estão faltando!';
		}

		return  true;
	}

	// Concluído - Iniciar com dados em branco
	// Condição: aba não bloqueada
	// Pós Ação: Cópia do questionario: SIM, mas inicia com um em branco
	function wfCondicaoIniciarDadosEmBranco($vcqid,$queid,$abacod_tela,$url){
		$habilitado	= "";
		global $db;

		$modelo 		   = new VinculoCeuQuestionario($vcqid);
		$mQResposta        = new QQuestionarioResposta($modelo->qrpid);

		if($mQResposta->queid != TIPO_QUESTIONARIO_UGL_GG && possui_perfil(CONFERENCIA_PERFIL_CADASTRO) && !$db->testa_superuser()){
		    return "Esta ação não é permitida para este questionário.";
		}else{
    		$mConfiguracao     = new ConfiguracaoLiberacao();
    		$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $modelo->ceuid);

    		if($habilitado == 'S'){
    			return true;
    		}else{
    			return 'Não é possível executar essa ação, a aba se encontra bloqueada';
    		}
		}


	}

	function wfPosAcaoIniciarDadosEmBranco($vcqid,$queid){
		$mVinculoCeu = new VinculoCeu();
		$vceid       = $mVinculoCeu->getVinculoSession();

		if(!empty($vceid) && !empty($queid)){
			$mVinculoQuestionario  = new VinculoCeuQuestionario();
			$mVinculoQuestionario->criarQuetionarioRespostaPorVinculoQuestionario($vceid, $queid);
			$mVinculoQuestionario->commit();
		}else{
			return 'Algumas das variáveis necessárias estão faltando!';
		}

		return  true;
	}

	//Correção - Concluir
	// Condição: todos os campos preenchidos e aba não bloqueada + adicionais da aba.
	function wfCondicaoConcluirCorrecao($vcqid,$queid,$abacod_tela,$url){
		//validação em javascript
		$habilitado	= "";

		$modelo 		   = new VinculoCeuQuestionario($vcqid);
		$mConfiguracao     = new ConfiguracaoLiberacao();
		$habilitado        = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $modelo->ceuid);

		if($habilitado == 'S'){
			return true;
		}else{
			return 'Não é possível executar essa ação, a aba se encontra bloqueada';
		}
	}

	function wfPosAcaoConcluirCorrecao($vcqid){
		//não possui
		return  true;
	}


/*
 * FIM - Funções workflow questionários
 */

function possui_perfil($perfil)
{

    global $db;

    if (!is_array($perfil))
        $perfil = Array($perfil);

    $sql = "SELECT
				count(1)
			FROM
				seguranca.perfilusuario
			WHERE
				usucpf = '{$_SESSION['usucpf']}'
				AND pflcod in ('" . implode("','", $perfil) . "')";

    return (boolean) $db->pegaUm($sql);
}

function getResponsabilidadesPreCadastro($pcnid, $campo = 'usu.usuemail')
{
    global $db;

    $sql = "SELECT DISTINCT
                {$campo}
            FROM
                seguranca.usuario usu
            INNER JOIN
                seguranca.perfilusuario pfu ON pfu.usucpf = usu.usucpf
            INNER JOIN
                seguranca.perfil pfl ON pfu.pflcod = pfl.pflcod
                                    AND pfl.sisid = {$_SESSION['sisid']}
                                    AND pfl.pflstatus = 'A'
            INNER JOIN
                seguranca.usuario_sistema uss ON uss.usucpf = usu.usucpf
                                             AND uss.sisid = {$_SESSION['sisid']}
                                             AND uss.suscod = 'A'
                                             AND uss.susstatus = 'A'
            INNER JOIN
                conferencia.usuarioresponsabilidade rpu ON rpu.usucpf = usu.usucpf
                                                       AND rpu.pflcod = '" . CONFERENCIA_PERFIL_CADASTRO . "'
                                                       AND rpu.pcnid = {$pcnid};";

    $resp = $db->carregarColuna($sql);
    return $resp ? $resp : array();
}

function getWhereResponsabilidadePreCadastro()
{
    global $db;
    if (!$db->testa_superuser() && possui_perfil(CONFERENCIA_PERFIL_CADASTRO)) {
        $sql  = "SELECT
                    pcn.pcnid
                FROM
                    conferencia.usuarioresponsabilidade usr
                LEFT JOIN
                    conferencia.preconferencia AS pcn ON usr.pcnid = pcn.pcnid
                WHERE
                    usr.rpustatus = 'A'
                    AND
                    NOT usr.pcnid IS NULL
                    AND
                    usr.usucpf = '{$_SESSION['usucpf']}'
                    AND
                    usr.pflcod = '" . CONFERENCIA_PERFIL_CADASTRO . "';";
        $resp = $db->carregarColuna($sql);

        if (is_array($resp) && count($resp) > 0) {
            return "pcn.pcnid IN (" . implode(", ", $resp) . ")";
        } else {
            return "false";
        }
    } else {
        return "true";
    }
}

function verificaResponsabilidadePreCadastro($pcnid = null, $verificaEstadoWf = false)
{
    require_once APPRAIZ . "includes/classes/Modelo.class.inc";
    require_once APPRAIZ . "conferencia/classes/PreConferencia.class.inc";

    global $db;

    $pcnid = is_null($pcnid) ? $_REQUEST['pcnid'] : $pcnid;

    $mPreConf  = new PreConferencia($pcnid);
    $dadosConf = $mPreConf->getTodosDados();

    if ( !in_array($dadosConf['esdid']  , array(WF_CEU_CONCLUIDA, WF_CEU_APROVADO))) {
        die("<script type='text/javascript'>
                alert('Esta conferência não foi aprovada!');
                history.back(-1);
            </script>");
    }

    if (!$db->testa_superuser() && possui_perfil(array(CONFERENCIA_PERFIL_CADASTRO))) {
        $arUsuResp = getResponsabilidadesPreCadastro($pcnid, 'usu.usucpf');
        if (is_array($arUsuResp)) {
            if (!in_array($_SESSION['usucpf'], $arUsuResp)) {
                die("<script type='text/javascript'>
                    alert('Você não possui responsabilidade nesta conferência!');
                    history.back(-1);
                </script>");
            }
        } else {
            die("<script type='text/javascript'>
                alert('Você não possui responsabilidade nesta conferência!');
                history.back(-1);
            </script>");
        }
    }
}

/**
 * Cadastra um usuário para o perfil parametrizado no sistema da SESSION.
 * Além de cadastrar, o usuário já será ativado
 * @param array $dadosUsuario
 * @param integer $pflcod
 * @return array Array com a senha do usuário (caso tenha sido cadastrado) e dois booleanos
 */
function cadastrarUsuario($dadosUsuario, $pflcod)
{
    global $db;

    $sisid = $_SESSION['sisid'];

// verifica se o cpf já está cadastrado no sistema
    $sql = sprintf(
            "SELECT
			u.ususexo,
			u.usucpf,
			u.regcod,
			u.usunome,
			u.usuemail,
			u.usustatus,
			u.usufoneddd,
			u.usufonenum,
			u.ususenha,
			u.usudataultacesso,
			u.usunivel,
			u.usufuncao,
			u.ususexo,
			u.entid,
			u.unicod,
			u.usuchaveativacao,
			u.usutentativas,
			u.usuobs,
			u.ungcod,
			u.usudatainc,
			u.usuconectado,
			u.suscod,
			u.muncod,
			u.carid
		FROM
			seguranca.usuario u
		WHERE
			u.usucpf = '%s'", $dadosUsuario['usucpf']
    );

    $usuario       = (object) $db->pegaLinha($sql);
    $cpfCadastrado = $usuario->usucpf ? true : false;

// verifica se o usuário já está cadastrado no módulo selecionado
    $sql            = sprintf("SELECT usucpf, sisid, suscod FROM usuario_sistema WHERE usucpf = '%s' AND sisid = %d", $dadosUsuario['usucpf'], $sisid);
    $usuarioSistema = (object) $db->pegaLinha($sql);
    $cpfCadSistema  = $usuarioSistema->sisid ? true : false;

    $senha = '';
    if (!$cpfCadastrado) {
// Gerar senha
        $senha = $db->gerar_senha();

// insere informações gerais do usuário
        $sql = sprintf(
                "INSERT INTO seguranca.usuario (
					usucpf, usunome, usuemail, usufoneddd, usufonenum,
					usufuncao, carid, entid, unicod, usuchaveativacao, regcod,
					ususexo, ungcod, ususenha, suscod, orgao,
					muncod, tpocod, usurgnum, usuorgexp, usurguf
				) values (
					'%s',
                    '%s',
                    '%s',
                    '%s',
                    '%s',
				    %s,
                    %s,
                    %s,
                    %s,
                    '%s',
					'%s',
                    '%s',
                    %s,
                    '%s',
                    '%s',
					%s,
                    '%s',
                    %s,
                    %s,
                    %s,
                    %s
				);",
                $dadosUsuario['usucpf'],
                str_to_upper( $dadosUsuario['usunome'] ),
                strtolower( $dadosUsuario['usuemail'] ),
                $dadosUsuario['usufoneddd'],
                $dadosUsuario['usufonenum'],
                'NULL',
                'NULL',
                'NULL',
                'NULL',
                'f',
                $dadosUsuario['regcod'],
                $dadosUsuario['ususexo'],
                'NULL',
                md5_encrypt_senha( $senha, '' ),
                'A',
                'NULL',
                $dadosUsuario['muncod'],
                'NULL', $dadosUsuario['usurgnum'] ? "'{$dadosUsuario['usurgnum']}'" : 'NULL',
                $dadosUsuario['usuorgexp'] ? "'{$dadosUsuario['usuorgexp']}'" : 'NULL',
                $dadosUsuario['usurguf'] ? "'{$dadosUsuario['usurguf']}'" : 'NULL'
        );
        $db->executar($sql);
    }

    if (!$cpfCadSistema) {
        $sql = sprintf(
                "INSERT INTO seguranca.usuario_sistema ( usucpf, sisid, pflcod ) values ( '%s', %d, %d )", $dadosUsuario['usucpf'], $sisid, $pflcod
        );
        $db->executar($sql);

        $sql = sprintf(
                "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) values ( '%s', %d )", $dadosUsuario['usucpf'], $pflcod
        );
        $db->executar($sql);
    }

// Ativa o usuário no módulo
    $justificativa = "Ativação automática de usuário pelo sistema";
    $suscod        = "A";

    $db->alterar_status_usuario($dadosUsuario['usucpf'], $suscod, $justificativa, $sisid);

    $db->commit();

    $arRetorno = array('senha'           => $senha, 'cpf_cad_sistema' => $cpfCadSistema, 'cpf_cadastrado'  => $cpfCadastrado);
    return $arRetorno;
}

function validarTelaConferencia($mcoid)
{
    require_once APPRAIZ . "includes/classes/Modelo.class.inc";
    require_once APPRAIZ . "conferencia/classes/MomentoConferencia.class.inc";

    $script = "<script type='text/javascript'>
                    alert('Este cadastro não está habilitado para esta Edição da Conferência.');
                    history.back(-1);
               </script>";

    $mMomento = new MomentoConferencia($mcoid);
    $dados    = $mMomento->getDados();
    switch (true) {
        case $_REQUEST['modulo'] == 'principal/conf/informacoesConferencia' && $dados["mcoinformacoes"] == 'f':
            die($script);
            break;
        case $_REQUEST['modulo'] == 'principal/conf/documentoConferencia' && $dados["mcodocumentos"] == 'f':
            die($script);
            break;
        case $_REQUEST['modulo'] == 'principal/conf/propostaConferencia' && $dados["mcopropostas"] == 'f':
            die($script);
            break;
        case $_REQUEST['modulo'] == 'principal/conf/listarDelegado' && $dados["mcodelegados"] == 'f':
            die($script);
            break;
    }
}

/**
 * Valida tamanho e extensão de um arquivo
 *
 * @param string $nameFile Nome da posição no $_FILES
 * @return bool|string
 */
function validarArquivo($nameFile){
    $maxUpload      = 1024 * 1024 * MAX_UPLOAD; // em bytes
    $arExtPermitida = array('jpg', 'gif', 'bmp', 'png', 'pdf');

    $arquivo = $_FILES[$nameFile];

    // Array com os tipos de erros de upload do PHP
    $arMsgErro[0] = 'Não houve erro';
    $arMsgErro[1] = 'O arquivo no upload é maior do que o limite do PHP';
    $arMsgErro[2] = 'O arquivo ultrapassa o limite de tamanho especifiado no HTML';
    $arMsgErro[3] = 'O upload do arquivo foi feito parcialmente';
    $arMsgErro[4] = 'Não foi feito o upload do arquivo';

    // Verifica se houve algum erro com o upload. Se sim, exibe a mensagem do erro
    if ($arquivo['error'] != 0) {
        return "Não foi possível fazer o upload, erro: {$arMsgErro[$arquivo['error']]}";
    }

    // Faz a verificação da extensão do arquivo
    $extensao = strtolower(end(explode('.', $arquivo['name'])));
    if (!in_array($extensao, $arExtPermitida)) {
        return "Por favor, envie arquivos com as seguintes extensões: " . implode(", ", $arExtPermitida);
    }else if ($arquivo['size'] > $maxUpload) {
        return "O arquivo enviado é muito grande, envie arquivos de até ".MAX_UPLOAD."MB.";
    }

    return true;
}

function getLabelTamMaxUpload($br = true){
    return ($br === true ? "<br />" : '') .  "<label style='font-size: 9px; font-weight: bold;'>Tamanho máximo: ".MAX_UPLOAD."MB</label>";
}

function getResponsabilidadeByUsuarioPerfil($pflcod, $campo, $usucpf = null){
    global $db;
    $usucpf = $usucpf ? $usucpf : $_SESSION["usucpf"];

    $sql = "SELECT
                {$campo}
            FROM
                conferencia.usuarioresponsabilidade
            WHERE
                pflcod = {$pflcod}
                AND
                usucpf = '{$usucpf}'
                AND
                rpustatus = 'A';";
    $arResp = $db->carregarColuna($sql);
    return $arResp ? $arResp : array();
}

function getWhereResponsabilidadeVinculoCeu($prefixoTabela = "vce", $usucpf = null){
    global $db;
    $usucpf = $usucpf ? $usucpf : $_SESSION["usucpf"];
    $whereResp = "true";

    if(possui_perfil(CONFERENCIA_PERFIL_CADASTRO) && !$db->testa_superuser()){
        $arResp    = getResponsabilidadeByUsuarioPerfil(CONFERENCIA_PERFIL_CADASTRO, "vceid");
        $whereResp = count($arResp) > 0 ? "{$prefixoTabela}.vceid IN (".implode(", ", $arResp).")" : "false";
    }

    return $whereResp;
}

function testarResponsabilidadeVinculoCeuTela($redirecionar = false, $validarBranco = true, $vceid = null){
    global $db;
    $mVinculoCeu = new VinculoCeu();
    $vceid       = $vceid ? $vceid : $mVinculoCeu->getVinculoSession();

    if($vceid){
        if(possui_perfil(CONFERENCIA_PERFIL_CADASTRO) && !$db->testa_superuser()){
            $arResp    = getResponsabilidadeByUsuarioPerfil(CONFERENCIA_PERFIL_CADASTRO, "vceid");
            if(!in_array($vceid, $arResp)){
                if($redirecionar){
                    die("<script type='text/javascript'>
                            alert('Você não possui responsabilidade no CEU informado!');
                            window.location = '?modulo=principal/ceus/listar&acao=A';
                         </script>");
                }
                return false;
            }
        }
    }elseif($validarBranco){
        if($redirecionar){
            die("<script type='text/javascript'>
                    alert('Nenhum CEU foi informado!');
                    window.location = '?modulo=principal/ceus/listar&acao=A';
                 </script>");
        }
        return false;
    }

    return true;
}

function verificarEnviarParaAprovacao($vceid){
    $mVinculoCeu = new VinculoCeu($vceid);

    if($mVinculoCeu->arqiddoccpf == ""){
        return "Por favor, informe o Documento contendo o número do CPF";
    }elseif($mVinculoCeu->arqidportaria == ""){
        return "Por favor, informe a Portaria de Constituição da UGL ou outro documento comprobatório do vínculo como responsável pelas informações deste CEU";
    }

    return true;
}

function enviarEmailSec($vceid){
    global $db;

    try {
        $destinatarios = getEmailPerfil(CONFERENCIA_PERFIL_SAI);
        return enviarEmailWorkflowCeu($vceid, $destinatarios);
    } catch (Exception $e) {
        return false;
    }
}

function enviarEmailCadastrador($vceid, $inativarDemaisVinculos = true){
    global $db;

    try {
        $sql = "SELECT usuemail FROM seguranca.usuario WHERE usucpf = '{$mVinculoCeu->usucpf}'";
        $usuemail = $db->pegaUm($sql);
        $destinatarios = array($usuemail);

        $mVinculoCeu = new VinculoCeu($vceid);
        $mVinculoCeu->ativarVinculo($vceid, $inativarDemaisVinculos);
        $mVinculoCeu->commit();

        return enviarEmailWorkflowCeu($vceid, $destinatarios);
    } catch (Exception $e) {
        return false;
    }
}

function enviarEmailCadastradorSemInativar($vceid){
    return enviarEmailCadastrador($vceid, false);
}

function inativarVinculo($vceid){
    $mVinculoCeu = new VinculoCeu($vceid);
    $mVinculoCeu->vceativo = 'false';
    $mVinculoCeu->salvar();
    $mVinculoCeu->commit();

    return true;
}

function enviarEmailWorkflowCeu($vceid, $destinatarios, $inserirComentario = false){
    global $db;

    try {
        $mVinculoCeu = new VinculoCeu($vceid);
        $mCeu        = new Ceu($mVinculoCeu->ceuid);

        $sql = "SELECT
                    *
                FROM
                    workflow.documento doc
                INNER JOIN
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                LEFT JOIN
                    workflow.historicodocumento hst ON hst.docid = doc.docid
                LEFT JOIN
                    workflow.comentariodocumento cmd ON hst.hstid = cmd.hstid
                WHERE
                    doc.docid = {$mVinculoCeu->docid}
                ORDER BY
                        htddata DESC
                    LIMIT 1";

        $dadosWf = $db->pegaLinha($sql);

        $remetente    = array("nome"  => 'CEUs - Centros de Artes e Esportes Unificados', "email" => $email_from);
        $destinatario = getEmailPerfil(CONFERENCIA_PERFIL_SAI);

        $assunto  = "Vínculo CEU {$vceid} tramitado para o estado {$dadosWf['esddsc']}";
        $conteudo = 'Prezado, <br/>';
        $conteudo .= "O vínculo <strong>{$vceid}</strong> com o CEU <strong>{$mCeu->ceucodigo} - {$mCeu->ceunome}</strong> foi tramitado para o estado {$dadosWf['esddsc']}";

        if($dadosWf['cmddsc'] != ""){
            $conteudo .= " pelo seguinte motivo: <br />{$dadosWf['cmddsc']}";
        }else{
            $conteudo .= ".";
        }

        enviar_email($remetente, '', $assunto, $conteudo, $destinatarios, '');
        return true;
    } catch (Exception $e) {
        return false;
    }
}

function verificarCeuInaugurado($vceid){
    $mDadosIniciais = new DadosIniciais();
    $dadosInicias   = $mDadosIniciais->getDadosByVinculoCeu($vceid);
    return ($dadosInicias['divdtinauguracao'] == '' ? false : true);
}

function verificarCeuNaoInaugurado($vceid){
    return !verificarCeuInaugurado($vceid);
}

function verificarRespostaPerguntaGGConstituidoSim($vceid){
    $queid       = TIPO_QUESTIONARIO_UGL_GG;

    $mVinculoQuestionario  = new VinculoCeuQuestionario();
    $arDadosQQResposta     = $mVinculoQuestionario->getDadosQuestionarioRespostaAtualPorVinculoQuestionario($vceid, $queid, true);

    $mResposta = new QResposta();
    $mResposta->recuperarLinha("*", array("perid = " . PERGUNTA_UGL_GG_CONSTITUIDO, "qrpid = {$arDadosQQResposta["qrpid"]}"));

    return $mResposta->itpid == ITEM_PERGUNTA_UGL_GG_CONSTITUIDO_SIM ? true : false;
}



function verificaCamposCeuInaugurado($vceid){
	return verificarRespostaPerguntaGGConstituidoSim($vceid) ? verificarCeuInaugurado($vceid) : false;
}

function verificaCamposCeuNaoInaugurado($vceid){
	return verificarRespostaPerguntaGGConstituidoSim($vceid) ? !verificarCeuInaugurado($vceid) : false;
}

function verificarVinculosAtivos($vceid){
    $mVinculoCeu = new VinculoCeu($vceid);
    $arOutrosVinculos = $mVinculoCeu->recuperarVinculosAtivos($vceid, true);

    if(is_array($arOutrosVinculos) && count($arOutrosVinculos) > 0){
        return true;
    }else{
        $mCeu = new Ceu($mVinculoCeu->ceuid);
        $msgErro = "Não existe nenhum outro vínculo ativo com o CEU {$mCeu->ceucodigo} - {$mCeu->ceunome}";
        return $msgErro;
    }
}

function calcularQuantidadeGruposGGTipoMembro($qrpid, $itpid){
    global $db;

    $sql = "SELECT
            	count(*) as qtd
            FROM
            	questionario.grupopergunta grp
            INNER JOIN
            	questionario.pergunta per ON per.grpid = grp.grpid
            				 AND per.perstatus = 'A'
            INNER JOIN
            	questionario.itempergunta itp ON itp.perid = per.perid
            				     AND itp.itpstatus = 'A'
            				     AND itp.itptitulo = (SELECT itptitulo FROM questionario.itempergunta WHERE itpid = {$itpid}) -- ITEM_PERGUNTA_UGL_GG_MEMBRO_SOCIEDADE
            INNER JOIN
            	questionario.resposta res ON res.itpid = itp.itpid
            				 AND res.qrpid = {$qrpid} -- ID DO QUESTIONÁRIO RESPOSTA
            WHERE
            	(grp.grpid_pai = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG." OR grp.grpid = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG.")
            	AND
            	grp.grpstatus = 'A'
                AND
            	(grp.qrpid = {$qrpid} OR grp.qrpid IS NULL)";

    return $db->pegaUm($sql);
}
