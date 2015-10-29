<?php

class QGrupo extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.grupopergunta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'grpid'                 => null,
    									'queid'                 => null, //Questionario
									  	'perid'                 => null, //Pergunta
    									'itpid'                 => null, //ItemPergunta
									  	'gru_grpid'             => null, //GrupoPergunta com ela mesma
									  	'grptitulo'             => null,
									  	'grpordem'              => null,
    									'grpstatus'             => null,
    									'grpajuda'              => null,
    									'grpid_matriz'          => null,
    									'queid_atual'           => null,
    									'grpid_pai'             => null,
                                        'grppermitemultiplo'    => null,
                                        'grpqtdmaximo'          => null,
                                        'qrpid'                 => null,  // FK para a tabela de questionário resposta criada para conseguir, relacionar grupos multiplos ao questionario de uma determinada escola
                                        'grpcondicao'           => null

									  );


	public function lista(Array $where = null, $coluna = null, $orderBy = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
			}elseif (get_class((object) $item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
			}elseif (get_class((object) $item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
			}elseif ($k == 'grpid'){
				$codicao[] = "gru_grpid = " . $item;
			}elseif ($k == 'itpid'){
				$codicao[] = "itpid = " . $item;
			}elseif ($k == 'queid'){
				$codicao[] = "queid = " . $item;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				WHERE
					grpstatus = 'A'
				" . ( count($codicao) ? " AND " . implode(" AND ", $codicao) : "" ) .
					($orderBy != null ? " ORDER BY {$orderBy}" : '');

		return $this->carregar($sql);
	}

    /**
     * Deleta o registro equivalente da tabela de ordenação
     *
     * @param integer $grpid
     * @param integer $queid
     */
    public function excluirOrdenacao($grpid = null, $queid = null ) {
        $grpid = $grpid ? $grpid : $this->grpid;
        // $queid = $queid ? $queid : ( $this->queid ? $this->queid : $this->queid_atual) ;

        $sql = "DELETE FROM questionario.questionarioordem WHERE (grpid = {$grpid})";
        $this->executar ( $sql );
    }

	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		foreach ($where as $k => $item){
			if ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif (get_class((object) $item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;
			}elseif (get_class((object) $item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " grpstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ). " ORDER BY grpordem";

				return $this->carregarObjeto($sql, get_class((object) $this));
	}

	public function pegaUmObjeto(Array $where = null, $coluna = null, $validaStatus = true){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpordem'){
				$codicao[] = "grpordem = " . $item;
				continue;
			}elseif ($k === 'queid'){
				$codicao[] = "queid = " . $item;
				continue;
			}elseif ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif (get_class((object) $item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;
			}elseif (get_class((object) $item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif (get_class((object) $item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}
		}

		if($validaStatus){
            // Adição do status na tabela ( 30/09/2013 )
            $codicao[] = " grpstatus = 'A' ";
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) .
				" " . ( $order ? " ORDER BY " . $order : "" ) .
				" LIMIT 1";

		return parent::pegaUmObjeto($sql, get_class((object) $this));

	}

	public function carregaUm(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ($k === 'grpid'){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif ($k === 'gru_grpid'){
				$codicao[] = "gru_grpid = " . $item;
				continue;
			}elseif ($k === 'grpordem'){
				$codicao[] = "grpordem = " . $item;
				continue;
			}elseif ($k === 'queid'){
				$codicao[] = "queid = " . $item;
				continue;
			}elseif ($k === 'itpid'){
				if ( is_array($item) ){
					$codicao[] = "itpid IN (" . implode(", ", $item) . ")";
				}else{
					$codicao[] = "itpid = " . ($item ? $item : 'NULL');
				}
				continue;
			}elseif (get_class((object) $item) == 'QQuestionario'){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif (get_class((object) $item) == 'QGrupo'){
				$codicao[] = "gru_grpid = " . $item->grpid;
				continue;
			}elseif (get_class((object) $item) == 'QItemPergunta'){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif (get_class((object) $item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}
		}


        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " grpstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) .
				" " . ( $order ? " ORDER BY " . $order : "" ) .
				" LIMIT 1";

		return parent::pegaUm($sql);

	}

//	function montaGrupo(Array $arr, $paiid){
//		foreach($arr as $obGru){
//
//			// IMG nível 3
//			$imgAltera = addslashes('<img src=\'/imagens/alterar.gif\' title=\'Alterar Tipo Documento\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&grpid=' . $obGru->grpid . '\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=480,height=240\'); w.focus();">');
//			$imgExclui = addslashes('<img src=\'/imagens/excluir.gif\' title=\'Excluir Grupo\' onClick="apagar(\'' . $_SERVER['REQUEST_URI'] . '&instrucao=excluir&grpid=' . $obGru->grpid . '\', \'Deseja apagar o Grupo ( ' . $obGru->grptitulo . ' )?\') "/>');
//			$arvore .= "arvore.add('gru_" . $obGru->grpid . "', 'que_" . $paiid . "', '$imgAltera $imgExclui " . $obGru->grptitulo . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
//
//			// IMG inclui nível 4
//			$imgInclui = addslashes('<img src=\'/imagens/gif_inclui.gif\' title=\'Incluir nova "Pergunta"\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=' . $obGru->grpid . '\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=400\'); w.focus();">');
//			$arvore .= "arvore.add('gru_imgInclui_" . $obGru->grpid . "', 'gru_" . $obGru->grpid . "', '" . $imgInclui . " Adicionar Pergunta', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
//
//			//$arrObPergunta = (array) $p->list  aObjeto( array( 0 => $obGru, "itpid" => null ) );
//			//$arvore .= $this->a->mo     ntaGrupo($arrObAba, $obQuestionario->queid);
//		}
//		return $arvore;
//	}

	public function ordena(Array $where){


	    $objQuestionario = new QQuestionario();
	    $objQuestionario->ordenaNivelDefault($this, $this->grpordem);
	    $this->commit();

// 		if($where['gru_grpid']){
// 			$where = "gru_grpid = " . $where['gru_grpid'];
// 		} elseif ($where['queid']){
// 			$where = "queid = " . $where['queid'];
// 		} else {
// 			$where = "itpid = " . $where['itpid'];
// 		}
// 		$sql = "SELECT grpid FROM questionario.grupopergunta WHERE $where AND grpstatus = 'A' ORDER BY grpordem";
// 		$arrGrpid = (array) $this->carregarColuna($sql);
// 		if($arrGrpid){
// 			$i = 0;
// 			foreach($arrGrpid as $grpidA){
// 				$i++;
// 				$ordemA = $i;
// 				$sql = "UPDATE questionario.grupopergunta SET grpordem = $ordemA  WHERE grpid = $grpidA";
// 				$this->executar($sql);
// 			}
// 			$this->commit();
// 		}
	}

	public function antesExcluir(){
		$grupo = new QGrupo();
		$pergunta = new QPergunta();
		$retornoG = $grupo->lista( array($this), "count(*) AS total" );
		$retornoP = $pergunta->lista( array($this), "count(*) AS total" );
		$this->commit();

		if ( $retornoG[0]['total'] > 0 || $retornoP[0]['total'] > 0 ){
			return false;
		}else{
			return true;
		}
	}

    /**
     * Busca os filhos diretos de um grupo pela ordem do próprio item
     *
     * @param integer $grpid
     * @return array
     */
    public function getFilhosOrdemDefault($grpid = null) {
        $grpid = $grpid ? $grpid : $this->grpid;
        $arRetorno = array ();

        $sql = "SELECT grpid AS id, 'g' AS tipo, grpordem AS ordem FROM questionario.grupopergunta WHERE (gru_grpid = {$grpid}) AND grpstatus = 'A'
        	    UNION ALL
        	    SELECT perid AS id, 'p' AS tipo, perordem AS ordem FROM questionario.pergunta WHERE (grpid = {$grpid}) AND perstatus = 'A'
        	    ORDER BY ordem ASC, tipo ASC";

        $dados = $this->carregar ( $sql );

        if (is_array ( $dados )) {
            foreach ( $dados as $filho ) {
                switch ($filho ['tipo']) {
                    case 'g' : // grupo
                        $arRetorno [] = new QGrupo ( $filho ['id'] );
                        break;
                    case 'p' : // pergunta
                        $arRetorno [] = new QPergunta ( $filho ['id'] );
                        break;
                }
            }
        }

        return $arRetorno;
    }

    public function setOrdem($ordem){
        $this->grpordem = $ordem;
    }

    public function getOrdem(){
        return $this->grpordem;
    }

    public function getObjPai(){
        if($this->perid){
            return new QPergunta($this->perid);
        }elseif($this->gru_grpid){
            return new QGrupo($this->gru_grpid);
        }elseif($this->queid){
            return new QQuestionario($this->queid);
        }elseif($this->itpid){
            return new QItemPergunta($this->itpid);
        }else{
            return null;
        }
    }

    public function getGruposMultiplosByQuestionario($queid, $qrpid ){

        $sql="SELECT
                    CASE
                       WHEN grp.grpqtdmaximo IS NULL THEN true
                       WHEN grp.grpqtdmaximo > (SELECT
                                                    COUNT(*)
                                                FROM
                                                    questionario.grupopergunta grp3
                                                WHERE
                                                    CASE
                                                   WHEN grp.grpid_pai IS NOT NULL THEN ( grp3.grpid_pai = grp.grpid_pai)
                                                   ELSE (grp3.grpid_pai = grp.grpid)
                                                    END AND
                                                    grp3.grpstatus = 'A' AND
                                                    grp3.qrpid = {$qrpid} ) THEN true
                        ELSE false END as exibebotao,
                    CASE
                        WHEN grppai.grpid IS NOT NULL THEN grppai.grpid
                        ELSE grp.grpid
                    END as grpid,
                    grp.grpid as grpid_real,
                    (SELECT
                        qor2.perid
                    FROM
                        questionario.questionarioordem qor2
                    JOIN
                           (SELECT
                                qor3.queid,
                                qor3.qorordem,
                                qor3.grpid
                            FROM
                                questionario.questionarioordem qor3
                            LEFT JOIN
                                questionario.grupopergunta grp2 ON qor3.grpid = grp2.grpid AND
                                                                   grp2.queid_atual = qor3.queid AND
                                                                   grp2.grpstatus = 'A'
                            WHERE
                                grp2.grppermitemultiplo IS TRUE ) sub1 ON sub1.queid = qor2.queid AND
                                                                          substring(qor2.qorordem from 1 for (char_length(sub1.qorordem) )) = sub1.qorordem
                    INNER JOIN
                        questionario.pergunta per ON per.perid = qor2.perid
                                                 AND per.perstatus = 'A'
                                                 AND per.itpid IS NULL
                    			                 AND per.perid NOT IN (SELECT
                                                                           p2.perid
                                                                       FROM
                                                                           questionario.pergunta p2
                                                                       INNER JOIN
                                                                           questionario.grupopergunta g2 ON g2.grpid = p2.grpid
                                                                       WHERE
                                                                           g2.itpid IS NOT NULL)

                    WHERE
                        qor2.queid = qor.queid AND
                        sub1.grpid = grp.grpid AND
                        qor2.perid IS NOT NULL
                    ORDER BY
                        qor2.qorordem DESC
                    LIMIT
                        1) AS perid
                FROM
                    questionario.grupopergunta grp
                LEFT JOIN
                    questionario.grupopergunta grppai ON grp.grpid_pai = grppai.grpid
                LEFT JOIN
                       questionario.questionarioordem qor ON qor.grpid = grp.grpid
                WHERE
                    qor.queid = {$queid} AND
                    (grp.qrpid = {$qrpid} OR grp.qrpid IS NULL)  AND
                    grp.grpstatus = 'A' AND
                    grp.grppermitemultiplo IS TRUE ";

        return $this->carregar($sql);
    }

    public function duplicaGrupo($grpid, $qrpid = null, $qrpidAnterior = null, $grpidPai = null){
        $modelo          = new QGeraScript();
        $obGrupoPergunta = $this->pegaUmObjeto( array("grpid" => $grpid ) );

        $modelo->queidNovo = $obGrupoPergunta->queid;

        if( $obGrupoPergunta->perid ){
            $pai      = "perid";
            $valorPai = $obGrupoPergunta->perid;
        }elseif( $obGrupoPergunta->gru_grpid ){
            $pai      = "gru_grpid";
            $valorPai = $obGrupoPergunta->gru_grpid;
        }elseif( $obGrupoPergunta->queid ){
            $pai      = "queid";
            $valorPai = $obGrupoPergunta->queid;
        }elseif( $obGrupoPergunta->itpid ){
            $pai      = "itpid";
            $valorPai = $obGrupoPergunta->itpid;
        }

        $ordem = $this->getOrdemItemDuplicado( $obGrupoPergunta );

        $qtdFilhos = $this->getQtdFilhos( $obGrupoPergunta->grpid, $qrpid );

        $grpqtdmaximo = $obGrupoPergunta->grpqtdmaximo ? $obGrupoPergunta->grpqtdmaximo : 'null';

        $grpid_matriz = $obGrupoPergunta->grpid_matriz ? $obGrupoPergunta->grpid_matriz : 'null';

        $qrpid        = $qrpid ? $qrpid : 'null';

        $grpcondicao = $obGrupoPergunta->grpcondicao;

        if($grpidPai){
            $titulo = $obGrupoPergunta->grptitulo;
        }else{
            $titulo   = $obGrupoPergunta->grptitulo . " (".($qtdFilhos + 1).")";
            $grpidPai = $obGrupoPergunta->grpid;
        }

		$arrDados = array(  "SQL"   => "INSERT INTO questionario.grupopergunta( varPai, grptitulo, grpstatus, grpordem, grpajuda, grpid_matriz, queid_atual,grpqtdmaximo,grppermitemultiplo, grpid_pai, qrpid, grpcondicao) VALUES ( resultPai, '".utf8_encode($titulo)."', '$obGrupoPergunta->grpstatus', 0, '".utf8_encode($obGrupoPergunta->grpajuda)."', {$grpid_matriz} , '$obGrupoPergunta->queid_atual' , {$grpqtdmaximo} , '$obGrupoPergunta->grppermitemultiplo','{$grpidPai}',{$qrpid}, '{$grpcondicao}' ) RETURNING grpid",
                            "tipo"  => "grupo",
                            "var"   => "grpid",
                            "itens" => array());

        // Busca Perguntas associadas
        $arrP = $modelo->montaArvorePergunta($obGrupoPergunta,$qrpid);
        if( is_array( $arrP )){
            array_push( $arrDados['itens'], $arrP );
        }

        // Busca Grupo Pergunta associadas
        $arrG = $modelo->montaArvoreGrupo($obGrupoPergunta, $qrpid);
        if( is_array( $arrG )){
            array_push( $arrDados['itens'], $arrG );
        }

        $grpidNovo = $modelo->percorreArrayGrupo($arrDados, $_SESSION['sisid'], $pai, $valorPai,$obGrupoPergunta->queid_atual, $qrpid, $qrpidAnterior);
        $grupoNovo = new QGrupo($grpidNovo);

        if($modelo->copiarRespostas && is_array($modelo->copiarRespostas)){
            foreach($modelo->copiarRespostas as $pergunta){
                $mQResposta = new QResposta();
			    $dadosResposta = $mQResposta->recuperarLinha("*", array("qrpid = {$qrpidAnterior}", "perid = {$pergunta['perid_original']}"), false);
			    if($dadosResposta){
                    $dadosResposta["perid"] = $pergunta['perid_novo'];
    		 	    $mQResposta->popularDadosObjeto($dadosResposta);
    		 	    $mQResposta->copiarResposta($qrpid);
			    }
            }
        }

        //organiza a ordem do prorpio item
        $objQuestionario = new QQuestionario();
        $objQuestionario->ordenaNivelDefault( $grupoNovo , (string) $ordem);

        //organiza a ordem na tabela de ordenação
        $objOrdem = new QQuestionarioOrdem();
        $objOrdem->ordenarQuestionario($obGrupoPergunta->getObjPai());

        // Replica tabela (pergunta)
		$tabela = new TabelaTabela();
		$tabela->replicarTabela($obGrupoPergunta->queid_atual);

        //$this->rollback();
        $this->commit();

		return $arrDados;
    }

    function getOrdemItemDuplicado( $obj ){

        $sql= "SELECT
                    COALESCE( MAX( grpordem ) , 0 ) AS maximo
                FROM
                    questionario.grupopergunta
                WHERE
                    grpid_pai = {$obj->grpid} AND
                    grpstatus = 'A';";

        $maxOrdemFilhos = $this->pegaUm($sql);

        if( $maxOrdemFilhos != 0 ){
            return $maxOrdemFilhos + 1;
        }else{
            return $obj->grpordem + 1;
        }
    }

    function getQtdFilhos( $grpid, $qrpid = null){

        $strQrpid = "";
        if( $qrpid ){
            $strQrpid = " AND qrpid = {$qrpid} ";
        }

        $sql= "SELECT
                    count(grpid)
                FROM
                    questionario.grupopergunta
                WHERE
                    grpid_pai = {$grpid} AND
                    grpstatus = 'A'
                    {$strQrpid};";

        $qtd = $this->pegaUm($sql);
        return $qtd;
    }

    function desabilitaGrupo( $objGrupo ){
        // Atualizando o status dos itens, grupos e perguntas filhos
        $objGrupo->dasabilitaItensByGrupo( $objGrupo );

        //organiza a ordem do prorpio item
        $objQuestionario = new QQuestionario();
        $objQuestionario->ordenaNivelDefault( $objGrupo , $objGrupo->getOrdem());

        //organiza a ordem na tabela de ordenação
        $objOrdem = new QQuestionarioOrdem();
        $objOrdem->deletaItensPeloPai( $objGrupo , false);
        $objOrdem->ordenarQuestionario($objGrupo->getObjPai());

        $objGrupo->grpstatus = 'I';
        $objGrupo->salvar();
        $objGrupo->commit();

        //Atualizar os textos dos filhos
        $this->atualizaTextoFilhos($objGrupo->grpid_pai, true, $objGrupo->qrpid);
    }

    public function atualizaTextoFilhos($grpid, $commit = true, $qrpid = false){
        $grupoPai = new QGrupo($grpid);
        $textoPai = $grupoPai->grptitulo;

        $strQrpid = "true";
        if($qrpid){
            $strQrpid = $qrpid ? " qrpid = {$qrpid} " : "true";
        }

        //recupera os filhos
        $filhos = $grupoPai->recuperarTodos("grpid", array("grpid_pai = '{$grpid}'","grpstatus = 'A'", $strQrpid), "grpid");
        if(is_array($filhos) && !empty($filhos)){
            foreach ($filhos as $key => $filho) {
                $grupo = new QGrupo($filho['grpid']);
                $grupo->grptitulo = $textoPai." (".($key + 1).")";
                $grupo->salvar();
            }
        }

        if( $commit ){
            $this->commit();
        }
    }

    public function dasabilitaItensByGrupo( $objGrupo ){
        $objOrdem    = new QQuestionarioOrdem();
        $objItem     = new QItemPergunta();
        $objPergunta = new QPergunta();

        $ordem    = $objOrdem->getOrdemByObj( $objGrupo );

        // verifica se a ordem do item não veio em branco e se ela é maior ou igual a 4 ( tamanho minimo possível )
        if( !empty($ordem->qorordem) && ( count($ordem->qorordem) >= 4 ) ){
            //recupera os grupos filhos ( utilizando a tabela de ordem )
            $arGrupos = $objOrdem->getGruposFilhosByOrdem($ordem->qorordem, $objGrupo->queid_atual);
            $objGrupo->desabilitaGrupos($arGrupos);

            //recupera os itens filhos ( utilizando a tabela de ordem )
            $arItens = $objOrdem->getItensFilhosByOrdem($ordem->qorordem, $objGrupo->queid_atual);
            $objItem->desabilitaItens($arItens);

            //recupera as perguntas filhos ( utilizando a tabela de ordem )
            $arPerguntas = $objOrdem->getPerguntasFilhasByOrdem($ordem->qorordem, $objGrupo->queid_atual);
            $objPergunta->desabilitaPerguntas($arPerguntas);
        }
    }

    public function desabilitaGrupos( $arGrupos ){
        if(is_array($arGrupos) && !empty($arGrupos)){
            $sql = "UPDATE {$this->stNomeTabela} SET grpstatus = 'I' WHERE grpid IN (".  implode(' , ', $arGrupos).");";
            $this->executar($sql);
        }
    }

    public function verificaMultiploHabilitado( $grpid ){
        $sql = "SELECT COUNT(*) FROM {$this->stNomeTabela} WHERE grpid_pai = {$grpid} AND grpstatus = 'A'";
        $quantidade = $this->pegaUm($sql);

        return $quantidade > 0 ? 'N' : 'S';
    }

    public function getGrupoPrimeiroNivelByQuestionario($queid){
        $sql = "SELECT
                    *
                FROM
                    questionario.grupopergunta grp
                INNER JOIN
                    questionario.questionarioordem qor ON qor.grpid = grp.grpid
                WHERE
                    grp.queid = {$queid}
                  AND
                    grp.grpstatus = 'A'
                  AND
                    CHAR_LENGTH(qor.qorordem) = 4
                  ORDER BY
                    qor.qorordem ASC";
        $rsGrupos = $this->carregar($sql);

        return $rsGrupos ? $rsGrupos : array();
    }

    public function getGruposMultiplosFilhosByGrupo($grpid, $qrpid = null){
        $whereResposta = "";
        if(!is_null($qrpid) && $qrpid){
            $whereResposta = " AND grp.qrpid = {$qrpid}";
        }

        $sql = "SELECT
                    *
                FROM
                    {$this->stNomeTabela} grp
                WHERE
                    grp.grpid_pai = {$grpid}
                    AND
                    grp.grpstatus = 'A'
                    {$whereResposta}";
        $rs = $this->carregar($sql);

        return $rs ? $rs : array();
    }


    public function recuperarGruposFilhosByGrupo(){
        $mOrdem = new QQuestionarioOrdem();
        $ordem  = $mOrdem->getOrdemByObj($this);

        $sql = "SELECT
                    *
                FROM
                    {$this->stNomeTabela} grp
                INNER JOIN
                    questionario.questionarioordem qor ON qor.grpid = grp.grpid
                WHERE
                    grp.grpstatus = 'A'
                    AND
                    qor.qorordem ILIKE '{$ordem->qorordem}%'
                    AND
                    grp.queid_atual = {$this->queid_atual}
                    AND
                    grp.grpid != {$this->grpid}";
        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

    public function recuperarPerguntasFilhasByGrupo(){
        $mOrdem = new QQuestionarioOrdem();
        $ordem  = $mOrdem->getOrdemByObj($this);

        $sql = "SELECT
                    *
                FROM
                    questionario.pergunta per
                INNER JOIN
                    questionario.questionarioordem qor ON qor.perid = per.perid
                WHERE
                    per.perstatus = 'A'
                    AND
                    qor.qorordem ILIKE '{$ordem->qorordem}%'
                    AND
                    per.queid_atual = {$this->queid_atual}";

        $dados = $this->carregar($sql);
        return $dados ? $dados : array();
    }

    public function realizarVerificacao($dadosCondicao){
        // verifica se há condição a ser realizada
        $grpcondicao = trim( $this->grpcondicao );

        // captura dados da chamada
        $chamada = tratarChamadaCondicao( $grpcondicao, $dadosCondicao );
        $funcao = $chamada['funcao'];
        $parametros = $chamada['parametros'];

        if ( !$funcao )
        {
            return true;
        }
        else
        {
            // realiza a verificação externa
            return call_user_func_array( $funcao, $parametros );
        }
    }
}