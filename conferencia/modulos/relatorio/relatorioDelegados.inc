<?php
set_time_limit(120);

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
    $existe_rel = 0;
    $sql = sprintf("SELECT
                        prtid
                    FROM
                        public.parametros_tela
                    WHERE
                        prtdsc = '%s'",
                    $_REQUEST['titulo']);
    $existe_rel = $db->pegaUm( $sql );

    if ($existe_rel > 0){
        $sql = sprintf("UPDATE public.parametros_tela SET
                            prtdsc = '%s',
                            prtobj = '%s',
                            prtpublico = 'FALSE',
                            usucpf = '%s',
                            mnuid = %d
                        WHERE
                            prtid = %d",
                        $_REQUEST['titulo'],
                        addslashes( addslashes( serialize( $_REQUEST ) ) ),
                        $_SESSION['usucpf'],
                        $_SESSION['mnuid'],
                        $existe_rel);
    }else{
        $sql = sprintf("INSERT INTO public.parametros_tela (
                            prtdsc, prtobj, prtpublico, usucpf, mnuid
                        ) VALUES (
                            '%s', '%s', %s, '%s', %d
                        )",
                    $_REQUEST['titulo'],
                    addslashes( addslashes( serialize( $_REQUEST ) ) ),
                    'FALSE',
                    $_SESSION['usucpf'],
                    $_SESSION['mnuid']);
    }
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioDelegados&acao=A';
         </script>");
}

// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
    $sql = sprintf("UPDATE public.parametros_tela SET
                        prtpublico = CASE
                                        WHEN prtpublico = true
                                        THEN false
                                     ELSE
                                        true
                                     END
                    WHERE
                        prtid = %d",
                    $_REQUEST['prtid']);
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioDelegados&acao=A';
         </script>");
}
// FIM - transforma consulta em pública

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
    $sql = sprintf("DELETE FROM
                        public.parametros_tela
                    WHERE
                        prtid = %d",
                    $_REQUEST['prtid']);
    $db->executar( $sql );
    $db->commit();

    die("<script type=\"text/javascript\">
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=relatorio/relatorioDelegados&acao=A';
         </script>");
}
// FIM - remove consulta

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ){
    unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
    if ( $_REQUEST['prtid'] ){
        $sql = sprintf( "SELECT
                            prtobj
                         FROM
                            public.parametros_tela
                         WHERE
                            prtid = " . $_REQUEST['prtid'] );
        $itens = $db->pegaUm( $sql );
        $dados = unserialize( stripslashes( stripslashes( $itens ) ) );
        $_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
        unset( $_REQUEST['salvar'] );
    }
    include "relatorioDelegadosResult.inc";
    exit;

}

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
    $sql = sprintf( "SELECT
                        prtobj
                     FROM
                        public.parametros_tela
                     WHERE
                        prtid = ".$_REQUEST['prtid'] );
    $itens = $db->pegaUm( $sql );

    $dados = unserialize( stripslashes( stripslashes( $itens ) ) );
    extract( $dados );
    $_REQUEST = $dados;
    unset( $_REQUEST['form'] );
    unset( $_REQUEST['pesquisa'] );
    $titulo = $_REQUEST['titulo'];

    $agrupador2 = array();

    if ( $_REQUEST['agrupador'] ){
        foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
            array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
        }
    }
}

