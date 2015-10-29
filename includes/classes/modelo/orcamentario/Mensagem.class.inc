<?php

class Mensagem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "orcamentario.mensagem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "menid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array( 'menid'			=> null,
    									'usucpf'		=> null,
    									'mendtcadastro'	=> null,
    									'mentitulo'		=> null,
    									'mentexto'		=> null,
    									'menstatus'		=> null,
    									'prsano'		=> null
									  );

	public function getLista( $where , $paginacao = false){

		$where[] = " men.prsano = '{$_SESSION['exercicio']}' ";

		$sql=" SELECT
					men.prsano,
					usu.usunome,
					men.mentitulo,
					men.mentexto,
					TO_CHAR(men.mendtcadastro,'DD/MM/yyyy') as mendtcadastro,
					CASE
						WHEN men.menstatus = 'A' THEN 'Ativo'
						WHEN men.menstatus = 'I' THEN 'Inativo'
					END as menstatus,
					men.menid
				FROM
					{$this->stNomeTabela} men
				JOIN
					seguranca.usuario usu ON usu.usucpf = men.usucpf
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

	public function getListaIdMensagens(){

		$sql= "SELECT
					menid
			   FROM
			   		orcamentario.mensagem
			   WHERE
			   		menstatus = 'A'";

	    return $this->carregarColuna($sql);
	}
}