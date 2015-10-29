<?php

class ProgramacaoExercicio extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "orcamentario.programacaoexercicio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prsano" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'prsano'				=> null,
									  	'prsdata_inicial'		=> null,
									  	'prsdata_termino'		=> null,
									  	'prsexerccorrente'		=> null,
									  	'prsstatus'				=> null,
									  	'prsativo'				=> null,
									  	'prsexercicioaberto'	=> null
									  );

	public function getLista( $where , $paginacao = false){

		$where[] = " prs.prsstatus = 'A' ";

		$sql=" SELECT
		            prs.prsano,
					TO_CHAR (prs.prsdata_inicial,'DD/MM/yyyy') as prsdata_inicial,
					TO_CHAR (prs.prsdata_termino,'DD/MM/yyyy') as prsdata_termino,
					CASE
						WHEN prs.prsexerccorrente = TRUE THEN 'Sim'
						WHEN prs.prsexerccorrente = FALSE THEN 'Não'
					END as prsexerccorrente,
					CASE
						WHEN prs.prsativo = 1 THEN 'Sim'
						WHEN prs.prsativo = 0 THEN 'Não'
					END as prsativo,
					CASE
						WHEN prs.prsexercicioaberto = TRUE THEN 'Sim'
						WHEN prs.prsexercicioaberto = FALSE THEN 'Não'
					END as prsexercicioaberto
				FROM
					{$this->stNomeTabela} prs
				WHERE
					". implode(" AND ", $where);

		if($paginacao){
            $retorno = Paginacao::getPaginacao($this, $sql,10);
            return $retorno;
        }else{
			$retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
        
	}

	public function gravarRegistro($dados){
		$prsano = $dados['prsano'];

		//verifica se já existe esse ano cadastrado
		$qtdAno = $this->getQdtAno($prsano);

		if( $qtdAno > 0){
			$sql ="UPDATE
				   	 	{$this->stNomeTabela}
				   SET
				   		prsdata_inicial 	= '{$dados['prsdata_inicial']}',
				   		prsdata_termino 	= '{$dados['prsdata_termino']}',
				   		prsexerccorrente 	= '{$dados['prsexerccorrente']}',
				   		prsativo			= '{$dados['prsativo']}',
				   		prsexercicioaberto	= '{$dados['prsexercicioaberto']}'
				   WHERE
				   		prsstatus = 'A' AND
				   		prsano = '{$prsano}'
				   RETURNING
				   		prsano";
		}else{
			$sql ="INSERT INTO
				   	 	{$this->stNomeTabela} ( prsano,
				   	 							prsdata_inicial,
				   	 							prsdata_termino,
				   	 							prsexerccorrente,
				   	 							prsativo,
				   	 							prsexercicioaberto,
				   	 							prsstatus )
				   VALUES ( '{$prsano}',
				   			'{$dados['prsdata_inicial']}',
				   			'{$dados['prsdata_termino']}',
				   			'{$dados['prsexerccorrente']}',
				   			'{$dados['prsativo']}',
				   			'{$dados['prsexercicioaberto']}',
				   			'A')
				   RETURNING
				   		prsano";

		}
		return $this->pegaUm($sql);

	}

	public function getQdtAno($prsano){
		$sql ="SELECT
					COUNT(prsano)
			   FROM
			   	 	{$this->stNomeTabela}
			   WHERE
			   		prsstatus = 'A' AND
			   		prsano = '{$prsano}'";

	   return $this->pegaUm($sql);

	}

	public function inabilitaCorrente(){
		$sql ="UPDATE
					{$this->stNomeTabela}
			   SET
				   	prsexerccorrente = 'f'";

	   return $this->executar($sql);
	}

	}