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

    // Estado
    if( $estuf[0]  && $estuf_campo_flag ){
        array_push($where, " est.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ");
    }

    // Municipio
    if( $muncod[0]  && $muncod_campo_flag ){
        array_push($where, " mun.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "') ");
    }

    // Orgão Gestor em cultura
    if( $ogcid[0]  && $ogcid_campo_flag ){
        array_push($where, " ogc.ogcid " . (!$ogcid_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ogcid ) . "') ");
    }

    array_push($where, " pcn.pcnstatus = 'A'");

    $sql = "SELECT
                mco.mconome,
                tpc.tpcnome,
                ogc.ogcnome,
                cgo.cgonome,
                mun.mundescricao as endmundescricao,
                est.estuf as endestuf,
                (array_to_string(array(SELECT
                                        COALESCE(mun2.mundescricao || '/', '') || epc.estuf
                                    FROM
                                        conferencia.estadopreconferencia epc
                                    LEFT JOIN
                                        territorios.municipio as mun2 ON epc.muncod = mun2.muncod
                                    WHERE
                                        epc.pcnid = pcn.pcnid AND epc.epcstatus = 'A'
                                    ORDER BY 1), ', '))
               AS mundescricao,
               pcn.pcntitulo,
               to_char(pcn.pcninicio, 'DD/MM/YYYY') || ' á ' || to_char(pcn.pcntermino, 'DD/MM/YYYY') as pcnperiodo,
               ende.endcep,
               ende.endlog,
               ende.endnum,
               este.esddsc,
               COALESCE(mun.mundescricao || '/', '') ||  est.estuf AS endmundescricaocompleto,
               pcn.pcnid,
               CASE
                    WHEN tpc.tpcsigla = 'L' THEN '-'
                    WHEN ( SELECT COUNT(*) FROM conferencia.anexopreconferencia apc WHERE apc.arqid IS NOT NULL AND apc.pcnid = pcn.pcnid AND apcstatus = 'A' AND apctipo = 'D' ) > 0 THEN 'Sim'
                    ELSE 'Não'
               END as possuidecretoconvocacao,
               CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    WHEN ( SELECT COUNT(*) FROM conferencia.anexopreconferencia apc WHERE apc.arqid IS NOT NULL AND apc.pcnid = pcn.pcnid AND apcstatus = 'A' AND apctipo = 'PD' ) > 0 THEN 'Sim'
                    ELSE 'Não'
               END as possuidecretoconvocacaodiario,
               CASE
                    WHEN tpc.tpcsigla = 'L' THEN '-'
                    WHEN ( SELECT COUNT(*) FROM conferencia.anexopreconferencia apc WHERE apc.arqid IS NOT NULL AND apc.pcnid = pcn.pcnid AND apcstatus = 'A' AND apctipo = 'P' ) > 0 THEN 'Sim'
                    ELSE 'Não'
               END as possuiportarianomeacao,
               CASE
                    WHEN tpc.tpcsigla = 'L' THEN '-'
                    ELSE  to_char(cnf.cnfdtpubconvocacao, 'DD/MM/YYYY')
               END AS cnfdtpubconvocacao,
               CASE
                    WHEN tpc.tpcsigla = 'L' THEN '-'
                    ELSE  to_char(cnf.cnfdtrealizacao, 'DD/MM/YYYY')
               END AS cnfdtrealizacao,
               CASE
                    WHEN tpc.tpcsigla != 'L' THEN '-'
                    ELSE  cnfsegmento
               END AS cnfsegmento,
               CASE
                    WHEN tpc.tpcsigla = 'R' OR tpc.tpcsigla = 'V' OR tpc.tpcsigla = 'E' THEN '-'
                    ELSE  cnfqtdptcconferencia::text
               END AS cnfqtdptcconferencia,
               CASE
                    WHEN tpc.tpcsigla = 'R' OR tpc.tpcsigla = 'V' OR tpc.tpcsigla = 'L' OR tpc.tpcsigla = 'E' OR tpc.tpcsigla = 'T' THEN '-'
                    ELSE  cnfqtdptcpreconferencia::text
               END AS cnfqtdptcpreconferencia,
               CASE
                    WHEN tpc.tpcsigla = 'R' OR tpc.tpcsigla = 'V'  OR tpc.tpcsigla = 'l' THEN '-'
                    WHEN tpc.tpcsigla = 'E' THEN ( cnfqtdsociedadecivil + cnfqtdpoderpublico)::text
                    ELSE  cnfqtddelegeleitos::text
               END AS cnfqtddelegeleitos,
                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  cnfqtddeleganteriores::text
               END AS cnfqtddeleganteriores,

                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  cnfqtddelegnatos::text
               END AS cnfqtddelegnatos,

                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  (cnfqtddelegnatos + cnfqtddeleganteriores)::text
               END AS totaldelegadospresentes,
                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  cnfqtdpublico::text
               END AS cnfqtdpublico,
                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  cnfqtdsociedadecivil::text
               END AS cnfqtdsociedadecivil,
                CASE
                    WHEN tpc.tpcsigla != 'E' THEN '-'
                    ELSE  cnfqtdpoderpublico::text
               END AS cnfqtdpoderpublico,
               cnfinfadc
            FROM
                conferencia.preconferencia pcn
            LEFT JOIN
                conferencia.conferencia cnf on pcn.pcnid = cnf.pcnid
            LEFT JOIN
                conferencia.cargo cgo on pcn.cgoid = cgo.cgoid
            INNER JOIN
                conferencia.tipoconferencia tpc on pcn.tpcid = tpc.tpcid
            LEFT JOIN
                conferencia.orgaogestorcultura ogc on pcn.ogcid = ogc.ogcid
            INNER JOIN
                workflow.documento doc on pcn.docid = doc.docid
            INNER JOIN
                workflow.estadodocumento este on doc.esdid = este.esdid
            INNER JOIN
                conferencia.momentoconferencia mco ON mco.mcoid = pcn.mcoid
            LEFT JOIN
                conferencia.endereco ende ON ende.endid = pcn.endid
            LEFT JOIN
		        territorios.municipio mun ON mun.muncod = ende.muncod
            LEFT JOIN
		         territorios.estado est ON est.estuf = ende.estuf
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
                                          "tpcnome",
                                          "mundescricao",
                                          "pcntitulo",
                                          "esddsc",
                                          "ogcnome",
                                          "cgonome",
                                          "endcep",
                                          "endlog",
                                          "endnum",
                                          "endmundescricaocompleto",
                                          "pcnperiodo",
                                          "possuidecretoconvocacao",
                                          "possuidecretoconvocacaodiario",
                                          "possuiportarianomeacao",

                                          "cnfdtpubconvocacao",
                                          "cnfdtrealizacao",
                                          "cnfsegmento",
                                          "cnfqtdptcconferencia",
                                          "cnfqtdptcpreconferencia",
                                          "cnfqtddelegeleitos",
                                          "cnfqtddeleganteriores",
                                          "cnfqtddelegnatos",
                                          "totaldelegadospresentes",
                                          "cnfqtdpublico",
                                          "cnfqtdsociedadecivil",
                                          "cnfqtdpoderpublico",
                                          "cnfinfadc")
                                        );

    foreach ( $agrupador as $val ){
        switch( $val ){
            case "pcnid":
                array_push($agp['agrupador'], array(
                                                    "campo" => "pcnid",
                                                    "label" => "Identificador da Conferência")
                                                    );
            break;
            case "mconome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "mconome",
                                                    "label" => "Edição da Conferência")
                                                    );
            break;
            case "tpcnome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "tpcnome",
                                                    "label" => "Tipo da Conferência")
                                                    );
            break;
            case "ogcnome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "ogcnome",
                                                    "label" => "Órgão Gestor em Cultura")
                                                    );
            break;
            case "cgonome":
                array_push($agp['agrupador'], array(
                                                    "campo" => "cgonome",
                                                    "label" => "Cargo")
                                                    );
            break;
            case "endmundescricao":
                array_push($agp['agrupador'], array(
                                                    "campo" => "endmundescricao",
                                                    "label" => "Município")
                                                    );
            break;
            case "endestuf":
                array_push($agp['agrupador'], array(
                                                    "campo" => "endestuf",
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
        	case "mundescricao":
                array_push($col, array("campo" => "mundescricao",
                                       "label" => "Município/UF",
                                       "type"  => "string"));
            break;
        	case "pcntitulo":
                array_push($col, array("campo" => "pcntitulo",
                                       "label" => "Título da Conferência",
                                       "type"  => "string"));
            break;
        	case "pcnperiodo":
                array_push($col, array("campo" => "pcnperiodo",
                                       "label" => "Período",
                                       "type"  => "string"));
            break;
        	case "esddsc":
                array_push($col, array("campo" => "esddsc",
                                       "label" => "Situação da Conferência",
                                       "type"  => "string"));
            break;
        	case "ogcnome":
                array_push($col, array("campo" => "ogcnome",
                                       "label" => "Órgão Gestor em Cultura",
                                       "type"  => "string"));
            break;
        	case "cgonome":
                array_push($col, array("campo" => "cgonome",
                                       "label" => "Cargo",
                                       "type"  => "string"));
            break;
        	case "endcep":
                array_push($col, array("campo" => "endcep",
                                       "label" => "CEP Realização",
                                       "type"  => "string"));
            break;
        	case "endlog":
                array_push($col, array("campo" => "endlog",
                                       "label" => "Logradouro Realização",
                                       "type"  => "string"));
            break;
        	case "endnum":
                array_push($col, array("campo" => "endnum",
                                       "label" => "Número Realização",
                                       "type"  => "string"));
            break;
        	case "endmundescricaocompleto":
                array_push($col, array("campo" => "endmundescricaocompleto",
                                       "label" => "Município/UF Realização",
                                       "type"  => "string"));
            break;
        	case "pcnperiodo":
                array_push($col, array("campo" => "pcnperiodo",
                                       "label" => "Período",
                                       "type"  => "string"));
            break;
        	case "possuidecretoconvocacao":
                array_push($col, array("campo" => "possuidecretoconvocacao",
                                       "label" => "Decreto de Convocação da Conferência",
                                       "type"  => "string"));
            break;
        	case "possuidecretoconvocacaodiario":
                array_push($col, array("campo" => "possuidecretoconvocacaodiario",
                                       "label" => "Publicação do Decreto de Convocação da Conferência em Diário Oficial",
                                       "type"  => "string"));
            break;
        	case "possuiportarianomeacao":
                array_push($col, array("campo" => "possuiportarianomeacao",
                                       "label" => "Portaria de Nomeação do Coordenador",
                                       "type"  => "string"));
            break;
        	case "cnfdtpubconvocacao":
                array_push($col, array("campo" => "cnfdtpubconvocacao",
                                       "label" => "Data de Publicação da Convocação",
                                       "type"  => "string"));
            break;
        	case "cnfdtrealizacao":
                array_push($col, array("campo" => "cnfdtrealizacao",
                                       "label" => "Data de Realização da Conferência",
                                       "type"  => "string"));
            break;
        	case "cnfsegmento":
                array_push($col, array("campo" => "cnfsegmento",
                                       "label" => "Segmento ou Entidade que Convocou a Conferência",
                                       "type"  => "string"));
            break;
        	case "cnfqtdptcconferencia":
                array_push($col, array("campo" => "cnfqtdptcconferencia",
                                       "label" => "Quantidade de participandes - Conferência",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtdptcpreconferencia":
                array_push($col, array("campo" => "cnfqtdptcpreconferencia",
                                       "label" => "Quantidade de participandes - Pré-Conferência",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtddelegeleitos":
                array_push($col, array("campo" => "cnfqtddelegeleitos",
                                       "label" => "Quantidade de participandes - Delegados Eleitos",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtddeleganteriores":
                array_push($col, array("campo" => "cnfqtddeleganteriores",
                                       "label" => "Quantidade de participandes - Delegados Municipais",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtddelegnatos":
                array_push($col, array("campo" => "cnfqtddelegnatos",
                                       "label" => "Quantidade de participandes - Delegados Natos",
                                       "type"  => "numeric"));
            break;
        	case "totaldelegadospresentes":
                array_push($col, array("campo" => "totaldelegadospresentes",
                                       "label" => "Quantidade de participandes - Total de Delegados Presentes",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtdpublico":
                array_push($col, array("campo" => "cnfqtdpublico",
                                       "label" => "Quantidade de participandes - Público Total da Conferência",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtdsociedadecivil":
                array_push($col, array("campo" => "cnfqtdsociedadecivil",
                                       "label" => "Delegados Eleitos Nesta Etapa - Sociedade Civil",
                                       "type"  => "numeric"));
            break;
        	case "cnfqtdpoderpublico":
                array_push($col, array("campo" => "cnfqtdpoderpublico",
                                       "label" => "Delegados Eleitos Nesta Etapa - Poder Público",
                                       "type"  => "numeric"));
            break;
        	case "cnfinfadc":
                array_push($col, array("campo" => "cnfinfadc",
                                       "label" => "Informações Adicionais",
                                       "type"  => "string"));
            break;
        }
    }

    return $col;
}

?>