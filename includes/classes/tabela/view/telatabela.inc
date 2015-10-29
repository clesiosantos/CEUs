	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2"><?= ("Tabela"); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita"><?= ("Título da Tabela:"); ?></td>
			<td>
			<?
			echo campo_texto('titulo','N','','',46,100,'','');
			?>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td align='left' style="vertical-align:top; width:25%"> 
				<input type="button" value="<?= ("Salvar"); ?>" onclick="TableManager.saveTabela();">
			</td>
		</tr>
	</table>