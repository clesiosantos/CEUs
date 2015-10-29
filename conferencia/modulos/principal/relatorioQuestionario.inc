<?php
header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$acoid = $_GET['acoid'];
$qrpid = $_GET['qrpid'];
$escid = $_GET['escid'] ? $_GET['escid'] : $_SESSION['escola']['escid'];
$proid = $_GET['proid'] ? $_GET['proid'] : $_SESSION['projeto']['proid'];
$tipo  = $_GET['tipo'];

$questResp = new QQuestionarioResposta();
if($tipo == 'P'){
    if ($acoid && $proid){
    	$qrpid = $questResp->carregaUm(array("acoid" => $acoid, "proid" => $proid), "qrpid");
    }
}else{
    if ($acoid && $escid){
    	$qrpid = $questResp->carregaUm(array("acoid" => $acoid, "escid" => $escid), "qrpid");
    }
}
?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<center>
<?php
if ($qrpid){
	$queid			   = $questResp->pegaQuestionario($qrpid);
	$_REQUEST['qrpid'] = $qrpid;
	$_REQUEST['queid'] = $queid;

	$dadosCondicao = $_REQUEST['dadoscondicao'] ? unserialize(str_replace("'", '"', $_REQUEST['dadoscondicao'])) : array();

	$obImprime = new QImpressao( array('tema' => 5, 'dadoscondicao' => $dadosCondicao) );
	echo $obImprime->montaArvore( null, false);
}elseif ($_REQUEST['queid'] && !isset($_REQUEST['qrpid'])){
	$obImprime = new QImpressao( array('tema' => 5, 'dadoscondicao' => $dadosCondicao) );
	echo $obImprime->montaArvore( null, true);
}else{
	echo Translate::get("Nenhum resultado foi encontrado.");
}
?>
</center>
<script>
window.print();
</script>