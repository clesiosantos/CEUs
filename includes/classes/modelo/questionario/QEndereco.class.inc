<?php

class QEndereco extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.endereco";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qenid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
                            		    'qenid' => null,
                                        'qencep' => null,
                                        'qenlogradouro' => null,
                                        'qennumero' => null,
                                        'qencomplemento' => null,
                                        'qenbairro' => null,
                                        'muncod' => null,
                                        'estuf' => null,
                                        'qencoordenadas' => null,
                                        'qenzoom' => null
									  );

    public function getDscMunicipio(){
        $mMunicipio = new Municipio($this->muncod);
        return $mMunicipio->mundescricao;
    }

    public function getCoordenada(){
        $sql = "SELECT st_astext('{$this->qencoordenadas}')";
        return $this->pegaUm($sql);
    }
}