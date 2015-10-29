<?php
/**
 * Tela que contem o formulário de cadastro de usuário específico para o módulo
 * de conferência do SIMINC e suas especificadas.
 *
 * @version 1.0
 */
// Carrega as bibliotecas internas do sistema.
include "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . 'www/conferencia/_constantes.php';
include APPRAIZ . 'www/conferencia/_funcoes.php';
include APPRAIZ . 'includes/classes/Modelo.class.inc';

// Variáveis utilizadas no arquivo.
$_SESSION['sisid'] = $_REQUEST['sisid'] ? $_REQUEST['sisid'] : '';
$usucpf            = $_REQUEST['usucpf'] ? $_REQUEST['usucpf'] : '';
$regcod            = $_REQUEST['regcod'] ? $_REQUEST['regcod'] : '';
$act               = $_REQUEST['act']    ? $_REQUEST['act'] : '';

$remetente['email'] = $email_from;
$remetente['nome']  = "Centros de Artes e Esportes Unificados";

$conteudo  = "Você foi cadastrado no sistema de gestão de CEUs - Centros de Artes e Esportes Unificados. Para entrar no sistema, utilize seu CPF e a senha ";

// Tema do sistema.
$theme = 'versao antiga';
if (isset($_POST["theme_simec"])) {
    $theme = $_POST["theme_simec"];
    setcookie("theme_simec", $_POST["theme_simec"], time() + 60 * 60 * 24 * 30, "/");
} else {
    if (isset($_COOKIE["theme_simec"])) {
        $theme = $_COOKIE["theme_simec"];
    }
}

// Valida se existe o sistema e o cpf informados.
if (empty($_SESSION['sisid']) || empty($usucpf)) {
    die("
        <script type='text/javascript'>
            alert('Sistema ou usuário não informados.');
            history.back(-2);
        </script>
    ");
}

// Classe do banco.
$db = new cls_banco();

// Requisições da tela.
switch ($act) :
    case 'carregaMunicipios':
        $sql                      = "SELECT
                    muncod AS codigo,
                    mundescricao AS descricao
                FROM
                    territorios.municipio
                WHERE
                    estuf = '{$regcod}'
                ORDER BY
                    mundescricao ASC";
        $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '200', 'S', 'muncod');
        die;
        break;
    case 'salvar':
        $_REQUEST['usucpf']       = corrige_cpf($_REQUEST['usucpf']);
        $_SESSION['usucpf']       = $_REQUEST['usucpf'];
        $_SESSION['usucpforigem'] = $_REQUEST['usucpf'];

        // Caso este usuário esteja na tabela de delegados para uma conferência ativa, ele será cadastrado com o perfil DELEGADO
//         $mDelegado = new Delegado();
//         $delegado  = $mDelegado->getDelegadoByCpf($_REQUEST['usucpf']);

