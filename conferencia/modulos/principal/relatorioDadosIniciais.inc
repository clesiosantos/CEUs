<?php
header ( 'Content-type: text/html; charset="iso-8859-1"', true );
header ( "Cache-Control: no-store, no-cache, must-revalidate" ); // HTTP/1.1
header ( "Cache-Control: post-check=0, pre-check=0", false );
header ( "Pragma: no-cache" ); // HTTP/1.0 Canhe Livre

testarResponsabilidadeVinculoCeuTela ( true );
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$mDadosInciais = new DadosIniciais ();
$mVinculoCeu = new VinculoCeu ();
$mFotoDadosIniciais = new FotoDadosIniciais ();
$mEndereco = new EnderecoCeu ();
$mEnderecoPrefeitura = new EnderecoCeu ();
$mArquivo = new Arquivo ();
$mTelPrefeitura = new TelefonePrefeitura ();
$mTelCeu = new TelefoneCeu ();

$mVinculoCeu->getVinculoSession ();

extract ( trataDadosVazios($mVinculoCeu->getDados ()) );
$mCeu = new Ceu ( $ceuid );
extract ( trataDadosVazios($mCeu->recuperarDados ()) );

$arDadosIniciais = $mDadosInciais->getDadosByVinculoCeu ( $vceid );

if (is_array ( $arDadosIniciais )) {
	extract ( trataDadosVazios($arDadosIniciais) );
}

$flgenderecocorreto = $arDadosIniciais['edccep'] ? 'f' : 't';
$flginaugurado = $arDadosIniciais['divdtinauguracao'] ? 't' : 'f';


$arTelefonesPrefeitura = array();
$arTelefonesCeu = array();
$dadosFotos = array();
if($divid && $ceuid){
	$arTelefonesPrefeitura = $mTelPrefeitura->recuperarTodos("*", array("tprstatus = 'A'", "divid = {$divid}"));
	$arTelefonesCeu = $mTelCeu->recuperarTodos("*", array("tcestatus = 'A'", "ceuid = {$ceuid}"));
	$dadosFotos = $mFotoDadosIniciais->getFotosDadosIniciaisByWhere(array(" divid = '{$divid}' "));
}

echo monta_cabecalho_relatorio ( '100' );

function trataDadosVazios($arDados){
	foreach ($arDados as $chave => $dado){
		if($dado == '' || $dado == null){
			$arDados[$chave] = 'Não preenchido';
		}
	}

	return $arDados;
}
?>
<style>
table.tabelaQuestionario {
	/* background-color:#cccccc; */
	font-family: arial, sans-serif;
	font-size: 14px;
}

table.tabelaGrupo {
	background-color: #d1d1d1;
	font-family: arial, sans-serif;
	font-size: 14px;
}

td.bordaDireita {
	background-color: #ffffff;
	font-family: arial, sans-serif;
	font-size: 14px;
	width: 300px;
}

.fieldGrupoTitulo {
	color: #000000;
	font-family: arial, sans-serif;
	font-size: 11px;
	font-weight: bold;
}

.fieldGrupo {
	background-color: #ffffff;
	font-family: arial, sans-serif;
	font-size: 12px;
	padding-bottom: 10px;
	margin: 10px;
	border: 1px solid #cccccc;
}

.divQuestionario {
	text-align: left;
}

.divTituloQuestionario {
	text-align: center;
	font-family: arial, sans-serif;
	font-size: 16px;
	font-weight: bold;
}

.divPergunta {
	font-family: verdana, arial, sans-serif;
	font-size: 11px;
	background-color: #f1f1f1;
	margin-left: 10px;
	margin-right: 10px;
	margin-top: 5px;
	padding-bottom: 5px;
	border: 1px solid #cccccc;
}

.divTituloPergunta {
	background-color: #dddddd;
	padding-top: 3px;
	padding-bottom: 5px;
	border-bottom: 1px solid #cccccc;
}

.divResposta {
	font-family: verdana, arial, sans-serif;
	font-size: 11px;
	background-color: #f1f1f1;
	padding-top: 3px;
	padding-left: 10px;
}
</style>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css" />
<script type="text/javascript" src="/includes/funcoes.js"></script>
<center>
	<div class="divQuestionario">
		<div class="divTituloQuestionario">Dados Iniciais</div>
	</div>
</center>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">Endereço da prefeitura para contato</legend>
	<div class="divPergunta">
		<div class="divTituloPergunta">CEP</div>
		<div class="divResposta"><?= $edccepprefeitura ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Logradouro</div>
		<div class="divResposta"><?= $edclogradouroprefeitura?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Número</div>
		<div class="divResposta"><?= $edcnumeroprefeitura ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Complemento</div>
		<div class="divResposta"><?= $edccomplementoprefeitura ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Bairro</div>
		<div class="divResposta"><?= $edcbairroprefeitura ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Município/UF</div>
		<div class="divResposta"><?= $mundescricaoprefeitura . "/" . $estufprefeitura  ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Coordenadas Geográficas</div>
		<div class="divResposta">
		    <?php
		      if($edccoordenadasprefeitura){
                  $zoom       = $edczoomprefeitura ? $edczoomprefeitura : 5;
                  $coordenada = str_replace(array("POINT(", ")"), "", $edccoordenadasprefeitura);
                  $coordenada = explode(" ", $coordenada);
    		      echo "<img border='0' src='http://maps.googleapis.com/maps/api/staticmap?size=400x200&maptype=roadmap\&markers=size:mid%7Ccolor:red%7C{$coordenada[1]},{$coordenada[0]}&sensor=false&zoom={$zoom}'>";
              }
		    ?>
	    </div>
	</div>
