<?
die('desativado');
include "config.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/classes/Modelo.class.inc";

include APPRAIZ . "includes/classes/modelo/planointerno/PropostaPi.class.inc";
include APPRAIZ . "includes/classes/modelo/planointerno/DetalhePi.class.inc";
include APPRAIZ . "includes/classes/modelo/planointerno/CronogramaFinanceiro.class.inc";
include APPRAIZ . "includes/classes/modelo/planointerno/CronogramaFisico.class.inc";
include APPRAIZ . "includes/classes/modelo/planointerno/CronogramaOrcamentario.class.inc";


$db = new cls_banco();


$sql	= " SELECT * FROM planointerno.detalhepi ";
$existe	= $db->pegaUm($sql);

//Se não existir ninguem na tabela
if ( !$existe ){
	$modelo = new PropostaPi();
	$dados  = $modelo->recuperarTodos("*", array());

	if( !empty($dados) && is_array($dados) ){

		$modelo			= new DetalhePi();
		$cFisico       	= new CronogramaFisico();
    	$cOrcamentario 	= new CronogramaOrcamentario();
    	$cFinanceiro   	= new CronogramaFinanceiro();

		foreach ( $dados as $dado ){
			$arDados					= $dado;
			$arDados['ppiid']	        = $dado['ppiid'];
			$arDados['pmuid']	        = $dado['pmuid'];
			$arDados['usucpf']	        = $dado['usucpf'];
			$arDados['eqdid']	        = $dado['eqdid'];
			$arDados['mpnid']	        = $dado['mpnid'];
			$arDados['mppid']	        = $dado['mppid'];
			$arDados['pprid']	        = $dado['pprid'];
			$arDados['areid']	        = $dado['areid'];
			$arDados['priid']			= $dado['priid'];
			$arDados['umpid']			= $dado['umpid'];
			$arDados['dpidescricao']	= $dado['ppidescricao'];
			$arDados['dpidatainicio']	= $dado['ppidatainicio'];
			$arDados['dpidatatermino']	= $dado['ppidatatermino'];
			$arDados['dpinome']			= $dado['ppinome'];
			$arDados['dpiqtdmeta']		= $dado['ppiqtdmeta'];
			$arDados['dpivlrcusteio']	= $dado['ppivlrcusteio'];
			$arDados['dpivlrcapital']	= $dado['ppivlrcapital'];
			$arDados['dpidtcadastro']	= $dado['ppidtcadastro'];
			$arDados['dpistatus']		= $dado['ppistatus'];
			$arDados['masid']			= $dado['masid'];
			$arDados['docid']			= null;
			$arDados['maiid']			= $dado['maiid'];

			$dpiid		= $modelo->popularDadosObjeto($arDados)->salvar();

		    $dadosFisico       = $cFisico->pegaPorPi($dado['ppiid']);
		    $dadosOrcamentario = $cOrcamentario->pegaPorPi($dado['ppiid']);
		    $dadosFinanceiro   = $cFinanceiro->pegaPorPi($dado['ppiid']);

		    $dadosFisico['ppiid'] = $dadosOrcamentario['ppiid'] = $dadosFinanceiro['ppiid'] = null;
		    $dadosFisico['crfid'] = $dadosOrcamentario['croid'] = $dadosFinanceiro['cfiid'] = null;
		    $dadosFisico['dpiid'] = $dadosOrcamentario['dpiid'] = $dadosFinanceiro['dpiid'] = $dpiid;

			$cFisico->popularDadosObjeto($dadosFisico)->salvar();
			$cOrcamentario->popularDadosObjeto($dadosOrcamentario)->salvar();
			$cFinanceiro->popularDadosObjeto($dadosFinanceiro)->salvar();

			$modelo->setDadosNull();
			$cFisico->setDadosNull();
			$cOrcamentario->setDadosNull();
			$cFinanceiro->setDadosNull();
		}

		$modelo->commit();
	}
	die('Cadastrado com sucesso.');
}else{
	die('A tabela já está populada.');
}

?>