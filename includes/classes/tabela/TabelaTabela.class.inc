<?php
class TabelaTabela extends TabelaMatriz
{
	public function replicarTabela($queid)
	{
		$perg = new QPergunta();
		$param['where'] = array("pertipo = 'EXT'");
		$param['obj'] 	= true;
		$arPerg = $perg->listarPorQuestionario($queid, $param);

		foreach($arPerg as $perg){

            //verifica se a pergunta já possui uma tabela
            $tabela = new Tabela();
            $tabela->carregarPorPergunta($perg->perid);
            if( !$tabela->tblid ){
                $tabela->setDadosNull();
                $perid_matriz  = $perg->perid_matriz;
                $tabela->carregarPorPergunta($perid_matriz);
                $tblid 		   	= $tabela->tblid;
                $tabela->tbltipo= null;
                $tabela->tblid 	= null;
                $tabela->perid 	= $perg->perid;
                $tblidNovo 	   	= $tabela->salvar();
                $tabela->commit();

                if ($tblid){
                    // LINHA
                    $gl = new Grupolinha();
                    $gl = $gl->listarPorTabela($tblid);
                    $gl = $gl ? $gl : array();
                    for($i=0; $i < count($gl); $i++){
                        $grlid = $gl[$i]['grlid'];
                        $gl[$i]['grlid'] = null;
                        $gl[$i]['tblid'] = $tblidNovo;
                        $grupo 			 = new Grupolinha();
                        $grlidNovo 		 = $grupo->popularDadosObjeto($gl[$i])->salvar();
                        $grupo->commit();

                        $l = new Linha();
                        $l = $l->listarPorGrupo($grlid);
                        $l = $l ? $l : array();
                        for ($a=0; $a<count($l); $a++){
                            // Salva ITEM
                            $it    = new Item();
                            $itmid = $it->popularDadosObjeto(array("itmdsc" => $l[$a]['itmdsc']))
                                        ->salvar();
                            $it->commit();

                            $l[$a]['itmid'] = $itmid;
                            $l[$a]['grlid'] = $grlidNovo;
                            $l[$a]['linid'] = null;
                            // Salva Linha
                            $linha = new Linha();
                            $linha->popularDadosObjeto($l[$a])
                                  ->salvar();
                            $linha->commit();
                        }
                    }
                    // COLUNA
                    $gc = new Grupocoluna();
                    $gc = $gc->listarPorTabela($tblid);
                    $gc = $gc ? $gc : array();
                    for($i=0; $i < count($gc); $i++){
                        $grcid = $gc[$i]['grcid'];
                        $gc[$i]['grcid'] = null;
                        $gc[$i]['tblid'] = $tblidNovo;
                        $grupo 			 = new Grupocoluna();
                        $grcidNovo 		 = $grupo->popularDadosObjeto($gc[$i])->salvar();
                        $grupo->commit();

                        $c = new Coluna();
                        $c = $c->listarPorGrupo($grcid);
                        $c = $c ? $c : array();
                        for ($a=0; $a<count($c); $a++){
                            // Salva ITEM
                            $it    = new Item();
                            $itmid = $it->popularDadosObjeto(array("itmdsc" => $c[$a]['itmdsc']))
                                        ->salvar();
                            $it->commit();

                            $c[$a]['itmid'] = $itmid;
                            $c[$a]['grcid'] = $grcidNovo;
                            $c[$a]['colid'] = null;
                            // Salva Coluna
                            $coluna = new Coluna();
                            $coluna->popularDadosObjeto($c[$a])
                                   ->salvar();
                            $coluna->commit();
                        }
                    }
                }
            }
		}
	}
	/**
	 * Função  matrizTela
	 * Método usado para carregar/exibir a tela de matriz
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function matrizTela($perid)
	{
		parent::_initJavaScript();
		parent::renderView("telamatriz");
		self::carregar($perid);
	}

	/**
	 * Função  tabelaTela
	 * Método usado para carregar/exibir a tela de tabela
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function tabelaTela()
	{
		parent::renderView("telatabela");
	}

	/**
	 * Função  tabelaTela
	 * Método usado para carregar/exibir a tela de tabela
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function grupoColunaTela()
	{
		parent::renderView("telagrupocoluna");
	}

	/**
	 * Função  tabelaTela
	 * Método usado para carregar/exibir a tela de tabela
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function colunaTela()
	{
		parent::renderView("telacoluna");
	}

	/**
	 * Função  tabelaTela
	 * Método usado para carregar/exibir a tela de tabela
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function grupoLinhaTela()
	{
		parent::renderView("telagrupolinha");
	}

	/**
	 * Função  tabelaTela
	 * Método usado para carregar/exibir a tela de tabela
	 *
	 * @access public
	 * @since  15/07/2011
	 * @return void
	 */
	public function linhaTela()
	{
		parent::renderView("telalinha");
	}

