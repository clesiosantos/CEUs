<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Módulo:listamunicipio.inc
   Finalidade: permitir o controle da lista de municipios
   */
$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";
print '<br>';
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$sql= "select muncod as codigo, mundsc as descricao, regcod as estado, muncodibge as ibge from municipio order by regcod,mundsc";
$RS = $db->record_set($sql);
$nlinhas = $db->conta_linhas($RS);
$titul = 'Lista de Municípios Cadastrados.';
monta_titulo($titulo_modulo,$titul);
$cabecalho = array('Código Interno','Descrição','UF','Código IBGE');
$db->monta_lista($sql,$cabecalho,50,20,'','','');
?>
