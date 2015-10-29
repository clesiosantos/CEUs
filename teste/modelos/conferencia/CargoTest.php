<?php
include_once APPRAIZ . 'conferencia/classes/Cargo.class.inc';

class CargoTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new Cargo();
	}

	public function testGetLista (){

		$this->assertEquals('array', gettype($this->_class->getLista(array(), false , 'cgo.cgostatus')));
		$this->assertEquals('array', gettype($this->_class->getLista(array(), true , 'cgo.cgostatus ')));
	}

}