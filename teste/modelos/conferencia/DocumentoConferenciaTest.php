<?php
include_once APPRAIZ . 'conferencia/classes/DocumentoConferencia.class.inc';

class DocumentoConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new DocumentoConferencia();
	}

	public function testDesabilitaPorConferencia (){

        $pcnid = 22;
		$this->assertEquals('array', gettype($this->_class->getDocumentoConferenciaByWhere( array(), true)));
		$this->assertEquals('array', gettype($this->_class->getDocumentoConferenciaByWhere( array())));
	}
	public function testVerificaExistenciaDocumentoByIdAndTipo (){

        $pcnid = 22;
        $dcftipo = 'RF';
		$this->assertEquals('NULL', gettype($this->_class->desabilitaPorConferencia( $pcnid)));
	}

}