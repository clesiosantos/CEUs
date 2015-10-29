<?php
include_once APPRAIZ . "conferencia/classes/MomentoTipoConferencia.class.inc";

class MomentoTipoConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;
    protected $_id;

    public function setUp()
	{
		parent::setUp();
		$this->_class = new MomentoTipoConferencia();
        $this->_id = 1;
	}

	public function testListarTipos (){
		$this->assertEquals('array', gettype($this->_class->listarTipos($this->_id)));
	}
    
	public function testGetTiposLiberadosByMomento (){
		$this->assertEquals('array', gettype($this->_class->getTiposLiberadosByMomento($this->_id)));
	}

}