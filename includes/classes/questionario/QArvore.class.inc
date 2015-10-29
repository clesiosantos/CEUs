<?php
class QArvore{
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
    private $staticWhere;
    private $queid;
    private $preview;
    private $ger;
    private $dadoscondicao;

	public function __construct(Array $param = null){
		$this->s  = new Sistema();
		$this->q  = new QQuestionario();
		$this->g  = new QGrupo();
		$this->p  = new QPergunta();
		$this->it = new QItemPergunta();
		$this->perid 	 = $param['perid'];
		$this->qrpid 	 = $param['qrpid'];
		$this->relatorio = $param['relatorio'];
		$this->ger       = $param["gerencia_questionario"];
        $this->dadoscondicao = isset($param['dadoscondicao']) ? $param['dadoscondicao'] : array();


		$this->preview = $param['preview'];

		if ($param['tipoArvore'] == "acordo"){
			$this->questionario = $param['questionario'];
			self::detalheTipoArvore('acordo');
		}elseif ($param['tipoArvore'] == "oei"){
			$this->questionario = $param['questionario'];
			self::detalheTipoArvore('oei');
		}elseif ($param['tipoArvore'] == "regra"){
			$this->questionario = $param['questionario'];
			self::detalheTipoArvore('regra');
		}elseif ($param['questionario']){
			$this->questionario = $param['questionario'];
			self::detalheTipoArvore('resposta');
		}else{
			self::detalheTipoArvore('cadastro');
		}
	}

	function staticWhere($tipo, $where){
		$this->staticWhere[$tipo] = is_array($this->staticWhere[$tipo]) ? $this->staticWhere[$tipo] : array();
		array_push($this->staticWhere[$tipo], $where);
	}

	function blockIcon($typeBlock = 'all', $specifyBlock = 'all')
	{
		$typeBlock    = (array) $typeBlock;
		$specifyBlock = (array) $specifyBlock;

		//deixar os valores minúsculos
		$typeBlock = array_map('strtolower', $typeBlock);

		//verifica se existe 'all' em qualquer posição do $typeBlock
		if(in_array ('all', $typeBlock))
			$typeBlock = array('s', 'q', 'g', 'p', 'i');

		//verifica se existe 'all' em qualquer posição do $specifyBlock
		if(in_array ('all', $specifyBlock))
			$specifyBlock = array('all');

		foreach ( $typeBlock as $type ){
			if ( isset($this->img[$type]) ){
				if (strtolower($specifyBlock[0]) == 'all'){
					foreach ( $this->img[$type] as $index => $img ){
						$this->img[$type][$index] = '';
					}
				}else{
					foreach ( $specifyBlock as $index ){
						if ( isset( $this->img[$type][$index] ))
							$this->img[$type][$index] = '';
					}
				}
			}
		}
	}

