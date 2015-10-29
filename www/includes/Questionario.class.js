function Questionario(param){
	this.obPerg  	 	= new Pergunta();
	var qrpid 	   	 	= param.qrpid;
	var peridAtual 	 	= param.peridAtual;
	var url	  	   	 	= param.url;
	var obDadosParam 	= "";

	var bntProx	= jQuery('[value="Próximo"]').attr('disabled');
	var bntAnt	= jQuery('[value="Anterior"]').attr('disabled');

	// Obj tela
	var tela = jQuery('#telacentral');
	// Obj tela da árvore
	var telaA = jQuery('#telaarvore');
	// Obj tela do questionário
	var telaQ = jQuery('#telaquestionario');

	var map;
	var vlayer;
	
	this.exibeMapa = function (elemento, coord, zoom){
	    var proj_4326   	= new OpenLayers.Projection("EPSG:4326");
	    var proj_900913 	= new OpenLayers.Projection("EPSG:900913");
	    var lonInicial 		= -47.885858621191524;
	    var latInicial 		= -15.791575490187014;
	    var zoomInicial		= "13";
	    var gLocalSearch	= new GlocalSearch();
	    var coordenadas     = $('[id="' + coord+'"]');
	    var highlightCtrl;
	    var selectCtrl;
	    var campoZoom = zoom ? $('[id="' + zoom+'"]') : null;

		var options = {
			units : "dd",
			controls : [],
			numZoomLevels : 25,
			theme : false,
			projection : proj_900913,
			"displayProjection" : proj_4326,
			eventListeners : { "zoomend" : setZoom }};

		zoomInicial = campoZoom && $(campoZoom).val() != "" ? $(campoZoom).val() : zoomInicial;

		map = new OpenLayers.Map(elemento, options);	
 		
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
		map.addControl(new OpenLayers.Control.Scale("mapScale"));
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

		vlayer = new OpenLayers.Layer.Vector("Questionário", {
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

		$(".btn_clear").click(function() {
			limparMapa();
			return false;
		});

		desenhaObjetoInicial();

	    function endDrag(feature, pixel) {
	        atualizaGeometry();
	    }
	    
	    function setZoom(event) {
	    	if(campoZoom){
	    		$(campoZoom).val(map.getZoom());
	    	}
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

	        	coordenadas.val(geometry);
	        }
	        centroid = geoCollection.getCentroid(true);
	    }

	    function limparMapa(){
	        vlayer.removeAllFeatures();
	        coordenadas.val("");
	    }

	    function desenhaObjetoInicial(){
	        if(coordenadas.val()){
	        	var polygonFeature = wkt.read(coordenadas.val());
	        	polygonFeature.geometry.transform(map.displayProjection, map.getProjectionObject());

	        	var bounds = polygonFeature.geometry.getBounds();
	        	vlayer.addFeatures([polygonFeature]);

	        	map.zoomToExtent(bounds);
	        	map.zoomTo(zoomInicial);
	        }
	    }
	    
	}
	
	this.recuperarEnderecoPorCep = function (id){
        var endcep = $('[name="qencep_'+id+'"]')[0];

        if (!endcep || endcep.value == '' || endcep.value.replace(/[^0-9]/ig, '').length != 8){
            this.limparDadosEndereco(id);
            return false;
        }
        
        var objPai = this;

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

                       objPai.limparDadosEndereco(id);

                       return false;
                   }

                   if (DNE[0] && DNE.length >= 1) {
                       $('[name="qenlogradouro_'+id+'"]').val(DNE[0].logradouro);
                       $('[name="qenbairro_'+id+'"]').val(DNE[0].bairro);
                       $('[name="mundescricao_'+id+'"]').val(DNE[0].cidade);
                       $('[name="muncod_'+id+'"]').val(DNE[0].muncod);
                       $('[name="estuf_'+id+'"]').val(DNE[0].estado);

                       objPai.buscarEnderecoMapa(id);
            	   }else{
            		   objPai.limparDadosEndereco(id);
            	   }
    		  }
		});
	}
	
	this.limparDadosEndereco = function(id){
    	$('[name="qenlogradouro_'+id+'"]').val('');
        $('[name="qenbairro_'+id+'"]').val('');
        $('[name="mundescricao_'+id+'"]').val('');
        $('[name="muncod_'+id+'"]').val('');
        $('[name="estuf_'+id+'"]').val('');
    } 
	
	this.buscarEnderecoMapa = function(id){
        var arEndereco = new Array();
    	var endereco = "";

        if(jQuery('[name="qencep_'+id+'"]').val() && jQuery('[name="qencep_'+id+'"]').val().substr(7,10) != '000'){
            arEndereco.push(jQuery('[name="qencep_'+id+'"]').val());
        }

        if(jQuery('[name="qenlogradouro_'+id+'"]').val()){
            arEndereco.push(jQuery('[name="qenlogradouro_'+id+'"]').val());
        }

        if(jQuery('[name="qennumero_'+id+'"]').val()){
            arEndereco.push(jQuery('[name="qennumero_'+id+'"]').val());
        }

        if(jQuery('[name="qenbairro_'+id+'"]').val()){
            arEndereco.push(jQuery('[name="qenbairro_'+id+'"]').val());
        }

        if(jQuery('[name="mundescricao_'+id+'"]').val()){
            arEndereco.push(jQuery('[name="mundescricao_'+id+'"]').val());
        }

        if(jQuery('[name="estuf_'+id+'"]').val()){
            arEndereco.push(jQuery('[name="estuf_'+id+'"]').val());
        }

        arEndereco.push('Brasil');

        endereco = arEndereco.join(', ');
        
        if(endereco != "" && jQuery('[name="qencoordenadas_'+id+'"]').length > 0){
	        jQuery.ajax({
	      		  type     : "GET",
	      		  url      : "http://maps.google.com/maps/api/geocode/json",
	      		  data     : { address: endereco, sensor : false },
	      		  dataType : "json",
	      		  success  : function (retorno){
	          		  if(retorno && retorno.results && retorno.results[0]){
	          		        var localizacao = retorno.results[0].geometry.location;
	          		        var zoom        = 17;
	          		        var coordenadas = "POINT("+localizacao.lng+" "+localizacao.lat+")";
	
	          	            var polygonFeature = wkt.read(coordenadas);
	                  	    polygonFeature.geometry.transform(map.displayProjection, map.getProjectionObject());
	
	                  	    var bounds = polygonFeature.geometry.getBounds();
	                  	    vlayer.addFeatures([polygonFeature]);
	
	                  	    map.zoomToExtent(bounds);
	                  	    map.zoomTo(zoom);
	
	                  	  jQuery('[name="qencoordenadas_'+id+'"]').val(coordenadas);
	          		  }
	      		  }
	  		});
        }
    }
	
	this.atualizaTela = function (perid, carregandoDiv, carregadoDiv){
		
	    	carregandoDiv = (typeof carregandoDiv == 'undefined') ? true : carregandoDiv;
	    	carregadoDiv  = (typeof carregadoDiv == 'undefined') ? true : carregadoDiv;

		if(carregandoDiv){
		    divCarregando();
		}
	    
	    	perid = perid ? perid : peridAtual;
		var urlParam = [{'name' : "ajax", 'value' : true},
						{'name' : "perid", 'value' : perid},
						{'name' : "qrpid", 'value' : qrpid}];

		urlParam = concatenaArray(urlParam, obDadosParam);

		desabilitaForm(true);

		telaQ.html('<center>carregando...</center>');

		jQuery.ajax({
			    type:	 "POST",
			    url: 	 url,
			    data: 	 urlParam,
			    async: 	 true,
			    success: function(html){
					peridAtual = perid;
					//extrairScript( trataRetornoAjax(html) );
			      	//alert( trataRetornoAjax(html) );
			      	//alert( html );
			      	//alert( peridAtual );
			      	tela.html( trataRetornoAjax(html) );
			      	verificaButtons();
			      	
			      	if(carregadoDiv){
    			      	    divCarregado();
			      	}
			    }
			   });
	}

	this.buscaSubPergunta = function (param, obj){

		var div, arrVal, z, valOption;
		var perid = param.perid;
		var itpid = param.itpid;


		if ( !jQuery('#linha_' + perid + '_' + itpid )[0] ){
			this.closeSubPerguntas(perid);
//			jQuery('#tr_subpergunta_' + perid).hide();
			return;
		}

		arrVal = jQuery(obj).val();

		if ( jQuery(obj).attr("type") == 'radio' || (jQuery(obj).attr('tagName') == 'SELECT' && typeof(arrVal) != 'object' ) ){
			this.closeSubPerguntas(perid);
		}

		if (typeof(arrVal) == 'object'){
			jQuery('option:not(:selected)', obj).each(function (){
				valOption = jQuery(this).val();
				jQuery('#linha_' + perid + '_' + valOption ).hide();
			});
		}else{
			arrVal = new Array();
			arrVal.push( itpid );
		}

		desabilitaForm(true);
		for (z=0; z < arrVal.length; z++){
			itpid = arrVal[z];

			var urlParam = [{'name' : 'ajax',  'value' : true},
							{'name' : 'perid', 'value' : perid},
							{'name' : 'qrpid', 'value' : qrpid},
							{'name' : 'itpid', 'value' : itpid}];

			urlParam = concatenaArray(urlParam, obDadosParam);

			if ( obj.checked || jQuery(obj).attr('tagName') == 'SELECT' ){
	//			jQuery('#tr_subpergunta_' + perid).show();
				div = jQuery('#linha_' + perid + '_' + itpid ).html();
				jQuery('#linha_' + perid + '_' + itpid ).show();
				if (div.length > 10){
					continue;
				}
				jQuery('#linha_' + perid + '_' + itpid ).html('carregando...');
			}else{
				jQuery('#linha_' + perid + '_' + itpid ).hide();
				continue;
			}


//			desabilitaForm(true);

			jQuery.ajax({
				    type:	 "POST",
				    url: 	 url,
				    data: 	 urlParam,
				    async: 	 false,
				    success: function(html){
				     	peridAtual = perid;
				     	jQuery('#linha_' + perid + '_' + itpid ).show();
				     	trataDiv = '<div>' + trataRetornoAjax(html);
				     	jQuery('#linha_' + perid + '_' + itpid ).html( trataDiv );
//						desabilitaForm(false);
						}
				   });

		}
		desabilitaForm(false);

	}

	this.salvar = function (perid, sairPag){
		
		if(!sairPag && sairPag !== false){
			sairPag = true;
		}

		jQuery("[name^='perg_']").each(function (){
			jQuery(this).attr("name", "perg[" + jQuery(this).attr('name').substring(5) + "]");
		});

/*		for (var i in dados ){
			if(dados[i].name.indexOf('perg_') == 0){
				jQuery("[name='" + dados[i].name + "']").attr('name', 'perg[' + dados[i].name.substring(5, dados[i].name.length) + ']');
				dados[i].name = 'perg[' + dados[i].name.substring(5, dados[i].name.length) + ']';
			}
		}
*/
		msgValidacao = this.obPerg.validaCampoObrig();
		if ( msgValidacao != "" ){
			alert(msgValidacao);
			return false;
		}

		divCarregando();
		
/*		if (dados.length == 0 || !condicao){
			alert('Responda a pergunta antes de salvar!');
			return;
		}*/
		
		dados = new Array();
		if( jQuery('#idTabela').val() || jQuery('#identExterno').val() || jQuery('[name^=qencep_]').length > 0 ){
			dados = jQuery('#formulario').serializeArray();
		} else {
			dados = this.obPerg.pegaDadosValido();
		}

		functionPosAcao = this.obPerg.getFuncaoPergunta();
		functionPosAcao = functionPosAcao.replace("qrpid", qrpid);
		functionPosAcao = functionPosAcao.replace("perid", perid);


		dados.push({
					name  : "salvar_questionario",
					value : true
					});
		/*
		 * Após salvar, busca a pergunta ANTERIOR | ATUAL | PRÓXIMA
		 */
		dados.push({
					name  : "ajax",
					value : true
					});

		dados.push({
					name  : "perid",
					value : perid
					});

		dados.push({
					name  : "qrpid",
					value : qrpid
					});

		dados = concatenaArray(dados, obDadosParam);
		/*
		 * FIM - Após salvar, busca a pergunta ANTERIOR | ATUAL | PRÓXIMA
		 */

		if( jQuery(':file').length > 0 ){
			peridAtual = perid;
			document.getElementById('perid').value = perid;
			jQuery('[name=formulario]').append("<input type='hidden' id='salvar_questionario' name='salvar_questionario' value='true' />");
			jQuery('[name=formulario]').append("<input type='hidden' id='qrpid' name='qrpid' value='"+qrpid+"' />");
			jQuery('[name=formulario]').after("<iframe id='frameQuestionario' name='frameQuestionario' style='display: none;'></iframe>");
			jQuery('[name=formulario]').attr('target', 'frameQuestionario');

			document.formulario.submit();

			document.getElementById('frameQuestionario').onload= function() {
		        quest.atualizaTela(perid, false, true);
        		jQuery('[name=formulario]').removeAttr('target');
        		jQuery('#salvar_questionario').remove();
        		jQuery('#frameQuestionario').remove();
        
        		verificaButtons();
        		eval( functionPosAcao );
        		desabilitaForm(false);
		    };
		}else{

			desabilitaForm(true);

			jQuery.ajax({
				type:	 "POST",
				url: 	 url,
				data: 	 dados,
				async: 	 true,
				success: function(html){
					if(sairPag){
						peridAtual = perid;
						//alert('Operação realizada com sucesso!');
						//alert( html );
						tela.html( trataRetornoAjax(html) );
						verificaButtons();
						eval( functionPosAcao );
						divCarregado();
						desabilitaForm(false);
					}
				}
			});
		}
		
		return true;
	}

	this.closeSubPerguntas =  function (perid){
		jQuery('[id*="linha_' + perid + '_"]').each(function (){
						jQuery(this).hide();
					})
/*
		var linha = jQuery('#tr_subpergunta_' + perid);
		if (linha[0]){
			jQuery('#tr_subpergunta_' + perid + ' td div').each(function (){
				jQuery(this).hide();
			})
		}
*/
	}

	this.duplicaGrupo =  function (grpid, perid){
	    perid = perid ? perid : null;
            if( confirm( ('Deseja realmente utilizar outra página?')) ){
    
                var urlParam = [{'name' : "ajax", 'value' : true},
                                {'name' : "duplicaGrupo", 'value' : true},
                                {'name' : "grpid", 'value' : grpid}];
    
                urlParam = concatenaArray(urlParam, obDadosParam);

                var quest = this;
                divCarregando();
                
                jQuery.ajax({
                        type:	 "POST",
                        url: 	 url,
                        data: 	 urlParam,
                        async: 	 true,
                        success: function(html){
                            if(perid){
                        	quest.atualizaTela(perid, false, true);
                            }else{
                        	divCarregado();
                            }
                        }
                       });
            }
	}

	this.carregaParamUrl = function (obParam){
		obDadosParam = eval( obParam );
//		alert( obDadosParam );
	}

	function concatenaArray(arrElemento, arrConcatena){
		for (var indArr=0; indArr < arrConcatena.length; indArr++){
			if( arrConcatena[indArr].name != 'perid' && arrConcatena[indArr].name.indexOf('perg') < 0){
				arrElemento.push(arrConcatena[indArr]);
			}
		}
//		for (indArr=0; indArr < arrElemento.length; indArr++){
//			alert( arrElemento[indArr].name + ' - ' + arrElemento[indArr].value )
//		}
		return arrElemento;
	}

	function trataRetornoAjax(html){
	   	var iniReturn = html.indexOf('<table');
//	   	iniReturn = iniReturn > -1 ? iniReturn : html.indexOf('<table');;
	   	iniReturn = iniReturn > -1 ? iniReturn : 0;

	   	var fimReturn = html.lastIndexOf('</table>');
//		fimReturn = fimReturn ? fimReturn : html.indexOf('</table>');
		fimReturn = fimReturn ? fimReturn : html.length;

		var retorno = html.substr(iniReturn, fimReturn);
		return retorno;
	}

	function desabilitaForm(param){
		jQuery('#formulario select,textarea,input').each(function (){
			if(jQuery(this).attr("value")){
				if(jQuery(this).attr("name") != "csFiltroArvore"){
					if ((jQuery(this).attr("value").indexOf('Próximo') > -1 && !bntProx)
							||
							(jQuery(this).attr("value").indexOf('Anterior') > -1 && !bntAnt)
							||
							(jQuery(this).attr("value").indexOf('Anterior') == -1 && jQuery(this).attr("value").indexOf('Próximo') == -1) ){
						jQuery(this).attr("disabled", param);
					}
					if ( ( (jQuery(this).attr("value") == 'Salvar Anterior')
							|| (jQuery(this).attr("value") == 'Salvar')
							|| (jQuery(this).attr("value") == 'Salvar Próximo') )
							&&  $('#preview').val() == 1
					){
						jQuery(this).attr("disabled", true);
					}
				}
			}
		});
	}

	function verificaButtons(){
		bntProx	= jQuery('[value="Próximo"]').attr('disabled');
		bntAnt	= jQuery('[value="Anterior"]').attr('disabled');
	}
	
	this.fecharQuestionario = function(url, perid){
		jQuery('#dialog-mensagem-conteudo').html(("Você gostaria de salvar as modificações?"));
		
		var quest = this;
		jQuery( "#dialog-mensagem" ).dialog({
		      modal: true,
		      buttons: {
		        'Yes': function() {
		          jQuery( this ).dialog( "close" );
		          if(quest.salvar(perid, false) != false){
		        	  window.location = url;
		          }
		        },
		        'No': function() {
		        	window.location = url;
		        }
		      }
		    }).dialog("open");
	}
	
	this.submeterQuestionario = function(perid){
		jQuery('#dialog-mensagem-conteudo').html(("Você tem certeza que deseja submeter o questionário?"));
		
		var quest = this;
		jQuery( "#dialog-mensagem" ).dialog({
			modal: true,
			buttons: {
				'Yes': function() {
					jQuery( this ).dialog( "close" );
					
					jQuery.ajax({
						type:	 "POST",
						url: 	 url,
						data: 	 {instrucao : 'submeter-questionario', 'ajax':true},
						async: 	 false,
						success: function(html){
							divCarregado();
							quest.salvar(perid);
						}
					});
				},
				'No': function() {
					jQuery( this ).dialog( "close" );
				}
			}
		}).dialog("open");
	}

}

