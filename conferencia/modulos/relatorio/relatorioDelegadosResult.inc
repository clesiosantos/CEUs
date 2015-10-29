<?php
set_time_limit(120);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

if( $_REQUEST['dadoscomplementares'] != 't' ){
	$rel->setMonstrarCabecalho(false);
	$monstrarCabecalho = false;
}else{
	$monstrarCabecalho = true;
}

// monta o sql, agrupador e coluna do relatório
$sql       = monta_sql();
$agrupador = monta_agp();
$coluna    = monta_col();
$dados     = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(true);
$rel->setEspandir(true);
$rel->setTolizadorLinha(true);
$rel->setTotalizador(true);

// Gera o XLS do relatório
if ( $_REQUEST['tipoRelatorio'] == 'xls' ){
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXlsEmColunas();
    die();
}

?>
<!DOCTYPE html>
<html>
    <head>
        <title> Conferência Nacional </title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    </head>
    <body>
        <center>
            <!--  Cabeçalho Brasão -->
            <?php echo monta_cabecalho_relatorio( '100',$monstrarCabecalho ); ?>
        </center>

        <!--  Monta o Relatório -->
        <? echo $rel->getRelatorio(); ?>

    </body>
</html>
<?php
function monta_sql(){
    $where = array();
    global $db;

    extract($_REQUEST);

    // Edição da Conferência
    if( $mcoid[0]  && $mcoid_campo_flag ){
        array_push($where, " mco.mcoid " . (!$mcoid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $mcoid ) . "') ");
    }

    // Tipo de Conferência
    if( $tpcid[0]  && $tpcid_campo_flag ){
        array_push($where, " tpc.tpcid " . (!$tpcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpcid ) . "') ");
    }

    // Segmento cultural
    if( $sgcid[0]  && $sgcid_campo_flag ){
        array_push($where, " sgc.sgcid " . (!$sgcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $sgcid ) . "') ");
    }

    // Status da Conferência
    if( $esdid[0]  && $esdid_campo_flag ){
        array_push($where, " doc.esdid " . (!$esdid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $esdid ) . "') ");
    }

    // Tipo de Delegado
    if( $deltipodelegado != 'A' ){
        array_push($where, " del.deltipodelegado = '{$deltipodelegado}' ");
    }

    // Tipo de Representante
    if( $deltiporepresentante != 'A' ){
        array_push($where, " del.deltiporepresentante = '{$deltiporepresentante}' ");
    }

    array_push($where, " pcn.pcnstatus = 'A'");

    $sql = "SELECT
                mco.mconome,
                CASE
                    WHEN ( del.delcaregoria = 'D' AND del.deltipodelegado = 'T') THEN 'Delegado Eleito Titular'
                    WHEN ( del.delcaregoria = 'D' AND del.deltipodelegado = 'S') THEN 'Delegado Eleito Suplente'
                    WHEN ( del.delcaregoria = 'N' AND del.deltipodelegado = 'T') THEN 'Delegado Nato Titular'
                    WHEN ( del.delcaregoria = 'N' AND del.deltipodelegado = 'S') THEN 'Delegado Nato Suplente'
                    WHEN ( del.delcaregoria = 'C' ) THEN 'Coordenadores Responsáveis pela Delegação'
                END AS deltipodelegado,
                del.delcpf,
                tpc.tpcnome,
                CASE
                    WHEN del.deltiporepresentante = 'C' THEN 'Sociedade Civil'
                    WHEN del.deltiporepresentante = 'P' THEN 'Poder Público'
                END as deltiporepresentante,
                sgc.sgcdescricao,
                pcn.pcntitulo,
                del.delnome,
                del.delnomecracha,
                del.delemail,
                del.delnumtelefone,
                del.delnumtelefone2,
                esd.esddsc,
                del.delid,
        		est.estuf as endestuf,
				dadosdelegado.defnomecracha,
				dadosdelegado.defnumrg,
				dadosdelegado.defemail,
				dadosdelegado.defnumtelefone,
				dadosdelegado.defnumtelefone2,
				dadosdelegado.endcep,
				dadosdelegado.endlog,
				dadosdelegado.endnum,
				dadosdelegado.estuf,
				dadosdelegado.mundescricao,
				dadosdelegado.sgcdescricaoficha,
				dadosdelegado.defsiniic,
				dadosdelegado.defconf1,
				dadosdelegado.defconf2,
				dadosdelegado.opcao1,
				dadosdelegado.opcao2,
				dadosdelegado.opcao3,
				dadosdelegado.opcao4,
				dadosdelegado.deftecnoassistiva,
				dadosdelegado.deftransporte,
				dadosdelegado.defhospedagem,
				dadosdelegado.defacomodespec,
				dadosdelegado.defnomeemergencia,
				dadosdelegado.defnumtelemergencia,
				dadosdelegado.defnumtelemergencia2,
				dadosdelegado.defdeclaracao
            FROM
                conferencia.preconferencia pcn
            JOIN
                conferencia.delegadoconferencia delconf ON pcn.pcnid = delconf.pcnid
            LEFT JOIN
                workflow.documento doc ON pcn.docid = doc.docid
            LEFT JOIN
                workflow.estadodocumento esd ON esd.esdid = doc.esdid
            JOIN
                conferencia.delegado del ON del.delid = delconf.delid
            LEFT JOIN
                conferencia.conferencia cnf on pcn.pcnid = cnf.pcnid
            INNER JOIN
                conferencia.tipoconferencia tpc on pcn.tpcid = tpc.tpcid
            INNER JOIN
                conferencia.momentoconferencia mco ON mco.mcoid = pcn.mcoid
            LEFT JOIN
                conferencia.segmentocultural sgc on sgc.sgcid = del.sgcid
            LEFT JOIN
                conferencia.endereco ende ON ende.endid = pcn.endid
            LEFT JOIN
		         territorios.estado est ON est.estuf = ende.estuf     
        	LEFT JOIN
        	(SELECT 
				def.defnome,
				def.defnomecracha,
				replace(to_char(def.defcpf::numeric, '000:000:000-00'), ':', '.') as defcpfformatado,
				def.defcpf,
				def.defnumrg,
				def.defemail,
				def.defnumtelefone,
				def.defnumtelefone2,
				ende.endcep,
				ende.endlog,
				ende.endnum,
				ende.estuf,
				mun.mundescricao,		
				sgc.sgcdescricao as sgcdescricaoficha,
				def.defsiniic,
				CASE
					WHEN defconf1 IS TRUE THEN 'Sim'
					WHEN defconf1 IS FALSE THEN 'Não'
				END as defconf1,	
			        CASE
					WHEN defconf2 IS TRUE THEN 'Sim'
					WHEN defconf2 IS FALSE THEN 'Não'
				END as defconf2,      
				( SELECT 
					set2.setnome 
			          FROM 
			                conferencia.delegadofichasubeixo dfs2 
			          JOIN
					conferencia.subeixotematico set2 ON set2.setid = dfs2.setid
			          WHERE 
			                dfs2.defid = def.defid AND 
			                dfs2.dfsordem = 1 AND 
			                dfs2.dfsstatus = 'A' ) as opcao1,
				( SELECT 
					set2.setnome 
			          FROM 
			                conferencia.delegadofichasubeixo dfs2 
			          JOIN
					conferencia.subeixotematico set2 ON set2.setid = dfs2.setid
			          WHERE 
			                dfs2.defid = def.defid AND 
			                dfs2.dfsordem = 2 AND 
			                dfs2.dfsstatus = 'A' ) as opcao2,
				( SELECT 
					set2.setnome 
			          FROM 
			                conferencia.delegadofichasubeixo dfs2 
			          JOIN
					conferencia.subeixotematico set2 ON set2.setid = dfs2.setid
			          WHERE 
			                dfs2.defid = def.defid AND 
			                dfs2.dfsordem = 3 AND 
			                dfs2.dfsstatus = 'A' ) as opcao3,
				( SELECT 
					set2.setnome 
			          FROM 
			                conferencia.delegadofichasubeixo dfs2 
			          JOIN
					conferencia.subeixotematico set2 ON set2.setid = dfs2.setid
			          WHERE 
			                dfs2.defid = def.defid AND 
			                dfs2.dfsordem = 4 AND 
			                dfs2.dfsstatus = 'A' ) as opcao4,
				CASE
					WHEN deftecnoassistiva IS TRUE THEN 'Sim'
					WHEN deftecnoassistiva IS FALSE THEN 'Não'
				END || ' - '||COALESCE(def.defdsctipotecnoassistiva, 'N/A') as deftecnoassistiva,       
				CASE
					WHEN deftransporte IS TRUE THEN 'Sim'
					WHEN deftransporte IS FALSE THEN 'Não'
				END as deftransporte,			
				CASE
					WHEN defhospedagem IS TRUE THEN 'Sim'
					WHEN defhospedagem IS FALSE THEN 'Não'
				END ||       
				CASE
					WHEN deftipohospedagem = 'M' THEN ' - Hospedagem masculina'
					WHEN deftipohospedagem = 'F' THEN ' - Hospedagem feminina'
				        ELSE ' '
				END as defhospedagem,
			
				CASE
					WHEN defacomodespec IS TRUE THEN 'Sim'
					WHEN defacomodespec IS FALSE THEN 'Não'
					ELSE 'Não se Aplica'
				END ||       
				CASE
					WHEN defmotivoacomodacao = 'D' THEN ' - Possuo deficiência física e necessito de quarto adaptado'
					WHEN defmotivoacomodacao = 'A' THEN ' - Viajarei com acompanhante porque preciso ser assistido. CPF do acompanhante: ' || COALESCE(def.defdscacompanhante,'N/A')
					WHEN defmotivoacomodacao = 'S' THEN ' - Possuo problema de saúde: ' || COALESCE(def.defproblemasaude,'N/A') || CASE WHEN defcuidadoespecifico IS TRUE THEN '. Necessita de algum cuidado específico: ' || COALESCE(def.defdsccuidadoespecifico,'N/A') ELSE ' ' END
					WHEN defmotivoacomodacao = 'G' THEN ' - Possuo identidade de gênero distinta do meu sexo morfológico'
					WHEN defmotivoacomodacao = 'O' THEN ' - Outros: ' || COALESCE(def.defoutromotivo,'N/A')
				        ELSE ' '
				END as defacomodespec,
				def.defnomeemergencia,
				def.defnumtelemergencia,
				def.defnumtelemergencia2,
				CASE
					WHEN defdeclaracao IS TRUE THEN 'Sim'
					WHEN defdeclaracao IS FALSE THEN 'Não'
				END as defdeclaracao
			FROM 
				conferencia.delegadoficha def
			LEFT JOIN
				conferencia.endereco ende ON ende.endid = def.endid
			LEFT JOIN
				territorios.municipio mun ON mun.muncod = ende.muncod
			LEFT JOIN
				conferencia.segmentocultural sgc ON sgc.sgcid = def.sgcid
			WHERE 
				defstatus = 'A') dadosdelegado ON dadosdelegado.defcpf = del.delcpf
            WHERE" .
            (count($where) ? implode(' AND ', $where) : "" );
    
    return $sql;
}

