<?
die('desativado');
include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/classes/Modelo.class.inc";

$db = new cls_banco();

$sql	= "SELECT * FROM planointerno.propostapi WHERE eqdid = 1 AND ppisequencialpi is null AND ppistatus = 'A' 
ORDER BY case ppiid when 291 then 1 when 487 then 1 else 100 end, ppiid";
$dados = $db->carregar($sql);

if( !empty($dados) && is_array($dados) ){

	foreach ( $dados as $dado ){
		geraCodigoPi($dado['ppiid']);
	}

	$db->commit();
	die('Atualizado com sucesso.');
}else{
	die('Não existem registros.');
}

?>