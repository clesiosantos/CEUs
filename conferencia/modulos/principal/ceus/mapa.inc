<?php
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
monta_titulo( $titulo_modulo, '' );
?>

<link rel="stylesheet" type="text/css" href="../includes/openlayers/openlayers.css" />
<link rel="stylesheet" type="text/css" href="../conferencia/mapaCeus/mapa.css" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui/jquery-ui-1.10.3/themes/base/jquery.ui.all.css" />

<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script type="text/javascript" src="../includes/openlayers/OpenLayers.js"></script>
<script type="text/javascript" src="../conferencia/mapaCeus/mapa.js"></script>
<script type="text/javascript" src="https://maps.google.com/maps/api/js?v=3.7&sensor=false&amp;language=pt_BR"></script>

<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
    <tr>
		<td class="SubTituloDireita" colspan="2" style="font-weight: bold; text-align: center; font-size: 16px; height: 27px;">
		    <label id="lblMapa">&nbsp;</label>
	        <img id="carregando" style="display: none;" alt="Carregando..." src="/imagens/loading_g.gif">
	    </td>
	</tr>
    <tr>
        <td class="SubTituloDireita" colspan="2">
			<div id="divMap" class="mapa">
			    <div id="mapaPainel">
    			    <div id="mapaPainelAbrir" title="Abrir Filtros" onclick="gerenciarFiltros(true);">
    			    </div>
    			    <div id="mapaPainelConteudo">
                        <div id="painelConteudo">
                            <div id="painelFiltros">

                            </div>
            			    <div id="mapaPainelFechar" title="Fechar Filtros" onclick="gerenciarFiltros(false);"></div>
        			    </div>
    			    </div>
			    </div>
			</div>
        </td>
    </tr>
</table>
<script type="text/javascript">
    $(function() {
    	inicializarMapa('divMap');
    });
</script>