	/**
	 * Função  salvarPessoaDefault
	 * Método usado salvar a pessoa e dar REFRESH na página atual.
	 *
	 * @access public
	 * @since  17/06/2010
	 * @return void
	 */
	public function salvar($data, $perid)
	{
		$data   = json_decode( stripslashes( utf8_encode($data) ) );

		if (is_array($data->grupoColuna))
			$data->grupoColuna = array_map ('utf8_decode', $data->grupoColuna);

		if (is_array($data->grupoLinha))
			$data->grupoLinha = array_map ('utf8_decode', $data->grupoLinha);

		// Salva TABELA
		$tb = new Tabela();
		$tb->carregarPorPergunta($perid);



		$dado = array("tbldescricao" 			=> utf8_decode($data->tabela),
					  "perid" 					=> $perid,
     				  "tbltotalhorizontal" 		=> $_POST['tbltotalhorizontal'],
    				  "tbltotalhorizontaltxt" 	=> $_POST['tbltotalhorizontaltxt'],
    				  "tbltotalvertical" 		=> $_POST['tbltotalvertical'],
    				  "tbltotalverticaltxt" 	=> $_POST['tbltotalverticaltxt']);

		$tblid  = $tb->popularDadosObjeto($dado)
			   		 ->salvar();
		$tb->commit();

		self::apagarAgregados($perid);

		/*
		 * LOOP Linhas
		 */
		for($a=0; $a < count($data->grupoLinha); $a++){
			// Salva GRUPO LINHA
			$gl = new Grupolinha();
			$grlid = $gl->popularDadosObjeto(array("tblid" => $tblid, "grldsc" => $data->grupoLinha[$a]))
			   			->salvar();
			$gl->commit();
			for($s=0; $s < count($data->linha[$a]); $s++){
				// Salva ITEM
				$it = new Item();
				$itmid = $it->popularDadosObjeto(array("itmdsc" => utf8_decode($data->linha[$a][$s])))
				   			->salvar();
				$it->commit();
				// Associa como Linha
				$ln = new Linha();
				$ln->popularDadosObjeto(array("itmid" => $itmid, "grlid" => $grlid, "lindsc" => "f"))
				   ->salvar();
				$ln->commit();
			}
		}
		/*
		 * LOOP Colunas
		 */
		for($a=0; $a < count($data->grupoColuna); $a++){
			// Salva GRUPO LINHA
			$gc = new Grupocoluna();
			$grcid = $gc->popularDadosObjeto(array("tblid" => $tblid, "grcdsc" => $data->grupoColuna[$a]))
			   			->salvar();
			$gc->commit();
			for($s=0; $s < count($data->coluna[$a]); $s++){
				if(empty($data->coluna[$a][$s])){
					continue;
				}
				list($coltipo, $itmdsc, $colnumeric) = explode("_||_", utf8_decode($data->coluna[$a][$s]));
				// Salva ITEM
				$it = new Item();
				$itmid = $it->popularDadosObjeto(array("itmdsc" => $itmdsc))
				   			->salvar();
				$it->commit();
				// Associa como Linha
				$cl = new Coluna();
				$cl->popularDadosObjeto(array("itmid" => $itmid, "grcid" => $grcid, "coltipo" => $coltipo, "colnumeric" => $colnumeric))
				   ->salvar();
				$cl->commit();
			}
		}

	}

	private function apagarAgregados($perid)
	{
		$tabela = new Tabela();
		$tabela->carregarPorPergunta($perid);

		if ($tabela->tblid){
			// LINHA
			$gl = new Grupolinha();
			$dados = $gl->listarPorTabela($tabela->tblid);
			$dados = $dados ? $dados : array();
			for($i=0; $i < count($dados); $i++){
				$l = new Linha();
				$l->apagarPorGrupo($dados[$i]['grlid']);
				$l->commit();
			}
			$gl->apagarPorTabela($tabela->tblid);
			$gl->commit();

			// COLUNA
			$gc = new Grupocoluna();
			$dados = $gc->listarPorTabela($tabela->tblid);
			$dados = $dados ? $dados : array();
			for($i=0; $i < count($dados); $i++){
				$c = new Coluna();
				$c->apagarPorGrupo($dados[$i]['grcid']);
				$c->commit();
			}
			$gc->apagarPorTabela($tabela->tblid);
			$gc->commit();
		}
	}

	private function carregar($perid)
	{
		if ($perid){
			$tabela = new Tabela();
			$tabela->carregarPorPergunta($perid);

			if ($tabela->tblid){
				$script = "TableManager.saveTabela('".$tabela->tbldescricao."');";
				// LINHA
				$gl = new Grupolinha();
				$gl = $gl->listarPorTabela($tabela->tblid);
				$gl = $gl ? $gl : array();
				for($i=0; $i < count($gl); $i++){
					$script .= "TableManager.saveGrupoLinha('".$gl[$i]['grldsc']."');";
					$l = new Linha();
					$l = $l->listarPorGrupo($gl[$i]['grlid']);
					$l = $l ? $l : array();
					for ($a=0; $a<count($l); $a++){
						$script .= "TableManager.saveLinha({$i}, '{$l[$a]['itmdsc']}');";
					}
				}
				// COLUNA
				$gc = new Grupocoluna();
				$gc = $gc->listarPorTabela($tabela->tblid);
				$gc = $gc ? $gc : array();
				for($i=0; $i < count($gc); $i++){
					$script .= "TableManager.saveGrupoColuna('".$gc[$i]['grcdsc']."');";
					$c = new Coluna();
					$c = $c->listarPorGrupo($gc[$i]['grcid']);
					$c = $c ? $c : array();
					for ($a=0; $a<count($c); $a++){
						$script .= "TableManager.saveColuna({$i}, '{$c[$a]['itmdsc']}', '{$c[$a]['coltipo']}' , '{$c[$a]['colnumeric']}');";
					}
				}

				echo "<script>" . $script . "</script>";
			}
		}
	}
}