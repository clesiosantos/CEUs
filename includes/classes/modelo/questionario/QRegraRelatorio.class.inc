<?php
	
class QRegraRelatorio extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.regrarelatorio";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "rerid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'rerid' 		 => null, 
    									'queid' 		 => null, 
									  	'rernome' 		 => null, 
    									'rerdescricao'	 => null, 
									  	'rerformula' 	 => null, 
									  	'rerstatus' 	 => null, 
									  	'rertipografico' => null, 
									  	'rertipotabela'  => null 
									  );
	  
									  
	 public function pegaRegraRelatorioComAcordo(array $where){
	 	$where = implode(' AND ', $where);
	 	$sql = "
	 			SELECT 
	 				rer.rernome,
	 				rer.rerdescricao,
	 				rer.rerformula,
	 				COALESCE(aco.aconome, 'Questionário Matriz') || ' - ' || que.quetitulo AS aconome,
	 				rer.rerid,
	 				rer.queid,
	 				rer.rerstatus
	 			FROM 
	 				questionario.regrarelatorio rer
	 			INNER JOIN 
	 				questionario.questionario que ON que.queid = rer.queid
	 												 AND que.questatus = 'A'
	 			LEFT JOIN
					questionario.acordo aco ON aco.acoid = que.acoid 
											   AND aco.acostatus = 'A'
				WHERE 
					{$where}
				ORDER BY 
					rer.rernome";

		return $this->carregar($sql);
	 }
	 
	 public function excluir( $id = null, $retorno = null ){
			$complemento = ";";
			if( count( $this->arChavePrimaria ) > 1 ) trigger_error( "Favor sobreescrever método na classe filha!" );
			
			if($retorno){
				$complemento = "returning $retorno;";
			}
			
			if( !$this->antesExcluir($id) ) return false;
			
			$stChavePrimaria = $this->arChavePrimaria[0];
			$id = $id ? $id : $this->$stChavePrimaria;
			
			$sql = " UPDATE $this->stNomeTabela SET rerstatus = 'I' WHERE $stChavePrimaria = $id $complemento";
			if($retorno){
				return $this->pegaUm( $sql );
			}else{
				return $this->executar( $sql );
			}
	}
	
	public function buscarPorAcordo( $acoid, $regra=false, $regraAgrupado=false)
	{
		$campo1 = $regra ? 'rer.rerformula' : 'rer.rerid';
		$where  = $regraAgrupado ? 'AND (rer.rertipografico IS NOT NULL OR rer.rertipotabela IS NOT NULL)' : '';
		$sql = "  SELECT 
				        {$campo1} AS codigo,
				        rer.rernome AS descricao
				  FROM 
					questionario.regrarelatorio rer
				  JOIN 
				  	questionario.questionario que ON que.queid = rer.queid
				  									AND que.questatus = 'A'
				  WHERE
					que.acoid = '{$acoid}' AND 
					rer.rerstatus = 'A'
					{$where}
				  ORDER BY
				    2
			   ";
	  	return $this->carregar($sql);
	}
	 
}