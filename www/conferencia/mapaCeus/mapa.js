var map;
var proj_4326 = new OpenLayers.Projection('EPSG:4326');
var proj_900913 = new OpenLayers.Projection('EPSG:900913');
var vlayerResults;
var highlightCtrl;
var selectCtrlResults;
var popup;

var lonInicial = -47.885858621191524;
var latInicial = -15.791575490187014;

var zoomInicial = '4';

function inicializarMapa(idDiv) {
	var options = {
		units : "dd",
		controls : [],
		numZoomLevels : 25,
		theme : false,
		projection : proj_900913,
		'displayProjection' : proj_4326
	};

	map = new OpenLayers.Map(idDiv, options);

	var google_satellite = new OpenLayers.Layer.Google("Google Maps Satélite",
			{
				type : google.maps.MapTypeId.SATELLITE,
				animationEnabled : true,
				sphericalMercator : true,
				maxExtent : new OpenLayers.Bounds(-20037508.34, -20037508.34,

				20037508.34, 20037508.34)
			});
	var google_hybrid = new OpenLayers.Layer.Google("Google Maps Híbrido", {
		type : google.maps.MapTypeId.HYBRID,
		animationEnabled : true,
		sphericalMercator : true,
		maxExtent : new OpenLayers.Bounds(-20037508.34, -20037508.34,
				20037508.34, 20037508.34)
	});

	var google_normal = new OpenLayers.Layer.Google("Google Maps Normal", {
		animationEnabled : true,
		sphericalMercator : true,
		maxExtent : new OpenLayers.Bounds(-20037508.34, -20037508.34,
				20037508.34, 20037508.34)
	});

	var google_physical = new OpenLayers.Layer.Google("Google Maps Físico", {
		type : google.maps.MapTypeId.TERRAIN,
		animationEnabled : true,
		sphericalMercator : true,
		maxExtent : new OpenLayers.Bounds(-20037508.34, -20037508.34,
				20037508.34, 20037508.34)
	});

	map.addLayers([ google_normal, google_satellite, google_hybrid,
			google_physical ]);

	map.addControl(new OpenLayers.Control.Navigation());
	map.addControl(new OpenLayers.Control.Zoom());
	map.addControl(new OpenLayers.Control.MousePosition());
	map.addControl(new OpenLayers.Control.ScaleLine());
	map.addControl(new OpenLayers.Control.Scale('mapScale'));
	map.addControl(new OpenLayers.Control.LayerSwitcher());

	style1 = new OpenLayers.Style({
		pointRadius : "8",
		fillColor : "#176D00",
		fillOpacity : "0.4",
		strokeColor : "#197700",
		strokeWidth : 1.0,
		graphicZIndex : 1,
		externalGraphic : "/imagens/map_point/map-point2.png",
		graphicOpacity : 1,
		graphicWidth : 20,
		graphicHeight : 34,
		graphicXOffset : -14,
		graphicYOffset : -27
	});

	style3 = new OpenLayers.Style({
		pointRadius : "8",
		fillColor : "#30E900",
		fillOpacity : "0.4",
		strokeColor : "#197700",
		strokeWidth : 1.0,
		graphicZIndex : 1
	});

	style1R = new OpenLayers.Style({
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

	style2R = new OpenLayers.Style({
		pointRadius : "8",
		fillColor : "#ff776b",
		fillOpacity : "0.4",
		externalGraphic : "/imagens/map_point/map-point1s.png",
		strokeColor : "#6e0900",
		strokeWidth : 2.0,
		graphicZIndex : 1
	});

	style3R = new OpenLayers.Style({
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

	var vlayerResultsStyles = new OpenLayers.StyleMap({
		"default" : style1R,
		"select" : style2R,
		"temporary" : style3R
	});

	vlayerResults = new OpenLayers.Layer.Vector("Obras", {
		styleMap : vlayerResultsStyles,
		rendererOptions : {
			zIndexing : true
		}
	});

	map.addLayer(vlayerResults);

	map.setLayerIndex(vlayerResults, 1);

	highlightCtrl = new OpenLayers.Control.SelectFeature([ vlayerResults ], {
		hover : true,
		highlightOnly : true,
		renderIntent : "temporary"
	});
	// Adiciona evento ao colocar o mouse em cima do feature
	highlightCtrl.events.register('featurehighlighted', null,
			onFeatureMouseOver);
	highlightCtrl.events.register('featureunhighlighted', null,
			onFeatureMouseOut);

	map.addControl(highlightCtrl);

	// Results
	selectCtrlResults = new OpenLayers.Control.SelectFeature(vlayerResults, {
		clickout : true,
		toggle : false,
		multiple : false,
		hover : false,
		onSelect : selecionarFeature,
		onUnselect : deSelecionarFeature,
		renderIntent : "select"
	});
	map.addControl(selectCtrlResults);
	// Results

	wkt = new OpenLayers.Format.WKT();

	var startPoint = new OpenLayers.LonLat(lonInicial, latInicial);
	startPoint.transform(proj_4326, map.getProjectionObject());
	map.setCenter(startPoint, zoomInicial);

	highlightCtrl.activate();
	selectCtrlResults.activate();

	buscarDados();

	$('.baseLbl').html('Mapa');
	$('.dataLbl').html('Itens');
}

function limparMapa() {
	vlayerResults.removeAllFeatures();
}

function desenhaMapaResults(geometry) {
	$('#lblMapa').html('&nbsp;');

	limparMapa();

	if (geometry && typeof geometry != 'object') {
		geometry = eval(geometry);
	}

	var features = new Array();
	if (geometry.length > 0) {
		for ( var i in geometry) {
			if (geometry[i].edccoordenadas) {
				var polygonFeature = wkt.read(geometry[i].edccoordenadas);
				polygonFeature.geometry.transform(map.displayProjection, map
						.getProjectionObject());
				polygonFeature.dados = geometry[i]; // Adiciona os valores
													// originais no objeto do
													// feature

				// Controla a imagem que será utilizada nos pontos
				if (polygonFeature.geometry.CLASS_NAME == 'OpenLayers.Geometry.Point'
						&& geometry[i].uniid) {
					var numIcone = '1';

					var styleTmp = {
						pointRadius : "8",
						fillColor : "#702a24",
						fillOpacity : "0.4",
						strokeColor : "#320400",
						strokeWidth : 2.0,
						graphicZIndex : 1,
						externalGraphic : "/imagens/map_point/map-point"
								+ numIcone + ".png",
						graphicOpacity : 1,
						graphicWidth : 20,
						graphicHeight : 34,
						graphicXOffset : -14,
						graphicYOffset : -27
					};

					polygonFeature.style = styleTmp;
				}

				features.push(polygonFeature);
			}
		}

		vlayerResults.addFeatures(features);
	}
	carregando(false);
}

function carregando(visible) {

	if (visible) {
		$('#carregando').show();
	} else {
		$('#carregando').hide();
	}
}

function onFeatureMouseOver(evt) {
	feature = evt.feature;
	if (feature.dados.obrdesc) {
		var dados = feature.dados.obrdesc;
		$('#lblMapa').html(dados);
	}
}

function selecionarFeature(feature) {
	var html = "<label class='lblBalaoTitulo'>CEU</label><img style='float:right; cursor: pointer;' title='Fechar' src='/imagens/fechar.jpeg' onclick='deSelecionarFeature();' /><br />"
			+ "<label class='lblBalao'>Código:</label> "
			+ feature.dados.ceucodigo
			+ "<br />"
			+ "<label class='lblBalao'>Nome:</label> "
			+ feature.dados.ceunome
			+ "<br />"
			+ "<label class='lblBalao'>Município:</label> "
			+ feature.dados.mundescricao
			+ "<br />"
			+ "<label class='lblBalao'>UF:</label> "
			+ feature.dados.estuf
			+ "<br />"
			+ "<label class='lblBalao'>Cadastro:</label> "
			+ feature.dados.ceudtcadastro
			+ "<br />"
			+ "<label class='lblBalao'>Cadastrado por:</label> "
			+ feature.dados.usunome
			+ "<br />"
			+ "<br />"
			+ "<a href='?modulo=sistema/apoio/incluirCeu&acao=A&ceuid="
			+ feature.dados.ceuid
			+ "' title='Acessar dados'>"
			+ "Acessar Dados" + "</a>";

	popup = new OpenLayers.Popup.FramedCloud("preview", feature.geometry
			.getBounds().getCenterLonLat(), new OpenLayers.Size(300, 180),
			html, null, false);
	popup.autoSize = false;

	feature.popup = popup;
	popup.feature = feature;
	map.addPopup(popup, true);

	$('#lblMapa').html('&nbsp;');
}

function deSelecionarFeature() {
	map.removePopup(popup);

	selectCtrlResults.unselectAll();

	selectCtrlResults.deactivate();
	selectCtrlResults.activate();
}

function onFeatureMouseOut(evt) {
	$('#lblMapa').html('&nbsp;');
}

function gerenciarFiltros(abrir) {
	$('#mapaPainelConteudo').animate({
		width : 'toggle'
	});
}

function buscarDados() {
	limparMapa();
	carregarObras();
}

function carregarObras() {
	carregando(true);
	$
			.ajax({
				url : '/conferencia/mapaCeus/dadosMapa.php',
				data : {
					'act' : 'buscarCeus'
				},
				dataType : 'JSON',
				type : 'POST',
				asyc : true,
				success : desenhaMapaResults,
				error : function() {
					carregando(false);
					$('#lblMapa')
							.html(
									'<img src="/imagens/atencao.png" style="vertical-align: top;"/>&nbsp;<label style="font-weight: normal; color: red;">Não foi possível carregar os dados.</label>');
				}
			});
}

function gerenciaDiv(tipo) {
	$('.divPainel').hide(200);
	if ($('#' + tipo + ':visible').length) {
		$('#' + tipo).hide(200);
	} else {
		$('#' + tipo).show(200);
	}
}
