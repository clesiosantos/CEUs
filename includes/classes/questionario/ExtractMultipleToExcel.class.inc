<?php

final class ExtractMultipleToExcel{
	private $questionario;
	private $acordo;
	private $unidadeProjeto;
	private $pergunta;
	private $item;
	private $resposta;

	public function __construct(Array $param)
	{
		$this->resposta = new QResposta();
		$this->pergunta = array();
		$this->buscarQuestionarioPorAcordo($param['acoid']);
		$this->buscarUnidadeProjetoPorQuestionario($this->questionario->queid);

		$this->buscarPergunta($this->questionario->queid, "questionario");
	}

	public function PHPExcel($tabelas)
	{
		$arDados = self::preparaArray();
		foreach($arDados['titulo'] as $i => $arTitulo){
		    foreach($arTitulo as $j => $titulo){
        	    $tabelas->setCellValueByColumnAndRow($j,$i+1,utf8_encode($titulo));
        		$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
		    }
		}

		$contL = $i+3;
		if(is_array($arDados['grupos'])){
    		foreach($arDados['grupos'] as $grpid => $grupo){
        		$contC = 0;
    		    // Cabeçalho do grupo
    			$tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode(("Projeto")));
        		$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
        		$contC++;

    			$tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($grupo['nome']));
        		$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
        		$contC++;

        		if(is_array($grupo['pergunta'])){
            		foreach($grupo['pergunta'] as $perid => $pergunta){
            		    for($i = 0; $i < QTD_COLUNAS_QSTOES_MULTI_EXPORT_EXCEL; $i++){
            		        // Pergunta
            		        $tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($pergunta . " (".($i+1).")"));
            		        $tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
            		        $contC++;
            		    }
            		}
        		}
        		$contL++;
    		    // Fim Cabeçalho do grupo

        		if(is_array($arDados['unidade'])){
            		foreach ($arDados['unidade'] as $arUnidade){
                		$contC = 0;

            			$tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($arUnidade['descricao']));
                		$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
                		$contC++;

                		$tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($grupo['nome']));
                		$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
                		$contC++;

