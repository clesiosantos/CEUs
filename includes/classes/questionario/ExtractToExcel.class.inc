<?php
final class ExtractToExcel{
	public function __construct(Array $param)
	{
		// Criar um novo objecto PHPExcel
		$PHPExcel = new PHPExcel();

        // Perguntas e grupos
	    $mAcordo = new QAcordo();
	    $queid   = $mAcordo->getQuestionarioByAcordo($param['acoid']);

	    $mPergunta   = new QPergunta();
	    $rsPerguntas = $mPergunta->getPrimeiroNivelByQuestionario($queid);

		$mGrupoPerg = new QGrupo();
        $rsGrupos   = $mGrupoPerg->getGrupoPrimeiroNivelByQuestionario($queid);

        if(count($rsPerguntas) == 0 && count($rsGrupos) > 0){
            foreach($rsGrupos as $k => $grupo){
                $titulo = strlen($grupo["grptitulo"]) > 31 ? substr($grupo["grptitulo"], 0, 28) . '...' : $grupo["grptitulo"];
                $titulo = str_replace(array("/"), " ", $titulo);

                $tabela = $k == 0 ? $PHPExcel->getActiveSheet() : $PHPExcel->createSheet();
        		$tabela->setTitle(utf8_encode($titulo));

        		$question = new ExtractQuestionToExcel(array('acoid' => $param['acoid'], 'grpid' => $grupo['grpid']));
        		$question->PHPExcel($tabela);
            }
        }else{
    		$tabela   = $PHPExcel->getActiveSheet();
    		$tabela->setTitle(("Questões"));

    		$question = new ExtractQuestionToExcel($param);
    		$question->PHPExcel($tabela);
        }
        // Perguntas e grupos

        // Perguntas Tabelas
		$tabela2  = $PHPExcel->createSheet();
		$tabela2->setTitle(("Tabela"));

		$table 	  = new ExtractTableToExcel($param);
		$table->PHPExcel($tabela2);
        // Perguntas Tabelas

		// Perguntas Múltiplas
		$tabelaMultiplas  = $PHPExcel->createSheet();
		$tabelaMultiplas->setTitle(("Multiplo"));

		$table 	  = new ExtractMultipleToExcel($param);
		$table->PHPExcel($tabelaMultiplas);
		// Perguntas Múltiplas

		// Perguntas e, f, g h
		$tabelaTabelasMultiplas  = $PHPExcel->createSheet();
		$tabelaTabelasMultiplas->setTitle("Others");

		$table 	  = new ExtractMultipleTableToExcel($param);
		$table->PHPExcel($tabelaTabelasMultiplas);
		// Perguntas e, f, g h

		// Indicação da criação do ficheiro
		$objWriter = PHPExcel_IOFactory::createWriter($PHPExcel, 'Excel5');

		// Encaminhar o ficheiro resultante para abrir no browser ou fazer download
		header('Content-Type: application/vnd.ms-excel');
		header('Content-Disposition: attachment;filename="relatorio.xls"');
		header('Cache-Control: max-age=0');
		$objWriter->save('php://output');
	}
}




