#!/usr/bin/env python

from pl_syntaxexception import SyntaxException
from pl_node import *
from pl_scanner import Scanner
from pl_token import Token
from pl_environment import Environment


class Parser(object):
    """ generated source for class Parser """
    def __init__(self):
        self.scanner = None
        self.env = Environment()

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
        return self.parseProg()

    def parseAssn(self):
        id = self.curr().lex() # Gets current lexeme

        self.match('id') # Checks if current lexeme is id
        self.match('=') # Checks if current lexeme is =

        expr = self.parseExpr()
    
        return NodeAssn(id, expr) 
    
    def parseFact(self):
        #If a fact is an id then match the token as an id and return the node fact with an id
        if(self.curr().tok() == "id"):
            id = self.curr().lex()
            self.match('id')
            return NodeFact(id, None, None)
        # If we have a ( then we have an expression
        elif("(" in self.curr().lex()):
            
            self.match('(')
            expr = self.parseExpr()
            self.match(')')

            
            return NodeFact(None, None, expr)
        # If not a ( or expr then expect a num 
        else:
            num = self.curr().lex()
            self.match('num')
            return NodeFact(None, num, None)
        
    # returns appropriate Addop Node
    def parseAddOp(self):
        addop = self.curr().tok()

        if(addop == "+"):
            self.match("+")
            return NodeAddOp(addop)
        elif(addop == "-"):
            self.match("-")
            return NodeAddOp(addop)
        else:
            return NodeAddOp(None)
    # Returns appropriate mulop node
    def parseMulOp(self):
        mulop = self.curr().lex()
        if(mulop == "*"):
            self.match("*")
            return NodeMulOp(mulop)
        elif(mulop == "/"):
            self.match("/")
            return NodeMulOp(mulop)
        else:
            return NodeMulOp(None)
    def parseTerm(self):
        # Parses fact
        fact = self.parseFact()
        # Parses mulop
        mulop = self.parseMulOp()
        # If we don't have a mulop then return term node with just fact
        if mulop.mulop is None:
            return NodeTerm(fact, None, None)
        # If we have a mulop then parse a term and append the fact and mulop onto the term node
        term = self.parseTerm()
        term.append(NodeTerm(fact, mulop, None))

        return term
    
    def parseExpr(self):
        # Parses term
        term = self.parseTerm()
        # parses addop
        addop = self.parseAddOp()
        # If we don't have an addop then return a node with just a term
        if addop.addop is None:
            return NodeExpr(term, None, None)
        # If we do have addop then parse an expression and add a term to the node
        expr = self.parseExpr()
        expr.append(NodeExpr(term, addop, None))
        return expr
    
    def parseWr(self):
        # Parses an expression and returns a wr node with an expression inside
        expr = self.parseExpr()
        return NodeWr(expr)
    
    def parseStmt(self):
        # Checks if the current token is a wr keyword if so parse a wr node
        if self.curr().tok() == "wr":
            self.match("wr")
            wr = self.parseWr()
            return NodeStmt(None, None, wr, None, None, None)
        # If not a wr then must be assn node
        elif self.curr().tok() == "rd":
            self.match("rd")
            rd = self.parseRd()
            return NodeStmt(None, rd, None, None, None, None)
        elif self.curr().tok() == "if":
            self.match("if")
            ifKey = self.parseIf()
            return NodeStmt(None, None, None, ifKey, None, None)
        elif(self.curr().tok() == "while"):
            self.match("while")
            whileKey = self.parseWhile()
            return NodeStmt(None, None, None, None, whileKey, None)
        elif(self.curr().tok() == "begin"):
            self.match("begin")
            begin = self.parseBegin()
            return NodeStmt(None, None, None, None, None, begin)
        else:
            assn = self.parseAssn() # Parses an assignment 
            return NodeStmt(assn, None, None, NOne, None, None)

        

    def parseBlock(self):
        stmt = self.parseStmt() # Parses a statement
        block_two = None # Optional second block 
        curr = self.curr() # Gets current token
        if curr.lexeme == ";": 
            self.scanner.next() # Advances to the next token so we don't parse the semicolon
            block_two = self.parseBlock() # Parses the next block
        
        return NodeBlock(stmt, block_two)

    def parseProg(self):
        block = self.parseBlock() # Parses a block
        return NodeProg(block)
    
    def parseRd(self):
        rID = self.curr().lex()
        self.match('id')
        return NodeRd(rID)
    def parseIf(self):
        boolExpr = self.parseBoolExpr()
        self.match('then')
        stmt = self.parseStmt()

        return NodeIf(boolExpr, stmt)
        
    def parseBoolExpr(self):
        expr = self.parseExpr()
        relop = self.parseRelOp()
        expr2 = self.parseExpr()
        

        return NodeBoolExpr(expr, relop, expr2)    

    def parseRelOp(self):
        if(self.curr().tok() == "<"):
            self.match("<")
            return NodeRelOp("<")
        elif(self.curr().tok() == "<="):
            self.match("<=")
            return NodeRelOp("<=")
        elif(self.curr().tok() == ">"):
            self.match(">")
            return NodeRelOp(">")
        elif(self.curr().tok() == ">="):
            self.match(">=")
            return NodeRelOp(">=")
        elif(self.curr().tok() == "<>"):
            self.match("<>")
            return NodeRelOp("<>")
        else:
            self.match("==")
            return NodeRelOp("==")
       