</fieldset>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">Telefone(s) da prefeitura para contato</legend>

	<?php
	     foreach($arTelefonesPrefeitura as $chave => $telefone):
	     ?>
			<div class="divPergunta">
				<div class="divTituloPergunta">Telefone <?= $chave + 1 ?></div>
				<div class="divResposta"><?= "(" . $telefone['tprddd'] . ")" . $telefone['tprtelefone']  ?></div>
			</div>
	     <?php
	     endforeach;
	?>
</fieldset>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">Endereço da Praça</legend>
	<div class="divPergunta">
		<div class="divTituloPergunta">CEP</div>
		<div class="divResposta"><?= $ceuedccep ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Logradouro</div>
		<div class="divResposta"><?= $ceuedclogradouro ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Número</div>
		<div class="divResposta"><?= $ceuedcnumero ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Complemento</div>
		<div class="divResposta"><?= $ceuedccomplemento ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Bairro</div>
		<div class="divResposta"><?= $ceuedcbairro ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">Município/UF</div>
		<div class="divResposta"><?= $ceumundescricao . "/" . $ceuestuf  ?></div>
	</div>
	<div class="divPergunta">
		<div class="divTituloPergunta">O endereço está correto?</div>
		<div class="divResposta"><?= $flgenderecocorreto == 't' ? 'Sim' : 'Não' ?></div>
	</div>
</fieldset>

<?php
if($flgenderecocorreto == 'f'):
?>
	<fieldset class="fieldGrupo">
		<legend class="fieldGrupoTitulo">Endereço da Praça atualizado</legend>
		<div class="divPergunta">
			<div class="divTituloPergunta">CEP</div>
			<div class="divResposta"><?= $edccep ?></div>
		</div>
		<div class="divPergunta">
			<div class="divTituloPergunta">Logradouro</div>
			<div class="divResposta"><?= $edclogradouro ?></div>
		</div>
		<div class="divPergunta">
			<div class="divTituloPergunta">Número</div>
			<div class="divResposta"><?= $edcnumero ?></div>
		</div>
		<div class="divPergunta">
			<div class="divTituloPergunta">Complemento</div>
			<div class="divResposta"><?= $edccomplemento ?></div>
		</div>
		<div class="divPergunta">
			<div class="divTituloPergunta">Bairro</div>
			<div class="divResposta"><?= $edcbairro ?></div>
		</div>
		<div class="divPergunta">
			<div class="divTituloPergunta">Município/UF</div>
			<div class="divResposta"><?= $mundescricao . "/" . $estuf  ?></div>
		</div>
		<div class="divPergunta">
		<div class="divTituloPergunta">Coordenadas Geográficas</div>
    		<div class="divResposta">
    		  <?php
    		      if($edccoordenadas){
                      $zoom       = $edczoom ? $edczoom : 5;
                      $coordenada = str_replace(array("POINT(", ")"), "", $edccoordenadas);
                      $coordenada = explode(" ", $coordenada);
        		      echo "<img border='0' src='http://maps.googleapis.com/maps/api/staticmap?size=400x200&maptype=roadmap\&markers=size:mid%7Ccolor:red%7C{$coordenada[1]},{$coordenada[0]}&sensor=false&zoom={$zoom}'>";
                  }
    		    ?>
    		</div>
		</div>
	</fieldset>
<?php
endif;
?>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">Telefone(s) para contato na Praça</legend>

	<?php
	     foreach($arTelefonesCeu as $chave => $telefone):
	     ?>
			<div class="divPergunta">
				<div class="divTituloPergunta">Telefone <?= $chave + 1 ?></div>
				<div class="divResposta"><?= "(" . $telefone['tceddd'] . ")" . $telefone['tcetelefone']  ?></div>
			</div>
	     <?php
	     endforeach;
	?>
</fieldset>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">CEU</legend>
	<div class="divPergunta">
		<div class="divTituloPergunta">Inaugurado</div>
		<div class="divResposta"><?= $flginaugurado == 't' ? 'Sim' : 'Não' ?></div>
	</div>
	<?php
    if($flginaugurado == 't'):
    ?>
		<div class="divPergunta">
				<div class="divTituloPergunta">Data de Inauguração</div>
				<div class="divResposta"><?= formata_data($divdtinauguracao) ?></div>
		</div>
	<?php
	endif;
	?>
</fieldset>

<fieldset class="fieldGrupo">
	<legend class="fieldGrupoTitulo">Fotos</legend>

	<?php
	     foreach($dadosFotos as $chave => $foto):
		     $mArquivo->carregarPorId($foto['arqid']);
		     $arDadosArquivo = $mArquivo->getDados();
	     ?>
			<div class="divPergunta">
				<div class="divTituloPergunta">Foto <?= $chave + 1 ?></div>
				<div class="divResposta"><?= $arDadosArquivo['arqnome'] . "." . $arDadosArquivo['arqextensao']?></div>
			</div>
	     <?php
	     endforeach;
	?>
</fieldset>

<script>
window.print();
</script>