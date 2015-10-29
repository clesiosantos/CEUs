<?php
include_once APPRAIZ . 'conferencia/classes/DelegadoConferencia.class.inc';

class DelegadoConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new DelegadoConferencia();
	}

	public function testVincularDelegado (){

        $pcnid = 13;
        $delid = 22;
		$this->assertEquals('boolean', gettype($this->_class->vincularDelegado($delid, $pcnid)));

	}
	public function testVerificaExiste (){

        $delcpf = "00000000000";
		$this->assertEquals('array', gettype($this->_class->verificaExiste($delcpf)));

	}
	public function testDesvincularDelegado (){

        $pcnid = 13;
        $delid = 22;
		$this->assertEquals('boolean', gettype($this->_class->desvincularDelegado($delid,$pcnid)));

	}

}