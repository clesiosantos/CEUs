<?php

class EnderecoCeu extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.endereco";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("edcid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                'edcid'          => null,
                                'edccep'         => null,
                                'edclogradouro'  => null,
                                'edcnumero'      => null,
                                'edccomplemento' => null,
                                'edcbairro'      => null,
                                'muncod'         => null,
                                'estuf'          => null,
                                'edccoordenadas' => null,
                                'edczoom'        => null,
                                'edcstatus'      => null
                            );

    public function getDados(){
        if($this->edcid){
            $sql = "SELECT
                        edc.edcid,
                        edc.edccep,
                        edc.edclogradouro,
                        edc.edcnumero,
                        edc.edccomplemento,
                        edc.edcbairro,
                        edc.muncod,
                        edc.estuf,
                        st_astext(edc.edccoordenadas) AS edccoordenadas,
                        edc.edczoom,
                        mun.mundescricao
                    FROM
                        {$this->stNomeTabela} as edc
                    INNER JOIN
                        territorios.municipio mun ON mun.muncod = edc.muncod
                    WHERE
                        edcid = {$this->edcid}
                        AND
                        edcstatus = 'A'";
            return $this->pegaLinha($sql);
        }else{
            return $this->arAtributos;
        }
    }

}

