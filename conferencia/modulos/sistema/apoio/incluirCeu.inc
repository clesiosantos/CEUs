<?php
$telaHabilitada = 'S';
$ceuid          = $_REQUEST["ceuid"];
$mCeu           = new Ceu($ceuid);
$mEndereco	    = new EnderecoCeu();

if($_REQUEST["act"] == "salvar"){
	$arDadosEndereco = array(
			'edcid'          => $_REQUEST["edcid"],
			'edccep'         => $_REQUEST["edccep"],
			'edclogradouro'  => $_REQUEST["edclogradouro"],
			'edcnumero'      => $_REQUEST["edcnumero"],
			'edccomplemento' => $_REQUEST["edccomplemento"],
			'edcbairro'      => $_REQUEST["edcbairro"],
			'muncod'         => $_REQUEST["muncod"],
			'estuf'          => $_REQUEST["estuf"],
			'edccoordenadas' => $_REQUEST["edccoordenadas"] ? $_REQUEST["edccoordenadas"] : null,
			'edczoom'        => $_REQUEST["edczoom"]
	);
	$edcid = $mEndereco->popularDadosObjeto($arDadosEndereco)->salvar();

    $arDados = array(
                            'ceuid'          => $ceuid,
                            'ceucodigo'      => $_REQUEST["ceucodigo"],
                            'ceunome'        => $_REQUEST["ceunome"],
                            'ceudescricao'   => $_REQUEST["ceudescricao"],
                            'ceustatus'      => "A",
                            'ceudtcadastro'  => "now()",
                            'usucpf'         => $_SESSION["usucpf"],
                            'edcid'          => $edcid
                      );
    $codTest = $mCeu->popularDadosObjeto($arDados)->salvar();

    $mCeu->commit();
    $db->sucesso("sistema/apoio/listarCeu");

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
}

include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";

if($ceuid){
    extract($mCeu->recuperarDados());

    $mEndereco = new EnderecoCeu($edcid);
    extract($mEndereco->getDados());
    $titulo_modulo = "Alterar CEU";
}

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
</style>
<link rel="stylesheet" type="text/css" href="../includes/openlayers/openlayers.css" />
<script type="text/javascript" src="../includes/openlayers/OpenLayers.js"></script>

