<?php
class TMapa extends TField{

	function __construct($name){
		parent::__construct($name);
	}


	function show(){

		$valor     = $this->getValue();

		$html = '
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
					quest.exibeMapa("divMap","'.$this->name.'");
				</script>

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
                    <input type="hidden" name="' . $this->name . '" id="'.$this->name.'" value="'. $valor .'" />
               </div>';

		echo $html;

	}
}
