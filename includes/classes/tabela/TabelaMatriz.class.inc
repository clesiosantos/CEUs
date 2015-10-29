<?php
abstract class TabelaMatriz
{
	private $arqView;
	protected $mantemSessao;
//	protected $view;

//	public function __construct($entid=null, $type=self::TIPOPESSOA, $mantemSessao=true)
	public function __construct()
	{
//		$entid = (empty($entid) && $mantemSessao) ? $_SESSION['agenda'][$type]['entid'] : $entid;
//		if (!empty($entid)){
//			$entidade = new Entidade();
//			$this->entidade = $entidade->recuperarDados($entid, array("e.entstatus = 'A'"));
//		}else{
//			$this->entidade = array();
//		}
//		$this->mantemSessao = $mantemSessao;
	}

	protected function _initJavaScript()
	{
	?>
		<script type="text/javascript">
		function TableManager()
		{
			var toolTabela = new ToolTip();
			toolTabela.config({url   : '',
							   id    : 'form_tabela',
							   css   : {width  : '500px',
									    height : '80px'}});

//			var toolGrupoColuna = new ToolTip();
//			toolGrupoColuna.config({url   : location.href + '&op=telagrupocoluna',
//								    id    : 'form_grupo_coluna',
//								    css   : {width  : '500px',
//										     height : '70px'}});
//
//			var toolColuna = new ToolTip();
//			toolColuna.config({url   : location.href + '&op=telacoluna',
//							   id    : '',
//							   css   : {width  : '500px',
//							   			height : '140px'}});
//
//			var toolGrupoLinha = new ToolTip();
//			toolGrupoLinha.config({url   : location.href + '&op=telagrupolinha',
//								   id    : 'form_grupo_linha',
//								   css   : {width  : '500px',
//										    height : '70px'}});
//
//			var toolLinha = new ToolTip();
//			toolLinha.config({url   : location.href + '&op=telalinha',
//							  id    : '',
//							  css   : {width  : '500px',
//									   height : '70px'}});

			//	var codigo = new Object();
			var objElement = this;
			var indiceEdicao = "";
			var indiceGrupoColuna = 0;
			var indiceGrupoLinha = 0;

			var table  		   = new Object();
			table.prop  	   = new Array();
			table.prop['text'] = '<?= ('Título Tabela') ?>';
			table.grupoLinha   = new Array();
			table.grupoColuna  = new Array();

			/***
			* Tabela - Início
			*/
			this.editTabela = function()
			{
				toolTabela.config({url   : location.href + '&op=telatabela',
								   id    : 'form_tabela',
								   css   : {width  : '500px',
										    height : '80px'}})
						  .open(true);
				$('[name=titulo]').val(table.prop['text']);
				$('#form_tabela').html(table.prop['text']);
			}

			this.saveTabela = function(text)
			{
				toolTabela.close();
				text  = text || 0;
				table.prop['text'] = (text ? text : $('[name=titulo]').val() || '');
				$('#form_tabela').html( (text ? text : $('[name=titulo]').val() || '') );
			}
			/***
			* Tabela - Fim
			*/
			/***
			* Grupo coluna - Início
			*/
			this.addGrupoColuna = function(indice)
			{
				toolTabela.config({url   : location.href + '&op=telagrupocoluna',
								   css   : {width  : '500px',
										    height : '80px'}})
						  .open(true);

				indiceEdicao = isNaN(indice) == false ? indice : "";
				if (indiceEdicao !== ""){
					$('[name=titulogrupocoluna]').val( table.grupoColuna[indiceEdicao].text );
				}else{
					$('[name=titulogrupocoluna]').val("");
				}
			}

			this.saveGrupoColuna = function(text)
			{
				toolTabela.close();
				text  = text || 0;
				var indice = (indiceEdicao !== "" ? indiceEdicao : table.grupoColuna.length);
				table.grupoColuna[indice] 		 = table.grupoColuna[indice] || new Object();
				table.grupoColuna[indice].text 	 = (text ? text : $('[name=titulogrupocoluna]').val() || '');
//				table.grupoColuna[indice].indice = indice;
				if (indiceEdicao === ""){
					criarTrGrupoColuna(indice);
				}else{
					$(table.grupoColuna[indice].id).find('span')
												   .first()
												   .html( table.grupoColuna[indice].text );
				}
			}

			function criarTrGrupoColuna(indice)
			{
				var ind 	= indice;
				var idGrupo = "grupo_coluna_" + ind;

				table.grupoColuna[ind].id 	   = "#"+idGrupo;
				table.grupoColuna[ind].colunas = table.grupoColuna[ind].colunas || new Array();

				tr = $("#grupo_linha_matrix");
				// Coluna I (tr)
				if (ind > 0)
					var tdI = $("<td>");
				else
					var tdI = $(tr).find('td').last();

				tdI.attr({id : idGrupo, align : 'center'})
				   .addClass('tdtabelaescuro')
				   .html( '<img src="/imagens/alterar.gif" onclick="TableManager.addGrupoColuna('+ind+')" title="<?= ('Editar Grupo de Coluna') ?>" class="simulaLink">&nbsp;' +
						  '<img src="/imagens/excluir.gif" onclick="TableManager.apagarGrupoColuna('+ind+');" title="<?= ('Excluir Grupo de Coluna') ?>" class="simulaLink">&nbsp;' +
						  '<span>' + table.grupoColuna[ind].text + '</span>' +
						  '<br><span class="simulaLink" id="form_coluna_'+ind+'" onclick="TableManager.addColuna('+ind+');">' +
						  '<img src="/imagens/gif_inclui.gif" title="<?= ('Adicionar Coluna') ?>">&nbsp;<?= ('Adicionar Coluna') ?></span>' );

				if (ind != 0)
					tr.append( tdI );
			}

			this.apagarGrupoColuna = function(indice)
			{
				if (confirm('<?= ('Deseja apagar este Grupo de Coluna?') ?>')){
					var id = table.grupoColuna[indice].id;
				    $(id).remove();

				    if (typeof table.grupoColuna[indice].colunas == 'object'){
					    $.each(table.grupoColuna[indice].colunas, function (indiceCol, element){
						    	if (typeof element == 'object'){
					    			objElement.apagarColuna(indice, 0, false);
						    	}else{
									// Linha \\
									$("#grupo_linha_0 table").remove();
									var tr  = $("#grupo_linha_0");
									var pos = 0;
									for (var a=0; a <= indice; a++){
										pos = table.grupoColuna[a].colunas.length + pos;
										pos = table.grupoColuna[a].colunas.length == 0 ? +1 : pos;
//										alert(pos);
										if (a == indice){
//											alert(pos);
											tr.find('td').eq(pos).remove();
										}
									}
							    }
						    });
				    }
					table.grupoColuna = $.grep(table.grupoColuna, function(i, v) {
											        return v != indice;
											      });

				    preencherTodasColunas();

					if (table.grupoColuna.length == 0){
						var tr = $("#grupo_linha_matrix");
						var td = $("<td>").addClass('tdtabelaescuro');
						tr.append( td )
					}else{
						objElement.reestruturarGrupoColuna();
					}
				}
			}

			this.reestruturarGrupoColuna = function ()
			{
				$.each(table.grupoColuna, function (i){
						// id Antigo
						var idAnt = table.grupoColuna[i].id;
						// ID novo
						var id = "grupo_coluna_" + i;
						table.grupoColuna[i].id = "#"+id;
						$(idAnt).attr({id : id});

						// Altera onclick IMG
						$('#'+id).find("img").each(function (ind){
								// EDITAR
								if (ind == 0){
									$(this).removeAttr("onclick")
										   .unbind("click")
										   .click(function (){
											   TableManager.addGrupoColuna(i);
											});
								// DELETAR
								}else if (ind == 1){
									$(this).removeAttr("onclick")
										   .unbind("click")
										   .click(function (){
											   TableManager.apagarGrupoColuna(i);
											});
								}
							});
						// Altera SPAN de Nova Coluna
						$('#'+id).find("[id*=form_coluna_]").attr("id", "form_coluna_"+i)
														   .removeAttr("onclick")
														   .unbind("click")
														   .click(function (){
															   	TableManager.addColuna(i);
															});
						// \\
						objElement.reestruturarColuna(i);
					})
			}
			/***
			* Grupo coluna - Fim
			*/

			/***
			* Coluna - Início
			*/
			this.addColuna = function(indTr, indice)
			{
//				$('[id*=form_coluna_][id*=_toolTip]').remove();
				toolTabela.config({url   : location.href + '&op=telacoluna',
								   css   : {width  : '500px',
								   			height : '140px'}})
						  .open(true);
				indiceGrupoColuna = indTr;

				indiceEdicao = isNaN(indice) == false ? indice : "";
				if (indiceEdicao !== ""){
					$('[name=titulocoluna]').val( table.grupoColuna[indTr].colunas[indiceEdicao].text );
					$('[name=typecoluna][value='+table.grupoColuna[indTr].colunas[indiceEdicao].type+']').attr({checked:true});
					$('#typecoluna_TX_numeric').attr('checked', table.grupoColuna[indTr].colunas[indiceEdicao].numeric);
				}else{
					$('[name=titulocoluna]').val("");
				}
			}

			this.saveColuna = function(indTr, text, type, colNumeric)
			{
				toolTabela.close();

				indTr	   = indTr || 0;
				text  	   = text || 0;
				type	   = type || 0;
				colNumeric = colNumeric || 0;
				if (colNumeric == 't') colNumeric = true; else if (colNumeric == 'f') colNumeric = false;

//				indiceGrupoColuna = indiceGrupoColuna ? indiceGrupoColuna : indTr;
				indiceGrupoColuna = indTr ? indTr : indiceGrupoColuna;
				colNumeric = colNumeric ? colNumeric : $('#typecoluna_TX_numeric').attr('checked');

				if (indiceEdicao !== ""){
					table.grupoColuna[indiceGrupoColuna].colunas[indiceEdicao].text = $('[name=titulocoluna]').val();
					table.grupoColuna[indiceGrupoColuna].colunas[indiceEdicao].type = $('[name=typecoluna]:checked').val();
					table.grupoColuna[indiceGrupoColuna].colunas[indiceEdicao].numeric = colNumeric;
					$(table.grupoColuna[indiceGrupoColuna].colunas[indiceEdicao].id).find('span')
												   									.first()
												   									.html( table.grupoColuna[indiceGrupoColuna].colunas[indiceEdicao].text );
					preencherTodasColunas();
					return;
				}

				var idGrupo = table.grupoColuna[indiceGrupoColuna].id;
				table.grupoColuna[indiceGrupoColuna].numCol = (table.grupoColuna[indiceGrupoColuna].numCol + 1 || 1);
				var numCol = table.grupoColuna[indiceGrupoColuna].numCol;
				$(idGrupo).attr({colspan : numCol});
				table.grupoColuna[indiceGrupoColuna].proxCol = (table.grupoColuna[indiceGrupoColuna].proxCol + 1 || 0);
				var proxCol  = table.grupoColuna[indiceGrupoColuna].proxCol;
				var idColuna = "coluna_"+indiceGrupoColuna+"_"+proxCol;


				var existeTd = table.grupoColuna[indiceGrupoColuna].colunas[proxCol] || false;
				table.grupoColuna[indiceGrupoColuna].colunas[proxCol] = {"text" 	: (text ? text : $('[name=titulocoluna]').val() || ''),
																		 "id"   	: '#'+idColuna,
																		 "numeric"  : colNumeric,
																		 "type" 	: type ? type : $('[name=typecoluna]:checked').val()};

				// Linha \\
				var tr = $("#grupo_linha_0");
				if (tr.length == 0){
					tr = $("<tr>");
					tr.attr({id : "grupo_linha_0"});
					$("#grupo_linha_matrix").after(tr);
					// Coluna \\
					var td = $("<td>");
					td.addClass("tdtabelaescuro")
					  .html("&nbsp;");

					tr.append( td );
				}

				$("#grupo_linha_0 table").remove();
				// Verifica se Grupos anteriores possuem COLUNAS
				var pos = 0;
				for (var a=0; a < indiceGrupoColuna; a++){
					pos = table.grupoColuna[a].colunas.length + pos;
					if (table.grupoColuna[a].colunas.length == 0){
						table.grupoColuna[a].colunas[0] = 1;
						var td = $("<td>");
						td.addClass("tdtabelaclaro")
						  .html("&nbsp;");

						$("#grupo_linha_0 td").eq(pos).after(td);
						pos++;
					}
				}

				preencherTodasColunas();

				var td = $("<td>");
				td.attr({id : idColuna, align : 'center'})
				  .addClass("tdtabelaclaro")
				  .html( '<img src="/imagens/alterar.gif" onclick="TableManager.addColuna('+indiceGrupoColuna+','+proxCol+');" title="Editar Coluna" class="simulaLink">&nbsp;' +
						 '<img src="/imagens/excluir.gif" onclick="TableManager.apagarColuna('+indiceGrupoColuna+','+proxCol+');" title="Excluir Coluna" class="simulaLink">&nbsp;' +
						 '<span>' + table.grupoColuna[indiceGrupoColuna].colunas[proxCol].text + '</span>' );

				pos++;
				if (numCol > 1){
					tr.find("[id*=coluna_"+indiceGrupoColuna+"]").last().after( td );
				}else if (existeTd){
					tr.find('td').eq(pos).replaceWith(td);
				}else{
					tr.append( td );
				}
			}

			function preencherTodasColunas()
			{

				$.each(table.grupoLinha, function (indGrupoLinha){
				if (typeof table.grupoLinha[indGrupoLinha].linhas == 'object'){
					$.each(table.grupoLinha[indGrupoLinha].linhas, function (indLinha){
							$("#linha_" + indGrupoLinha + "_"+indLinha+" table").remove();
							$("#linha_" + indGrupoLinha + "_"+indLinha+" td:not(:first)").remove();

							var pos = 0;
							$.each(table.grupoColuna, function (indGrupoColuna){
									if (typeof table.grupoColuna[indGrupoColuna].colunas == 'object'){
										$.each(table.grupoColuna[indGrupoColuna].colunas, function (indColuna){
												var td = $("<td>").attr("align", "center")
												  				  .append( montarCampo( table.grupoColuna[indGrupoColuna].colunas[indColuna].type ) );
//												  .html( montarCampo('CK') );
												$("#linha_" + indGrupoLinha + "_"+indLinha+" td").eq(pos).after(td);
												pos++;
											});
									}
								});
						});
				}
			})


			}

			this.apagarColuna = function (indTr, indice, confirme)
			{
//				alert(indTr + '\n' + table.grupoColuna[indTr].colunas + '\n' + indice + '\n');
				confirme 		   = confirme == false ? confirme : true;
				var confirmeResult = confirme == false ? true : confirm('Deseja apagar a Coluna?');
				if (confirmeResult){
					var id = table.grupoColuna[indTr].colunas[indice].id;
					table.grupoColuna[indTr].colunas = $.grep(table.grupoColuna[indTr].colunas, function(i, v) {
														      	return v != indice;
														      });
				    $(id).remove();

				    if (table.grupoColuna[indTr].colunas.length == 0 && confirme == true){
						// Linha \\
//						var tr  = $("#grupo_linha_0");
						$("#grupo_linha_0 table").remove();
						var pos = 0;
//						var ate = indTr == 0 ? 1 : indTr;
						for (var a=0; a <= indTr; a++){
							pos = table.grupoColuna[a].colunas.length + pos;
							if (table.grupoColuna[a].colunas.length == 0){
								table.grupoColuna[a].colunas[0] = 1;
								var td = $("<td>");
								td.addClass("tdtabelaclaro")
								  .html("&nbsp;");
								$("#grupo_linha_0 td").eq(pos).after(td);
//								tr.find('td').eq(pos).after(td);
								pos++;
							}
						}
				    }else{
				    	$(table.grupoColuna[indTr].id).attr("colspan",table.grupoColuna[indTr].colunas.length);
					}

				    preencherTodasColunas();

				    if (confirme == true)
				    	objElement.reestruturarColuna(indTr);
				}
			}

			this.reestruturarColuna = function(indTr)
			{
				table.grupoColuna[indTr].proxCol = undefined;
				table.grupoColuna[indTr].numCol  = undefined;

				if (typeof table.grupoColuna[indTr].colunas == 'object'){
					$.each(table.grupoColuna[indTr].colunas , function (i, element){
							if(typeof element != 'object') return;
							// id Antigo
							var idAnt = table.grupoColuna[indTr].colunas[i].id;
							// ID novo
							var id = "coluna_" + indTr + "_"+i;
							table.grupoColuna[indTr].colunas[i].id 	= "#"+id;
							table.grupoColuna[indTr].proxCol 		= i;
							table.grupoColuna[indTr].numCol  		= (i + 1);
							$(idAnt).attr("id", id);

							// Altera onclick IMG
							$('#'+id).find("img").each(function (ind){
									// EDITAR
									if (ind == 0){
										$(this).removeAttr("onclick")
											   .unbind("click")
											   .click(function (){
												   TableManager.addColuna(indTr, i);
												});
									// DELETAR
									}else if (ind == 1){
										$(this).removeAttr("onclick")
											   .unbind("click")
											   .click(function (){
												   TableManager.apagarColuna(indTr, i);
												});
									}
								});
						});
				}
			}
			/***
			* Coluna - Fim
			*/

			/***
			* Grupo Linha - Início
			*/
			this.addGrupoLinha = function (indice)
			{
				toolTabela.config({url   : location.href + '&op=telagrupolinha',
								   css   : {width  : '500px',
										    height : '80px'}})
						  .open(true);
				indiceEdicao = isNaN(indice) == false ? indice : "";
				if (indiceEdicao !== ""){
					$('[name=titulogrupolinha]').val( table.grupoLinha[indiceEdicao].text );
				}else{
					$('[name=titulogrupolinha]').val("");
				}
			}

			this.saveGrupoLinha = function(text)
			{
				toolTabela.close();
				text  = text || 0;
				if (indiceEdicao !== ""){
					table.grupoLinha[indiceEdicao].text = $('[name=titulogrupolinha]').val();
					$(table.grupoLinha[indiceEdicao].id).find('span')
												   		.first()
												   		.html( table.grupoLinha[indiceEdicao].text );
					return;
				}

				var indice  = table.grupoLinha.length;
				var idGrupo = "grupo_linha_"+indice;
				table.grupoLinha[indice] = {"text" : (text ? text : $('[name=titulogrupolinha]').val() || ''),
											"id"   : "#"+idGrupo};
				// Coluna \\
				td = $("<td>");
				td.attr({align 	 : 'center',
						 colspan : (indice == 0 ? 1 : 100)})
				  .addClass("tdtabelaescuro")
				  .html( '<img src="/imagens/alterar.gif" onclick="TableManager.addGrupoLinha('+indice+');" title="Editar Grupo Linha" class="simulaLink">&nbsp;' +
						 '<img src="/imagens/excluir.gif" onclick="TableManager.apagarGrupoLinha('+indice+');" title="Excluir Grupo Linha" class="simulaLink">&nbsp;' +
						 '<span>' + table.grupoLinha[indice].text + '</span>' +
						 '<br><span class="simulaLink" id="form_linha_'+indice+'" onclick="TableManager.addLinha('+indice+');">' +
						 '<img src="/imagens/gif_inclui.gif" title="Adicionar Linha">&nbsp;Adicionar Linha</span>');

				var tr = $("#grupo_linha_"+indice);
				if (tr.length == 0){
					// Linha \\
					tr = $("<tr>");
					tr.attr({id : idGrupo});
					if (indice == 0){
						$("#grupo_linha_matrix").after(tr);
					}else{
						if (typeof table.grupoLinha[indice-1].linhas == "object" && table.grupoLinha[indice-1].linhas.length > 0){
							indLinha = (table.grupoLinha[indice-1].linhas.length - 1);
							var idRefe = table.grupoLinha[indice-1].linhas[indLinha].id;
						}else{
							var idRefe = table.grupoLinha[indice-1].id;
						}
						$(idRefe).after(tr);
					}
					tr.append( td );
				}else{
					// Linha \\
					td.replaceAll(tr.find("td").first());
				}
			}

			this.apagarGrupoLinha = function (indice)
			{
				if (confirm('Deseja apagar o Grupo de Linha?')){
					var id = table.grupoLinha[indice].id;
					if (indice == 0){
						var totLinha = table.grupoLinha.length;
						if (totLinha > 1){
							$(table.grupoLinha[indice].id).find('td:first').replaceWith( $(table.grupoLinha[indice + 1].id).find('td:first') );
							$(table.grupoLinha[indice].id).find('td:first').attr("colspan", 1);
							$(table.grupoLinha[indice + 1].id).remove();
						}else{
							var possuiColuna = false;
							$.each(table.grupoColuna, function (i){
									if (table.grupoColuna[i].colunas.length > 0){
										possuiColuna = true;
										return;
									}
								});
							if (possuiColuna){
								$(table.grupoLinha[indice].id).find('td:first').html("&nbsp;");
							}else{
								$(id).remove();
							}
						}
					}else{
						$(id).remove();
					}
					if (typeof table.grupoLinha[indice].linhas == 'object'){
						$.each(table.grupoLinha[indice].linhas, function (i){
								objElement.apagarLinha(indice, i, false);
							});
					}
					table.grupoLinha = $.grep(table.grupoLinha, function(i, v) {
											return v != indice;
										});

					objElement.reestruturarGrupoLinha();
				}
			}

			this.reestruturarGrupoLinha = function ()
			{
				$.each(table.grupoLinha, function (i){
						// id Antigo
						var idAnt = table.grupoLinha[i].id;
						// ID novo
						var id = "grupo_linha_" + i;
						table.grupoLinha[i].id = "#"+id;
						$(idAnt).attr({id : id});

						// Altera onclick IMG
						$('#'+id).find("img").each(function (ind){
								// EDITAR
								if (ind == 0){
									$(this).removeAttr("onclick")
										   .unbind("click")
										   .click(function (){
											   TableManager.addGrupoLinha(i);
											});
								// DELETAR
								}else if (ind == 1){
									$(this).removeAttr("onclick")
										   .unbind("click")
										   .click(function (){
											   TableManager.apagarGrupoLinha(i);
											});
								}
							});
						// Altera SPAN de Nova Linha
						$('#'+id).find("[id*=form_linha_]").attr("id", "form_linha_"+i)
														   .removeAttr("onclick")
														   .unbind("click")
														   .click(function (){
															   	TableManager.addLinha(i);
															});
						// \\
						objElement.reestruturarLinha(i);
					})
			}
			/***
			* Grupo Linha - Fim
			*/
			/***
			* Linha - Início
			*/
			this.apagarLinha = function (idTr, indice, confirme)
			{
				confirme = confirme == false ? confirme : true;
				var confirmeResult = confirme == false ? true : confirm('Deseja apagar a Linha?');

				if (confirmeResult){
					var id = table.grupoLinha[idTr].linhas[indice].id;
					$(id).remove();
				    if (confirme == true){
				    	table.grupoLinha[idTr].linhas = $.grep(table.grupoLinha[idTr].linhas, function(i, v) {
								return v != indice;
							});
				    	objElement.reestruturarLinha(idTr);
					}
				}
			}

			this.reestruturarLinha = function (indTr)
			{
				table.grupoLinha[indTr].proxLinha = undefined;
				table.grupoLinha[indTr].numLinha  = undefined;

				if (typeof table.grupoLinha[indTr].linhas == 'object'){
					$.each(table.grupoLinha[indTr].linhas , function (i){
							// id Antigo
							var idAnt = table.grupoLinha[indTr].linhas[i].id;
							// ID novo
							var id = "linha_" + indTr + "_"+i;
							table.grupoLinha[indTr].linhas[i].id 	= "#"+id;
							table.grupoLinha[indTr].proxCol 		= i;
							table.grupoLinha[indTr].numCol  		= (i + 1);
							$(idAnt).attr("id", id);

							// Altera onclick IMG
							$('#'+id).find("img").each(function (ind){
									// EDITAR
									if (ind == 0){
										$(this).removeAttr("onclick")
											   .unbind("click")
											   .click(function (){
												   TableManager.addLinha(indTr, i);
												});
									// DELETAR
									}else if (ind == 1){
										$(this).removeAttr("onclick")
											   .unbind("click")
											   .click(function (){
												   TableManager.apagarLinha(indTr, i);
												});
									}
								});
						});
				}
			}

			this.addLinha = function(indTr, indice)
			{
				toolTabela.config({url   : location.href + '&op=telalinha',
								  css   : {width  : '500px',
										   height : '80px'}})
						  .open(true);
				indiceGrupoLinha = indTr;

				indiceEdicao = isNaN(indice) == false ? indice : "";
				if (indiceEdicao !== ""){
					$('[name=titulolinha]').val( table.grupoLinha[indTr].linhas[indiceEdicao].text );
				}else{
					$('[name=titulolinha]').val("");
				}

//				$('[name=titulolinha]').val("");
			}

			this.saveLinha = function(indTr, text)
			{
				indTr = indTr || 0;
				indiceGrupoLinha = indiceGrupoLinha ? indiceGrupoLinha : indTr;
				toolTabela.close();

				if (indiceEdicao !== ""){
					table.grupoLinha[indiceGrupoLinha].linhas[indiceEdicao].text = $('[name=titulolinha]').val();
					$(table.grupoLinha[indiceGrupoLinha].linhas[indiceEdicao].id).find('span')
												   								 .first()
												   								 .html( table.grupoLinha[indiceGrupoLinha].linhas[indiceEdicao].text );
					return;
				}

				var idGrupo   = table.grupoLinha[indiceGrupoLinha].id;
				table.grupoLinha[indiceGrupoLinha].numLinha = table.grupoLinha[indiceGrupoLinha].numLinha + 1 || 1;
				var numLinha  = table.grupoLinha[indiceGrupoLinha].numLinha;
				var classe 	  = (numLinha % 2) == 1 ? 'tdtabelaclaro' : 'tdtabelaescuro';
				table.grupoLinha[indiceGrupoLinha].proxLinha = table.grupoLinha[indiceGrupoLinha].proxLinha + 1 || 0;
				var proxLinha = table.grupoLinha[indiceGrupoLinha].proxLinha;
				var idLinha   = "linha_"+indiceGrupoLinha+"_"+proxLinha;

				table.grupoLinha[indiceGrupoLinha].linhas = table.grupoLinha[indiceGrupoLinha].linhas || new Array();
				table.grupoLinha[indiceGrupoLinha].linhas[proxLinha] = {"text" : (text ? text : $('[name=titulolinha]').val() || ''),
																		"id"   : "#"+idLinha};

				var tr = $('<tr>').attr({id:idLinha});

				var td = $('<td>').addClass(classe)
								  .html( '<img src="/imagens/alterar.gif" onclick="TableManager.addLinha('+indiceGrupoLinha+', '+proxLinha+');" title="Editar Linha" class="simulaLink">&nbsp;' +
										 '<img src="/imagens/excluir.gif" onclick="TableManager.apagarLinha('+indiceGrupoLinha+', '+proxLinha+');" title="Excluir Linha" class="simulaLink">&nbsp;' +
										 '<span>'+table.grupoLinha[indiceGrupoLinha].linhas[proxLinha].text+'</span>' );


				tr.append( td );

				// Coloca colunas de campo
//				$.each(table.grupoColuna, function (i){
//						for (var aa=0; aa < table.grupoColuna[i].colunas.length; aa++){
//							tr.append( $("<td>").html("a"));
//						}
//					});

				if (numLinha > 1)
					$("[id*=linha_"+indiceGrupoLinha+"]").last().after( tr );
				else
					$(idGrupo).after(tr);

				preencherTodasColunas();
			}
			/***
			* Linha - Fim
			*/

			function montarCampo(type)
			{
				var campo = $('<input>');
				switch (type){
					case 'CK':
						campo.attr('type', 'checkbox');
						return campo;
					case 'RA':
						campo.attr('type', 'radio');
						return campo;
					case 'TX':
						campo.attr({type:"text", size:10});
						return campo;
				}
			}

			this.montarJsonCampo = function ()
			{
				var objJson = new Object();
				objJson.tabela = table.prop['text'];
				// Trata Linhas
				$.each(table.grupoLinha, function (indGrupoLinha){
						objJson.grupoLinha 				  = objJson.grupoLinha || new Array();
						objJson.grupoLinha[indGrupoLinha] = table.grupoLinha[indGrupoLinha].text;
						if (typeof table.grupoLinha[indGrupoLinha].linhas == "object"){
							$.each(table.grupoLinha[indGrupoLinha].linhas, function (indLinha){
								objJson.linha 							= objJson.linha || new Array();
								objJson.linha[indGrupoLinha] 			= objJson.linha[indGrupoLinha] || new Array();
								objJson.linha[indGrupoLinha][indLinha] 	= table.grupoLinha[indGrupoLinha].linhas[indLinha].text;
							});
						}
					});
				// Trata Colunas
				$.each(table.grupoColuna, function (indGrupoColuna){
					objJson.grupoColuna 				  = objJson.grupoColuna || new Array();
					objJson.grupoColuna[indGrupoColuna] = table.grupoColuna[indGrupoColuna].text;
					if (typeof table.grupoColuna[indGrupoColuna].colunas == "object"){
						$.each(table.grupoColuna[indGrupoColuna].colunas, function (indColuna){
							objJson.coluna 							  		  = objJson.coluna || new Array();
							objJson.coluna[indGrupoColuna] 			  		  = objJson.coluna[indGrupoColuna] || new Array();
							if (table.grupoColuna[indGrupoColuna].colunas == 1){
								objJson.coluna[indGrupoColuna][indColuna] = '';
							}else{
								var type = table.grupoColuna[indGrupoColuna].colunas[indColuna].type || 'TX';
								var text = table.grupoColuna[indGrupoColuna].colunas[indColuna].text || '';
								var numeric = table.grupoColuna[indGrupoColuna].colunas[indColuna].numeric || false;
								if (type != 'TX')
									numeric = false;
								objJson.coluna[indGrupoColuna][indColuna] = type+'_||_'+text+'_||_'+numeric;
							}
						});
					}
				});
				$('#tabela_json').val( $.JSON.encode(objJson) );

//				$.post(location.href+'&op=salvartabela', { data : $.JSON.encode(objJson)},
//						   function(data) {
//						     $('#testando').html(data);
//						   });
			}

			this.montarJsonCampoClear = function ()
			{
				$('#tabela_json').html();
			}
		}

		var TableManager = new TableManager();
		</script>
	<?php
	}


	public function setView($view)
	{
		$this->arqView = $view;

		return $this;
	}

	public function getView()
	{
		return $this->arqView;
	}

	protected function renderView($view)
	{
		if (!empty($view)){
			self::setView($view);
		}else{
			$view = self::getView();
		}
		$caminho = APPRAIZ . "includes/classes/tabela/view/";
		$arquivo = $caminho . self::addExtensao($view);

		if (file_exists($arquivo)){
//			extract($this->view);
			require_once $arquivo;
		}else{
			die("O arquivo de view ({$view}) da classe de Tela não foi encontrado!");
		}
	}

	protected function extract($dados)
	{
		if ($dados instanceof Modelo){
			$dados = (array) $dados->getDados();
		}

		foreach ($dados as $k => $v){
			$this->$k = $v;
//			$this->view[$k] = $v;
		}
	}

	private function addExtensao($file)
	{
		return $file . ".inc";
	}
}