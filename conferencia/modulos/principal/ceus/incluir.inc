<?php
testarResponsabilidadeVinculoCeuTela(true, false);
$mVinculoCeu    = new VinculoCeu();
$mArquivo       = new Arquivo();
$vceid          = $mVinculoCeu->getVinculoSession();
$telaHabilitada = $mVinculoCeu->getPermissaoTelaVinculoCeu();
$mCeu           = new Ceu();

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if ($_REQUEST['act'] == 'salvar' || $_REQUEST['act'] == 'salvar_tramitar'){
    $gravar  = true;
    $msgErro = false;

    $arDadosVinculo = array(
                                'vceid'           => $vceid,
                                'ceuid'           => $_REQUEST["ceuid"],
                                'vcestatus'       => "A"
                            );

    if(isset($_REQUEST["vceativo"])){
        $arDadosVinculo["vceativo"] = $_REQUEST["vceativo"];
    }

    if ($_FILES["arquivo_convocacao"] && $_FILES["arquivo_convocacao"]['name'] && $_FILES["arquivo_convocacao"]['size']) {
        $validarArquivo = validarArquivo("arquivo_convocacao");
        if($validarArquivo === true){
            $file                = new FilesSimec();
            $arqid               = $file->setUploadArquivoEspecifico("", "arquivo_convocacao");
            $arDadosVinculo['arqiddoccpf'] = $arqid;
        }else{
            $gravar  = false;
            $msgErro = $validarArquivo;
        }
    }

    if ($gravar && $_FILES["arquivo_portaria"] && $_FILES["arquivo_portaria"]['name'] && $_FILES["arquivo_portaria"]['size']) {
        $validarArquivo = validarArquivo("arquivo_portaria");
        if($validarArquivo === true){
            $file                = new FilesSimec();
            $arqid               = $file->setUploadArquivoEspecifico("", "arquivo_portaria");
            $arDadosVinculo['arqidportaria'] = $arqid;
        }else{
            $gravar  = false;
            $msgErro = $validarArquivo;
        }
    }


    if($gravar === true){
        if(!$vceid){
            $arDadosVinculo["usucpf"]        = $_SESSION["usucpf"];
            $arDadosVinculo["vcedtcadastro"] = "now()";
            $arDadosVinculo["vcestatus"]     = "A";
            $arDadosVinculo['docid']         = wf_cadastrarDocumento(WF_PRECADASTRO_CONFERENCIA, 'Fluxo de Vínculo CEU');
            $flg_inclusao                    = true;
        }

        $vceid = $mVinculoCeu->popularDadosObjeto($arDadosVinculo)->salvar();

        if($flg_inclusao && possui_perfil(CONFERENCIA_PERFIL_CADASTRO)){
            $sqlResp = "INSERT INTO conferencia.usuarioresponsabilidade(pflcod, usucpf, rpustatus, rpudata_inc, vceid)
                            VALUES (" . CONFERENCIA_PERFIL_CADASTRO . ", '{$_SESSION['usucpf']}', 'A', now(), {$vceid});";
            $db->executar($sqlResp);
        }
    }

    if($gravar === true){
        $mVinculoCeu->setVinculoSession($vceid);
        $mVinculoCeu->commit();

        if($_REQUEST['act'] == 'salvar_tramitar'){
            wf_alterarEstado($mVinculoCeu->docid, WF_CEU_ACAO_CADASTRAMENTO_ENVIAR_APROVACAO, "Salvar e enviar para aprovação",array('vceid' => $vceid));
        }

        $db->sucesso($_REQUEST["modulo"]);
    }else{
        $mVinculoCeu->rollback();
        extract($_POST);

        if($msgErro && $msgErro != ''){
            echo "<script type='text/javascript'>alert('{$msgErro}');</script>";
        }
    }
}elseif( $_REQUEST['act'] == 'filtraMunicipio' ){
    if( $_REQUEST['estuf'] ){
        $sql = $mCeu->getSqlMunicipioCeus("mun.muncod AS codigo, mun.mundescricao AS descricao", array("mun.estuf = '{$_REQUEST['estuf']}'"), "descricao");
        $habilitado_municipio = $telaHabilitada;
    }elseif($_REQUEST['estuf'] == ''){
        $sql = array(array('codigo' => 0, 'descricao' => 'Escolha a UF'));
        $habilitado_municipio = 'N';
    }

    $muncod  = $_REQUEST['muncod'];
    $db->monta_combo("muncod", $sql, $habilitado_municipio, 'Selecione...', 'filtraCeu', '', '', 250, 'S', "muncod");
    die();
}elseif( $_REQUEST['act'] == 'filtraCeu' ){
    $muncod = $_REQUEST["muncod"];
    $ceuid  = $_REQUEST["ceuid"];
    if( $muncod ){
        $habilitado_ceu = $telaHabilitada;
        $dadosCeu = $mCeu->recuperarTodos("ceuid as codigo, ceucodigo || ' - ' || ceunome as descricao", array("ceustatus = 'A'", "mun.muncod = '{$muncod}'"), 2);
    }elseif($estuf == ''){
        $dadosCeu       = array(array('codigo' => 0, 'descricao' => 'Escolha o Município'));
        $habilitado_ceu = 'N';
    }

    $db->monta_combo("ceuid", $dadosCeu, $habilitado_ceu, 'Selecione...', '', '', '', 250, 'S', "ceuid");
    die();
}elseif ($_POST['act'] == 'download'){
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arqid']);
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";


if($vceid){
	    extract($mVinculoCeu->getDados());

	    $mCeu = new Ceu($ceuid);
	    extract($mCeu->getDados());
	    $mEndereco = new EnderecoCeu($edcid);
	    extract($mEndereco->getDados());

	if($mVinculoCeu->verificaSeVinculoAprovado( $vceid )){
	    echo $db->cria_aba($abacod_tela, $url, $parametros);
	}
}
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<form action="" method="post" name="formulario" enctype="multipart/form-data">
    <input type="hidden" name="act" />
    <input type="hidden" name="arqid" value=""/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <?php if($vceid) : ?>
            <tr>
                <td class="SubTituloDireita" style="width: 25%;">
                    Identificador:
                </td>
                <td>
                    <?= $vceid; ?>
                </td>
                <td rowspan="9">
                    <?php
                        if ($vceid && $docid) {
                            wf_desenhaBarraNavegacao($docid, array("vceid" => $vceid), null, 'S', false);
                            $mVinculoCeu->desenharBarraVinculo();
                        }
                    ?>
                </td>
            </tr>
        <?php endif; ?>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                UF:
            </td>
            <td>
                <?php
    	            $sql     = $mCeu->getSqlUfCeus("est.estuf as codigo, est.estuf as descricao, regdescricao", array(), "descricao");
    	            $dadosUf = $db->carregar($sql);
    	            $db->monta_combo("estuf", $dadosUf, $telaHabilitada, 'Selecione...', 'filtraMunicipio', '', '', '100', 'S', 'estuf');
    	        ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Região:
            </td>
            <td>
                <label id="lblregiao">-</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Município:
            </td>
            <td>
                <span id="spanMunicipio">
    	            <?php
    	                if( $estuf ){
    	                    $sql = $mCeu->getSqlMunicipioCeus("mun.muncod AS codigo, mun.mundescricao AS descricao", array("mun.estuf = '{$estuf}'"), "descricao");
    	                    $habilitado_municipio = $telaHabilitada;
    	                }else{
    	                    $sql = array(array('codigo' => 0, 'descricao' => 'Escolha a UF'));
    	                    $habilitado_municipio = 'N';
    	                }

    	                $db->monta_combo("muncod", $sql, $habilitado_municipio, 'Selecione...', 'filtraCeu', '', '', 250, 'S', "muncod");
    	            ?>
    	        </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Nº Contrato - Modelo do CEU:
            </td>
            <td>
                <span id="spanCeu">
    	            <?php
    	                if( $muncod ){
    	                    $habilitado_ceu = $telaHabilitada;
                            $dadosCeu = $mCeu->recuperarTodos("ceu.ceuid as codigo, ceu.ceucodigo || ' - ' || ceu.ceunome as descricao", array("ceu.ceustatus = 'A'", "mun.muncod = '{$muncod}'"), 2);
    	                }else{
    	                    $dadosCeu       = array(array('codigo' => 0, 'descricao' => 'Escolha o Município'));
    	                    $habilitado_ceu = 'N';
    	                }

    	                $db->monta_combo("ceuid", $dadosCeu, $habilitado_ceu, 'Selecione...', '', '', '', 250, 'S', "ceuid");
    	            ?>
    	        </span>
            </td>
        </tr>
        <?php if($db->testa_superuser()) : ?>
            <tr>
                <td class="SubTituloDireita" style="width: 25%;">
                    Ativo?
                </td>
                <td>
                    <input type="radio" name="vceativo" id="vceativo-t" <?= ($vceativo == 't' ? ' checked="checked" ' : "")  ?> value="t" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
    				<label for="vceativo-t" style="cursor: pointer;"> Sim</label>
                    &nbsp;
    				<input type="radio" name="vceativo" id="vceativo-f" <?= ($vceativo == 'f'  || empty($vceativo) ? ' checked="checked" ' : "") ?> value="f" <?= ($telaHabilitada == 'S' ? '' : 'disabled') ?> />
    				<label for="vceativo-f" style="cursor: pointer;"> Não</label>
                </td>
            </tr>
        <?php endif; ?>
         <tr>
            <td align='right' class="SubTituloDireita" style="font-weight: bold; text-align: center;" colspan="2">
                Anexos
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:30%">
                Documento contendo o número do CPF:
                <br />
                <label style='font-size: 9px; font-weight: bold;'>Tamanho máximo: <?= MAX_UPLOAD ?>MB</label>
                <br />
                (Formatos permitidos: .jpg, .gif, .bmp, .png e .pdf)
            </td>
            <td class="tdArquivo">
                <?php
                if($arqiddoccpf):
                    $mArquivo->carregarPorId($arqiddoccpf);
                    $arDadosArquivo = $mArquivo->getDados();
                ?>
                    <div class="divArquivo">
                         <input type="hidden" name="arqid_arqiddoccpf" value="arqiddoccpf" />
                         <?php if($telaHabilitada == 'S') : ?>
                            <img src='/imagens/anexo.gif' title="Alterar arquivo" onclick="alterarArquivo(this);" style="cursor: pointer;" />
                         <?php endif; ?>
                         <a href="javascript: download( <?= $arqiddoccpf; ?> );" style="cursor:pointer;" title="Realizar Download"  >
                            <img src='/imagens/salvar.png' />
                            <label style="vertical-align: top; cursor:pointer;">
                                <?= "{$arDadosArquivo["arqnome"]}.{$arDadosArquivo["arqextensao"]}"; ?>
                            </label>
    	                 </a>
    	                 <br />
                    </div>
                <?php
                endif;
                ?>
                <div class="divAnexo">
                    <?php
                    if($arqiddoccpf) : ?>
                        <img src='/imagens/anexo.gif' title="Retornar ao anexo original" onclick="alterarArquivo(this);" style="cursor: pointer;" />
                    <?php endif; ?>
                    <input type="file" name="arquivo_convocacao" id="arquivo_convocacao"  size="39" <?php echo $telaHabilitada == 'S' ? "" : "disabled=\"disabled\""; ?>/>
                </div>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="width:30%">
                Portaria de Constituição da UGL ou outro documento comprobatório do vínculo como responsável pelas informações deste CEU:
                <br />
                <label style='font-size: 9px; font-weight: bold;'>Tamanho máximo: <?= MAX_UPLOAD ?>MB</label>
                <br />
                (Formatos permitidos: .jpg, .gif, .bmp, .png e .pdf)
            </td>
            <td class="tdArquivo">
                <?php
                if($arqidportaria):
                    $mArquivo->carregarPorId($arqidportaria);
                    $arDadosArquivo = $mArquivo->getDados();
                ?>
                    <div class="divArquivo">
                         <input type="hidden" name="arqid_arqidportaria" value="arqidportaria" />
                         <?php if($telaHabilitada == 'S') : ?>
                             <img src='/imagens/anexo.gif' title="Alterar arquivo" onclick="alterarArquivo(this);" style="cursor: pointer;" />
                         <?php endif; ?>
                         <a href="javascript: download( <?= $arqidportaria; ?> );" style="cursor:pointer;" title="Realizar Download"  >
                            <img src='/imagens/salvar.png' />
                            <label style="vertical-align: top; cursor:pointer;">
                                <?= "{$arDadosArquivo["arqnome"]}.{$arDadosArquivo["arqextensao"]}"; ?>
                            </label>
    	                 </a>
    	                 <br />
                    </div>
                <?php
                endif;
                ?>
                <div class="divAnexo">
                    <?php
                    if($arqidportaria) : ?>
                        <img src='/imagens/anexo.gif' title="Retornar ao anexo original" onclick="alterarArquivo(this);" style="cursor: pointer;" />
                    <?php endif; ?>
                    <input type="file" name="arquivo_portaria" id="arquivo_portaria"  size="39" <?php echo $telaHabilitada == 'S' ? "" : "disabled=\"disabled\""; ?>/>
                </div>
            </td>
        </tr>

        <tr style="background-color: #cccccc">
                <td align="right" style="width:25%;">
                    &nbsp;
                </td>
                <td colspan="3">
                    <?php if($telaHabilitada == 'S') :?>
                        <input type="button" name="btnGravar" value="Salvar" onclick="enviarFormulario('salvar');"/>
                        <input type="button" name="btnGravarTramitar" value="Salvar e Enviar para aprovação" onclick="enviarFormulario('salvar_tramitar');" />
                    <?php endif;?>
                    <input type="button" name="btnVoltar" value="Voltar" onclick="cancelar();"/>
                </td>
            </tr>
    </table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script language="javascript" type="text/javascript" >

    $(function(){
        $("[type=file][name^=arquivo]").each(function(){
        	if($(this).closest(".tdArquivo").find(".divArquivo").length > 0){
            	$(this).closest(".tdArquivo").find(".divArquivo").show();
            	$(this).closest(".tdArquivo").find(".divAnexo").hide();
        	}
        });

        definirRegiao();
    });


    function definirRegiao(){
        var estuf   = $('[name=estuf]').val();
        var dadosUf = eval('(<?= json_encode($dadosUf); ?>)');
        $('#lblregiao').html("-");

        if(dadosUf){
        	$.each( dadosUf, function( key, dados ) {
                if(dados.codigo == estuf){
                    $('#lblregiao').html(dados.regdescricao);
                    return true;
                }
    		});
        }
    }

    function filtraMunicipio(estuf, muncod){
    	definirRegiao();

        $('#spanMunicipio').html('carregando...');
        $('#spanMunicipio').load('?modulo=principal/ceus/incluir&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }

    function filtraCeu(muncod, ceuid){
        $('#spanCeu').html('carregando...');
        $('#spanCeu').load('?modulo=principal/ceus/incluir&acao=A',
                          {'act'    : 'filtraCeu',
                           'ceuid'  : ceuid,
                           'muncod' : muncod});
    }

    function cancelar(){
        window.location = "?modulo=principal/ceus/listar&acao=A";
    }

    function enviarFormulario(acao){
    	var d = document;
        var f = d.formulario;
        var msg = '';

        if (f.estuf.value == '') {
            msg += '\n\tUF';
        }

        if (f.muncod.value == '') {
            msg += '\n\tMunicípio';
        }

        if (f.ceuid.value == '') {
            msg += '\n\tCEU';
        }
        if(acao == 'salvar'){
            if (msg != '') {
                alert('Os campos listados são obrigatórios e devem ser preenchidos:\n' + msg);
                return false;
            }
        }else if(acao == 'salvar_tramitar'){
            if ($('[name=arquivo_convocacao]:visible').length > 0 &&  $('[name=arquivo_convocacao]:visible').val() == '') {
                msg += '\n\tDocumento contendo o número do CPF';
            }

            if ($('[name=arquivo_portaria]:visible').length > 0 &&  $('[name=arquivo_portaria]:visible').val() == '') {
                msg += '\n\tPortaria de Constituição da UGL ou outro documento comprobatório do vínculo como responsável pelas informações deste CEU';
            }

            if (msg != '') {
                alert('Para enviar para aprovação os campos listados são obrigatórios e devem ser preenchidos:\n' + msg);
                return false;
            }
        }

        if(msg == ''){
            f.act.value = acao;
            f.submit();
        }
    }

    function download( cod ){
    	document.formulario.arqid.value  = cod;
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
</script>