if ( isset( $_REQUEST['pesquisa'] ) || isset( $_REQUEST['tipoRelatorio'] ) ){
    include "relatorioDelegadosResult.inc";
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório Delegados";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<!-- CARREGANDO DADOS AJAX -->
<div id="loader-container" style="display:none;">
    <div id="loader">
        <img src="../imagens/wait.gif" border="0" align="middle">
        <span>Aguarde! Carregando Dados...</span>
    </div>
</div>

<form action="" method="post" name="filtro">
    <input type="hidden" name="form" value="1"/>
    <input type="hidden" name="pesquisa" value="1"/>
    <input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
    <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
    <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
    <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita">Título</td>
            <td>
                <?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Agrupadores</td>
            <td>
                <?php

                    // Início dos agrupadores
                    $agrupador = new Agrupador('filtro','');

                    $agrupador->width = 345;

                    // Dados padrão de destino (nulo)
                    $destino = isset( $agrupador2 ) ? $agrupador2 : array();

                    // Dados padrão de origem
                    $origem = array(
                       array(
                            'codigo'    => 'mconome',
                            'descricao' => 'Edição da Conferência'
                        ),
                        array(
                            'codigo'    => 'deltipodelegado',
                            'descricao' => 'Tipo de Delegado'
                        ),
                        array(
                            'codigo'    => 'delcpf',
                            'descricao' => 'CPF'
                        ),
                        array(
                            'codigo'    => 'tpcnome',
                            'descricao' => 'Tipo da Conferência'
                        ),
                        array(
                            'codigo'    => 'deltiporepresentante',
                            'descricao' => 'Tipo de Representante'
                        ),
                        array(
                            'codigo'    => 'sgcdescricao',
                            'descricao' => 'Segmento cultural'
                        ),
                        array(
                            'codigo'    => 'pcntitulo',
                            'descricao' => 'Título da Conferência'
                        ),
                        array(
                            'codigo'    => 'delid',
                            'descricao' => 'Identificador Delegado'
                        ),
                        array(
                            'codigo'    => 'estuf',
                            'descricao' => 'Estado(UF)'
                        )
                    );

                    // exibe agrupador
                    $agrupador->setOrigem( 'naoAgrupador', null, $origem );
                    $agrupador->setDestino( 'agrupador', null, $destino );
                    $agrupador->exibir();
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Colunas</td>
            <td>
                <?php
                    unset($agrupador, $destino, $origem);
                    // Início das colunas
                    $agrupador = new Agrupador('filtro','');

                    $agrupador->width = 345;

                    // Dados padrão de destino (nulo)
                    $destino = isset( $agrupador2 ) ? $agrupador2 : array();

                    // Dados padrão de origem
                    $origem = array(
                        array(
                            'codigo'    => 'mconome',
                            'descricao' => 'Edição da Conferência'
                        ),
                        array(
                            'codigo'    => 'deltipodelegado',
                            'descricao' => 'Tipo de Delegado'
                        ),
                        array(
                            'codigo'    => 'delcpf',
                            'descricao' => 'CPF'
                        ),
                        array(
                            'codigo'    => 'tpcnome',
                            'descricao' => 'Tipo da Conferência'
                        ),
                        array(
                            'codigo'    => 'deltiporepresentante',
                            'descricao' => 'Tipo de Representante'
                        ),
                        array(
                            'codigo'    => 'sgcdescricao',
                            'descricao' => 'Segmento cultural'
                        ),
                        array(
                            'codigo'    => 'pcntitulo',
                            'descricao' => 'Título da Conferência'
                        ),
                        array(
                            'codigo'    => 'delnome',
                            'descricao' => 'Nome completo'
                        ),
                        array(
                            'codigo'    => 'delnomecracha',
                            'descricao' => 'Nome crachá'
                        ),
                        array(
                            'codigo'    => 'delemail',
                            'descricao' => 'E-mail'
                        ),
                        array(
                            'codigo'    => 'delnumtelefone',
                            'descricao' => 'Telefone'
                        ),
                        array(
                            'codigo'    => 'delnumtelefone2',
                            'descricao' => 'Outro Telefone'
                        ),
                        array(
                            'codigo'    => 'esddsc',
                            'descricao' => 'Status da Conferência'
                        ),
                        array(
                        		'codigo'    => 'defnomecracha',
                        		'descricao' => 'Nome crachá (Ficha de Inscrição de Delegado)'
                        ),
                        array(
                        		'codigo'    => 'defnumrg',
                        		'descricao' => 'RG'
                        ),
                        array(
                        		'codigo'    => 'defemail',
                        		'descricao' => 'Email'
                        ),
                        array(
                        		'codigo'    => 'defnumtelefone',
                        		'descricao' => 'Telefone (Ficha de Inscrição de Delegado)'
                        ),
                        array(
                        		'codigo'    => 'defnumtelefone2',
                        		'descricao' => 'Outro telefone (Ficha de Inscrição de Delegado)'
                        ),
                        array(
                        		'codigo'    => 'endcep',
                        		'descricao' => 'CEP'
                        ),
                        array(
                        		'codigo'    => 'endlog',
                        		'descricao' => 'Logradouro'
                        ),
                        array(
                        		'codigo'    => 'endnum',
                        		'descricao' => 'Número'
                        ),
                        array(
                            'codigo'    => 'estuf',
                            'descricao' => 'Estado(UF)'
                        ),
                        array(
                            'codigo'    => 'mundescricao',
                            'descricao' => 'Município'
                        ),
                        array(
                            'codigo'    => 'sgcdescricaoficha',
                            'descricao' => 'Segmento cultural (Ficha de Inscrição de Delegado)'
                        ),
                        array(
                            'codigo'    => 'defsiniic',
                            'descricao' => 'Número SNIIC'
                        ),
                        array(
                            'codigo'    => 'defconf1',
                            'descricao' => 'Participou da I Conferência Nacional de Cultura, realizada em 2005?'
                        ),
                        array(
                            'codigo'    => 'defconf2',
                            'descricao' => 'Participou da II Conferência Nacional de Cultura, realizada em 2009?'
                        ),
                        array(
                            'codigo'    => 'opcao1',
                            'descricao' => 'Opção 1'
                        ),
                        array(
                            'codigo'    => 'opcao2',
                            'descricao' => 'Opção 2'
                        ),
                        array(
                            'codigo'    => 'opcao3',
                            'descricao' => 'Opção 3'
                        ),
                        array(
                            'codigo'    => 'opcao4',
                            'descricao' => 'Opção 4'
                        ),
                        array(
                            'codigo'    => 'deftecnoassistiva',
                            'descricao' => 'Necessita de tecnologia assistiva?'
                        ),
                        array(
                            'codigo'    => 'deftransporte',
                            'descricao' => 'Necessita de transporte acessível?'
                        ),
                        array(
                            'codigo'    => 'defhospedagem',
                            'descricao' => 'Necessita de hospedagem disponibilizada pelo Ministério da Cultura?'
                        ),
                        array(
                            'codigo'    => 'defacomodespec',
                            'descricao' => 'Necessita de acomodação especial?'
                        ),
                        array(
                            'codigo'    => 'defnomeemergencia',
                            'descricao' => 'Nome contato de emergência'
                        ),
                        array(
                            'codigo'    => 'defnumtelemergencia',
                            'descricao' => 'Telefone contato de emergência'
                        ),
                        array(
                            'codigo'    => 'defnumtelemergencia2',
                            'descricao' => 'Outro telefone contato de emergência'
                        ),
                        array(
                            'codigo'    => 'defdeclaracao',
                            'descricao' => 'Declaração de aptidão'
                        )

                    );

                    // exibe agrupador
                    $agrupador->setOrigem( 'naoColuna', null, $origem );
                    $agrupador->setDestino( 'coluna', null, $destino );
                    $agrupador->exibir();
                ?>
            </td>
        </tr>
        </table>

        <!-- OUTROS FILTROS -->

        <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
            <tr>
                <td onclick="javascript:onOffBloco( 'outros' );">
                    <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                    Relatórios Gerenciais
                    <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
                </td>
            </tr>
        </table>

        <div id="outros_div_filtros_off"></div>

        <div id="outros_div_filtros_on" style="display:none;">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
                        <?php

                            if( $db->testa_superuser() ){
                                $bt_publicar = "<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;";
                            }

                            $sql = sprintf("SELECT
                                                CASE WHEN prtpublico = true AND usucpf = '%s'
                                                    THEN '{$bt_publicar}<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' else '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;<img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' end as acao, '' || prtdsc || '' as descricao
                                            FROM
                                                public.parametros_tela
                                            WHERE
                                                mnuid = %d AND
                                                prtpublico = TRUE",
                                $_SESSION['usucpf'],
                                $_SESSION['mnuid'],
                                $_SESSION['usucpf']
                            );

                            $cabecalho = array('Ação', 'Nome');
                        ?>
                        <td><?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
                        </td>
                    </tr>
            </table>
        </div>
        <script language="javascript">  //alert( document.formulario.agrupador_combo.value );   </script>

        <!-- FIM OUTROS FILTROS -->



        <!-- MINHAS CONSULTAS -->
        <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
            <tr>
                <td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
                    <!-- -->
                    <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                    Minhas Consultas
                    <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
                </td>
            </tr>
        </table>
        <div id="minhasconsultas_div_filtros_off">
        </div>
        <div id="minhasconsultas_div_filtros_on" style="display:none;">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                        <?php

                            $sql = sprintf(
                                "SELECT
                                    CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                                 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                                       <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                    END as acao,
                                    '' || prtdsc || '' as descricao
                                 FROM
                                    public.parametros_tela
                                 WHERE
                                    mnuid = %d AND usucpf = '%s'",
                                $_SESSION['mnuid'],
                                $_SESSION['usucpf']
                            );

                            $cabecalho = array('Ação', 'Nome');

                        ?>
                        <td>
                            <?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
                        </td>
                    </tr>
            </table>
        </div>
        <!-- FIM MINHAS CONSULTAS -->

        <!-- MEUS FILTROS -->
        <table class="tabela" style=" border-bottom:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <?php
            /********************************************************************
             * Edição da Conferência */
            $stSql = "  SELECT DISTINCT
                            mcoid AS codigo,
                            mconome AS descricao
                        FROM
                            conferencia.momentoconferencia
                        WHERE
                            mcostatus = 'A'
                        ORDER BY
                            2";

            mostrarComboPopup( 'Edição da Conferência', 'mcoid',  $stSql, $stSqlCarregados, 'Selecione a(s) edição(ões) da conferência' );

            /********************************************************************
             * Tipo de Conferência */
            $stSql = "  SELECT DISTINCT
                            tpcid AS codigo,
                            tpcsigla || ' - ' || tpcnome AS descricao
                        FROM
                            conferencia.tipoconferencia
                        WHERE
                            tpcstatus = 'A'
                        ORDER BY
                            2";

            mostrarComboPopup( 'Tipo de Conferência', 'tpcid',  $stSql, $stSqlCarregados, 'Selecione o(s) tipo(s) de conferência' );

            /********************************************************************
             * Segmento cultural */
            $stSql = "  SELECT DISTINCT
                            sgcid AS codigo,
                            sgcdescricao AS descricao
                        FROM
                            conferencia.segmentocultural
                        WHERE
                            sgcstatus = 'A'
                        ORDER BY
                            2";

            mostrarComboPopup( 'Segmento cultural', 'sgcid',  $stSql, $stSqlCarregados, 'Selecione o(s) segmento(s) Cultural(ais)' );

            /********************************************************************
             * Status Conferência */
            $stSql = "  SELECT DISTINCT
                            esdid AS codigo,
                            esddsc AS descricao
                        FROM
                            workflow.estadodocumento
                        WHERE
                            esdstatus = 'A' AND
                            tpdid = ".WF_PRECADASTRO_CONFERENCIA."
                        ORDER BY
                            2";

            mostrarComboPopup( 'Status da Conferência', 'esdid',  $stSql, $stSqlCarregados, 'Selecione os Status das Conferências' );


            ?>

            <tr>
                <td class="SubTituloDireita">Tipo de Delegado</td>
                <td>
                    <input type="radio" name="deltipodelegado" value="T" id="deltipodelegadoT"><label for="deltipodelegadoT">Titular</label> &nbsp;
                    <input type="radio" name="deltipodelegado" value="S" id="deltipodelegadoS"><label for="deltipodelegadoS">Suplente</label> &nbsp;
                    <input type="radio" name="deltipodelegado" value="A" id="deltipodelegadoA" checked="checked"><label for="deltipodelegadoA">Todos</label>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Tipo de Representante</td>
                <td>
                    <input type="radio" name="deltiporepresentante" value="P" id="deltiporepresentanteP" ><label for="deltiporepresentanteP">Poder Público</label> &nbsp;
                    <input type="radio" name="deltiporepresentante" value="C" id="deltiporepresentanteC" ><label for="deltiporepresentanteC">Sociedade Civil</label> &nbsp;
                    <input type="radio" name="deltiporepresentante" value="A" id="deltiporepresentanteA" checked="checked"><label for="deltiporepresentanteA">Todos</label>
                </td>
            </tr>


            <tr>
                <td class="SubTituloDireita">
                    Adicionar dados complementares?
                </td>
                <td>
                    <input type="radio" name="dadoscomplementares" id="dadoscomplementares-t" <?php echo ($dadoscomplementares == 't' || empty($dadoscomplementares) ) ? ' checked="checked" ' : "" ?> value="t" /> <label for="dadoscomplementares-t" style="cursor:pointer;"> Sim</label>
					<input type="radio" name="dadoscomplementares" id="dadoscomplementares-f" <?php echo $dadoscomplementares == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="dadoscomplementares-f" style="cursor:pointer;"> Não</label>
                </td>
            </tr>
        </table>

        <table class="tabela" style="border-top:none;" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" >
            <tr>
                <td bgcolor="#CCCCCC" width="195"></td>
                <td bgcolor="#CCCCCC">
                    <input type="button" value="Visualizar" onclick="exiberelatorioDelegados('exibir');" style="cursor: pointer;"/>
                    <input type="button" value="Visualizar XLS" onclick="exiberelatorioDelegados('xls');" style="cursor: pointer;"/>
                    <input type="button" value="Salvar Consulta" onclick="exiberelatorioDelegados('salvar');" style="cursor: pointer;"/>

                </td>
            </tr>
        </table>
</form>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

    function formataNumero(elemento){
        var valor = elemento.value;
        var valorFormatado = new Number(valor.replace(",",".") );

        if(valorFormatado > 100){
            elemento.value = '100,00';
        }
    }

    function exiberelatorioDelegados(tipo){

        var formulario = document.filtro;
        var agrupador  = document.getElementById( 'agrupador' );
        var coluna     = document.getElementById( 'coluna' );

        // Tipo de relatorio
        formulario.pesquisa.value='1';

        prepara_formulario();
        selectAllOptions( formulario.agrupador );
        selectAllOptions( formulario.coluna );

        if ( tipo == 'relatorio' ){

            formulario.action = '?modulo=relatorio/relatorioDelegados&acao=A';
            window.open( '', 'relatorio', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            formulario.target = 'relatorio';

        }else {
            if ( tipo == 'salvar' ){

                if ( formulario.titulo.value == '' ) {
                    alert( 'É necessário informar o título do relatório!' );
                    formulario.titulo.focus();
                    return false;
                }

                if ( !agrupador.options.length ){
                    alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
                    agrupador.focus();
                    return false;
                }

                if ( !coluna.options.length ){
                    alert( 'Favor selecionar ao menos um item de coluna para o resultado!' );
                    coluna.focus();
                    return false;
                }

                var nomesExistentes = new Array();
                <?php
                    $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
                    $nomesExistentes = $db->carregar( $sqlNomesConsulta );
                    if ( $nomesExistentes ){
                        foreach ( $nomesExistentes as $linhaNome )
                        {
                            print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
                        }
                    }
                ?>
                var confirma = true;
                var i, j = nomesExistentes.length;
                for ( i = 0; i < j; i++ ){
                    if ( nomesExistentes[i] == formulario.titulo.value ){
                        confirma = confirm( 'Deseja alterar a consulta já existente?' );
                        break;
                    }
                }
                if ( !confirma ){
                    return;
                }
                formulario.action = '?modulo=relatorio/relatorioDelegados&acao=A&salvar=1';
                formulario.target = '_self';

            }else if( tipo == 'exibir' || tipo == 'xls' || tipo == 'mapa'){
                if(tipo != 'mapa'){
                    if ( !agrupador.options.length ){
                        alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
                        agrupador.focus();
                        return false;
                    }

                    if ( !coluna.options.length ){
                        alert( 'Favor selecionar ao menos um item de coluna para o resultado!' );
                        coluna.focus();
                        return false;
                    }

                    selectAllOptions( agrupador );
                    selectAllOptions( coluna );
                }

                if(tipo == 'xls'){
                	formulario.action = '?modulo=relatorio/relatorioDelegados&acao=A&tipoRelatorio=xls';
                }else if(tipo == 'mapa'){
                	formulario.action = '?modulo=relatorio/relatorioDelegados&acao=A&tipoRelatorio=mapa';
                }else{
                	formulario.action = '?modulo=relatorio/relatorioDelegados&acao=A';
                }
                formulario.target = 'resultadoFtGeral';
                var janela = window.open( '', 'resultadoFtGeral', 'width=800,height=550,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
                janela.focus();
            }
        }

        formulario.submit();

    }

    function tornar_publico( prtid ){
        document.filtro.publico.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

    function excluir_relatorio( prtid ){
        document.filtro.excluir.value = '1';
        document.filtro.prtid.value = prtid;
        document.filtro.target = '_self';
        document.filtro.submit();
    }

//    function carregar_consulta( prtid ){
//        document.filtro.carregar.value = '1';
//        document.filtro.prtid.value = prtid;
//        document.filtro.target = '_self';
//        document.filtro.submit();
//    }

    function carregar_relatorio( prtid ){
        document.filtro.prtid.value = prtid;
        exiberelatorioDelegados( 'relatorio' );
    }

    /* Função para substituir todos */
    function replaceAll(str, de, para){
        var pos = str.indexOf(de);
        while (pos > -1){
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }
    /* Função para adicionar linha nas tabelas */

    /**
     * Alterar visibilidade de um bloco.
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco( bloco )
    {
        var div_on = document.getElementById( bloco + '_div_filtros_on' );
        var div_off = document.getElementById( bloco + '_div_filtros_off' );
        var img = document.getElementById( bloco + '_img' );
        var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo( campo )
    {
        var div_on = document.getElementById( campo + '_campo_on' );
        var div_off = document.getElementById( campo + '_campo_off' );
        var input = document.getElementById( campo + '_campo_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }
</script>
