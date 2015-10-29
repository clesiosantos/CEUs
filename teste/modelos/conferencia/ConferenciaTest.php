<?php
include_once APPRAIZ . 'conferencia/classes/Conferencia.class.inc';

class ConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new Conferencia();
	}

	public function testGetByPreConferencia (){

        $pcnid = 22;
		$this->assertEquals('array', gettype($this->_class->getByPreConferencia( $pcnid)));
	}

}