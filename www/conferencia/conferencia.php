<?
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
function __autoload($class_name) {
		$arCaminho = array(
							APPRAIZ . "includes/classes/modelo/seguranca/",
							APPRAIZ . "includes/classes/modelo/public/",
							APPRAIZ . "includes/classes/modelo/territorios/",
							APPRAIZ . "includes/classes/questionario/",
							APPRAIZ . "includes/classes/modelo/entidade/",
							APPRAIZ . "includes/classes/modelo/planointerno/",
							APPRAIZ . "includes/classes/modelo/questionario/",
							APPRAIZ . "includes/classes/modelo/tabela/",
							APPRAIZ . "includes/classes/modelo/workflow/",
							APPRAIZ . "includes/classes/tabela/",
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

	// inclui os objetos do sistema.
	include_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . "includes/classes/Modelo.class.inc";

$modulo=$_REQUEST['modulo'];
// include "_funcoesliberacao.php";
// include "_constantes.php";

include_once '_constantes.php';
include_once '_funcoes.php';


//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
