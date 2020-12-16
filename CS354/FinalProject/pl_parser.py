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

    def parseMulop(self):
        """ generated source for method parseMulop """
        if self.curr() == Token("*"):
            self.match("*")
            return NodeMulop(self.pos(), "*")
        if self.curr() == Token("/"):
            self.match("/")
            return NodeMulop(self.pos(), "/")
        return None

    def parseRelop(self):
        """ generated source for method parseRelop """
        if self.curr() == Token("<"):
            self.match("<")
            return NodeRelop(self.pos(), "<")
        if self.curr() == Token("<="):
            self.match("<=")
            return NodeRelop(self.pos(), "<=")
        if self.curr() == Token(">"):
            self.match(">")
            return NodeRelop(self.pos(), ">")
        if self.curr() == Token(">="):
            self.match(">=")
            return NodeRelop(self.pos(), ">=")
        if self.curr() == Token("<>"):
            self.match("<>")
            return NodeRelop(self.pos(), "<>")
        if self.curr() == Token("=="):
            self.match("==")
            return NodeRelop(self.pos(), "==")
        return None

    def parseAddop(self):
        """ generated source for method parseAddop """
        if self.curr() == Token("+"):
            self.match("+")
            return NodeAddop(self.pos(), "+")
        if self.curr() == Token("-"):
            self.match("-")
            return NodeAddop(self.pos(), "-")
        return None

    def parseBoolExpr(self):
        """ generated source for method parseBoolExpr """
        exprl = self.parseExpr()
        relop = self.parseRelop()
        exprr = self.parseExpr()
        boolexpr = NodeBoolExpr(exprl, relop, exprr)
        return boolexpr

    def parseFact(self):
        """ generated source for method parseFact """
        if self.curr() == Token("("):
            self.match("(")
            expr = self.parseExpr()
            self.match(")")
            return NodeFactExpr(expr)
        if self.curr() == Token("-"):
            self.match("-")
            fact = self.parseFact()
            return NodeFactFact(fact)
        if(self.curr() == Token("*")):
            self.match("*")
            id = self.curr().lex()
            self.match("id")
            return NodeFactDeref(id)

        if self.curr() == Token("id"):
            nid = self.curr()
            self.match("id")

            if(self.curr() == Token("(")):
                self.match("(")
                expr = self.parseExpr()
                self.match(")")
                return NodeCall(self.pos(), nid.lex(), expr)
            return NodeFactId(self.pos(), nid.lex())
        num = self.curr()
        self.match("num")
        return NodeFactNum(num.lex())

    def parseTerm(self):
        """ generated source for method parseTerm """
        fact = self.parseFact()
        mulop = self.parseMulop()
        if mulop == None:
            return NodeTerm(fact, None, None)
        term = self.parseTerm()
        term.append(NodeTerm(fact, mulop, None))
        return term

    def parseExpr(self):
        """ generated source for method parseExpr """
        term = self.parseTerm()
        addop = self.parseAddop()
        if addop == None:
            return NodeExpr(term, None, None)
        expr = self.parseExpr()
        expr.append(NodeExpr(term, addop, None))
        return expr

    def parseAssn(self):
        """ generated source for method parseAssn """
        nid = self.curr()
        if(nid.lex() == "pointer"):
            self.match("pointer")
            id = self.curr().lex()
            self.match("id")
            self.match("=")
            checkDeref = self.curr().lex()
            if(checkDeref == '&'):
                self.match("&")
                addrID = self.curr().lex()
                expr = self.parseExpr()
                assn = NodeAssnAddr(id, addrID, expr)
                return assn
            elif(checkDeref == '*'):
                expr = self.parseExpr()
                assn = NodeAssnDeref(id, expr)
                return assn
            else:
                expr = self.parseExpr()
                assn = NodeAssn(id, expr)
                return assn
        elif(nid.lex() == "*"):
            self.match("*")
            id = self.curr().lex()
            self.match("id")
            self.match("=")
            checkDeref = self.curr().lex()
            if(checkDeref == '&'):
                self.match("&")
                addrID = self.curr().lex()
                expr = self.parseExpr()
                assn = NodeAssnAddr(id, addrID, expr)
                return assn
            elif(checkDeref == '*'):
                expr = self.parseExpr()
                assn = NodeAssnDeref(id, expr)
                return assn
            else:
                expr = self.parseExpr()
                assn = NodeAssnDeref(id, expr)
                return assn

        else:
            self.match("id")
            self.match("=")
            expr = self.parseExpr()
            assn = NodeAssn(nid.lex(), expr)
            return assn

    def parseRd(self):
        """ generated source for method parseRd """
        self.match("rd")
        nid = self.curr()
        self.match("id")
        rd = NodeRd(nid.lex())
        return rd

    def parseWr(self):
        """ generated source for method parseWr """
        self.match("wr")
        if(self.curr() == Token("pointer")):
            self.match("pointer")
            expr = self.parseExpr()
            wr = NodeWr(expr, 1)
            return wr
        else:
            expr = self.parseExpr()
            wr = NodeWr(expr, 0)
            return wr

    def parseIfElse(self):
        """ generated source for method parseIfElse """
        self.match("if")
        boolexpr = self.parseBoolExpr()
        self.match("then")
        thenstmt = self.parseStmt()
        elsestmt = None
        if self.curr() == Token("else"):
            self.match("else")
            elsestmt = self.parseStmt()
        ifelse = NodeIfElse(boolexpr, thenstmt, elsestmt)
        return ifelse

    def parseWhileDo(self):
        """ generated source for method parseWhileDo """
        self.match("while")
        boolexpr = self.parseBoolExpr()
        self.match("do")
        stmt = self.parseStmt()
        whiledo = NodeWhileDo(boolexpr, stmt)
        return whiledo

    def parseBegin(self):
        """ generated source for method parseBegin """
        self.match("begin")
        block = self.parseBlock()
        self.match("end")
        begin = NodeBegin(block)
        return begin

    def parseFunc(self):
        self.match("def")
        funcID = self.curr().lex()
        self.match("id")
        self.match("(")
        funcParam = self.curr().lex()
        self.match("id")
        self.match(")")
        self.match("=")
        expr = self.parseExpr()

        return NodeFunc(funcID, funcParam, expr)

    def parseStmt(self):
        """ generated source for method parseStmt """
        if self.curr() == Token("id") or self.curr() == Token("*") or self.curr() == Token("pointer"):
            assn = self.parseAssn()
            return NodeStmt(assn)
        if self.curr() == Token("rd"):
            rd = self.parseRd()
            return NodeStmt(rd)
        if self.curr() == Token("wr"):
            wr = self.parseWr()
            return NodeStmt(wr)
        if self.curr() == Token("if"):
            ifelse = self.parseIfElse()
            return NodeStmt(ifelse)
        if self.curr() == Token("while"):
            whiledo = self.parseWhileDo()
            return NodeStmt(whiledo)
        if self.curr() == Token("def"):
            func = self.parseFunc()
            return func
        begin = self.parseBegin()
        return NodeStmt(begin)

    def parseBlock(self):
        """ generated source for method parseBlock """
        stmt = self.parseStmt()
        rest = None
        if self.curr() == Token(";"):
            self.match(";")
            rest = self.parseBlock()
        block = NodeBlock(stmt, rest)
        return block

    def parseProg(self):
        """ generated source for method parseProg """
        block = self.parseBlock()
        if not self.scanner.done():
            raise SyntaxException(self.pos(), Token("EOF"), self.curr())
        prog = NodeProg(block)
        return prog

    def parse(self, program):
        """ generated source for method parse """
        self.scanner = Scanner(program)
        self.scanner.next()
        return self.parseProg()
