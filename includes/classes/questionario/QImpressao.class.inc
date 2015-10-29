<?php
class QImpressao{
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
    private $dadoscondicao;

    /*
	 * Monta a Tela de impressão do questionário
	 *
	 * Includes necessários: 	include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
	 * 							include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 *
	 * Exemplo de como montá-lo: 	$obImprime = new QImpressao();
	 * 								echo $obImprime->montaArvore();
	 *
	 * Parametros:
	 *
	 * cabecalho - Permite habilitar ou não o cabecalho padrão do questionário. Recebe valores 'S'(habilitar) e 'N'(desabilitar). Ex.: $obImprime = new QImpressao( array('cabecalho' => 'N') );
	 * tema 	 - recebe valores de 1 até 4. 1 = Marrom, 2 = verde, 3 = azul, 4 = rosa e cinza por padrão. Ex.: $obImprime = new QImpressao( array('tema' => 1) );
	 *
	 */

	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->it = new QItemPergunta();
		$this->r  = new QResposta();

		$this->cabecalho = $param['cabecalho'];

		$this->perid = $param['perid'];
		$this->qrpid = $_REQUEST['qrpid'];
		$this->queid = array( "queid" => $_REQUEST['queid'] );
		$this->dadoscondicao = isset($param['dadoscondicao']) ? $param['dadoscondicao'] : array();

		if( $param['tema'] == 1 ){ //marrom
			$this->borda 	= '#c3baa7';
			$this->titulo 	= '#e9e0cd';
			$this->fundo 	= '#fffeed';
		} elseif( $param['tema'] == 2 ){ //verde
			$this->borda 	= '#aac6a0';
			$this->titulo 	= '#c9dcc3';
			$this->fundo 	= '#e0eadc';
		} elseif( $param['tema'] == 3 ){ //azul
			$this->borda 	= '#96c1be';
			$this->titulo 	= '#bdd8d7';
			$this->fundo 	= '#d8e8e7';
		} elseif( $param['tema'] == 4 ){ //rosa
			$this->borda 	= '#c5b6ca';
			$this->titulo 	= '#dbd1de';
			$this->fundo 	= '#e9e4eb';
		} else { //padrão
			$this->borda 	= '#cccccc';
			$this->titulo 	= '#dddddd';
			$this->fundo 	= '#f1f1f1';
		}

