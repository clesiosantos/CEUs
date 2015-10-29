<?php
$habilitado = $_REQUEST['habilitado'] ? $_REQUEST['habilitado'] : 'S';
?>
<html>
    <head>
        <meta http-equiv="Cache-Control" content="no-cache">
        <meta http-equiv="Pragma" content="no-cache">
        <meta http-equiv="Expires" content="-1">
        <title> </title>

        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <script src="../includes/JQuery/jquery-1.4.2.min.js"></script>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

        <style type="text/css">
            .principal { width:645px;}
        </style>
        <style type="text/css" media="print">
            .principal {width:100%;}
            .noPrint {display:none;}
            table{
                -webkit-print-color-adjust: exact;
            }
        </style>
    </head>
    <body>
        <?php
        monta_titulo( "Histórico de Questionário de {$titulo_modulo}", '' );

        $arConfig = array("style"           => "width:95%;",
                          "totalLinha"      => false,
                          "totalRegistro"   => true);
        $arCabecalho = array("Ação", "Tipo","Situação", "Cadastrado por", "Data de Cadastro");

        $mVinculoCeuQuestionario = new VinculoCeuQuestionario();
        $rs = $mVinculoCeuQuestionario->listarQuestionarioHistorico($vceid, $queid);

//         <img src=\'/imagens/duplicar.png\' style="cursor:pointer; width: 16px; height: 16px;" title="Duplicar a partir deste" onclick="window.opener.duplicarQuestionario( {vcqid} )">

        if($habilitado == 'S'){
            $acao = '<center>
                    	<img src="/imagens/consultar.gif" style="cursor:pointer" title="Visualizar questionário" onclick="consultarQuestionario({vcqid});">
                     </center>';
        }else{
            $acao = '<center>
                    	<img src="/imagens/consultar.gif" style="cursor:pointer" title="Visualizar questionário" onclick="consultarQuestionario({vcqid});">
                     </center>';
        }

        $oLista = new Lista($arConfig);
        $oLista->setCabecalho( $arCabecalho );
        $oLista->setCorpo( $rs, $arParamCol );
        $oLista->setAcao( $acao );
        $oLista->show();
        ?>
    <script type="text/javascript">
        function consultarQuestionario(vcqid){
            window.opener.consultarQuestionario(vcqid);
            window.close();
        }
    </script>
    </body>
</html>