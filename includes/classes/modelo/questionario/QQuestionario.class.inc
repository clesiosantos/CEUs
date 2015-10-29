<?php

class QQuestionario extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "queid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'queid' => null,
									  	'queid_matriz' => null,
									  	'sisid' => null,
									  	'quetitulo' => null,
									  	'quedescricao' => null,
									  	'quedtinicio' => null,
									  	'quedtfim' => null,
									  	'questatus' => null,
									  );

	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $item){
			if (get_class((object) $item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
				continue;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid_matriz = " . $item->queid;
				continue;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return $this->carregar($sql, get_class((object) $this));
	}

	public function listaAcordo(Array $param = array()){
		extract($_POST);
		$where = array();

		if ($esdid)
				$where[] = "ed.esdid = {$esdid}";

		if ($quetitulo)
				$where[] = "q.quetitulo ILIKE '%{$quetitulo}%'";

		$sql = array();
		// Monta SELECT filtrando pelos critérios do perfil GESTOR "acordo"
		if (possui_perfil(PERFIL_GESTOR)){
			$sql[] = "SELECT
							aconome,
							quetitulo,
							to_char(quedtinicio,'DD-MM-YYYY') AS quedtinicio,
							to_char(quedtfim,'DD-MM-YYYY') AS quedtfim,
							esddsc,
							CASE
								WHEN ed.esdid = " . WF_VALIDACAO_FINALIZADA . " THEN '<img src=\'/imagens/principal.gif\' style=\'cursor:pointer\' title=\"". ("Responder Questionário")."\" onclick=\'location.href=\"?modulo=principal/questionario&acao=A&queid='||queid||'\";\'>'
								ELSE '<img src=\'/imagens/principal_disabled.gif\' title=\"". ("Não é possível responder este questionário")."\" ;\'>'
							END AS esdid,
							queid
						FROM
							questionario.questionario q
						JOIN questionario.acordo a ON a.acoid = q.acoid AND
													  a.acostatus = 'A'
						JOIN survey.usuarioresponsabilidade r ON r.acoid = a.acoid
														   AND r.usucpf = '{$_SESSION['usucpf']}'
														   AND r.rpustatus = 'A'
						JOIN workflow.documento d ON d.docid = q.docid
						JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
						WHERE
							q.questatus = 'A'
						" . (count($where) > 0 ? " AND " . implode(" AND ", $where) : "");
		}

		// Monta SELECT filtrando pelos critérios do perfil ESCOLA
		if (possui_perfil(PERFIL_ESCOLA)){
			$sql[] = "	SELECT
							DISTINCT
							aconome,
							quetitulo,
							to_char(quedtinicio,'DD-MM-YYYY') AS quedtinicio,
							to_char(quedtfim,'DD-MM-YYYY') AS quedtfim,
							esddsc,
							CASE
								WHEN ed.esdid = " . WF_VALIDACAO_FINALIZADA . " THEN '<img src=\'/imagens/principal.gif\' style=\'cursor:pointer\' title=\"". ("Responder Questionário")."\" onclick=\'location.href=\"?modulo=principal/questionario&acao=A&queid='||queid||'\";\'>'
								ELSE '<img src=\'/imagens/principal_disabled.gif\' title=\"". ("Não é possível responder este questionário")."\" ;\'>'
							END AS esdid,
							queid
						FROM
							questionario.questionario q
						JOIN questionario.acordo a ON a.acoid = q.acoid AND
									      			  a.acostatus = 'A'
						JOIN questionario.escolaacordo ea ON ea.acoid = a.acoid
						JOIN survey.usuarioresponsabilidade ur ON ur.escid = ea.escid
																  AND rpustatus = 'A'
																  AND usucpf = '{$_SESSION['usucpf']}'
						JOIN workflow.documento d ON d.docid = q.docid
						JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
						WHERE
							q.questatus = 'A'
						" . (count($where) > 0 ? " AND " . implode(" AND ", $where) : "");
		}

		// Monta SELECT trazendo todos os resultados, pois os respectivos perfis tem acesso total (sobrescreve os sql anteriores)
		if (possui_perfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR))){
			$sql   = array();
			$sql[] = "SELECT
							aconome,
							quetitulo,
							to_char(quedtinicio,'DD-MM-YYYY') AS quedtinicio,
							to_char(quedtfim,'DD-MM-YYYY') AS quedtfim,
							esddsc,
							CASE
								WHEN ed.esdid = " . WF_VALIDACAO_FINALIZADA . " THEN '<img src=\'/imagens/principal.gif\' style=\'cursor:pointer\' title=\"". ("Responder Questionário")."\" onclick=\'location.href=\"?modulo=principal/questionario&acao=A&queid='||queid||'\";\'>'
								ELSE '<img src=\'/imagens/principal_disabled.gif\' title=\"". ("Não é possível responder este questionário")."\" ;\'>'
							END AS esdid,
							queid
						FROM
							questionario.questionario q
						JOIN questionario.acordo a ON a.acoid = q.acoid
													  AND a.acostatus = 'A'
						JOIN workflow.documento d ON d.docid = q.docid
						JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
						WHERE
							q.questatus = 'A'
						" . (count($where) > 0 ? " AND " . implode(" AND ", $where) : "");
		}
//		$sql = "SELECT
//					f.*
//				FROM(
//					".(implode(' UNION ', $sql))."
//				) AS f
//				";
		$sql = implode(' UNION ', $sql);

		return $this->carregar($sql);
	}


	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if (get_class((object) $item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
			}else{
				$codicao[] = $item;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return $this->carregarObjeto($sql, get_class((object) $this));
	}

	public function pegaUmObjeto( Array $where , $coluna = null ){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'queid'){
				$codicao[] = "queid = {$item}";
			}elseif (get_class((object) $item) == 'Sistema'){
				$codicao[] = "sisid = " . $item->sisid;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::pegaUmObjeto($sql, get_class((object) $this));
	}

	public function pegaUmObjetoPorAcordo($acoid){
		$sql = "SELECT
					*
				FROM
					questionario.questionario
				WHERE
					questatus = 'A' AND
					acoid = {$acoid}";

		return parent::pegaUmObjeto($sql, get_class((object) $this));
	}

	public function antesExcluir(){
		$grupo = new QGrupo();
		$pergunta = new QPergunta();
//		$questionario = new QQuestionario();
		$retornoG = $grupo->lista( array($this), "count(*) AS total" );
		$retornoP = $pergunta->lista( array($this), "count(*) AS total" );
//		$retornoQ = $questionario->lista( array($this), "count(*) AS total" );
		$this->commit();

		if ( $retornoG[0]['total'] > 0 || $retornoP[0]['total'] > 0 || $retornoQ[0]['total'] > 0 ){
			return false;
		}else{
			return true;
		}
	}

	public function excluir( $id = null, $retorno = null ){
		$complemento = ";";
		if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );

		if($retorno){
			$complemento = "returning $retorno;";
		}

		if( !$this->antesExcluir($id) ) return false;

		$stChavePrimaria = $this->arChavePrimaria[0];
		$id = $id ? $id : $this->$stChavePrimaria;

		$sql = " UPDATE $this->stNomeTabela SET questatus = 'I' WHERE $stChavePrimaria = $id $complemento";
		if($retorno){
			return $this->pegaUm( $sql );
		}else{
			return $this->executar( $sql );
		}
	}

	public function pegaQuestionariosComAcordo($queidMatriz, $sisid = ''){

		$sisid = $sisid ? $sisid : $_SESSION['sisid'];

		$sql = "
				(SELECT
					que.queid AS codigo,
					'".("Questionário Matriz")." - ' || que.quetitulo AS descricao
				FROM
					questionario.questionario que
				WHERE
					acoid IS NULL
					AND
						queid_matriz IS NULL
					AND
						questatus = 'A'
					AND
						que.queid = {$queidMatriz}
					AND
						que.sisid = {$sisid})

				UNION

				(SELECT
					que.queid AS codigo,
					aco.aconome || ' - ' || que.quetitulo AS descricao
				FROM
					questionario.questionario que
				INNER JOIN
					questionario.acordo aco ON aco.acoid = que.acoid AND aco.acostatus = 'A'
				WHERE
					que.queid_matriz = {$queidMatriz}
					AND
						que.sisid = {$sisid}
					AND
						que.questatus = 'A'
				ORDER BY
					aco.aconome)

					";


		return $this->carregar($sql);
	}

	public function pegaEstadoQuestionario($queid){
		$sql = "SELECT
					doc.esdid
				FROM
					workflow.documento doc
				INNER JOIN
					questionario.questionario que ON que.docid = doc.docid AND que.queid = {$queid}";
		return $this->pegaUm($sql);
	}

	public function listaAcordoEscola($where = array()){
		$where[] = "que.questatus = 'A'";
		//ESCOLA
		if(!possui_perfil($GLOBALS["GRUPO_PERFIS_ACESSO3"])){
			// Buscas as escolas vinculadas o usuario
			$escola = new QEscola();
			$escolaVinculada = $escola->buscarEscolasPorUsuario(null, null);
			if(!$escolaVinculada)
				$where[] = "FALSE";
			else
				$where[] = "esc.escid IN (".implode(",", $escolaVinculada).")";
		//GESTOR
		}elseif(!possui_perfil($GLOBALS["GRUPO_PERFIS_ACESSO2"])){
			// Buscas as escolas vinculadas o usuario
			$acordo = new QAcordo();
			$acordoVinculado = $acordo->buscarAcordosPorUsuario(null, null);
			if(!$acordoVinculado)
				$where[] = "FALSE";
			else
				$where[] = "aco.acoid IN (".implode(",", $acordoVinculado).")";
		}
		$where = implode(" AND ", $where);
		$sql = "SELECT
					que.quetitulo,
					aco.aconome,
					COALESCE(esc.esccod,'N/A') ||' - '|| esc.escnome AS escnome,
					esc.esctelefone AS esctelefone,
					to_char(quedtinicio,'DD-MM-YYYY') AS quedtinicio,
					to_char(quedtfim,'DD-MM-YYYY') AS quedtfim,
					que.queid,
					esc.escid,
					COALESCE(totalpergunta,0) AS totalpergunta,
					COALESCE(totalperguntarespondida,0) AS totalperguntarespondida,
					esc.escddd AS escddd,
					aco.paiid AS paiid
				FROM
					questionario.questionario que
				INNER JOIN
					questionario.acordo aco ON aco.acoid = que.acoid AND aco.acostatus = 'A'
				INNER JOIN
					questionario.escolaacordo eac ON eac.acoid = aco.acoid AND eac.eacstatus = 'A'
				INNER JOIN
					questionario.escola esc ON esc.escid = eac.escid
				INNER JOIN
					workflow.documento doc ON doc.docid = que.docid
				INNER JOIN
					workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.esdid = " . WF_VALIDACAO_FINALIZADA . "
				LEFT JOIN(
					SELECT
						queid_atual as queid, count(*) as totalpergunta
					FROM
						questionario.pergunta
					WHERE
						itpid IS NULL AND
                        perstatus = 'A'
					GROUP BY
						queid_atual
				) t1 ON t1.queid = que.queid
				LEFT JOIN(
					SELECT
						queid, escid, COUNT(*) as totalperguntarespondida
					FROM (
						SELECT
							distinct
							p.perid, qr.queid, qr.escid
						FROM
							questionario.questionarioresposta qr
						JOIN questionario.pergunta p ON p.queid_atual = qr.queid
										AND p.itpid IS NULL and
                                        p.perstatus = 'A'
						JOIN questionario.resposta r ON r.perid = p.perid
										AND r.qrpid = qr.qrpid
					) f
					GROUP BY
						queid, escid
				) t2 ON t2.queid = que.queid AND t2.escid = esc.escid
				WHERE
					{$where}";
		return $this->carregar($sql);
	}

	public function listaAcordoProjeto($where = array()){
		$where[] = "que.questatus = 'A'";
		//PROJETO
		if(!possui_perfil($GLOBALS["GRUPO_PERFIS_ACESSO3"])){
			// Buscas os projetos vinculadas o usuario
			$projeto = new QProjeto();
			$projetoVinculado = $projeto->buscarProjetosPorUsuario(null, null);
			if(!$projetoVinculado)
				$where[] = "FALSE";
			else
				$where[] = "pro.proid IN (".implode(",", $projetoVinculado).")";
		//GESTOR
		}elseif(!possui_perfil($GLOBALS["GRUPO_PERFIS_ACESSO2"])){
			// Buscas os projetos vinculadas o usuario
			$acordo = new QAcordo();
			$acordoVinculado = $acordo->buscarAcordosPorUsuario(null, null);
			if(!$acordoVinculado)
				$where[] = "FALSE";
			else
				$where[] = "aco.acoid IN (".implode(",", $acordoVinculado).")";
		}
		$where = implode(" AND ", $where);
		$sql = "SELECT
					que.quetitulo,
					aco.aconome,
					pro.pronome,
					to_char(quedtinicio,'DD-MM-YYYY') AS quedtinicio,
					to_char(quedtfim,'DD-MM-YYYY') AS quedtfim,
					que.queid,
					pro.proid,
					COALESCE(totalpergunta,0) AS totalpergunta,
					COALESCE(totalperguntarespondida,0) AS totalperguntarespondida,
					aco.paiid AS paiid,
                    pai.paidescricao,
		            t2.qrpfinalizado
				FROM
					questionario.questionario que
				INNER JOIN
					questionario.acordo aco ON aco.acoid = que.acoid AND aco.acostatus = 'A'
				INNER JOIN
					questionario.projetoacordo pra ON pra.acoid = aco.acoid AND pra.prastatus = 'A'
				INNER JOIN
					questionario.projeto pro ON pro.proid = pra.proid
					AND pro.prostatus = 'A'
				INNER JOIN
					workflow.documento doc ON doc.docid = que.docid
				INNER JOIN
					workflow.estadodocumento esd ON esd.esdid = doc.esdid AND esd.esdid = " . WF_VALIDACAO_FINALIZADA . "
		        LEFT JOIN
			    	territorios.pais pai ON pai.paiid = pro.paiid
				LEFT JOIN(
					SELECT
						queid_atual as queid, count(*) as totalpergunta
					FROM
						questionario.pergunta
					WHERE
						itpid IS NULL
						AND
                        perstatus = 'A'
                        AND
                        qrpid IS NULL
                        AND
                        perobrigatorio IS TRUE
                        AND perid NOT IN (SELECT
                                               p2.perid
                                           FROM
                                               questionario.pergunta p2
                                           INNER JOIN
                                               questionario.grupopergunta g2 ON g2.grpid = p2.grpid
                                           WHERE
                                               g2.itpid IS NOT NULL)
					GROUP BY
						queid_atual
				) t1 ON t1.queid = que.queid
				LEFT JOIN(
					SELECT
						queid, proid, COUNT(*) as totalperguntarespondida, qrpfinalizado
					FROM (
						SELECT
							distinct
							p.perid, qr.queid, qr.proid, qr.qrpfinalizado
						FROM
							questionario.questionarioresposta qr
						JOIN questionario.pergunta p ON p.queid_atual = qr.queid
										            AND p.itpid IS NULL
                                                    AND p.perstatus = 'A'
                                                    AND p.qrpid IS NULL
                                                    AND p.perobrigatorio IS TRUE
                                                    AND p.perid NOT IN (SELECT
                                                                           p2.perid
                                                                       FROM
                                                                           questionario.pergunta p2
		  	                                                           INNER JOIN
		  	                                                               questionario.grupopergunta g2 ON g2.grpid = p2.grpid
		  	                                                           WHERE
		  	                                                               g2.itpid IS NOT NULL)
						JOIN questionario.resposta r ON r.perid = p.perid
										AND r.qrpid = qr.qrpid
					) f
					GROUP BY
						queid, proid, qrpfinalizado
				) t2 ON t2.queid = que.queid AND t2.proid = pro.proid
				WHERE
					{$where}";

		return $this->carregar($sql);
	}

	public function ordenaQuestionario($id){
	    $sql = "SELECT grpid AS id, 'g' AS tipo, grpordem FROM questionario.grupopergunta WHERE (queid = {$id} OR gru_grpid = {$id}) AND grpstatus = 'A'
                UNION ALL
                SELECT perid AS id, 'p' AS tipo, perordem FROM questionario.pergunta WHERE (queid = {$id} OR grpid = {$id}) AND perstatus = 'A'
                ORDER BY 3";

	    $dados = $this->carregar($sql);

	    if(is_array($dados)){
	        $i = 0;
	        foreach($dados as $dado){
	            $i++;
	            $ordemA = $i;
	            if($dado['tipo'] == 'g'){
    	            $sql = "UPDATE questionario.grupopergunta SET grpordem = $ordemA  WHERE grpid = {$dado['id']}";
	            }else if($dado['tipo'] == 'p'){
    	            $sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = {$dado['id']}";
	            }
	            $this->executar($sql);
	            $this->ordenaQuestionario($dado['id']);
	        }
	    }
	}


	public function listaObjetoGeral(Array $where = null, $qrpid = null , $padrao = true){
		$where  = (array) $where;

        $wherePergunta = array();
        $whereGrupo    = array("grpstatus = 'A'");

        $sqlPergunta = "SELECT per.perid AS id, per.pertitulo AS titulo, qor.qorordem AS ordem, per.perid_matriz AS id_matriz, per.pertipo AS tipo, per.percarregaitens AS carregaitens, 'p' AS tipoitem,
                               per.grpid AS grpidpai, per.queid AS queidpai, per.itpid AS itpidpai
                        FROM questionario.pergunta per
                        LEFT JOIN questionario.questionarioordem qor ON qor.perid = per.perid";

        $sqlGrupo    = "SELECT grp.grpid AS id, grp.grptitulo AS titulo, qor.qorordem AS ordem, null AS id_matriz, null AS tipo, null AS carregaitens, 'g' AS tipoitem,
                               grp.gru_grpid AS grpidpai, grp.queid AS queidpai, grp.itpid AS itpidpai
                        FROM questionario.grupopergunta grp
                        LEFT JOIN questionario.questionarioordem qor ON qor.grpid = grp.grpid";

        // adiciona uma verificação apenas se o qrpid for informado, nela verifica se o grupo possui o qrpid, se possuir ele filtra
        if( $padrao ){
            $whereGrupo[]       = " grp.qrpid IS NULL ";
            $wherePergunta[]    = " per.qrpid IS NULL ";
        }else{
            if( $qrpid ){
                $whereGrupo[]       = "( grp.qrpid IS NULL OR grp.qrpid = {$qrpid} )";
                $wherePergunta[]    = "( per.qrpid IS NULL OR per.qrpid = {$qrpid} )";
            }
        }

		foreach ($where as $k => $item){
			if ($k === 'itpid'){
				if ( is_array($item) ){
					$wherePergunta[] = "per.itpid IN (" . implode(", ", $item) . ")";
					$whereGrupo[]    = "grp.itpid IN (" . implode(", ", $item) . ")";
				}elseif($item == 'NULL'){
				    // Grupos e perguntas filhos de um item
				    $sqlNotGrupo    = "SELECT grpid FROM questionario.grupopergunta WHERE NOT itpid IS NULL AND grpstatus = 'A'";
				    $sqlNotPergunta = "SELECT perid FROM questionario.pergunta WHERE NOT itpid IS NULL AND perstatus = 'A'";

				    // Nenhum grupo filho de uma pergunta filha de um item
					$whereGrupo[]    = "grp.perid NOT IN ({$sqlNotPergunta})";

					// Nenhuma pergunta filha de um grupo filha de um item
					$wherePergunta[] = "(per.grpid IS NULL OR per.grpid NOT IN({$sqlNotGrupo}))";
					// Nenhum grupo filho de um grupo filho de um item
					$whereGrupo[]    = "(grp.gru_grpid IS NULL OR grp.gru_grpid NOT IN ({$sqlNotGrupo}))";

					// Nenhuma pergunta filha de um item
					$wherePergunta[] = "per.itpid IS NULL";
					// Nenhum grupo filho de um item
					$whereGrupo[]    = "grp.itpid IS NULL";
				}else{
					$wherePergunta[] = "per.itpid = " . ($item ? $item : 'NULL');
					$whereGrupo[]    = "grp.itpid = " . ($item ? $item : 'NULL');
				}
				continue;
			}elseif ($k === 'grpid'){
				$wherePergunta[] = "per.grpid     = " . $item;
				$whereGrupo[]    = "grp.gru_grpid = " . $item;
				continue;
			}elseif ($k === 'queid_ordem'){
				$wherePergunta[] = "qor.queid = " . $item;
				$whereGrupo[]    = "qor.queid = " . $item;
				continue;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$wherePergunta[] = "per.queid = " . $item->queid;
				$whereGrupo[]    = "grp.queid = " . $item->queid;
				continue;
			}elseif (get_class((object) $item) == 'QGrupo'){
				$wherePergunta[] = "per.grpid = " . $item->grpid;
				$whereGrupo[]    = "grp.gru_grpid = " . $item->grpid;
				continue;
			}elseif (get_class((object) $item) == 'QItemPergunta'){
				$wherePergunta[] = "per.itpid = " . $item->itpid;
				$whereGrupo[]    = "grp.itpid = " . $item->itpid;
				continue;
			}
		}

        $wherePergunta[] = " per.perstatus = 'A' ";
        $whereGrupo[]    = " grp.grpstatus = 'A' ";

		$sqlPergunta = $sqlPergunta .
				      ( count($wherePergunta) ? " WHERE " . implode(" AND ", $wherePergunta) : "" );

		$sqlGrupo = $sqlGrupo .
				      ( count($whereGrupo) ? " WHERE " . implode(" AND ", $whereGrupo) : "" );

        $sql = "({$sqlPergunta} UNION ALL {$sqlGrupo}) ORDER BY ordem";

        return $this->carregar($sql);
	}

	/**
	 * Busca os filhos diretos de um questionário pela ordem do próprio item
	 * @param integer $queid
	 * @return array
	 */
	public function getFilhosOrdemDefault($queid = null){
        $queid     = $queid ? $queid : $this->queid;
        $arRetorno = array();

        $sql = "SELECT grpid AS id, 'g' AS tipo, grpordem AS ordem FROM questionario.grupopergunta WHERE (queid = {$queid}) AND grpstatus = 'A'
                UNION ALL
                SELECT perid AS id, 'p' AS tipo, perordem AS ordem FROM questionario.pergunta WHERE (queid = {$queid}) AND perstatus = 'A'
                ORDER BY ordem ASC, tipo ASC";

        $dados = $this->carregar($sql);

        if(is_array($dados)){
            foreach($dados as $filho){
                switch ($filho['tipo']) {
                	case 'g': // grupo
                	    $arRetorno[] = new QGrupo($filho['id']);
                	break;
                	case 'p': // pergunta
                	    $arRetorno[] = new QPergunta($filho['id']);
                	break;
                }
            }
        }

        return $arRetorno;
	}

	/**
	 * Ordena um item e empurra todos os outros itens do mesmo nível
	 *
	 * @param object $objOrdenar
	 * @param integer $ordemNova
	 */
	public function ordenaNivelDefault($objOrdenar, $ordemNova){
	    $ordemAnterior = $objOrdenar->getOrdem();
	    $objPai        = $objOrdenar->getObjPai();

	    if($ordemAnterior != '' && $ordemAnterior != $ordemNova){
	        $filhos = $objPai->getFilhosOrdemDefault();

            foreach($filhos as $k => $filho){
	            if($filho->getOrdem() >= $ordemNova){
	                $filho->setOrdem($filho->getOrdem() + 1);
	                $filho->salvar();
	            }
	        }
	    }

	    $objOrdenar->setOrdem($ordemNova);
	    $objOrdenar->salvar();

        //recuperando os filhos atualizados e renumerando em ordem sequencial
        $filhos = $objPai->getFilhosOrdemDefault();
        foreach ($filhos as $key => $filho) {
            $filho->setOrdem($key + 1);
            $filho->salvar();
        }
	}

}