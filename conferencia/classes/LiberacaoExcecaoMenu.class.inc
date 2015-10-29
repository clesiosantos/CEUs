<?php

class LiberacaoExcecaoMenu extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.liberacaoexcecaomenu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("lemid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'lemid'     => null,
                                    'ceuid'     => null,
                                    'cleid'     => null,
                                    'lemstatus' => null
                                  );

    public function desabilitaPorConfiguracaoLiberacaoExcecao($cleid){
    	$sql = "UPDATE {$this->stNomeTabela} SET lemstatus = 'I' WHERE cleid = {$cleid} ";
    	$this->executar($sql);
    }

    public function getIdByFks($cleid, $ceuid){
    	$sql = "SELECT
    				lemid
    			FROM {$this->stNomeTabela}
    			WHERE
    					cleid = {$cleid}
    				AND
    					ceuid = {$ceuid}";

    	return $this->pegaUm($sql);
    }

    public function getCeusByConfiguracaoLiberacaoExcecao($cleid){
    	$sql = "SELECT
    	           ceu.ceuid as codigo,
    	           ceu.ceucodigo || ' - ' || ceu.ceunome || ' (' || mun.mundescricao || '/' || mun.estuf || ')' AS descricao
    			FROM
    			   {$this->stNomeTabela} lem
    			INNER JOIN
                   conferencia.ceu ceu ON lem.ceuid = ceu.ceuid
                LEFT JOIN
                	conferencia.endereco edc ON edc.edcid = ceu.edcid
				LEFT JOIN
					territorios.municipio mun ON edc.muncod = mun.muncod
    			WHERE
    				lemstatus = 'A'
    				AND
    				cleid = {$cleid}";

    	return $this->carregar($sql);
    }



}



