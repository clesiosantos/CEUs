<?php
class Tela{
	private $queid;
	private $ger;
	private $js;
	private $table;
	private $styleParam;
	private $tableQuestionario;
	private $cellCabecalho;
	private $cellQuestionario;
	private $cellRodape;
	private $cellArvore;
	private $nivel;
	private $itemPergunta;
	private $tipoTela;
	private $subPergunta;
	private $arrDivSubPergunta;
	private $preview;
	private $padrao;
	private $urlFechar; // URL de retorno ao fechar o questionário
	/*
	 * Monta a Tela principal do questionário
	 *
	 * Includes necessários: include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
	 * 						 include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
	 * Exemplo de como montá-lo: $tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S') );
	 *
	 * @param $arrParam com os seguintes parametros que podem ou não ser passados:
	 * qrpid 		- único parametro obrigatório. É o identificador de uma determinada resposta ao questionário. Cada entidade de resposta individual terá um. Ex: "qrpid" => $qrpid (qrpid atribuido anteriormente)
	 * tamDivArvore - Tamanho horizontal da árvore do questionário em porcentagem. Ex: 'tamDivArvore' => 25 (25 %)
	 * tamDivPx 	- Tamanho horizontal da árvore do questionário em pixels. 'tamDivPx' => 400 (400 px)
	 * habilitado	- Habilita/Desabilita o questionário. Precisa de parametro 'S' ou 'N' para habilitado e desabilitado respectivamente. Ex: 'habilitado' => 'S' (habilitado)
	 * relatorio	- Habilita/Desabilita a impressão de relatórios do questionário. Como parametro é passado a pagina onde o relatório será impresso a partir do modulo. Ex: 'relatorio' => 'modulo=relatorioQuestionario&acao=A'
	 *
	 * TODOS OS PARAMETROS, A NÃO SER O QRPID, SÃO OPCIONAIS.
	 *
	 */

