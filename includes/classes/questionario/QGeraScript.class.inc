
<?php
class QGeraScript{
	private $g;
	private $q;
	private $p;
	private $s;
	private $it;
	public $arvore;
	private $tipoArvore;
	private $img;
	private $questionario;
    private $perid;
    private $qrpid;
    private $db;
    static $i;

    /*
	 * Monta a Tela de Geração de Script do Questionário
	 *
	 * include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
	 * include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/QGeraScript.class.inc";
	 * include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 *
	 * $obGera = new QGeraScript();
	 * $obGera->exportarArquivo();
	 * echo $obGera->montaArvoreQ();
	 *
	 * Desenvolvedor: Victor Benzi
	 *
	 */

	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->it = new QItemPergunta();
		$this->r  = new QResposta();

		$this->perid = $param['perid'];

		$queid = !empty($param['queid']) ? $param['queid'] : $_REQUEST['queid'];
		$this->queid = array("queid" => $queid);

		global $db;
		$this->db = $db;
	}

	public function exportarArquivo(){
		self::montaArvoreQ();
//		self::geraScript();
//		dbg($this->script, 1);
		ob_clean();

		// Headers for an download:
//		ver($this->arrDados,d);
//		header('Content-Type: application/octet-stream');
//		header('content-type: application/json;charset=iso-8859-1');
		header('Content-Disposition: attachment; filename="Script.txt"');
		header('Content-Transfer-Encoding: binary');

		echo json_encode( $this->arrDados );
		exit;
	}

	public function extrairArquivo($arquivo, $sisid){
//		phpinfo();
		$script	= file_get_contents($arquivo);
		$this->script = json_decode( $script, true );
//		dbg($this->script);
//		die;
		if( is_array( $this->script ) ){
			$exec = self::percorreArray( $this->script, $sisid );
		} else {
			$exec == 0;
		}
		if( $exec == 1 ){
			return 'Questionário gerado com sucesso!';
		} else {
			return 'Erro encontrado!';
		}
	}

	public function replicarQuestionario(Array $param = array())
	{
		$queid = !empty($param['queid']) ? $param['queid'] : $_REQUEST['queid'];
		$this->queid = array("queid" => $queid);
		self::montaArvoreQ($param);
		$queid = self::percorreArray($this->arrDados, $_SESSION['sisid']);

		$arPergRegra = $this->p->recuperarTodos("perid, perregra", array("perregra IS NOT NULL", "perregra != ''", "queid_atual = $queid"));

		foreach ($arPergRegra as $perg){
			$regra 	  = explode(";", $perg['perregra']);
			$regra[0] = $this->p->recuperarUm("perid", array("queid_atual = {$queid}", "perid_matriz = {$regra[0]}"));
			$regra[2] = $this->p->recuperarUm("perid", array("queid_atual = {$queid}", "perid_matriz = {$regra[2]}"));
			if ($regra[0] && $regra[2]){
				$pergunta = new QPergunta($perg['perid']);
				$pergunta->perregra = implode(";", $regra);
				$pergunta->salvar();
				$pergunta->commit();
			}
		}

		return $queid;
	}

	private function montaArvoreQ(Array $param = array()){
		$obPai = $this->q->pegaUmObjeto( $this->queid );

		$acoid = !empty($param['acoid']) ? $param['acoid'] : 'NULL';
		if (!empty($param['queid_matriz'])){
			$queid_matriz = $param['queid_matriz'];
		}else{
			$queid_matriz = !empty($obPai->queid_matriz) ? $obPai->queid_matriz : 'NULL';
		}
//		$queid_matriz = !empty($param['queid_matriz']) ? $param['queid_matriz'] : $obPai->queid_matriz;
	//	pg_query( $db->link, 'begin transaction;');
		$arrDados = array(
			"SQL"   => "INSERT INTO questionario.questionario ( sisid, quetitulo, quedescricao, quedtinicio, quedtfim, acoid, queid_matriz ) VALUES ('$obPai->sisid', '".utf8_encode($obPai->quetitulo)."', '".utf8_encode($obPai->quedescricao)."', '$obPai->quedtinicio', '$obPai->quedtfim', $acoid, $queid_matriz) RETURNING queid",
			"tipo"  => "questionario",
			"var"   => "queid",
			"itens" => array());

			// Busca Perguntas associadas
			$arrP = self::montaArvoreP($obPai);
			if( is_array( $arrP )){
				array_push( $arrDados['itens'], $arrP );
			}

			// Busca Grupo Pergunta associadas
			$arrG = self::montaArvoreG($obPai);
			if( is_array( $arrG )){
				array_push( $arrDados['itens'], $arrG );
			}

		$this->arrDados = $arrDados;

	//	ver($arrDados);
	//	self::percorreArray( $arrDados );
//		self::exportarArquivo();
	}

	private function montaArvoreG( $obPai, $qrpid = null ){

		// Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("*"));
		$i = 0;
		$gru = '';
		if( get_class((object) $obPai) == 'QGrupo' ){
			$gru = 'gru_';
		}

		foreach ($arrObGrupoPergunta as $obGrupoPergunta){

            $obGrupoPergunta->grpqtdmaximo = $obGrupoPergunta->grpqtdmaximo ? $obGrupoPergunta->grpqtdmaximo : 'null';
            $qrpid = $qrpid ? $qrpid : 'null';

			$grupos[] = array(
				"SQL"   => "INSERT INTO questionario.grupopergunta( ".$gru."varPai, grptitulo, grpstatus, grpordem, grpajuda, grpid_matriz, queid_atual,grpqtdmaximo,grppermitemultiplo, qrpid,grpcondicao) VALUES ( resultPai, '".utf8_encode($obGrupoPergunta->grptitulo)."', '$obGrupoPergunta->grpstatus', $obGrupoPergunta->grpordem, '".utf8_encode($obGrupoPergunta->grpajuda)."', '$obGrupoPergunta->grpid', queidAtual , {$obGrupoPergunta->grpqtdmaximo} , '$obGrupoPergunta->grppermitemultiplo', {$qrpid}, '{$obGrupoPergunta->grpcondicao}' ) RETURNING grpid",
				"tipo"  => "grupo",
				"var"   => "grpid",
				"itens" => array());

			// Busca Perguntas associadas
			$arrG = self::montaArvoreP($obGrupoPergunta, $qrpid);
			if( is_array( $arrG )){
				array_push( $grupos[$i]['itens'], $arrG );
			}

			// Busca Grupo Pergunta associadas
			$arrP = self::montaArvoreG($obGrupoPergunta, $qrpid);
			if( is_array( $arrP )){
				array_push( $grupos[$i]['itens'], $arrP );
			}
			$i++;
		}
		return $grupos;
	}

	private function montaArvoreP( $obPai, $qrpid = null ){
		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array( '*' ));
		$i = 0;
		$queid = $this->queid;
        $qrpid = $qrpid ? $qrpid : 'null';

		foreach ($arrObPergunta as $obPergunta){
			$perguntas[] = array(
				"SQL"      => "INSERT INTO questionario.pergunta( varPai, perordem, pertitulo, perajuda, pertipo, permascara, perobrigatorio, permultipla, pertipografico, pertabela, perregra, perid_matriz, queid_atual, qrpid, percondicao,perexibemapa,pervalidacaoonblur) VALUES ( resultPai, $obPergunta->perordem, '".utf8_encode($obPergunta->pertitulo)."', '".utf8_encode($obPergunta->perajuda)."', '$obPergunta->pertipo', '$obPergunta->permascara', '$obPergunta->perobrigatorio', '$obPergunta->permultipla', '$obPergunta->pertipografico', '$obPergunta->pertabela', '$obPergunta->perregra', '$obPergunta->perid', queidAtual, {$qrpid}, '{$obPergunta->percondicao}', '{$obPergunta->perexibemapa}' , '{$obPergunta->pervalidacaoonblur}') RETURNING perid",
				"tipo"     => "pergunta",
				"var"      => "perid",
				"matriz"   => $obPergunta->perid,
				"itens"    => array());

			// Busca Itens associados
			$arrI = self::montaArvoreI( $obPergunta, $qrpid );
			if( is_array( $arrI )){
				array_push( $perguntas[$i]['itens'], $arrI );
			}
			$i++;
		}
		return $perguntas;
	}

	private function montaArvoreI( $obPai, $qrpid = null ){
		$where[0] = $obPai;

		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo, itpordem"));
		$i = 0;
		$queid = $this->queid;
		foreach ($arrObItPergunta as $obItPergunta){
			$itens[] = array(
				"SQL" => "INSERT INTO questionario.itempergunta(perid, itptitulo, itpid_matriz, itpordem, queid_atual ) VALUES ( resultPai, '".utf8_encode($obItPergunta->itptitulo)."','$obItPergunta->itpid','$obItPergunta->itpordem', queidAtual) RETURNING itpid",
				"tipo" => "item",
				"var"  => "itpid",
				"itens" => array());

			// Busca Perguntas associadas
			$arrP = self::montaArvoreP($obItPergunta, $qrpid);
			if( is_array( $arrP )){
				array_push( $itens[$i]['itens'], $arrP );
			}

			// Busca Grupo Pergunta associadas
			$arrG = self::montaArvoreG($obItPergunta, $qrpid);
			if( is_array( $arrG )){
				array_push( $itens[$i]['itens'], $arrG );
			}
			$i++;
		}
		return $itens;
	}

	private function percorreArray( $arrDados, $sisid = null, $pai = null, $resultPai = null ){
		if( isset($arrDados['SQL']) && $arrDados['SQL'] && strlen( $arrDados['SQL'] ) > 5 ){
			$sql = str_replace('resultSisid', $sisid, utf8_decode($arrDados['SQL']));
			$sql = str_replace('varPai', $pai, $sql);
			$sql = str_replace('queidAtual', $this->queidNovo, $sql);
            $sql = str_replace('resultPai', $resultPai, $sql);

            $resultPai = $this->db->pegaUm( $sql );
			$this->queidNovo = is_numeric($this->queidNovo) ? $this->queidNovo : $resultPai;
		}
		if( isset($arrDados['itens']) &&  $arrDados['itens']){
			$pai = isset($arrDados['var']) ? $arrDados['var'] : "";
			if( is_array( $arrDados['itens'] ) ){
				foreach( $arrDados['itens'] as $arr ){
					self::percorreArray( $arr, $sisid, $pai, $resultPai );
				}
			}
		} else {
			if( is_array($arrDados) && !empty($arrDados)){
				foreach( $arrDados as $arr ){
					self::percorreArray( $arr, $sisid, $pai, $resultPai );
				}
			}
		}

		return $this->queidNovo;
	}

	public function percorreArrayGrupo( $arrDados, $sisid = null, $pai = null, $resultPai = null, $queid, $qrpidAtual = null, $qrpidAnterior = null ){
		if( isset($arrDados['SQL']) && $arrDados['SQL'] && strlen( $arrDados['SQL'] ) > 5 ){
			$sql = str_replace('resultSisid', $sisid, utf8_decode($arrDados['SQL']));
			$sql = str_replace('varPai', $pai, $sql);
			$sql = str_replace('queidAtual', $queid, $sql);
            $sql = str_replace('resultPai', $resultPai, $sql);
            $resultPai = $this->db->pegaUm( $sql );
			$this->grpidNovo = is_numeric($this->grpidNovo) ? $this->grpidNovo : $resultPai;

			if($arrDados["tipo"] == "pergunta" && $qrpidAtual && $qrpidAnterior && $arrDados["matriz"]){
			    $this->copiarRespostas = $this->copiarRespostas && is_array($this->copiarRespostas) ? $this->copiarRespostas : array();
			    $this->copiarRespostas[] = array('perid_novo' => $resultPai, 'perid_original' => $arrDados["matriz"]);
			    // $mQResposta = new QResposta();
			    // $dadosResposta = $mQResposta->recuperarLinha("*", array("qrpid = {$qrpidAnterior}", "perid = {$arrDados["matriz"]}"), false);
			    // if($dadosResposta){
                //     $dadosResposta["perid"] = $resultPai;
    		 	//    $mQResposta->popularDadosObjeto($dadosResposta);
    		 	//    $mQResposta->copiarResposta($qrpidAtual);
			    // }
			}
		}

		if( isset($arrDados['itens']) && $arrDados['itens'] ){
			$pai = $arrDados['var'];
			if( is_array( $arrDados['itens'] ) ){
				foreach( $arrDados['itens'] as $arr ){
					self::percorreArrayGrupo( $arr, $sisid, $pai, $resultPai,$queid, $qrpidAtual, $qrpidAnterior );
				}
			}
		} elseif(is_array($arrDados)) {
			foreach( $arrDados as $arr ){
				self::percorreArrayGrupo( $arr, $sisid, $pai, $resultPai,$queid, $qrpidAtual, $qrpidAnterior);
			}
		}

		return $this->grpidNovo;
	}

    public function montaArvoreGrupo( $obj, $qrpid ){
        return self::montaArvoreG( $obj, $qrpid );
    }

    public function montaArvorePergunta( $obj, $qrpid ){
        return self::montaArvoreP( $obj,$qrpid );
    }

}
