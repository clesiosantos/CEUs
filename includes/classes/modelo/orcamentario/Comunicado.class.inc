<?php

class Comunicado extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "orcamentario.comunicado";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "comid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array( 'comid'             => null,
    									'comdsc'            => null,
    									'arqid'             => null,
    									'comstatus'         => null,
    									'usucpf'            => null,
                                        'comdtcadastro'		=> null
									  );

	public function getLista( $where , $paginacao = false, $orderby = ""){

        $where[] = " com.comstatus = 'A' ";
        
        if( $orderby) {
            $order = " ORDER BY {$orderby}";
        }
        
		$sql=" SELECT
					com.comdsc,
                    usu.usunome,
                    TO_CHAR (com.comdtcadastro,'DD/MM/yyyy') AS comdtcadastro,
					com.comid,
					com.arqid
				FROM
					{$this->stNomeTabela} com
                JOIN
                    seguranca.usuario usu ON usu.usucpf = com.usucpf
				WHERE
					". implode(" AND ", $where)." {$order}";

		if($paginacao){
            $retorno = Paginacao::getPaginacao($this, $sql,10);
            return $retorno;
        }else{
			$retorno = $this->carregar($sql);
            return $retorno ? $retorno : array();
        }
        
	}
}