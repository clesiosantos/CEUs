<?php
	
class Linha extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tabela.linha";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "linid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'linid' => null, 
									  	'itmid' => null, 
									  	'grlid' => null, 
									  	'lindsc' => null, 
									  );
									  
	public function pegaDados( $tabela ){
		$sql = "SELECT
					l.linid as codigo,
					i.itmdsc as descricao,
					t.tbldescricao as tabela,
					l.grlid as grupoid,
					gl.grldsc as grupolinha,
					gl.gru_grlid as grupopai,
					l.lindsc			
				FROM
					{$this->stNomeTabela} l
				INNER JOIN tabela.item i ON l.itmid = i.itmid
				INNER JOIN tabela.grupolinha gl USING (grlid)
				INNER JOIN tabela.tabela t USING (tblid)
				WHERE
					tblid = {$tabela}
				ORDER BY grupoid, codigo";
		return $this->carregar( $sql );
	}
	
	public function conta( $grupoid ){
		$sql = "SELECT
					count(l.linid)
				FROM
					{$this->stNomeTabela} l
				WHERE
					grlid = ".$grupoid;
		return $this->pegaUm( $sql );
	}
	
	public function listarPorGrupo($grlid)
	{
		$sql = "select 
					*
				from 
					tabela.linha l
				join tabela.item i on i.itmid = l.itmid	 
				where 
					grlid=".$grlid." order by linid";
		
		return $this->carregar( $sql );
	}
	
	public function apagarPorGrupo($grlid)
	{
		$sql = "select 
					itmid
			   	from
			   		tabela.linha
			   	where
			   		grlid={$grlid}";
		$linha = $this->carregarColuna($sql);
		
		$sql = "DELETE FROM
					tabela.linha
				WHERE
					grlid={$grlid}";
		
		$this->executar($sql);
		
		if ($linha){
			$sql = "DELETE FROM
						tabela.item
					WHERE
						itmid IN ( ".(implode(", ", $linha))." )";
			
			$this->executar($sql);
		}
	}	
	
	
}