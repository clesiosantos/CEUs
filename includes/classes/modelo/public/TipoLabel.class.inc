<?php
	
class TipoLabel extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.tipolabel";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "tlbid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */ 
    protected $arAtributos     = array(
									  	'tlbid' 	=> null, 
    									'tlbdsc' 	=> null, 
									  );
									  							  
}