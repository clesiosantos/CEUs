<?php

class DadosIniciais extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "conferencia.dadosiniciaisvinculoceu";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("divid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
                                'divid'            => null,
                                'edcid'  		   => null,
                                'divdtinauguracao' => null,
                                'divstatus'		   => null,
                                'usucpf'     	   => null,
                                'divdtcadastro'    => null,
                                'ceuid'            => null,
                                'edcidprefeitura'  => null
                            );

    public function getDadosByVinculoCeu($vceid){
            $sql = "SELECT
            			div.divid,
            			div.divdtinauguracao,
            			vceid,
                        edc.edcid,
                        edc.edccep,
                        edc.edclogradouro,
                        edc.edcnumero,
                        edc.edccomplemento,
                        edc.edcbairro,
                        edc.muncod,
                        edc.estuf,
                        ST_ASTEXT(edc.edccoordenadas) AS edccoordenadas,
                        edc.edczoom AS edczoom,
                        mun.mundescricao,
                        div.edcidprefeitura,
                        edcp.edccep AS edccepprefeitura,
                        edcp.edclogradouro AS edclogradouroprefeitura,
                        edcp.edcnumero AS edcnumeroprefeitura,
                        edcp.edccomplemento AS edccomplementoprefeitura,
                        edcp.edcbairro AS edcbairroprefeitura,
                        edcp.muncod AS muncodprefeitura,
                        edcp.estuf AS estufprefeitura,
                        munp.mundescricao AS mundescricaoprefeitura,
                        ST_ASTEXT(edcp.edccoordenadas) AS edccoordenadasprefeitura,
                        edcp.edczoom AS edczoomprefeitura
                    FROM
                        {$this->stNomeTabela} as div
                    INNER JOIN
                        conferencia.vinculoceu vce ON vce.ceuid = div.ceuid
                                                  AND vce.vceid = {$vceid}
                    LEFT JOIN
                        conferencia.endereco edcp ON edcp.edcid = div.edcidprefeitura AND edcp.edcstatus = 'A'
                    LEFT JOIN
                        territorios.municipio munp ON munp.muncod = edcp.muncod
                    LEFT JOIN
                        conferencia.endereco edc ON edc.edcid = div.edcid AND edc.edcstatus = 'A'
                    LEFT JOIN
                        territorios.municipio mun ON mun.muncod = edc.muncod
                    WHERE
                        div.divstatus = 'A'";
            return $this->pegaLinha($sql);
    }

    public function verificarSeExistePorVinculo($vceid, $redirecionar = false){
        $dados = $this->getDadosByVinculoCeu($vceid);
        if(is_array($dados) && count($dados) > 0){
            return true;
        }else{
            if($redirecionar){
                $msgRedirecionar = "<script type='text/javascript'>
                                        alert('O preenchimento da aba dados iniciais é obrigatório antes de prosseguir.');
                                        window.location = '?modulo=principal/ceus/dadosIniciais&acao=A';
                                    </script>";
                die($msgRedirecionar);
            }
            return false;
        }
    }


}

