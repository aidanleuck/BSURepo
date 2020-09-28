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

        expr = self.parseExpr()
    
        return NodeAssn(id, expr) 
    def parseFact(self):
        if(self.curr().lex() == "id"):
            id = self.curr().lex()
            self.match('id')
            return NodeFact(id)
        elif(self.curr().lex() == "("):
            self.match('(')
            expr = self.parseExpr()
            self.match(")")


            return NodeFact(expr)
        else:
            num = self.curr().lex()
            self.match('num')

            return NodeFact(num)


    def parseTerm(self):
        fact = self.parseFact()
        mulop = self.curr().lex()
        
        if mulop != "*":
            return NodeTerm(fact, None, None)
        
        self.match("*")
        term = self.parseTerm()
        term.append(NodeTerm(term, None, fact))

        return NodeTerm(fact, None, term)

    def parseExpr(self):
        term = self.parseTerm()
        addop = self.curr().lex()

        if addop != "+":
            return NodeExpr(term, None, None)
        
        self.match("+")
        expr = self.parseExpr()
        expr.append(NodeExpr(term, addop, None))
        return NodeExpr(term, None, expr)
    def parseWr(self):
        expr = self.parseExpr()
        return NodeWr(expr)

    def parseStmt(self):
        if self.curr().lexeme == "wr":
            wr = self.parseWr()
            return NodeStmt(wr)
        else:
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


