<?php
include_once APPRAIZ . "conferencia/classes/OrgaoGestorCultura.class.inc";

class OrgaoGestorCulturaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new OrgaoGestorCultura();
	}

	public function testGetLista (){
		$this->assertEquals('array', gettype($this->_class->getLista(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getLista(array(), true)));
	}

}