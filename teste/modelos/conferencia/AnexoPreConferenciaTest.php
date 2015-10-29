<?php
include_once APPRAIZ . 'conferencia/classes/AnexoPreConferencia.class.inc';

class AnexoPreConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new AnexoPreConferencia();
        $this->_id = 1;
	}

	public function testDesabilitaPorPreConferencia (){
		$this->assertEquals('NULL', gettype($this->_class->desabilitaPorPreConferencia($this->_id)));
	}
    
	public function testGetAnexoCoordenadorResponsavelByWhere (){
		$this->assertEquals('array', gettype($this->_class->getAnexoCoordenadorResponsavelByWhere(array())));
	}
}