	function detalheTipoArvore($tipoArvore = 'cadastro'){
		$this->tipoArvore = $tipoArvore;

		if ( $tipoArvore == 'cadastro' ){
			// Sistema
			$this->img['s']['open']	   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['incluirQ']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir novo Questionário").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroQuestionario&acao=A&sisid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">';
			$this->img['s']['eventoQ']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroQuestionario&acao=A&sisid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();";
			$this->img['s']['alterar']              = '';
			$this->img['s']['excluir']              = '';
			// Questionario
			$this->img['q']['open']                 = '../includes/dtree/img/base.gif';
			$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
			$this->img['q']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir nova Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&queid=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();">';
			$this->img['q']['eventoP']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&queid=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();";
			$this->img['q']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir novo Grupo de Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();">';
			$this->img['q']['eventoG']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();";
			$this->img['q']['preview']              = '<img src=\'/imagens/consultar.gif\' title="'.("Pré-visualizar o questionário").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/visualizarQuestionario&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=1000,height=700\'); w.focus();">';
			$this->img['q']['alterar']              = '<img src=\'/imagens/alterar.gif\' title="'.("Alterar Questionário").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroQuestionario&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">';
			$this->img['q']['excluir']              = '<img src=\'/imagens/excluir.gif\' title="'.("Excluir Questionário").'" onClick="apagar(\'%s&instrucao=excluir&queid=%s\', \''.("Deseja apagar o Questionário").' ( %s )?\',1) "/>';
			$this->img['q']['gera']                 = '';//<img src=\'/imagens/report.gif\' title="'.Translate::get("Gerar Script de Questionário", true).'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/geraScript&acao=A&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();">';
			$this->img['q']['imprime']              = '../imagens/print.png';
			// Grupo
			$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
			$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
			$this->img['g']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir nova Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=%s&queval=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();">';
			$this->img['g']['eventoP']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=%s&queval=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();";
			$this->img['g']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir novo Grupo de Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&gru_grpid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();">';
			$this->img['g']['eventoG']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&gru_grpid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();";
			$this->img['g']['alterar']              = '<img src=\'/imagens/alterar.gif\' title="'.("Alterar Grupo").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&grpid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();">';
			$this->img['g']['excluir']              = '<img src=\'/imagens/excluir.gif\' title="'.("Excluir Página").'" onClick="apagar(\'%s&instrucao=excluir&grpid=%s\', \''.("Deseja apagar o Grupo").' ( %s )?\') "/>';
			// Pergunta
			$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
			$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
			$this->img['p']['incluir']              = '';
			$this->img['p']['alterar']              = '<img src=\'/imagens/alterar.gif\' title="'.("Alterar Pergunta").'" onclick="janela(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=%s&queval=%s\',600,600,\'Janela1\');">';
//			$this->img['p']['alterar']              = '<img src=\'/imagens/alterar.gif\' title="'.Translate::get("Alterar Pergunta", true).'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();">';
			$this->img['p']['excluir']              = '<img src=\'/imagens/excluir.gif\' title="'.("Excluir Pergunta").'" onClick="apagar(\'%s&instrucao=excluir&perid=%s\', \''.("Deseja apagar a Pergunta").' ( %s )?\') "/>';
			// Item
			$this->img['i']['open']                 = '../imagens/ico_config.gif';
			$this->img['i']['close']    			= '../imagens/ico_config.gif';
			$this->img['i']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir nova Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&itpid=%s&queval=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();">';
			$this->img['i']['eventoP']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&itpid=%s&queval=%s\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();";
			$this->img['i']['incluirG']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir novo Grupo de Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&itpid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();">';
			$this->img['i']['eventoG']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroGrupo&acao=A&itpid=%s&queval=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=480,height=350\'); w.focus();";
			$this->img['i']['alterar'] 				= '';
			$this->img['i']['excluir'] 				= '';
		}elseif ($tipoArvore == 'acordo' || $tipoArvore == 'oei' || $tipoArvore == 'regra'){
			// Sistema
			$this->img['s']['open']                 = '../includes/dtree/img/globe.gif';
			$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['incluirQ']             = '';
			$this->img['s']['eventoQ']              = '';
			$this->img['s']['alterar']              = '';
			$this->img['s']['excluir']              = '';
			// Questionario
			$this->img['q']['open']                 = '../includes/dtree/img/base.gif';
			$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
			$this->img['q']['incluirP']             = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Incluir nova Pergunta").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&queid=%s&tp=acordo\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();">';
			$this->img['q']['eventoP']              = "w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&queid=%s&tp=acordo\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=600\'); w.focus();";
			$this->img['q']['incluirG']             = '';
			$this->img['q']['eventoG']             = '';
			$this->img['q']['alterar']              = '';
			$this->img['q']['excluir']              = '';
			$this->img['q']['imprime']              = '../imagens/print.png';
			// Grupo
			$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
			$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
			$this->img['g']['incluirP']             = '';//'<img src=\'/imagens/gif_inclui.gif\' title="'.Translate::get("Incluir nova Pergunta", true).'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&grpid=%s&tp=acordo&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=550\'); w.focus();">';
			$this->img['g']['eventoP']              = '';
			$this->img['g']['incluirG']             = '';
			$this->img['g']['eventoG']             = '';
			$this->img['g']['alterar']              = '';
			$this->img['g']['excluir']              = '';
			// Pergunta
			$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
			$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
			$this->img['p']['respondido']     		= '<img src=\'/imagens/check_p.gif\' title="'.("Pergunta respondida").'">';
			$this->img['p']['incluir']              = '';
			$this->img['p']['alterar']              = '<img src=\'/imagens/alterar.gif\' title="'.("Sugerir/Avaliar Item").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/alteracaoLegendaPergunta&acao=A&perid=%s&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">';
			$this->img['p']['excluir']              = '';
			// Item
			$this->img['i']['open']                 = '../imagens/ico_config.gif';
			$this->img['i']['close']    			= '../imagens/ico_config.gif';
			$this->img['i']['incluirP']             = '';
			$this->img['i']['eventoP']              = '';
			$this->img['i']['incluirG']             = '';
			$this->img['i']['eventoG']              = '';
			$this->img['i']['alterar'] 				= '<img src=\'/imagens/alterar.gif\' title="'.("Sugerir/Avaliar Item").'" onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/alteracaoLegendaItem&acao=A&itpid=%s&queid=%s\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=600,height=400\'); w.focus();">';

			$this->img['i']['excluir'] 				= '';
		}else{
			// Sistema
			$this->img['s']['open']                 = '../includes/dtree/img/globe.gif';
			$this->img['s']['close']   				= '../includes/dtree/img/globe.gif';
			$this->img['s']['incluirQ']             = '';
			$this->img['s']['eventoQ']              = '';
			$this->img['s']['alterar']              = '';
			$this->img['s']['excluir']              = '';
			// Questionario
			$this->img['q']['open']                 = '../includes/dtree/img/base.gif';
			$this->img['q']['close']  				= '../includes/dtree/img/base.gif';
			$this->img['q']['incluirP']             = '';
			$this->img['q']['eventoP']              = '';
			$this->img['q']['incluirG']             = '';
			$this->img['q']['eventorG']             = '';
			$this->img['q']['alterar']              = '';
			$this->img['q']['excluir']              = '';
			$this->img['q']['imprime']              = '../imagens/print.png';
			// Grupo
			$this->img['g']['open']                 = '../includes/dtree/img/folderopen.gif';
			$this->img['g']['close']  				= '../includes/dtree/img/folder.gif';
			$this->img['g']['incluirP']             = '';
			$this->img['g']['eventoP']              = '';
			$this->img['g']['incluirG']             = '';
			$this->img['g']['eventoG']              = '';
			$this->img['g']['alterar']              = '';
			$this->img['g']['excluir']              = '';
            $this->img['g']['excluirMultiplo']      = '<img src=\'/imagens/excluir.gif\' title="'.("Excluir Página").'"  onclick="apagarGrupoMultiplo( \'%s&instrucao=excluirMultiplo&ajax=1&grpid=%s\' ,\''.("Deseja excluir a página?").'\') " />';
            $this->img['g']['novo']                 = '<img src=\'/imagens/gif_inclui.gif\' title="'.("Nova página").'" onClick="quest.duplicaGrupo(%s, %s); "/>';

			// Pergunta
			$this->img['p']['open']                 = '../includes/dtree/img/page.gif';
			$this->img['p']['close']  				= '../includes/dtree/img/page.gif';
			$this->img['p']['respondido']     		= '<img src=\'/imagens/check_p.gif\' title="'.("Pergunta respondida").'">';
			$this->img['p']['incluir']              = '';
			$this->img['p']['alterar']              = '';
			$this->img['p']['excluir']              = '';
			// Item
			$this->img['i']['open']                 = '../imagens/ico_config.gif';
			$this->img['i']['close']    			= '../imagens/ico_config.gif';
			$this->img['i']['incluirP']             = '';
			$this->img['i']['eventoP']              = '';
			$this->img['i']['incluirG']             = '';
			$this->img['i']['eventoG']              = '';
			$this->img['i']['alterar'] 				= '';
			$this->img['i']['excluir'] 				= '';
		}
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

	function montaArvore( $sisid = null, $queid = null, $padrao = true ){
    	if ($this->tipoArvore == 'cadastro'){
    		if($queid)
    			$this->queid = $queid;

        	$this->arvore .= "arvore.add(0, -1, '".("Questionários")."');\n";
            $arrObSistema  = $this->s->listaObjeto(array('sisid' => $sisid), array('sisid', 'sisdsc'));

            foreach($arrObSistema as $obSistema){
				$this->arvore .= "arvore.add('sis_" . $obSistema->sisid . "', 0, '".$obSistema->sisdsc."', 'javascript:void(0)', '', '', '".$this->img['s']['open']."', '".$this->img['s']['close']."');\n";
				// Busca Questionarios associados
				$imgInclui = $this->img['s']['incluirQ'] ? sprintf($this->img['s']['incluirQ'], $obSistema->sisid) : '';
				$imgInclui = addslashes($imgInclui);
				$eventoQ   = $this->img['s']['eventoQ'] ? sprintf($this->img['s']['eventoQ'], $obSistema->sisid) : 'void(0);';

				$this->arvore .= "arvore.add('sis_imgInclui_" . $obSistema->sisid . "', 'sis_" . $obSistema->sisid . "', '" . $imgInclui . (" Adicionar Questionário") . "', 'javascript:{$eventoQ}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				self::montaArvoreQ($obSistema,$padrao);
			}
		}else{
			self::montaArvoreQ( $this->questionario,$padrao );
		}

		$dadosCondicao = str_replace('"', "'", serialize($this->dadoscondicao));

		return	'<script language="javascript" type="text/javascript" src="../includes/dtree/dtree.js"></script>
				<link rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">
				<script>
				arvore = new dTree( "arvore" );

				arvore.config.folderLinks = true;
				arvore.config.useIcons = true;
				arvore.config.useCookies = true;

				'.$this->arvore.'

				elemento = document.getElementById( "_arvore" );
				elemento.innerHTML = arvore;

				 /*
				 * adaptação de largura e controle de overflow de conteúdo para o internet explorer
				 */
				var dav = navigator.appVersion;
				IE = document.all ? true : false;
				IE6 = dav.indexOf( "MSIE 6.0" ) >= 0;
				IE7 = dav.indexOf( "MSIE 7.0" ) >= 0;
				if ( IE ) {
					width = document.body.offsetWidth;
					height = document.body.offsetHeight;
					document.getElementById( "bloco" ).style.width = ( width * 0.80 ) + "px";
				}
				arvore.openTo("per_'.$this->perid.'", true, false);
				</script>
				<script>
                function apagarGrupoMultiplo( url, mens ){
                    if ( confirm(mens) ) {
                        divCarregando();
						$.ajax({
						   type		: "POST",
                           async    : true,
						   url		: url,
						   success: function(data){
                               quest.atualizaTela(null, false, true);
                               alert(data);
						   }
						 });
                   }
                }

				function apagar (url,mens,questionario) {
					var msg;
					if (confirm(mens)) {

						$.ajax({
						   type		: "POST",
						   url		: url,
						   dataType	: "json",
						   success: function(data){
									if (!data.erro){
										window.location = window.location;
									}
									msg = data.msg;
									msg = data.msg.replace(/\n/, "\n");
									alert(msg);
						   }
						 });

//						$.post(url,{dataType:"json"}, function(data) {
//									alert(data);
//									if (!data.erro){
//										if (questionario == 1){
//											$("#sisfiltro").change();
//											$("#quefiltro").change();
//										}else{
//											$("#queid").change();
//										}
//									}
//									alert(data.msg);
//								});
						return true;
					}
					return false;
				}
				function imprimeQ(queid){
					var url = String(document.location).split(\'?\');
//					alert(url[0]+"?'.$this->relatorio.'&imprime=true&queid="+queid+"&qrpid='.$this->qrpid.'","page","toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600");
//					window.open(url[0]+"?'.$this->relatorio.'&imprime=true&queid="+queid+"&qrpid='.$this->qrpid.'","page","toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600");
					var janela = window.open(url[0]+"?modulo=principal/relatorioQuestionario&acao=A&queid="+queid+"'.($this->qrpid ? '&qrpid='.$this->qrpid : '')."&dadoscondicao={$dadosCondicao}".'","page","toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600");
			        janela.focus();
				}
				</script>';
    }

	function montaArvoreQ( $obPai, $padrao = true ){
		$arWhere = $this->staticWhere['questionario'];
		if (is_array($arWhere)){
			array_push($arWhere, $obPai);
		}else{
			$arWhere = array($obPai);
		}
		// Filtra Questionarios
		if($this->queid)
			$arWhere[] = "queid = {$this->queid}";

		$arrObQuestionario = (array) $this->q->listaObjeto( $arWhere, array('queid', 'quetitulo') );

		foreach($arrObQuestionario as $obQuestionario){
			$queid = $obQuestionario->queid;
			$this->queid = $queid;
			if ($this->tipoArvore == 'cadastro'){
				$idPai = "sis_" . $obPai->sisid;
				//Perguntas

				$imgInclui = $this->img['q']['incluirP'] ? sprintf($this->img['q']['incluirP'], $obQuestionario->queid) : '';
				$imgInclui = addslashes($imgInclui);

				$eventoP = $this->img['q']['eventoP'] ? sprintf($this->img['q']['eventoP'], $obQuestionario->queid) : 'void(0);';

				$addP = "arvore.add('que_imgInclui_per_" . $obQuestionario->queid . "', 'que_" . $obQuestionario->queid . "', '" . $imgInclui . (" Adicionar Pergunta") . "', 'javascript:{$eventoP}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				//Grupos
				$imgInclui = $this->img['q']['incluirG'] ? sprintf($this->img['q']['incluirG'], $obQuestionario->queid) : '';
				$imgInclui = addslashes($imgInclui);

				$eventoG = $this->img['q']['eventoG'] ? sprintf($this->img['q']['eventoG'], $obQuestionario->queid) : 'void(0);';

				$addG = "arvore.add('que_imgInclui_gru_" . $obQuestionario->queid . "', 'que_" . (($obQuestionario->queid)) . "', '" . $imgInclui . (" Adicionar Grupo de Pergunta") . "', 'javascript:{$eventoG}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}else{
				$idPai = "-1";
			}


			if(($this->tipoArvore == 'acordo') && ($this->q->pegaEstadoQuestionario($this->queid) != WF_VALIDACAO_FINALIZADA)){
				//Perguntas
				$imgInclui = $this->img['q']['incluirP'] ? sprintf($this->img['q']['incluirP'], $obQuestionario->queid) : '';
				$imgInclui = addslashes($imgInclui);

				$eventoP = $this->img['q']['eventoP'] ? sprintf($this->img['q']['eventoP'], $obQuestionario->queid) : 'void(0);';

				$addP = "arvore.add('que_imgInclui_per_" . $obQuestionario->queid . "', 'que_" . $obQuestionario->queid . "', '" . $imgInclui . (" Adicionar Pergunta") . "', 'javascript:{$eventoP}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
//
//				//Perguntas
//				$imgInclui = $this->img['g']['incluirP'] ? sprintf($this->img['g']['incluirP'], $obGrupoPergunta->grpid, $this->queid) : '';
//				$imgInclui = addslashes($imgInclui);
//				$addP = "arvore.add('grp_imgInclui_per_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . Translate::get(" Adicionar Pergunta", true) . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}

			$imgGeraScript = $this->img['q']['gera'] ? sprintf($this->img['q']['gera'], $obQuestionario->queid) : '';
			$imgGeraScript = addslashes($imgGeraScript);
			$imgPreview = $this->img['q']['preview'] ? sprintf($this->img['q']['preview'], $obQuestionario->queid) : '';
			$imgPreview = addslashes($imgPreview);
			$imgAltera = $this->img['q']['alterar'] ? sprintf($this->img['q']['alterar'], $obQuestionario->queid) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['q']['excluir'] ? sprintf($this->img['q']['excluir'], $_SERVER['REQUEST_URI'], $obQuestionario->queid, $obQuestionario->quetitulo) : '';
			$imgExclui = addslashes($imgExclui);


			$this->arvore .= "arvore.add('que_" . $obQuestionario->queid . "', '$idPai', '$imgPreview $imgGeraScript $imgAltera $imgExclui " . self::limitaString($obQuestionario->quetitulo) . "', 'javascript:void(0)', '', '', '".$this->img['q']['open']."', '".$this->img['q']['close']."');\n";

            $this->arvore .= $addP;
			$this->arvore .= $addG;
			self::montaArvoreGeral($obQuestionario , $padrao);

//			// Busca Perguntas associadas
//			self::montaArvoreP($obQuestionario);
//
//			// Busca Grupo Pergunta associadas
//			self::montaArvoreG($obQuestionario);
		}
		if( $this->relatorio != 'N'){
			$this->arvore .= "arvore.add('imprimir', '$idPai', '&nbsp;". ("Imprimir Questionário")."', 'javascript:imprimeQ(".$queid.")', '', '', '".$this->img['q']['imprime']."', '".$this->img['q']['imprime']."');\n";
		}
	}

    function montaArvoreGeral( $obPai, $padrao = true ){
		if (get_class((object) $obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class((object) $obPai) == 'QGrupo'){
			$idPai = 'grp_' . $obPai->grpid;
		}elseif (get_class((object) $obPai) == 'QItemPergunta'){
			$idPai = 'itp_' . $obPai->itpid;
		}else{
			die('Erro: montaArvoreGeral falta OBJETO do parametro $obPai.');
		}

        // Filtra perguntas vinculadas
		$arrItens = (array) $this->q->listaObjetoGeral(array( $obPai ), $this->qrpid , $padrao);

        foreach($arrItens as $item){
            if($item['tipoitem'] == 'g'){
                if($this->ger && $this->ger->realizarVerificacaoGrupo( $item['id'] ) !== true){
                    continue;
                }

                $obGrupoPergunta = new QGrupo($item['id']);
                if ($this->tipoArvore == 'cadastro'){
                    //Perguntas
                    $imgInclui = $this->img['g']['incluirP'] ? sprintf($this->img['g']['incluirP'], $obGrupoPergunta->grpid, $this->queid) : '';
                    $imgInclui = addslashes($imgInclui);

                    $eventoP = $this->img['g']['eventoP'] ? sprintf($this->img['g']['eventoP'], $obGrupoPergunta->grpid, $this->queid) : 'void(0);';

                    $addP = "arvore.add('grp_imgInclui_per_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . (" Adicionar Pergunta") . "', 'javascript: {$eventoP}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
                    //Grupos
                    $imgInclui = $this->img['g']['incluirG'] ? sprintf($this->img['g']['incluirG'], $obGrupoPergunta->grpid, $this->queid) : '';
                    $imgInclui = addslashes($imgInclui);

                    $eventoG = $this->img['g']['eventoG'] ? sprintf($this->img['g']['eventoG'], $obGrupoPergunta->grpid, $this->queid) : 'void(0);';

                    $addG = "arvore.add('grp_imgInclui_gru_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . (" Adicionar Grupo de Pergunta") . "', 'javascript:{$eventoG}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
                }
                $imgAltera = $this->img['g']['alterar'] ? sprintf($this->img['g']['alterar'], $obGrupoPergunta->grpid, $this->queid ) : '';
                $imgAltera = addslashes($imgAltera);
                $imgExclui = $this->img['g']['excluir'] ? sprintf($this->img['g']['excluir'], $_SERVER['REQUEST_URI'], $obGrupoPergunta->grpid, $obGrupoPergunta->grptitulo) : '';
                $imgExclui = addslashes($imgExclui);
                $imgNovo   = $this->img['g']['novo'] ? sprintf($this->img['g']['novo'], $obGrupoPergunta->grpid, $this->ger->getPeridPai()) : '';
                $imgNovo   = addslashes($imgNovo);

                //montando a imagem de excluir grupos multiplos
                $imgExcluiGrupo = '';
                $imgNovaPagina  = '';
                if( !$this->preview && $this->img['g']['excluirMultiplo'] ){
                    if( is_array($_SESSION['questionario']['grupoMultiplo']) && !empty($_SESSION['questionario']['grupoMultiplo']) ){
                        foreach( $_SESSION['questionario']['grupoMultiplo'] as $grupoMultiplo){
                            if($grupoMultiplo['grpid_real'] == $obGrupoPergunta->grpid && $grupoMultiplo['grpid'] != $grupoMultiplo['grpid_real']){
                                $imgExcluiGrupo = sprintf( $this->img['g']['excluirMultiplo'] , $_SERVER['REQUEST_URI'], $obGrupoPergunta->grpid );
                                break;
                            }
                        }
                    }
                    $imgExcluiGrupo = addslashes($imgExcluiGrupo);
                }

                if(!$this->preview && $this->img['g']['novo']){
                    if($obGrupoPergunta->grppermitemultiplo == 't'){
                        $imgNovaPagina = $imgNovo;
                        foreach( $_SESSION['questionario']['grupoMultiplo'] as $grupoMultiplo){
                            if(($grupoMultiplo['grpid_real'] == $obGrupoPergunta->grpid && $grupoMultiplo['grpid'] != $grupoMultiplo['grpid_real'])
		                        || $grupoMultiplo['exibebotao'] == 'f'){
                                    $imgNovaPagina = "";
                                break;
                            }
                        }
                    }
                }

                $this->arvore .= "arvore.add('grp_" . $obGrupoPergunta->grpid . "', '{$idPai}', \"$imgAltera $imgExclui $imgNovaPagina $imgExcluiGrupo " . self::limitaString($obGrupoPergunta->grptitulo) . " \", 'javascript:void(0)', '', '', '". $this->img['g']['close'] ."', '". $this->img['g']['open'] ."');\n";

                $this->arvore .= $addP;

                // Busca Grupo Pergunta associadas
                if(!(get_class((object) $obPai) == 'ItemPergunta')){//Se for filho de um item então não inclui mais grupos!
                    $this->arvore .= $addG;
                }

                self::montaArvoreGeral($obGrupoPergunta,$padrao);

            }else if($item['tipoitem'] == 'p'){
                if($this->ger && $this->ger->realizarVerificacaoPergunta( $item['id'] ) !== true){
                    continue;
                }

                $obPergunta = new QPergunta($item['id']);

                $obPergunta->pertitulo = str_replace(array("\n", "\t", "\r"), "", $obPergunta->pertitulo);
                $imgAltera = $this->img['p']['alterar'] ? sprintf($this->img['p']['alterar'], $obPergunta->perid, $this->queid) : '';
                $imgAltera = addslashes($imgAltera);
                $imgExclui = $this->img['p']['excluir'] ? sprintf($this->img['p']['excluir'], $_SERVER['REQUEST_URI'], $obPergunta->perid, $obPergunta->pertitulo) : '';
                $imgExclui = addslashes($imgExclui);

                $titulo = '<label title="'.("Código:").' '.$obPergunta->perid.'" >'.self::limitaString($obPergunta->pertitulo.'<label>');

                if($this->tipoArvore == 'resposta'){
                    $titulo = self::limitaString($obPergunta->pertitulo);
                    if(!$this->preview){
                        $arrPerguntaResp = $this->p->perguntaRespondida($obPergunta->perid, $this->qrpid);
                        if($arrPerguntaResp){
                            $imgResp = $this->img['p']['respondido'];
                            $imgResp = addslashes($imgResp);
                        } else {
                            $imgResp = '';
                        }
                    }
                    if($this->perid == $obPergunta->perid){
                        $negrito = "<b>";
                        $negritoF = "</b>";
                    }
                    $this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgResp $imgAltera $imgExclui $negrito $titulo $negritoF', 'javascript: quest.atualizaTela(" . $obPergunta->perid . ");', '".("Clique para abrir a pergunta")."', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                    $negrito = "";
                    $negritoF = "";
                    if(!$this->preview)
                        $arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );
                    if($arrItemResp && ($obPergunta->percarregaitens == 't')){
                        self::montaArvoreI($obPergunta, $arrItemResp);
                    }
                } else {
                    $tipoArv = array("acordo", "oei");
                    $javascriptFunc = "javascript:void(0)";

                    if(in_array($this->tipoArvore, $tipoArv) && self::pendenciaLegenda($obPergunta->perid) || ($this->tipoArvore == "regra" && in_array($obPergunta->pertipo, array("TX", "EXT")))){
                        $negrito = "<b>";
                        $negritoF = "</b>";
                    }

                    if ($this->tipoArvore == "acordo" && self::legendaAprovada($obPergunta->perid)){
                        $imgAltera = "";
                    }

                    if ($this->tipoArvore == "oei" && !self::pendenciaLegenda($obPergunta->perid)){
                        $imgAltera = "";
                    }

                    if(in_array($this->tipoArvore, $tipoArv) && ($this->q->pegaEstadoQuestionario($this->queid) == WF_VALIDACAO_FINALIZADA))
                        $this->blockIcon();


                    if ($this->tipoArvore == "regra"){
                        $titulo = self::limitaString($obPergunta->pertitulo);
                        $imgAltera = "";
    //					$javascriptFunc = "javascript:definePergunta({$obPergunta->perid})";
                        $peridTmp = $obPergunta->perid_matriz ? $obPergunta->perid_matriz : $obPergunta->perid;
                        if (in_array($obPergunta->pertipo, array("TX", "EXT"))){
                            if ($obPergunta->pertipo == 'TX'){
                                $javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}}\')";
                                $arvoreEscolha  = "arvore.add('".$obPergunta->perid."_item1', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}}</b> Valor respondido pela unidade', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                                $javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:totalgeral}\')";
                                $arvoreEscolha  .= "arvore.add('".$obPergunta->perid."_item2', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:totalgeral}</b> Total dos valores respondidos no acordo', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                            }else{
                                $javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:total}\')";
                                $arvoreEscolha  = "arvore.add('".$obPergunta->perid."_item1', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:total}</b> Valor respondido pela unidade', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                                $javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:totalgeral}\')";
                                $arvoreEscolha  .= "arvore.add('".$obPergunta->perid."_item2', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:totalgeral}</b> Total dos valores respondidos no acordo', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                            }
                            $javascriptFunc = "javascript:void(0);";
                        }else{
                            $arvoreEscolha = "";
                            $javascriptFunc = "javascript:alert(\'Os descritores só podem utilizar campos do tipo texto e tabela. Estes estão em negrito na árvore.\');";
                        }

                    }
                    $this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui $negrito $titulo $negritoF', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
                    $this->arvore .= $arvoreEscolha;
                    if($obPergunta->percarregaitens == 't')
                        self::montaArvoreI($obPergunta);
                    $negrito = "";
                    $negritoF = "";
                }

            }
        }
    }

	function montaArvoreG( $obPai ){
		if (get_class((object) $obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class((object) $obPai) == 'QGrupo'){
			$idPai = 'grp_' . $obPai->grpid;
		}elseif (get_class((object) $obPai) == 'QItemPergunta'){
			$idPai = 'itp_' . $obPai->itpid;
		}else{
			die('Erro: montaArvoreG falta OBJETO do parametro $obPai.');
		}
		// Filtra Grupos vinculadas
		$arrObGrupoPergunta = (array) $this->g->listaObjeto(array( $obPai ), array("grpid, grptitulo, grpordem"));

		foreach ($arrObGrupoPergunta as $obGrupoPergunta){
			if ($this->tipoArvore == 'cadastro'){
				//Perguntas
				$imgInclui = $this->img['g']['incluirP'] ? sprintf($this->img['g']['incluirP'], $obGrupoPergunta->grpid, $this->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$eventoP   = $this->img['g']['eventoP'] ? sprintf($this->img['g']['eventoP'], $obGrupoPergunta->grpid, $this->queid) : 'void(0);';
				$addP = "arvore.add('grp_imgInclui_per_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . (" Adicionar Pergunta") . "', 'javascript:{$eventoP}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";

				//Grupos
				$imgInclui = $this->img['g']['incluirG'] ? sprintf($this->img['g']['incluirG'], $obGrupoPergunta->grpid, $this->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$eventoG   = $this->img['g']['eventoG'] ? sprintf($this->img['g']['eventoG'], $obGrupoPergunta->grpid, $this->queid) : 'void(0);';

				$addG = "arvore.add('grp_imgInclui_gru_" . $obGrupoPergunta->grpid . "', 'grp_" . $obGrupoPergunta->grpid . "', '" . $imgInclui . (" Adicionar Grupo de Pergunta") . "', 'javascript:{$eventoG}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}

			$imgAltera = $this->img['g']['alterar'] ? sprintf($this->img['g']['alterar'], $obGrupoPergunta->grpid, $this->queid ) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['g']['excluir'] ? sprintf($this->img['g']['excluir'], $_SERVER['REQUEST_URI'], $obGrupoPergunta->grpid, $obGrupoPergunta->grptitulo) : '';
			$imgExclui = addslashes($imgExclui);
			$this->arvore .= "arvore.add('grp_" . $obGrupoPergunta->grpid . "', '{$idPai}', '$imgAltera $imgExclui " . self::limitaString($obGrupoPergunta->grptitulo) . "', 'javascript:void(0)', '', '', '". $this->img['g']['close'] ."', '". $this->img['g']['open'] ."');\n";

			// Busca Perguntas associadas
			$this->arvore .= $addP;
			self::montaArvoreP($obGrupoPergunta);

			// Busca Grupo Pergunta associadas
			if(!(get_class((object) $obPai) == 'ItemPergunta')){//Se for filho de um item então não inclui mais grupos!
				$this->arvore .= $addG;
				self::montaArvoreG($obGrupoPergunta);
			}

		}

	}

	function montaArvoreP( $obPai ){

		if (get_class((object) $obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (get_class((object) $obPai) == 'QGrupo'){
			$idPai = 'grp_' . $obPai->grpid;
		}elseif (get_class((object) $obPai) == 'QItemPergunta'){
			$idPai = 'itp_' . $obPai->itpid;
		}else{
			die('Erro: MontaArvoreP falta OBJETO do parametro $obPai.');
		}
		// Filtra perguntas vinculadas
		$arrObPergunta = (array) $this->p->listaObjeto(array( $obPai ), array("perid, pertitulo, perordem, perid_matriz, pertipo, percarregaitens"));
		foreach ($arrObPergunta as $obPergunta){
			$obPergunta->pertitulo = str_replace(array("\n", "\t", "\r"), "", $obPergunta->pertitulo);
			$imgAltera = $this->img['p']['alterar'] ? sprintf($this->img['p']['alterar'], $obPergunta->perid, $this->queid) : '';
			$imgAltera = addslashes($imgAltera);
			$imgExclui = $this->img['p']['excluir'] ? sprintf($this->img['p']['excluir'], $_SERVER['REQUEST_URI'], $obPergunta->perid, $obPergunta->pertitulo) : '';
			$imgExclui = addslashes($imgExclui);

			$titulo = '<label title="'.("Código:").' '.$obPergunta->perid.'" >'.self::limitaString($obPergunta->pertitulo.'<label>');

			if($this->tipoArvore == 'resposta'){
				$titulo = self::limitaString($obPergunta->pertitulo);
				if(!$this->preview){
					$arrPerguntaResp = $this->p->perguntaRespondida($obPergunta->perid, $this->qrpid);
					if($arrPerguntaResp){
						$imgResp = $this->img['p']['respondido'];
						$imgResp = addslashes($imgResp);
					} else {
						$imgResp = '';
					}
				}
				if($this->perid == $obPergunta->perid){
					$negrito = "<b>";
					$negritoF = "</b>";
				}
				$this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgResp $imgAltera $imgExclui $negrito $titulo $negritoF', 'javascript: quest.atualizaTela(" . $obPergunta->perid . ");', '".("Clique para abrir a pergunta")."', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
				$negrito = "";
				$negritoF = "";
				if(!$this->preview)
					$arrItemResp = $this->p->perguntaRespondidaComItem( $obPergunta->perid, $this->qrpid );
				if($arrItemResp && ($obPergunta->percarregaitens == 't')){
					self::montaArvoreI($obPergunta, $arrItemResp);
				}
			} else {
				$tipoArv = array("acordo", "oei");
				$javascriptFunc = "javascript:void(0)";

				if(in_array($this->tipoArvore, $tipoArv) && self::pendenciaLegenda($obPergunta->perid) || ($this->tipoArvore == "regra" && in_array($obPergunta->pertipo, array("TX", "EXT")))){
					$negrito = "<b>";
					$negritoF = "</b>";
				}

				if ($this->tipoArvore == "acordo" && self::legendaAprovada($obPergunta->perid)){
					$imgAltera = "";
				}

				if ($this->tipoArvore == "oei" && !self::pendenciaLegenda($obPergunta->perid)){
					$imgAltera = "";
				}

				if(in_array($this->tipoArvore, $tipoArv) && ($this->q->pegaEstadoQuestionario($this->queid) == WF_VALIDACAO_FINALIZADA))
					$this->blockIcon();


				if ($this->tipoArvore == "regra"){
					$titulo = self::limitaString($obPergunta->pertitulo);
					$imgAltera = "";
//					$javascriptFunc = "javascript:definePergunta({$obPergunta->perid})";
					$peridTmp = $obPergunta->perid_matriz ? $obPergunta->perid_matriz : $obPergunta->perid;
					if (in_array($obPergunta->pertipo, array("TX", "EXT"))){
						if ($obPergunta->pertipo == 'TX'){
							$javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}}\')";
							$arvoreEscolha  = "arvore.add('".$obPergunta->perid."_item1', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}}</b> Valor respondido pela unidade', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
							$javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:totalgeral}\')";
							$arvoreEscolha  .= "arvore.add('".$obPergunta->perid."_item2', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:totalgeral}</b> Total dos valores respondidos no acordo', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
						}else{
							$javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:total}\')";
							$arvoreEscolha  = "arvore.add('".$obPergunta->perid."_item1', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:total}</b> Valor respondido pela unidade', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
							$javascriptFunc = "javascript:definePergunta(\'{{$peridTmp}:totalgeral}\')";
							$arvoreEscolha  .= "arvore.add('".$obPergunta->perid."_item2', 'per_" . $obPergunta->perid . "', '<b>{{$peridTmp}:totalgeral}</b> Total dos valores respondidos no acordo', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
						}
						$javascriptFunc = "javascript:void(0);";
					}else{
						$arvoreEscolha = "";
						$javascriptFunc = "javascript:alert(\'Os descritores só podem utilizar campos do tipo texto e tabela. Estes estão em negrito na árvore.\');";
					}

				}
				$this->arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui $negrito $titulo $negritoF', '$javascriptFunc', '', '', '". $this->img['p']['close'] ."', '". $this->img['p']['open'] ."');\n";
				$this->arvore .= $arvoreEscolha;
				if($obPergunta->percarregaitens == 't')
					self::montaArvoreI($obPergunta);
				$negrito = "";
				$negritoF = "";
			}
		}
	}

	function montaArvoreI( $obPai, $arrItemResp = null ){

		if (get_class((object) $obPai) == 'QPergunta'){
			$idPai = 'per_' . $obPai->perid;
		}else{
			die('Erro: MontaArvoreI falta OBJETO do parametro $obPai.');
		}
		$where[0] = $obPai;
		if($arrItemResp[0]){
			$where['itpid'] = $arrItemResp;
		}
		// Filtra perguntas vinculadas
		$arrObItPergunta = (array) $this->it->listaObjeto($where, array("itpid, itptitulo"));
		foreach ($arrObItPergunta as $obItPergunta){
			$imgAltera = $this->img['i']['alterar'] ? sprintf($this->img['i']['alterar'], $obItPergunta->itpid , $this->queid) : '';
			$imgAltera = addslashes($imgAltera);

			if ($this->tipoArvore == 'cadastro'){
				//Perguntas

				$imgInclui = $this->img['i']['incluirP'] ? sprintf($this->img['i']['incluirP'], $obItPergunta->itpid, $this->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$eventoP   = $this->img['i']['eventoP'] ? sprintf($this->img['i']['eventoP'], $obItPergunta->itpid, $this->queid) : 'void(0);';
				$addP      = "arvore.add('itp_imgInclui_per_" . $obItPergunta->itpid . "', 'itp_" . $obItPergunta->itpid . "', '" . $imgInclui . (" Adicionar Pergunta") . "', 'javascript:{$eventoP}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				//Grupos
				$imgInclui = $this->img['i']['incluirG'] ? sprintf($this->img['i']['incluirG'], $obItPergunta->itpid, $this->queid) : '';
				$imgInclui = addslashes($imgInclui);
				$eventoG   = $this->img['i']['eventoG'] ? sprintf($this->img['i']['eventoG'], $obItPergunta->itpid, $this->queid) : 'void(0);';
				$addG      = "arvore.add('itp_imgInclui_gru_" . $obItPergunta->itpid . "', 'itp_" . $obItPergunta->itpid . "', '" . $imgInclui . (" Adicionar Grupo de Pergunta") ."', 'javascript:{$eventoG}', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
			}

			if( self::pendenciaLegenda(null, $obItPergunta->itpid)){
				$negrito = "<b>";
				$negritoF = "</b>";
			}

			if ($this->tipoArvore == "acordo" && self::legendaItemAprovada($obItPergunta->itpid)){
				$imgAltera = "";
			}

//			if ($this->tipoArvore == "oei" && !self::legendaItemAprovada($obItPergunta->itpid)){
//				$imgAltera = "";
//			}
			if ($this->tipoArvore == "oei" && !self::pendenciaLegenda(null, $obItPergunta->itpid)){
				$imgAltera = "";
			}

			if ($this->tipoArvore == "regra"){
					$imgAltera = "";
			}

			$this->arvore .= "arvore.add('itp_" . $obItPergunta->itpid . "', '{$idPai}', '$imgAltera $imgExclui $negrito" .  self::limitaString($obItPergunta->itptitulo) . "$negritoF', 'javascript:void(0)', '', '', '". $this->img['i']['close'] ."', '". $this->img['i']['open'] ."');\n";

//			// Busca Perguntas associadas
//			$verifica = $this->it->verificaGrupo( $obItPergunta->itpid );
//			if(!($verifica)){
//				$this->arvore .= $addP;
//				self::montaArvoreP($obItPergunta);
//			}
//
//			// Busca Grupo Pergunta associadas
//			unset( $verifica );
//			$verifica = $this->it->verificaPergunta( $obItPergunta->itpid );
//			if(!($verifica)){
//				$this->arvore .= $addG;
//				self::montaArvoreG($obItPergunta);
//			}

            $this->arvore .= $addP;
            $this->arvore .= $addG;
            self::montaArvoreGeral($obItPergunta);

            $negrito = "";
			$negritoF = "";
		}
	}

	private function pendenciaLegenda($pergunta, $item=null)
	{
		$legenda = new QLegendaPergunta();
		if($pergunta)
			return $legenda->verificarPendencia($pergunta);
		if($item)
			return $legenda->verificarPendencia(null, null, $item);
	}

	private function legendaAprovada($pergunta)
	{
		$legenda = new QLegendaPergunta();
		return $legenda->verificarAprovado($pergunta);
	}

	private function legendaItemAprovada($item)
	{
		$legenda = new QLegendaPergunta();
		return $legenda->verificarItemAprovado($item);
	}
}