		self::detalheTipoArvore('resposta');
	}

	function detalheTipoArvore( $tipoArvore ){
		$this->tipoArvore = $tipoArvore;

		// Sistema
		$this->img['s']['open']                 = '../includes/dtree/img/globe.gif';
		$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
		$this->img['s']['incluirQ']             = '';
		$this->img['s']['alterar']              = '';
		$this->img['s']['excluir']              = '';
		// Questionario
		$this->img['q']['open']                 = '../includes/dtree/img/base.gif';
		$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
		$this->img['q']['incluirP']             = '';
		$this->img['q']['incluirG']             = '';
		$this->img['q']['alterar']              = '';
		$this->img['q']['excluir']              = '';
		$this->img['q']['imprime']              = '../imagens/print.png';
		// Grupo
		$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
		$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
		$this->img['g']['incluirP']             = '';
		$this->img['g']['incluirG']             = '';
		$this->img['g']['alterar']              = '';
		$this->img['g']['excluir']              = '';
		// Pergunta
		$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
		$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
		$this->img['p']['respondido']     		= '<img src=\'/imagens/check_p.gif\' title=\''.("Pergunta respondida").'\'>';
		$this->img['p']['incluir']              = '';
		$this->img['p']['alterar']              = '';
		$this->img['p']['excluir']              = '';
		// Item
		$this->img['i']['open']                 = '../imagens/ico_config.gif';
		$this->img['i']['close']    			= '../imagens/ico_config.gif';
		$this->img['i']['incluirP']             = '';
		$this->img['i']['incluirG']             = '';
		$this->img['i']['alterar'] 				= '';
		$this->img['i']['excluir'] 				= '';
	}

	function limitaString( $titulo ){
		if(strlen($titulo) > 60){
			$titulo = substr($titulo,0,60) . '...';
		}
		return $titulo;
	}

	function quebraTitulo( $titulo ){
		$titulo = wordwrap($titulo, 60, "<br />");
		return $titulo;
	}

	function montaArvore( $sisid = null, $padrao = true ){

		echo "<style>

		table.tabelaQuestionario {
			/* background-color:#cccccc; */
			font-family: arial, sans-serif;
			font-size: 14px;
		}

		table.tabelaGrupo {
			background-color:#d1d1d1;
			font-family: arial, sans-serif;
			font-size: 14px;
		}

		td.bordaDireita {
			background-color:#ffffff;
			font-family: arial, sans-serif;
			font-size: 14px;
			width:300px;
		}

		.fieldGrupoTitulo {
			color:#000000;
			font-family:arial, sans-serif;
			font-size: 11px;
			font-weight: bold;

		}

		.fieldGrupo {
			background-color:#ffffff;
			font-family:arial, sans-serif;
			font-size: 12px;
			padding-bottom:10px;
			margin:10px;
			border:1px solid ".$this->borda.";

		}

		.divQuestionario {
			text-align:left;
		}

		.divTituloQuestionario {
			text-align:center;
			font-family:arial, sans-serif;
			font-size: 16px;
			font-weight: bold;
		}

		.divPergunta {
			font-family:verdana, arial, sans-serif;
			font-size: 11px;
			background-color:".$this->fundo.";
			margin-left:10px;
			margin-right:10px;
			margin-top:5px;
			padding-bottom:5px;
			border:1px solid ".$this->borda.";
		}

		.divTituloPergunta {
			background-color:".$this->titulo.";
			padding-top:3px;
			padding-bottom:5px;
			border-bottom:1px solid ".$this->borda.";
		}

		.divResposta {
			font-family:verdana, arial, sans-serif;
			font-size: 11px;
			background-color:".$this->fundo.";
			padding-top:3px;
			padding-left:10px;
		}

		</style>";
		if(!$this->cabecalho){
			echo monta_cabecalho_relatorio('100');
//			echo '<table width="100%" border="0" cellpadding="0" cellspacing="0" style="font-family:arial, sans-serif;">
//				<tr>
//					<td valign="top" width="50" rowspan="2">
//						<img src="../imagens/brasao.gif" width="45" height="45" border="0">
//					</td>
//					<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
//						<font style="font-size:11px;">
//							SIMEC - Sistema Integrado do Ministério da Educação<br/>
//							'.$_SESSION['sisdsc'].'
//						</font>
//					</td>
//					<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
//						<font style="font-size:11px;">
//							'.Translate::get("Impresso por:").' <b>'.$_SESSION['usunome'].'</b><br/>
//							'.Translate::get("Data/Hora da Impressão:").' <b>'.date( 'd/m/Y - H:i:s' ).'</b><br/>
//						</font>
//					</td>
//				</tr>
//					<tr><td>&nbsp;</td></tr>
//			</table>';
		}elseif($this->cabecalho != 'N' && $this->cabecalho){
			echo $this->cabecalho;
		}
		self::montaArvoreQ( $this->queid, $padrao );

	}

	function montaArvoreQ( $queid, $padrao = true ){

		$obPai = $this->q->pegaUmObjeto( $queid );
		// Filtra Questionarios
		$arrObQuestionario = (array) $this->q->listaObjeto( array($obPai), array('queid', 'quetitulo') );

		foreach($arrObQuestionario as $obQuestionario){

			echo "<div class='divQuestionario'><div class='divTituloQuestionario'>".$obQuestionario->quetitulo."</div></center>";

//			self::montaListaEscolaResponsavel();

			// Busca Perguntas associadas
// 			self::montaArvoreP($obQuestionario);

			// Busca Grupo Pergunta associadas
// 			self::montaArvoreG($obQuestionario);


			self::montaArvoreGeral($obQuestionario, $padrao);
			echo "</div>";
		}
	}

	function montaArvoreGeral( $obPai, $padrao = true ){

	    // Filtra Grupos vinculadas
	    $arrItens = (array) $this->q->listaObjetoGeral(array( $obPai ),$this->qrpid, $padrao);
	    foreach ($arrItens as $item){
	        if($item['tipoitem'] == 'g'){
	            $obGrupoPergunta = new QGrupo($item['id']);

	            if(!$obGrupoPergunta->realizarVerificacao($this->dadoscondicao)){
	                continue;
	            }

    	        echo "<fieldset class='fieldGrupo'>";
    	        echo "<legend class='fieldGrupoTitulo'>".$obGrupoPergunta->grptitulo."</legend>";

    	        // Busca Perguntas associadas
    	        self::montaArvoreGeral($obGrupoPergunta,$padrao);

    	        echo "</fieldset>";
            }else if($item['tipoitem'] == 'p'){
                $obPergunta = new QPergunta($item['id']);

	            if(!$obPergunta->realizarVerificacao($this->dadoscondicao)){
	                continue;
	            }

                echo "<div class='divPergunta'>";
                echo "<div class='divTituloPergunta'>".$obPergunta->pertitulo."</div>";
                //			dbg($this->qrpid);
                if ($this->qrpid){
                    $arrR 		 = array( "qrpid" => $this->qrpid, "perid" => $obPergunta->perid );

                    $resposta 	 = $this->r->pegaUmObjeto( $arrR );
                    $arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );
                }

                if( !is_null($resposta->resdsc) ){
                    if($obPergunta->pertipo == 'EXF' && !is_null($resposta->resdsc)){
                        echo "<div class='divResposta'>";
                        $obMonta = new CampoExternoControle();
                        if( method_exists ( $obMonta , retornoRelatorio )){
                            $obMonta->retornoRelatorio( $this->qrpid, $obPergunta->perid );
                        } else {
                            echo ("Campo Externo");
                        }
                        echo "</div>";
                    } elseif( $obPergunta->pertipo != 'EXT' ) {
                        echo "<div class='divResposta'>";
                        echo $resposta->resdsc;
                        echo "</div>";
                    }
                }

                if($obPergunta->pertipo == 'EXT'){
                    echo "<div class='divResposta'>";
                    $campo = new Montatabela();
                    $campo->montaNovaTabela( $obPergunta->perid, $this->qrpid, 90, 1, true);
                    $campo->show();
                    echo "</div>";
                }

                if($obPergunta->pertipo == 'END'){
                    echo "<div class='divResposta'>";
                    if($resposta->qenid){
                        $mEndereco = new QEndereco($resposta->qenid);
        				echo "<strong>CEP:</strong> {$mEndereco->qencep} <br />";
        				echo "<strong>Logradouro:</strong> {$mEndereco->qenlogradouro} <br />";
        				echo "<strong>Número:</strong> {$mEndereco->qennumero} <br />";
        				echo "<strong>Complemento:</strong> {$mEndereco->qencomplemento} <br />";
        				echo "<strong>Bairro:</strong> {$mEndereco->qenbairro} <br />";
        				echo "<strong>Município/UF:</strong> {$mEndereco->getDscMunicipio()}/{$mEndereco->estuf} <br />";

        				if($mEndereco->qencoordenadas){
        				    $zoom       = $mEndereco->qenzoom ? $mEndereco->qenzoom : 5;
        				    $coordenada = str_replace(array("POINT(", ")"), "", $mEndereco->getCoordenada());
        				    $coordenada = explode(" ", $coordenada);
            				echo "<br /><strong>Coordenadas Geográficas:</strong><br />";
        				    echo "<img border='0' src='http://maps.googleapis.com/maps/api/staticmap?size=400x200&maptype=roadmap\&markers=size:mid%7Ccolor:red%7C{$coordenada[1]},{$coordenada[0]}&sensor=false&zoom={$zoom}'>";
        				}
                    }
                    echo "</div>";
                }

                $resp = $this->r->listaObjeto( array($obPergunta), array( "resid", "resdsc" ) );
                //			if( $r->resdsc ){
                //				echo "<div class='divResposta'>";
                //				echo $r->resdsc;
                //				echo "</div>";
                //			}
                if($obPergunta->pertipo == 'ARQ' && $resposta->arqid){
    				echo "<div class='divResposta'>";
    				echo ("Arquivo");
    				echo "</div>";
    			}elseif(!$arrItemResp && is_null($resposta->resdsc) && $obPergunta->pertipo != 'EXT'){
    				echo "<div class='divResposta'>";
    				echo ("Não informado.");
    				echo "</div>";
    			}

                if($arrItemResp){
                    echo $obpergunta->pertitulo;
                    if( $obPergunta->pertipo == 'CK' ){
                        $respostaCK = $this->r->listaObjeto( array( "qrpid" => $this->qrpid, "perid" => $obPergunta->perid ) );
                        echo "<div class='divResposta'>";
                        $pegaItens = $this->it->carregaItensPergunta( $obPergunta->perid, array("itpid", "itptitulo") );
                        foreach($pegaItens as $item){
                            $tem = false;
                            foreach($respostaCK as $resp){
                                if( $item['itpid'] == $resp->itpid ){
                                    $arrItemResp = $this->it->listaObjeto( array( "itpid" => $resp->itpid, "qrpid" => $this->qrpid), array("itpid, itptitulo") );
                                    $arrObPergunta = (array) $this->p->listaObjeto(array( "qrpid" => $this->qrpid, "itpid" => $arrItemResp[0]->itpid ), array("perid, pertitulo, perordem, pertipo"));
                                    echo "<img src=\"/imagens/checked.gif\" align='bottom' >&nbsp;";
                                    echo $arrItemResp[0]->itptitulo;
                                    if( $arrObPergunta[0] ){
                                        self::montaArvoreP( $arrObPergunta[0] );
//                                        self::montaArvoreGeral( $arrObPergunta[0] );
                                    }
                                    $tem = true;
                                }
                            }
                            if( $tem == false ){
                                echo "<img src=\"/imagens/check.gif\" align='bottom' >&nbsp;";
                                echo $item['itptitulo'];
                                echo "<br />";
                            }
                        }
                        echo "</div>";
                    } else {
                        self::montaArvoreI($obPergunta, $arrItemResp, $padrao);
                    }
                }
                echo "</div>";

            }


	    }

	}

	function montaArvoreG( $obPai ){

		// Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("grpid, grptitulo, grpordem"));
		foreach ($arrObGrupoPergunta as $obGrupoPergunta){

			echo "<fieldset class='fieldGrupo'>";
			echo "<legend class='fieldGrupoTitulo'>".$obGrupoPergunta->grptitulo."</legend>";

			// Busca Perguntas associadas
			self::montaArvoreP($obGrupoPergunta);

			// Busca Grupo Pergunta associadas
			if(!(get_class((object) $obPai) == 'ItemPergunta')){//Se for filho de um item então não inclui mais grupos!
				self::montaArvoreG($obGrupoPergunta);
			}
			echo "</fieldset>";

		}

	}

	function montaArvoreP( $obPai ){

		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array("perid, pertitulo, perordem, pertipo"));
		foreach ($arrObPergunta as $obPergunta){
			echo "<div class='divPergunta'>";
			echo "<div class='divTituloPergunta'>".$obPergunta->pertitulo."</div>";
//			dbg($this->qrpid);
			if ($this->qrpid){
				$arrR 		 = array( "qrpid" => $this->qrpid, "perid" => $obPergunta->perid );

				$resposta 	 = $this->r->pegaUmObjeto( $arrR );
				$arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );
			}
		//	dbg($obPergunta->pertipo);
			if( !is_null($resposta->resdsc) ){
				if($obPergunta->pertipo == 'EXF' && !is_null($resposta->resdsc)){
					echo "<div class='divResposta'>";
					$obMonta = new CampoExternoControle();
					if( method_exists ( $obMonta , retornoRelatorio )){
						$obMonta->retornoRelatorio( $this->qrpid, $obPergunta->perid );
					} else {
						echo ("Campo Externo");
					}
					echo "</div>";
				} elseif( $obPergunta->pertipo != 'EXT' ) {
					echo "<div class='divResposta'>";
					echo $resposta->resdsc;
					echo "</div>";
				}
			}

			if($obPergunta->pertipo == 'EXT'){
				echo "<div class='divResposta'>";
				$campo = new Montatabela();
				$campo->montaNovaTabela( $obPergunta->perid, $this->qrpid, 90, 1, true);
				$campo->show();
				echo "</div>";
			}

			$resp = $this->r->listaObjeto( array($obPergunta), array( "resid", "resdsc" ) );
