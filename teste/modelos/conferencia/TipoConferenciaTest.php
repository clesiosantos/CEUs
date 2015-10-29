<?php
include_once APPRAIZ . "conferencia/classes/TipoConferencia.class.inc";

class TipoConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new TipoConferencia();
	}

	public function testGetSiglaById (){
		$this->assertEquals('string', gettype($this->_class->getSiglaById(1)));
	}
    
    public function testGetDadosConferencia (){
		$this->assertEquals('array', gettype($this->_class->getDadosConferencia(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getDadosConferencia(array(), true)));
	}

}