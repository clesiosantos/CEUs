<?php

/**
 * Sistema Integrado de Monitoramento, Execução e Controle
 * Setor responsvel: DTI/SE/MEC
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 24/08/2008
 */

//Verifica Temas
if(isset($_COOKIE["theme_simec"])){
	$theme = $_COOKIE["theme_simec"];
}

 if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
}else
{
	$theme = "versao antiga";
}
// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
	die('<script>
			alert(\'CPF inválido!\');
			history.go(-1);
		 </script>');
}

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

// executa a rotina de autenticação quando o formulário for submetido
if ( $_POST['formulario'] ) {
	include APPRAIZ . "includes/autenticar.inc";
}

if ( $_REQUEST['expirou'] ) {
	$_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}


//Define um tema existente (padrão), caso nenhum tenha sido escolhido

if(!$theme) {

	$diretorio = APPRAIZ."www/includes/layout";
	if(is_dir($diretorio)){
		if ($handle = opendir($diretorio)) {
		   while (false !== ($file = readdir($handle))) {
			  if ($file != "." && $file != ".." && $file != ".svn" && is_dir($diretorio."/".$file)) {
				  $dirs[] = $file;
			  }
		   }
		   closedir($handle);
		}
	}

	if($dirs) {
		// sorteia um tema para exibição
		$theme = $dirs[rand(0, (count($dirs)-1))];
		$_SESSION['theme_temp'] = $theme;
	}

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
    <script type="text/javascript" src="../includes/funcoes.js"></script>
	<?php if(is_file( "includes/layout/".$theme."/include_login.inc" )) include "includes/layout/".$theme."/include_login.inc"; ?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery.accordion.source.js"></script>
	<script src="../includes/BeautyTips/excanvas.js" type="text/javascript"></script>
	<script type="text/javascript" src="../includes/BeautyTips/jquery.bt.min.js"></script>

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
            	<a class="logo" href="login.php"><img src="img/logo.png" alt="//"></a>
            	<ul class="btns-top">
                	<li><a href="./cadastrar_usuario.php" class="btn btn-large btn-inverse">Solicitar Cadastro</a></li>
                    <li><a href="recupera_senha.php" class="btn btn-large btn-primary">ESQUECEU A SENHA?</a></li>
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
            			<h3 class="nm text-center">Acesse o <strong>Sistema</strong></h3>
            			<div class="cont">
	            			<p class="text-center">
	            			    &nbsp;
	            			</p>

                            <form class="nm" id="formulario" name="formulario" method="post">

<input type="hidden" name="formulario" value="1"/>

<input type="hidden" id="arquivo_login" name="arquivo_login" value="" />

	            				<div  id="loading" style="display: none" class='alert'>
					  				<a class='close' data-dismiss='alert'>×</a>
					  				Loading
								</div>
								<div id="response"></div>
								<? if ( $_SESSION['MSG_AVISO'] ): ?>
		  <div class="error_msg">
		  <ul><li><?= implode( '</li><li>', (array) $_SESSION['MSG_AVISO'] ); ?></li></ul>
		  </div>
		  <? endif; ?>
		  <? $_SESSION['MSG_AVISO'] = array(); ?>
	            				<div class="row-fluid">
                            		<div class="span6">
                                        <input style="margin-bottom:10px"  type="text" name="usucpf" value="" size="20" required placeholder="Digite seu CPF" class="input-large" onkeypress="return controlar_foco_cpf( event );" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
                                	</div>
                                	<div class="span6">
                                        <input type="password" name="ususenha" class="input-large" required placeholder="Digite sua Senha" autocomplete="off" size="20" onkeypress="return controlar_foco_senha( event );" />
                               	    </div>
                                </div>
	            				<div class="row-fluid">
                            		<div class="span12 text-center">
										<a class="btn btn-large btn-primary" href="javascript:enviar_formulario()" >Acessar o Sistema</a>
                               	    </div>
                                </div>
	            				<div class="row-fluid">
                            		<div class="span6"></div>
                                	<div class="span6"></div>
                                </div>
	            				<div class="row-fluid">
                            		<div class="span12"></div>
                                </div>
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
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<script language="javascript">

	$('#img_change_theme').bt({
  		trigger: 'none',
  		contentSelector: "$('#tutorial_theme')",
  		width: 200,
  		shadow: true,
	    shadowColor: 'rgba(0,0,0,.5)',
	    shadowBlur: 8,
	    shadowOffsetX: 4,
	    shadowOffsetY: 4
	});

$(document).ready(function () {
	$('#img_change_theme').btOn();
	window.setTimeout("$('#img_change_theme').btOff()", 10000);
});

	if ( document.formulario.usucpf.value == '' ) {
		document.formulario.usucpf.focus();
	} else {
		document.formulario.ususenha.focus();
	}

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
		if ( document.formulario.ususenha.value == "" ) {
			mensagem += '\nÉ necessário preencher a senha.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}

		//limpa variavel de download
		var arquivo = document.getElementById("arquivo_login");
		arquivo.value = "";

		return validacao;
	}

	function controlar_foco_cpf( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return document.formulario.ususenha.focus();
			};
		} else {
			return true;
		}
	}

	function controlar_foco_senha( evento ) {
		if ( window.event || evento.which ) {
			if ( evento.keyCode == 13) {
				return enviar_formulario();
			};
		} else {
			return true;
		}
	}

	function abreArquivo(arq)
	{
		var form	= document.getElementById("formulario");
		var arquivo = document.getElementById("arquivo_login");

		arquivo.value = arq;
		form.submit();
	}

	/*** INICIO SHOW MODAL ***/
	function montaShowModal() {
		var alert='';
		alert += '<p align=center style=font-size:15;><font size=4 color=red><b>Atenção!</b></font><br>Seu navegador de internet está ultrapassado.<br/><br/>Em breve vamos descontinuar o suporte para Internet Explorer 6 e versões anteriores.<strong><br/><br/> Atualize seu navegador para obter uma experiência on-line mais rica, sugerimos algumas opções para download nos links abaixo:</strong></p>';
		alert += '<p><a target=_blank href=http://www.google.com/chrome/index.html?brand=CHNY&amp;utm_campaign=en&amp;utm_source=en-et-youtube&amp;utm_medium=et><img src=../imagens/browsers_chrome.png border=0></a> <a target=_blank href=http://www.microsoft.com/windows/internet-explorer/default.aspx><img src=../imagens/browsers_ie.png border=0></a> <a target=_blank href=http://www.mozilla.com/?from=sfx&amp;uid=267821&amp;t=449><img src=../imagens/browsers_firefox.png border=0></a></p>';
		alert += '<p align=center><input type=button value=Fechar onclick=closeMessage();></p>';
		displayStaticMessage(alert,false,'280');
		return false;
	}

	function displayStaticMessage(messageContent,cssClass,height) {
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(570,height);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes
		messageObj.display();
	}

	function closeMessage() {
		messageObj.close();
	}
	/*** FIM SHOW MODAL ***/

</script>

<?php
// verificando se o browser é IE6 ou inferior
require APPRAIZ . "includes/classes/browser.class.inc";
$browser = new Browser();
if( $browser->getBrowser() == Browser::BROWSER_IE && $browser->getVersion() <= 6 ) {
	?>
		<script>montaShowModal();</script>
	<?
}
?>
