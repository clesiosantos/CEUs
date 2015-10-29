<?php

error_reporting(E_ERROR);

function __autoload($class_name) {

    $arCaminho = array(
                        APPRAIZ . "credito/classes/",
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

date_default_timezone_set ('America/Sao_Paulo');

/**
 * Obtщm o tempo com precisуo de microsegundos. Essa informaчуo щ utilizada para
 * calcular o tempo de execuчуo da pсgina.
 *
 * @return float
 * @see /includes/rodape.inc
 */
function getmicrotime(){
    list( $usec, $sec ) = explode( ' ', microtime() );
    return (float) $usec + (float) $sec;
}

// obtщm o tempo inicial da execuчуo
$Tinicio = getmicrotime();

$desativaDevTeste = true;

// carrega as funчѕes gerais
include_once realpath("../global/config.inc");
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Paginacao.class.inc";

// carrega as funчѕes especэficas do mѓdulo
include_once APPRAIZ . 'www/planointerno/_constantes.php';
include_once APPRAIZ . 'www/emenda/_constantes.php';
include_once APPRAIZ . 'www/planointerno/_funcoes.php';
include_once APPRAIZ . 'www/planointerno/_componentes.php';

// abre conexуo com o servidor de banco de dados
$db = new cls_banco();