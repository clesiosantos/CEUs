<?php

class Menu extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "seguranca.menu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "mnuid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array( 'mnuid' 		=> null,
									  	'mnucod' 		=> null,
									  	'mnucodpai' 	=> null,
									  	'mnudsc' 		=> null,
									  	'mnustatus' 	=> null,
									  	'mnulink' 		=> null,
									  	'mnutipo' 		=> null,
									  	'mnustile' 		=> null,
									  	'mnuhtml' 		=> null,
									  	'mnusnsubmenu' 	=> null,
									  	'mnutransacao' 	=> null,
									  	'mnushow' 		=> null,
									  	'abacod' 		=> null,
									  	'mnuhelp' 		=> null,
									  	'sisid'			=> null,
									  	'mnuidpai'		=> null,
									  	'mnuordem'		=> null );
}