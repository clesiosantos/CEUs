<?php

final class ExtractMultipleTableToExcel{
	private $peridVez;
	private $questionario;
	private $questionarioResposta;
	private $tabelaResposta;
	private $acordo;
	private $unidadeProjeto;
	private $pergunta;
	private $tabela;
	private $grupoCol;
	private $coluna;
	private $grupoLinha;
	private $linha;

	public function __construct(Array $param)
	{
		$this->tabelaResposta 		= new Tabelaresposta();
		$this->questionarioResposta = new QQuestionarioResposta();
		$this->pergunta 			= array();
		$this->buscarQuestionarioPorAcordo($param['acoid']);
		$this->buscarUnidadeProjetoPorQuestionario($this->questionario->queid);

		$this->buscarPergunta();
	}

	public function PHPExcel($tabelas)
	{
	    $arDados = $this->preparaArray();
		$linhaIniTable = 1;

		$arDetalhe = array("Male", "Female");
		$arTitulo  = array(("Projeto"), ("Detalhe"), "Total");
		for($i = 1; $i <= 8; $i++){
		    $arTitulo[] = "Level/grade {$i}";
		}
	    foreach($arDados['total'] as $ppertitulo => $perg){
	        $tabela = $this->tabela[$perg["perguntas"]['perid']];

			$tabelas->setCellValueByColumnAndRow(0,$linhaIniTable,utf8_encode($perg["perguntas"]['perptitulo']));
			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);

			$celFim = $tabelas->getCellByColumnAndRow(count($arTitulo), $linhaIniTable)->getColumn();
			$tabelas->mergeCells("A{$linhaIniTable}:{$celFim}{$linhaIniTable}");
			$linhaIniTable++;

			foreach($arTitulo as $k => $titulo){
    			$tabelas->setCellValueByColumnAndRow($k, $linhaIniTable, utf8_encode($titulo));
    			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
			}
			$linhaIniTable++;
			foreach ($this->unidadeProjeto as $kUnidade => $unidadeProjeto){
			    for($i = 0; $i < QTD_COLUNAS_QSTOES_MULTI_EXPORT_EXCEL; $i++){
    			    $tabelas->setCellValueByColumnAndRow(0,$linhaIniTable,utf8_encode($unidadeProjeto['descricao'] . " (".($i+1).")"));
        			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
    			    $linhaFim = $linhaIniTable + count($arDetalhe) - 1;
    			    $tabelas->mergeCells("A{$linhaIniTable}:A{$linhaFim}");

        			foreach($arDetalhe as $detalhe){
    			        $colAtual = 1;
    			        $tabelas->setCellValueByColumnAndRow($colAtual,$linhaIniTable,utf8_encode($detalhe));
            			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
    			        $colAtual++;

    			        foreach($arTitulo as $k => $titulo){
    			            if($k < 2){
    			                continue; // ignora as duas primeiras colunas
    			            }

    			            // Coloca o valor da resposta
    			            $tipo = $k == 2 ? "total" : "level_grade";
    			            if(isset($arDados[$tipo][$ppertitulo]["resposta"][$titulo][$detalhe][$unidadeProjeto['codigo']][$i])){
            			        $tabelas->setCellValueByColumnAndRow($colAtual,$linhaIniTable,utf8_encode($arDados[$tipo][$ppertitulo]["resposta"][$titulo][$detalhe][$unidadeProjeto['codigo']][$i]));
                    			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);

                    			// Se for total, passa para a próxima linha e ignora o resto do cabeçalho
                    			if($tipo == "total"){
                    			    foreach($arTitulo as $k2 => $titulo2){
                    			        if($k2 < 3){
                    			            continue; // ignora as 3 primeiras colunas
                    			        }
                        			    // Se na mesma posição $i exisitir um level_grade, joga ele para a última posição e o retira da posição atual
                        			    if(isset($arDados["level_grade"][$ppertitulo]["resposta"][$titulo2][$detalhe][$unidadeProjeto['codigo']][$i])){
                        			        $arDados["level_grade"][$ppertitulo]["resposta"][$titulo2][$detalhe][$unidadeProjeto['codigo']][] = $arDados["level_grade"][$ppertitulo]["resposta"][$titulo2][$detalhe][$unidadeProjeto['codigo']][$i];
                        			        unset($arDados["level_grade"][$ppertitulo]["resposta"][$titulo2][$detalhe][$unidadeProjeto['codigo']][$i]);
                        			    }
                    			    }
                    			    break;
                    			}
    			            }
        			        $colAtual++;
    			        }
            			$linhaIniTable++;
        			}
			    }
			}
    		$linhaIniTable += 2;
	    }

	}

	private function preparaArray()
	{
	    $arDados = array();
	    $this->ordenarPerguntas();
	    foreach ($this->pergunta as $tipo => $arPerguntas){
	        foreach($arPerguntas as $perg){
	            $tabela = $this->tabela[$perg['perid']];
	            $arDados[$tipo][$perg['perptitulo']]['perguntas'] = $perg;
	            foreach ($this->unidadeProjeto as $kProjeto => $unidadeProjeto){
    				// Buscar resposta
    				$qrpid = $this->buscarQuestionarioResposta($unidadeProjeto['codigo'], $unidadeProjeto['tipo']);
    				if ($qrpid && ($qrpid == $perg['qrpid'] || is_null($perg['qrpid']))){
    					$tableResp = $this->buscarRespostaTabela($tabela['dados']['tblid'], $qrpid);
    				}else{
    					$tableResp = array();
    				}

    				foreach($tableResp as $linid => $arColunas){
				        $dadosLinha = $this->questionarioResposta->pegaLinha("SELECT * FROM tabela.linha lin INNER JOIN tabela.item itm ON lin.itmid = itm.itmid WHERE lin.linid = {$linid}");
				        // No desenho do excel as informações irão se inverter, o que é linha vira coluna e o que é coluna vira linha
				        $arDados[$tipo][$perg['perptitulo']]['titulo']['coluna'][$dadosLinha["itmdsc"]] = $dadosLinha["itmdsc"];
    				    foreach($arColunas as $colid => $tbrdsc){
    				        $dadosColuna = $this->questionarioResposta->pegaLinha("SELECT * FROM tabela.coluna col INNER JOIN tabela.item itm ON col.itmid = itm.itmid WHERE col.colid = {$colid}");
    				        // No desenho do excel as informações irão se inverter, o que é linha vira coluna e o que é coluna vira linha
    				        $arDados[$tipo][$perg['perptitulo']]['titulo']['linha'][$dadosColuna["itmdsc"]] = $dadosColuna["itmdsc"];
    				        // Armazena a resposta
    				        $arDados[$tipo][$perg['perptitulo']]['resposta'][$dadosLinha["itmdsc"]][$dadosColuna["itmdsc"]][$unidadeProjeto['codigo']][] = $tbrdsc;
    				    }

    				}

	            }
	        }
	    }

	    return $arDados;
	}

	private function ordenarPerguntas(){
	    if(is_array($this->pergunta['total']) && count($this->pergunta['total']) > 0){
            $arPerids = array();
	        foreach($this->pergunta['total'] as $pergunta){
                $arPerids[] = $pergunta['perid'];
            }
            $this->pergunta['total'] = $this->getPerguntasOrdenadas($arPerids);
	    }

	    if(is_array($this->pergunta['level_grade']) && count($this->pergunta['level_grade']) > 0){
            $arPerids = array();
	        foreach($this->pergunta['level_grade'] as $pergunta){
                $arPerids[] = $pergunta['perid'];
            }
            $this->pergunta['level_grade'] = $this->getPerguntasOrdenadas($arPerids);
	    }
	}

	private function getPerguntasOrdenadas($arPerids){
	    $sql = "SELECT
                        per.perid,
	                    per.pertitulo,
	                    per.pertipo,
	                    COALESCE(grp.grpid_pai, grp.grpid) AS grpid,
	                    COALESCE(grpp.grptitulo, grp.grptitulo) AS grptitulo,
	                    grp.qrpid, perp.pertitulo AS perptitulo,
	                    perp.perid AS perpid
                    FROM
                       questionario.pergunta per
	                INNER JOIN
                        questionario.itempergunta itp ON itp.itpid = per.itpid
	                INNER JOIN
	                   questionario.pergunta perp ON itp.perid = perp.perid
                    LEFT JOIN
                        questionario.questionarioordem qor ON qor.perid = perp.perid
                    INNER JOIN
                        questionario.grupopergunta grp ON grp.grpid = perp.grpid AND grp.grpstatus = 'A'
                    LEFT JOIN
                        questionario.grupopergunta grpp ON grpp.grpid = grp.grpid_pai
                    WHERE
                        per.perid IN (" . implode(", ", $arPerids) . ")
                        AND
                        per.perstatus = 'A'
                    ORDER BY
                        qor.qorordem ASC;";
	    return $this->questionarioResposta->carregar($sql);
	}

	private function buscarPergunta()
	{
		$pergunta = new QPergunta();

		$peridLevelGrade = $GLOBALS["ARRAY_EXPORTAR_TABELA_SUBITEM"]['level_grade'];
		$peridTotal      = $GLOBALS["ARRAY_EXPORTAR_TABELA_SUBITEM"]['total'];

		$where = array("(perid IN (".implode(", ", $peridTotal).") OR perid_matriz IN (".implode(", ", $peridTotal)."))");
		self::addCadaPergunta($pergunta->lista($where, 'perid, pertitulo, pertipo, grpid', 'perordem'), 'total');

		$where = array("(perid IN (".implode(", ", $peridLevelGrade).") OR perid_matriz IN (".implode(", ", $peridLevelGrade)."))");
		self::addCadaPergunta($pergunta->lista($where, 'perid, pertitulo, pertipo, grpid', 'perordem'), 'level_grade');
	}


	private function buscarQuestionarioResposta($codigo, $tipo = '')
	{
	    $queid     = $this->questionario->queid;
	    $questResp = $this->questionarioResposta;

	    if($tipo == 'P'){
	        $qrpid = $questResp->carregaUm(array("queid" => $queid,
	                "proid" => $codigo));
	    }else{
	        $qrpid = $questResp->carregaUm(array("queid" => $queid,
	                "escid" => $codigo));
	    }

	    return $qrpid;
	}

	private function buscarQuestionarioPorAcordo($acoid)
	{
	    $this->acordo 		= new QAcordo($acoid);
	    $questionario 		= new QQuestionario();
	    $this->questionario = $questionario->pegaUmObjetoPorAcordo($acoid);
	}

	private function buscarUnidadeProjetoPorQuestionario($queid)
	{
	    // Busca as unidades
	    $escola   = new QEscola();
	    $rsEscola = $escola->buscarEscolasPorQuestionario($queid, "escid AS codigo, escnome AS descricao, 'E' AS tipo");
	    $rsEscola = is_array($rsEscola) ? $rsEscola : array();

	    // Busca os projetos
	    $projeto   = new QProjeto();
	    $rsProjeto = $projeto->buscarProjetosPorQuestionario($queid, "proid AS codigo, pronome AS descricao, 'P' AS tipo");
	    $rsProjeto = is_array($rsProjeto) ? $rsProjeto : array();

	    $this->unidadeProjeto = array_merge($rsEscola, $rsProjeto);
	}

	private function buscarTabela($perid)
	{
	    $tabela = new Tabela();
	    $tabela->carregarPorPergunta($perid);
	    $this->tabela[$perid]['dados']   = $tabela->getDados();
	    $this->tabela[$perid]['colspan'] = 0;
	    if ($tabela->tblid){
	        self::buscarGrupoColuna($tabela->tblid);
	        self::buscarGrupoLinha($tabela->tblid);
	    }
	}

	private function buscarGrupoColuna($tblid)
	{
	    $grupoC = new Grupocoluna();
	    $grupoC = $grupoC->listarPorTabela($tblid);
	    $grupoC = is_array($grupoC) ? $grupoC : array();

	    $this->grupoCol[$tblid]['dados']   = $grupoC;

	    foreach ($grupoC as $grupoC){
	        $this->grupoCol[$tblid][$grupoC['grcid']]['colspan'] = 0;
	        self::buscarColuna($grupoC['grcid']);
	    }
	}

	private function buscarColuna($grcid)
	{
	    $perid = $this->peridVez;
	    $tblid = $this->tabela[$perid]['dados']['tblid'];

	    $coluna = new Coluna();
	    $coluna = $coluna->listarPorGrupo($grcid);
	    $coluna = is_array($coluna) ? $coluna : array();
	    $this->coluna[$grcid] = $coluna;

	    $totC = count($coluna);
	    $this->tabela[$perid]['colspan']   += $totC;
	    $this->grupoCol[$tblid][$grcid]['colspan'] = $totC;
	}

	private function buscarGrupoLinha($tblid)
	{
	    $grupoL = new Grupolinha();
	    $grupoL = $grupoL->listarPorTabela($tblid);
	    $grupoL = is_array($grupoL) ? $grupoL : array();

	    $this->grupoLinha[$tblid]['dados']   = $grupoL;

	    foreach ($grupoL as $grupoL){
	        $this->grupoLinha[$tblid][$grupoL['grlid']]['rowspan'] = 0;
	        self::buscarLinha($grupoL['grlid']);
	    }
	}

	private function buscarLinha($grlid)
	{
	    $perid = $this->peridVez;
	    $tblid = $this->tabela[$perid]['dados']['tblid'];

	    $linha = new Linha();
	    $linha = $linha->listarPorGrupo($grlid);
	    $linha = is_array($linha) ? $linha : array();
	    $this->linha[$grlid] = $linha;

	    $totL = count($linha);
	    $this->tabela[$perid]['rowspan']   += $totL;
	    $this->grupoLinha[$tblid][$grlid]['rowspan'] = $totL;
	}

	private function buscarRespostaTabela($tblid, $qrpid)
	{
	    $resposta = array();
	    $sql 	  = "SELECT
	                       linid, colid, tbrdsc
	                 FROM
	                       tabela.tabelaresposta tbr
                     INNER JOIN
                            tabela.tabela tbl ON tbl.tblid = tbr.tblid
                     INNER JOIN
                        questionario.pergunta per ON per.perid = tbl.perid AND per.perstatus = 'A'
                     INNER JOIN
                        questionario.itempergunta itp ON itp.itpid = per.itpid
                     INNER JOIN
			             questionario.pergunta perp ON perp.perid = itp.perid
                     INNER JOIN
                        questionario.grupopergunta grp ON grp.grpid = perp.grpid AND grp.grpstatus = 'A'
                     INNER JOIN
                        questionario.questionarioresposta qrp ON qrp.qrpid = tbr.qrpid
                     INNER JOIN
			             questionario.resposta res ON itp.itpid = res.itpid AND qrp.qrpid = res.qrpid
                     WHERE
	                       tbr.tblid = {$tblid}
	                   AND
	                       tbr.qrpid = {$qrpid}";

	    $arrResp  = $this->questionarioResposta->carregar( $sql );
	    $arrResp  = !is_bool( $arrResp ) ? $arrResp : array();

	    foreach( $arrResp as $r ){
	        $resposta[$r['linid']][$r['colid']] = $r['tbrdsc'];
	    }

	    return $resposta;
	}

	private function addCadaPergunta($pergunta, $tipo)
	{
	    if (is_array($pergunta)){
	        foreach ($pergunta as $pergunta){
	            $this->pergunta[$tipo][] = $pergunta;
	            $this->peridVez = $pergunta['perid'];
	            self::buscarTabela($pergunta['perid']);
	        }
	    }
	}
}




