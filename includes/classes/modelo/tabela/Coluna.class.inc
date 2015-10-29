<?php
	
class Coluna extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tabela.coluna";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "colid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'colid' => null, 
									  	'itmid' => null, 
									  	'grcid' => null, 
									  	'coltipo' => null, 
									  	'colnumeric' => null, 
									  );
	public function pegaDados( $tabela ){
		$sql = "SELECT
				c.colid as codigo,
				i.itmdsc as descricao,
				c.coltipo,
				c.colnumeric
			FROM
				{$this->stNomeTabela} c
			INNER JOIN tabela.item i ON c.itmid = i.itmid
			INNER JOIN tabela.grupocoluna gc USING (grcid)
			INNER JOIN tabela.tabela t USING (tblid)
			WHERE
				tblid = ".$tabela."
			ORDER BY
				grcid, colid";
		return $this->carregar( $sql );
	}
	
	public function conta( $arrGrcid ){
		
		$arrGrcid = (array) $arrGrcid;
		
		$sql = "SELECT
					count(colid)
				FROM
					{$this->stNomeTabela}
				WHERE
					grcid IN (".implode(",", $arrGrcid).")";

					
		return $this->pegaUm( $sql );
	}
	
	public function listarPorGrupo($grcid)
	{
		$sql = "select 
					* 
				from 
					tabela.coluna c
				join tabela.item i on i.itmid = c.itmid	 
				where 
					grcid=".$grcid." order by colid ";
		
		return $this->carregar( $sql );
	}	
	
	public function apagarPorGrupo($grcid)
	{
		$sql = "select 
					itmid
			   	from
			   		tabela.coluna
			   	where
			   		grcid={$grcid}";
		$coluna = $this->carregarColuna($sql);
		
		$sql = "DELETE FROM
					tabela.coluna
				WHERE
					grcid={$grcid}";
		
		$this->executar($sql);
		
		if ($coluna){
			$sql = "DELETE FROM
						tabela.item
					WHERE
						itmid IN (".(implode(", ", $coluna)).")";
			
			$this->executar($sql);
		}
	}		
}