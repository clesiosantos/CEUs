<?php
class TEndereco extends TField{

    protected $_exibirMapa;

	function __construct($name){
		parent::__construct($name);

		$this->_exibirMapa = true;
	}

	public function setExibirMapa($exibirMapa){
        $this->_exibirMapa = $exibirMapa;
	}

	function show(){
		$html = '<table cellspacing="1" cellpadding="3" bgcolor="#f5f5f5" align="center" width="100%" class="tabela" valign="top">
            		<tr>
                        <td align="right" class="SubTituloDireita">
                            CEP:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    ' . campo_texto( 'qencep_'.$this->name, $this->obrig, $this->habil, '', 13, 10, '##.###-###', '', '', 0, '', '', null, $this->value['endereco']->qencep, "quest.recuperarEnderecoPorCep('".$this->name."');") . '
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Logradouro:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    ' . campo_texto( 'qenlogradouro_'.$this->name, $this->obrig, $this->habil, '', 65, 150, '', '', '', 0, '', '', null, $this->value['endereco']->qenlogradouro) . '
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Número:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    ' . campo_texto( 'qennumero_'.$this->name, $this->obrig, $this->habil, '', 13, 8, '########', '', '', 0, '', '', null, $this->value['endereco']->qennumero) . '
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Complemento:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    ' . campo_texto( 'qencomplemento_'.$this->name, 'N', $this->habil, '', 65, 150, '', '', '', 0, '', '', null, $this->value['endereco']->qencomplemento) . '
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Bairro:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    ' . campo_texto( 'qenbairro_'.$this->name, $this->obrig, $this->habil, '', 65, 150, '', '', '', 0, '', '', null, $this->value['endereco']->qenbairro) . '
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Município/UF:
                        </td>
                        <td style="white-space: nowrap;">
            	 		    <input type="hidden" name="muncod_'.$this->name.'" value="'.$this->value['endereco']->muncod.'" />
            	 		    ' . campo_texto( 'mundescricao_'.$this->name, 'N', 'N', '', 55, 150, '', '', '', 0, '', '', null, ($this->value['endereco'] ? $this->value['endereco']->getDscMunicipio() : "")) . '
            	 		    ' . campo_texto( 'estuf_'.$this->name, $this->obrig, 'N', '', 5, 150, '', '', '', 0, '', '', null, $this->value['endereco']->estuf) . '
                        </td>
                    </tr>';
        if($this->_exibirMapa){
            $html .= '<tr>
                          <td colspan="2">
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

                				<script language="javascript" type="text/javascript" >
                					quest.exibeMapa("divMap","qencoordenadas_' . $this->name . '", "qenzoom_' . $this->name . '");
                				</script>

                				<div style="background-color: #DCDCDC; margin: 10px;">
                                   <div style="background-color: #DCDCDC; padding: 5px 0px 5px 0px; text-align: center;">
                                        Coordenadas Geográficas
                                    </div>
                        			<div id="divMap" class="mapa">
                        			</div>
                        			<div class="painel-mapa" '.($this->habil == 'N' ? 'style="display: none;"' : '').'>
                        				<div id="panel" class="olControlEditingToolbar"></div>
                        				<div class="btns">
                        					<ul>
                        						<li><a href="#" class="btn_clear">LIMPAR MAPA</a></li>
                        					</ul>
                        				</div>
                        				<div style="clear: both;"></div>
                        			</div>
                                    <input type="hidden" name="qencoordenadas_' . $this->name . '" id="qencoordenadas_' . $this->name . '" value="'. ($this->value['endereco'] ? $this->value['endereco']->getCoordenada() : "") .'" />
                                    <input type="hidden" name="qenzoom_' . $this->name . '" id="qenzoom_' . $this->name . '" value="'. $this->value['endereco']->qenzoom .'" />
                               </div>
                          </td>
                      </tr>';

        }
		$html .= "</table>";

		echo $html;
	}
}
