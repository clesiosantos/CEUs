<?php

class RegiaoAdministrativa extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "territorios.regiaoadministrativa";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rgaid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'rgaid'		=> null,
									  	'rgadsc'	=> null,
									  	'rgastatus'	=> null
									  );

}