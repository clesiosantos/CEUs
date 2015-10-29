<?php
if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {
	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}else{
		$theme = 'versao antiga';
	}
}


	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Módulo: Segurança
	 * Finalidade: Permite que o usuário solicite uma nova senha.
	 * Última modificação: 26/08/2006
	 */

	function erro(){
		global $db;
		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'] = func_get_args();
		header( "Location: ". $_SERVER['PHP_SELF'] );
		exit();
	}

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}

	// executa a rotina de recuperação de senha quando o formulário for submetido
	if ( $_POST['formulario'] ) {

		// verifica se a conta está ativa
		$sql = sprintf(
			"SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = '%s'",
			corrige_cpf( $_REQUEST['usucpf'] )
		);
		$usuario = (object) $db->pegaLinha( $sql );
		if ( $usuario->suscod != 'A' ) {
			erro( "A conta não está ativa." );
		}

		$_SESSION['mnuid'] = 10;
		$_SESSION['sisid'] = 4;
		$_SESSION['exercicio_atual'] = $db->pega_ano_atual();
		$_SESSION['usucpf'] = $usuario->usucpf;
		$_SESSION['usucpforigem'] = $usuario->usucpf;

		// cria uma nova senha
	    //$senha = $db->gerar_senha();
	    $senha = strtoupper(senha());
		$sql = sprintf(
			"UPDATE seguranca.usuario SET ususenha = '%s', usuchaveativacao = 'f' WHERE usucpf = '%s'",
			md5_encrypt_senha( $senha, '' ),
			$usuario->usucpf
		);
		$db->executar( $sql );

		// envia email de confirmação
// 	    $sql = "select ittemail from public.instituicao where ittstatus = 'A'";
	    $remetente = $email_from; // vem do global
		$destinatario = $usuario->usuemail;
		$assunto = $GLOBALS['parametros_sistema_tela']['sigla'] . " - Recuperação de Senha";
	    $conteudo = sprintf(
	    	"%s %s<br/>Sua senha foi alterada para %s<br>Ao se conectar, altere esta senha para a sua senha preferida.",
	    	$usuario->ususexo == 'F' ?  'Prezada Sra.': 'Prezado Sr.',
	    	$usuario->usunome,
	    	$senha
	    );
		enviar_email( $remetente, $destinatario, $assunto, $conteudo );

		$db->commit();
		$_SESSION = array();
		$_SESSION['MSG_AVISO'][] = "Recuperação de senha concluída. Em breve você receberá uma nova senha por email.";
		header( "Location: /" );
		exit();
	}

	if ( $_REQUEST['expirou'] ) {
		$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
	}

?>
<!DOCTYPE html>
<!--[if lt IE 7 ]><html class="ie ie6" lang="en"> <![endif]-->
<!--[if IE 7 ]><html class="ie ie7" lang="en"> <![endif]-->
<!--[if IE 8 ]><html class="ie ie8" lang="en"> <![endif]-->
<!--[if (gte IE 9)|!(IE)]><html lang="en" class="no-js"> <![endif]-->
<head>

	<!-- Basic Page Needs
	================================================== -->
	<meta charset="utf-8">
	<title><?php echo $GLOBALS['parametros_sistema_tela']['sigla-nome_completo'] ?></title>
	<meta name="description" content="description">
	<meta name="author" content="">

	<!-- Mobile Specific Metas
	================================================== -->
	<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">

    <!-- Fav and touch icons
	================================================== -->
    <link rel="shortcut icon" href="img/favicon.ico">
    <link rel="apple-touch-icon-precomposed" sizes="114x114" href="img/apple-touch-icon-114-precomposed.html">
    <link rel="apple-touch-icon-precomposed" sizes="72x72" href="img/apple-touch-icon-72-precomposed.html">
    <link rel="apple-touch-icon-precomposed" sizes="52x52" href="img/apple-touch-icon-57x57.html">

	<!-- Custom styles
	================================================== -->
	<link rel="stylesheet" href="css/responsiveslides.css" type="text/css" media="screen"/>
	<link rel="stylesheet" href="css/animate-custom.css"   type="text/css" media="screen"/>
	<link rel="stylesheet" href="css/style.css"            type="text/css" media="screen">

	<!-- Media querys
	================================================== -->
	<link href="css/media-queries.css" rel="stylesheet" media="screen">
	<!--[if IE 8 ]><link href="css/ie8.css" rel="stylesheet" media="screen"><![endif]-->

	<!-- Scripts Libs
	================================================== -->
	<script type="text/javascript" src="js/jquery.min.js"></script> <!-- 1.9.1 -->
    <?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
	<script type="text/javascript" src="../includes/funcoes.js"></script>

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements
	================================================== -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
</head>

