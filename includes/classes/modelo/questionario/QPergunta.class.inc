<?php

class QPergunta extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.pergunta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "perid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */

    protected $arAtributos     = array(
									  	'perid' 				  => null,
									  	'grpid' 				  => null,
									  	'queid' 				  => null,
									  	'itpid' 				  => null,
//									  	'perboleana' 			  => null,
//									  	'perflagitem' 			  => null,
//									  	'perflagmultiescolha' 	  => null,
									  	'pertitulo' 			  => null,
									  	'perordem' 				  => null,
									  	'perajuda' 				  => null,
    									'perobrigatorio' 		  => null,
    									'permascara'			  => null,
    									'pertipo' 				  => null,
    									'permultipla' 			  => null,
    									'perposacao' 			  => null,
    									'pertipografico' 		  => null,
    									'pertabela' 			  => null,
    									'persugerido' 			  => null,
    									'usucpfsugestao' 		  => null,
    									'perregra' 				  => null,
    									'perid_matriz'			  => null,
    									'queid_atual'			  => null,
    									'percarregaitens'         => null,
    									'perdescricao'            => null,
    									'perqtdchardescricao'     => null,
    									'perdescricaoobrigatoria' => null,
    									'perextensao'             => null,
    									'perstatus'               => null,
    									'qrpid'                   => null,
    									'percondicao'             => null,
    									'perexibemapa'            => null,
    									'pervalidacaoonblur'      => null,
									  );

	public function lista(Array $where = null, $coluna = null, $orderBy = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k=>$item){
			if (get_class((object) $item) == 'QGrupo'){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class((object) $item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( get_class((object) $item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif(is_numeric($k)){
			    $codicao[] = $item;
			    continue;
			}elseif ( $k == 'queid' ){
				$codicao[] = "queid = " . $item;
				continue;
			}elseif ( $k == 'queid_atual' ){
				$codicao[] = "queid_atual = " . $item;
				continue;
			}elseif ( $k == 'grpid' ){
				$codicao[] = "grpid = " . $item;
				continue;
			}elseif ( $k == 'itpid' ){
				$codicao[] = "itpid = " . $item;
				continue;
			}elseif ( $k == 'pertipo' ){
				$codicao[] = "pertipo = '{$item}'";
				continue;
			}elseif ( $k == 'notpertipo' ){
				$codicao[] = "pertipo != '{$item}'";
				continue;
			}
		}
        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " perstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . ($orderBy != null ? " ORDER BY {$orderBy}" : '');

		return $this->carregar($sql, get_class((object) $this));
	}

	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;
			}elseif ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class((object) $item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class((object) $item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class((object) $item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " perstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . " ORDER BY perordem";
		return $this->carregarObjeto($sql, get_class((object) $this));
	}

//	$this->pegaUmObjeto()

	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;
			}elseif ( get_class((object) (object)  $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class((object) $item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class((object) $item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class((object) $item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " perstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) .
				" " . ( $order ? " ORDER BY " . $order : "" ) .
				" LIMIT 1";
//		dbg($sql);
		return parent::pegaUmObjeto($sql, get_class((object) $this));
	}

	public function carregaUm(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;
			}elseif ($k === 'perid_matriz'){
				$codicao[] = "perid_matriz = {$item}";
				continue;
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;
			}elseif ($k === 'queid_atual'){
				$codicao[] = "queid_atual = {$item}";
				continue;
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;
			}elseif ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class((object) $item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( get_class((object) $item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( get_class((object) $item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " perstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) .
				" " . ( $order ? " ORDER BY " . $order : "" ) .
				" LIMIT 1";
// 		dbg($sql);
		return parent::pegaUm($sql);
	}

	public function carregarColuna(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if (get_class((object) $item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( get_class((object) $item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ($k === 'itpid'){
				$codicao[] = "itpid IN(" . (is_array($item) ? implode(', ', $item) : $item) . ")";
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " perstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::carregarColuna($sql, $coluna);
	}


	public function montaPergunta(Array $arr, $obPai){

		if (get_class((object) $obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class((object) $obPai) == 'QGrupo'){
			$idPai = 'gru_' . $obPai->grpid;
		}elseif (get_class((object) $obPai) == 'QItemPergunta'){
			$idPai = 'item_' . $obPai->itpid;
		}

		foreach($arr as $obPergunta){

				// IMG nível 4
				$imgAltera = addslashes('<img src=\'/imagens/alterar.gif\' title=\'Alterar Tipo Documento\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=' . $obPergunta->perid . '\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=400\'); w.focus();">');
				$imgExclui = addslashes('<img src=\'/imagens/excluir.gif\' title=\'Excluir Tipo Documento\' onClick="apagar(\'' . $_SERVER['REQUEST_URI'] . '&instrucao=excluir&perid=' . $obPergunta->perid . '\', \'Deseja apagar a Pergunta ( ' . substr($obPergunta->pertitulo, 0, 80) . (strlen($obPergunta->pertitulo) > 80 ? '...' : '') . ' )?\') "/>');
				$arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui " . substr($obPergunta->pertitulo, 0, 100) . (strlen($obPergunta->pertitulo) > 100 ? '...' : '') . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";

				//$arrObItem = (array) $it->listaObjeto( array( $obPergunta) );
		}
		return $arvore;
	}

	public function perguntaRespondida( $perid = null, $qrpid = null ){

		$perid = $perid ? $perid : $this->perid;

		if($qrpid){
			$where = "AND r.qrpid = {$qrpid}";
		}

		$sql = "SELECT
						r.perid
				FROM
						questionario.resposta r
				WHERE
					r.perid = {$perid} {$where}";

		return parent::carregarColuna($sql);

	}

	public function perguntaRespondidaComItem( $perid, $qrpid ){

		$sql = "SELECT
						ip.itpid
				FROM
						questionario.itempergunta ip
				INNER JOIN questionario.resposta r using(itpid)
				LEFT JOIN questionario.pergunta p using(itpid)
				LEFT JOIN questionario.grupopergunta g using(itpid)
				WHERE
					ip.perid = {$perid}
					AND r.qrpid = {$qrpid}
		--			AND	(p.itpid IS NOT NULL OR g.itpid IS NOT NULL)";

		return parent::carregarColuna($sql);

	}

	public function ordena(Array $where){

	    $objQuestionario = new QQuestionario();
	    $objQuestionario->ordenaNivelDefault($this, $this->perordem);
	    $this->commit();

// 		if($where['grpid']){
// 			$where = "grpid = " . $where['grpid'];
// 		} elseif ($where['queid']){
// 			$where = "queid = " . $where['queid'];
// 		} else {
// 			$where = "itpid = " . $where['itpid'];
// 		}
// 		$sql = "SELECT perid FROM questionario.pergunta WHERE $where AND perstatus = 'A' ORDER BY perordem";
// 		$arrPerid = (array) parent::carregarColuna($sql);
// 		if($arrPerid){
// 			$i = 0;
// 			foreach($arrPerid as $peridA){
// 				$i++;
// 				$ordemA = $i;
// 				$sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = $peridA";
// 				$this->executar($sql);
// 			}
// 			$this->commit();
// 		}
	}

	public function antesExcluir(){
		$itp 		   = new QItemPergunta();

        $ordem         = new QQuestionarioOrdem();
        $ordem->deletaItensPeloPai($this,false);

		$arrObItemPerg = (array) $itp->listaObjeto( array($this), "itpid" );
		// Faz iteração verificando se os "itens perguntas" podem ser deletados
		$retorno = true;
		foreach($arrObItemPerg as $obItemPerg){
			if ( !$obItemPerg->antesExcluir() ){
				$retorno = false;
				break;
			}
		}

		// Se TODOS os "itens perguntas" podem ser deletados, ou seja,
		// não possuem sub-pergunta, faz a deleção dos mesmos.
		if ($retorno){
			foreach($arrObItemPerg as $obItemPerg){
				$obItemPerg->excluir();
				$this->commit();
			}
		}
		return $retorno;
	}


	public function getCountPerSugeridoPorQuestionario($queid){
		global $db;
		$sql = "SELECT
					COUNT(0) AS qtd
				FROM
					questionario.legendapergunta leg
			 	INNER JOIN
				 	questionario.pergunta per ON per.perid = leg.perid AND
                                                 per.perstatus = 'A'
			 	WHERE
			 		per.persugerido = TRUE AND leg.queid = {$queid}";
		$qtd = $db->pegaUm($sql);
		return $qtd;
	}

	public function recuperarColunaParent( $stColuna, $arClausulas){

        $arClausulas[] = " perstatus = 'A' ";

		$sql  = " SELECT $stColuna FROM $this->stNomeTabela ";
		$sql .= " WHERE ". implode( " AND ", $arClausulas );

		$resultado = parent::carregarColuna($sql);
		return $resultado;
	}

	public function listarPorQuestionario($queid, Array $param=array())
	{
		$where = count($param['where']) ? " AND " . implode(" AND ", $param['where']) : "";
		$sql = "SELECT
					*
				FROM
					questionario.pergunta p
				WHERE
                    perstatus = 'A' AND
					queid_atual = {$queid}
					{$where}";
		$result = ($param['obj'] ? $this->carregarObjeto($sql, get_class((object) $this)) : $this->carregar($sql));

		return ($result ? $result : array());
	}

    /**
     * Busca os filhos diretos de uma pergunta pela ordem do próprio item
     *
     * @param integer $perid
     * @return array
     */
    public function getFilhosOrdemDefault($perid = null) {
        $perid = $perid ? $perid : $this->perid;
        $arRetorno = array ();

        $sql = "SELECT itpid AS id, itpordem AS ordem FROM questionario.itempergunta WHERE (perid = {$perid}) AND itpstatus = 'A'
	            ORDER BY ordem ASC, id ASC";

        $dados = $this->carregar ( $sql );

        if (is_array ( $dados )) {
            foreach ( $dados as $filho ) {
                $arRetorno [] = new QItemPergunta ( $filho ['id'] );
            }
        }

        return $arRetorno;
    }

    /**
     * Deleta o registro equivalente da tabela de ordenação
     *
     * @param integer $perid
     * @param integer $queid
     */
    public function excluirOrdenacao($perid = null, $queid = null ) {
        $perid = $perid ? $perid : $this->perid;
        $queid = $queid ? $queid : ( $this->queid ? $this->queid : $this->queid_atual) ;

        $sql = "DELETE FROM questionario.questionarioordem WHERE (perid = {$perid} AND queid = {$queid})";
        $this->executar ( $sql );
    }


    public function setOrdem($ordem){
        $this->perordem = $ordem;
    }

    public function getOrdem(){
        return $this->perordem;
    }

    public function getObjPai(){
        if($this->grpid){
            return new QGrupo($this->grpid);
        }elseif($this->queid){
            return new QQuestionario($this->queid);
        }elseif($this->itpid){
            return new QItemPergunta($this->itpid);
        }else{
            return null;
        }
    }

    public function desabilitaPerguntas( $arPerguntas ){
        if(is_array($arPerguntas) && !empty($arPerguntas)){
            $sql = "UPDATE {$this->stNomeTabela} SET perstatus = 'I' WHERE perid IN (".  implode(' , ', $arPerguntas).");";
            $this->executar($sql);
        }
    }

    public function getPrimeiroNivelByQuestionario($queid){
        $sql = "SELECT
                    *
                FROM
                    questionario.pergunta per
                INNER JOIN
                    questionario.questionarioordem qor ON qor.perid = per.perid
                WHERE
                    per.queid = {$queid}
                    AND
                    per.perstatus = 'A'
                    AND
                    CHAR_LENGTH(qor.qorordem) = 4
                ORDER BY
                    qor.qorordem ASC";
        $rsPerguntas = $this->carregar($sql);

        return $rsPerguntas ? $rsPerguntas : array();
    }

    public function getGrupoPai($perid){
        $this->carregarPorId($perid);

        if($this->grpid){
            return $this->grpid;
        }elseif($this->itpid){
            $mItem = new QItemPergunta($this->itpid);
            return $this->getGrupoPai($mItem->perid);
        }elseif($this->queid){
            return null;
        }

        return null;
    }

    public function realizarVerificacao($dadosCondicao){
        // verifica se há condição a ser realizada
        $percondicao = trim( $this->percondicao );

        // captura dados da chamada
        $chamada = tratarChamadaCondicao( $percondicao, $dadosCondicao );
        $funcao = $chamada['funcao'];
        $parametros = $chamada['parametros'];

        if ( !$funcao ) {
            return true;
        } else {
            // realiza a verificação externa
            return call_user_func_array( $funcao, $parametros );
        }
    }

}