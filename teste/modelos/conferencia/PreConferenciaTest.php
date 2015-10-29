<?php
include_once APPRAIZ . "conferencia/classes/PreConferencia.class.inc";
include_once APPRAIZ . "www/conferencia/_constantes.php";

class PreConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;
    protected $_id;

    public function setUp()
	{
		parent::setUp();
		$this->_class = new PreConferencia();
        $this->_id = 1;
	}

	public function testGetDadosConferencia (){
		$this->assertEquals('array', gettype($this->_class->getDadosConferencia(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getDadosConferencia(array(), true)));
	}
    
	public function testGetDadosListaIncioConferencia (){
		$this->assertEquals('array', gettype($this->_class->getDadosListaIncioConferencia(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getDadosListaIncioConferencia(array(), true)));
	}
    
	public function testGetTodosDados (){
		$this->assertEquals('array', gettype($this->_class->getTodosDados($this->_id)));
	}

}