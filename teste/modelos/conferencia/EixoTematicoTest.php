<?php
include_once APPRAIZ . 'conferencia/classes/EixoTematico.class.inc';

class EixoTematicoTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new EixoTematico();
	}

	public function testGetLista (){
		$this->assertEquals('array', gettype($this->_class->getLista( array())));
		$this->assertEquals('array', gettype($this->_class->getLista( array(), true)));
    }
    
	public function testGetListaSubEixo (){
		$this->assertEquals('array', gettype($this->_class->getListaSubEixo()));
    }

}