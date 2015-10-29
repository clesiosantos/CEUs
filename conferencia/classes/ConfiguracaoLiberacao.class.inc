<?php

class ConfiguracaoLiberacao extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.configuracaoliberacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("colid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                        'colid'          	=> null,
                                        'mnuid'      	 	=> null,
                                        'colliberado'    	=> null,
                                        'usucpf'   		 	=> null,
                                        'coldtcadastro'  	=> null,
                                        'colstatus'  		=> null
                                  );


    public function listar( $arrWhere , $paginacao = false){

    	$arrWhere[] = " col.colstatus = 'A' ";

		$sql=" SELECT
					mun.mnudsc,
					CASE
    					WHEN col.colliberado IS TRUE THEN
    				'Sim'
    				ELSE
    					'Não'
    				END AS colliberado,
					TO_CHAR(col.coldtcadastro, 'DD/MM/YYYY as HH:MI') AS coldtcadastro,
					col.colid,
					col.colstatus
				FROM
					{$this->stNomeTabela} col

			    INNER JOIN
			        seguranca.menu mun ON mun.mnuid = col.mnuid
			    WHERE ".
					implode(" AND ", $arrWhere);

		if($paginacao){
			$dado = Paginacao::getPaginacao($this, $sql,10);
		    return $dado;
		}else{
			$retorno = $this->carregar($sql);
			return $retorno ? $retorno : array();
		}
    }



    public function getMenusCeu(){

    	$sql="SELECT DISTINCT
    			  mnu.mnuid AS codigo,
    			  mnu.mnudsc AS descricao
    		  FROM
    			  seguranca.menu mnu
    		  JOIN
    			  seguranca.aba_menu abamenu ON abamenu.mnuid = mnu.mnuid AND
    			                                abamenu.abacod = '".ABA_VINCULAR_CEU."'
    		  WHERE
    			  mnu.mnustatus = 'A'
    			  AND
    			  mnu.mnuid NOT IN (".MENU_CEU_LISTAR.",".MENU_CEU_SOLICITACAO.")
    	      ORDER BY
    			  2;";

    	$retorno = $this->carregar($sql);
    	return $retorno ? $retorno : array();
    }





    public function verificarAbaLiberada($abacod, $url, $ceuid){
        $orExcecao = "";
        if($ceuid){
            $orExcecao = "OR mun.mnuid IN (SELECT DISTINCT
	                                          cle.mnuid
    	                                   FROM
    	                                      conferencia.configuracaoliberacaoexcecao cle
    	                                   INNER JOIN
    	                                      conferencia.liberacaoexcecaomenu lem ON lem.cleid = cle.cleid
                                           INNER JOIN
		                                      seguranca.aba_menu abm ON cle.mnuid = mun.mnuid
    	                                   WHERE
    	                                      cle.clestatus = 'A'
                                              AND
    	                                      lem.lemstatus = 'A'
                                              AND
                                              cle.cledatalimite >= CURRENT_DATE
                                              AND
                                              lem.ceuid = {$ceuid})";
        }

        $sql=" SELECT
					mun.mnudsc AS descricao,
					mun.mnulink AS link
				FROM
			        seguranca.menu mun
		        INNER JOIN
		           seguranca.aba_menu abm ON abm.mnuid = mun.mnuid
			    LEFT JOIN
					{$this->stNomeTabela} col ON mun.mnuid = col.mnuid
					                         AND col.colstatus = 'A'
		        WHERE
			       abm.abacod = {$abacod}
				   AND
				   (col.colliberado IS TRUE
		            {$orExcecao})
		            AND
				    mun.mnulink = '{$url}'
			    ORDER BY
			       mun.mnucod ASC";

        $abas = $this->carregar($sql);
		return is_array($abas) && count($abas) > 0 ? 'S' : 'N';
    }

    public function getIdByMenu($mnuid){
    	$sql = "SELECT
    				colid
    			FROM
    				{$this->stNomeTabela}
    		    WHERE
    		    	mnuid = {$mnuid}";
    	return $this->pegaUm($sql);
    }

}



