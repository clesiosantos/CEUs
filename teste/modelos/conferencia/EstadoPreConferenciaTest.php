<?php
include_once APPRAIZ . "conferencia/classes/EstadoPreConferencia.class.inc";

class EstadoPreConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;
    protected $_id;

    public function setUp()
	{
		parent::setUp();
		$this->_class = new EstadoPreConferencia();
        $this->_id = 1;
	}

	public function testDesabilitaPorPreConferencia (){
		$this->assertEquals('NULL', gettype($this->_class->desabilitaPorPreConferencia($this->_id)));
	}
    
	public function testGetEstadoPreConferenciaByWhere (){
		$this->assertEquals('array', gettype($this->_class->getEstadoPreConferenciaByWhere(array())));
	}

}