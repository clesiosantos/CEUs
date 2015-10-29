	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2"><?= ("Coluna"); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita"><?= ("Título da Coluna:"); ?></td>
			<td>
			<?
			echo campo_texto('titulocoluna','N','','',46,100,'','');
			?>
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita"><?= ("Tipo de campo:"); ?></td>
			<td>
				<label for="typecoluna_TX">
					<input id="typecoluna_TX" name="typecoluna" type="radio" value="TX"/>
					<span style="border:1px dashed #999999; padding:3px 3px 3px 3px;">
						(<?= ("Texto"); ?> <input size="5" type="text"/>)
						&nbsp;
						(<input type="checkbox" id="typecoluna_TX_numeric" name="numeric" /> <?= ("Somente números?"); ?> )
					</span>
				</label>
				<br>
				<label for="typecoluna_CK" onclick="$('#typecoluna_TX_numeric').attr({'checked':false});">
					<input id="typecoluna_CK" name="typecoluna" type="radio" value="CK"/>
					<span style="border:1px dashed #999999; padding:3px 3px 3px 3px;">
						(check <input type="checkbox"/>)
					</span>
				</label>
				<br>
				<label for="typecoluna_RA" onclick="$('#typecoluna_TX_numeric').attr({'checked':false});">
					<input id="typecoluna_RA" name="typecoluna" type="radio" value="RA"/>
					<span style="border:1px dashed #999999; padding:3px 3px 3px 3px;">
						(radio <input type="radio"/>)
					</span>
				</label>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td align='left' style="vertical-align:top; width:25%"> 
				<input type="button" value="<?= ("Salvar"); ?>" onclick="TableManager.saveColuna();">
			</td>
		</tr>
	</table>