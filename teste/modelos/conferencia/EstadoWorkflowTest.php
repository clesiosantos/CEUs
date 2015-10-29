<?php
include_once APPRAIZ . "conferencia/classes/EstadoWorkflow.class.inc";

class EstadoWorkflowTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new EstadoWorkflow();
	}

	public function testGetLista (){
		$this->assertEquals('array', gettype($this->_class->getEstadoWorkflow()));
	}

}