<?php

class QQuestionarioResposta extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.questionarioresposta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "qrpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'qrpid' 	          => null,
									  	'queid' 	          => null,
									  	'qrptitulo'           => null,
									  	'qrpdata' 	          => null,
									  	'qrpfinalizado'       => null,
                                        'qrpfinalizadousucpf' => null,
                                        'qrpfinalizadodata'   => null,
                                        'qrppreenchimento'    => null

									  );

	public function carregaUm(Array $where = null, $coluna = 'qrpid'){
		$coluna = $coluna == '*' ? array_keys($this->arAtributos) : $coluna;
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		$inner  = array();

		foreach ($where as $k => $item){
			if ($k == "qrpid"){
				$codicao[] = "qr.qrpid = " . $item;
			}elseif ($k == "queid"){
				$codicao[] = "qr.queid = " . $item;
			}elseif ($k == "escid"){
				$codicao[] = "qr.escid = " . $item;
			}elseif ($k == "proid"){
				$codicao[] = "qr.proid = " . $item;
			}elseif ($k == "acoid"){
				$inner[] = "JOIN questionario.questionario q ON q.queid = qr.queid
								  							    AND q.acoid = {$item}
															    AND q.questatus = 'A'";
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela} qr
				".(implode("", $inner))."
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::pegaUm($sql);
	}

	public function pegaQuestionario( $qrpid ){

		$sql = "SELECT
					queid
				FROM
					{$this->stNomeTabela}
				WHERE
					qrpid = ".$qrpid;

		return parent::pegaUm($sql);
	}

	public function submeterQuestionario($qrpid = null){
	    $qrpid = $qrpid ? $qrpid : $this->qrpid;

	    $this->carregarPorId($qrpid);

	    $this->popularDadosObjeto(array('qrpfinalizado'       => 'true',
	                                    'qrpfinalizadousucpf' => $_SESSION['usucpf'],
	                                    'qrpfinalizadodata'   => 'now()'));
	    $this->salvar();

	    // Salva as respostas do survey no Painel
	    executarExtracaoPorQuestionarioResposta($qrpid);
	}
}