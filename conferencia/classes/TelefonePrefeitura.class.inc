<?php

class TelefonePrefeitura extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.telefoneprefeitura";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tprid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'tprid' => null,
                                    'tprddd' => null,
                                    'tprtelefone' => null,
                                    'tprstatus' => null,
                                    'tprdtcadastro' => null,
                                    'usucpf' => null,
                                    'divid' => null
                                  );

    public function desabilitarPorDadosIniciais($divid){
        $sql = "UPDATE {$this->stNomeTabela} SET tprstatus = 'I' WHERE divid = {$divid}";
        $this->executar($sql);
    }

}