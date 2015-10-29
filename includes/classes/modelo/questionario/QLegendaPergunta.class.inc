<?php

class QLegendaPergunta extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.legendapergunta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "lgpid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'lgpid' => null,
									  	'leg_lgpid' => null,
									  	'perid' => null,
									  	'lgptitulo' => null,
									  	'lgpsituacao' => null,
									  	'usucpfsugestao' => null,
									  	'usucpfavaliador' => null,
    									'queid' => null,
    									'itpid' => null,
									  );

	public function verificarPendencia($pergunta = null, $questionario = null, $item = null)
	{
		$where = array();
		if ($pergunta){
			$perid   = $pergunta instanceof QPergunta ? $pergunta->perid : $pergunta;
			$where[] = "perid = $perid";
		}

		if ($questionario){
			$queid   = $questionario instanceof QQuestionario ? $questionario->queid : $questionario;
			$where[] = "queid = $queid";
		}

		if ($item){
			$itpid   = $item instanceof QItemPergunta ? $item->itpid : $item;
			$where[] = "itpid = $itpid";
		}

		$sql = "SELECT
					COUNT(lgpid) as pendencia
				FROM
					questionario.legendapergunta
				WHERE
					" . (count($where) ? implode(" AND ", $where) : "") . "
					AND lgpsituacao = 'V'";
		$pendencia = $this->pegaUm($sql);

		return $pendencia > 0;
	}

	public function verificarAprovado($pergunta)
	{
		$perid = get_class((object) $pergunta) == "QPergunta" ? $pergunta->perid : $pergunta;

		$sql = "SELECT
					COUNT(lgpid) as pendencia
				FROM
					questionario.legendapergunta
				WHERE
					lgpsituacao = 'A'
					AND perid = $perid";
		$pendencia = $this->pegaUm($sql);

		return $pendencia > 0;
	}

	public function verificarItemAprovado($item)
	{
		$itpid = get_class((object) $item) == "QItemPergunta" ? $item->itpid : $item;

		$sql = "SELECT
					COUNT(lgpid) as pendencia
				FROM
					questionario.legendapergunta
				WHERE
					lgpsituacao = 'A'
					AND itpid = $item";

		$pendencia = $this->pegaUm($sql);

		return $pendencia > 0;
	}

	public function buscarTodasPendencia($queid)
	{
		$sql = "SELECT
					leg.lgptitulo || '&nbsp' AS lgptitulo,

					CASE
						WHEN leg.itpid::varchar != '' THEN itp.itptitulo || '&nbsp'
						WHEN leg.perid::varchar != '' THEN per.pertitulo || '&nbsp'
					END AS pertitulo,
					usu.usunome,
					CASE
						WHEN per.persugerido = FALSE THEN '<center><FONT COLOR=\'#FF0000\'> ". ("Não") ." </FONT></center>'
						WHEN per.persugerido = TRUE	 THEN '<center><FONT COLOR=\'#006400\'> ". ("Sim") ." </FONT></center>'
						ELSE '<center><FONT COLOR=\'#FF0000\'> ". ("Não") ." </FONT></center>'
					END AS persugerido,
					leg.perid,
					leg.queid,
					CASE
						WHEN leg.itpid::varchar != '' THEN  '<img src=\'/imagens/alterar.gif\' style=\'cursor:pointer\' title=\'". ("Sugerir/Avaliar Item")."\' onclick=\"w = window.open(\'seguranca.php?modulo=principal/questionario/popup/alteracaoLegendaItem&acao=A&itpid='||leg.itpid||'&queid='||leg.queid||'\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=370\'); w.focus();\">'
						WHEN leg.perid::varchar != '' THEN  '<img src=\'/imagens/alterar.gif\' style=\'cursor:pointer\' title=\'". ("Sugerir/Avaliar Item")."\' onclick=\"w = window.open(\'seguranca.php?modulo=principal/questionario/popup/alteracaoLegendaPergunta&acao=A&perid='||per.perid||'&queid='||leg.queid||'\',\'Janela1\',\'scrollbars=no,location=no,toolbar=no,menubar=no,width=530,height=370\'); w.focus();\">'
					END AS acao
				FROM
					questionario.legendapergunta leg
					LEFT JOIN seguranca.usuario         usu ON usu.usucpf = leg.usucpfsugestao
					LEFT JOIN questionario.pergunta     per ON per.perid  = leg.perid AND per.perstatus = 'A'
					LEFT JOIN questionario.itempergunta itp ON itp.itpid  = leg.itpid AND itp.itpstatus = 'A'
				WHERE
					leg.lgpsituacao = 'V'
					AND leg.queid = {$queid}";
		$rs = $this->carregar($sql);
	  	return $rs  == false ? array() : $rs;

	}
}