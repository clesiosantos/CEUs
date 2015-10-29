<?
 /*
   Sistema Simec
   Setor respons�vel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   M�dulo:simec.php
   Finalidade: permitir a abertura de todas as p�ginas do sistema com seguran�a
   */
//Mede o tempo de execu��o da p�gina em microsegundos
function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);}
$Tinicio = getmicrotime();
//controla cache de navega��o
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Cache-control: private, no-cache");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Pragma: no-cache");


function __autoload($class_name) {
    $arCaminho = array(
            APPRAIZ . "includes/classes/modelo/seguranca/",
            APPRAIZ . "includes/classes/modelo/public/",
            APPRAIZ . "includes/classes/modelo/territorios/",
            APPRAIZ . "includes/classes/questionario/",
            APPRAIZ . "includes/classes/modelo/entidade/",
            APPRAIZ . "includes/classes/tabela/",
            APPRAIZ . "credito/classes/",
            APPRAIZ . "includes/classes/modelo/questionario/",
            APPRAIZ . "includes/classes/modelo/tabela/",
            APPRAIZ . "conferencia/classes/",
            APPRAIZ . "includes/classes/controller/",
            APPRAIZ . "includes/classes/",
            APPRAIZ . "includes/classes/view/",
            APPRAIZ . "includes/classes/html/",
    );

    foreach($arCaminho as $caminho){
        $arquivo = $caminho . $class_name . '.class.inc';
        if ( file_exists( $arquivo ) ){
            require_once( $arquivo );
            break;
        }
    }
}

/* Define o limite de tempo do cache em 100 minutos */
include "config.inc";

include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/classes/Modelo.class.inc";
include APPRAIZ . "seguranca/www/_funcoes.php";


$db = new cls_banco();
//Emula outro usu�rio

if ($_POST['usucpf_simu'] and ($_SESSION['superuser'] or $db->testa_uma() or $_SESSION['usuuma'])) $_SESSION['usucpf']=$_POST['usucpf_simu'];

if( ! isset($_SESSION['usucpf']) ) {
        $_SESSION = array();
		$_SESSION['MSG_AVISO'] = 'Sua sess�o expirou ou n�o foi autorizada. Fa�a novo acesso.';
		header('Location: login.php');
        exit();
}
$modulo=$_REQUEST['modulo'];



$sql= "select ittemail, orgcod,ittabrev from instituicao where ittstatus='A'";
$RS = $db->record_set($sql);
$res = $db->carrega_registro($RS,0);
$_SESSION['ittemail']= trim($res['ittemail']);
$_SESSION['ittorgao']= trim($res['orgcod']);
$_SESSION['ittabrev']= trim($res['ittabrev']);
$_SESSION['sigla']= trim($res['ittsistemasigla']);

 $sql = "select u.usuchaveativacao from seguranca.usuario u where u.usucpf='".
 $_SESSION['usucpf']."'";
 $chave = $db->recuperar($sql);

// include "includes/erros.inc";

 if ($chave['usuchaveativacao']== 'f')
 {

    include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/sistema/usuario/altsenha.inc";
	include APPRAIZ."includes/rodape.inc";
}

else
{
    if ($_REQUEST['modulo'])
    {
        include APPRAIZ.'includes/testa_acesso.inc';
        include APPRAIZ.$_SESSION['sisdiretorio']."/modulos/".$_REQUEST['modulo'].".inc";
	   	include APPRAIZ."includes/rodape.inc";
    }
    else
    {
	   header("Location: login.php");
    }
}
?>