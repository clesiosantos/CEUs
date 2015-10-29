<?php
include_once APPRAIZ . "conferencia/classes/MomentoConferencia.class.inc";

class MomentoConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new MomentoConferencia();
	}

	public function testGetLista (){
		$this->assertEquals('array', gettype($this->_class->getLista(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getLista(array(), true)));
	}
    
	public function testGetCombo (){
		$this->assertEquals('NULL', gettype($this->_class->getCombo()));
	}
    
	public function testRecuperarDadosCombo (){
		$this->assertEquals('array', gettype($this->_class->recuperarDadosCombo()));
	}
	
}