<?php
testarResponsabilidadeVinculoCeuTela(true);
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$mDadosInciais       = new DadosIniciais();
$mVinculoCeu 	     = new VinculoCeu();
$mFotoDadosIniciais  = new FotoDadosIniciais();
$mEndereco           = new EnderecoCeu();
$mEnderecoPrefeitura = new EnderecoCeu();
$mArquivo            = new Arquivo();
$mTelPrefeitura      = new TelefonePrefeitura();
$mTelCeu             = new TelefoneCeu();

$mVinculoCeu->getVinculoSession();
$mConfiguracao     = new ConfiguracaoLiberacao();
$telaHabilitada    = $mConfiguracao->verificarAbaLiberada($abacod_tela, $url, $mVinculoCeu->ceuid);

extract($mVinculoCeu->getDados());
$mCeu = new Ceu($ceuid);
extract($mCeu->recuperarDados());

$arDadosIniciais = $mDadosInciais->getDadosByVinculoCeu($vceid);

if($_REQUEST["act"] == "salvar"){
	$gravar  = true;
	$msgErro = false;
	$edcid = null;
	$arrDadosNulo = array();

	$arDadosEnderecoPrefeitura = array(
                                       		'edcid'          => $arDadosIniciais['edcidprefeitura'],
                                       		'edccep'         => $_REQUEST["edccepprefeitura"],
                                       		'edclogradouro'  => $_REQUEST["edclogradouroprefeitura"],
                                       		'edcnumero'      => $_REQUEST["edcnumeroprefeitura"],
                                       		'edccomplemento' => $_REQUEST["edccomplementoprefeitura"],
                                       		'edcbairro'      => $_REQUEST["edcbairroprefeitura"],
                                       		'muncod'         => $_REQUEST["muncodprefeitura"],
                                       		'estuf'          => $_REQUEST["estufprefeitura"],
                                            'edccoordenadas' => $_REQUEST["edccoordenadasprefeitura"] ? $_REQUEST["edccoordenadasprefeitura"] : null,
                                            'edczoom'        => $_REQUEST["edczoomprefeitura"] ? $_REQUEST["edczoomprefeitura"] : null,
                                       );
	$edcidprefeitura = $mEnderecoPrefeitura->popularDadosObjeto($arDadosEnderecoPrefeitura)->salvar();

	if ($_REQUEST['flgenderecocorreto'] == 'f') {
		$arDadosEndereco = array(
				'edcid'          => $_REQUEST["edcid"],
				'edccep'         => $_REQUEST["edccep"],
				'edclogradouro'  => $_REQUEST["edclogradouro"],
				'edcnumero'      => $_REQUEST["edcnumero"],
				'edccomplemento' => $_REQUEST["edccomplemento"],
				'edcbairro'      => $_REQUEST["edcbairro"],
				'muncod'         => $_REQUEST["muncod"],
				'estuf'          => $_REQUEST["estuf"],
                'edccoordenadas' => $_REQUEST["edccoordenadasceu"] ? $_REQUEST["edccoordenadasceu"] : null,
                'edczoom'        => $_REQUEST["edczoomceu"] ? $_REQUEST["edczoomceu"] : null,
		);
		$edcid = $mEndereco->popularDadosObjeto($arDadosEndereco)->salvar();

	} else if($_REQUEST['flgenderecocorreto'] == 't'){
		$arrDadosNulo[] = 'edcid';
	}

	if(!$_REQUEST["divdtinauguracao"]){
		$arrDadosNulo[] = 'divdtinauguracao';
	}

    $arDados = array(
                            'divid'            => $mDadosInciais->recuperarUm("divid", array("ceuid = {$mVinculoCeu->ceuid}", "divstatus = 'A'")),
                            'vceid'            => $vceid,
                            'divdtcadastro'    => $_REQUEST["divdtcadastro"],
                            'divdtinauguracao' => $_REQUEST["divdtinauguracao"] ? $_REQUEST["divdtinauguracao"] : null,
                            'ceunome'          => $_REQUEST["ceunome"],
                            'ceudescricao'     => $_REQUEST["ceudescricao"],
                            'edcid'            => $edcid,
                            'ceuid'            => $ceuid,
                            'edcidprefeitura'  => $edcidprefeitura,
                            'usucpf'           => $_SESSION["usucpf"]
                      );

    $divid = $mDadosInciais->popularDadosObjeto($arDados)->salvar(true, true, $arrDadosNulo);

    $mFotoDadosIniciais->desabilitaPorDadosIniciais($divid, $_REQUEST['arqid']);
    //Remove o input file vazio da tr pai clonada
    unset($_FILES['arquivoFoto']);
	foreach ( $_FILES as $chave => $file ) {
		if (!empty($_FILES [$chave] ['name'])) {
			$arqid = null;
			$fdiid = null;

			//Se for update pega os ids dos arquivos alterados
			$arIdsFoto = explode ( '_', $chave );
			if (is_array ( $arIdsFoto )) {
				$arqid = $arIdsFoto [0];
				$fdiid = $arIdsFoto [1];
			}

			$dadosFoto = array (
					'arqid'     => $arqid,
					'fdiid'     => $fdiid,
					'divid'     => $divid,
					'fdistatus' => 'A'
			);

			$validarArquivo = validarArquivo ( $chave );

			if ($validarArquivo === true) {
				$file = new FilesSimec ();
				$arqid = $file->setUploadArquivoEspecifico ( "", $chave );
				$dadosFoto ['arqid'] = $arqid;
			} else {
				$gravar = false;
				$msgErro = $validarArquivo;
			}

			$mFotoDadosIniciais->popularDadosObjeto ( $dadosFoto )->salvar ();
		}
	}

    $mTelPrefeitura->desabilitarPorDadosIniciais($divid);
	foreach($_REQUEST['tprtelefone'] as $k => $tprtelefone){
	    if($tprtelefone != ""){
            $arDadosTelefone = array(
                                    'tprid'         => $_REQUEST['tprid'][$k],
                                    'tprddd'        => $_REQUEST['tprddd'][$k],
                                    'tprtelefone'   => $tprtelefone,
                                    'tprstatus'     => 'A',
                                    'tprdtcadastro' => 'now()',
                                    'usucpf'        => $_SESSION['usucpf'],
                                    'divid'         => $divid
                                  );
            $mTelPrefeitura->popularDadosObjeto($arDadosTelefone);
            $mTelPrefeitura->salvar();
            $mTelPrefeitura->setDadosNull();
	    }
	}

    $mTelCeu->desabilitarPorCeu($ceuid);
	foreach($_REQUEST['tcetelefone'] as $k => $tcetelefone){
	    if($tcetelefone != ""){
            $arDadosTelefone = array(
                                    'tceid'         => $_REQUEST['tceid'][$k],
                                    'tceddd'        => $_REQUEST['tceddd'][$k],
                                    'tcetelefone'   => $tcetelefone,
                                    'tcestatus'     => 'A',
                                    'tcedtcadastro' => 'now()',
                                    'usucpf'        => $_SESSION['usucpf'],
                                    'ceuid'         => $ceuid
                                  );
            $mTelCeu->popularDadosObjeto($arDadosTelefone);
            $mTelCeu->salvar();
            $mTelCeu->setDadosNull();
	    }
	}

    if($gravar === true){
	    $mDadosInciais->commit();
	    $db->sucesso("principal/ceus/dadosIniciais");
    } else {
    	$mDadosInciais->rollback();
    	extract($_POST);
    	$mVinculoCeu->getVinculoSession();
    	extract($mVinculoCeu->getDados());

    	if($msgErro && $msgErro != ''){
    		echo "<script type='text/javascript'>alert('{$msgErro}');</script>";
    	}
    }

}elseif( $_REQUEST['act'] == 'filtraMunicipio' ){
    if( $_REQUEST['estuf'] ){
        $sql = "SELECT
                    muncod AS codigo,
                    mundescricao AS descricao
                FROM
                    territorios.municipio
                WHERE
                    estuf = '{$_REQUEST['estuf']}'
                ORDER BY
                2";
        $habilitado_municipio = $telaHabilitada;
    }elseif($_REQUEST['estuf'] == ''){
        $sql = array(array('codigo' => 0, 'descricao' => 'Escolha a UF'));
        $habilitado_municipio = 'N';
    }

    $muncod  = $_REQUEST['muncod'];
    $db->monta_combo("muncod", $sql, $habilitado_municipio, 'Selecione...', '', '', '', 250, 'S', "muncod");
    die();
} elseif ($_POST['act'] == 'download'){
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arqid_download']);
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

if (is_array($arDadosIniciais)){
	extract($arDadosIniciais);
}
$flgenderecocorreto = $edccep ? 'f' : 't';
$flginaugurado = $divdtinauguracao ? 't' : 'f';

if ($divid) {
	$dadosFotos = $mFotoDadosIniciais->getFotosDadosIniciaisByWhere(array(" divid = '{$divid}' "));
}

echo $db->cria_aba($abacod_tela, $url, $parametros);
echo $mVinculoCeu->desenharCabecalho(true);
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );

