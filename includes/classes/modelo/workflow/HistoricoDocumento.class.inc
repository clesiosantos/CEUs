<?php
class HistoricoDocumento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "workflow.historicodocumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "hstid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
                                        'hstid' => null,
                                        'aedid' => null,
                                        'docid' => null,
                                        'usucpf' => null,
                                        'pflcod' => null,
                                        'htddata' => null
									  );
}