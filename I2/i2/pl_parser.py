#!/usr/bin/env python

from pl_syntaxexception import SyntaxException
from pl_node import *
from pl_scanner import Scanner
from pl_token import Token

class Parser(object):
    """ generated source for class Parser """
    def __init__(self):
        self.scanner = None

    def match(self, s):
        """ generated source for method match """
        self.scanner.match(Token(s))

    def curr(self):
        """ generated source for method curr """
        return self.scanner.curr()

    def pos(self):
        """ generated source for method pos """
        return self.scanner.position()

    def parse(self, program):
        """ generated source for method parse """
        if program == '': return None
        self.scanner = Scanner(program)
        self.scanner.next()
        return self.parseBlock()

    def parseAssn(self):
        id = self.curr().lex() # Gets current lexeme

        self.match('id') # Checks if current lexeme is id
        self.match('=') # Checks if current lexeme is =

        num = self.curr().lex() # Saves current number
        self.match('num') # Checks if current lexeme is num
        
        return AssnNode(id, num) 

    def parseStmt(self):
        assn = self.parseAssn() # Parses an assignment 
        return NodeStmt(assn)

    def parseBlock(self):
        stmt = self.parseStmt() # Parses a statement
        block_two = None # Optional second block 
        curr = self.curr() # Gets current token
        if curr.lexeme == ";": 
            self.scanner.next() # Advances to the next token so we don't parse the semicolon
            block_two = self.parseBlock() # Parses the next block
        
        return NodeBlock(stmt, block_two)


