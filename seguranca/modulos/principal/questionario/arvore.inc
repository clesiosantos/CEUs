<?php
include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
include_once APPRAIZ . "includes/classes/questionario/QArvore.class.inc";

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";


$obArvore = new QArvore( array("relatorio" => 'N') );

//array("questionario" => new Questionario(10) )
if ( $_GET['instrucao'] == 'excluir' ){
    excluirRegistro($_REQUEST);
	// die("<script>alert('$msg'); window.location = 'seguranca.php?modulo=principal/questionario/arvore&acao=A&sisidFiltro=".$_GET['sisidFiltro']."';</script>");
}

function excluirRegistro( array $parametros){
    //	$s  = new Sistema();
    $q  = new QQuestionario($parametros['queid']);
    $a  = new QGrupo($parametros['grpid']);
    $p  = new QPergunta($parametros['perid']);
    //	$it = new Itempergunta();
    if ($parametros['queid']){
        if( $q->excluir() ){
            $q->commit();
            $msg = array ( 'msg'  => ('Operação realizada com sucesso!'),
                    'erro' => false );
            //die("<script>alert('$msg'); window.location = 'survey.php?modulo=sistema/questionario/arvore&acao=A&sisidFiltro=".$parametros['sisidFiltro']."';</script>");

        }else{
            $msg = array ( 'msg'  => ('Operação não realizada!') ."\n". ('Existem Grupos/Perguntas associadas a este Questionário.'),
                    'erro' => true );
        }
    }elseif ($parametros['grpid']){
        if($a->gru_grpid){
            $where = array("gru_grpid" => $a->gru_grpid);
        } elseif ($a->queid){
            $where = array("queid" => $a->queid);
        } else {
            $where = array("itpid" => $a->itpid);
        }
        $a->excluirOrdenacao( $parametros['grpid'] );
        if( $a->excluir() ){
            $a->commit();
            $a->ordena($where);
            $msg = array ( 'msg'  => ('Operação realizada com sucesso!'),
                    'erro' => false );
        }else{
            $msg = array ( 'msg'  => ('Operação não realizada!') ."\n". ('Existem Grupos/Perguntas associadas a este grupo.'),
                    'erro' => true );
        }
    }elseif ($parametros['perid']){
        if($p->grpid){
            $where = array("grpid" => $p->grpid);
        } elseif ($p->queid){
            $where = array("queid" => $p->queid);
        } else {
            $where = array("itpid" => $p->itpid);
        }
        if($p->perguntaRespondida()){
            $msg = array ( 'msg'  => ('Operação não realizada!')."\n".('Essa pergunta já possui resposta.'),
                    'erro' => true );
        } else {
            //             $p->excluirOrdenacao( $parametros['perid'], $p->queid );
            if( $p->excluir() ){
                $p->commit();
                $p->ordena($where);
                $msg = array ( 'msg'  => ('Operação realizada com sucesso!'),
                        'erro' => false );
            }else{
                $msg = array ( 'msg'  => ('Operação não realizada!')."\n".('Existem Sub-perguntas associadas a esta pergunta.'),
                        'erro' => true );
            }
        }
    }

    $msg['msg'] = utf8_encode($msg['msg']);
    //die("<script>alert('$msg'); window.location = 'survey.php?modulo=sistema/questionario/arvore&acao=A&sisidFiltro=".$parametros['sisidFiltro']."&queidFiltro=".$parametros['queidFiltro']."';</script>");
    die(json_encode($msg));
}



include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Árvore de Questionários', '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1.min.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/dtree/dtree.js"></script>
	<script>
		function montarA( sisid ){
			if( sisid ){
				window.location = '?modulo=principal/questionario/arvore&acao=A&sisidFiltro=' + sisid;
			}
		}
	</script>
<link
	rel="stylesheet" type="text/css" href="../includes/dtree/dtree.css">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td><font>Filtre o sistema:</font>
		<?
		$sql = "SELECT
					sisid as codigo,
					sisdsc as descricao
				FROM
					seguranca.sistema
				WHERE
					sisstatus = 'A'";
		$db->monta_combo('sisfiltro',$sql,'',"Selecione",'montarA','','','300','S', 'sisfiltro', '', $_GET['sisidFiltro'])
		?>
		</td>
	</tr>
<?php
	if ($_GET['sisidFiltro']):
?>
	<tr>
		<td>
			<div id="bloco" style="overflow: hidden;">
				<p>
					<a href="javascript: arvore.openAll();">Abrir Todos</a>
					&nbsp;|&nbsp; <a href="javascript: arvore.closeAll();">Fechar Todos</a>
				</p>
				<div id="_arvore"></div>
			</div>
		</td>
	</tr>
<?=
    $obArvore->montaArvore( $_GET['sisidFiltro'] );
	endif;
?>
</table>