<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:cadastro_usuario_elaboracao_responsabilidades.php

   */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ."includes/classes_simec.inc";
include APPRAIZ."includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if(!$pflcod && !$usucpf) {
	?><font color="red">Requisi��o inv�lida</font><?
	exit();
}

$sqlResponsabilidadesPerfil = "SELECT
								tr.*
							   FROM
							   	conferencia.tprperfil p
							    INNER JOIN conferencia.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
							   WHERE
							    tprsnvisivelperfil = TRUE AND
							    p.pflcod = '%s'
							   ORDER BY
							    tr.tprdsc";
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);
$responsabilidadesPerfil = (array) $db->carregar($query);

if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil)<1) {
	print "<font color='red'>N�o foram encontrados registros</font>";
}else {
	foreach ($responsabilidadesPerfil as $rp) {
		// monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (a��o, programas, etc)
		$sqlRespUsuario = "";
		switch ($rp["tprsigla"]) {
			case "P": // Pr�-cadastro da confer�ncia
				$aca_prg = "Pr�-cadastro da confer�ncia";
				$sqlRespUsuario = "SELECT
									p.pcnid AS codigo, p.pcntitulo AS descricao, u.rpustatus AS status
								   FROM
								    conferencia.usuarioresponsabilidade u
								    INNER JOIN conferencia.preconferencia p ON p.pcnid = u.pcnid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND
								    u.rpustatus='A'";
			break;
			case "v":
				$aca_prg = "V�nculo CEU";
				$sqlRespUsuario = "SELECT
									c.ceuid AS codigo, c.ceucodigo || ' - ' || c.ceunome AS descricao, u.rpustatus AS status
								   FROM
								    conferencia.usuarioresponsabilidade u
								    INNER JOIN conferencia.vinculoceu p ON p.vceid = u.vceid
		                            inner join conferencia.ceu c ON c.ceuid = p.ceuid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND
								    u.rpustatus='A'";
			break;
		}

		if(!$sqlRespUsuario) continue;
		$query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod, $_SESSION['exercicio']));
		$respUsuario = (array) $db->carregar($query);

		if (!$respUsuario[0] || @count($respUsuario) < 1) {
			print "<center><font color='red'>N�o existem $aca_prg a este Perfil.</font></center>";
		}else {
		?>
			<table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
				<tr>
				  <td colspan="3"><?=$rp["tprdsc"]?></td>
				</tr>
				<tr style="color:#000000;">
			      <td valign="top" width="12">&nbsp;</td>
				  <td valign="top">C�digo</td>
				  <td valign="top">Descri��o</td>
			    </tr>
			    <?php if($respUsuario): ?>
					<?
					foreach ($respUsuario as $ru) {
					?>
					<tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
				      <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
					  <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?if ($rp["tprsigla"]=='A'){?><a href="simec_er.php?modulo=principal/acao/cadacao&acao=C&acaid=<?=$ru["acaid"]?>&prgid=<?=$ru["prgid"]?>"><?=$ru["codigo"]?></a><?} else {print $ru["codigo"];}?></td>
					  <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?=$ru["descricao"]?></td>
					</tr>
					<?
					}
					?>
				<?php else: ?>
					<tr>
						<td colspan="3">Nunhum v�nculo encontrado.</td>
					</tr>
				<?php endif; ?>
				<tr>
				  <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">
				    Total: (<?=@count($respUsuario)?>)
				  </td>
				</tr>
			</table>
	<?
		}
	}
}
$db->close();
exit();
?>