//			if( $r->resdsc ){
//				echo "<div class='divResposta'>";
//				echo $r->resdsc;
//				echo "</div>";
//			}

			if(!$arrItemResp && is_null($resposta->resdsc) && $obPergunta->pertipo != 'EXT'){
				echo "<div class='divResposta'>";
				echo ("Não informado.");
				echo "</div>";
			}

			if($arrItemResp){
				echo $obpergunta->pertitulo;
				if( $obPergunta->pertipo == 'CK' ){
					$respostaCK = $this->r->listaObjeto( array( "qrpid" => $this->qrpid, "perid" => $obPergunta->perid ) );
					echo "<div class='divResposta'>";
					$pegaItens = $this->it->carregaItensPergunta( $obPergunta->perid, array("itpid", "itptitulo") );
					foreach($pegaItens as $item){
						$tem = false;
						foreach($respostaCK as $resp){
							if( $item['itpid'] == $resp->itpid ){
								$arrItemResp = $this->it->listaObjeto( array( "itpid" => $resp->itpid, "qrpid" => $this->qrpid), array("itpid, itptitulo") );
								$arrObPergunta = (array) $this->p->listaObjeto(array( "qrpid" => $this->qrpid, "itpid" => $arrItemResp[0]->itpid ), array("perid, pertitulo, perordem, pertipo"));
								echo "<img src=\"/imagens/checked.gif\" align='bottom' >&nbsp;";
								echo $arrItemResp[0]->itptitulo;
								if( $arrObPergunta[0] ){
									self::montaArvoreP( $arrObPergunta[0] );
								}
								$tem = true;
							}
						}
						if( $tem == false ){
							echo "<img src=\"/imagens/check.gif\" align='bottom' >&nbsp;";
							echo $item['itptitulo'];
							echo "<br />";
						}
					}
					echo "</div>";
				} else {
					self::montaArvoreI($obPergunta, $arrItemResp, $padrao);
				}
			}
			echo "</div>";
		}
	}

	function montaArvoreI( $obPai, $arrItemResp = null, $padrao = true ){
		$where[0] = $obPai;
		if($arrItemResp[0]){
			$where['itpid'] = $arrItemResp;
		}
		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		foreach ($arrObItPergunta as $obItPergunta){
			echo "<div class='divResposta'>";
			echo $obItPergunta->itptitulo."<br>";

			self::montaArvoreGeral($obItPergunta, $padrao);

// 			// Busca Perguntas associadas
// 			$verifica = $this->it->verificaGrupo( $obItPergunta->itpid );
// 			if(!($verifica)){
// 				self::montaArvoreP($obItPergunta);
// 			}

// 			// Busca Grupo Pergunta associadas
// 			unset( $verifica );
// 			$verifica = $this->it->verificaPergunta( $obItPergunta->itpid );
// 			if(!($verifica)){
// 				self::montaArvoreG($obItPergunta);
// 			}
			echo "</div>";
		}
	}


	function montaListaEscolaResponsavel(){
		if ($this->qrpid){
			$escid = $_REQUEST['escid'] ? $_REQUEST['escid'] : $_SESSION['escola']['escid'];
			$escola = new QEscola();
			$dadosEscola 	  = $escola->pegaDadosEscola($escid);
			$dadosResponsavel = $escola->buscarUsuarioPorEscola($escid);
		}
		echo "<fieldset class='fieldGrupo'>";
		echo "<legend class='fieldGrupoTitulo'>".("Dados da Unidade")." </legend>";

		echo "<center><br><table width='95%' style='border:1px solid ".$this->titulo."'>
					<tr>
						<td class='divTituloPergunta' width='35%'>".("Nome da Unidade:")."</td>
						<td class='divResposta'>".($dadosEscola['escnome'] ? $dadosEscola['escnome'] : 'N/A')."</td>
					</tr>
					<tr>
						<td class='divTituloPergunta'>".("Telefone:")."</td>
						<td class='divResposta'>(".(!empty($dadosEscola['escddd']) ? $dadosEscola['escddd'] : "N/A").") ".(!empty($dadosEscola['esctelefone']) ? formata_fone_fax($dadosEscola['esctelefone']) : "N/A")."</td>
					</tr>
					<tr>
						<td class='divTituloPergunta'>".("Região:")."</td>
						<td class='divResposta'>".(!empty($dadosEscola['regdescricao']) ? $dadosEscola['regdescricao'] : "N/A")."</td>
					</tr>
					<tr>
						<td class='divTituloPergunta'>".("Estado:")."</td>
						<td class='divResposta'>".(!empty($dadosEscola['estdescricao']) ? $dadosEscola['estdescricao'] : "N/A")."</td>
					</tr>
					<tr>
						<td class='divTituloPergunta'>".("Município:")."</td>
						<td class='divResposta'>".(!empty($dadosEscola['mundescricao']) ? $dadosEscola['mundescricao'] : "N/A")."</td>

					</tr>
					<tr>
						<td class='divTituloPergunta'>".("Nível/modalidade de ensino:")."</td>
						<td class='divResposta'>".(!empty($dadosEscola['tipdescricao']) ? $dadosEscola['tipdescricao'] : "N/A")."</td>

					</tr>";
		echo "</table></center><br>";
		echo "</fieldset>";

		echo "<fieldset class='fieldGrupo'>";
		echo "<legend class='fieldGrupoTitulo'>".("Responsáveis pelo Preenchimento")."</legend>";

		echo "<center><br><table width='95%' style='border:1px solid ".$this->titulo."'>";

		if( !is_array($dadosResponsavel) ){
			echo "
				  <tr>
						<td class='divResposta' colspan='2' align='center'>".("Não existem responsáveis cadastrados!")."</td>
		  		  </tr>";
		}else{
			foreach($dadosResponsavel AS $indice => $dado){
				echo "
				<tr>
					<td class='divTituloPergunta' width='35%'>".("Nome:")."</td>
					<td class='divResposta'>".(!empty($dado['usunome']) ? $dado['usunome'] : "N/A")."</td>
				</tr>
				<tr>
					<td class='divTituloPergunta' width='35%'>".("Email:")."</td>
					<td class='divResposta'>".(!empty($dado['usuemail']) ? $dado['usuemail'] : "N/A")."</td>
				</tr>
				<tr>
					<td class='divTituloPergunta'>".("Telefone:")."</td>
					<td class='divResposta'>".( strlen($dado['telefone']) > 4 ? $dado['telefone'] : "N/A")."</td>
				</tr>
				<tr>
					<td class='divTituloPergunta'>".("Estado:")."</td>
					<td class='divResposta'>".(!empty($dado['estdescricao']) ? $dado['estdescricao'] : "N/A")."</td>
				</tr>
				<tr>
					<td class='divTituloPergunta'>".("Município:")."</td>
					<td class='divResposta'>".(!empty($dado['mundescricao']) ? $dado['mundescricao'] : "N/A")."</td>
				</tr>";

				if( ($indice+1) != count($dadosResponsavel) ){
				  echo"<tr>
						 <td class='divTituloPergunta' colspan='2' align='center'></td>
			  		  </tr>";
				}
			}
		}

		echo "</table></center><br>";
		echo "</fieldset>";

	}
}