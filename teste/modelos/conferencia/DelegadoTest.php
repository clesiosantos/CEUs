<?php
include_once APPRAIZ . 'conferencia/classes/Delegado.class.inc';

class DelegadoTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new Delegado();
	}

	public function testGetDelegadosByPcnid (){

        $pcnid = 22;
		$this->assertEquals('array', gettype($this->_class->getDelegadosByPcnid( $pcnid , false )));
		$this->assertEquals('array', gettype($this->_class->getDelegadosByPcnid( $pcnid , true )));

	}
	public function testGetDelegadosWhere (){

        $where = array("de.delnome ILIKE '%A%'");
		$this->assertEquals('array', gettype($this->_class->getDelegadosWhere( $where , false)));
		$this->assertEquals('array', gettype($this->_class->getDelegadosWhere( $where , true)));

	}

}