<?php

class Ceu extends Modelo
{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.ceu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ceuid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                        'ceuid'          => null,
                                        'ceucodigo'      => null,
                                        'ceunome'        => null,
                                        'ceudescricao'   => null,
                                        'ceustatus'      => null,
                                        'ceudtcadastro'  => null,
                                        'edcid' 		 => null,
                                        'usucpf'         => null
                                  );



	public function listar( $arrwhere , $paginacao = false, $retornarSql = false){

    	$arrwhere[] = " ceu.ceustatus = 'A' ";

    		$sql=" SELECT
    					ceu.ceucodigo,
    					ceu.ceunome,
    					ceu.ceudescricao,
    					mun.mundescricao,
    					mun.estuf,
    					(SELECT
    						count(vce.*)
    					 FROM
    					 	conferencia.vinculoceu vce
    					 JOIN
    					 	conferencia.dadosiniciaisvinculoceu div ON div.ceuid = vce.ceuid AND
    					 											   div.divstatus = 'A' AND
    					 											   div.edcid IS NOT NULL
    					 WHERE
    					 	vce.ceuid = ceu.ceuid AND
    					 	vce.vcestatus = 'A' ) AS vinculoerrado,
    					TO_CHAR(ceu.ceudtcadastro, 'DD/MM/YYYY as HH:MI') AS ceudtcadastro,
    					usu.usunome,
    					st_astext(edc.edccoordenadas) AS edccoordenadas,
    					ceu.ceuid,
    					ceu.ceuid AS codigo,
    					ceu.ceucodigo || ' - ' || ceu.ceunome || ' (' || mun.mundescricao || '/' || mun.estuf || ')' AS descricao
    				FROM
    					{$this->stNomeTabela} ceu
    				LEFT JOIN
                    	conferencia.endereco edc ON edc.edcid = ceu.edcid
    				LEFT JOIN
    					territorios.municipio mun ON edc.muncod = mun.muncod
    				INNER JOIN
    					seguranca.usuario usu ON ceu.usucpf = usu.usucpf
    				WHERE ".
    					implode(" AND ", $arrwhere);
		if(!$_POST["ordem"]){
		    $sql .= " ORDER BY ceu.ceunome";
		}

		if($retornarSql){
            return $sql;
		}elseif($paginacao){
    			$dado = Paginacao::getPaginacao($this, $sql, 50);
    			return $dado;
    	}else{
    		$retorno = $this->carregar($sql);
    		return $retorno ? $retorno : array();
        }
    }

    public function recuperarTodos( $stCampos, $arClausulas = null, $stOrdem = null , $returnArray = true){
    	$stCampos = $stCampos == '*' ? implode(', ', array_keys($this->arAtributos)) : $stCampos;

    	$sql  = " SELECT $stCampos FROM $this->stNomeTabela ceu ";
    	$sql .= "INNER JOIN
                    	conferencia.endereco edc ON edc.edcid = ceu.edcid
    			INNER JOIN
    					territorios.municipio mun ON edc.muncod = mun.muncod";
    	$sql .= $arClausulas ? " WHERE ". implode( " AND ", $arClausulas ) : "";
    	$sql .= $stOrdem ? " ORDER BY $stOrdem" : "";

    	$resultado = $this->carregar( $sql );

    	$retorno = null;
    	if ($returnArray){
    		$retorno = $resultado ? $resultado : array();
    	} else {
    		$retorno = $sql;
    	}

    	return $sql;
    }

    public function recuperarDados(){
        $sql = "SELECT
                    ceu.ceuid,
                    ceu.ceucodigo,
                    ceu.ceunome,
                    ceu.ceudescricao,
                    ceu.ceustatus,
                    ceu.ceudtcadastro,
                    ceu.usucpf,
                    ceu.edcid,
    				mun.mundescricao   as ceumundescricao,
    				mun.estuf		   as ceuestuf,
    				reg.regdescricao   as ceuregdescricao,
                    edc.edccep         as ceuedccep,
                    edc.edclogradouro  as ceuedclogradouro,
                    edc.edcnumero      as ceuedcnumero,
                    edc.edccomplemento as ceuedccomplemento,
                    edc.edcbairro      as ceuedcbairro,
                    edc.muncod         as ceumuncod,
                    edc.estuf          as ceuestuf,
                    st_astext(edc.edccoordenadas) as ceuedccoordenadas,
                    edc.edczoom        as ceuedczoom
                FROM
                    {$this->stNomeTabela} ceu
                INNER JOIN
                    	conferencia.endereco edc ON edc.edcid = ceu.edcid
    			INNER JOIN
    					territorios.municipio mun ON edc.muncod = mun.muncod
    			 INNER JOIN
                    territorios.estado est ON mun.estuf = est.estuf
                INNER JOIN
                    territorios.regiao reg ON reg.regcod = est.regcod
                WHERE
                    ceuid = {$this->ceuid}";
        return $this->pegaLinha($sql);
    }

    public function getSqlUfCeus($campos = "*", $arWhere = array(), $orderBy = "1"){
        $arWhere[] = "ceu.ceustatus = 'A'";

        $sql = "SELECT DISTINCT
                    {$campos}
                FROM
                    territorios.estado est
                INNER JOIN
                    territorios.municipio mun ON mun.estuf = est.estuf
                INNER JOIN
                    territorios.regiao reg ON reg.regcod = est.regcod
                INNER JOIN
                    conferencia.endereco edc ON edc.muncod = mun.muncod
                INNER JOIN
                    {$this->stNomeTabela} ceu ON ceu.edcid = edc.edcid
                WHERE
                    ".implode(" AND ", $arWhere)."
                ORDER BY
                     {$orderBy}";
        return $sql;
    }

    public function getSqlMunicipioCeus($campos = "*", $arWhere = array(), $orderBy = "1"){
        $arWhere[] = "ceu.ceustatus = 'A'";

        $sql = "SELECT DISTINCT
                    {$campos}
                FROM
                    territorios.municipio mun
                INNER JOIN
                    conferencia.endereco edc ON edc.muncod = mun.muncod
                INNER JOIN
                    {$this->stNomeTabela} ceu ON ceu.edcid = edc.edcid
                WHERE
                    ".implode(" AND ", $arWhere)."
                ORDER BY
                     {$orderBy}";
        return $sql;
    }

}