<script type="text/javascript" src="https://maps.google.com/maps/api/js?v=3.7&sensor=false&amp;language=pt_BR"></script>
<script type="text/javascript" src="http://www.google.com/uds/api?file=uds.js&v=1.0"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<form action="" method="post" name="formulario" onsubmit="return enviarFormulario();">
    <input type="hidden" name="act" />
    <input type="hidden" name="ceuid" value="<?= $ceuid; ?>"/>
    <input type="hidden" name="edcid" value="<?= $edcid; ?>"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Contrato:
            </td>
            <td>
                <?= campo_texto("ceucodigo", 'S', "S", "", "10", "8", "########", ""); ?>
            </td>
            <td rowspan="10" style="width: 50%;">
               <div style="background-color: #DCDCDC; margin: 10px;">
                   <div style="background-color: #DCDCDC; padding: 5px 0px 5px 0px; text-align: center;">
                        Coordenadas Geográficas
                    </div>
        			<div id="divMap" class="mapa">
        			</div>
        			<div class="painel-mapa">
        				<div id="panel" class="olControlEditingToolbar"></div>
        				<div class="btns">
        					<ul>
        						<li><a href="#" class="btn_clear">LIMPAR MAPA</a></li>
        					</ul>
        				</div>
        				<div style="clear: both;"></div>
        			</div>
            		<input type="hidden" name="edczoom" id="edczoom" value="<?= $edczoom; ?>" />
                    <input type="hidden" name="edccoordenadas" id="edccoordenadas" value="<?= $edccoordenadas; ?>" />
               </div>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Tipo:
            </td>
            <td>
                <?= campo_texto("ceunome", 'S', "S", "", "45", "255", "", ""); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width: 25%;">
                Descrição:
            </td>
            <td>
                <?= campo_textarea("ceudescricao", "N", "S", "", 70, 4, 500) ?>
            </td>
        </tr>
         <tr>
            <td class="SubTituloDireita" colspan="2" style="text-align: center; font-weight: bold;">
                 Endereço
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita">
                CEP:
            </td>
            <td>
	 		    <?= campo_texto( 'edccep', 'S', $permissao_formulario, '', 13, 10, '##.###-###', '', '', 0, '', '', null, null, 'recuperarEnderecoPorCep();'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Logradouro:
            </td>
            <td>
	 		    <?= campo_texto( 'edclogradouro', 'S', $permissao_formulario, '', 65, 150, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Número:
            </td>
            <td>
	 		    <?= campo_texto( 'edcnumero', 'S', $permissao_formulario, '', 13, 8, '########', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Complemento:
            </td>
            <td>
	 		    <?= campo_texto( 'edccomplemento', 'N', $permissao_formulario, '', 65, 150, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Bairro:
            </td>
            <td>
	 		    <?= campo_texto( 'edcbairro', 'S', $permissao_formulario, '', 65, 150, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Município/UF:
            </td>
            <td>
	 		    <input type="hidden" name="muncod" value="<?= $muncod; ?>" />
	 		    <?= campo_texto( 'mundescricao', 'N', 'N', '', 55, 150, '', ''); ?>
	 		    <?= campo_texto( 'estuf', 'S', 'N', '', 5, 150, '', ''); ?>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align="right" style="width:25%;">
                &nbsp;
            </td>
            <td colspan="2">
                <input type="submit" name="btnGravar" value="Salvar" />
                <input type="button" name="btnVoltar" value="Voltar" onclick="cancelar();"/>
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >
    function enviarFormulario(){
    	var d = document;
        var f = d.formulario;
        var msg = '';

        if (f.ceucodigo.value == '') {
            msg += '\n\tContrato';
        }

        if (f.ceunome.value == '') {
            msg += '\n\tTipo';
        }

        if (f.estuf.value == '') {
            msg += '\n\tUF';
        }

        if (f.muncod.value == '') {
            msg += '\n\tMunicípio';
        }

        if (msg != '') {
            alert('Os campos listados são obrigatórios e devem ser preenchidos:\n' + msg);
            return false;
        }

        f.act.value = "salvar";
        return true;
    }

    function cancelar(){
        window.location = "?modulo=sistema/apoio/listarCeu&acao=A";
    }

    function filtraMunicipio(estuf, muncod){
        $('#spanMunicipio').html('carregando...');
        $('#spanMunicipio').load('?modulo=sistema/apoio/incluirCeu&acao=A',
                          {'act'    : 'filtraMunicipio',
                           'estuf'  : estuf,
                           'muncod' : muncod});
    }


    var map;
    var proj_4326 = new OpenLayers.Projection('EPSG:4326');
    var proj_900913 = new OpenLayers.Projection('EPSG:900913');
    var vlayer;
    var highlightCtrl;
    var selectCtrl;

    var lonInicial = -47.885858621191524;
    var latInicial = -15.791575490187014;

    var zoomInicial;

    var gLocalSearch    = new GlocalSearch();

    jQuery(window)
    .load(
    		function() {
    			var options = {
    				units : "dd",
    				controls : [],
    				numZoomLevels : 25,
    				theme : false,
    				projection : proj_900913,
    				'displayProjection' : proj_4326,
    				eventListeners : {
    					"zoomend" : setZoom
    				}
    			};

    			gLocalSearch.setSearchCompleteCallback(null, buscarLocalEndereco);

                zoomInicial = jQuery("#edczoom").val();
    			zoomInicial = zoomInicial ? zoomInicial : '13';

    			map = new OpenLayers.Map('divMap', options);
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

    			map.addLayers([ google_normal, google_hybrid, google_satellite, google_physical ]);

    			map.addControl(new OpenLayers.Control.Navigation());
    			map.addControl(new OpenLayers.Control.Zoom());
    			map.addControl(new OpenLayers.Control.MousePosition());
    			map.addControl(new OpenLayers.Control.ScaleLine());
    			map.addControl(new OpenLayers.Control.Scale('mapScale'));
    			map.addControl(new OpenLayers.Control.LayerSwitcher());

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

    			vlayer = new OpenLayers.Layer.Vector("Edificação/Estabelecimento", {
    				styleMap : vlayerStyles,
    				rendererOptions : {
    					zIndexing : true
    				}
    			});
    			map.addLayer(vlayer);

    			var drag = new OpenLayers.Control.DragFeature(vlayer, {
    				onComplete : endDrag
    			});
    			map.addControl(drag);

    			// Eventos executados
    			vlayer.events.on({
    				beforefeaturesadded : function(event) {
    					// Antes de adicionar um elemento, limpa todos os elementos do mapa
    					limparMapa();
    				},
    				featuresadded : function(event) {
    					atualizaGeometry(event);
    				},
    				featuremodified : function(event) {
    					atualizaGeometry(event);
    				},
    				featuresremoved : function(event) {
    					atualizaGeometry(event);
    				}
    			});

    			highlightCtrl = new OpenLayers.Control.SelectFeature(
    					vlayer, {
    						hover : true,
    						highlightOnly : true,
    						renderIntent : "temporary"
    					});

    			selectCtrl = new OpenLayers.Control.SelectFeature(
    					vlayer, {
    						clickout : true,
    						toggle : false,
    						multiple : false,
    						hover : false,
    						renderIntent : "select"
    					});
    			map.addControl(highlightCtrl);
    			map.addControl(selectCtrl);

    			wkt = new OpenLayers.Format.WKT();

    			var startPoint = new OpenLayers.LonLat(lonInicial,
                        latInicial);
    			startPoint.transform(proj_4326, map
    					.getProjectionObject());
    			map.setCenter(startPoint, zoomInicial);

    			var container = document.getElementById("panel");
    			var panel = new OpenLayers.Control.EditingToolbar(
    					vlayer, {
    						div : container
    					});
    			map.addControl(panel);
    			panel.activateControl(panel.controls[0]);

    			drag.activate();
    			highlightCtrl.activate();
    			selectCtrl.activate();

    			$('.btn_clear').click(function() {
    				limparMapa();
    				return false;
    			});

    			desenhaObjetoInicial();

    			jQuery('.baseLbl').html('Mapa');
    			jQuery('.dataLbl').html('Itens');

    		});

    function endDrag(feature, pixel) {
        atualizaGeometry();
    }

    function setZoom(event) {
    	jQuery("#edczoom").val(map.getZoom());
    }

    function recuperarEnderecoPorCep(){
        var endcep = $('[name=edccep]')[0];

        if (!endcep || endcep.value == '' || endcep.value.replace(/[^0-9]/ig, '').length != 8){
            limparDadosEndereco();
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

                       limparDadosEndereco();

                       return false;
                   }

                   if (DNE[0] && DNE.length >= 1) {
                       $('[name=edclogradouro]').val(DNE[0].logradouro);
                       $('[name=edcbairro]').val(DNE[0].bairro);
                       $('[name=mundescricao]').val(DNE[0].cidade);
                       $('[name=muncod]').val(DNE[0].muncod);
                       $('[name=estuf]').val(DNE[0].estado);

            	   }else{
            		   limparDadosEndereco();
            	   }
    		  }
		});
    }

    function limparDadosEndereco(){
    	$('[name=edclogradouro]').val('');
        $('[name=edcbairro]').val('');
        $('[name=mundescricao]').val('');
        $('[name=muncod]').val('');
        $('[name=estuf]').val('');
    } 

        //Método responsável por todas as alterações no mapa
    function atualizaGeometry(event) {
        var geoCollection = new OpenLayers.Geometry.Collection;

        for (var i = 0; i < vlayer.features.length; i++) {
        	newFeature = vlayer.features[i].clone();
        	newFeature.geometry.transform(proj_900913, proj_4326);
        	geoCollection.addComponents(newFeature.geometry);

        	var format = new OpenLayers.Format.WKT();
        	var geometry = format.write(newFeature);

        	jQuery('#edccoordenadas').val(geometry);
        }
        centroid = geoCollection.getCentroid(true);
    }

    function limparMapa(){
        vlayer.removeAllFeatures();
        jQuery('#edccoordenadas').val('');
    }

    function desenhaObjetoInicial(){
        if(jQuery('#edccoordenadas').val()){
        	var polygonFeature = wkt.read(jQuery('#edccoordenadas').val());
        	polygonFeature.geometry.transform(map.displayProjection, map.getProjectionObject());

        	var bounds = polygonFeature.geometry.getBounds();
        	vlayer.addFeatures([polygonFeature]);

        	map.zoomToExtent(bounds);
        	map.zoomTo(zoomInicial);
        }
    }

    //Busca o local de um endereço
    function buscarLocalEndereco() {
        if (!gLocalSearch.results) return;

        var lat  = gLocalSearch.results[0].lat;
        var lng  = gLocalSearch.results[0].lng;
        var zoom = 17;

        var enderecoPoint = new OpenLayers.LonLat(lng, lat);
        enderecoPoint.transform(proj_4326, map.getProjectionObject());
        map.setCenter(enderecoPoint, zoom);
    }
</script>