	function __construct(Array $arrParam){

		include_once APPRAIZ . "includes/classes/html/TElement.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTable.class.inc";
		include_once APPRAIZ . "includes/classes/html/TMapa.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableRow.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTableCell.class.inc";
		include_once APPRAIZ . "includes/classes/html/TField.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCombo.class.inc";
		include_once APPRAIZ . "includes/classes/html/TTextArea.class.inc";
		include_once APPRAIZ . "includes/classes/html/TText.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TCheckButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioGroup.class.inc";
		include_once APPRAIZ . "includes/classes/html/TRadioButton.class.inc";
		include_once APPRAIZ . "includes/classes/html/TData.class.inc";
		include_once APPRAIZ . "includes/classes/html/TLabel.class.inc";
		include_once APPRAIZ . "includes/classes/html/FormularioQuestionario.class.inc";

		include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";
		include_once APPRAIZ . "includes/classes/questionario/JavaScript.class.inc";

		$this->preview   = $arrParam['preview'] = isset($arrParam['preview']);
		$this->urlFechar = isset($arrParam['urlFechar']) && $arrParam['urlFechar'] ? $arrParam['urlFechar'] : false;

		if($this->preview){
			$hiddenPrv = new TElement('input');
			$hiddenPrv->id    = 'preview';
			$hiddenPrv->value = 1;
			$hiddenPrv->type  = 'hidden';
			$hiddenPrv->show();

            $this->padrao = true;
        }else{
            $this->padrao = false;
        }


        if ( $_REQUEST['ajax']){
			ob_clean();
		}

		$arrParam['qrpid'] = $_POST['qrpid'] ? $_POST['qrpid'] : $arrParam['qrpid'];
		$arrParam['queid'] = $_POST['queid'] ? $_POST['queid'] : $arrParam['queid'];
		$this->queid 	   = $arrParam['queid'];
		$arrParam['perid'] = $_POST['perid'] ? $_POST['perid'] : $arrParam['perid'];
		$arrParam['itpid'] = $_POST['itpid'] ? $_POST['itpid'] : $arrParam['itpid'];

		$arrParam['dadoscondicao'] = $_POST['dadoscondicao'] ? $_POST['dadoscondicao'] : $arrParam['dadoscondicao'];

		$this->styleParam['tamDivArvore'] = $arrParam['tamDivArvore'] ? $arrParam['tamDivArvore'] : 30;
		$this->styleParam['tamDivPx'] 	  = $arrParam['tamDivPx'] ? $arrParam['tamDivPx'] : 400;
		$this->relatorio = $arrParam['relatorio'] ? $arrParam['relatorio'] : 'N';

		if ($arrParam['itpid']){
			$this->tipoTela = "questionario";
		}

		if($_REQUEST['instrucao'] == 'excluirMultiplo'){
		    $modeloGrupo = new QGrupo($_REQUEST['grpid']);
		    $modeloGrupo->desabilitaGrupo($modeloGrupo);
		    die(("Página excluída com sucesso!"));
		}else if($_REQUEST['instrucao'] == 'submeter-questionario'){
            $mQuestResp = new QQuestionarioResposta($arrParam['qrpid']);
            $mQuestResp->submeterQuestionario();
            $mQuestResp->commit();
		    die("ok");
		}

        //trata aduplicação de grupos multiplos
        if($_POST['duplicaGrupo']){
            $modeloGrupo = new QGrupo();
            $modeloGrupo->duplicaGrupo($_REQUEST['grpid'],$arrParam['qrpid']);
        }

	    if ($_POST['salvar_questionario']){
			$arrParamSalv 						 = $arrParam;
			$arrParamSalv['salvar_questionario'] = $_POST['salvar_questionario'];
			$obQ = new GerenciaQuestionario($arrParamSalv);
			$obQ->salvar($_POST, $_FILES);
		}

		if($_POST['download_arquivo'] && $_POST['arqid']){
		    $obQ = $obQ ? $obQ : $obQ = new GerenciaQuestionario($arrParam);
		    $obQ->downloadArquivo($_POST['arqid']);
		}
		$this->ger 	 = new GerenciaQuestionario($arrParam);

        $this->desab = $this->ger->desabilitaQuestionario( $arrParam['habilitado'] );

        // Desabilita o questionário por já ter sido submetido
        if($this->desab){
            $mQuestResp  = new QQuestionarioResposta($arrParam['qrpid']);
            $this->desab = $mQuestResp->qrpfinalizado == 't' ? false : true;
        }

        if($this->preview){
			$this->arv 	 = new QArvore( array("questionario" => $this->ger->getQuestionario(), "perid" => $this->ger->getPeridPai(), "relatorio" => $this->relatorio, 'preview' => $this->preview, 'queid' => $arrParam['queid'], "gerencia_questionario" => $this->ger, "dadoscondicao" => $arrParam['dadoscondicao'] ) );
        }else{
			$this->arv 	 = new QArvore( array("questionario" => $this->ger->getQuestionario(), "perid" => $this->ger->getPeridPai(), "qrpid" => $arrParam['qrpid'], "relatorio" => $this->relatorio, 'preview' => !$this->desab, "gerencia_questionario" => $this->ger, "dadoscondicao" => $arrParam['dadoscondicao']) );
        }

		$this->js 	 = new JavaScript( $this->ger );

		$this->nivel 		= 0;
		$this->itemPergunta = array();
		if ( $this->tipoTela != "questionario" ){
			self::montaTela();
		}else{
			self::montaTelaQuestionario();
		}
		if ( $_POST['ajax'] ){
			ob_flush();
			ob_end_clean();
			exit;
		}
	}

	private function imgNivel(){
		$img 		= new TElement( 'img' );
		$img->style = 'padding-left:' . $this->nivel * 10 . 'px;';
		$img->src 	= '../imagens/seta_filho.gif';
		$img->align = 'absmiddle';
		return $img;
	}


	private function imgAjuda( $ajuda ){
//		$ajuda = htmlspecialchars_decode(str_replace(array("\n", "\r", "\t"), "", nl2br($ajuda)), ENT_QUOTES);

		$img 			  = new TElement( 'img' );
		$img->src 		  = '/imagens/ajuda.gif';
		$img->title 	  = $ajuda;
//		$img->onmouseover = "return escape('" . addslashes( $ajuda ) . "')";
		return $img;
	}

	function montaTelaQuestionario(){
		$this->cellQuestionario = new TElement("div");
		self::questionario();

		if($this->tableQuestionario){
    		// Gera linha do javascript
    		$row 		= $this->tableQuestionario->addRow();
    		$row->style = "display : none;";
    		// Monta o JavaScript que insere a sub-pergunta na class Pergunta => (JavaScript).
    		$this->js->notInclude();
    		$this->js->notFonte();
    		$this->js->notParamJs();
//		    $this->js->atualizaPergunta();
    		$row->addCell( $this->js );
		}

		$this->cellQuestionario->show();
	}