                		if(is_array($grupo['pergunta'])){
                		    foreach($grupo['pergunta'] as $perid => $pergunta){
            				    for($i = 0; $i < QTD_COLUNAS_QSTOES_MULTI_EXPORT_EXCEL; $i++){
            				        $resposta = isset($arUnidade["grupo"][$grpid]['resposta'][$pergunta][$i]) ? $arUnidade["grupo"][$grpid]['resposta'][$pergunta][$i] : '';

                        			// Respostas
                				    $tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($resposta));
                    				$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
                    				$contC++;
            				    }
            				}
                		}

                		$contL++; // Pula linha para a próxima unidade
            		}
        		}

        		$contL++; // Pula linha para o próximo grupo
    		}
		}
	}

	private function preparaArray()
	{
		$arDado = array();
		$arDado['titulo'][] = array(("Acordo"), ("Questionário"));
		$arDado['titulo'][] = array($this->acordo->aconome, $this->questionario->quetitulo);

		$this->ordenarPerguntas();

		// Monta cabeçalho
		// Monta linhas com os dados por escola
		foreach ($this->unidadeProjeto as $k => $unidadeProjeto){
			$arDado['unidade'][$k]['descricao'] = $unidadeProjeto['descricao'];
			// Buscar Grupo de resposta (resposta do questionário pela escola)
			$questionarioResposta = new QQuestionarioResposta();

			if($unidadeProjeto['tipo'] == 'P'){
    			$qrpid = $questionarioResposta->carregaUm(array("acoid" => $this->acordo->acoid,
    															"queid" => $this->questionario->queid,
    															"proid" => $unidadeProjeto['codigo']));
			}else{
    			$qrpid = $questionarioResposta->carregaUm(array("acoid" => $this->acordo->acoid,
    															"queid" => $this->questionario->queid,
    															"escid" => $unidadeProjeto['codigo']));
			}
			if ($qrpid){
				// Buscar resposta
				foreach ($this->pergunta as $kP => $pergunta){
				    if($pergunta['qrpid'] == $qrpid || is_null($pergunta['qrpid'])){
    				    if(is_null($pergunta['qrpid'])){
        				    $arDado['unidade'][$k]['grupo'][$pergunta['grpid']]['nome'] = $pergunta['grptitulo'];
        				    $arDado['unidade'][$k]['grupo'][$pergunta['grpid']]['pergunta'][$pergunta['perid']] = $pergunta['pertitulo'];
    				    }
    				    $arDado['unidade'][$k]['grupo'][$pergunta['grpid']]['resposta'][$pergunta['pertitulo']][] = $this->buscarResposta($qrpid, $pergunta);
				    }
				}
			}
		}

		if(is_array($arDado['unidade'])){
            foreach($arDado['unidade'] as $unidade){
                if(is_array($unidade['grupo'])){
                    foreach($unidade['grupo'] as $grpid => $grupo){
                        $arDado['grupos'][$grpid] = $grupo;
                    }
                }
            }
		}

		return $arDado;
	}

	private function ordenarPerguntas(){
	    if(is_array($this->pergunta) && count($this->pergunta) > 0){
            $arPerids = array();
	        foreach($this->pergunta as $pergunta){
                $arPerids[] = $pergunta['perid'];
            }
            $sql = "SELECT
                        per.perid, per.pertitulo, per.pertipo, COALESCE(grp.grpid_pai, grp.grpid) AS grpid, COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo, grp.qrpid
                    FROM
                        questionario.pergunta per
                    LEFT JOIN
                        questionario.questionarioordem qor ON qor.perid = per.perid
                    LEFT JOIN
                        questionario.grupopergunta grp ON grp.grpid = per.grpid
                    LEFT JOIN
                        questionario.grupopergunta grpp ON grpp.grpid = grp.grpid_pai
                    WHERE
                        per.perid IN (" . implode(", ", $arPerids) . ")
                        AND
                        grp.grpstatus != 'I'
                    ORDER BY
                        qor.qorordem ASC;";
            $this->pergunta = $this->resposta->carregar($sql);
	    }
	}

	private function buscarQuestionarioPorAcordo($acoid)
	{
		$this->acordo 		= new QAcordo($acoid);
		$questionario 		= new QQuestionario();
		$this->questionario = $questionario->pegaUmObjetoPorAcordo($acoid);
	}

	private function buscarUnidadeProjetoPorQuestionario($queid)
	{
	    // Busca as unidades
		$escola   = new QEscola();
		$rsEscola = $escola->buscarEscolasPorQuestionario($queid, "escid AS codigo, escnome AS descricao, 'E' AS tipo");
		$rsEscola = is_array($rsEscola) ? $rsEscola : array();

	    // Busca os projetos
		$projeto   = new QProjeto();
		$rsProjeto = $projeto->buscarProjetosPorQuestionario($queid, "proid AS codigo, pronome AS descricao, 'P' AS tipo");
		$rsProjeto = is_array($rsProjeto) ? $rsProjeto : array();


		$this->unidadeProjeto = array_merge($rsEscola, $rsProjeto);
	}

	private function buscarPergunta($param, $type)
	{
		$pergunta = new QPergunta();
		switch($type){
			case "questionario":
				$where = array("queid"=>$param, "notpertipo"=>'EXT');
				break;
			case "grupo":
				$where = array("grpid"=>$param, "notpertipo"=>'EXT');
				break;
			case "item":
				$where = array("itpid"=>$param, "notpertipo"=>'EXT');
				break;
		}

		// Perguntas múltiplas originiais (não cópias)
		$where[] = "perid IN (SELECT
                                  per.perid
                              FROM
                              	  questionario.grupopergunta grp
                              INNER JOIN
                              	  questionario.pergunta per ON per.grpid = grp.grpid AND per.perstatus = 'A'
                              WHERE
                              	  grp.grpstatus = 'A'
                              	  AND
                              	  grp.grppermitemultiplo IS TRUE)";

		self::addCadaPergunta($pergunta->lista($where, 'perid, pertitulo, pertipo, grpid', 'perordem'));
		self::buscarGrupo($param, $type);
	}

	private function buscarGrupo($param, $type)
	{
		$grupo = new QGrupo();
		switch($type){
			case "questionario":
				$where = array("queid" => $param);
				break;
			case "grupo":
				$where = array("grpid" => $param);
				break;
			case "item":
				$where = array("itpid" => $param);
				break;
		}
		$grupo = $grupo->lista($where, "grpid", "grpordem");
		if (is_array($grupo)){
			foreach ($grupo as $grupo){
				self::buscarGrupo($grupo['grpid'], "grupo");
				self::buscarPergunta($grupo['grpid'], "grupo");
			}
		}
	}

	private function buscarItem($perid/*$param, $type*/)
	{
		$item = new QItemPergunta();
//		switch($type){
//			case "pergunta":
				$where = array("perid" => $perid);
//				break;
//		}
//		$item = $item->lista($where, "itpid", "itptitulo");
		$item = $item->carregarColuna($where);
		if (!$this->item[$perid]){
			$this->item[$perid] = $item;
		}
		if (is_array($item)){
			foreach ($item as $item){
				self::buscarPergunta($item, "item");
				self::buscarGrupo($item, "item");
			}
		}
	}

	private function addCadaPergunta($pergunta)
	{
		if (is_array($pergunta)){
			foreach ($pergunta as $pergunta){
				array_push($this->pergunta, $pergunta);
				if (in_array($pergunta['pertipo'], array('CK', 'RA', 'CM', 'CB', 'BM'))){
					self::buscarItem($pergunta['perid'], 'pergunta');
				}
			}
		}
	}

	private function buscarResposta($qrpid, $pergunta)
	{
		$resposta = $this->resposta;
		// Carrega itens das perguntas
//		if (!$this->item[$pergunta['perid']] && in_array($pergunta['pertipo'], array('CK', 'RA', 'CM', 'CB', 'BM'))){
//			$item = new QItemPergunta();
//			$this->item[$pergunta['perid']] = $item->carregarColuna(array("perid" => $pergunta['perid']));
//		}
		// Busca resposta das perguntas do tipo que possui ITEM
		if (in_array($pergunta['pertipo'], array('CK', 'RA', 'CM', 'CB', 'BM'))){
			$resp = $resposta->carregarResposta(array("res.perid" => $pergunta['perid'], "res.qrpid" => $qrpid), 'itp.itptitulo');

			$resp = $resp ? $resp : array();
			$val = implode(', ', $resp);

//			foreach ($resp as $resp){
//				$pos = array_search($resp['perid'], $this->item[$pergunta['perid']]);
//				$pos = is_int($pos) ? ($pos + 1) : '';
//				$val .= empty($val) ? $pos : ','.$pos;
//			}
		}else{
			$val = $resposta->pegaResposta($pergunta['perid'], $qrpid);
		}

		return $val;
	}
}




