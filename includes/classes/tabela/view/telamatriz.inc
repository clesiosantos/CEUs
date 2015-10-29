<style type="text/css">
.tdtabelaescuro{
	background-color:#a8a8a8;
	font-weight:bold;
}

.tdtabelaclaro{
	background-color:#cccccc;
	font-weight:bold;
}

.simulaLink{
	cursor:pointer;
}

</style>
<div style="width:650px;">
<input id="tabela_json" name="tabela_json" type="hidden"/>
<table class="tabela" align="center">
	<tr>
		<td colspan="100" align="center" class="tdtabelaescuro">
			<div style="width:200px;">
				<img class="simulaLink" src="/imagens/alterar.gif" title="Editar título" onclick="TableManager.editTabela();">&nbsp;
				<span id="form_tabela"><?= ("Título Tabela") ?></span>
			</div>
		</td>
	</tr>
	<tr id="grupo_coluna_matrix">
		<td class="tdtabelaescuro" width="40%">&nbsp;</td>
		<td colspan="100" class="tdtabelaescuro">
			<span class="simulaLink" onclick="TableManager.addGrupoColuna();">
				<img src="/imagens/gif_inclui.gif" title="<?= ("Adicionar Grupo de Coluna") ?>">&nbsp;<?= ("Grupo de Coluna") ?>
			</span>
			<span id="form_grupo_coluna"></span>
		</td>
	</tr>
	<tr id="grupo_linha_matrix">
		<td class="tdtabelaescuro">
			<span class="simulaLink" onclick="TableManager.addGrupoLinha();">
				<img src="/imagens/gif_inclui.gif" title="<?= ("Adicionar Grupo de Linha") ?>">&nbsp;<?= ("Grupo de Linha") ?>
			</span>
			<span id="form_grupo_linha"></span>
		</td>
		<td class="tdtabelaescuro">
			&nbsp;
		</td>
	</tr>
</table>
</div>

<!--
<input onclick="TableManager.montarJsonCampo();">
<div id="testando"></div>
-->