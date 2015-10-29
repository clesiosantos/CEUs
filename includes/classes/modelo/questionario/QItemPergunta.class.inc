<?php

class QItemPergunta extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.itempergunta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
									  	'itpid' 		=> null,
									  	'perid' 		=> null,
									  	'itptitulo' 	=> null,
									  	'queid_atual' 	=> null,
									  	'itpid_matriz' 	=> null,
									  	'itpordem' 	    => null,
									  	'itpstatus' 	=> null
									  );

	public function lista(Array $where = null, $coluna = null, $orderBy = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if (get_class((object) $item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
			}elseif ($k == 'perid'){
				$codicao[] = "perid = " . $item;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " itpstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" )
				  . ($orderBy != null ? " ORDER BY {$orderBy}" : '');

		return $this->carregar($sql, get_class((object) $this));
	}

	public function carregarColuna(Array $where = null, $coluna = 'itpid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if (get_class((object) $item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
			}elseif ( $k == 'perid'){
				$codicao[] = "perid IN ('" . (is_array($item) ? implode("', '", $item) : $item) . "')";
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " itpstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" )." ORDER BY itptitulo";

		return parent::carregarColuna($sql, $coluna);
	}

	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(", ip.", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( get_class((object) $item) == 'QItemPergunta' ){
				$codicao[] = "ip.itpid = " . $item->itpid;
				continue;
			}elseif ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "ip.perid = " . $item->perid;
				continue;
			}elseif ( $k === 'itpid' ){
				$codicao[] = "ip.itpid IN ('" . (is_array($item) ? implode("', '", $item) : $item) . "')";
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " ip.itpstatus = 'A' ";

// 		$orderBy = !$coluna || isset($coluna['itptitulo']) ? 'ip.itptitulo' : 'ip.itpid"';
		$orderBy = "qor.qorordem ASC, ip.itpordem ASC, ip.itptitulo ";

		$sql = "SELECT
					 " . ($coluna ? "ip.{$coluna}" : "ip.*") . "
				FROM
					 {$this->stNomeTabela} ip
			    LEFT JOIN
			        questionario.questionarioordem qor ON qor.itpid = ip.itpid
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) ." ORDER BY {$orderBy} ASC";

		return $this->carregarObjeto($sql, get_class((object) $this));
	}

	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "ip.itpid = " . $item;
				continue;
			}elseif ( $k === 'respondido' && $item === true ){
				$inner = " JOIN questionario.resposta r USING(itpid) ";
				continue;
			}elseif ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "ip.perid = " . $item->perid;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $codicao[] = " ip.itpstatus = 'A' ";

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela} ip
					{$inner}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		return parent::pegaUmObjeto($sql, get_class((object) $this));
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

	public function possuiPerguntaGrupoAssociado(Array $param = null){
		if ( !$param ){
			$clausula[] = "ip.itpid = " . $this->itpid;
		}
		$param = (array) $param;
		foreach($param as $k => $value){
			if ($k == "itpid"){
				$clausula[] = "ip.itpid = " . $value;
				continue;
			}elseif ( get_class((object) $value) == "QItemPergunta" ){
				$clausula[] = "ip.itpid = " . $value->itpid;
				continue;
			}
		}

        // Adição do status na tabela ( 30/09/2013 )
        $clausula[] = " ip.itpstatus = 'A' ";
//         $clausula[] = " p.perstatus = 'A' ";
//         $clausula[] = " gp.grpstatus = 'A' ";

		$sql = "SELECT
					count(*)
				FROM
					questionario.itempergunta ip
				LEFT JOIN questionario.pergunta p USING(itpid)
				LEFT JOIN questionario.grupopergunta gp USING(itpid)
				WHERE
				(p.itpid IS NOT NULL OR gp.itpid IS NOT NULL) AND
				" . implode(" AND ", $clausula);

		return $this->pegaUm($sql);
	}

	public function verificaGrupo( $item ){
		if( $item ){
			$sql = "SELECT grpid FROM questionario.grupopergunta WHERE grpstatus = 'A' AND itpid = ". $item;
			return $this->pegaUm($sql);
		}
	}

	public function verificaPergunta( $item ){
		if( $item ){
			$sql = "SELECT count(perid) as per FROM questionario.pergunta WHERE perstatus = 'A' AND itpid = ". $item;
			return $this->pegaUm($sql);
		}
	}

	public function carregaItensPergunta( $perid, $coluna = null ){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela} ip
				WHERE perid = ".$perid." AND itpstatus = 'A'; ";

		return parent::carregar($sql);
	}

	public function recuperarColunaParent( $stColuna, $arClausulas){

        $arClausulas[]  = " itpstatus = 'A' ";

		$sql  = " SELECT $stColuna FROM $this->stNomeTabela ";
		$sql .= " WHERE ". implode( " AND ", $arClausulas );

		$resultado = parent::carregarColuna($sql);
		return $resultado;
	}

    /**
     * Busca os filhos diretos de um item pela ordem do próprio item
     *
     * @param integer $itpid
     * @return array
     */
    public function getFilhosOrdemDefault($itpid = null) {
        $itpid = $itpid ? $itpid : $this->itpid;
        $arRetorno = array ();

         $sql = "SELECT grpid AS id, 'g' AS tipo, grpordem AS ordem FROM questionario.grupopergunta WHERE (itpid = {$itpid}) AND grpstatus = 'A'
                UNION ALL
                SELECT perid AS id, 'p' AS tipo, perordem AS ordem FROM questionario.pergunta WHERE (itpid = {$itpid}) AND perstatus = 'A'
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


    public function setOrdem($ordem){
        $this->itpordem = $ordem;
    }

    public function getOrdem(){
        return $this->itpordem;
    }

    public function getObjPai(){
        if($this->perid){
            return new QPergunta($this->perid);
        }else{
            return null;
        }
    }

    public function desabilitaItens( $arItens ){
        if(is_array($arItens) && !empty($arItens)){
            $sql = "UPDATE {$this->stNomeTabela} SET itpstatus = 'I' WHERE itpid IN (".  implode(' , ', $arItens).");";
            $this->executar($sql);
        }
    }
}