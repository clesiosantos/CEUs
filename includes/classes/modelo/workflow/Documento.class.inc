<?php
class Documento extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "workflow.documento";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "docid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
                                          'docid'           => null,
                                          'tpdid'           => null,
                                          'esdid'           => null,
                                          'docdsc'          => null,
                                          'unicod'          => null,
                                          'docdatainclusao' => null
									  );

    public function duplicarHistoricoWorkflow($docidOrigem, $docidDestino){
        $mComentarioDocumento = new ComentarioDocumento();
        $mHistoricoDocumento  = new HistoricoDocumento();
        $arHistorico = $mHistoricoDocumento->recuperarTodos("*", array("docid = {$docidOrigem}"));

        foreach($arHistorico as $historico){
            $arDadosHistorico = array(
                                         'aedid'   => $historico['aedid'],
                                         'docid'   => $docidDestino,
                                         'usucpf'  => $historico['usucpf'],
                                         'pflcod'  => $historico['pflcod'],
                                         'htddata' => $historico['htddata']
                                     );
            $hstid = $mHistoricoDocumento->popularDadosObjeto($arDadosHistorico)->salvar();

            $arDadosComentario = $mComentarioDocumento->recuperarLinha('*', array("hstid = {$historico['hstid']}"), false);
            if($arDadosComentario){
                $arDadosComentario = array(
                                              'docid'     => $docidDestino,
                                              'cmddsc'    => $arDadosComentario['cmddsc'],
                                              'cmdstatus' => $arDadosComentario['cmdstatus'],
                                              'cmddata'   => $arDadosComentario['cmddata'],
                                              'hstid'     => $hstid
                                          );
                $mComentarioDocumento->popularDadosObjeto($arDadosComentario)->salvar();
            }

            $mHistoricoDocumento->setDadosNull();
            $mComentarioDocumento->setDadosNull();
        }
    }

}