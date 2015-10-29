<?php
// abre conexão com o servidor de banco de dados
include_once "config.inc";

include_once APPRAIZ . "includes/classes_simec.inc";

$db = new cls_banco();

// inclui os objetos do sistema.
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

// carrega as funções específicas do módulo
include_once '../_constantes.php';
include_once '../_funcoes.php';

function __autoload($class_name) {
	$arCaminho = array(
						APPRAIZ . "includes/classes/modelo/public/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/controller/",
						APPRAIZ . "includes/classes/view/",
						APPRAIZ . "includes/classes/html/",
						APPRAIZ . "conferencia/classes/"
					  );

	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;
		}
	}
}

if($_REQUEST['act'] == 'buscarCeus'){
    $arWhere   = array();
    // $arWhere[] = "NOT ceu.ceucoordenadas IS NULL";
    $arWhere[] = "ceu.ceustatus = 'A'";

    $mCeu = new Ceu();
    $arResults = $mCeu->listar($arWhere);

    $arResults = $arResults ? $arResults : array();
    foreach($arResults as $k => $dados){
        foreach($dados as $i => $d){
            $arResults[$k][$i] = utf8_encode($d);
        }
    }

    $rJson = json_encode($arResults);
    die($rJson);
}