function Pergunta(){
	var pergunta 	    = new Array();
	var msgValObrig	    = '';
	var functionPosAcao = '';

	this.add = function (obParam){
		var nivelProx;

		if ( pergunta.length == 0 ){
			pergunta[0] 		   		            = new Array();
			pergunta[0]["id"] 	   		            = obParam.id;
			pergunta[0]["obrig"]   		            = obParam.obrig;
			pergunta[0]["tipo"]    		            = obParam.tipo;
			pergunta[0]["idPai"]   		            = obParam.idPai;
			pergunta[0]["itemPai"] 		            = obParam.itemPai;
			pergunta[0]["txt"] 	   		            = obParam.txt;
			pergunta[0]["perposacao"] 	            = obParam.perposacao;
			pergunta[0]["perextensao"] 	            = obParam.perextensao;
			pergunta[0]["perdescricaoobrigatoria"] 	= obParam.perdescricaoobrigatoria;
			pergunta[0]["new"]     					= new Array();
		}else{
			elementPergunta = this.buscaElemento( obParam.idPai );
			if(elementPergunta && elementPergunta["new"]){
				nivelProx 		= (elementPergunta["new"].length == 0 ? 0 : elementPergunta["new"].length);

				elementPergunta["new"][nivelProx] 		     	                = new Array();
				elementPergunta["new"][nivelProx]["id"]      	                = obParam.id;
				elementPergunta["new"][nivelProx]["obrig"]   	                = obParam.obrig;
				elementPergunta["new"][nivelProx]["tipo"]    	                = obParam.tipo;
				elementPergunta["new"][nivelProx]["idPai"] 	 	                = obParam.idPai;
				elementPergunta["new"][nivelProx]["itemPai"] 	                = obParam.itemPai;
				elementPergunta["new"][nivelProx]["txt"]	 	                = obParam.txt;
				elementPergunta["new"][nivelProx]["perposacao"]	                = obParam.perposacao;
				elementPergunta["new"][nivelProx]["perextensao"] 	            = obParam.perextensao;
				elementPergunta["new"][nivelProx]["perdescricaoobrigatoria"] 	= obParam.perdescricaoobrigatoria;
				
				elementPergunta["new"][nivelProx]["new"]     	= new Array();
			}
		}

	}

	this.clean = function (){
		pergunta = new Array();
	}
/*
	this.buscaElementoPai = function (elementPergunta, idPai){
		var i;

		if (elementPergunta["id"] == idPai){
			return elementPergunta;
		}else{
			for(i=0; i < elementPergunta["new"].length; i++){
				return this.buscaElementoPai( elementPergunta["new"][i], idPai );
			}
		}
	}
*/
	this.buscaElemento = function (id, elementPergunta){
		var i;
		elementPergunta = elementPergunta ? elementPergunta : pergunta[0];

		if (elementPergunta["id"] == id){
			return elementPergunta;
		}else{
			for(i=0; i < elementPergunta["new"].length; i++){
				elemento = this.buscaElemento( id, elementPergunta["new"][i] );

				if (elemento){
					return elemento;
				}
			}
		}
	}

	this.buscaArrElementoPorItem = function ( itemPai, elementPergunta, arrElementPergunta ){
		var i;

		arrElementPergunta = arrElementPergunta ? arrElementPergunta : new Array()
		elementPergunta    = elementPergunta ? elementPergunta : pergunta[0];

		if (elementPergunta["itemPai"] == itemPai){
			arrElementPergunta.push( elementPergunta );
		}

		for(i=0; i < elementPergunta["new"].length; i++){
			arrElementPergunta = this.buscaArrElementoPorItem( itemPai, elementPergunta["new"][i], arrElementPergunta );
		}

		return arrElementPergunta;
	}



	this.validaCampoObrig = function (perg){
		var i, ii, b, valPerg, subPerg, msgRetorno;
		var msg = new Array();

		perg = perg ? perg : pergunta[0];

		if(perg.tipo == "END" && perg.obrig == true){
			var sufixo = 'perg[' + perg.id + ']';
			if($('[name="qencep_'+sufixo+'"]').val() == ''){
				msg.push('O campo "CEP" é obrigatório!');
			}
			
			if($('[name="qenlogradouro_'+sufixo+'"]').val() == ''){
				msg.push('O campo "Logradouro" é obrigatório!');
			}
			
			if($('[name="qennumero_'+sufixo+'"]').val() == ''){
				msg.push('O campo "Numero" é obrigatório!');
			}
		    
			if($('[name="qenbairro_'+sufixo+'"]').val() == ''){
				msg.push('O campo "Bairro" é obrigatório!');
			}
		    
		    if($('[name="muncod_'+sufixo+'"]').val() == ''){
		    	msg.push('O campo "Município" é obrigatório!');
			}
		    
		    if($('[name="estuf_'+sufixo+'"]').val() == ''){
		    	msg.push('O campo "UF" é obrigatório!');
			}
		    
		    if($('[name="qencoordenadas_'+sufixo+'"]').length > 0 
		    && $('[name="qencoordenadas_'+sufixo+'"]').val() == ''){
		    	msg.push('A definição das coordenadas no mapa é obrigatória!');
		    }
		}else if(perg.tipo != "ARQ" ){
			if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'radio' || jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'checkbox' ){
				valPerg = new Array();
				jQuery('[name*="perg[' + perg.id + ']"]:checked').each(function (){
					valPerg.push(jQuery(this).val());
				});
			}else if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('tagName') == "SELECT" ){
				valPerg = new Array();
				jQuery('[name*="perg[' + perg.id + ']"] option:selected').each(function (){
					if (trim(jQuery(this).val())){
						valPerg.push(jQuery(this).val());
					}
				});
			}else{
				valPerg = jQuery('[name*="perg[' + perg.id + ']"]').val();
			}
			valPerg = (typeof(valPerg) != 'object' ? trim( valPerg ) : valPerg);
//		valPerg = (typeof(valPerg) != 'object' ? jQuery.trim( valPerg ) : valPerg);
			if ( perg.obrig == true && valPerg == "" ){
				msg.push(('O campo "'+perg.txt+'" é obrigatório!'));
			}else if ( typeof(valPerg) == 'object' ){
				for (ii=0; ii < valPerg.length; ii++){
					subPerg = this.buscaArrElementoPorItem( valPerg[ii] );
					for(b=0; b < subPerg.length; b++){
						msgRetorno = this.validaCampoObrig( subPerg[b] );
						if ( msgRetorno ){
							msg.push(msgRetorno);
						}
					}
				}
			}
		}

		if(perg.tipo == "ARQ"
			&& perg.perdescricaoobrigatoria
			&& perg.perdescricaoobrigatoria == 'true'
			&& jQuery('[name*="dsc_perg[' + perg.id + ']"]').val() == ''){
			msg.push('O campo "Descrição" é obrigatório!');
		}

		if(perg.tipo == "ARQ"
	    && perg.obrig
	    && perg.obrig == true
	    && jQuery('[name^="perg[' + perg.id + ']"]:visible').val() == ''){
			msg.push('O campo "'+ perg.txt + '" é obrigatório!');
		}

		if(perg.tipo == "ARQ"
			&& jQuery('[name*="dsc_perg[' + perg.id + ']"]').length > 0
			&& jQuery('[name*="dsc_perg[' + perg.id + ']"]').val() != ''
			&& jQuery('[name^="perg[' + perg.id + ']"]:visible').val() == ''){
			msg.push('O campo "'+ perg.txt + '" deve ser preenchido para que uma "Descrição" seja gravada!');
		}

		msg = msg.join('\n');

		return ( msg );
	}


	this.validaRegra = function ( perg ){

		perg = perg ? perg : pergunta[0];
		var msg = "";

		var resposta = jQuery('#hiddenRegra').val();
		var tituloPergunta = jQuery('#hiddenRegraTitulo').val();
		if ( resposta ){
			var quebra = resposta.split(';');

			if ( !quebra[2] ){
//				return 'A pergunta "'+ tituloPergunta +'" relacionada à regra, ainda não foi preenchida!';
				('A pergunta "'+tituloPergunta+'" relacionada à regra, ainda não foi preenchida!');
			}
			quebra[0] = jQuery('[name*="perg[' + perg.id + ']"]').val();

			resposta = quebra.join(' ');

			resposta = resposta.replace(/;/g,' ');

			if(!eval(resposta)){
//				return 'A regra não está valida!';
				var operador;
				switch (quebra[1]) {
				 case '<':
					 operador = ('menor');
			        	break;
			        case '<=':
			        	operador = ('menor ou igual');
			        	break;
			        case '>':
			        	operador = ('maior');
			        	break;
			        case '>=':
			        	operador = ('maior ou igual');
			        	break;
			        case '=':
			        	operador = ('igual');
			        	break;
			        case '==':
			        	operador = ('igual');
			        	break;
			        default:
			        	operador = quebra[1];
			        	break;
				}

				return 'O valor informado nesta pergunta deve ser ' + operador + ' ao valor informado na pergunta "' + tituloPergunta + '"!';
//				return ('O valor informado nesta pergunta deve ser %s ao valor informado na pergunta "%s"!', Array(true,operador,tituloPergunta));
			}else{
				//return 'tudo OK!  '+resposta;
				return '';
			}
		}else{
			//return 'sem regra!';
			return '';
		}
	}


	this.pegaDadosValido = function (perg, ArrObDados){
		var ii, bbb;
		var valPerg 	= new Array();
		functionPosAcao = ArrObDados ? functionPosAcao : "";
		ArrObDados  	= ArrObDados ? ArrObDados : new Array();
		perg 			= perg ? perg : pergunta[0];

		if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'radio' || jQuery('[name*="perg[' + perg.id + ']"]').attr('type') == 'checkbox' ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"]:checked').each(function (){
				valPerg.push(jQuery(this).val());
			});
		}else if ( jQuery('[name*="perg[' + perg.id + ']"]').attr('tagName') == "SELECT" ){
			valPerg = new Array();
			jQuery('[name*="perg[' + perg.id + ']"] option:selected').each(function (){
				if ( trim(jQuery(this).val()) ){
					valPerg.push(jQuery(this).val());
				}
			});
		}else{
			valPerg = jQuery('[name*="perg[' + perg.id + ']"]').val();
		}

		if ( typeof(valPerg) == 'object' ){
			for (ii=0; ii < valPerg.length; ii++){
				ArrObDados.push({
								 "name"  : 'perg[' + perg.id + '][]',
								 "value" : valPerg[ii]
								});

				// Concatena funções
				functionPosAcao += perg.perposacao + ';';
//				functionPosAcao.replace("perid", perg.id);

				var subPerg = this.buscaArrElementoPorItem( valPerg[ii] );

				for(bbb=0; bbb < subPerg.length; bbb++){
					ArrObDados = this.pegaDadosValido( subPerg[bbb], ArrObDados );
				}
			}
		}else{
			ArrObDados.push({
							 "name"  : 'perg[' + perg.id + ']',
							 "value" : valPerg
							});

			// Concatena funções
			functionPosAcao += perg.perposacao + ';';
//			functionPosAcao.replace("perid", perg.id);
		}

		return ArrObDados;
	}

	this.getFuncaoPergunta = function(){
		return functionPosAcao;
	}

	this.validarExtensao = function(extensao, input){
		extensao = extensao ? extensao : pergunta[0]["perextensao"];
		if(extensao != ''){
			var valor = jQuery(input).val();
			if(valor != ''){
				valor = valor.split('.');
				if(extensao.indexOf(valor[1]) == -1){
					alert(("O arquivo informado é inválido! \nSomente arquivos dos seguintes tipos estão habilitados para esta resposta:") + " " + extensao);
					$(input).val('');
				}
			}
		}
	}
	
	this.validarEmail = function(elemento){
		if( jQuery(elemento).val() != '' && !validaEmail(jQuery(elemento).val())){
			alert('O Email informado não é válido!');
			jQuery(elemento).val('');
			return false;
		}
	}

	this.enviarOutroArquivo = function(nome){
		jQuery('[id="arqid_'+nome+'"]').val('');
		jQuery('[id="spnArquivo'+nome+'"]').hide();
		jQuery('[id="spnArquivoNovo'+nome+'"]').show();
	}

	this.downloadArquivo = function(arquivo){
		jQuery('[name=formulario]').append("<input type='hidden' name='ajax' value='download_arquivo' />");
		jQuery('[name=formulario]').append("<input type='hidden' name='arqid' value='"+arquivo+"' />");
		jQuery('[name=formulario]').append("<input type='hidden' name='download_arquivo' value='true' />");
		jQuery('[name=formulario]').removeAttr("onsubmit");
		jQuery('[name=formulario]').submit();
		
		jQuery('[name=formulario] [name=arqid]').remove();
		jQuery('[name=formulario] [name=ajax]').remove();
		jQuery('[name=formulario] [name=download_arquivo]').remove();
		jQuery('[name=formulario]').attr("onsubmit", "return false;");
	}
	
}

