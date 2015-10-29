<?php
	
class Idioma extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.idioma";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "idiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */ 
    protected $arAtributos     = array(
									  	'idiid' 	=> null, 
    									'idiidioma' => null, 
    									'idicodigo' => null,
									  );

}