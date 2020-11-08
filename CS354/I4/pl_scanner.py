#!/usr/bin/env python
""" generated source for module Scanner """
#  (C) 2013 Jim Buffenbarger
#  All rights reserved.
from pl_syntaxexception import SyntaxException
from pl_token import Token
import sys


class Scanner(object):
    """ generated source for class Scanner """
    program = ""
    whitespace = {' ','\n','\t'}
    digits = set("0123456789")
    letters = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
    legits = set("_").union(digits).union(letters)
    #symbols = set("@[\]^_`!\"#$%&',)(*+-./:;<=>?")
    operators = {"=", ";", "(", ")", "+", "-", "*", "/", "<", "<=", "<>", ">=", ">", "==" }
    keywords = {"wr", "rd", "if", "while", "begin", "end", "then", "else", "do"}
    token = ""
    lexeme = ""


    def __init__(self, program):
        """ generated source for method __init__ """
        self.program = program
        self.pos = 0
        self.token = None

    def done(self):
        """ generated source for method done """
        return self.pos >= len(self.program)

    def many(self, s):
        """ generated source for method many """
        while not self.done() and self.program[self.pos] in s:
            self.pos += 1

    def past(self, c):
        """ generated source for method past """
        while not self.done() and c != self.program[self.pos]:
            self.pos += 1
        if not self.done() and c == self.program[self.pos]:
            self.pos += 1

    def nextNumber(self):
       startPos = self.pos

       self.many(self.digits)

       retrievedToken = self.program[startPos:self.pos]

       self.token = Token("num", retrievedToken)
       
    def nextKwId(self):
        startPos = self.pos

        self.many(self.legits)
        # Gets a token
        retrievedToken = self.program[startPos:self.pos]
        # Checks if the token is a keyword
        if(retrievedToken in self.keywords):
            self.token = Token(retrievedToken, retrievedToken)
        else:
            self.token = Token("id", retrievedToken)

    # Gets an operator symbol
    def nextOp(self):           
        old = self.pos
        self.pos = old + 2
        if not self.done():
            self.lexeme = self.program[old: self.pos]
            if self.lexeme in self.operators:
                self.token = Token(self.lexeme)
                return
        self.pos = old + 1
        self.lexeme = self.program[old: self.pos]
        self.token = Token(self.lexeme)
           
        
    

        


    def next(self):
        """ generated source for method next """
        self.many(self.whitespace)
        if self.done():
            return False
        c = self.program[self.pos]
        ### fill in here (is c a digit? keyword? id?)
        if c in self.digits:
            self.nextNumber()
        elif c in self.legits:
            self.nextKwId()
        else:
            self.nextOp()
        return True

    def match(self, t):
        """ generated source for method match """
        if not t == self.curr():
            raise SyntaxException(self.pos, t, self.curr())
        self.next()

    def curr(self):
        """ generated source for method curr """
        if self.token == None:
            raise SyntaxException(self.pos, Token("ANY"), Token("EMPTY"))
        return self.token

    def position(self):
        """ generated source for method pos """
        return self.pos

if __name__ == '__main__':
    scanner = Scanner(sys.argv[1])
    while scanner.next():
        print(scanner.curr())