	function montaTela(){
		$this->table   = new TTable;
		$tela 		   = $this->table;

		if ( !$_POST['ajax'] ){
			$tela 		    = new FormularioQuestionario();
			$tela->onsubmit = 'return false;';
			$div = new TElement('div');
			$div->id = 'telacentral';
			$div->add( $this->table );
			$tela->add( $div );
//			$tela->add( $form );
		}
		$this->table->border = 0;
		$this->table->height = 500;
		$row  		   		 = $this->table->addRow();
		// Gera célula da árvore
		$cell 			  = $row->addCell('');
		$cell->id 		  = 'telaarvore';
//		$cell->style 	  = 'width:30%;';
		$cell->rowspan 	  = 3;
		$this->cellArvore = $cell;
		// Gera célula do cabecalho
//		$row  		   	  = $this->table->addRow();
		$cell 		  	  = $row->addCell('');
		$cell->id 	  	  = 'telacabecalho';
		$cell->align  	  = "center";
		$cell->valign 	  = "bottom";
		$cell->style 	  = "padding-bottom:0px; height: 5px;";
		$this->cellCabecalho = $cell;
		// Gera célula do questionario
		$row  		   	  		= $this->table->addRow();
		$cell 		  			= $row->addCell('');
		$cell->id 	  			= 'telaquestionario';
		$cell->align  			= "center";
		$cell->valign 			= "top";
		$this->cellQuestionario = $cell;
		// Gera célula do rodapé
		$row  		   	  = $this->table->addRow();
		$cell 		  	  = $row->addCell('');
		$cell->id 	  	  = 'telarodape';
		$cell->align  	  = "center";
		$cell->valign 	  = "bottom";
		$cell->style 	  = "padding-bottom:13px;";
		$this->cellRodape = $cell;

		self::arvore();
		self::questionario();

		if ( $_POST['ajax'] ){
			$this->js->notInclude();
			$this->js->notFonte();
			$this->js->notParamJs();
			$this->js->atualizaPergunta();
			// Gera linha do javascript
			$row = $this->table->addRow();
			$row->style = "display : none;";
			$row->addCell( $this->js );
		}else{
			$tela->add( $this->js );
		}

		$div        = new TElement('div');
		$div->id    = "dialog-mensagem";
		$div->style = "font-size: 13px; display: none;";

		$span       = new TElement('span');
		$span->id   = "dialog-mensagem-conteudo";

		$div->add($span);
		$tela->add($div);

		$tela->show();
	}


	private function arvore(){
		// conteúdo célula I
		$fieldSet = new TElement("fieldset");
		$fieldSet->style = "overflow:auto; background:#FFF; height:510px%; width:".$this->styleParam['tamDivPx']."px;";
		$legend = new TElement("legend");
		$legend->add( $this->arv->quebraTitulo( $this->ger->getQueTitulo() ) );
		$fieldSet->add( $legend );

		// Div externa
		$dI 		  = new TElement('div');
		$dI->id    = "bloco";
		$dI->class = "dtree";
		$dI->style = "overflow:auto; text-align:left; height:500px; width:".$this->styleParam['tamDivPx']."px;";
//		$dI->style = "overflow:hidden; text-align:left; width:100px;";
		// Paragrafo Interno a div externa
		$p = new TElement('p');
		// Link I de dentro do paragrafo
		$aI = new TElement('a');
		$aI->href = 'javascript: arvore.openAll();';
		$aI->add(('Abrir Todos'));
		// Link II de dentro do paragrafo
		$aII = new TElement('a');
		$aII->href = 'javascript: arvore.closeAll();';
		$aII->add(('Fechar Todos'));
		// Div da árvore
		$dII 		  = new TElement('div');
		$dII->id    = "_arvore";
//		$dII->width = "100";
//		$dII->style = "overflow:auto; width:100px;";

		$p->add( $aI );
		$p->add(" &nbsp;|&nbsp; ");
		$p->add( $aII );

		$dI->add( $p );
		$dI->add( $dII );

		$fieldSet->add( $dI );
		$fieldSet->add( $this->arv->montaArvore( null, null, $this->padrao) );
//		// Div javascript
//		$dJS = new TElement('div');
//		$dJS->style = "display:none;";
//		$dJS->add($this->arv->montaArvore());

		// célula I
		$cell = $this->cellArvore;
		$cell->add( $fieldSet );
//		$cell->add( $dJS );
		$cell->valign = 'top';
//		$cell->width  = '30%';
		$cell->align  = 'center';
		$cell->style  = 'overflow:auto; width:'.$this->styleParam['tamDivArvore'].'%;';
	}