<body>
    <?php include "barragoverno.php"; ?>
	<!-- modals da Esqueceu a Senha -->
	<div id="modal-senha" class="modal hide fade">
		<div class="modal-header">
			<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
			<h3>Reenviar Senha por e-mail</h3>
		</div>
		<form class="nm" action="#" method="post">
		<div class="modal-body">
			<input class="input-xlarge" type="text" required placeholder="Digite seu CPF" name="name" />

		</div>
		<div class="modal-footer">
			<a href="#" class="btn btn-primary">Lembrar Senha</a>
		</div>
		</form>
	</div>

    <!-- modals da Solicitação de Cadastro -->
	<div id="modal-register" class="modal hide fade">
		<div class="modal-header">
			<button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
			<h3>Solicitar Cadastro</h3>
		</div>
		<form class="nm" action="#" method="post">
		<div class="modal-body">
        <input class="input-xlarge" type="text" required placeholder="Módulo Conferência" disabled name="name" />
			<input class="input-xlarge" type="text" required placeholder="Digite seu CPF" name="name" />

		</div>
		<div class="modal-footer">
			<a href="#" class="btn btn-primary">Cadastrar</a>
		</div>
		</form>
	</div>

	<!-- Header-->
    <header>
    	<!-- Top Bar -->
    	<div class="top">
        	<div class="container">
            	<a class="logo" href="index.html"><img src="img/logo.png" alt="//"></a>
            	<ul class="btns-top">
                	<li><a href="cadastrar_usuario.php" class="btn btn-large btn-inverse">Solicitar Cadastro</a></li>
                    <li><a href="#" class="btn btn-large btn-primary">ESQUECEU A SENHA?</a></li>
                </ul>
            </div>
        </div>
        <!-- End Top Bar -->

        <!-- Slider -->
        <div class="form-curse">
            <img src="/imagens/ceus-texto-governo.png" style="width: 20%; height: 20%; float: left; margin-top: 150px;" alt=" " />
        	<div  style="width: 20%; height: 20%; float: right; margin-top: 150px;">&nbsp;</div>
        	<div class="bg-map animated fadeIn"></div>


        	<div class="container">
            	<div class="row-fluid">
            		<div class="span7 curse-form-box flipInX">
            			<h3 class="nm text-center">Recupere a <strong> Senha</strong></h3>
            			<div class="cont">


                            <form class="nm" method="post" name="formulario">
	<input type=hidden name="formulario" value="1"/>
	<input type=hidden name="modulo" value="./inclusao_usuario.php"/>

	            				<div  id="loading" style="display: none" class='alert'>
					  				<a class='close' data-dismiss='alert'>×</a>
					  				Loading
								</div>
								<div id="response"></div>
                                <table border="0" cellspacing="0" cellpadding="3">
				<tr>
					<td style="font-weight: bold;text-align: right;width:150px">CPF:</td>
					<td style="width:250px" >
						<input type="text" name="usucpf" value="" class="login_input" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
						<?= obrigatorio(); ?>
					</td>
			 	</tr>
				<tr>
					<td></td>
					<td align="left"  >
						<a class="btn btn-large btn-primary" href="javascript:enviar_formulario()" >Lembrar Senha</a>
						<a class="btn btn-large btn-primary" href="./login.php" >Voltar</a>
					</td>
				</tr>
          </table>
	            			</form>
						</div>
            		</div>
                </div>
            </div>
		</div>
        <!-- End Slider -->

    </header>
    <!-- End Header-->



    <!--Content Intro--><!--End Content Intro-->


     <!--Content Steps--><!--End Content Steps-->



     <!--Video List--><!--End Video List-->




     <!--Testimonials--><!--End Testimonials-->


     <!--learn--><!--End Video List-->

     <!--learn--><!--End Video List-->


     <!--Brands--><!--End Brands-->

	<!-- pricing
	======================================================= --><!-- end pricing -->


    <!--Copyright-->
	 <div class="copy">
		 <section class="container">
  			<div class="row-fluid">
	    		<div class="span12">
	    			<p>© 2014 Ministério da Cultura. Todos os Direitos Reservados. <a href="#">MinC</a></p>
	    		</div>
			</div>
		</section>
	</div>
    <!--end Copyright-->





	<!-- ======================= JQuery libs =========================== -->

        <!-- Bootstrap.js-->
        <script src="js/bootstrap.js"></script>

        <!-- Video Responsive-->
        <script src="js/jquery.fitvids.min.js" type="text/javascript"></script>

        <!--Video Responsive-->
        <script src="js/jquery.placeholder.min.js" type="text/javascript"></script>

        <!-- Slider -->
        <script type="text/javascript" src="js/responsiveslides.min.js"></script>

        <!-- Custom js -->
        <script src="js/jquery-func.js" type="text/javascript"></script>

	<!-- ======================= End JQuery libs =========================== -->

  </body>
</html>
<script language="javascript">

	document.formulario.usucpf.focus();

	function enviar_formulario() {
		if ( validar_formulario() ) {
			document.formulario.submit();
		}
	}

	function validar_formulario() {
		var validacao = true;
		var mensagem = '';
		if ( !validar_cpf( document.formulario.usucpf.value ) ) {
			mensagem += '\nO cpf informado não é válido.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}

</script>
