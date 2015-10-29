<?php

class Montatabela {

	private $htm;
	private $desab;

	public function montaNovaTabela( $perid, $qrpid, $percent = 90, $ro = 0, $desab = false ){
		$this->desab   = $desab;
		//global $db;
		$htm .= "<script>
					function checkRadioLinha(obj, linha)
					{
						$('[name*=campo_'+linha+']:radio').attr('checked', false);
						$(obj).attr('checked', true);
					}
				 </script>";

		//DADOS DA TABELA
		$obTabela 		= new Tabela( );
		$arrDadosTabela = $obTabela->pegaDados( $perid );

		if (!$arrDadosTabela){
			$htm = "<center>A estrutura da tabela desta pergunta está vazia.</center>";
			$this->htm = $htm;
			return;
		}

		$tabela 		= $arrDadosTabela['tblid'];
		$obTabela->carregarPorId($tabela);

		if ($qrpid){
			//RESPOSTAS
			$obTabelaResposta = new Tabelaresposta( );
			$arrResp 		  = $obTabelaResposta->recuperarResposta( $tabela, $qrpid );
		}
		//SUBTITULO
		$obGrupoColuna  = new Grupocoluna( );
		$arrGrupoColuna = $obGrupoColuna->pegaDados( $tabela );
		$arrGrupoColuna = $arrGrupoColuna ? $arrGrupoColuna : array();

		//LINHAS
		$obLinha  		= new Linha( );
		$obGrupolinha  	= new Grupolinha( );
		$arrLinha 		= $obLinha->pegaDados( $tabela );
		$arrLinha 		= $arrLinha ? $arrLinha : array();

		//COLUNAS
		$obColuna  = new Coluna( );
		$arrColuna = $obColuna->pegaDados( $tabela );
		$arrColuna = $arrColuna ? $arrColuna : array();

		$l = 0;

		if( $ro == 1 ){
			$habil = 'N';
		} else {
			$habil = 'S';
		}

		$bgcolor1 = "#A8A8A8";
		$bgcolor2 = "#CCCCCC";

		//Montando a Tabela
		$htm .= "<input type='hidden' name='idTabela' id='idTabela' value='{$tabela}'>";
//		$htm .= "<input type='hidden' name='tipoTabela' id='tipoTabela' value='{$arrDadosTabela['tbltipo']}'>";
		$htm .= "<table class='tabela' style='width:{$percent}%;' align='center'>";

        if( !empty($arrDadosTabela['descricao']) ){
            $htm .= "<tr>";
            $htm .= "<td bgcolor='$bgcolor1' colspan='50' align='center'><b>{$arrDadosTabela['descricao']}</b></td>";
            $htm .= "</tr>";
        }

		$htm .= "<tr>";
		$grupo = $obGrupolinha->conta( $tabela );

		$rolspan = $obGrupoColuna->verificaNiveis( $tabela );
		$colspan = $grupo > 1 ? 2 : 1;

		if( $arrDadosTabela['colunas'] && $arrDadosTabela['linhas'] ){
			$htm .= "<td bgcolor='$bgcolor1' align='center' colspan='$colspan' rowspan='{$rolspan}'><b>{$arrDadosTabela['colunas']} / {$arrDadosTabela['linhas']}</b></td>";
		} else {
			$flag = 2;
		}
		foreach( $arrGrupoColuna as $index=>$subTitulo ){
			$arrGrcid = $obGrupoColuna->pegaArrUltimoNivel( $subTitulo['codigo'] );

			$col = $obColuna->conta( $arrGrcid );

			if( $subTitulo['descricao'] <> '&nbsp;' && !empty($subTitulo['descricao']) ){
				if( ($arrDadosTabela['colunas'] == '' && $arrDadosTabela['linhas'] == '') && $index == 0 ){
					$htm .= "<td bgcolor='$bgcolor1' align='center'></td>";
				}
				if( $grupai <> $subTitulo['grupai'] ){
					$htm .= "</tr><tr><td bgcolor='$bgcolor1' align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
				} else {
					$htm .= "<td bgcolor='$bgcolor1' align='center' colspan='{$col}'><b>{$subTitulo['descricao']}</b></td>";
				}
			}
			$grupai = $subTitulo['grupai'];
		}

		if ($obTabela->tbltotalhorizontal == 't')
			$htm .= "<td rowspan='2' bgcolor='$bgcolor1' align='center'><b>{$obTabela->tbltotalhorizontaltxt}</b></td>";

		$htm .= "</tr>";
		//Monta as colunas
		$htm .= "<tr>";
		if( $flag == 2 ){
			$htm .= "<td bgcolor='$bgcolor1' align='center' width='40%'><b>{$arrLinha[0]['grupolinha']}</b></td>";
		}
		foreach( $arrColuna as $coluna ){
			$htm .= " <td bgcolor='$bgcolor2' align='center'><b>{$coluna['descricao']}</b></td>";
		}
		$htm .= "</tr>";

		$grp = 0;
		$bgcolor = "$bgcolor2";
		foreach( $arrLinha as $index=>$linha ){
			if( $grp <> $linha['grupoid'] ){
				if( ($linha['grupolinha'] <> '&nbsp;' && $linha['grupopai'] || $grupo > 1)  && $index != 0){
					$numero = $obLinha->conta( $linha['grupoid'] );
					$colspanGrupo = count($arrColuna)+1;
					$htm .= "<tr><td bgcolor='$bgcolor1' colspan='{$colspanGrupo}' width='10%' align='center'><b>{$linha['grupolinha']}</b></td></tr>";
					$l = 0;
				}
			}
			$htm .= "<tr>";

			if ($l % 2){
				$bgcolor = $bgcolor1;
			}else{
				$bgcolor = $bgcolor2;
			}

			if( $linha['lindsc'] == 't' ){
				$resposta = isset($arrResp[$linha['codigo']]['']) ? $arrResp[$linha['codigo']][''] : '';
				$htm .= "<td bgcolor='$bgcolor'><b>{$linha['descricao']}: ".campo_texto('campo_'.$linha['codigo'],'N','S','',20,60,'','','','','','campo_'.$linha['codigo'], '', $resposta)."</b></td>";
			} else {
				$htm .= "<td bgcolor='$bgcolor'><b>{$linha['descricao']}</b></td>";
			}


			$l++;
			$grp = $linha['grupoid'];

			foreach( $arrColuna as $coluna ){
				$c++;
				if ($l % 2){
					$bgcolor = $bgcolor1;
				}else{
					$bgcolor = $bgcolor2;
				}

				$somaRespostaColuna[$c] = $somaRespostaColuna[$c] ? $somaRespostaColuna[$c]: null;

				// O miolo da Tabela
				$resposta = isset($arrResp[$linha['codigo']][$coluna['codigo']]) ? $arrResp[$linha['codigo']][$coluna['codigo']] : '';

				if ($obTabela->tbltotalhorizontal == 't' && (is_numeric($resposta) || empty($resposta)))
					$somaRespostaLinha = ($somaRespostaLinha || $resposta) ? $somaRespostaLinha + $resposta : null;

//				switch ($arrDadosTabela['tbltipo']){
				switch ($coluna['coltipo']){
	 			case 'RA':
	 				if($this->desab)
	 					$habilCK = 'disabled';
	 				$checked = $resposta == 1 ? 'checked="checked"' : '';
					$htm .= "<td align='center'><input type=\"radio\" $habilCK onclick=\"checkRadioLinha(this, '{$linha['codigo']}');\" name=\"campo_" . $linha['codigo'].'_'.$coluna['codigo'] . "\" value='1' " . $checked . " />";
	 				break;
	 			case 'CK':
	 				if($this->desab)
	 					$habilCK = 'disabled';
	 				$checked = $resposta == 1 ? 'checked="checked"' : '';
					$htm .= "<td align='center'><input type=\"checkbox\" $habilCK name=\"campo_" . $linha['codigo'].'_'.$coluna['codigo'] . "\" value='1' " . $checked . " />";
	 				break;
	 			case 'TX':
	 				if($this->desab)
	 					$habilCK = 'N';
	 				else
	 					$habilCK = 'S';

					if ($obTabela->tbltotalvertical == 't' && (is_numeric($resposta) || empty($resposta))){
						$somaRespostaColuna[$c] = ($somaRespostaColuna[$c] || $resposta) ? $somaRespostaColuna[$c] + $resposta : null;
					}

					$mascara = $coluna['colnumeric'] == 't' ? '##########' : '';

					$htm .= "<td align='center'>".campo_texto('campo_'.$linha['codigo'].'_'.$coluna['codigo'],'N',$habilCK,'',20,60,$mascara,'','','','','campo_'.$linha['codigo'].'_'.$coluna['codigo'], '', $resposta)."</td>";
	 				break;
	 			}
			}

			if ($obTabela->tbltotalhorizontal == 't'){
				$htm .= "<td align='center' style='color:#3A66CC;'><b>{$somaRespostaLinha}</b></td>";
				$somaRespostaLinha=0;
			}

			$c = 0;
			$htm .= "</tr>";
		}

		if ($obTabela->tbltotalvertical == 't'){
			$htm .= "<td bgcolor='$bgcolor1' align='center'><b>{$obTabela->tbltotalverticaltxt}</b></td>";
			foreach ($somaRespostaColuna as $somaRespostaColuna){
				$htm .= "<td align='center' style='color:#3A66CC;'><b>{$somaRespostaColuna}</b></td>";
			}
			if ($obTabela->tbltotalhorizontal == 't'){
				$htm .= "<td><b>&nbsp;</b></td>";
			}
		}

		$htm .= "</table>";
		$this->htm = $htm;
	}

	function salvar() {

		$qrpid = $_POST['qrpid'];
		$perid = $_POST['perid'];

		$obTabelaResposta = new Tabelaresposta( );
		$obTabelaResposta->excluirPorQuestionarioTabela( $qrpid, $_POST['idTabela'] );
		$obTabelaResposta->commit();

		foreach ($_POST as $chave => $values)
		{
			if($values <> '' && strstr($chave,"campo_")){
				$obTabelaResposta = new Tabelaresposta( );
				list( , $linha, $coluna ) = explode("_",$chave);;

				$arrCampos = array('tblid', 'linid', 'colid', 'qrpid', 'tbrdsc');
				$arrDados  = array("tblid" => $_POST['idTabela'],
									  "linid" => $linha,
									  "colid" => $coluna,
									  "qrpid" => $qrpid,
									  "tbrdsc" => $values);
				$obTabelaResposta->popularObjeto( $arrCampos, $arrDados );
				$obTabelaResposta->salvar();
				$obTabelaResposta->commit();
			}
		}

		// Salva as respostas do survey no Painel
 		//extrairDadosSurveyParaPainel($qrpid, $perid);
	}

	function show(){
		echo $this->htm;
	}
}

?>