?>
<style type="text/css">
.btns ul {
	margin: 0;
	padding: 9px 10px 0;
	clear: both;
	overflow: hidden;
	font-size: 90%;
}
.btns ul li {
	padding: 0 2px;
	float: left;
	list-style: none;
}
.btns ul li a {
	padding: 0 6px;
	color: #5c5c5c;
	background: #f2f7fa;
	float: left;
	line-height: 24px;
	text-decoration: none;
	border: 1px solid #d1d1d1;
}
.btns ul li a.btns_red {
	background: #FFE9EC;
}
.btns ul li a.btns_gray {
	background: #eee;
}
.btns ul li a:hover {
	text-decoration: underline;
}

.mapa{
    height:285px;
}

.painel-mapa{ <?= $telaHabilitada == 'S' ? "" : "display: none;" ?> }

.painel-mapa .btns { float:left; }
.painel-mapa .btns ul { padding: 4px; }
.painel-mapa .btn_find { float:left; margin:10px 0 0 5px; }
.painel-mapa input.findtext {
    margin-top:9px;
    padding:5px 3px 0 3px;
    height:24px;
    float:left;
    font-size:14px;
    font-weight:bold;
    color:#666;
    width:250px;
    border:1px #ccc solid;
}
.report-find-loading{
    float:left;
    height:31px;
    margin:9px 0 0 3px;
}

/* Altera a cor das coordenadas do mapa */
div.olControlMousePosition{
    color: #FFFFFF;
}

/* Remove botões de Polygon e PathItem */
.olControlDrawFeaturePathItemInactive {
    display: none;
}

.olControlDrawFeaturePolygonItemInactive {
    display: none;
}

.trEnderecoDiv {
	display: none;
}
</style>
<link rel="stylesheet" type="text/css" href="../includes/openlayers/openlayers.css" />
<script type="text/javascript" src="../includes/openlayers/OpenLayers.js"></script>

