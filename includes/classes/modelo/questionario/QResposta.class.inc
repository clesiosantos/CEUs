<?php

class QResposta extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.resposta";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "resid" );

    /**
     * Atributos
     * @var array
     * @access protected
     *
     */
    protected $arAtributos     = array(
									  	'resid' => null,
									  	'perid' => null,
									  	'qrpid' => null,
									  	'usucpf' => null,
									  	'itpid' => null,
									  	'resdsc' => null,
									  	'arqid' => null,
									  	'qenid' => null
									  );

	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){

			if ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = " . $item;
				continue;
			}


		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return parent::pegaUmObjeto($sql, get_class((object) $this));
	}

	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = " . $item;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

		return $this->carregarObjeto($sql, get_class((object) $this));

	}

	public function pegaResposta($perid, $qrpid)
	{
		if (is_numeric($perid)){
			$sql = "SELECT
						res.resdsc
					FROM
						questionario.resposta res
					WHERE
						perid = {$perid} AND
						qrpid = {$qrpid}";
		}else{
			$sql = "select tabela.pega_total_resposta({$qrpid}, '{$perid}');";
		}
		return $this->pegaUm($sql);
	}

	public function carregarColuna(Array $where = null, $coluna='resid'){
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( get_class((object) $item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ($k === 'perid'){
				$codicao[] = "perid = " . $item;
				continue;
			}elseif ($k === 'qrpid'){
				$codicao[] = "qrpid = " . $item;
				continue;
			}
		}

		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );


		return parent::carregarColuna($sql, $coluna);
	}

   public function carregarResposta(Array $where = null, $coluna='resid'){
        $where  = (array) $where;

        foreach ($where as $k => $item){
            if ( get_class((object) $item) == 'QPergunta' ){
                $codicao[] = "res.perid = " . $item->perid;
                continue;
            }elseif ($k === 'res.perid'){
                $codicao[] = "res.perid = " . $item;
                continue;
            }elseif ($k === 'res.qrpid'){
                $codicao[] = "res.qrpid = " . $item;
                continue;
            }
        }

        $sql = "SELECT
                    " . ($coluna ? $coluna : "*") . "
                FROM
                    {$this->stNomeTabela} res
                LEFT JOIN
                    questionario.itempergunta itp ON itp.itpid = res.itpid AND itp.itpstatus = 'A'
                " . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );

        return parent::carregarColuna($sql);
    }

    public function copiarResposta($qrpidDestino, $perid = null){
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file      = new FilesSimec();
        $mResposta = new QResposta();
        $perid     = $perid ? $perid : $this->perid;

        $qenid = null;
        if($this->qenid){
            $mEndereco        = new QEndereco($this->qenid);
            $arDados          = $mEndereco->getDados();
            $arDados['qenid'] = null;
            $mEndereco->setDadosNull();
            $mEndereco->popularDadosObjeto($arDados);

            $qenid = $mEndereco->salvar();
        }

        $itpid = $this->itpid;
        if($this->itpid){
            $mItemPergunta = new QItemPergunta($this->itpid);
            $dadosItemPergunta = $mItemPergunta->recuperarLinha("*", array("perid = {$perid}", "itptitulo = '{$mItemPergunta->itptitulo}'"), false);
            if($dadosItemPergunta){
                $itpid = $dadosItemPergunta['itpid'];
            }
        }

        $arDadosResposta = array(
                'perid'  => $perid,
                'qrpid'  => $qrpidDestino,
                'usucpf' => $_SESSION["usucpf"],
                'itpid'  => $itpid,
                'resdsc' => $this->resdsc,
                'arqid'  => $this->arqid ? $file->copiarArquivo($this->arqid) : null,
                'qenid'  => $qenid
        );

        $mResposta->popularDadosObjeto($arDadosResposta)->salvar();
        return $mResposta;
    }
}