function monta_agp(){
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    $agp = array(
                "agrupador"      => array(),
                "agrupadoColuna" => array("mconome",
                                          "deltipodelegado",
                                          "delcpf",
                                          "tpcnome",
                                          "deltiporepresentante",
                                          "sgcdescricao",
                                          "pcntitulo",
                                          "delnome",
                                          "delnomecracha",
                                          "delemail",
                                          "delnumtelefone",
                                          "delnumtelefone2",
                                          "esddsc",
                                          "estuf",
				                		  "defnomecracha",
				                		  "defnumrg",
				                		  "defemail",
				                		  "defnumtelefone",
				                		  "defnumtelefone2",
				                		  "endcep",
				                		  "endlog",
				                		  "endnum",
				                		  "estuf",
				                		  "mundescricao",
				                		  "sgcdescricaoficha",
				                		  "defsiniic",
				                		  "defconf1",
				                		  "defconf2",
				                		  "opcao1",
				                		  "opcao2",
				                		  "opcao3",
				                		  "opcao4",
				                		  "deftecnoassistiva",
				                		  "deftransporte",
				                		  "defhospedagem",
				                		  "defacomodespec",
				                		  "defnomeemergencia",
				                		  "defnumtelemergencia",
				                		  "defnumtelemergencia2",
				                		  "defdeclaracao")
                                        );

    foreach ( $agrupador as $val ){
        switch( $val ){
            case "mconome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "mconome",
                                                    "label" => "Edição da Conferência")
                                                    );
            break;
            case "delid":
                array_push($agp['agrupador'], array(
                                                    "campo" => "delid",
                                                    "label" => "Identificador Delegado")
                                                    );
            break;
            case "deltipodelegado":
                array_push($agp['agrupador'], array(
                                                    "campo" => "deltipodelegado",
                                                    "label" => "Tipo de Delegado")
                                                    );
            break;
            case "delcpf":
                array_push($agp['agrupador'], array(
                                                    "campo" => "delcpf",
                                                    "label" => "CPF")
                                                    );
            break;
            case "tpcnome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "tpcnome",
                                                    "label" => "Tipo da Conferência")
                                                    );
            break;
            case "deltiporepresentante":
                array_push($agp['agrupador'], array(
                                                    "campo" => "deltiporepresentante",
                                                    "label" => "Tipo de Representante")
                                                    );
            break;
            case "sgcdescricao":
                array_push($agp['agrupador'], array(
                                                    "campo" => "sgcdescricao",
                                                    "label" => "Segmento cultural")
                                                    );
            break;
            case "pcntitulo":
                array_push($agp['agrupador'], array(
                                                    "campo" => "pcntitulo",
                                                    "label" => "Título da Conferência")
                                                    );
            break;
            case "estuf":
                array_push($agp['agrupador'], array(
                                                    "campo" => "estuf",
                                                    "label" => "Estado(UF)")
                                                    );
            break;
        }
    }

    return $agp;
}

