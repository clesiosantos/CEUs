<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

/**
 * Tela com o espelho de uma conferência específica.
 *
 */

//Verifica se há requisição para download
if ($_REQUEST['dw'] == 'download') {
    $file = new FilesSimec($_REQUEST['tipoArquivo'], null, "conferencia");
    $file->getDownloadArquivo($_REQUEST['arqid']);
}
// Variáveis da tela.
$pcnid = $_REQUEST['pcnid'] ? $_REQUEST['pcnid'] : '';

$objPreConf = new PreConferencia($pcnid);
$titulo_modulo = "Espelho da Conferência";
monta_titulo($titulo_modulo, '');

?>

<html>
    <head>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <script type="text/javascript" src="/includes/funcoes.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript">
        function download(tipo, arqid) {
            if (tipo == 'anexocoordenadorresponsavel') {
            	$('#formDownload #tipoArquivo').val(tipo);
                $('#formDownload #arqid').val(arqid);
                $('#formDownload').submit();
            } else if (tipo == 'anexopreconferencia') {
            	$('#formDownloadPre #tipoArquivo').val(tipo);
                $('#formDownloadPre #arqid').val(arqid);
                $('#formDownloadPre').submit();
            }
        }
        </script>
    </head>
    <body>
     <form action="" method="post" name="formDownloadPre" id="formDownloadPre">
        <input type="hidden" name="dw" id="dw" value="download"/>
        <input type="hidden" name="arqid" id="arqid" value=""/>
        <input type="hidden" name="tipoArquivo" id="tipoArquivo" value=""/>
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <tr>
				<td class="subtitulodireita" colspan="2" style="text-align:center;">Conferência</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Título</td>
				<td>
				    <?php echo $objPreConf->arAtributos['pcntitulo']; ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Local de realização</td>
				<td>
				    <?php echo $objPreConf->arAtributos['pcnlocal']; ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Período</td>
				<td>
				    <?php echo formata_data($objPreConf->arAtributos['pcninicio']); ?>
				    à
				    <?php echo formata_data($objPreConf->arAtributos['pcntermino']); ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Tema</td>
				<td>
				    <?php echo $objPreConf->arAtributos['pcntema']; ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Órgão gestor em cultura</td>
				<td>
				    <?php
				        $objOrgGestor = new OrgaoGestorCultura($objPreConf->arAtributos['ogcid']);
				        echo $objOrgGestor->arAtributos['ogcnome'];
				    ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Cargo</td>
				<td>
				    <?php
				        $objCargo = new Cargo($objPreConf->arAtributos['cgoid']);
				        echo $objCargo->arAtributos['cgonome'];
				    ?>
				</td>
			</tr>
			<tr>
				<td class="subtitulodireita" width="200px">Tipo da conferência</td>
				<td>
				    <?php
				        $objTipoConf = new TipoConferencia($objPreConf->arAtributos['tpcid']);
				        echo $objTipoConf->arAtributos['tpcnome'];
				    ?>
				</td>
			</tr>
			<?php if ($objPreConf->arAtributos['tpcid'] == 1) :?>
			<tr id="trUF">
			    <td class="subtitulodireita" width="200px">UF</td>
				<td>
				    <?php
				        $objEstConfMun = new EstadoPreConferencia();
				        $dadosEstado = $objEstConfMun->getEstadoPreConferenciaByWhere(array("pcnid = {$objPreConf->arAtributos['pcnid']}"));
				        echo $dadosEstado[0]['estuf'];
				    ?>
				</td>
			</tr>
			<tr id="trMunicipio">
			    <td class="subtitulodireita" width="200px">Município</td>
				<td>
				    <?php
				        $objMun = new Municipio($dadosEstado[0]['muncod']);
				        echo $objMun->arAtributos['mundescricao'];
				    ?>
				</td>
			</tr>
			<?php endif; ?>

			<?php if ($objPreConf->arAtributos['tpcid'] == 2 || $objPreConf->arAtributos['tpcid'] == 3) :?>
			<tr id="trMultiMunicipios">
				<td class="subtitulodireita" colspan="2" style="text-align:center;">Municípios</td>
			</tr>
			<tr id="trMultiMunicipiosListagem">
				<td colspan="2">
				     <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				         <thead>
    				         <tr>
    				             <th>UF</th>
    				             <th>Município</th>
    				         </tr>
				         </thead>
				         <?php
				         $objEstConfMun = new EstadoPreConferencia();
				         $dadosEstado = $objEstConfMun->getEstadoPreConferenciaByWhere(array("pcnid = {$objPreConf->arAtributos['pcnid']}"));

				         if ($dadosEstado) {
				            foreach ($dadosEstado as $i=>$dado) {
                                $cor = ($i % 2) ? '#F7F7F7' : '#FFFFFF';
                            ?>
    				            <tr bgcolor="<?php echo $cor; ?>">
        				            <td><?php echo $dado['estuf']?></td>
        				            <td><?php echo $dado['mundescricao']?></td>
    				            </tr>
				            <?php
			                 }
				         }
				         ?>
				     </table>
				</td>
			</tr>
			<?php endif; ?>
			<?php
			$anexoPreConferencia  = new AnexoPreConferencia();
			$dadosAnexo = $anexoPreConferencia->getAnexoCoordenadorResponsavelByWhere(array(" apc.pcnid = '{$objPreConf->arAtributos['pcnid']}' "));
            foreach ($dadosAnexo as $dadoAnexo):
                $modeloArquivo = new Arquivo($dadoAnexo['arqid']);
                extract($modeloArquivo->getDados());
                $tipoanexo = $dadoAnexo['apctipo'] == 'D' ? 'Decreto de Convocação da Conferência' : 'Portaria de Nomeação do Coordenador'
                ?>
                <tr>
                    <td align='right' class="SubTituloDireita" style="width:30%">
                        <?php
                        echo $tipoanexo . ':';
                        ?>
                    </td>
                    <td>
                        <?php if ($arqnome) { ?>
                            <a href="javascript: download( 'anexopreconferencia', <?= $dadoAnexo['arqid']; ?> );" style="cursor:pointer;" title="Download do Anexo"  >
                                <img src='/imagens/salvar.png' />
                                <label style="vertical-align: top; cursor:pointer;"> <?= $arqnome . '.' . $arqextensao; ?> </label>
                            </a><br>
                        <?php } ?>
                    </td>
                </tr>
            <?php endforeach; ?>
		</table>
		</form>
		<?php
	    $objConf = new Conferencia();
	    $dadosConf = $objConf->getByPreConferencia($objPreConf->arAtributos['pcnid']);

	    if (!empty($dadosConf)) :
		?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		    <tr>
				<td class="subtitulodireita" colspan="2" style="text-align:center;">Informações da Conferência</td>
			</tr>
			<tr>
                <td align='right' class="SubTituloDireita" width="200px">
                    Data de Publicação da Convocação
                </td>
                <td>
                    <?php echo formata_data($dadosConf['cnfdtpubconvocacao']); ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" width="200px">
                    Data de Realização da Conferência
                </td>
                <td>
                    <?php echo formata_data($dadosConf['cnfdtrealizacao']); ?>
                </td>
            </tr>
           <?php if ($dadosConf['tpcsigla'] != TIPO_CONFERENCIA_LIVRE && $dadosConf['tpcsigla'] != TIPO_CONFERENCIA_REGIONAL && $dadosConf['tpcsigla'] != TIPO_CONFERENCIA_VIRTUAL) : ?>
            <tr>
                <td align='right' class="SubTituloDireita" width="200px">
                    Quantidade de participantes
                </td>
                <td>
                    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="width: 100%;">
                        <tr>
                            <td align='right' class="SubTituloDireita" style="white-space: nowrap;">
                                Conferência
                            </td>
                            <td style="white-space: nowrap;">
                                <?php echo $dadosConf['cnfqtdptcconferencia']; ?>
                            </td>
                            <td align='right' class="SubTituloDireita" style="white-space: nowrap;">
                                Pré-Conferência
                            </td>
                            <td style="white-space: nowrap;">
                                <?php echo $dadosConf['cnfqtdptcpreconferencia']; ?>
                            </td>
                        </tr>
                        <tr>
                            <td align='right' class="SubTituloDireita" style="white-space: nowrap;">
                                Delegados Eleitos
                            </td>
                            <td style="white-space: nowrap;">
                                <?php echo $dadosConf['cnfqtddelegeleitos']; ?>
                            </td>
                        </tr>
                    </table>
                </td>
            </tr>
            <? endif; ?>
            <tr>
                <td align='right' class="SubTituloDireita" width="200px">
                    Informações Adicionais
                </td>
                <td>
                    <?php echo $dadosConf['cnfinfadc']; ?>
                </td>
            </tr>
		</table>
		<?php endif; ?>

		<?php
		$objDocConf = new DocumentoConferencia();
		$dadosAnexo = $objDocConf->getDocumentoConferenciaByWhere(array("dcf.pcnid = {$objPreConf->arAtributos['pcnid']}"));

		if (!empty($dadosAnexo)) :
	    ?>
	    <form action="" method="post" name="formDownload" id="formDownload">
	        <input type="hidden" name="dw" id="dw" value="download"/>
	        <input type="hidden" name="arqid" id="arqid" value=""/>
	        <input type="hidden" name="tipoArquivo" id="tipoArquivo" value=""/>
    	    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    		    <tr>
    				<td class="subtitulodireita" colspan="2" style="text-align:center;">Documentos da Conferência</td>
    			</tr>
    			<?php
    			foreach ($dadosAnexo as $dadoAnexo) :
                    $modeloArquivo = new Arquivo($dadoAnexo['arqid']);
                    extract($modeloArquivo->getDados());
                    $tipoanexo = $dadoAnexo['acrtipo'] == 'D' ? 'Decreto de Convocação da Conferência' : 'Portaria de Nomeação do Coordenador'
                    ?>
                    <tr>
                        <td align='right' class="SubTituloDireita" style="width:200px">
                            <?php
                            $tipoLabel = '';
                            if ($dadoAnexo['dcftipo'] == 'RI')
                                $tipoLabel = 'Regimento Interno';
                            elseif ($dadoAnexo['dcftipo'] == 'PC')
                                $tipoLabel = 'Programação da Conferencia';
                            elseif ($dadoAnexo['dcftipo'] == 'RF')
                                $tipoLabel = 'Relatório Final da Conferência';
                            echo $tipoLabel . ':';
                            ?>
                        </td>
                        <td>
                            <?php if ($arqnome) { ?>
                                <a href="javascript: download('anexocoordenadorresponsavel', <?= $dadoAnexo['arqid']; ?> );" style="cursor:pointer;" title="Download do Anexo"  >
                                    <img src='/imagens/salvar.png' />
                                    <label style="vertical-align: top; cursor:pointer;"> <?= $arqnome . '.' . $arqextensao; ?> </label>
                                </a><br>
                            <?php } ?>
                        </td>
                    </tr>
                    <?php endforeach; ?>
    		</table>
		</form>
		<?php endif; ?>
    </body>
</html>