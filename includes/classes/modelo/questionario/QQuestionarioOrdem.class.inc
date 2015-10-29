<?php

class QQuestionarioOrdem extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionarioordem";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qorid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'qorid' => null,
									  	'queid' => null,
									  	'grpid' => null,
									  	'perid' => null,
									  	'itpid' => null,
									  	'qorordem' => null
									  );

    /**
     * Ordena os filhos de um determinado objeto
     * @param object $objPai
     */
    public function ordenarQuestionario($objPai){
        $mascaraOrdem = "0000";
        $qtdCharOrdem = strlen($mascaraOrdem);

        switch (get_class((object) $objPai)) {
        	case 'QQuestionario':
                $filhos = $objPai->getFilhosOrdemDefault();
        	break;
        	case 'QGrupo':
                $filhos = $objPai->getFilhosOrdemDefault();
        	break;
        	case 'QPergunta':
                $filhos = $objPai->getFilhosOrdemDefault();
        	break;
        	case 'QItemPergunta':
                $filhos = $objPai->getFilhosOrdemDefault();
        	break;
        	default:
        	   throw new Exception("Objeto inválido ao tentar ordenar");
        	break;
        }

        if(count($filhos)){
            // Pega dados de ordem do pai
            $objOrdemPai = $this->getOrdemByObj($objPai);

            foreach($filhos as $ordem => $filho){
                $ordemFilho = $ordem + 1;

                $objOrdem           = $this->getOrdemByObj($filho);
                $objOrdem->qorordem = $objOrdemPai->qorordem . substr("{$mascaraOrdem}{$ordemFilho}", ($qtdCharOrdem*(-1)), $qtdCharOrdem);
                $objOrdem->queid    = $filho->queid_atual;

                switch (get_class((object) $filho)) {
                	case 'QGrupo':
                	    $objOrdem->grpid = $filho->grpid;
                	    break;
                	case 'QPergunta':
                	    $objOrdem->perid = $filho->perid;
                	    break;
                	case 'QItemPergunta':
                	    $objOrdem->itpid = $filho->itpid;
                	    break;
                	default:
                	    throw new Exception("Objeto inválido ao criar ordem");
                	    break;
                }

                $objOrdem->salvar();

                // Ordena os filhos
                $this->ordenarQuestionario($filho);
            }
        }
    }

    /**
     * Recupera os dados de ordem de um objeto qualquer
     * @param object $obj
     * @return QQuestionarioOrdem
     */
    public function getOrdemByObj($obj){
        $where = array();

        // Se for um questionário, vai mandar um objeto sem informações
        if(get_class((object) $obj) == 'QQuestionario'){
            $qorid = null;
        }else{
            switch (get_class((object) $obj)) {
            	case 'QGrupo':
            	    $where = "grpid = {$obj->grpid}";
            	    break;
            	case 'QPergunta':
            	    $where = "perid = {$obj->perid}";
            	    break;
            	case 'QItemPergunta':
            	    $where = "itpid = {$obj->itpid}";
            	    break;
        	    default:
    	           throw new Exception("Objeto inválido ao buscar a ordenação");
    	           break;
            }

            $sql   = "SELECT qorid FROM {$this->stNomeTabela} WHERE {$where};";
            $qorid = $this->pegaUm($sql);
        }

        return new QQuestionarioOrdem($qorid);
    }

    public function deletaItensPeloPai( $obj, $commit = true ){

        switch (get_class((object) $obj)) {
            case 'QGrupo':
                $where = " grpid = '{$obj->grpid}' AND queid = '{$obj->queid_atual}' ";
                break;
            case 'QPergunta':
                $where = " perid = '{$obj->perid}' AND queid = '{$obj->queid_atual}' ";
                break;
            case 'QItemPergunta':
                $where = " itpid = '{$obj->itpid}' AND queid = '{$obj->queid_atual}' ";
                break;
            default:
               throw new Exception("Objeto inválido ao excluir a ordenação");
               break;
        }

        $sql      = "SELECT qorordem FROM {$this->stNomeTabela} WHERE {$where};";
        $qorordem = $this->pegaUm($sql);

        $sql   = "DELETE FROM {$this->stNomeTabela} WHERE qorordem ILIKE '$qorordem%' AND queid = '{$obj->queid_atual}';";
        $this->executar($sql);

        if( $commit ){
            $this->commit();
        }
     }

    public function getGruposFilhosByOrdem( $ordem,$queid ){
        $sql      = "SELECT grpid FROM {$this->stNomeTabela} WHERE grpid IS NOT NULL AND qorordem ILIKE '{$ordem}%' AND queid = '{$queid}';";
        $retorno  = $this->carregarColuna($sql);
        return $retorno ? $retorno : array();
     }

    public function getItensFilhosByOrdem( $ordem,$queid ){
        $sql      = "SELECT itpid FROM {$this->stNomeTabela} WHERE itpid IS NOT NULL AND qorordem ILIKE '{$ordem}%' AND queid = '{$queid}';";
        $retorno  = $this->carregarColuna($sql);
        return $retorno ? $retorno : array();
     }

    public function getPerguntasFilhasByOrdem( $ordem,$queid ){
        $sql      = "SELECT perid FROM {$this->stNomeTabela} WHERE perid IS NOT NULL AND qorordem ILIKE '{$ordem}%' AND queid = '{$queid}';";
        $retorno  = $this->carregarColuna($sql);
        return $retorno ? $retorno : array();
     }


}