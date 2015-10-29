<?
die('desativado');
include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/classes/Modelo.class.inc";

$db = new cls_banco();

$sql	= " SELECT * FROM planointerno.detalhepi ";
$existe	= $db->pegaUm($sql);

//Se não existir ninguem na tabela
if ( $existe ){
	$dados  = $db->carregar( "SELECT dpiid FROM planointerno.detalhepi" );
	if( !empty($dados) && is_array($dados) ){

		foreach ( $dados as $dado ){

			$sql1 = "INSERT INTO
						workflow.documento (tpdid,esdid,docdsc,docdatainclusao )
					 VALUES
					 	(101,507,'Detalhe Plano Interno',now())
					 returning docid";
			$docid = $db->pegaUm($sql1);

			$sql2 = "UPDATE
						planointerno.detalhepi
					 SET
					 	docid = {$docid}
					 WHERE
					 	dpiid = {$dado['dpiid']}";

			$db->executar($sql2);

//dbg($sql1);
//dbg($sql2);

		}

		$db->commit();

	}
	die('Atualizado com sucesso.');
}else{
	die('Não existem registros.');
}

?>