	private function questionario(){
		$arrObDados = (array) $this->ger->getDados();

		if(count($arrObDados)){
    		$this->tableQuestionario = new TTable;
    		$this->tableQuestionario->valign = 'top';
    		$this->cellQuestionario->add( $this->tableQuestionario );
		}

		foreach($arrObDados as $obDados){
			if ( method_exists($this, 'tela' . get_class((object)  $obDados ) )){
				$this->{'tela' . get_class((object)  $obDados ) }( $obDados );
				$this->nivel++;
			}
		}

		if ( $this->tipoTela != "questionario" ){
			self::questionarioCabecalho();
			self::questionarioRodape();
		}/*else{
			$this->tableQuestionario->style = "width : 100%";
		}*/
	}

	private function questionarioCabecalho(){

		$table = new TTable;
		$this->cellCabecalho->add($table);

		// linha I
		$row 			= $table->addRow();
		$row->bgcolor   = '#cccccc';
		// célula I
		$cell = $row->addCell('');

// 		$pergAnterior = $this->ger->anteriorPergunta();
// 		$pergProxima  = $this->ger->proximaPergunta();

// 		$pergAnterior = $this->ger->anteriorPerguntaGeral( $this->padrao);
// 		$pergProxima  = $this->ger->proximaPerguntaGeral( $this->padrao);

		// Div/Botão I
// 		$div = new TElement('div');
// 		$div->style = "float:left; width:30%; text-align: left;";
		//Botão
// 		$botao = new TElement('input');
// 		if ( $pergAnterior ){
// 			$botao->onclick = "javascript: quest.atualizaTela(" . $pergAnterior->perid . ");";
// 		}else{
// 			$botao->disabled = "disabled";
// 		}
// 		$botao->value = ('Anterior', true);
// 		$botao->type  = 'button';
// 		$div->add($botao);
// 		$cell->add($div);
// 		$cell->add("&nbsp;");

		//Botão Salvar Anterior
// 		$botao = new TElement('input');
// 		if ( $pergAnterior && !$this->preview){
// 			if( $this->desab == false || $this->preview ){
// 				$botao->disabled = "disabled";
// 				$botao->style = "display:none;";
// 			}else{
// 				$botao->onclick = "javascript: quest.salvar(" . $pergAnterior->perid . ");";
// 			}
// 		}else{
// 			$botao->disabled = "disabled";
// 			$botao->style = "display:none;";
// 		}
// 		$botao->value = ('Anterior', true);
// 		$botao->type  = 'button';
// 		$div->add($botao);

		// Div Central
		$div = new TElement('div');
		$div->style = "width:100%; text-align: center;";
		$cell->add($div);

		//barra de progresso
		$perguntas = $this->ger->buscaTotalRespostas();
		$div->add(barraProgresso($perguntas['respondidas'], $perguntas['total'], true, true, false, '90%', '15px', '13px'));

		//Botão
// 		$botao = new TElement('input');
// 		$botao->onclick  = '';
// 		$botao->value 	 = ('Salvar', true);
// //		$botao->type  	 = 'submit';
// 		$botao->type  	 = 'button';
// 		if( $this->desab == false || $this->preview){
// 			$botao->disabled = "disabled";
// 			$botao->style = "display:none;";
// 		}else{
// 			$botao->onclick  = 'quest.salvar(' . $this->ger->getPeridPai() . ');';
// 		}

// 		$div->add($botao);

// 		// Div/Botão II
// 		$div = new TElement('div');
// 		$div->style = "float:right; width:30%; text-align: right;";
// 		//Botão Salvar Próximo
// 		$botao = new TElement('input');
// 		if ( $pergProxima && !$this->preview){
// 			if( $this->desab == false || $this->preview ){
// 				$botao->disabled = "disabled";
// 				$botao->style = "display:none;";
// 			}else{
// 				$botao->onclick = "javascript: quest.salvar(" . $pergProxima->perid . ");";
// 			}
// 		}else{
// 			$botao->disabled = "disabled";
// 			$botao->style = "display:none;";
// 		}
// 		$botao->value = ('Próximo', true);
// 		$botao->type  = 'button';

// 		$div->add($botao);

		//Botão
// 		$botao = new TElement('input');
// 		if ( $pergProxima ){
// 			$botao->onclick = "javascript: quest.atualizaTela(" . $pergProxima->perid . ");";
// 		}else{
// 			$botao->disabled = "disabled";
// 		}
// 		$botao->value = ('Próximo', true);
// 		$botao->type  = 'button';
// 		$div->add($botao);
// 		$div->add("&nbsp;");
// 		$cell->add($div);

	}
	private function questionarioRodape(){

		$table = new TTable;
		$this->cellRodape->add($table);

		// linha I
		$row 			= $table->addRow();
		$row->bgcolor   = '#cccccc';
		// célula I
		$cell = $row->addCell('');

		$pergAnterior = $this->ger->anteriorPerguntaGeral($this->padrao);
		$pergProxima  = $this->ger->proximaPerguntaGeral($this->padrao);

		// Div/Botão I
		$div = new TElement('div');
		$div->style = "float:left; width:30%; text-align: left;";
		//Botão
		$botao = new TElement('input');
 		if ( $pergAnterior /* && ($this->preview || $this->desab == false) */){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergAnterior->perid . ");";
    		$botao->value = ('Anterior');
    		$botao->type  = 'button';
    		$div->add($botao);
		}else{
    		$div->add("&nbsp;");
		}

		//Botão
		$botao = new TElement('input');
		if ( $pergAnterior && !$this->preview){
			if( $this->desab == false || $this->preview ){
				$botao->disabled = "disabled";
				$botao->style = "display:none;";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergAnterior->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
			$botao->style = "display:none;";
		}
		$botao->value = ('Gravar e Anterior');
		$botao->type  = 'button';

		$div->add($botao);
		$cell->add($div);

		// Div/Botão II
		$div = new TElement('div');
		$div->style = "float: left; width:40%; text-align: center;";

		//Botão
		$botao = new TElement('input');
		$botao->onclick  = '';
		$botao->value 	 = ('Gravar');
//		$botao->type  	 = 'submit';
		$botao->type  	 = 'button';
		if( $this->desab == false || $this->preview){
			$botao->disabled = "disabled";
			$botao->style = "display:none;";
		}else{
			$botao->onclick  = 'quest.salvar(' . $this->ger->getPeridPai() . ');';
		}

		$div->add($botao);
        //Lógica de pagina extra
        if( $this->desab == true && !$this->preview){
            $grpid = false;
            if( is_array($_SESSION['questionario']['grupoMultiplo']) && !empty($_SESSION['questionario']['grupoMultiplo']) ){
                foreach( $_SESSION['questionario']['grupoMultiplo'] as $grupoMultiplo){
                    if($grupoMultiplo['perid'] == $this->ger->getPeridPai() && $grupoMultiplo['exibebotao'] == 't'){
                        $grpid = $grupoMultiplo['grpid'];
                         break;
                    }
                }
            }

//             if($grpid){
//                 $botao = new TElement('input');
//                 if( $this->desab == false || $this->habilitado == 'N' || $this->preview ){
//                     $botao->disabled = "disabled";
//                 }else{
//                     $botao->onclick = "quest.duplicaGrupo(" . $grpid . "); quest.atualizaTela(" . $this->ger->getPeridPai() . ");";
//                 }
//                 $botao->value = ('Nova Página');
//                 $botao->type  = 'button';
//                 $botao->id    = 'btnNovaPagina';
//                 $div->add($botao);
//             }
        }

		$cell->add($div);


		// Div/Botão III
		$div = new TElement('div');
		$div->style = "float:right; width:30%; text-align: right;";
		//Botão
		$botao = new TElement('input');
		if ( $pergProxima && !$this->preview){
			if( $this->desab == false || $this->preview ){
				$botao->disabled = "disabled";
				$botao->style = "display:none;";
			}else{
				$botao->onclick = "javascript: quest.salvar(" . $pergProxima->perid . ");";
			}
		}else{
			$botao->disabled = "disabled";
			$botao->style = "display:none;";
		}
		$botao->value = ('Gravar e Próximo');
		$botao->type  = 'button';

		$div->add($botao);

		//Botão
		$botao = new TElement('input');
 		if ( $pergProxima /*&& ($this->preview || $this->desab == false)*/){
			$botao->onclick = "javascript: quest.atualizaTela(" . $pergProxima->perid . ");";
    		$botao->value = ('Próximo');
    		$botao->type  = 'button';
    		$div->add($botao);
		}else{
    		$div->add("&nbsp;");
		}

		$cell->add($div);

        $this->addSubmeterFecharQuestionario($cell, $pergProxima);

	}

	private function addSubmeterFecharQuestionario(TElement $elem, $pergProxima){
	    // Adiciona botão para fechar o questionário
	    if(($this->urlFechar && !$this->preview && $this->desab) || is_null($pergProxima)){
	        $br = new TElement('br');
// 	        $elem->add($br);
// 	        $elem->add($br);


	        $div = new TElement('div');
	        $div->style = "width:100%; text-align: center;";

	        if(($this->urlFechar && !$this->preview && $this->desab)){
    	        $botao = new TElement('input');
    	        $botao->onclick = "javascript: quest.fecharQuestionario('" . $this->urlFechar . "', ".$this->ger->getPeridPai().");";
    	        $botao->title = ('Fechar questionário');
    	        $botao->value = ('Fechar');
    	        $botao->type  = 'button';
    	        $div->add($botao);
	        }

	        if(is_null($pergProxima) && !$this->preview && $this->desab){
//     	        $botao = new TElement('input');
//     	        $botao->onclick = "javascript: quest.submeterQuestionario(".$this->ger->getPeridPai().");";
//     	        $botao->value = ('Submeter');
//     	        $botao->type  = 'button';
//     	        $div->add($botao);
	        }

	        $elem->add($div);
	    }
	}

	private function telaQPergunta( QPergunta $obj){
		// Inclui pergunta ao JavaScript
		$pergPai = $this->ger->perguntaPai( array("perid" => $obj->perid) );

		$this->js->addPergunta($obj, ($pergPai instanceof QPergunta ? $pergPai->perid : "" ), $this->ger->itemPai($obj) );

		if ( $this->arrDivSubPergunta[$this->ger->itemPai($obj)] instanceof TElement ){
			$table = new TTable;
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->style = "margin-top:5px;";
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->add( $table );
		}else{
			$table = $this->tableQuestionario;
		}

		// linha I
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->pertitulo . ' ' . ($obj->perobrigatorio == "t" ? obrigatorio() : '' )  );
		$cell->class	= "SubTituloEsquerda";
		$cell->style	= "font-size: 18px; color: black;";
// 		$imagem 		= $obj->perajuda ? self::imgAjuda( $obj->perajuda ) : '';
		$cell->add( $imagem );

		// Caso exista regra adiciona hidden
		if ($obj->perregra){
			$hiddenRegra = new TElement('input');
			$hiddenRegra->name  = 'hiddenRegra';
			$hiddenRegra->id    = 'hiddenRegra';
			$hiddenRegra->value = self::buscarRespostaRegraPergunta($obj->perregra, ($obj->queid_atual ? $obj->queid_atual : $this->queid));
			$hiddenRegra->type  = 'hidden';
			$cell->add( $hiddenRegra );

			$perguntaReferenciada = explode(";", $obj->perregra);
			$pergunta = is_numeric($perguntaReferenciada[2]) ? $perguntaReferenciada[2] : current(explode(":", $perguntaReferenciada[2]));
			$obPerguntaReferenciada = new QPergunta($pergunta);
			$perguntaReferenciada = $obPerguntaReferenciada->getDados();
			$hiddenRegraTitulo = new TElement('input');
			$hiddenRegraTitulo->name  = 'hiddenRegraTitulo';
			$hiddenRegraTitulo->id    = 'hiddenRegraTitulo';
			$hiddenRegraTitulo->value = $this->arv->limitaString($perguntaReferenciada['pertitulo']);
			$hiddenRegraTitulo->type  = 'hidden';

			$cell->add( $hiddenRegraTitulo );
		}

		// linha II
		$row 			= $table->addRow();

 		switch ($obj->pertipo){
 			case 'RA':
 				$row->addCell( $this->campoRadio( $obj ) );
 				break;
 			case 'CK':
 				$row->addCell( $this->campoCheck( $obj ) );
 				break;
 			case 'TX':
 				$row->addCell( $this->campoText( $obj ) );
 				break;
 			case 'TA':
 				$row->addCell( $this->campoTextArea( $obj ) );
 				break;
 			case 'CB':
 				$cell = $row->addCell( $this->campoCombo( $obj ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'CM':
 				$cell = $row->addCell( $this->campoComboMultiplo( $obj ) );
 				$this->inserirDivSubPerg($cell, $obj);
 				break;
 			case 'DT':
 				$row->addCell( $this->campoData( $obj ) );
 				break;
 			case 'EXT':
 				$cell = $row->addCell( $this->campoExternoTabela( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
 			case 'EXF':
 				$cell = $row->addCell( $this->campoExternoFormulario( $obj ) );
 				$cell->add( $this->campoHidden( $obj ) );
 				break;
			case 'ARQ':
			    $row->addCell( $this->campoFile( $obj ) );
			    break;
			case 'MAP':
			    $row->addCell( $this->campoMapa($obj) );
			    break;
			case 'END':
			    $row->addCell( $this->campoEndereco($obj) );
			    break;
 		}
//		if ($obj->perid != 416 && $obj->perid != 417){
//			ver($obj, d);
//			ver($table,d);
//		}
// 		if($this->itemPergunta[$obj->perid]){
//
//			$resposta = $this->ger->trataResposta($this->ger->carregaResposta( $obj ));
//			if ( count($resposta) < 1 ){
//				$style = "display:none;";
// 			}else{
//				$style = "";
//			}
//			$itemPai = $this->ger->itemPai($obj);
//			if ( $itemPai && $this->subPergunta ){
//				$row = $this->divSubPergunta[$itemPai]->addRow();
//			}else{
// 				$row = $this->tableQuestionario->addRow();
//			}
//
// 			$row->id 	= "tr_subpergunta_" . $obj->perid;
// 			$row->style = $style;
//
// 			$cell 		= $row->addCell('');
// 			foreach($this->itemPergunta[$obj->perid] as $itpid => $itemPergunta){
// 				// linhas dos sub-itens
// 				$div 		= new TElement("div");
// 				$div->id 	= $itemPergunta;
// 				$div->add('');
//
// 				if ( count($resposta) > 0 && in_array($itpid, $resposta) ){
// 					$subPergTable 		    	  = new TTable;
//	 				$this->divSubPergunta[$itpid] = $subPergTable;
//					$div->add( $this->divSubPergunta[$itpid] );
// 				}
//	 			$cell->add( $div );
// 			}
//
// 			if ( is_null($this->subPergunta) ){
//				$this->subPergunta = true;
//				$this->nivel = 0;
//			}
// 		}
	}

	private function campoTextArea( QPergunta $obj ){
		$campo = new TTextArea("perg_{$obj->perid}");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
// 		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setHabil( $this->desab );

		return $campo;

	}

	private function campoText( QPergunta $obj ){
		$campo = new TText("perg[{$obj->perid}]");
 		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setMascara( $obj->permascara );

		//se foi definida alguma validação especial
		switch ($obj->pervalidacaoonblur){
			case 'EMAIL':
		    	$campo->setOnblur( " quest.obPerg.validarEmail(this); " );
		    	break;
		}

// 		$campo->setObrig( $obj->perobrigatorio == "t" );
//		dbg($this->desab);
		$campo->setHabil( $this->desab );

		return $campo;
	}

	private function campoFile( QPergunta $obj ){
		$campo = new TFile("perg[{$obj->perid}]", $obj->perdescricao == "t");

		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );

// 		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setExtensao( $obj->perextensao );
		$campo->setHabil( $this->desab );

		if($obj->perdescricao == "t"){
    		$campo->getDescricao()->setObrig( $obj->perdescricaoobrigatoria == "t" );
    		$campo->getDescricao()->setMaxlength( $obj->perqtdchardescricao );
		}

		return $campo;
	}

	private function campoMapa( QPergunta $obj ){
		$campo = new TMapa("perg[{$obj->perid}]");

		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );

		return $campo;
	}

	private function campoEndereco( QPergunta $obj ){
		$campo = new TEndereco("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );
		$campo->setExibirMapa(($obj->perexibemapa == 't' ? true : false));

		return $campo;
	}

	private function campoExternoTabela( QPergunta $obj ){
		$campo = new Montatabela( );
		$campo->montaNovaTabela( $obj->perid, $this->ger->getQrpid(), 90, 0, !$this->desab  );

		return $campo;

	}

	private function campoExternoFormulario( QPergunta $obj ){

		$campo = new CampoExternoControle( );
		$campo->montaNovoCampo( $obj->perid, $this->ger->getQrpid() );

		return $campo;

	}

	private function campoHidden( QPergunta $obj ){

		$hidden = new TElement('input');
		$hidden->type  = 'hidden';
		$hidden->value  = true;
		$hidden->name  = 'perg['.$obj->perid.']';
		$hidden->id    = 'perg['.$obj->perid.']';

		return $hidden;

	}

	private function campoData( QPergunta $obj ){
		$campo = new TData("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setLabel( $obj->perajuda );
// 		$campo->setObrig( $obj->perobrigatorio == "t" );
		$campo->setHabil( $this->desab );
		$perMascara = $obj->permascara ? $obj->permascara : "dd/mm/yyyy";
		$campo->setMascaraData($perMascara);

		return $campo;
	}

	private function campoCombo( QPergunta $obj ){

		$campo = new TCombo("perg[{$obj->perid}]");

		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$campo->setHabil( $this->desab );

		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
//			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
////				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
//				$this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
//			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;
	}

	private function campoComboMultiplo( QPergunta $obj ){

		$campo = new TCombo("perg[{$obj->perid}]");
		$campo->setMultiplo(true);
		$campo->setOpcao(null);
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setAcao("javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : this.value}, this)");
		$campo->setHabil( $this->desab );

		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo) );
		}
		return $campo;
	}

	private function campoRadio( QPergunta $obj ){

		$campo = new TRadioGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );

		$arrObItens = (array) $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick, $divSubPerg);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
//				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}else{
				$onclick = "javascript: quest.closeSubPerguntas({$obj->perid});";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;
	}

	private function campoCheck( QPergunta $obj ){

		$campo 		= new TCheckGroup("perg[{$obj->perid}]");
		$campo->setValue( $this->ger->trataResposta($this->ger->carregaResposta( $obj )) );
		$campo->setHabil( $this->desab );

		$arrObItens = $this->ger->carregaObItensPergunta($obj);
		foreach($arrObItens as $obItem ){
			unset($onclick, $divSubPerg);
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
//				$this->itemPergunta[$obj->perid][$obItem->itpid] = "linha_{$obj->perid}_{$obItem->itpid}";
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obj->perid}_{$obItem->itpid}");
				$onclick = "javascript: quest.buscaSubPergunta({\"perid\" : {$obj->perid}, \"itpid\" : {$obItem->itpid}}, this)";
			}
			$campo->setItens( array("codigo" => $obItem->itpid, "descricao" => $obItem->itptitulo, "onClick" => $onclick, "divSubPergunta" => $divSubPerg ));
		}
		return $campo;
	}

	private function telaQQuestionario($obj){
		if ( is_null($this->subPergunta) ){
			$table = $this->tableQuestionario;
		}/*else{
			$table = $this->divSubPergunta[$this->ger->itemPai($obj)];
		}*/
		$row 			= $table->addRow();
		$cell 			= $row->addCell( $obj->quetitulo. ' ' );
// 		$imagem 		= $obj->quedescricao ? self::imgAjuda( $obj->quedescricao ) : '';
		$cell->add( $imagem );
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
	}

	private function telaQGrupo($obj){

        if ( $this->arrDivSubPergunta[$this->ger->itemPai($obj)] instanceof TElement ){
            $table = new TTable;
			$this->arrDivSubPergunta[$this->ger->itemPai($obj)]->add( $table );
		}else{
			$table = $this->tableQuestionario;
		}

		$row 			= $table->addRow();
		$cell 			= $row->addCell( self::imgNivel() );
		$cell->class	= "SubTituloEsquerda";
		$cell->colspan 	= 2;
		$cell->add( $obj->grptitulo. ' ' );
// 		$imagem 		= $obj->grpajuda ? self::imgAjuda( $obj->grpajuda ) : '';
		$cell->add( $imagem );
	}

	private function criaDivSubPerg($itpid, $idDiv){
		$divSubPerg 	   = new TElement("div");
        $divSubPerg->id    = $idDiv;
        $divSubPerg->style = "display:none; margin-top:5px;";
        $divSubPerg->add('');

		$this->arrDivSubPergunta[$itpid] = $divSubPerg;

		return $divSubPerg;
	}

	private function inserirDivSubPerg(TElement $cell, QPergunta $obPerg){
		$arrObItens = $this->ger->carregaObItensPergunta($obPerg);
		foreach($arrObItens as $obItem ){
			if ($obItem->possuiPerguntaGrupoAssociado() > 0){
				$divSubPerg = $this->criaDivSubPerg($obItem->itpid , "linha_{$obPerg->perid}_{$obItem->itpid}");
				$cell->add( $divSubPerg );
			}
		}
	}

	private function buscarRespostaRegraPergunta($regra , $queid)
	{
		$regra 	 	= explode(";", $regra);
		$resp 		= new QResposta();
		$regra[2]   = $resp->pegaResposta($regra[2], $this->ger->getQrpid());
//		$regra[2]   = $resp->recuperaRespostaPorMatriz( $regra[2] , $queid);
//		dbg($regra);
		return implode(";", $regra);
	}

}
?>
