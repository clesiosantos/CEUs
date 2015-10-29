<?php
class Hierarquia
{
	const QUESTIONARIO = "QQuestionario";
	const GRUPO 	   = "QGrupo";
	const PERGUNTA 	   = "QPergunta";
	const ITEMPERGUNTA = "QItemPergunta";
	
	private $map;
	private $hierarquia;
	
	function __construct($id, $origemMapeamento)
	{
		$ob  = new $origemMapeamento($id);
		$this->map = array(self::QUESTIONARIO => array("descricao" => "quetitulo", "pk" => "queid"),
						   self::GRUPO		  => array("descricao" => "grptitulo", "pk" => "grpid"),
						   self::PERGUNTA	  => array("descricao" => "pertitulo", "pk" => "perid"),
						   self::ITEMPERGUNTA => array("descricao" => "itptitulo", "pk" => "itpid")); 

		$this->buscaHierarquiaObjeto($ob);
		$this->hierarquia = array_reverse($this->hierarquia);
	}
	
	private function buscaHierarquiaObjeto(Modelo $ob )
	{
		$class = get_class((object) $ob);
		$map = $this->map;
		$this->hierarquia[] = $ob->$map[$class]["descricao"];

		//encontra o PAI pela FK do filho
		foreach ($map as $index => $value){
			if ( key_exists($value["pk"], $ob->getDados()) && ($map[$class]['pk'] != $value["pk"]) && ($ob->$value["pk"]) ){
				$ob = new $index($ob->$value["pk"]);
				$this->buscaHierarquiaObjeto($ob);
				break;
			}				
		}
		
		//caso seja um GRUPO filho de outro GRUPO
		if ( $class == self::GRUPO && key_exists('gru_grpid', $ob->getDados()) && $ob->gru_grpid){
			$ob = new $class($ob->gru_grpid);	
			$this->buscaHierarquiaObjeto($ob);
		}
	}

	public function getHierarquia()
	{
		$recuo = "&nbsp;&nbsp;";
		$txtHierarquia = "";
		foreach ($this->hierarquia as $index => $value){
			$txtHierarquia .= "{$value} ";
			if($index < (count($this->hierarquia)-1)){
				$txtHierarquia .= "<br>{$recuo}<img src='../imagens/seta_filho.gif' align='absmiddle'/> ";
				$recuo .= "&nbsp;&nbsp;";
			}
		}
		return $txtHierarquia;
	}
	
	/*
	 * 
	 */
	public static function buscaPergunta($origemId, $origemClasse, $perid=null){
		$map = array(self::QUESTIONARIO => array("pk" => "queid"),
					 self::GRUPO		=> array("pk" => "grpid", "pk_pai" => "gru_grpid"), 
					 self::ITEMPERGUNTA => array("pk" => "itpid")); 
		$objPergunta = new QPergunta();			 
		$pergunta    = $objPergunta->recuperarColunaParent("perid", array("{$map[$origemClasse]['pk']} = {$origemId}"));
		
		if($perid != null){
			if (in_array($perid, $pergunta)){
				return true;
			}else{
				$objItem = new QItemPergunta();
				foreach ($pergunta as $peridLocal){
					$itens = $objItem->recuperarColunaParent("itpid", array("perid = {$peridLocal}"));
					if(count($itens) > 0)
						foreach ($itens as $item){
							$resultado = Hierarquia::buscaPergunta($item, self::ITEMPERGUNTA, $perid);
							if ($resultado)
								return true;
						}
				}
				$pergunta = array();
			}
		}
		
		if(count($pergunta) > 0 ){
			return true;
		}else{
			$objGrupo = new QGrupo();
			
			$origem = $map[$origemClasse]['pk'];
			if($origemClasse == self::GRUPO)
				$origem = $map[$origemClasse]['pk_pai'];				
			
			$grupos = $objGrupo->recuperarColuna("grpid", array("{$origem} = {$origemId}"));
			if(count($grupos) > 0){
				foreach ($grupos as $grupo){
					$resultado = Hierarquia::buscaPergunta($grupo, self::GRUPO, $perid);
					if ($resultado)
						return true;
				}
			}
		}
		return false;
	}
	
}