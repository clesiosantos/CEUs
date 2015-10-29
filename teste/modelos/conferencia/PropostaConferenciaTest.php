<?php
include_once APPRAIZ . "conferencia/classes/PropostaConferencia.class.inc";

class PropostaConferenciaTest extends PHPUnit_Framework_TestCase
{
	protected $_stats;
	protected $_class;
	protected $_id;

	public function setUp()
	{
		parent::setUp();
		$this->_class = new PropostaConferencia();
        $this->_id = 1;
	}

	public function testGetTodosDadosById (){
		$this->assertEquals('array', gettype($this->_class->getTodosDadosById($this->_id)));
	}
    
	public function testSetNumeroPropostaByIdPreConferencia (){
		$this->assertEquals('string', gettype($this->_class->setNumeroPropostaByIdPreConferencia($this->_id)));
	}
    
	public function testGetLista (){
		$this->assertEquals('array', gettype($this->_class->getLista(array(), false)));
		$this->assertEquals('array', gettype($this->_class->getLista(array(), true)));
	}
    
    

}