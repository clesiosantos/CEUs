<?php

class TelaInicial extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.telainicial";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("tinid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                      'tinid'         => null,
                                      'tinhtml'       => null,
                                      'tinstatus'     => null,
                                      'tindtcadastro' => null,
                                      'usucpf'        => null
                                  );

    // Busca a primeira tela inicial
    public function getTelaInicial(){
        $sql   = "SELECT * FROM {$this->stNomeTabela}";
        $dados = $this->pegaLinha($sql);
        $dados = $dados ? $dados : array();

        $this->popularDadosObjeto($dados);
        return $dados;
    }

}