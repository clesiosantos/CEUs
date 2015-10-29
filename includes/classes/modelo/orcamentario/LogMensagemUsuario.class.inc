<?php

class LogMensagemUsuario extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "orcamentario.logmensagemusuario";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "lmuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array( 'lmuid'			=> null,
    									'menid'			=> null,
    									'usucpf'		=> null,
    									'lmudtcadastro'	=> null
									  );

	public function getLogMensagensUsuarioByUsuario($usucpf){

		$sql= "SELECT
					menid
			   FROM
			   		orcamentario.logmensagemusuario
			   WHERE
			   		usucpf = '{$usucpf}'";

	    return $this->carregarColuna($sql);
	}
}