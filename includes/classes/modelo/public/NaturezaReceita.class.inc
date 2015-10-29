<?php
	
class NaturezaReceita extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.naturezareceita";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "nrcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'nrcid'     => null, 
									  	'ctecod'    => null, 
									  	'oricod'    => null, 
									  	'espcod'    => null, 
									  	'rbrcod'    => null, 
									  	'alncod'    => null, 
									  	'salcod'    => null, 
									  	'nrcano'    => null, 
									  	'nrccod'    => null, 
									  	'nrcdsc'    => null, 
									  	'nrcstatus' => null, 
									  );
    
    public function getIdByCodigo($nrccod){
        
        $sql="SELECT
                   nrcid
              FROM
                  {$this->stNomeTabela}
              WHERE
                   nrccod = '{$nrccod}'";
                   
        return $this->pegaUm($sql);
    }
}