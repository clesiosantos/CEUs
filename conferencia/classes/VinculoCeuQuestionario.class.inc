<?php

class VinculoCeuQuestionario extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.vinculoceuquestionario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vcqid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'vcqid'         => null,
                                    'qrpid'         => null,
                                    'docid'         => null,
                                    'vcqstatus'     => null,
                                    'usucpf'        => null,
                                    'vcqdtcadastro' => null,
                                    'ceuid'         => null
                                  );

    /**
     * Recupera o questionário resposta, se não existir cria de acordo com o parâmtro
     */
    public function getDadosQuestionarioRespostaAtualPorVinculoQuestionario($vceid, $queid, $criarQrpid = false){
        $mVinculoCeu = new VinculoCeu($vceid);

        $sql = "SELECT
                    *
                FROM
                    {$this->stNomeTabela} vcq
                INNER JOIN
                    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                                                         AND qrp.queid = {$queid}
                WHERE
                    vcq.ceuid = {$mVinculoCeu->ceuid}
                    AND
                    vcq.vcqstatus = 'A'";
        $arDados = $this->pegaLinha($sql);

        if(!$arDados && $criarQrpid){
            $this->criarQuetionarioRespostaPorVinculoQuestionario($vceid, $queid);
            $this->commit();

            $arDados = $this->getDadosQuestionarioRespostaAtualPorVinculoQuestionario($vceid, $queid);
        }

        return $arDados ? $arDados : array();
    }

    public function getDadosQuestionarioRespostaPorVinculoQuestionario($vceid, $vcqid){
        $mVinculoCeu = new VinculoCeu($vceid);

        $sql = "SELECT
                    *,
                    TO_CHAR(vcq.vcqdtcadastro, 'DD/MM/YYYY as  HH:mi') AS vcqdtcadastroformatada
                FROM
                    {$this->stNomeTabela} vcq
                WHERE
                    vcq.ceuid = {$mVinculoCeu->ceuid}
                    AND
                    vcq.vcqid = '{$vcqid}'";

        $arDados = $this->pegaLinha($sql);
        return $arDados ? $arDados : array();
    }

    public function criarQuetionarioRespostaPorVinculoQuestionario($vceid, $queid){
        $mVinculoCeu = new VinculoCeu($vceid);
        $arDadosQVinculoAntigo = $this->getDadosQuestionarioRespostaAtualPorVinculoQuestionario($vceid, $queid);

        $this->definirRespostasComoHistoricoPorVinculoQuestionario($vceid, $queid);


        $mResposta = new QQuestionarioResposta();
        $arDadosQQResposta = array(
    								  'queid' 	            => $queid,
    								  'qrptitulo'           => "Respostas do Vínculo de Centros de ARtes e Esportes Unificados",
    								  'qrpdata' 	        => 'now()',
    								  'qrpfinalizado'       => null,
                                      'qrpfinalizadousucpf' => null,
                                      'qrpfinalizadodata'   => null
							       );

        $qrpid = $mResposta->popularDadosObjeto($arDadosQQResposta)->salvar();
        $arDados = array(
                           'qrpid'         => $qrpid,
                           'docid'         => wf_cadastrarDocumento(WF_QUESTIONARIO, 'Fluxo de Questionários'),
                           'ceuid'         => $mVinculoCeu->ceuid,
                           'vcqstatus'     => 'A',
                           'usucpf'        => $_SESSION["usucpf"],
                           'vcqdtcadastro' => "now()"
                         );

        $vcqid = $this->popularDadosObjeto($arDados)->salvar();


        if($arDadosQVinculoAntigo){
            $mDocWorkflow = new Documento();
            $mDocWorkflow->duplicarHistoricoWorkflow($arDadosQVinculoAntigo['docid'], $this->docid);
        }

        return $vcqid;
    }

    public function definirRespostasComoHistoricoPorVinculoQuestionario($vceid, $queid){
        $mVinculoCeu = new VinculoCeu($vceid);

        $sql = "UPDATE
                    {$this->stNomeTabela}
                SET
                    vcqstatus = 'H'
                WHERE
                    ceuid = {$mVinculoCeu->ceuid}
                    AND
                    qrpid IN (SELECT
                                  qrpid
                              FROM
                                  questionario.questionarioresposta
                              WHERE
                                  queid = {$queid});";

        $this->executar($sql);
    }

    public function listarQuestionarioHistorico($vceid, $queid){
        $mVinculoCeu = new VinculoCeu($vceid);


        $sql = "SELECT
                    CASE WHEN vcq.vcqstatus = 'A' THEN
                        'Atual'
                    ELSE
                        'Histórico'
                    END AS tipo,
                    esd.esddsc,
                    usu.usunome,
                    TO_CHAR(vcq.vcqdtcadastro, 'DD/MM/YYYY as HH:MI') AS vcsdtcadastroformatado,
                    vcq.vcqid,
                    vcq.qrpid
                FROM
                    {$this->stNomeTabela} vcq
                INNER JOIN
                    workflow.documento doc ON doc.docid = vcq.docid
                INNER JOIN
                    workflow.estadodocumento esd ON esd.esdid = doc.esdid
                INNER JOIN
                    seguranca.usuario usu ON usu.usucpf = vcq.usucpf
                INNER JOIN
                    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                WHERE
                    vcq.ceuid = {$mVinculoCeu->ceuid}
                    AND
                    qrp.queid = {$queid}
                    AND
                    vcq.vcqstatus != 'I'
                ORDER BY
                    vcq.vcqdtcadastro ASC";

                    $arDados = $this->carregar($sql);
        return $arDados ? $arDados : array();
    }

    public function duplicarQuestionario($vcqidDuplicar){
        $mVinculoCeuQuestionarioDuplicar = new VinculoCeuQuestionario($vcqidDuplicar);

        $mVinculoCeu = new VinculoCeu();
        $vceid = $mVinculoCeu->recuperarUm("vceid", array("ceuid = {$mVinculoCeuQuestionarioDuplicar->ceuid}"));

        $mQQResposta     = new QQuestionarioResposta($mVinculoCeuQuestionarioDuplicar->qrpid);
        $mQResposta      = new QResposta();
        $mTabelaResposta = new Tabelaresposta();
        $mQGrupo         = new QGrupo();

        $vcqidNovo = $this->criarQuetionarioRespostaPorVinculoQuestionario($vceid, $mQQResposta->queid);
        $mVinculoCeuQuestionarioNovo = new VinculoCeuQuestionario($vcqidNovo);

        $arIdOriginalPerguntas = array();
        $gruposMultiplos       = $mQGrupo->getGruposMultiplosByQuestionario($mQQResposta->queid, $mVinculoCeuQuestionarioDuplicar->qrpid);

        foreach($gruposMultiplos as $grupoMultiplo){
            if($grupoMultiplo["grpid"] != $grupoMultiplo["grpid_real"]){
                $arDadosDuplicados     = $mQGrupo->duplicaGrupo($grupoMultiplo["grpid_real"], $mVinculoCeuQuestionarioNovo->qrpid, $mVinculoCeuQuestionarioDuplicar->qrpid, $grupoMultiplo["grpid"]);
            }
        }

        $arRespostas = $mQResposta->listaObjeto(array("qrpid" => $mVinculoCeuQuestionarioDuplicar->qrpid));
        $arRespostas = $arRespostas ? $arRespostas : array();
        foreach($arRespostas as $resposta){
            $resposta->copiarResposta($mVinculoCeuQuestionarioNovo->qrpid);
        }

        $arTabelaResposta = $mTabelaResposta->recuperarTodos("*", array("qrpid = {$mVinculoCeuQuestionarioDuplicar->qrpid}"));
        foreach($arTabelaResposta as $tabelaResposta){
            $arDadosTabelaResp  = array(
									  	'tblid'  => $tabelaResposta["tblid"],
									  	'linid'  => $tabelaResposta["linid"],
									  	'colid'  => $tabelaResposta["colid"],
									  	'qrpid'  => $mVinculoCeuQuestionarioNovo->qrpid,
									  	'tbrdsc' => $tabelaResposta["tbrdsc"],
									  );
            $mTabelaResposta->popularDadosObjeto($arDadosTabelaResp)->salvar();
            $mTabelaResposta->setDadosNull();
        }
    }

    public function recuperarIdPerguntasOriginais($arDadosDuplicados){
        $arPeridOriginais = array();
        if(is_array($arDadosDuplicados)){
            foreach($arDadosDuplicados as $arItensDuplicados){
                foreach($arItensDuplicados as $dadosDuplicados){
                    if($dadosDuplicados['tipo'] == 'pergunta' && $dadosDuplicados['original'] != ""){
                        $arPeridOriginais[] = $dadosDuplicados['original'];
                    }

                    if(is_array($dadosDuplicados["itens"])){
                        $arPeridOriginais = array_merge($arPeridOriginais, $this->recuperarIdPerguntasOriginais($dadosDuplicados["itens"]));
                    }
                }
            }
        }

        return $arPeridOriginais;
    }

    public function comboItemPerguntaRhPorPergunta($perid){
    	$sql = "SELECT
					itp.itptitulo as codigo,
					itp.itptitulo as descricao
				FROM
					questionario.itempergunta as itp
				WHERE
					itp.perid = {$perid}";

    	$rs = $this->carregar($sql);
    	return $rs ? $rs : array();
    }

}