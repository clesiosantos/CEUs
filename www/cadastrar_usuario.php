<?php

if(isset($_POST["theme_simec"])){
	$theme = $_POST["theme_simec"];
	setcookie("theme_simec", $_POST["theme_simec"] , time()+60*60*24*30, "/");
} else {
	if(isset($_COOKIE["theme_simec"])){
		$theme = $_COOKIE["theme_simec"];
	}else{
			$theme = "versao antiga";
	}
}

	/**
	 * Sistema Integrado de Monitoramento do Ministério da Educação
	 * Setor responsvel: SPO/MEC
	 * Desenvolvedor: Desenvolvedores Simec
	 * Módulo: Segurança
	 * Finalidade: Solicitação de cadastro de contas de usuário.
	 * Data de criação:
	 * Última modificação: 30/08/2006
	 */

	define("SIS_PDEESCOLA", 34);
	define("SIS_PSEESCOLA", 65);

	// carrega as bibliotecas internas do sistema
	include "config.inc";
	require APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";

	// Constantes do CNC, forçando a escolha do sistema
	include APPRAIZ . "/www/conferencia/_constantes.php";
	$_REQUEST['sisid']  = CNC_SISID;
	$bloquearSisEscolha = true;

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	if(!$theme) {
		$theme = $_SESSION['theme_temp'];
	}

	// Particularidade feita para o PDE Escola
	$selecionar_modulo_habilitado = 'S';
	if($_REQUEST['banner_pdeescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PDEESCOLA;
	}
	if($_REQUEST['banner_pseescola']=='acessodireto') {
		$selecionar_modulo_habilitado = 'N';
		$_REQUEST['sisid'] = SIS_PSEESCOLA;
	}


	$sisid  		= $_REQUEST['sisid'];
	$usucpf 		= $_REQUEST['usucpf'];

	// leva o usuário para o passo seguinte do cadastro
	if ($_REQUEST['usucpf'] && $_REQUEST['modulo'] && $_REQUEST['varaux'] == '1') {
		$_SESSION = array();
		if($theme) $_SESSION['theme_temp'] = $theme;
		header("Location: cadastrar_usuario_2.php?sisid=$sisid&usucpf=$usucpf");
		exit();
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
	<script>
		function ImprimeStatus(texto){
    		document.formul.numCaracteres.value = texto
		}
	</script>

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements
	================================================== -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
    <![endif]-->
</head>

<body>
    <? include "barragoverno.php"; ?>
    <?php
	$mensagens = implode( '<br/>', (array) $_SESSION['MSG_AVISO'] );
	$_SESSION['MSG_AVISO'] = null;
	$titulo_modulo = 'Solicitação de Cadastro de Usuários';
	$subtitulo_modulo = 'Preencha os Dados Abaixo e clique no botão: "Continuar".<br/>'. obrigatorio() .' Indica Campo Obrigatório.'. $mensagens;
	//	monta_titulo( $titulo_modulo, $subtitulo_modulo );
	?>
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
                	<li><a href="#" class="btn btn-large btn-inverse">Solicitar Cadastro</a></li>
                    <li><a href="recupera_senha.php" class="btn btn-large btn-primary">ESQUECEU A SENHA?</a></li>
                </ul>
            </div>
        </div>
        <!-- End Top Bar -->

        <!-- Slider -->
        <div class="form-curse">
        	<div class="bg-map animated fadeIn"></div>
            <img src="/imagens/ceus-texto-governo.png" style="width: 20%; height: 20%; float: left; margin-top: 150px;" alt=" " />
            <div  style="width: 20%; height: 20%; float: right; margin-top: 150px;">&nbsp;</div>

        	<div class="container">
            	<div class="row-fluid">
            		<div class="span7 curse-form-box flipInX">
            			<h3 class="nm text-center">Solicitação de <strong>Cadastro</strong></h3>
            			<div class="cont">
	            			<p class="text-center">Preencha os Dados Abaixo e clique no botão <br><strong>"Continuar"</strong></p>

                            <form class="nm" method="post" name="formulario" id="formulario" onsubmit="return false;">
<input type=hidden name="modulo" value="./inclusao_usuario.php"/>
<input type=hidden name="varaux" value="">

	            				<div  id="loading" style="display: none" class='alert'>
					  				<a class='close' data-dismiss='alert'>×</a>
					  				Loading
								</div>
								<div id="response"></div>
                                <tr>
  	<td align="center">
  	<? if( strlen($mensagens) > 5 ){?>
	<div class="error_msg"><? echo (($mensagens)?$mensagens:""); ?></div>
	<? } ?>
	</td>
  </tr>

	            				<table width="95%">
	<tr>
		<td width="26%" align='right' style="font-weight: bold;"></td>
		<td width="74%">
		<?php
		$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t' order by descricao ";
		if($bloquearSisEscolha === true && $_REQUEST['sisid']){
    		$sql = "select s.sisid as codigo, s.sisabrev as descricao from seguranca.sistema s where s.sisstatus='A' and sismostra='t' and sisid = {$_REQUEST['sisid']} order by descricao ";
            $sistema = $db->pegaLinha($sql);
            echo "<input type='hidden' id='sisid' name='sisid' value='{$sistema['codigo']}' />";
        }else{
    		$db->monta_combo( "sisid", $sql, $selecionar_modulo_habilitado, "&nbsp;", 'selecionar_modulo', '');
    		echo obrigatorio();
        }
		?>
		</td>
	</tr>
	<?php if( $sisid ): ?>
		<tr>
			<td align='right' class="subtitulodireita">&nbsp;</td>
			<td>
				<?php
					$sql = sprintf( "select sisid, sisdsc, sisfinalidade, sispublico, sisrelacionado from sistema where sisid = %d", $sisid );
					$sistema = (object) $db->pegaLinha( $sql );
					if ( $sistema->sisid ) :
				?>
				<?php endif; ?>
			</td>
		</tr>
	<?php endif; ?>
	<input type="hidden" name="sisfinalidade_selc" value="<?=$sisfinalidade_selc?>"/>

	<tr>
		<td style="font-weight: bold;" align='right'>CPF:</td>
		<td>
			<input id="usucpf" type="text" name="usucpf" value=<? print '"'.$usucpf.'"'; ?> class="login_input" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" />
			<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.'>
		</td>
	</tr>
	<tr>
		<td>&nbsp;</td>
		<td>
			<a class="btn btn-large btn-primary" href="javascript:validar_formulario()" >Continuar</a>
			<a class="btn btn-large btn-primary" href="./login.php" >Voltar</a>
		</td>
	</tr>
	</table>
	            				<div class="row-fluid">
                            		<div class="span12 text-center">

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
<script language="javascript">

	function selecionar_modulo()
    {
		document.formulario.submit();
	}

	function validar_formulario()
    {
        var validacao = true;
        var mensagem  = '';

        if (document.formulario.sisid.value == "" ) {
            mensagem += '\nSelecione o módulo no qual você pretende ter acesso.';
            validacao = false;
        }

        if (document.formulario.usucpf.value == '' || !validar_cpf(document.formulario.usucpf.value)) {
            mensagem += '\nO cpf informado não é válido.';
            validacao = false;
        }

        document.formulario.varaux.value = '1';

        if ( !validacao ) {
            alert( mensagem );
        }else{
        	document.formulario.submit();
        }
	}
</script>