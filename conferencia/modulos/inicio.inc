<?php
// if (! $db->testa_superuser () && possui_perfil ( CONFERENCIA_PERFIL_DELEGADO)) {
//     header ( "Location: ?modulo=principal/delegado/fichaDelegado&acao=A" );
// }elseif (! $db->testa_superuser () && ! possui_perfil ( CONFERENCIA_PERFIL_CADASTRO )) {
//     header ( "Location: ?modulo=principal/conf/listarConferencia&acao=A" );
// }

include APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$mTelaInicial = new TelaInicial ();
$mTelaInicial->getTelaInicial ();

echo $mTelaInicial->tinhtml;

echo "<br />";

//monta link manual
require_once APPRAIZ . "includes/arquivo.inc";
$strSql = "  select
                 arqid
             from
                 public.manual as manual
             WHERE
                 manual.sisid = {$_SESSION['sisid']} ";
$arrDadosManual = $db->pegaLinha( $strSql );
$booArquivoExiste = existeArquivoFisico( $arrDadosManual["arqid"] );
if( $booArquivoExiste && sizeof( $arrDadosManual ) > 0 && $arrDadosManual["arqid"] > 0 ) {
    $intArqId = $arrDadosManual["arqid"];

	echo '<div style="text-align: center;">
			<a href="../mostra_arquivo.php?id='.$intArqId.';">
				<img src="../imagens/manual.png" style="vertical-align: sub; margin-right: 3px;" alt="Manual do Sistema" />
				Manual do Sistema
			</a>
		  </div>';
}
