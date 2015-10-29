<?php

	$configuracao = array();
	array_push( $configuracao, $_SESSION['sisid'] == 5 ? 'elaboracao' : 'proposta'  );
	include APPRAIZ .'includes/cadastro_usuario_geral.php';

?>