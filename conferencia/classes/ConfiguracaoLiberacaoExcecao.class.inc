<?php
class ConfiguracaoLiberacaoExcecao extends Modelo {

	/**
	 * Nome da tabela especificada
	 *
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = "conferencia.configuracaoliberacaoexcecao";

	/**
	 * Chave primaria.
	 *
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array (
			"cleid"
	);

	/**
	 * Atributos
	 *
	 * @var array
	 * @access protected
	 */
	protected $arAtributos = array (
			'cleid' => null,
			'mnuid' => null,
			'cledatalimite' => null,
			'usucpf' => null,
			'cledtcadastro' => null,
			'clestatus' => null
	);
	public function listar($arrWhere, $paginacao = false) {
		$arrWhere [] = " cle.clestatus = 'A' ";

		$sql = " SELECT
	    	mnu.mnudsc,
	    	TO_CHAR(cle.cledatalimite, 'DD/MM/YYYY') AS cledatalimite,
	    	ARRAY_TO_STRING(
	    			ARRAY ((SELECT
						ceu.ceucodigo || ' - ' || ceu.ceunome || ' (' || mun.mundescricao || '/' || mun.estuf || ')' AS descricao
					 FROM
					   conferencia.ceu
					 INNER JOIN
					 	 conferencia.liberacaoexcecaomenu lem ON lem.ceuid = ceu.ceuid AND lemstatus = 'A'
				 	 LEFT JOIN
                    	conferencia.endereco edc ON edc.edcid = ceu.edcid
     				 LEFT JOIN
    					territorios.municipio mun ON edc.muncod = mun.muncod
					 WHERE
					 	 lem.cleid = cle.cleid
						)), ', ') as ceus,
			TO_CHAR(cle.cledtcadastro, 'DD/MM/YYYY as HH:MI') AS cledtcadastro,
    		usu.usunome,
	    	cle.cleid
    	FROM
    	{$this->stNomeTabela} cle

    	INNER JOIN
    		seguranca.menu mnu ON mnu.mnuid = cle.mnuid
    	INNER JOIN
    		seguranca.usuario usu ON cle.usucpf = usu.usucpf
    	WHERE " . implode ( " AND ", $arrWhere );

		if ($paginacao) {
			$dado = Paginacao::getPaginacao ( $this, $sql, 10 );
			return $dado;
		} else {
			$retorno = $this->carregar ( $sql );
			return $retorno ? $retorno : array ();
		}
	}
}



