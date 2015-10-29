<?php

include_once APPRAIZ . "conferencia/classes/EixoTematicoConferencia.class.inc";

class EixoTematicoConferenciaTest extends PHPUnit_Framework_TestCase
{

    protected $_stats;
    protected $_class;
    protected $_id;

    public function setUp()
    {
        parent::setUp();
        $this->_class = new EixoTematicoConferencia();
        $this->_id    = 1;
    }

    public function testDesabilitaPorConferencia()
    {
        $this->assertEquals('NULL', gettype($this->_class->desabilitaPorConferencia($this->_id)));
    }

    public function testGetEixoTematicoByWhere()
    {
        $this->assertEquals('array', gettype($this->_class->getEixoTematicoByWhere(array())));
        $this->assertEquals('array', gettype($this->_class->getEixoTematicoByWhere(array(), true)));
    }

    public function testGetSubEixoTematicoByWhere()
    {
        $this->assertEquals('array', gettype($this->_class->getSubEixoTematicoByWhere(array())));
        $this->assertEquals('array', gettype($this->_class->getSubEixoTematicoByWhere(array(), true)));
    }

}