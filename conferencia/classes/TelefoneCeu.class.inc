<?php

class TelefoneCeu extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.telefoneceu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tceid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                    'tceid' => null,
                                    'tceddd' => null,
                                    'tcetelefone' => null,
                                    'tcestatus' => null,
                                    'tcedtcadastro' => null,
                                    'usucpf' => null,
                                    'ceuid' => null
                                  );

    public function desabilitarPorCeu($ceuid){
        $sql = "UPDATE {$this->stNomeTabela} SET tcestatus = 'I' WHERE ceuid = {$ceuid}";
        $this->executar($sql);
    }

}