function monta_col(){
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    $coluna = ($coluna ? $coluna : array());

    $col = array();

    foreach ( $coluna as $val ){
        switch( $val ){
        	case "mconome":
                array_push($col, array("campo" => "mconome",
                                       "label" => "Edição da Conferência",
                                       "type"  => "string"));
            break;
        	case "tpcnome":
                array_push($col, array("campo" => "tpcnome",
                                       "label" => "Tipo da Conferência",
                                       "type"  => "string"));
            break;
        	case "deltipodelegado":
                array_push($col, array("campo" => "deltipodelegado",
                                       "label" => "Tipo de Delegado",
                                       "type"  => "string"));
            break;
        	case "delcpf":
                array_push($col, array("campo" => "delcpf",
                                       "label" => "CPF",
                                       "type"  => "string"));
            break;
        	case "deltiporepresentante":
                array_push($col, array("campo" => "deltiporepresentante",
                                       "label" => "Tipo de Representante",
                                       "type"  => "string"));
            break;
        	case "sgcdescricao":
                array_push($col, array("campo" => "sgcdescricao",
                                       "label" => "Segmento cultural",
                                       "type"  => "string"));
            break;
        	case "pcntitulo":
                array_push($col, array("campo" => "pcntitulo",
                                       "label" => "Título da Conferência",
                                       "type"  => "string"));
            break;
        	case "delnome":
                array_push($col, array("campo" => "delnome",
                                       "label" => "Nome completo",
                                       "type"  => "string"));
            break;
        	case "delnomecracha":
                array_push($col, array("campo" => "delnomecracha",
                                       "label" => "Nome crachá",
                                       "type"  => "string"));
            break;
        	case "delemail":
                array_push($col, array("campo" => "delemail",
                                       "label" => "E-mail",
                                       "type"  => "string"));
            break;
        	case "delnumtelefone":
                array_push($col, array("campo" => "delnumtelefone",
                                       "label" => "Telefone",
                                       "type"  => "string"));
            break;
        	case "delnumtelefone2":
                array_push($col, array("campo" => "delnumtelefone2",
                                       "label" => "Outro Telefone",
                                       "type"  => "string"));
            break;
        	case "esddsc":
                array_push($col, array("campo" => "esddsc",
                                       "label" => "Status da Conferência",
                                       "type"  => "string"));
            break;
        	case "estuf":
                array_push($col, array("campo" => "estuf",
                                       "label" => "Estado(UF)",
                                       "type"  => "string"));
            break;
            
            
            
        	case "defnomecracha":
                array_push($col, array("campo" => "defnomecracha",
                                       "label" => "Nome crachá (Ficha de Inscrição de Delegado)",
                                       "type"  => "string"));
            break;
        	case "defnumrg":
                array_push($col, array("campo" => "defnumrg",
                                       "label" => "RG",
                                       "type"  => "string"));
            break;
        	case "defemail":
                array_push($col, array("campo" => "defemail",
                                       "label" => "Email",
                                       "type"  => "string"));
            break;
        	case "defnumtelefone":
                array_push($col, array("campo" => "defnumtelefone",
                                       "label" => "Telefone (Ficha de Inscrição de Delegado)",
                                       "type"  => "string"));
            break;
        	case "defnumtelefone2":
                array_push($col, array("campo" => "defnumtelefone2",
                                       "label" => "Outro telefone (Ficha de Inscrição de Delegado)",
                                       "type"  => "string"));
            break;
        	case "endcep":
                array_push($col, array("campo" => "endcep",
                                       "label" => "CEP",
                                       "type"  => "string"));
            break;
        	case "endlog":
                array_push($col, array("campo" => "endlog",
                                       "label" => "Logradouro",
                                       "type"  => "string"));
            break;
        	case "endnum":
                array_push($col, array("campo" => "endnum",
                                       "label" => "Número",
                                       "type"  => "string"));
            break;
        	case "estuf":
                array_push($col, array("campo" => "estuf",
                                       "label" => "Estado(UF)",
                                       "type"  => "string"));
            break;
        	case "mundescricao":
                array_push($col, array("campo" => "mundescricao",
                                       "label" => "Município",
                                       "type"  => "string"));
            break;
        	case "sgcdescricaoficha":
                array_push($col, array("campo" => "sgcdescricaoficha",
                                       "label" => "Segmento cultural (Ficha de Inscrição de Delegado)",
                                       "type"  => "string"));
            break;
        	case "defsiniic":
                array_push($col, array("campo" => "defsiniic",
                                       "label" => "Número SNIIC",
                                       "type"  => "string"));
            break;
        	case "defconf1":
                array_push($col, array("campo" => "defconf1",
                                       "label" => "Participou da I Conferência Nacional de Cultura, realizada em 2005?",
                                       "type"  => "string"));
            break;
        	case "defconf2":
                array_push($col, array("campo" => "defconf2",
                                       "label" => "Participou da II Conferência Nacional de Cultura, realizada em 2009?",
                                       "type"  => "string"));
            break;
        	case "opcao1":
                array_push($col, array("campo" => "opcao1",
                                       "label" => "Opção 1",
                                       "type"  => "string"));
            break;
        	case "opcao2":
                array_push($col, array("campo" => "opcao2",
                                       "label" => "Opção 2",
                                       "type"  => "string"));
            break;
        	case "opcao3":
                array_push($col, array("campo" => "opcao3",
                                       "label" => "Opção 3",
                                       "type"  => "string"));
            break;
        	case "opcao4":
                array_push($col, array("campo" => "opcao4",
                                       "label" => "Opção 4",
                                       "type"  => "string"));
            break;
        	case "deftecnoassistiva":
                array_push($col, array("campo" => "deftecnoassistiva",
                                       "label" => "Necessita de tecnologia assistiva?",
                                       "type"  => "string"));
            break;
        	case "deftransporte":
                array_push($col, array("campo" => "deftransporte",
                                       "label" => "Necessita de transporte acessível?",
                                       "type"  => "string"));
            break;
        	case "defhospedagem":
                array_push($col, array("campo" => "defhospedagem",
                                       "label" => "Necessita de hospedagem disponibilizada pelo Ministério da Cultura?",
                                       "type"  => "string"));
            break;
        	case "defacomodespec":
                array_push($col, array("campo" => "defacomodespec",
                                       "label" => "Necessita de acomodação especial?",
                                       "type"  => "string"));
            break;
        	case "defnomeemergencia":
                array_push($col, array("campo" => "defnomeemergencia",
                                       "label" => "Nome contato de emergência",
                                       "type"  => "string"));
            break;
        	case "defnumtelemergencia":
                array_push($col, array("campo" => "defnumtelemergencia",
                                       "label" => "Telefone contato de emergência",
                                       "type"  => "string"));
            break;
        	case "defnumtelemergencia2":
                array_push($col, array("campo" => "defnumtelemergencia2",
                                       "label" => "Outro telefone contato de emergência",
                                       "type"  => "string"));
            break;
        	case "defdeclaracao":
                array_push($col, array("campo" => "defdeclaracao",
                                       "label" => "Declaração de aptidão",
                                       "type"  => "string"));
            break;

        }
    }

    return $col;
}

?>