<script type="text/javascript" src="https://maps.google.com/maps/api/js?v=3.7&sensor=false&amp;language=pt_BR"></script>
<script type="text/javascript" src="http://www.google.com/uds/api?file=uds.js&v=1.0"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form action="" method="post" enctype="multipart/form-data"
	name="formulario" onsubmit="return enviarFormulario();">
	<input type="hidden" name="act" /> <input type="hidden" name="arqid_download"
		value="" />
	<input type="hidden" name="edcid" value="<?= $edcid; ?>" />
	<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita" colspan="3" style="text-align: center; font-weight: bold;">Endereço da prefeitura para contato</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita">CEP:</td>
			<td>
	 		    <?= campo_texto( 'edccepprefeitura', 'S', $telaHabilitada, '', 13, 10, '##.###-###', '', '', 0, '', '', null, null, "recuperarEnderecoPorCep('prefeitura');"); ?>
            </td>
            <td rowspan="8" style="width: 50%;">
               <div style="background-color: #DCDCDC; margin: 10px;">
        			<div id="divMapprefeitura" class="mapa">
        			</div>
        			<div class="painel-mapa" id="painel-mapaprefeitura">
        				<div id="panelprefeitura" class="olControlEditingToolbar"></div>
        				<div class="btns">
        					<ul>
        						<li><a href="#" class="btn_clear">LIMPAR MAPA</a></li>
        					</ul>
        				</div>
        				<div style="clear: both;"></div>
        			</div>
            		<input type="hidden" name="edczoomprefeitura" id="edczoomprefeitura" value="<?= $edczoomprefeitura; ?>" />
                    <input type="hidden" name="edccoordenadasprefeitura" id="edccoordenadasprefeitura" value="<?= $edccoordenadasprefeitura; ?>" />
               </div>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Logradouro:</td>
			<td>
	 		    <?= campo_texto( 'edclogradouroprefeitura', 'S', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número:</td>
			<td>
	 		    <?= campo_texto( 'edcnumeroprefeitura', 'S', $telaHabilitada, '', 13, 8, '########', '', '', 0, '', '', null, null, "buscarEnderecoMapa('prefeitura');"); ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento:</td>
			<td>
	 		    <?= campo_texto( 'edccomplementoprefeitura', 'N', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro:</td>
			<td>
	 		    <?= campo_texto( 'edcbairroprefeitura', 'S', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr>
            <td class="SubTituloDireita">
                Município/UF:
            </td>
            <td>
	 		    <input type="hidden" name="muncodprefeitura" value="<?= $muncodprefeitura; ?>" />
	 		    <?= campo_texto( 'mundescricaoprefeitura', 'N', 'N', '', 55, 150, '', ''); ?>
	 		    <?= campo_texto( 'estufprefeitura', 'S', 'N', '', 5, 150, '', ''); ?>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"
				style="text-align: center; font-weight: bold;"> Telefone(s) da prefeitura para contato</td>
		</tr>
		<tr>
		    <td class="SubTituloDireita">&nbsp;</td>
			<td>
				<table style="width: 450px;">
					<?php if($telaHabilitada == 'S') : ?>
					<tr>
						<td align='left' style="text-align: left;" colspan="2" style="width: 40px;">
    						<a href="javascript:incluirLinhaTelefone(this, 'prefeitura');">
								<img src="/imagens/gif_inclui.gif" style="cursor: pointer; vertical-align: bottom;" />
								Adicionar Telefone
    						</a>
					   </td>
					</tr>
					<?php endif;?>
					<tr>
						<td>
							<table align="center" class="tabela" bgcolor="#f5f5f5" style="width: 420px;">
								<tr>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 40px;">
									   Ação
								   </td>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 160px;">
									   DDD
								   </td>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 160px;">
									   Telefone
								   </td>
								</tr>
								<tr>
									<td colspan="3">
										<div style="width: 100%; padding: 0; margin: 0; height: 75px; overflow: auto;">
											<table id="tableTelefone-prefeitura" width="100%" align="rigth" border="0" cellspacing="0" cellpadding="2" style="margin-right: 15px;">
												<tr class="trPaiTelefone" style="display: none;" >
													<?php if($telaHabilitada == 'S') : ?>
													<td align='center' style="text-align: center; width: 40px;">
													    <input type="hidden" name="tprid[]" value="" />
														<img src="/imagens/excluir.gif" onclick="excluirLinhaTelefone(this, 'prefeitura');" style="cursor: pointer;" />
													</td>
													<?php endif;?>
													<td align='center' style="text-align: center; width: 160px;">
                                                        <?= campo_texto("tprddd[]", "S", $telaHabilitada, "", 2, 2, "##", ""); ?>
													</td>
													<td align='center' style="text-align: center; width: 160px;">
                                                        <?= campo_texto("tprtelefone[]", "S", $telaHabilitada, "", 10, 10, "####-####|#####-####", ""); ?>
													</td>
												</tr>
												<?php
												    if($divid):
                                                        $arTelefonesPrefeitura = $mTelPrefeitura->recuperarTodos("*", array("tprstatus = 'A'", "divid = {$divid}"));
                                                        foreach($arTelefonesPrefeitura as $telefone):
                                                        ?>
        												<tr >
        													<?php if($telaHabilitada == 'S') : ?>
        													<td align='center' style="text-align: center; width: 40px;">
        													    <input type="hidden" name="tprid[]" value="<?= $telefone['tprid'] ?>" />
        														<img src="/imagens/excluir.gif" onclick="excluirLinhaTelefone(this, 'prefeitura');" style="cursor: pointer;" />
        													</td>
        													<?php endif;?>
        													<td align='center' style="text-align: center; width: 160px;">
                                                                <?= campo_texto("tprddd[]", "S", $telaHabilitada, "", 2, 2, "##", "", 'left', '', 0, '', '', $telefone['tprddd']); ?>
        													</td>
        													<td align='center' style="text-align: center; width: 160px;">
                                                                <?= campo_texto("tprtelefone[]", "S", $telaHabilitada, "", 10, 10, "####-####|#####-####", "", 'left', '', 0, '', '', $telefone['tprtelefone']); ?>
        													</td>
        												</tr>
                                                        <?php
                                                        endforeach;
                                                    endif;
												?>
                                            </table>
										</div>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="3" style="text-align: center; font-weight: bold;">Endereço da Praça</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita">CEP:</td>
			<td>
	 		    <?= $ceuedccep; ?>
	 		    <input type="hidden" name="edczoomceuoriginal" id="edczoomceuoriginal" value="<?= $ceuedczoom; ?>" />
	 		    <input type="hidden" name="edccoordenadasceuoriginal" id="edccoordenadasceuoriginal" value="<?= $ceuedccoordenadas; ?>" />
            </td>
            <td id="tdMapaCeu" rowspan="17" style="width: 50%;">
               <div style="background-color: #DCDCDC; margin: 10px;">
        			<div id="divMapceu" class="mapa">
        			</div>
        			<div class="painel-mapa" id="painel-mapaceu">
        				<div id="panelceu" class="olControlEditingToolbar"></div>
        				<div class="btns">
        					<ul>
        						<li><a href="#" class="btn_clear">LIMPAR MAPA</a></li>
        					</ul>
        				</div>
        				<div style="clear: both;"></div>
        			</div>
            		<input type="hidden" name="edczoomceu" id="edczoomceu" value="<?= $edczoom; ?>" />
                    <input type="hidden" name="edccoordenadasceu" id="edccoordenadasceu" value="<?= $edccoordenadas; ?>" />
               </div>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Logradouro:</td>
			<td>
	 		    <?= $ceuedclogradouro; ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Número:</td>
			<td>
	 		    <?= $ceuedcnumero; ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Complemento:</td>
			<td>
	 		    <?= $ceuedccomplemento; ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Bairro:</td>
			<td>
	 		    <?= $ceuedcbairro; ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município/UF:</td>
			<td>
	 		    <?= $ceumundescricao; ?>
	 		    <?= $ceuestuf; ?>
            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2" style="text-align: center; font-weight: bold;"> &nbsp;</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">O endereço está correto?</td>
			<td>
				<input type="radio" name="flgenderecocorreto" id="flgenderecocorreto-t" <?php echo ($flgenderecocorreto == 't' || empty($flgenderecocorreto) ) ? ' checked="checked" ' : ""  ?> value="t" onclick="mostraEnderecoDiv();" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
				<label for="flgenderecocorreto-t" style="cursor: pointer;"> Sim</label>

				<input type="radio" name="flgenderecocorreto" id="flgenderecocorreto-f" <?php echo $flgenderecocorreto == 'f' ? ' checked="checked" ' : "" ?> value="f" onclick="mostraEnderecoDiv();" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
				<label for="flgenderecocorreto-f" style="cursor: pointer;"> Não</label></td>
		</tr>
		<tr class="trEnderecoDiv">
			<td class="SubTituloDireita" colspan="2" style="text-align: center; font-weight: bold;">Endereço da Praça atualizado</td>
		</tr>
		<tr class="trEnderecoDiv">
			<td align="right" class="SubTituloDireita">CEP:</td>
			<td>
	 		    <?= campo_texto( 'edccep', 'S', $telaHabilitada, '', 13, 10, '##.###-###', '', '', 0, '', '', null, null, 'recuperarEnderecoPorCep();'); ?>
            </td>
		</tr>
		<tr class="trEnderecoDiv">
			<td class="SubTituloDireita">Logradouro:</td>
			<td>
	 		    <?= campo_texto( 'edclogradouro', 'S', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr class="trEnderecoDiv">
			<td class="SubTituloDireita">Número:</td>
			<td>
	 		    <?= campo_texto( 'edcnumero', 'S', $telaHabilitada, '', 13, 8, '########', '', '', 0, '', '', null, null, "buscarEnderecoMapa();"); ?>
            </td>
		</tr>
		<tr class="trEnderecoDiv">
			<td class="SubTituloDireita">Complemento:</td>
			<td>
	 		    <?= campo_texto( 'edccomplemento', 'N', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr class="trEnderecoDiv">
			<td class="SubTituloDireita">Bairro:</td>
			<td>
	 		    <?= campo_texto( 'edcbairro', 'S', $telaHabilitada, '', 65, 150, '', ''); ?>
            </td>
		</tr>
		<tr class="trEnderecoDiv">
            <td class="SubTituloDireita">
                Município/UF:
            </td>
            <td>
	 		    <input type="hidden" name="muncod" value="<?= $muncod; ?>" />
	 		    <?= campo_texto( 'mundescricao', 'N', 'N', '', 55, 150, '', ''); ?>
	 		    <?= campo_texto( 'estuf', 'S', 'N', '', 5, 150, '', ''); ?>
            </td>
        </tr>
        <tr>
			<td class="SubTituloDireita" colspan="2"
				style="text-align: center; font-weight: bold;"> Telefone(s) para contato na Praça</td>
		</tr>
		<tr>
		    <td class="SubTituloDireita">&nbsp;</td>
			<td>
				<table style="width: 450px;">
					<?php if($telaHabilitada == 'S') : ?>
					<tr>
						<td align='left' style="text-align: left;" colspan="2" style="width: 40px;">
    						<a href="javascript:incluirLinhaTelefone(this, 'ceu');">
								<img src="/imagens/gif_inclui.gif" style="cursor: pointer; vertical-align: bottom;" />
								Adicionar Telefone
    						</a>
					   </td>
					</tr>
					<?php endif;?>
					<tr>
						<td>
							<table align="center" class="tabela" bgcolor="#f5f5f5" style="width: 420px;">
								<tr>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 40px;">
									   Ação
								   </td>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 160px;">
									   DDD
								   </td>
									<td align='center' class="SubTituloDireita" style="text-align: center; width: 160px;">
									   Telefone
								   </td>
								</tr>
								<tr>
									<td colspan="3">
										<div style="width: 100%; padding: 0; margin: 0; height: 75px; overflow: auto;">
											<table id="tableTelefone-ceu" width="100%" align="rigth" border="0" cellspacing="0" cellpadding="2" style="margin-right: 15px;">
												<tr class="trPaiTelefone" style="display: none;" >
													<?php if($telaHabilitada == 'S') : ?>
													<td align='center' style="text-align: center; width: 40px;">
													    <input type="hidden" name="tceid[]" value="" />
														<img src="/imagens/excluir.gif" onclick="excluirLinhaTelefone(this, 'ceu');" style="cursor: pointer;" />
													</td>
													<?php endif;?>
													<td align='center' style="text-align: center; width: 160px;">
                                                        <?= campo_texto("tceddd[]", "S", $telaHabilitada, "", 2, 2, "##", ""); ?>
													</td>
													<td align='center' style="text-align: center; width: 160px;">
                                                        <?= campo_texto("tcetelefone[]", "S", $telaHabilitada, "", 10, 10, "####-####|#####-####", ""); ?>
													</td>
												</tr>
												<?php
												    if($divid):
                                                        $arTelefonesCeu = $mTelCeu->recuperarTodos("*", array("tcestatus = 'A'", "ceuid = {$ceuid}"));
                                                        foreach($arTelefonesCeu as $telefone):
                                                        ?>
        												<tr >
        													<?php if($telaHabilitada == 'S') : ?>
        													<td align='center' style="text-align: center; width: 40px;">
        													    <input type="hidden" name="tceid[]" value="<?= $telefone['tceid'] ?>" />
        														<img src="/imagens/excluir.gif" onclick="excluirLinhaTelefone(this, 'ceu');" style="cursor: pointer;" />
        													</td>
        													<?php endif;?>
        													<td align='center' style="text-align: center; width: 160px;">
                                                                <?= campo_texto("tceddd[]", "S", $telaHabilitada, "", 2, 2, "##", "", 'left', '', 0, '', '', $telefone['tceddd']); ?>
        													</td>
        													<td align='center' style="text-align: center; width: 160px;">
                                                                <?= campo_texto("tcetelefone[]", "S", $telaHabilitada, "", 10, 10, "####-####|#####-####", "", 'left', '', 0, '', '', $telefone['tcetelefone']); ?>
        													</td>
        												</tr>
                                                        <?php
                                                        endforeach;
                                                    endif;
												?>
                                            </table>
										</div>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="3" style="text-align: center; font-weight: bold;"> &nbsp;</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Inaugurado</td>
			<td>
				<input type="radio" name="flginaugurado" id="flginaugurado-t" <?php echo $flginaugurado == 't' ? ' checked="checked" ' : "" ?> value="t" onclick="mostraDtInauguracao();" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
				<label for="flginaugurado-t" style="cursor: pointer;"> Sim</label>

				<input type="radio" name="flginaugurado" id="flginaugurado-f" <?php echo ($flginaugurado == 'f' || empty($flginaugurado) ) ? ' checked="checked" ' : ""  ?> value="f" onclick="mostraDtInauguracao();" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
				<label for="flginaugurado-f" style="cursor: pointer;"> Não</label></td>
		</tr>
		<tr id="trDtInauguracao">
			<td class="SubTituloDireita">Data de Inauguração:</td>
			<td>
		 		    <?= campo_data2( 'divdtinauguracao','S', $telaHabilitada, '', 'S' ); ?>
	            </td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="3" style="text-align: center; font-weight: bold;"> Fotos</td>
		</tr>
		<tr id="trTipoIR">
		    <td class="SubTituloDireita">&nbsp;</td>
			<td>
				<table style="width: 450px;">
					<?php if($telaHabilitada == 'S') : ?>
					<tr>
						<td align='left' style="text-align: left;" colspan="2"
							style="width: 40px;"><a href="javascript:incluirLinhaFoto(this);">
								<img src="/imagens/gif_inclui.gif" style="cursor: pointer; vertical-align: bottom;" />
								Adicionar Foto
						</a></td>
					</tr>
					<?php endif;?>
					<tr>
						<td>
							<table align="center" class="tabela" bgcolor="#f5f5f5" style="">
								<tr>
									<td align='center' class="SubTituloDireita"
										style="text-align: center; width: 40px;">Ação</td>
									<td align='center' class="SubTituloDireita"
										style="text-align: center;">Arquivo</td>
								</tr>
								<tr>
									<td colspan="6">
										<div id="divEstado" style="width: 100%; padding: 0; margin: 0; height: 75px; overflow: auto;">
											<table id="tableFotos" width="100%" align="rigth" border="0" cellspacing="0" cellpadding="2" style="margin-right: 15px;">
												<tr class="trPaiFoto" style="display: none">
													<?php if($telaHabilitada == 'S') : ?>
													<td align='center' style="text-align: center; width: 40px;">
														<img src="/imagens/excluir.gif" onclick="excluirLinhaFoto(this);" style="cursor: pointer;" />
													</td>
													<?php endif;?>
													<td align='center' style="text-align: center;">
														<input type="file" name="arquivoFoto[]" id="arquivoFoto[]" size="39" /></td>
												</tr>
                                                    <?php
                                                    if ($divid && is_array($dadosFotos)) {
                                                        foreach ($dadosFotos as $dadoFoto):

                                                        $mArquivo->carregarPorId($dadoFoto['arqid']);
                                                        $arDadosArquivo = $mArquivo->getDados();
                                                            ?>
                                                <tr>
                                                   	            <?php  if ($telaHabilitada == 'S') : ?>
                                                    <td align='center' style="text-align: center; width: 40px;">
	                                                   	<img src="/imagens/excluir.gif" onclick="excluirLinhaFoto(this);" style="cursor: pointer;" />
													</td>
                                                                <?php  endif; ?>
													<td align='center' style="text-align: center;" class="tdArquivo" />
													<div class="divArquivo">
														<input type="hidden" name="arqid[]" value="<?= $dadoFoto['arqid']; ?>" />
														<input type="hidden" name="fdiid[]" value="<?= $dadoFoto['fdiid']; ?>" />
                                                        <?php if($telaHabilitada == 'S'): ?>
    														<img src='/imagens/anexo.gif' title="Alterar arquivo" onclick="alterarArquivo(this);" style="cursor: pointer;" />
                                                        <?php endif; ?>
															<a href="javascript: download( <?= $dadoFoto['arqid']; ?> );" style="cursor: pointer;" title="Realizar Download">
															<img src='/imagens/salvar.png' />
															<label style="vertical-align: top; cursor: pointer;"> <?= "{$arDadosArquivo["arqnome"]}.{$arDadosArquivo["arqextensao"]}"; ?></label>
															</a>
														<br />
													</div>
													<div class="divAnexo">
														<img src='/imagens/anexo.gif' title="Retornar ao anexo original" onclick="alterarArquivo(this);" style="cursor: pointer;" />
														<input type="file" name="<?= $dadoFoto['arqid'] . '_' . $dadoFoto['fdiid'] ?>" id="<?= $dadoFoto['arqid'] . '_' . $dadoFoto['fdiid'] ?>" size="39" <?php echo $telaHabilitada == 'S' ? "" : "disabled=\"disabled\""; ?> />
													</div>
													</td>
												</tr>
                                                            <?php
                                                        endforeach;
                                                    }
                                                    ?>
                                                </table>
										</div>
									</td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>

		<tr style="background-color: #cccccc">
			<td align="right" style="width: 25%;">&nbsp;</td>
			<td colspan="3">
			    <?php if($telaHabilitada == 'S'): ?>
    			    <input type="submit" name="btnGravar" value="Salvar" onsubmit="return enviarFormulario();" />
			    <?php endif; ?>
    			<input type="button" name="btnGravar" value="Imprimir" onclick="abrirTelaDeImpressao();" />
			</td>
		</tr>
	</table>
</form>

<script language="javascript" type="text/javascript">

	$(function() {
		inicializarMapa('prefeitura');
		inicializarMapa('ceu');

		$("[type=file]").each(function(){
        	if($(this).closest(".tdArquivo").find(".divArquivo").length > 0){
            	$(this).closest(".tdArquivo").find(".divArquivo").show();
            	$(this).closest(".tdArquivo").find(".divAnexo").hide();
        	}
        });
		mostraEnderecoDiv();
		mostraDtInauguracao();
		zebrar('#tableFotos tr:visible');

		desenhaObjetoInicial('prefeitura');
		desenhaObjetoInicial('ceu');

	});

    function enviarFormulario(){
    	var d = document;
        var f = d.formulario;
        var msg = '';
        var flagEnderecoCorreto = $('[name=flgenderecocorreto]:checked').val();
        var flagInaugurado = $('[name=flginaugurado]:checked').val();
        var countArquivos = 0;
		var countArquivosUpdate = 0;

        if (f.edccepprefeitura.value == '') {
            msg += '\n\tCEP da Prefeitura';
        }

        if (f.edclogradouroprefeitura.value == '') {
            msg += '\n\tLogradouro da Prefeitura';
        }

        if (f.edcnumeroprefeitura.value == '') {
            msg += '\n\tNúmero da Prefeitura';
        }

        if (f.edcbairroprefeitura.value == '') {
            msg += '\n\tEndereço da Prefeitura';
        }

        if (f.estufprefeitura.value == '') {
            msg += '\n\tUF da Prefeitura';
        }

        if (f.muncodprefeitura.value == '') {
            msg += '\n\tMunicípio da Prefeitura';
        }

        if($('#tableTelefone-prefeitura tr:visible').length == 0){
            msg += '\n\tInforme ao menos um telefone da prefeitura para contato';
        }else{
        	$('#tableTelefone-prefeitura tr:visible').each(function(){
        		  if($(this).find('[name^=tprddd]').val() == "" || $(this).find('[name^=tprtelefone]').val() == ""){
                      msg += '\n\tPreencha todos os campos dos telefones para contato da prefeitura';
                      return false;
        		  }
        	});
        }

        $('input:file').each(function(){
        	if ($( this ).val() != ''){
        		countArquivos++;
        	}
    	});

        $("input:hidden[name^='arqid']").each(function(){
        	if ($( this ).val() != ''){
        		countArquivosUpdate++;
        	}
    	});

		if(flagEnderecoCorreto == 'f'){

	        if (f.edccep.value == '') {
	            msg += '\n\tCEP da Praça';
	        }

	        if (f.edclogradouro.value == '') {
	            msg += '\n\tLogradouro da Praça';
	        }

	        if (f.edcnumero.value == '') {
	            msg += '\n\tNúmero da Praça';
	        }

	        if (f.edcbairro.value == '') {
	            msg += '\n\tEndereço da Praça';
	        }

	        if (f.estuf.value == '') {
	            msg += '\n\tUF da Praça';
	        }

	        if (f.muncod.value == '') {
	            msg += '\n\tMunicípio da Praça';
	        }
		}


        if($('#tableTelefone-ceu tr:visible').length == 0){
            msg += '\n\tInforme ao menos um telefone da praça para contato';
        }else{
        	$('#tableTelefone-ceu tr:visible').each(function(){
        		  if($(this).find('[name^=tceddd]').val() == "" || $(this).find('[name^=tcetelefone]').val() == ""){
                      msg += '\n\tPreencha todos os campos dos telefones para contato da praça';
                      return false;
        		  }
        	});
        }

		if(flagInaugurado == 't'){
			if (f.divdtinauguracao.value == '') {
	            msg += '\n\tData de Inauguração';
	        }else if(isDataFutura(f.divdtinauguracao)){
                msg += '\n\tA Data de Inauguração não pode ser futura';
            }else{
                if(new Data().comparaData(f.divdtinauguracao.value, "01/01/2012", "<")){
                    msg += '\n\tA Data de Inauguração não pode ser anterior ao ano de 2012';
                }
            }

            if($('#tableFotos tr:visible').length == 0){
                msg += '\n\tInforme ao menos uma foto';
            }else{
            	$('#tableFotos tr:visible').each(function(){
            		  if($(this).find('[name^=foto]').val() == ""){
                          msg += '\n\tInforme ao menos uma foto';
                          return false;
            		  }
            	});
            }
		}

        if (msg != '') {
            alert('Os campos listados são obrigatórios e devem ser preenchidos:\n' + msg);
            return false;
        }

        // if (countArquivos < 1 && countArquivosUpdate < 1){
		// 	alert('É necessário adicionar ao menos uma Foto.');
		// 	return false;
        // }

        f.act.value = "salvar";
        return true;
    }

    function filtraMunicipio(estuf, muncod){
        $('#spanMunicipio').html('carregando...');
        $('#spanMunicipio').load('?modulo=sistema/apoio/incluirCeu&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function mostraEnderecoDiv(){
        var flag = $('[name=flgenderecocorreto]:checked').val();
        if(flag == 't'){
        	$('.trEnderecoDiv').hide();
        	$('.trEnderecoDiv input').val('');
        	limparDadosEndereco();
        	$('#painel-mapaceu').hide();
        	$('#tdMapaCeu').attr('rowspan', "10");

        	if(wkt){
                var coordenadas = $('#edccoordenadasceuoriginal').val();
            	var polygonFeature = wkt.read(coordenadas);
           	    polygonFeature.geometry.transform(map['ceu'].displayProjection, map['ceu'].getProjectionObject());

           	    var bounds = polygonFeature.geometry.getBounds();
           	    vlayer['ceu'].addFeatures([polygonFeature]);

           	    map['ceu'].zoomToExtent(bounds);
           	    map['ceu'].zoomTo($('#edczoomceuoriginal').val());
           	    jQuery('#edccoordenadasceu').val(coordenadas);
        	}
        }else if(flag == 'f'){
        	$('.trEnderecoDiv').show();
        	$('#painel-mapaceu').show();
        	$('#tdMapaCeu').attr('rowspan', "17");

        	if(map['ceu']){
            	var startPoint = new OpenLayers.LonLat(lonInicial,latInicial);
        		startPoint.transform(proj_4326, map['ceu'].getProjectionObject());
        		map['ceu'].setCenter(startPoint, 13);
        	}
        }
     }

    function mostraDtInauguracao(){
        var flag = $('[name=flginaugurado]:checked').val();
        if(flag == 'f'){
        	$('#trDtInauguracao').hide();
        	$('#trDtInauguracao input').val('');
        }else if(flag == 't'){
        	$('#trDtInauguracao').show();
        }
     }

    function recuperarEnderecoPorCep(sufixo){
    	sufixo = sufixo ? sufixo : "";
        var endcep = $('[name=edccep'+sufixo+']')[0];

        if (!endcep || endcep.value == '' || endcep.value.replace(/[^0-9]/ig, '').length != 8){
            limparDadosEndereco(sufixo);
            return false;
        }

    	$.ajax({
    		  type: "POST",
    		  url: "/geral/dne.php",
    		  data: { opt: 'dne', endcep: endcep.value },
    		  success: function (retorno){
                   eval(retorno);
                   if (DNE[0] && DNE[0].muncod == '') {
                       alert('CEP não encontrado!');
                       endcep.value = '';
                       endcep.select();

                       limparDadosEndereco(sufixo);
                       return false;
                   }

                   if (DNE[0] && DNE.length >= 1) {
                       $('[name=edclogradouro'+sufixo+']').val(DNE[0].logradouro);
                       $('[name=edcbairro'+sufixo+']').val(DNE[0].bairro);
                       $('[name=mundescricao'+sufixo+']').val(DNE[0].cidade);
                       $('[name=muncod'+sufixo+']').val(DNE[0].muncod);
                       $('[name=estuf'+sufixo+']').val(DNE[0].estado);
            	   }else{
            		   limparDadosEndereco(sufixo);
            	   }
    		  }
		});
    }

    function limparDadosEndereco(sufixo){
    	sufixo = sufixo ? sufixo : "";
    	$('[name=edccep'+sufixo+']').val('');
    	$('[name=edclogradouro'+sufixo+']').val('');
    	$('[name=edccomplemento'+sufixo+']').val('');
    	$('[name=edcnumero'+sufixo+']').val('');
        $('[name=edcbairro'+sufixo+']').val('');
        $('[name=muncod'+sufixo+']').val('');
        $('[name=mundescricao'+sufixo+']').val('');
        $('[name=estuf'+sufixo+']').val('');
        $('[name=estuf'+sufixo+']').val('');
    }


    function incluirLinhaFoto() {
        var oClone = $('#tableFotos .trPaiFoto:first').clone();
        var id = 'foto' + Math.floor((Math.random() * 99999) + 1);

        oClone.show();
        oClone.removeClass('trPaiFoto');
        oClone.attr('id', id);

        $('#tableFotos tr:last').after(oClone);
        $('#tableFotos tr:last input:file').attr('name', id);
        $('#tableFotos tr:last input:file').attr('id', id);

        $('#tableFotos tr:last input:file').attr('onChange', "validaArquivo(this)");

        zebrar('#tableFotos tr:visible');
    }

    function incluirLinhaTelefone(elemento, sufixo) {
    	sufixo = sufixo ? sufixo : "";
        var oClone = $('#tableTelefone-'+sufixo+' .trPaiTelefone:first').clone();
        var id = 'foto' + Math.floor((Math.random() * 99999) + 1);

        oClone.show();
        oClone.removeClass('trPaiTelefone');
        oClone.attr('id', id);

        $('#tableTelefone-'+sufixo+' tr:last').after(oClone);
        $('#tableTelefone-'+sufixo+' tr:last input:file').attr('name', id);
        $('#tableTelefone-'+sufixo+' tr:last input:file').attr('id', id);

        $('#tableTelefone-'+sufixo+' tr:last input:file').attr('onChange', "validaTelefone(this)");

        zebrar('#tableTelefone-'+sufixo+' tr:visible');
    }

    function validaArquivo(elem) {
        if (!validoFormato(elem.value)){
            alert('Formato de arquivo inválido para imagem');
    		$(elem).closest('tr').remove();
        }
    }

    function download( cod ){
    	document.formulario.arqid_download.value  = cod;
    	document.formulario.act.value = 'download';
    	document.formulario.submit();
    }

    function alterarArquivo(elem){
        if($(elem).closest(".tdArquivo").find(".divArquivo").length > 0){
            if($(elem).closest(".tdArquivo").find(".divArquivo:visible").length > 0){
            	$(elem).closest(".tdArquivo").find(".divArquivo").hide();
            	$(elem).closest(".tdArquivo").find(".divAnexo").show();
            }else{
            	$(elem).closest(".tdArquivo").find(".divArquivo").show();
            	$(elem).closest(".tdArquivo").find(".divAnexo").hide();
            }
            $(elem).closest(".tdArquivo").find(".divArquivo [name^=arquivo]").val("");
        }
    }


    function validoFormato(arquivo){
           var extensoes, ext, valido;
           extensoes = new Array('.gif', '.jpg', '.png', '.jpeg', '.bmp');

           ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
           valido = false;

           for(var i = 0; i <= arquivo.length; i++){
               if(extensoes[i] == ext){
                   valido = true;
                   break;
               }
           }

           if(valido){
               return true;
           }

         return false;
    }

    function excluirLinhaFoto(elem) {
        $(elem).parent().parent().remove();
        zebrar('#tableFotos tr:visible');
    }

    function excluirLinhaTelefone(elem, sufixo) {
    	sufixo = sufixo ? sufixo : "";
        $(elem).parent().parent().remove();
        zebrar('#tableTelefone-'+sufixo+' tr:visible');
    }

    function zebrar(seletor) {
        var cor1 = '#F5F5F5';
        var cor2 = '#E9E9E9';
        var cor = cor1;
        $(seletor).each(function(i, elem) {
            cor = (i % 2 == 0) ? cor2 : cor1;
            $(elem).css('background-color', cor);
        });
    }

    // *****************
    // ****** MAPA *****
    // *****************
    var map = new Array();
    var proj_4326 = new OpenLayers.Projection('EPSG:4326');
    var proj_900913 = new OpenLayers.Projection('EPSG:900913');
    var vlayer = new Array();
    var highlightCtrl = new Array();
    var selectCtrl = new Array();

    var lonInicial = -47.885858621191524;
    var latInicial = -15.791575490187014;

    var zoomInicial;

    var gLocalSearch = new GlocalSearch();

    function inicializarMapa(sufixo){
    	sufixo = sufixo ? sufixo : "";

		var options = {
			units : "dd",
			controls : [],
			numZoomLevels : 25,
			theme : false,
			sufixo : sufixo,
			projection : proj_900913,
			'displayProjection' : proj_4326,
			eventListeners : {
				"zoomend" : setZoom
			}
		};

		gLocalSearch.setSearchCompleteCallback(null, buscarLocalEndereco);

        zoomInicial = jQuery("#edczoom"+sufixo).val();
		zoomInicial = zoomInicial ? zoomInicial : '13';

		map[sufixo] = new OpenLayers.Map('divMap'+sufixo, options);
		var google_satellite = new OpenLayers.Layer.Google(
				"Google Maps Satélite", {
					type : google.maps.MapTypeId.SATELLITE,
					animationEnabled : true,
					sphericalMercator : true,
					maxExtent : new OpenLayers.Bounds(
							-20037508.34, -20037508.34,

							20037508.34, 20037508.34)
				});
		var google_hybrid = new OpenLayers.Layer.Google(
				"Google Maps Híbrido", {
					type : google.maps.MapTypeId.HYBRID,
					animationEnabled : true,
					sphericalMercator : true,
					maxExtent : new OpenLayers.Bounds(
							-20037508.34, -20037508.34,
							20037508.34, 20037508.34)
				});

		var google_normal = new OpenLayers.Layer.Google(
				"Google Maps Normal", {
					animationEnabled : true,
					sphericalMercator : true,
					maxExtent : new OpenLayers.Bounds(
							-20037508.34, -20037508.34,
							20037508.34, 20037508.34)
				});

		var google_physical = new OpenLayers.Layer.Google(
				"Google Maps Físico", {
					type : google.maps.MapTypeId.TERRAIN,
					animationEnabled : true,
					sphericalMercator : true,
					maxExtent : new OpenLayers.Bounds(
							-20037508.34, -20037508.34,
							20037508.34, 20037508.34)
				});

		map[sufixo].addLayers([ google_normal, google_hybrid, google_satellite, google_physical ]);

		map[sufixo].addControl(new OpenLayers.Control.Navigation());
		map[sufixo].addControl(new OpenLayers.Control.Zoom());
		map[sufixo].addControl(new OpenLayers.Control.MousePosition());
		map[sufixo].addControl(new OpenLayers.Control.ScaleLine());
		map[sufixo].addControl(new OpenLayers.Control.Scale('mapScale'));
		map[sufixo].addControl(new OpenLayers.Control.LayerSwitcher());

		style1 = new OpenLayers.Style(
				{
					pointRadius : "8",
					fillColor : "#702a24",
					fillOpacity : "0.4",
					strokeColor : "#320400",
					strokeWidth : 2.0,
					graphicZIndex : 1,
					externalGraphic : "/imagens/map_point/map-point1.png",
					graphicOpacity : 1,
					graphicWidth : 20,
					graphicHeight : 34,
					graphicXOffset : -14,
					graphicYOffset : -27
				});

		style3 = new OpenLayers.Style({
			pointRadius : "8",
			fillColor : "#ff776b",
			fillOpacity : "0.4",
			externalGraphic : "/imagens/map_point/map-point1s.png",
			strokeColor : "#6e0900",
			strokeWidth : 2.0,
			graphicZIndex : 1
		});

		var vlayerStyles = new OpenLayers.StyleMap({
			"default" : style1,
			"temporary" : style3
		});

		vlayer[sufixo] = new OpenLayers.Layer.Vector("Local", {
			styleMap : vlayerStyles,
			rendererOptions : {
				zIndexing : true
			}
		});
		map[sufixo].addLayer(vlayer[sufixo]);

		var drag = new OpenLayers.Control.DragFeature(vlayer[sufixo], {
		    sufixo : sufixo,
			onComplete : endDrag
		});
		map[sufixo].addControl(drag);

		// Eventos executados
		vlayer[sufixo].events.on({
			beforefeaturesadded : function(event) {
				// Antes de adicionar um elemento, limpa todos os elementos do mapa
				limparMapa(sufixo);
			},
			featuresadded : function(event) {
				atualizaGeometry(event, sufixo);
			},
			featuremodified : function(event) {
				atualizaGeometry(event, sufixo);
			},
			featuresremoved : function(event) {
				atualizaGeometry(event, sufixo);
			}
		});

		highlightCtrl[sufixo] = new OpenLayers.Control.SelectFeature(
				vlayer[sufixo], {
					hover : true,
					highlightOnly : true,
					renderIntent : "temporary"
				});

		selectCtrl[sufixo] = new OpenLayers.Control.SelectFeature(
				vlayer[sufixo], {
					clickout : true,
					toggle : false,
					multiple : false,
					hover : false,
					renderIntent : "select"
				});
		map[sufixo].addControl(highlightCtrl[sufixo]);
		map[sufixo].addControl(selectCtrl[sufixo]);

		wkt = new OpenLayers.Format.WKT();

		var startPoint = new OpenLayers.LonLat(lonInicial,
                latInicial);
		startPoint.transform(proj_4326, map[sufixo]
				.getProjectionObject());
		map[sufixo].setCenter(startPoint, zoomInicial);

		var container = document.getElementById("panel"+sufixo);
		var panel = new OpenLayers.Control.EditingToolbar(
				vlayer[sufixo], {
					div : container
				});
		map[sufixo].addControl(panel);
		panel.activateControl(panel.controls[0]);

		drag.activate();
		highlightCtrl[sufixo].activate();
		selectCtrl[sufixo].activate();

		$('.btn_clear').click(function() {
			limparMapa(sufixo);
			return false;
		});

		desenhaObjetoInicial(sufixo);

		jQuery('.baseLbl').html('Mapa');
		jQuery('.dataLbl').html('Itens');
    }

    function endDrag(feature, pixel) {
    	var sufixo = this.sufixo ? this.sufixo : '';
        atualizaGeometry(null, sufixo);
    }

    function setZoom(event) {
    	sufixo = this.sufixo ? this.sufixo : '';
    	jQuery("#edczoom"+sufixo).val(map[sufixo].getZoom());
    }

    //Método responsável por todas as alterações no mapa
    function atualizaGeometry(event, sufixo) {
        sufixo = sufixo ? sufixo : "";
        var geoCollection = new OpenLayers.Geometry.Collection;
        for (var i = 0; i < vlayer[sufixo].features.length; i++) {
        	newFeature = vlayer[sufixo].features[i].clone();
        	newFeature.geometry.transform(proj_900913, proj_4326);
        	geoCollection.addComponents(newFeature.geometry);

        	var format = new OpenLayers.Format.WKT();
        	var geometry = format.write(newFeature);

        	jQuery('#edccoordenadas'+sufixo).val(geometry);
        }
        centroid = geoCollection.getCentroid(true);
    }

    function limparMapa(sufixo){
        vlayer[sufixo].removeAllFeatures();
        jQuery('#edccoordenadas'+sufixo).val('');
    }

    function desenhaObjetoInicial(sufixo){
        if(jQuery('#edccoordenadas'+sufixo).val()){
        	var polygonFeature = wkt.read(jQuery('#edccoordenadas'+sufixo).val());
        	polygonFeature.geometry.transform(map[sufixo].displayProjection, map[sufixo].getProjectionObject());

        	var bounds = polygonFeature.geometry.getBounds();

        	vlayer[sufixo].addFeatures([polygonFeature]);

        	map[sufixo].zoomToExtent(bounds);
        	map[sufixo].zoomTo(zoomInicial);
        }
    }

    //Busca o local de um endereço
    function buscarLocalEndereco() {
        if (!gLocalSearch.results) return;

        var lat  = gLocalSearch.results[0].lat;
        var lng  = gLocalSearch.results[0].lng;
        var zoom = 17;

        var enderecoPoint = new OpenLayers.LonLat(lng, lat);
        enderecoPoint.transform(proj_4326, map[sufixo].getProjectionObject());
        map[sufixo].setCenter(enderecoPoint, zoom);
    }

    function buscarEnderecoMapa(sufixo){
    	sufixo = sufixo ? sufixo : "";
        var arEndereco = new Array();
    	var endereco = "";

    	if(jQuery('[name=edccep'+sufixo+']').val() && jQuery('[name=edccep'+sufixo+']').val().substr(7,10) != '000'){
            arEndereco.push(jQuery('[name=edccep'+sufixo+']').val());
        }

        if(jQuery('[name=edclogradouro'+sufixo+']').val()){
            arEndereco.push(jQuery('[name=edclogradouro'+sufixo+']').val());
        }

        if(jQuery('[name=edcnumero'+sufixo+']').val()){
            arEndereco.push(jQuery('[name=edcnumero'+sufixo+']').val());
        }

        if(jQuery('[name=edcbairro'+sufixo+']').val()){
            arEndereco.push(jQuery('[name=edcbairro'+sufixo+']').val());
        }

        if(jQuery('[name=mundescricao'+sufixo+']').val()){
            arEndereco.push(jQuery('[name=mundescricao'+sufixo+']').val());
        }

        if(jQuery('[name=estuf'+sufixo+']').val()){
            arEndereco.push(jQuery('[name=estuf'+sufixo+']').val());
        }

        arEndereco.push('Brasil');
        endereco = arEndereco.join(', ');
        $.ajax({
      		  type     : "GET",
      		  url      : "http://maps.google.com/maps/api/geocode/json",
      		  data     : { address: endereco, sensor : false },
      		  dataType : "json",
      		  success  : function (retorno){
          		  console.log(retorno);
          		  if(retorno && retorno.results && retorno.results[0]){
          			    sufixo = sufixo ? sufixo : 'ceu';

          		        var localizacao = retorno.results[0].geometry.location;
          		        var zoom        = 17;
          		        var coordenadas = "POINT("+localizacao.lng+" "+localizacao.lat+")";

          		        if(wkt){
              	            var polygonFeature = wkt.read(coordenadas);
                      	    polygonFeature.geometry.transform(map[sufixo].displayProjection, map[sufixo].getProjectionObject());

                      	    var bounds = polygonFeature.geometry.getBounds();
                      	    vlayer[sufixo].addFeatures([polygonFeature]);

                      	    map[sufixo].zoomToExtent(bounds);
                      	    map[sufixo].zoomTo(zoom);


                      	    jQuery('#edccoordenadas'+sufixo).val(coordenadas);
          		        }
          		  }
      		  }
  		});
    }

	function abrirTelaDeImpressao(){
		var janela = window.open("?modulo=principal/relatorioDadosIniciais&acao=A","page","toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=600");
		janela.focus();
	}

</script>