/*
var p = new Pergunta();
p.add({"id" : 1 , "obrig" : true, "tipo" : "CK" ,"idPai" : "", "itemPai" : ""});
p.add({"id" : 2 , "obrig" : true, "tipo" : "CK" ,"idPai" : 1, "itemPai" : "224"});
p.add({"id" : 3 , "obrig" : false,"tipo" : "CK" ,"idPai" : 2, "itemPai" : "224"});
p.add({"id" : 4 , "obrig" : true, "tipo" : "CK" ,"idPai" : 2, "itemPai" : "224"});
p.add({"id" : 5 , "obrig" : false,"tipo" : "CK" ,"idPai" : 4, "itemPai" : "224"});
p.add({"id" : 6 , "obrig" : false,"tipo" : "CK" ,"idPai" : 4, "itemPai" : "224"});
p.add({"id" : 7 , "obrig" : false,"tipo" : "CK" ,"idPai" : 1, "itemPai" : "224"});
p.add({"id" : 8 , "obrig" : true, "tipo" : "CK","idPai" : 5, "itemPai" : "224"});
p.add({"id" : 9 , "obrig" : true, "tipo" : "CK","idPai" : 8, "itemPai" : "224"});

alert( p.validaCampoObrig() );
*/
//alert( p.validaCampoObrig() );

/*
ele = p.buscaElemento(9);
alert(ele['idPai']);

ele = p.buscaArrElementoPorItem(224);
alert(ele[0]['id'] + '\n'
		+ ele[1]['id'] + '\n'
		+ ele[2]['id'] + '\n'
		+ ele[3]['id'] + '\n'
		+ ele[4]['id'] + '\n'
		+ ele[5]['id'] + '\n'
		+ ele[6]['id'] + '\n'
		+ ele[7]['id']);
*/

