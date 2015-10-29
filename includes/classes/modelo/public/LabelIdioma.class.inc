<?php
	
class LabelIdioma extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "public.labelidioma";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "lbiid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */ 
    protected $arAtributos     = array(
									  	'lbiid' 	=> null, 
    									'tlbid' 	=> null, // Referencia tipolabel
    									'idiid' 	=> null, // Referencia idioma
    									'lbidsc' 	=> null, 
									  );

	public function recuperaTraducao($text, $idIdioma)
	{
		$sql = "SELECT
					lbidsc
				FROM
					public.labelidioma l
				JOIN public.tipolabel t ON t.tlbid = l.tlbid
										   AND tlbdsc = '{$text}'
				WHERE
					l.idiid = {$idIdioma}";
					
		
		return $this->pegaUm($sql);
	}
										  	
	public function recuperaTodaTraducao($idIdioma)
	{
		$sql = "SELECT
					t.tlbdsc AS original,
					l.lbidsc AS traducao
				FROM
					public.tipolabel t 
				LEFT JOIN 
					public.labelidioma l ON t.tlbid = l.tlbid AND l.idiid = {$idIdioma}";
		$dados = $this->carregar($sql);
		return $dados ? $dados : array();
	}									  	
}