//         $perfilPadrao = (count($delegado) > 0) ? CONFERENCIA_PERFIL_DELEGADO : CONFERENCIA_PERFIL_CADASTRO;
        $perfilPadrao = CONFERENCIA_PERFIL_CADASTRO;
        $arRetorno    = cadastrarUsuario($_REQUEST, $perfilPadrao);

        if ($arRetorno['cpf_cad_sistema'] && $arRetorno['cpf_cadastrado']) {
            die("
                <script type='text/javascript'>
                    alert('Usuário já cadastrado no sistema.');
                    window.location.href = 'login.php';
                </script>
            ");
        }

        if (!$arRetorno['cpf_cad_sistema'] || !$arRetorno['cpf_cadastrado']) {
            $conteudo = $conteudo . $arRetorno["senha"];
            enviar_email(
                    $remetente, $_REQUEST['usuemail'], "Cadastro no sistema ".$GLOBALS['parametros_sistema_tela']['sigla'], $conteudo
            );
            die("
                <script type='text/javascript'>
                    alert('Usuário cadastrado com sucesso.');
                    window.location.href = 'login.php';
                </script>
            ");
        }

        break;
endswitch;
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
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <?php
        if (is_file("includes/layout/{$theme}/include_login.inc")) {
            include "includes/layout/{$theme}/include_login.inc";
        }
        ?>
        <script type="text/javascript">
            function exibeThemas() {
                var div = document.getElementById('menu_theme');

                if (div.style.display == 'none')
                    div.style.display = '';
                else
                    div.style.display = 'none';
            }

            function alteraTema() {
                document.getElementById('formTheme').submit();
            }

            /**
             * Envia o ajax para carregar os municípios de determinado estado.
             */
            function listarMunicipios(regcod) {
                if (regcod) {
                    $.ajax(
                            {
                                url: 'cadastrar_usuario_conferencia.php?sisid=<?php echo $_SESSION['sisid']; ?>&usucpf=<?php echo $usucpf; ?>',
                                type: 'POST',
                                data: {'act': 'carregaMunicipios', 'regcod': regcod},
                                async: 'false',
                                success: function(html) {
                                    if (html) {
                                        $('#divMunicipio').html(html);
                                    }
                                }
                            }
                    );
                } else {
                    $('#divMunicipio').html('<span style="color: #AAAAAA;">Selecione uma UF</span>');
                }
            }

            function salvar() {
                var msg = '';

                if ($('#usucpf').val() == '') {
                    msg += '\n\tCPF';
                }

                if ($('#usunome').val() == '') {
                    msg += '\n\tNome';
                }

                if ($('#ususexo').val() == '') {
                    msg += '\n\tSexo';
                }

                if ($('#regcod').val() == '') {
                    msg += '\n\tUF';
                }

                if ($('#muncod').val() == '') {
                    msg += '\n\tMunicípio';
                }

                if ($('#usurgnum').val() == '') {
                    msg += '\n\tNúmero de RG (Identidade)';
                }

                if ($('#usuorgexp').val() == '') {
                    msg += '\n\tÓrgão Expeditor';
                }

                if ($('#usurguf').val() == '') {
                    msg += '\n\tUF';
                }

                if ($('#usufoneddd').val() == '') {
                    msg += '\n\tDDD';
                }

                if ($('#usufonenum').val() == '') {
                    msg += '\n\tTelefone';
                }

                if ($('#usuemail').val() == '') {
                    msg += '\n\tE-mail';
                }

                if ($('#usuemailconfirmacao').val() == '') {
                    msg += '\n\tConfirmação do e-mail';
                }

                if (msg != '') {
                    alert('Os campos listados são obrigatórios e devem ser preenchidos:' + msg);
                    return false;
                }

                if ($('#usuemail').val() != $('#usuemailconfirmacao').val()) {
                    alert('Os e-mails informados não conferem.');
                    return false;
                }

                $('#act').val('salvar');
                $('#formCadastraUsuarioConferencia').submit();
            }
        </script>

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
                	<li><a href="cadastrar_usuario.php" class="btn btn-large btn-inverse">Solicitar Cadastro</a></li>
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
            			<h3 class="nm text-center">Solicitação de <strong>Cadastro</strong></h3>
            			<div class="cont">
	            			<p class="text-center">&nbsp;</p>
	            			<form class="nm" action="" method="post" name="formCadastraUsuarioConferencia" id="formCadastraUsuarioConferencia">
            <input type="hidden" name="sisid" id="sisid" value="<?php echo $_SESSION['sisid']; ?>">
            <input type="hidden" id="usucpf" name="usucpf" value="<?php echo $usucpf; ?>"/>
            <input type="hidden" id="act" name="act" value=""/>
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="34%">Módulo:</td>
    <td width="66%" style="font-size:14px"><strong>Centros de Artes e Esportes Unificados</strong></td>
  </tr>
  <tr>
    <td>CPF:</td>
    <td><?php echo campo_texto('usucpf', 'N', 'N', '', 20, 50, '', '', '', '', '', 'id="usucpf"', ''); ?></td>
  </tr>
  <tr>
    <td>Nome:</td>
    <td><?php echo campo_texto('usunome', 'S', 'S', '', 40, 50, '', '', '', '', '', 'id="usunome"', ''); ?></td>
  </tr>
  <tr>
    <td>Sexo:</td>
    <td><input id="sexo_masculino" type="radio" name="ususexo" value="M" <?= ($ususexo == 'M' ? "CHECKED" : "") ?> <?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> Masculino<br><input id="sexo_feminino" type="radio" name="ususexo" value="F"	<?= ($ususexo == 'F' ? "CHECKED" : "") ?>	<?= $cpf_cadastrado ? 'disabled="disabled"' : '' ?> /> Feminino</td>
  </tr>
  <tr>
    <td>UF:</td>
    <td><?php
                        $sql = "SELECT regcod AS codigo, regcod||' - '||descricaouf AS descricao FROM uf WHERE codigoibgeuf IS NOT NULL ORDER BY 2";
                        $db->monta_combo("regcod", $sql, 'S', "Selecionar...", 'listarMunicipios', '', '', '', 'S', 'regcod');
                        ?></td>
  </tr>
  <tr>
    <td>Município:</td>
    <td><div id="divMunicipio">
                            <span style="color: #AAAAAA;">Selecione uma UF.</span>
                        </div></td>
  </tr>
  <!--
  <tr>
    <td>Número de RG:</td>
    <td><?= campo_texto('usurgnum', 'S', 'S', '', 9, 10, '#.###.###', '', '', '', '', 'id="usurgnum"', ''); ?></td>
  </tr>
  <tr>
    <td>Órgão Expeditor:</td>
    <td><?= campo_texto('usuorgexp', 'S', 'S', '', 9, 9, '', '', '', '', '', 'id="usuorgexp"', ''); ?></td>
  </tr>
  <tr>
    <td>UF:</td>
    <td><?php
                        $db->monta_combo("usurguf", $sql, 'S', 'Selecione...', '', '', '', '200', 'S', "usurguf", false, null);
                        ?></td>
  </tr>
   -->
  <tr>
    <td>DDD:</td>
    <td><?= campo_texto('usufoneddd', '', 'S', '', 3, 2, '##', ''); ?></td>
  </tr>
  <tr>
    <td>Telefone:</td>
    <td><?= campo_texto('usufonenum', 'S', 'S', '', 18, 15, '###-####|####-####|#####-####', ''); ?></td>
  </tr>
  <tr>
    <td>E-mail:</td>
    <td><?php echo campo_texto('usuemail', 'S', 'S', '', 40, 100, '', '', '', '', '', 'id="usuemail"', ''); ?></td>
  </tr>
  <tr>
    <td>Confirmação do E-mail:</td>
    <td><?php echo campo_texto('usuemailconfirmacao', 'S', 'S', '', 40, 100, '', '', '', '', '', 'id="usuemailconfirmacao"', ''); ?></td>
  </tr>
</table>

            <div class="span6"></div>
                               	<div class="row-fluid">
                            		<div class="span12 text-center">

                                        <input class="btn btn-large btn-primary" type="button" value="Salvar" onclick="salvar();"/>
                        <input class="btn btn-large btn-primary" type="button" value="Voltar" />
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
