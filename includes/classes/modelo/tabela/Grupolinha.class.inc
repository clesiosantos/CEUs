<?php
	
class Grupolinha extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tabela.grupolinha";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "grlid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'grlid' => null, 
									  	'tblid' => null, 
									  	'gru_grlid' => null, 
									  	'grldsc' => null, 
									  );
									  
	public function conta( $tabela, $where = null ){
		if( $where ){
			$cond = " AND grldsc <> '$nbsp;'";
		}
		$sql = "SELECT
					count(grlid)
				FROM
					{$this->stNomeTabela}
				WHERE
					tblid = ".$tabela."
					".$cond;
		return $this->pegaUm( $sql );
	}
	
	public function listarPorTabela($tblid)
	{
		$sql = "select * from tabela.grupolinha where tblid=".$tblid." order by grlid;";
		
		return $this->carregar( $sql );
	}
	
	public function apagarPorTabela($tblid)
	{
		$sql = "DELETE FROM
					tabela.grupolinha
				WHERE
					tblid={$tblid}";
		
		$this->executar($sql);
	}
}