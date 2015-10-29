<?php

class FotoDadosIniciais extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.fotodadosiniciais";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("fdiid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'fdiid'     => null,
        'divid'     => null,
        'arqid'     => null,
        'fdistatus' => null,
    );

    public function desabilitaPorDadosIniciais($divid, $arrArqIds = null)
    {
    	$whereArqid = '';
    	if(is_array($arrArqIds)){
    		$whereArqid = " AND arqid NOT IN (".implode(',', $arrArqIds).")";
    	}
        $sql = "UPDATE {$this->stNomeTabela} SET fdistatus = 'I' WHERE divid = {$divid} " . $whereArqid;
        $this->executar($sql);
    }

    public function getFotosDadosIniciaisByWhere($where)
    {

        $where[] = " fdi.fdistatus = 'A' ";

        $sql = " SELECT
    				fdi.fdiid,
    				fdi.divid,
    				fdi.arqid
		    	FROM
		    		{$this->stNomeTabela} fdi
		    	WHERE
    				" . implode(" AND ", $where) . "";


        $retorno = $this->carregar($sql);
        return $retorno ? $retorno : array();
    }

}