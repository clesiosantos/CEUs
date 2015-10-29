<?php

final class ExtractQuestionToExcel{
	private $questionario;
	private $acordo;
	private $unidadeProjeto;
	private $pergunta;
	private $item;
	private $resposta;
//	private $db;
//	private $arrPergunta;

	public function __construct(Array $param)
	{
		$this->resposta = new QResposta();
		$this->pergunta = array();
//		$this->db 		= new cls_banco();
		$this->buscarQuestionarioPorAcordo($param['acoid']);
		$this->buscarUnidadeProjetoPorQuestionario($this->questionario->queid);

		if(isset($param['grpid']) && $param['grpid']){
    		$this->buscarPergunta($param['grpid'], "grupo");
		}else{
    		$this->buscarPergunta($this->questionario->queid, "questionario");
		}

//		$this->buscarPerguntaPorQuestionario($this->questionario->queid);
	}

//	public function donwload()
//	{
//		$this->db->sql_to_excel(self::preparaArray(), str_replace(" ", "_", $this->acordo->aconome));
//	}

	public function PHPExcel($tabelas)
	{
		$dados = self::preparaArray();
		$contL = 1;
		foreach ($dados as $linha){
			$contC = 0;
			foreach ($linha as $coluna){
				// Adiciona Cell na linha
			    $tabelas->setCellValueByColumnAndRow($contC,$contL,utf8_encode($coluna));
				// Dimensão
				$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
				$contC++;
			}
			$contL++;
		}
	}

	private function preparaArray()
	{
		$arDado = array();
		array_push($arDado, array(("Acordo"), ("Questionário")));
		array_push($arDado, array($this->acordo->aconome, $this->questionario->quetitulo));
//		array_push($arDado, array("Acordo" => $this->acordo->aconome, "Questionário" => $this->questionario->quetitulo));
		// Monta cabeçalho
		$linha = array();
		array_push($linha, "Unidade/Projeto");

		$this->ordenarPerguntas();

		foreach ($this->pergunta as $perg){
			array_push($linha, $perg['pertitulo']);
		}
		array_push($arDado, $linha);
		// Monta linhas com os dados por escola
		foreach ($this->unidadeProjeto as $unidadeProjeto){
			$linha = array();
			array_push($linha, $unidadeProjeto['descricao']);

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
				foreach ($this->pergunta as $pergunta){
					array_push($linha, $this->buscarResposta($qrpid, $pergunta));
				}
			}
			array_push($arDado, $linha);
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
                        per.perid, per.pertitulo, per.pertipo
                    FROM
                        questionario.pergunta per
                    LEFT JOIN
                        questionario.questionarioordem qor ON qor.perid = per.perid
                    LEFT JOIN
                        questionario.grupopergunta grp ON grp.grpid = per.grpid
                    WHERE
                        per.perid IN (" . implode(", ", $arPerids) . ")
                        AND
                        (grp.grpstatus != 'I' OR grp.grpstatus IS NULL)
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

		// Exclui as perguntas múltiplas
		$where[] = "perid NOT IN (SELECT
                                      per.perid
                                  FROM
                                  	  questionario.grupopergunta grp
                                  INNER JOIN
                                  	  questionario.pergunta per ON per.grpid = grp.grpid AND per.perstatus = 'A'
                                  WHERE
                                  	  grp.grpstatus = 'A'
                                  	  AND
                                  	  grp.grppermitemultiplo IS TRUE)";

		self::addCadaPergunta($pergunta->lista($where, 'perid, pertitulo, pertipo', 'perordem'));
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




