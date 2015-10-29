<?php

class VinculoCeu extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.vinculoceu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("vceid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'vceid'           => null,
                                    'ceuid'           => null,
                                    'arqiddoccpf'     => null,
                                    'arqidportaria'   => null,
                                    'docid'           => null,
                                    'vcestatus'       => null,
                                    'vcedtcadastro'   => null,
                                    'usucpf'          => null,
                                    'vceativo'        => null
                                  );

    public function setVinculoSession($vceid){
        $_SESSION["vinculo_ceu"]["vceid"] = $vceid;
    }

    public function getVinculoSession(){
        $vceid = $_SESSION["vinculo_ceu"]["vceid"];
        if($vceid){
            $this->carregarPorId($vceid);
        }

        return $vceid;
    }

    public function limparVinculoSession(){
        unset($_SESSION["vinculo_ceu"]["vceid"]);
    }


    public function listar( $arrwhere , $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "vce.vceid,
				    	ceu.ceucodigo,
	                    ceu.ceunome AS ceunome,
				    	mun.estuf,
				    	mun.mundescricao,
				    	mun.muncod,
				    	esd.esddsc,
				    	CASE
				    	    WHEN div.edcid IS NULL THEN
			    	            'Não'
			    	        ELSE
				    	        'Sim'
	    	            END AS enderecoalterado,
				    	CASE
				    	    WHEN vceativo IS TRUE THEN
			    	            'Sim'
			    	        ELSE
				    	        'Não'
	    	            END AS ativo,
				    	usu.usunome,
				    	TO_CHAR(vce.vcedtcadastro, 'DD/MM/YYYY as HH:MI') AS vcedtcadastro";
        }


    	$arrwhere[] = " vce.vcestatus = 'A' ";

		$sql=" SELECT DISTINCT
					{$colunas}
				FROM
					{$this->stNomeTabela} vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
			    INNER JOIN
			        territorios.municipio mun ON mun.muncod = edc.muncod
			    INNER JOIN
			        workflow.documento doc ON doc.docid = vce.docid
		        INNER JOIN
		            workflow.estadodocumento esd ON esd.esdid = doc.esdid
	            INNER JOIN
	                seguranca.usuario usu ON usu.usucpf = vce.usucpf
                LEFT JOIN
                    conferencia.dadosiniciaisvinculoceu div ON div.ceuid = vce.ceuid
                                                           AND div.divstatus = 'A'
				WHERE ".
					implode(" AND ", $arrwhere);

			if(!$_POST["ordem"]){
			    $sql .= " ORDER BY 1";
			}

			if($paginacao){
				$dado = Paginacao::getPaginacao($this, $sql,30);
			return $dado;
			}else{
				$retorno = $this->carregar($sql);
				return $retorno ? $retorno : array();
			}
    }

    public function listarAbasVinculoCeu( $arrwhere , $paginacao = false, $colunasCodigoNomeCeuSeparadas = true){

        $colunaCeu = "ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome";
        if($colunasCodigoNomeCeuSeparadas){
            $colunaCeu = "ceu.ceucodigo, ceu.ceunome";
        }

    	$arrwhere[] = " vce.vcestatus = 'A' ";

    	//retirando o status do where da aba de dados iniciais
    	$arrwhereDadosIniciais = $arrwhere;
    	if(isset($arrwhereDadosIniciais['estado'])){
    		unset($arrwhereDadosIniciais['estado']);
    		$arrwhereDadosIniciais[] = ' false ';
    	}

		$sql="(SELECT
					vce.vceid,
					{$colunaCeu},
					mun.estuf as uf,
					mun.mundescricao as municipio,
					mun.muncod,
					'2. ' || mnu.mnudsc as aba,
					'' as percentualpreenchimento,
					esd.esddsc as estado,
					COALESCE(usu.usunome, usucadastro.usunome) as ultimaatualizacao,
					TO_CHAR(COALESCE(hst.htddata, vcq.vcqdtcadastro), 'DD/MM/yyyy' ) as dfultimaatualizacao,
					CASE
						WHEN col.colliberado IS TRUE OR mnu.mnuid IN (SELECT DISTINCT
																	      cle.mnuid
																	   FROM
																	      conferencia.configuracaoliberacaoexcecao cle
																	   INNER JOIN
																	      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
																	   INNER JOIN
																	      seguranca.aba_menu abm ON cle.mnuid = mnu.mnuid
																	   WHERE
																	      cle.clestatus = 'A' AND
																	      lem.lemstatus = 'A' AND
																	      cle.cledatalimite >= CURRENT_DATE AND
																	      lem.ceuid = ceu.ceuid ) THEN 'Liberada'
						WHEN col.colliberado IS FALSE THEN 'Bloqueada'
					END as situacaoaba,
					mnu.mnuid,
				    qrp.queid,
				    qrp.qrpid,
			        NULL::integer AS divid,
			        qrp.qrppreenchimento
				FROM
				     conferencia.vinculoceu vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
				INNER JOIN
				    territorios.municipio mun ON mun.muncod = edc.muncod
				LEFT JOIN
				    seguranca.menu mnu ON mnu.mnuid = " . MENU_CEU_UGL_E_GG . "
				LEFT JOIN
				    conferencia.vinculoceuquestionario vcq ON vcq.ceuid = vce.ceuid AND
				                                              vcq.vcqstatus = 'A' AND
									      					  vcq.qrpid IN (select qrpid FROM questionario.questionarioresposta WHERE queid = " . TIPO_QUESTIONARIO_UGL_GG . " )
				LEFT JOIN
				    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
				LEFT JOIN
				    workflow.documento doc ON doc.docid = vcq.docid
				LEFT JOIN
				    workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
				    workflow.historicodocumento hst ON hst.docid = doc.docid AND
							                           hst.htddata = (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE docid = doc.docid)
				LEFT JOIN
				    seguranca.usuario usu ON usu.usucpf = hst.usucpf
				LEFT JOIN
				    seguranca.usuario usucadastro ON usucadastro.usucpf = vcq.usucpf
				LEFT JOIN
				    conferencia.configuracaoliberacao col ON col.mnuid = mnu.mnuid AND
									                         col.colstatus = 'A'
				WHERE
				    ". implode(" AND ", $arrwhere) . ")

		    UNION ALL

				(SELECT
					vce.vceid,
					{$colunaCeu},
					mun.estuf as uf,
					mun.mundescricao as municipio,
					mun.muncod,
					'3. ' || mnu.mnudsc as aba,
					'' as percentualpreenchimento,
					esd.esddsc as estado,
					COALESCE(usu.usunome, usucadastro.usunome) as ultimaatualizacao,
					TO_CHAR(COALESCE(hst.htddata, vcq.vcqdtcadastro), 'DD/MM/yyyy' ) as dfultimaatualizacao,
					CASE
						WHEN col.colliberado IS TRUE OR mnu.mnuid IN (SELECT DISTINCT
																	      cle.mnuid
																	   FROM
																	      conferencia.configuracaoliberacaoexcecao cle
																	   INNER JOIN
																	      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
																	   INNER JOIN
																	      seguranca.aba_menu abm ON cle.mnuid = mnu.mnuid
																	   WHERE
																	      cle.clestatus = 'A' AND
																	      lem.lemstatus = 'A' AND
																	      cle.cledatalimite >= CURRENT_DATE AND
																	      lem.ceuid = ceu.ceuid ) THEN 'Liberada'
						WHEN col.colliberado IS FALSE THEN 'Bloqueada'
					END as situacaoaba,
					mnu.mnuid,
				    qrp.queid,
				    qrp.qrpid,
				    NULL::integer AS divid,
			        qrp.qrppreenchimento
				FROM
				     conferencia.vinculoceu vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
				INNER JOIN
				    territorios.municipio mun ON mun.muncod = edc.muncod
				LEFT JOIN
				    seguranca.menu mnu ON mnu.mnuid = " . MENU_CEU_MOBILIZACAO_SOCIAL . "
				LEFT JOIN
				    conferencia.vinculoceuquestionario vcq ON vcq.ceuid = vce.ceuid AND
				                                              vcq.vcqstatus = 'A' AND
									      					  vcq.qrpid IN (select qrpid FROM questionario.questionarioresposta WHERE queid = " . TIPO_QUESTIONARIO_MOBILIZACAO_SOCIAL . " )
				LEFT JOIN
				    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
				LEFT JOIN
				    workflow.documento doc ON doc.docid = vcq.docid
				LEFT JOIN
				    workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
				    workflow.historicodocumento hst ON hst.docid = doc.docid AND
							                           hst.htddata = (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE docid = doc.docid)
				LEFT JOIN
				    seguranca.usuario usu ON usu.usucpf = hst.usucpf
				LEFT JOIN
				    seguranca.usuario usucadastro ON usucadastro.usucpf = vcq.usucpf
				LEFT JOIN
				    conferencia.configuracaoliberacao col ON col.mnuid = mnu.mnuid AND
									                         col.colstatus = 'A'
				WHERE
				    ". implode(" AND ", $arrwhere) . " )

			 UNION ALL

				(SELECT
					vce.vceid,
					{$colunaCeu},
					mun.estuf as uf,
					mun.mundescricao as municipio,
					mun.muncod,
					'5. ' || mnu.mnudsc as aba,
					'' as percentualpreenchimento,
					esd.esddsc as estado,
					COALESCE(usu.usunome, usucadastro.usunome) as ultimaatualizacao,
					TO_CHAR(COALESCE(hst.htddata, vcq.vcqdtcadastro), 'DD/MM/yyyy' ) as dfultimaatualizacao,
					CASE
						WHEN col.colliberado IS TRUE OR mnu.mnuid IN (SELECT DISTINCT
																	      cle.mnuid
																	   FROM
																	      conferencia.configuracaoliberacaoexcecao cle
																	   INNER JOIN
																	      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
																	   INNER JOIN
																	      seguranca.aba_menu abm ON cle.mnuid = mnu.mnuid
																	   WHERE
																	      cle.clestatus = 'A' AND
																	      lem.lemstatus = 'A' AND
																	      cle.cledatalimite >= CURRENT_DATE AND
																	      lem.ceuid = ceu.ceuid ) THEN 'Liberada'
						WHEN col.colliberado IS FALSE THEN 'Bloqueada'
					END as situacaoaba,
					mnu.mnuid,
				    qrp.queid,
				    qrp.qrpid,
				    NULL::integer AS divid,
			        qrp.qrppreenchimento
				FROM
				     conferencia.vinculoceu vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
				INNER JOIN
				    territorios.municipio mun ON mun.muncod = edc.muncod
				LEFT JOIN
				    seguranca.menu mnu ON mnu.mnuid = " . MENU_CEU_RH . "
				LEFT JOIN
				    conferencia.vinculoceuquestionario vcq ON vcq.ceuid = vce.ceuid AND
				                                              vcq.vcqstatus = 'A' AND
									      					  vcq.qrpid IN (select qrpid FROM questionario.questionarioresposta WHERE queid = " . TIPO_QUESTIONARIO_RH . " )
				LEFT JOIN
				    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
				LEFT JOIN
				    workflow.documento doc ON doc.docid = vcq.docid
				LEFT JOIN
				    workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
				    workflow.historicodocumento hst ON hst.docid = doc.docid AND
							                           hst.htddata = (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE docid = doc.docid)
				LEFT JOIN
				    seguranca.usuario usu ON usu.usucpf = hst.usucpf
				LEFT JOIN
				    seguranca.usuario usucadastro ON usucadastro.usucpf = vcq.usucpf
				LEFT JOIN
				    conferencia.configuracaoliberacao col ON col.mnuid = mnu.mnuid AND
									                         col.colstatus = 'A'
				WHERE
				    ". implode(" AND ", $arrwhere) . ")

			 UNION ALL

				(SELECT
					vce.vceid,
					{$colunaCeu},
					mun.estuf as uf,
					mun.mundescricao as municipio,
					mun.muncod,
					'4. ' || mnu.mnudsc as aba,
					'' as percentualpreenchimento,
					esd.esddsc as estado,
					COALESCE(usu.usunome, usucadastro.usunome) as ultimaatualizacao,
					TO_CHAR(COALESCE(hst.htddata, vcq.vcqdtcadastro), 'DD/MM/yyyy' ) as dfultimaatualizacao,
					CASE
						WHEN col.colliberado IS TRUE OR mnu.mnuid IN (SELECT DISTINCT
																	      cle.mnuid
																	   FROM
																	      conferencia.configuracaoliberacaoexcecao cle
																	   INNER JOIN
																	      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
																	   INNER JOIN
																	      seguranca.aba_menu abm ON cle.mnuid = mnu.mnuid
																	   WHERE
																	      cle.clestatus = 'A' AND
																	      lem.lemstatus = 'A' AND
																	      cle.cledatalimite >= CURRENT_DATE AND
																	      lem.ceuid = ceu.ceuid ) THEN 'Liberada'
						WHEN col.colliberado IS FALSE THEN 'Bloqueada'
					END as situacaoaba,
					mnu.mnuid,
				    qrp.queid,
				    qrp.qrpid,
				    NULL AS divid,
			        qrp.qrppreenchimento
				FROM
				     conferencia.vinculoceu vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
				INNER JOIN
				    territorios.municipio mun ON mun.muncod = edc.muncod
				LEFT JOIN
				    seguranca.menu mnu ON mnu.mnuid = " . MENU_CEU_ARTICULACAO_SNC . "
				LEFT JOIN
				    conferencia.vinculoceuquestionario vcq ON vcq.ceuid = vce.ceuid AND
				                                              vcq.vcqstatus = 'A' AND
									      					  vcq.qrpid IN (select qrpid FROM questionario.questionarioresposta WHERE queid = " . TIPO_QUESTIONARIO_ARTICULACAO_SNC . " )
				LEFT JOIN
				    questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
				LEFT JOIN
				    workflow.documento doc ON doc.docid = vcq.docid
				LEFT JOIN
				    workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
				    workflow.historicodocumento hst ON hst.docid = doc.docid AND
							                           hst.htddata = (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE docid = doc.docid)
				LEFT JOIN
				    seguranca.usuario usu ON usu.usucpf = hst.usucpf
				LEFT JOIN
				    seguranca.usuario usucadastro ON usucadastro.usucpf = vcq.usucpf
				LEFT JOIN
				    conferencia.configuracaoliberacao col ON col.mnuid = mnu.mnuid AND
									                         col.colstatus = 'A'
				WHERE
				    ". implode(" AND ", $arrwhere) . ")

			 UNION ALL

				(SELECT
					vce.vceid,
					{$colunaCeu},
					mun.estuf as uf,
					mun.mundescricao as municipio,
					mun.muncod,
					'1. ' || mnu.mnudsc as aba,
					NULL AS percentualpreenchimento,
					NULL AS estado,
					usu.usunome AS ultimaatualizacao,
					TO_CHAR(div.divdtcadastro, 'DD/MM/yyyy') AS dfultimaatualizacao,
					CASE
						WHEN col.colliberado IS TRUE OR mnu.mnuid IN (SELECT DISTINCT
																	      cle.mnuid
																	   FROM
																	      conferencia.configuracaoliberacaoexcecao cle
																	   INNER JOIN
																	      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
																	   INNER JOIN
																	      seguranca.aba_menu abm ON cle.mnuid = mnu.mnuid
																	   WHERE
																	      cle.clestatus = 'A' AND
																	      lem.lemstatus = 'A' AND
																	      cle.cledatalimite >= CURRENT_DATE AND
																	      lem.ceuid = ceu.ceuid ) THEN 'Liberada'
						WHEN col.colliberado IS FALSE THEN 'Bloqueada'
					END as situacaoaba,
					mnu.mnuid,
				    NULL::integer AS queid,
				    NULL::integer AS qrpid,
				    div.divid,
			        100 AS qrppreenchimento
				FROM
				     conferencia.vinculoceu vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
				INNER JOIN
				    territorios.municipio mun ON mun.muncod = edc.muncod
				LEFT JOIN
				    seguranca.menu mnu ON mnu.mnuid = " . MENU_CEU_DADOS_INICIAIS . "
				LEFT JOIN
				    conferencia.configuracaoliberacao col ON col.mnuid = mnu.mnuid AND
									                         col.colstatus = 'A'
				LEFT JOIN
				    conferencia.dadosiniciaisvinculoceu div ON div.ceuid = vce.ceuid AND
				    		                                   div.divstatus = 'A'
				LEFT JOIN
				    seguranca.usuario usu ON usu.usucpf = div.usucpf
				WHERE
				    ". implode(" AND ", $arrwhereDadosIniciais) . ")";


			if(!$_POST["ordem"]){
			    $sql .= " ORDER BY 1,5";
			}

			if($paginacao){
				$dado = Paginacao::getPaginacao($this, $sql,30);
			return $dado;
			}else{
				$retorno = $this->carregar($sql);
				return $retorno ? $retorno : array();
			}
    }

    public function listarRhCeus( $arWhere = array(), $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "ceu.ceucodigo,
	                    ceu.ceunome,
            			mun.estuf as uf,
						mun.mundescricao as municipio,
	                    d.resposta[1] AS espaco,
	                    d.resposta[2] AS nome,
	                    d.resposta[3] AS sexo,
	                    d.resposta[4] AS rg,
	                    d.resposta[5] AS escolaridade,
	                    d.resposta[6] AS formacao,
	                    d.resposta[7] AS tipovinculo,
	                    d.resposta[8] AS funcao,
	                    d.resposta[9] AS cargahoraria,
	                    d.grptitulo,
	                    d.ceuid,
            			d.grpid";
        }

        $arWhere[] = "ceu.ceustatus = 'A'";
        $sql = "WITH dados AS (
                	SELECT
                		ceu.ceuid,
                		ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
                		grp.grpid,
                		grp.grptitulo,
                		per.perid,
                		per.pertitulo,
                		COALESCE(res.resdsc, itp.itptitulo) AS resposta
                	FROM
                		conferencia.ceu ceu
                	INNER JOIN
                		conferencia.vinculoceuquestionario vcq ON ceu.ceuid = vcq.ceuid
                						                      AND vcq.vcqstatus = 'A'
                	INNER JOIN
                		questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                						                     AND qrp.queid = ".TIPO_QUESTIONARIO_RH."
                	INNER JOIN
                		questionario.grupopergunta grp ON (qrp.qrpid = grp.qrpid OR grp.qrpid IS NULL )
        					                          AND grp.queid_atual = qrp.queid
        					                          AND grp.grpstatus = 'A'
                	INNER JOIN
                		questionario.pergunta per ON qrp.queid = per.queid_atual
                					             AND grp.grpid = per.grpid
                					             AND per.perstatus = 'A'
                	LEFT JOIN
                		questionario.resposta res ON res.perid = per.perid
                					             AND res.qrpid = qrp.qrpid
                	LEFT JOIN
                		questionario.itempergunta itp ON itp.itpid = res.itpid
                	WHERE
                		ceu.ceustatus = 'A'
                	ORDER BY
                		ceu.ceuid, grp.grpid, per.perid
                )
			        SELECT
			            {$colunas}
			        FROM
			            (SELECT
			            	ceuid,
			            	ceunome,
			            	grpid,
			            	grptitulo,
			            	array_agg(resposta) AS resposta
			             FROM
			        	    dados
			             GROUP BY
			        	    ceuid, ceunome, grpid, grptitulo) d
	                INNER JOIN
	                     conferencia.ceu ceu ON ceu.ceuid = d.ceuid
	                INNER JOIN
	                     conferencia.endereco edc ON edc.edcid = ceu.edcid
	                INNER JOIN
	                     territorios.municipio mun ON mun.muncod = edc.muncod
                     WHERE
                          ". implode(" AND ", $arWhere);

		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY d.ceuid, d.grpid";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 30);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }

    public function listaPopUpCeu($ceuid){

		$sql=" SELECT
					vce.vceid,
			    	ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
			    	mun.estuf,
			    	mun.mundescricao,
			    	esd.esddsc,
			    	edcdados.edccep,
			    	edcdados.edclogradouro,
			    	edcdados.edcnumero,
			    	edcdados.edccomplemento,
			    	edcdados.edcbairro,
			    	edcdados.edcbairro,
			    	mundados.mundescricao || '/' || edcdados.estuf,
			    	ceu.ceuid
				FROM
					{$this->stNomeTabela} vce
				INNER JOIN
				    conferencia.ceu ceu ON ceu.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edc ON edc.edcid = ceu.edcid
			    INNER JOIN
			        territorios.municipio mun ON mun.muncod = edc.muncod
			    INNER JOIN
			        workflow.documento doc ON doc.docid = vce.docid
		        INNER JOIN
		            workflow.estadodocumento esd ON esd.esdid = doc.esdid
	            INNER JOIN
	                seguranca.usuario usu ON usu.usucpf = vce.usucpf
                LEFT JOIN
                    conferencia.dadosiniciaisvinculoceu div ON div.ceuid = vce.ceuid
				INNER JOIN
				    conferencia.endereco edcdados ON edcdados.edcid = div.edcid
			    INNER JOIN
			        territorios.municipio mundados ON mundados.muncod = edcdados.muncod
				WHERE
					vce.vcestatus = 'A' AND
					ceu.ceuid = " . (int) $ceuid;

		$retorno = $this->carregar($sql);
		return $retorno ? $retorno : array();
    }

	public function verificaSeVinculoAprovado($vceid) {
		$sql = " SELECT
					(CASE
					WHEN esd.esdid = " . WF_CEU_APROVADO . "
						THEN
							1
						ELSE
							0
						END) as flg_aprovado
				FROM
					{$this->stNomeTabela} vce
			    INNER JOIN
			        workflow.documento doc ON doc.docid = vce.docid
		        INNER JOIN
		            workflow.estadodocumento esd ON esd.esdid = doc.esdid
				WHERE vceid = {$vceid}";

		return $this->pegaUm ( $sql );
	}

    public function getPermissaoTelaVinculoCeu(){
        $habilitado = 'S';
        if($this->docid){
            $arDados              = wf_pegarEstadoAtual($this->docid);
            $arEstadosHabilitados = array(WF_CEU_EM_CADASTRAMENTO, WF_CEU_EM_CORRECAO);
            $habilitado           = in_array($arDados["esdid"], $arEstadosHabilitados) ? "S" : "N";
        }

        return $habilitado;
    }

    public function desenharCabecalho(){
        $mCeu     = new Ceu($this->ceuid);
        $dadosCeu = $mCeu->recuperarDados();

        $out = "<table align='center' class='tabela' bgcolor='#f5f5f5' cellspacing='1' cellpadding='3'>
                    <tr>
                        <td style='text-align: center; background-color: #DCDCDC; font-weight: bold;'>
                           UF
                        </td>
                        <td style='text-align: center; background-color: #DCDCDC; font-weight: bold;'>
                            Região
                        </td>
                        <td style='text-align: center; background-color: #DCDCDC; font-weight: bold;'>
                            Município
                        </td>
                        <td style='text-align: center; background-color: #DCDCDC; font-weight: bold;'>
                            CEU
                        </td>
                    </tr>
                    <tr>
                        <td style='text-align: center;'>
                            {$dadosCeu["ceuestuf"]}
                        </td>
                        <td style='text-align: center;'>
                            {$dadosCeu["ceuregdescricao"]}
                        </td>
                        <td style='text-align: center;'>
                            {$dadosCeu["ceumundescricao"]}
                        </td>
                        <td style='text-align: center;'>
                            {$dadosCeu["ceucodigo"]} - {$dadosCeu["ceunome"]}
                        </td>
                    </tr>
                </table>";

        return $out;
    }

    public function ativarVinculo($vceid, $inativarDemaisVinculos = true){
        $this->carregarPorId($vceid);

        if($inativarDemaisVinculos){
            $sql = "UPDATE
                        {$this->stNomeTabela}
                    SET
                        vceativo = false
                    WHERE
                        ceuid = {$this->ceuid}
                        AND
                        vceativo IS TRUE
                        AND
                        vceid != $vceid";

            $this->executar($sql);
        }

        $this->vceativo = 'true';
        $this->salvar();
    }

    public function recuperarVinculosAtivos($vceid, $desconsiderarVinculoParametro = true){
        $this->carregarPorId($vceid);

        $arWhere = array("vce.vcestatus = 'A'",
                         "ceu.ceuid = {$this->ceuid}",
                         "vce.vceativo IS TRUE");

        if($desconsiderarVinculoParametro){
            $arWhere[] = "vce.vceid != {$vceid}";
        }

        $arDados = $this->listar( $arWhere , false);

        return $arDados ? $arDados : array();
    }

    public function desenharBarraVinculo(){
        $vceid = $this->getVinculoSession();
        $mCeu  = new Ceu($this->ceuid);

        $img = "";
        $msg = "";
        $msgEscape = "";
        $arVinculosAtivos = $this->recuperarVinculosAtivos($vceid);
        if(is_array($arVinculosAtivos) && count($arVinculosAtivos) > 0){
            $img = "atencao2.png";
            $msg = "Existem outros vínculos ativos utilizando o CEU {$mCeu->ceucodigo} - {$mCeu->ceunome} para os seguintes usuários:";
            $msgEscape = "{$msg}<br />";
            $msg .= '\n';

            $arVinculos = array();
            foreach($arVinculosAtivos as $vinculo){
                $arVinculos[] = "{$vinculo['vceid']} - {$vinculo['usunome']} em {$vinculo['vcedtcadastro']}";
            }

            $msgEscape .= implode("<br />", $arVinculos);
            $msg .= implode('\n', $arVinculos);
        }else{
            $img = "atencao1.gif";
            $msg = "Não existe nenhum outro vínculo ativo utilizando o CEU {$mCeu->ceucodigo} - {$mCeu->ceunome}.";
            $msgEscape = $msg;
        }

        // $msgEscape = '<div style=\'width: 350px;\'>'.$msgEscape.'</div>';

        $html = '
            <table border="0" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
                <tr style="background-color: #c9c9c9; text-align:center;">
                    <td style="font-size:7pt; text-align:center; font-weight: bold;">
                        outros vínculos
                    </td>
                </tr>
        		<tr>
    				<td style="font-size: 7pt; text-align: center; border-top-width: 2px; border-top-style: solid; border-top-color: rgb(208, 208, 208);" onmouseover="return escape(\''.$msgEscape.'\');" onclick="alert(\''.$msg.'\');">
					    <img style="cursor: pointer;" src="/imagens/'.$img.'">
    				</td>
    			</tr>
    	    </table>';
        echo $html;
    }

    public function listarMembrosUGLCeus( $arWhere = array(), $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "ceu.ceucodigo,
	                    ceu.ceunome,
            			mun.estuf,
			    		mun.mundescricao,
                        d.resposta[1] AS tipo,
                        d.resposta[2] AS nome,
                        d.resposta[3] AS ddd,
                        d.resposta[4] AS telefone,
                        d.resposta[5] AS email,
                        d.grptitulo,
                        d.grpid,
                        d.ceuid";
        }

        $arWhere[] = "ceu.ceustatus = 'A'";
        $sql = "WITH dados AS (
                    	SELECT
                    		ceu.ceuid,
                    		ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
                    		COALESCE(grpp.grpid, grp.grpid) AS grpid,
                    		COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo,
                    		per.perid,
                    		per.pertitulo,
                    		COALESCE(res.resdsc, itp.itptitulo) AS resposta
                    	FROM
                    		conferencia.ceu ceu
                    	INNER JOIN
                    		conferencia.vinculoceuquestionario vcq ON ceu.ceuid = vcq.ceuid
                    						                      AND vcq.vcqstatus = 'A'
                    	INNER JOIN
                    		questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                    						                     AND qrp.queid = ".TIPO_QUESTIONARIO_UGL_GG."
                    	INNER JOIN
                    		questionario.grupopergunta grp ON (qrp.qrpid = grp.qrpid OR grp.qrpid IS NULL )
            					                          AND grp.queid_atual = qrp.queid
            					                          AND grp.grpstatus   = 'A'
    						                              AND (grp.grpid      = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_UGL."
					                                        OR grp.grpid_pai  = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_UGL."
			                                                OR grp.grpid IN (SELECT
                                            									 grpfilho.grpid
                                        									 FROM
                                            									 questionario.grupopergunta grpfilho
                                                							 INNER JOIN
                                            									 questionario.grupopergunta grppai ON grpfilho.gru_grpid = grppai.grpid
                                        													                      AND grppai.grpstatus   = 'A'
                                                							 WHERE
					                                                            grppai.grpid    = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_UGL."
                                                                             OR grppai.grpid_pai = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_UGL."))
                        LEFT JOIN
                            questionario.grupopergunta grpp ON grp.gru_grpid  = grpp.grpid
                                                           AND grpp.grpstatus = 'A'
                    	INNER JOIN
                    		questionario.pergunta per ON qrp.queid = per.queid_atual
                    					             AND grp.grpid = per.grpid
                    					             AND per.perstatus = 'A'
                    	LEFT JOIN
                    		questionario.resposta res ON res.perid = per.perid
                    					             AND res.qrpid = qrp.qrpid
                    	LEFT JOIN
                    		questionario.itempergunta itp ON itp.itpid = res.itpid
                    	WHERE
                    		ceu.ceustatus = 'A'
                    	ORDER BY
                    		ceu.ceuid, grp.grpid, per.perid
                    )
    			        SELECT
    			            {$colunas}
    			        FROM
    			            (SELECT
    			            	ceuid,
    			            	ceunome,
    			            	grpid,
    			            	grptitulo,
    			            	array_agg(resposta) AS resposta
    			             FROM
    			        	    dados
    			             GROUP BY
    			        	    ceuid, ceunome, grpid, grptitulo) d
    	                INNER JOIN
    	                     conferencia.ceu ceu ON ceu.ceuid = d.ceuid
    	                INNER JOIN
    	                     conferencia.endereco edc ON edc.edcid = ceu.edcid
    	                INNER JOIN
    	                     territorios.municipio mun ON mun.muncod = edc.muncod
                         WHERE
                              ". implode(" AND ", $arWhere);

		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY d.ceuid, d.grpid";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 30);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }

    public function listarMembrosGGCeus( $arWhere = array(), $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "ceu.ceucodigo,
	                    ceu.ceunome,
            			mun.estuf,
			    		mun.mundescricao,
                        d.resposta[1] AS tipo,
                        d.resposta[2] AS titularidade,
                        d.resposta[3] AS nome,
                        d.resposta[4] AS ddd,
                        d.resposta[5] AS telefone,
                        d.resposta[6] AS email,
                        d.grptitulo,
                        d.grpid,
                        d.ceuid";
        }

        $arWhere[] = "ceu.ceustatus = 'A'";
        $sql = "WITH dados AS (
                    	SELECT
                    		ceu.ceuid,
                    		ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
                    		COALESCE(grpp.grpid, grp.grpid) AS grpid,
                    		COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo,
                    		per.perid,
                    		per.pertitulo,
                    		COALESCE(res.resdsc, itp.itptitulo) AS resposta
                    	FROM
                    		conferencia.ceu ceu
                    	INNER JOIN
                    		conferencia.vinculoceuquestionario vcq ON ceu.ceuid = vcq.ceuid
                    						                      AND vcq.vcqstatus = 'A'
                    	INNER JOIN
                    		questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                    						                     AND qrp.queid = ".TIPO_QUESTIONARIO_UGL_GG."
                    	INNER JOIN
                    		questionario.grupopergunta grp ON (qrp.qrpid = grp.qrpid OR grp.qrpid IS NULL )
            					                          AND grp.queid_atual = qrp.queid
            					                          AND grp.grpstatus   = 'A'
    						                              AND (grp.grpid      = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG."
					                                        OR grp.grpid_pai  = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG."
			                                                OR grp.grpid IN (SELECT
                                            									 grpfilho.grpid
                                        									 FROM
                                            									 questionario.grupopergunta grpfilho
                                                							 INNER JOIN
                                            									 questionario.grupopergunta grppai ON grpfilho.gru_grpid = grppai.grpid
                                        													                      AND grppai.grpstatus   = 'A'
                                                							 WHERE
					                                                            grppai.grpid    = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG."
                                                                             OR grppai.grpid_pai = ".GRUPO_PERGUNTA_UGL_GG_MEMBROS_DO_GG."))
                        LEFT JOIN
                            questionario.grupopergunta grpp ON grp.gru_grpid  = grpp.grpid
                                                           AND grpp.grpstatus = 'A'
                    	INNER JOIN
                    		questionario.pergunta per ON qrp.queid = per.queid_atual
                    					             AND grp.grpid = per.grpid
                    					             AND per.perstatus = 'A'
                    	LEFT JOIN
                    		questionario.resposta res ON res.perid = per.perid
                    					             AND res.qrpid = qrp.qrpid
                    	LEFT JOIN
                    		questionario.itempergunta itp ON itp.itpid = res.itpid
                    	WHERE
                    		ceu.ceustatus = 'A'
                    	ORDER BY
                    		ceu.ceuid, grp.grpid, per.perordem, per.perid
                    )
    			        SELECT
    			            {$colunas}
    			        FROM
    			            (SELECT
    			            	ceuid,
    			            	ceunome,
    			            	grpid,
    			            	grptitulo,
    			            	array_agg(resposta) AS resposta
    			             FROM
    			        	    dados
    			             GROUP BY
    			        	    ceuid, ceunome, grpid, grptitulo) d
    	                INNER JOIN
    	                     conferencia.ceu ceu ON ceu.ceuid = d.ceuid
    	                INNER JOIN
    	                     conferencia.endereco edc ON edc.edcid = ceu.edcid
    	                INNER JOIN
    	                     territorios.municipio mun ON mun.muncod = edc.muncod
                         WHERE
                              ". implode(" AND ", $arWhere);

		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY d.ceuid, d.grpid";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 30);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }

    public function listarMapeamentoSocioculturalCeus( $arWhere = array(), $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "ceu.ceucodigo,
	                    ceu.ceunome,
            			mun.estuf,
			    		mun.mundescricao,
                        d.resposta[1] AS tipoautor,
                        d.resposta[2] AS nomeautor,
                        d.resposta[3] AS endereco,
                        d.resposta[4] AS tema,
                        d.resposta[5] AS nomecontato,
                        d.resposta[6] AS cargo,
                        d.resposta[7] AS tel1,
                        d.resposta[8] AS tel2,
                        d.resposta[9] AS email,
                        d.grptitulo,
                        d.grpid,
                        d.ceuid";
        }

        $arWhere[] = "ceu.ceustatus = 'A'";
        $sql = "WITH dados AS (
                    	SELECT
                    		ceu.ceuid,
                    		ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
                    		COALESCE(grpp.grpid, grp.grpid) AS grpid,
                    		COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo,
                    		per.perid,
                    		per.pertitulo,
                    		CASE WHEN qen.qenid IS NULL THEN
            					COALESCE(res.resdsc, itp.itptitulo)
        				    ELSE
                                qen.qenlogradouro || ' ' || qen.qencomplemento|| ' Nº ' || qen.qennumero || ' - ' || qen.qenbairro || ' - ' || munr.mundescricao || '/' || munr.estuf
        			        END AS resposta
                    	FROM
                    		conferencia.ceu ceu
                    	INNER JOIN
                    		conferencia.vinculoceuquestionario vcq ON ceu.ceuid = vcq.ceuid
                    						                      AND vcq.vcqstatus = 'A'
                    	INNER JOIN
                    		questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                    						                     AND qrp.queid = ".TIPO_QUESTIONARIO_MOBILIZACAO_SOCIAL."
                    	INNER JOIN
                    		questionario.grupopergunta grp ON (qrp.qrpid = grp.qrpid OR grp.qrpid IS NULL )
            					                          AND grp.queid_atual = qrp.queid
            					                          AND grp.grpstatus   = 'A'
    						                              AND (grp.grpid      = ".GRUPO_PERGUNTA_MOBILIZACAO_SOCIAL_MAPEAMENTO_SOCIOCULTURAL."
					                                        OR grp.grpid_pai  = ".GRUPO_PERGUNTA_MOBILIZACAO_SOCIAL_MAPEAMENTO_SOCIOCULTURAL."
			                                                OR grp.grpid IN (SELECT
                                            									 grpfilho.grpid
                                        									 FROM
                                            									 questionario.grupopergunta grpfilho
                                                							 INNER JOIN
                                            									 questionario.grupopergunta grppai ON grpfilho.gru_grpid = grppai.grpid
                                        													                      AND grppai.grpstatus   = 'A'
                                                							 WHERE
					                                                            grppai.grpid    = ".GRUPO_PERGUNTA_MOBILIZACAO_SOCIAL_MAPEAMENTO_SOCIOCULTURAL."
                                                                             OR grppai.grpid_pai = ".GRUPO_PERGUNTA_MOBILIZACAO_SOCIAL_MAPEAMENTO_SOCIOCULTURAL."))
                        LEFT JOIN
                            questionario.grupopergunta grpp ON grp.gru_grpid  = grpp.grpid
                                                           AND grpp.grpstatus = 'A'
                    	INNER JOIN
                    		questionario.pergunta per ON qrp.queid = per.queid_atual
                    					             AND grp.grpid = per.grpid
                    					             AND per.perstatus = 'A'
                    	LEFT JOIN
                    		questionario.resposta res ON res.perid = per.perid
                    					             AND res.qrpid = qrp.qrpid
			            LEFT JOIN
				            questionario.endereco qen ON qen.qenid = res.qenid
			            LEFT JOIN
			                territorios.municipio munr ON munr.muncod = qen.muncod
                    	LEFT JOIN
                    		questionario.itempergunta itp ON itp.itpid = res.itpid
                    	WHERE
                    		ceu.ceustatus = 'A'
                    	ORDER BY
                    		ceu.ceuid, grp.grpid, per.perordem, per.perid
                    )
    			        SELECT
    			            {$colunas}
    			        FROM
    			            (SELECT
    			            	ceuid,
    			            	ceunome,
    			            	grpid,
    			            	grptitulo,
    			            	array_agg(resposta) AS resposta
    			             FROM
    			        	    dados
    			             GROUP BY
    			        	    ceuid, ceunome, grpid, grptitulo) d
    	                INNER JOIN
    	                     conferencia.ceu ceu ON ceu.ceuid = d.ceuid
    	                INNER JOIN
    	                     conferencia.endereco edc ON edc.edcid = ceu.edcid
    	                INNER JOIN
    	                     territorios.municipio mun ON mun.muncod = edc.muncod
                         WHERE
                              ". implode(" AND ", $arWhere);

		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY d.ceuid, d.grpid";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 30);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }

    public function listarParceriasInstitucionaisCeus( $arWhere = array(), $paginacao = false, $colunas = ""){
        if($colunas == ""){
            $colunas = "ceu.ceucodigo,
	                    ceu.ceunome,
            			mun.estuf,
			    		mun.mundescricao,
                        d.resposta[1] 	AS instituicao,
                        d.resposta[2] 	AS tema_atuacao,
                        d.resposta[3] 	AS tema_atuacao_sec,
                        d.resposta[4] 	AS tema_atuacao_ter,
                        d.resposta[5] 	AS acoes_conjuntas,
                        d.resposta[6] 	AS duracao_parceria,
                        d.resposta[7] 	AS envolve_recursos,
                        d.resposta[8] 	AS nome,
                        d.resposta[9] 	AS cargo,
                        d.resposta[10] 	AS tipo_contato,
                        d.resposta[11] 	AS ddd,
                        d.resposta[12] 	AS telefone,
                        d.resposta[13] 	AS email,
                        d.grptitulo,
                        d.grpid,
                        d.ceuid";
        }

        $arWhere[] = "ceu.ceustatus = 'A'";
        $sql = "WITH dados AS (
                    	SELECT
                    		ceu.ceuid,
                    		ceu.ceucodigo || ' - ' || ceu.ceunome AS ceunome,
                    		COALESCE(grpp.grpid, grp.grpid) AS grpid,
                    		COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo,
                    		per.perid,
                    		per.pertitulo,
                    		COALESCE(res.resdsc, itp.itptitulo) AS resposta
                    	FROM
                    		conferencia.ceu ceu
                    	INNER JOIN
                    		conferencia.vinculoceuquestionario vcq ON ceu.ceuid = vcq.ceuid
                    						                      AND vcq.vcqstatus = 'A'
                    	INNER JOIN
                    		questionario.questionarioresposta qrp ON qrp.qrpid = vcq.qrpid
                    						                     AND qrp.queid = ".TIPO_QUESTIONARIO_ARTICULACAO_SNC."
                    	INNER JOIN
                    		questionario.grupopergunta grp ON (qrp.qrpid = grp.qrpid OR grp.qrpid IS NULL )
            					                          AND grp.queid_atual = qrp.queid
            					                          AND grp.grpstatus   = 'A'
    						                              AND (grp.grpid      = ".GRUPO_PERGUNTA_ARTICULACAO_SNC_PARCERIAS_INSTITUCIONAIS."
					                                        OR grp.grpid_pai  = ".GRUPO_PERGUNTA_ARTICULACAO_SNC_PARCERIAS_INSTITUCIONAIS."
			                                                OR grp.grpid IN (SELECT
                                            									 grpfilho.grpid
                                        									 FROM
                                            									 questionario.grupopergunta grpfilho
                                                							 INNER JOIN
                                            									 questionario.grupopergunta grppai ON grpfilho.gru_grpid = grppai.grpid
                                        													                      AND grppai.grpstatus   = 'A'
                                                							 WHERE
					                                                            grppai.grpid    = ".GRUPO_PERGUNTA_ARTICULACAO_SNC_PARCERIAS_INSTITUCIONAIS."
                                                                             OR grppai.grpid_pai = ".GRUPO_PERGUNTA_ARTICULACAO_SNC_PARCERIAS_INSTITUCIONAIS."))
                        LEFT JOIN
                            questionario.grupopergunta grpp ON grp.gru_grpid  = grpp.grpid
                                                           AND grpp.grpstatus = 'A'
                    	INNER JOIN
                    		questionario.pergunta per ON qrp.queid = per.queid_atual
                    					             AND grp.grpid = per.grpid
                    					             AND per.perstatus = 'A'
                    	LEFT JOIN
                    		questionario.resposta res ON res.perid = per.perid
                    					             AND res.qrpid = qrp.qrpid
                    	LEFT JOIN
                    		questionario.itempergunta itp ON itp.itpid = res.itpid
                    	WHERE
                    		ceu.ceustatus = 'A'
                    	ORDER BY
                    		ceu.ceuid, grp.grpid, per.perordem, per.perid
                    )
    			        SELECT
    			            {$colunas}
    			        FROM
    			            (SELECT
    			            	ceuid,
    			            	ceunome,
    			            	grpid,
    			            	grptitulo,
    			            	array_agg(resposta) AS resposta
    			             FROM
    			        	    dados
    			             GROUP BY
    			        	    ceuid, ceunome, grpid, grptitulo) d
    	                INNER JOIN
    	                     conferencia.ceu ceu ON ceu.ceuid = d.ceuid
    	                INNER JOIN
    	                     conferencia.endereco edc ON edc.edcid = ceu.edcid
    	                INNER JOIN
    	                     territorios.municipio mun ON mun.muncod = edc.muncod
                         WHERE
                              ". implode(" AND ", $arWhere);

		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY d.ceuid, d.grpid";
		}

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql, 30);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }

}