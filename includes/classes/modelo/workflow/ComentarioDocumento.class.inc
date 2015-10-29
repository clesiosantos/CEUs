<?php
class ComentarioDocumento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "workflow.comentariodocumento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "cmdid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
                                        'cmdid' => null,
                                        'docid' => null,
                                        'cmddsc' => null,
                                        'cmdstatus' => null,
                                        'cmddata' => null,
                                        'hstid' => null
									  );
}