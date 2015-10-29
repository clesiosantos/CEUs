<?php
final class ExtractTableToExcel{
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
		$this->buscarPergunta($this->questionario->queid);
	}

	private function ordenarPerguntas(){
		if(is_array($this->pergunta) && count($this->pergunta) > 0){
			$arPerids = array();
			foreach($this->pergunta as $pergunta){
				$arPerids[] = $pergunta['perid'];
			}
			$sql = "SELECT
                        per.perid, per.pertitulo, per.pertipo
                    FROM
                        questionario.pergunta per
                    LEFT JOIN
                        questionario.questionarioordem qor ON qor.perid = per.perid
                    LEFT JOIN
                        questionario.grupopergunta grp ON grp.grpid = per.grpid
                    WHERE
                        per.perid IN (" . implode(", ", $arPerids) . ")
                        AND
                        grp.grpstatus != 'I'
                    ORDER BY
                        qor.qorordem ASC;";
			$this->pergunta = $this->tabelaResposta->carregar($sql);
		}
	}

	public function PHPExcel($tabelas)
	{
//		$tableResp = new Tabelaresposta();
//		/*
//		 * Instância da classe EXCEL
//		 */
//		require_once APPRAIZ . 'includes\classes\Excel\PHPExcel.php';
//		// Criar um novo objecto PHPExcel
//		$PHPExcel = new PHPExcel();
//		$tabelas  = $PHPExcel->getActiveSheet();
//		/*
//		 * Instância da classe EXCEL - FIM
//		 */
		$linhaIniTable = 1;

		$this->ordenarPerguntas();

		foreach ($this->pergunta as $perg){
			$tabela = $this->tabela[$perg['perid']];
			// Adiciona Pegunta
			$tabelas->setCellValueByColumnAndRow(0,$linhaIniTable,utf8_encode($perg['pertitulo']));
			// Dimensão
			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
			$celFim = $tabelas->getCellByColumnAndRow(($tabela['colspan']+2), $linhaIniTable)->getColumn();
			$tabelas->mergeCells("A{$linhaIniTable}:{$celFim}{$linhaIniTable}");
			$linhaIniTable++;
			// Adiciona Título da Tabela
			$tabelas->setCellValueByColumnAndRow(0,$linhaIniTable,utf8_encode($tabela['dados']['tbldescricao']));
			// Dimensão
			$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
			$tabelas->mergeCells("A{$linhaIniTable}:C{$linhaIniTable}");
			// Adiciona Grupos de coluna
			$cgc 	  = 3;
			$grupoColuna = $this->grupoCol[$tabela['dados']['tblid']];
			foreach ($grupoColuna['dados'] as $grupoCol){
				$tabelas->setCellValueByColumnAndRow($cgc,$linhaIniTable,utf8_encode($grupoCol['grcdsc']));
				// Dimensão
				$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
				$celIni = $tabelas->getCellByColumnAndRow($cgc, $linhaIniTable)->getColumn();
				$colIni = $cgc;
				$cgc += ($this->grupoCol[$tabela['dados']['tblid']][$grupoCol['grcid']]['colspan']-1);
				$celFim = $tabelas->getCellByColumnAndRow($cgc, $linhaIniTable)->getColumn();
				$tabelas->mergeCells("{$celIni}{$linhaIniTable}:{$celFim}{$linhaIniTable}");
				$cgc++;
				// Adiciona Coluna
				$coluna = $this->coluna[$grupoCol['grcid']];
				foreach ($coluna as $coluna){
					$tabelas->setCellValueByColumnAndRow($colIni,($linhaIniTable+1),utf8_encode($coluna['itmdsc']));
					// Dimensão
					$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
					$colIni++;
				}
			}
			$linhaIniTable += 2;
			// Linha de início da célula de total
			$linhaIniCellTotal = $linhaIniTable;
			$linhaFimCellTotal = $linhaIniTable + ($tabela['rowspan'] - 1);
			$tabelas->setCellValueByColumnAndRow(0,$linhaIniCellTotal,utf8_encode("Total"));
			$tabelas->mergeCells("A{$linhaIniCellTotal}:A{$linhaFimCellTotal}");
			$linhaIniTable = $linhaFimCellTotal+1;

			foreach ($this->unidadeProjeto as $unidadeProjeto){
				// Buscar resposta
				$qrpid 	   = $this->buscarQuestionarioResposta($unidadeProjeto['codigo'], $unidadeProjeto['tipo']);
				if ($qrpid){
					$tableResp = $this->buscarRespostaTabela($tabela['dados']['tblid'], $qrpid);
				}else{
					$tableResp = array();
				}
				// Adiciona Grupo linha
				$grupoLinha = $this->grupoLinha[$tabela['dados']['tblid']];
				$linhaIniGrlLinha = $linhaIniTable;
				$linhaIniGrlTotal = $linhaIniCellTotal;

				// Contador LINHA
				$linhaIniLinha 		= $linhaIniTable;
				$linhaIniLinhaTotal = $linhaIniCellTotal;
				foreach ($grupoLinha['dados'] as $grupoLin){
					// Adiciona grupo linha (total)
					$tabelas->setCellValueByColumnAndRow(1,$linhaIniGrlTotal,utf8_encode($grupoLin['grldsc']));
					$linhaFimGrlTotal = ($linhaIniGrlTotal + $this->grupoLinha[$tabela['dados']['tblid']][$grupoLin['grlid']]['rowspan'] -1);
					$tabelas->mergeCells("B{$linhaIniGrlTotal}:B{$linhaFimGrlTotal}");
					$linhaIniGrlTotal = ($linhaFimGrlTotal+1);

					$tabelas->setCellValueByColumnAndRow(1,$linhaIniGrlLinha,utf8_encode($grupoLin['grldsc']));
					// Dimensão
					$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
					$linhaFimGrlLinha = ($linhaIniGrlLinha + $this->grupoLinha[$tabela['dados']['tblid']][$grupoLin['grlid']]['rowspan'] -1);
					$tabelas->mergeCells("B{$linhaIniGrlLinha}:B{$linhaFimGrlLinha}");
					$linhaIniGrlLinha = ($linhaFimGrlLinha+1);

					// Adiciona linha
					$linha 				= $this->linha[$grupoLin['grlid']];
//					$linhaIniLinha 		= $linhaIniTable;
//					$linhaIniLinhaTotal = $linhaIniCellTotal;
					foreach ($linha as $linha){
						// Adiciona linha (total)
						$tabelas->setCellValueByColumnAndRow(2,$linhaIniLinhaTotal,utf8_encode($linha['itmdsc']));

						$tabelas->setCellValueByColumnAndRow(2,$linhaIniLinha,utf8_encode($linha['itmdsc']));
						// Dimensão
						$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
						$indCol = 3;

						// Adiciona Resposta
						foreach ($grupoColuna['dados'] as $grupoCol){
							foreach ($this->coluna[$grupoCol['grcid']] as $coluna){
								//calcula o total
								if(is_numeric($tableResp[$linha['linid']][$coluna['colid']])):
									if($totalRespostas[$linha['linid']][$coluna['colid']] == null):
										$totalRespostas[$linha['linid']][$coluna['colid']] = 0;
									endif;
									$totalRespostas[$linha['linid']][$coluna['colid']] += $tableResp[$linha['linid']][$coluna['colid']];
									// Adiciona resposta (total)
									$tabelas->setCellValueByColumnAndRow($indCol,$linhaIniLinhaTotal,utf8_encode($totalRespostas[$linha['linid']][$coluna['colid']]));
								endif;

								$tabelas->setCellValueByColumnAndRow($indCol,$linhaIniLinha,utf8_encode($tableResp[$linha['linid']][$coluna['colid']]));
								// Dimensão
								$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
								$indCol++;
							}
						}
						$linhaIniLinhaTotal++;
						$linhaIniLinha++;
					}
				}
				// Adiciona escola
				$tabelas->setCellValueByColumnAndRow(0,$linhaIniTable,utf8_encode($unidadeProjeto['descricao']));
				// Dimensão
				$tabelas->getColumnDimension($tabelas->getHighestColumn())->setAutoSize(true);
				$linhaFimTable = $linhaIniTable + ($tabela['rowspan'] - 1);
				$tabelas->mergeCells("A{$linhaIniTable}:A{$linhaFimTable}");
				$linhaIniTable = ($linhaFimTable+1);
			}
			$linhaIniTable++;
		}

//		// Indicação da criação do ficheiro
//		$objWriter = PHPExcel_IOFactory::createWriter($PHPExcel, 'Excel5');
//
//		// Encaminhar o ficheiro resultante para abrir no browser ou fazer download
//		header('Content-Type: application/vnd.ms-excel');
//		header('Content-Disposition: attachment;filename="teste.xls"');
//		header('Cache-Control: max-age=0');
//		$objWriter->save('php://output');
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

	private function buscarPergunta($queid)
	{
		$pergunta = new QPergunta();
		$where 	  = array("queid_atual"=>$queid, "pertipo"=>'EXT');
		self::addCadaPergunta($pergunta->lista($where, 'perid, pertitulo, pertipo', 'pertitulo'));
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
		return $this->tabelaResposta->recuperarResposta($tblid, $qrpid);
	}

	private function addCadaPergunta($pergunta)
	{
		if (is_array($pergunta)){
			foreach ($pergunta as $pergunta){
				array_push($this->pergunta, $pergunta);
				$this->peridVez = $pergunta['perid'];
				self::buscarTabela($pergunta['perid']);
			}
		}
	}
}




