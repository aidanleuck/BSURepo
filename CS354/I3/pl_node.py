#!/usr/bin/env python
""" generated source for module Node """

#  (C) 2013 Jim Buffenbarger
#  All rights reserved.

class Node(object):
    """ generated source for class Node """
    pos = 0

    def __str__(self):
        """ generated source for method toString """
        result = ""
        result += str(self.__class__.__name__)
        result += " ( "
        fields = self.__dict__
        for field in fields:
            result += "  "
            result += str(field)
            result += str(": ")
            result += str(fields[field])
        result += str(" ) ")
        return result

class NodeFact(Node):
    def __init__(self, fact):
        self.fact = fact


class NodeAssn(Node):
    def __init__(self, id, expr):
        self.id = id
        self.expr = expr
class NodeBlock(Node):
    def __init__(self, stmt, block_two):
        self.stmt = stmt
        self.block_two = block_two

class NodeTerm(Node):
    def __init__(self, fact, mulop, term):
        self.fact = fact
        self.mulop = mulop
        self.term = term

    def append(self, term):
        if self.term is None:
            self.mulop = term.mulop
            self.term = term
            term.mulop = None

        else:
            self.term.append(term)
            
class NodeExpr(Node):

    def __init__(self, term, addop, expr):

        self.term = term
        self.addop = addop
        self.expr = expr

    def append(self, expr):
        if self.expr is None:
            self.addop = expr.addop
            self.expr = expr
            expr.addop = None
        else:
            self.expr.append(expr)   

class NodeStmt(Node):
    def __init__(self, assn):
        self.assn = assn
class NodeWr(Node):
    def __init__(self, expr):
        self.expr = expr