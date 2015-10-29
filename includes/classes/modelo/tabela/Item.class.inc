<?php

class Item extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "tabela.item";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itmid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array(
									  	'itmid' => null,
									  	'itmdsc' => null,
									  );

    public function getPerguntaPaiByItem($itmid){
        $sql = "SELECT
                    tbl.perid
                FROM
                    tabela.tabela tbl
                LEFT JOIN
                    tabela.grupolinha grl ON grl.tblid = tbl.tblid
                LEFT JOIN
                    tabela.linha lin ON lin.grlid = grl.grlid
                LEFT JOIN
                    tabela.item itm ON itm.itmid = lin.itmid
                LEFT JOIN
                    tabela.grupocoluna grc ON grc.tblid = tbl.tblid
                LEFT JOIN
                    tabela.coluna col ON col.grcid = grc.grcid
                LEFT JOIN
                    tabela.item itm2 ON itm2.itmid = col.itmid
                WHERE
                    itm.itmid = {$itmid} OR itm2.itmid = {$itmid};";
        $perid = $this->pegaUm($sql);

        return $perid;
    }

    public function getItemFilhoLinhaByPerguntaPaiDscItem($peridPai, $itmdsc){
        $sql = "SELECT
                    itm.itmid
                FROM
                    tabela.tabela tbl
                INNER JOIN
                    questionario.pergunta per ON per.perid = tbl.perid
                INNER JOIN
                    questionario.itempergunta itp ON itp.itpid = per.itpid
                INNER JOIN
                    tabela.grupolinha grl ON grl.tblid = tbl.tblid
                INNER JOIN
                    tabela.linha lin ON lin.grlid = grl.grlid
                INNER JOIN
                    tabela.item itm ON itm.itmid = lin.itmid
                WHERE
                    itm.itmdsc = '{$itmdsc}'
                    AND
                    itp.perid = {$peridPai};";

        $itmid = $this->pegaUm($sql);
        return $itmid;
    }

    public function getItemFilhoColunaByPerguntaPaiDscItem($peridPai, $itmdsc){
        $sql = "SELECT
                    itm.itmid
                FROM
                    tabela.tabela tbl
                INNER JOIN
                    questionario.pergunta per ON per.perid = tbl.perid
                INNER JOIN
                    questionario.itempergunta itp ON itp.itpid = per.itpid
                INNER JOIN
                    tabela.grupocoluna grc ON grc.tblid = tbl.tblid
                INNER JOIN
                    tabela.coluna col ON col.grcid = grc.grcid
                INNER JOIN
                    tabela.item itm ON itm.itmid = col.itmid
                WHERE
                    itm.itmdsc = '{$itmdsc}'
                    AND
                    itp.perid = {$peridPai};";

        $itmid = $this->pegaUm($sql);
        return $itmid;
    }
}