<?php
include_once "config.inc";
include_once APPRAIZ . 'includes/funcoes.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/php-excel/PHPExcel.php';

$db = new cls_banco();

$objReader = PHPExcel_IOFactory::createReader('Excel2007');
$objReader->setReadDataOnly(true);

$objPHPExcel = $objReader->load("CEUs Geocodificados_2.xlsx");
$qtdSheet = $objPHPExcel->getSheetCount();

$objWorksheet = $objPHPExcel->setActiveSheetIndex(0);

$arInserts = array();

foreach ($objWorksheet->getRowIterator() as $k => $row) {
    if($k == 1){
        continue;
    }

    $nLinha = $row->getRowIndex();
    $cellIterator = $row->getCellIterator();
    $cellIterator->setIterateOnlyExistingCells(false);

    // Laço das colunas
    foreach ($cellIterator as $cell) {
        // Letra da coluna
        $lColuna = $cell->getColumn();
        $columnValue = arrumaString($cell-> getCalculatedValue());

        switch ($lColuna) {
        	case 'A':
                $muncod = $columnValue;
        	break;
        	case 'B':
                $ceucodigo = $columnValue;
        	break;
        	case 'D':
                $estuf = $columnValue;
        	break;
        	case 'G':
                $ceunome = $columnValue;
        	break;
        	case 'H':
        	    $endereco = $columnValue;
        	break;
        	case 'X':
        	    $lat = $columnValue;
        	break;
        	case 'Y':
        	    $long = $columnValue;
        	break;
        }
    }
    preg_match('/[0-9]{8}/', $endereco, $cep);
    $cep = $cep[0];

    $dadosEndereco = $db->pegaLinha("select * from cep.v_endereco where cep = '{$cep}' order by cidade asc");

    $edccep        = "";
    $edclogradouro = "";
    $edcnumero     = 0;
    $edcbairro     = "";
    if ($dadosEndereco) {
        $edccep        = formataCEPComPontos($dadosEndereco["cep"]);
        $edclogradouro = str_replace("'", "''", $dadosEndereco["logradouro"]);
        $edcnumero     = 0;
        $edcbairro     = str_replace("'", "''", $dadosEndereco["bairro"]);
    }

    $edccoordenadas = "POINT({$long} {$lat})";

    $sqlInsertEndereco = "INSERT INTO conferencia.endereco(
                                                            edccep, edclogradouro, edcnumero, edccomplemento, edcbairro,
                                                            muncod, estuf, edccoordenadas, edczoom)
                                                    VALUES ('{$edccep}', '{$edclogradouro}', '{$edcnumero}', null, '{$edcbairro}',
                                                            '{$muncod}', '{$estuf}', '{$edccoordenadas}', 13);";

    $sqlInsert = "INSERT INTO conferencia.ceu(
                                            ceucodigo, ceunome, ceudescricao, ceustatus, ceudtcadastro, usucpf, edcid)
                                    VALUES ('{$ceucodigo}', '{$ceunome}', null, 'A',
                                            now(), '03700155689', (SELECT MAX(edcid) FROM conferencia.endereco));";
    $arInserts[] = $sqlInsertEndereco;
    $arInserts[] = $sqlInsert;
}

echo implode("<br />", $arInserts);

// Arruma codificação e retira espaços em branco
function arrumaString($valor){
    return trim(utf8_decode($valor));
}