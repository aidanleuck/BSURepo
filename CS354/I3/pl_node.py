#!/usr/bin/env python
""" generated source for module Node """

#  (C) 2013 Jim Buffenbarger
#  All rights reserved.
from pl_evalexception import EvalException
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
    """
    Parent eval function for node class, overriden by child nodes
    """
    def eval(self, env):
         raise EvalException(self.pos, "cannot eval() node!")
        
        
        
"""
Contains information about a fact node
A fact can have an id, number or be an expr
Only can have one at a time.

id - character set by parser
num - number
expr - expression
"""
class NodeFact(Node):
    def __init__(self, id, num, expr):
        self.id = id
        self.num = num
        self.expr = expr
    """
    Evaluates a node fact
    env - environment variable
    """
    def eval(self, env):
        # checks if id is set, if so gets value from environment
        if(self.id is not None):
            idVal = env.get(self.pos, self.id)
            return idVal
        # Checks if num is set if so returns the num
        elif(self.num is not None):
            return self.num
        # Checks if expr is set if so returns the evaluated expression
        elif(self.expr is not None):
            return self.expr.eval(env)
"""
Holds information about the add node
+ : Signal to add expr and term together
- : Signal to subtract expression from term
"""
class NodeAddOp(Node):
    def __init__(self, addop):
        super(NodeAddOp, self).__init__()
        self.addop = addop
    """
    Evaluates a addop node
    Adds or subtracts termVal and exprVal

    """    
    def eval(self, termVal, exprVal, env):
        if(self.addop == "+"):
            return termVal + exprVal
        else:
            return exprVal - termVal
"""
Holds information about a mulop node
* - Multiply
/ - Divide
"""
class NodeMulOp(Node):
    def __init__(self, mulop):
        super(NodeMulOp, self).__init__()
        self.mulop = mulop
    """
    Evaluates a mulop node, if / divides numbers if * multiplies numbers
    """
    def eval(self, factVal, termVal, env):
        if(self.mulop == "*"):
            return termVal * factVal
        else:
            return termVal / factVal
""" Holds information on an assignment node"""
class NodeAssn(Node):
    def __init__(self, id, expr):
        super(NodeAssn, self).__init__()
        self.id = id
        self.expr = expr
        """Stores id and the result of evaluated expression into the environment"""
    def eval(self, env):
        return env.put(self.id, self.expr.eval(env))
""" Holds information about a node block """
class NodeBlock(Node):
    def __init__(self, stmt, block_two):
        super(NodeBlock, self).__init__()
        self.stmt = stmt
        self.block_two = block_two
    """ Evaluates first and second blocks values"""
    def eval(self, env):    
        stmt1= self.stmt.eval(env)

        if(self.block_two is not None):
            self.block_two.eval(env)
            
        return stmt1       

"""Stores information about a node term """
class NodeTerm(Node):
    def __init__(self, fact, mulop, term):
        super(NodeTerm, self).__init__()
        self.fact = fact
        self.mulop = mulop
        self.term = term
    """ If there is a secondary term, option to append the term onto the node"""
    def append(self, term):
        if self.term is None:
            self.mulop = term.mulop
            self.term = term
            term.mulop = None

        else:
            self.term.append(term)
    """Evaluates a term"""
    def eval(self, env):
        factVal = 0
        termVal = 0
        # If a term has a second term then evaluate the term and convert the value to an integer
        if(self.term is not None):
            termVal = int(self.term.eval(env))
        # If term has a fact then evaluate that fact and store the result in factVal
        if(self.fact is not None):
            factVal= int(self.fact.eval(env))
        # If a term has a mulop then evaluate the result of factVal * termVal
        if(self.mulop is not None):
            return self.mulop.eval(factVal, termVal, env)
        return factVal
    """ Holds information about an expression"""           
class NodeExpr(Node):

    def __init__(self, term, addop, expr):
        super(NodeExpr, self).__init__()
        self.term = term
        self.addop = addop
        self.expr = expr
    # Appends a second expression into the node
    def append(self, expr):
        if self.expr is None:
            self.addop = expr.addop
            self.expr = expr
            expr.addop = None
        else:
            self.expr.append(expr)   
    # Evaluates an expression
    def eval(self, env):
        # If the expression has a term evaluate it and store result
        if(self.term is not None):
             termVal = int(self.term.eval(env))
        # If expression has a second expression then evaluate and store result
        if(self.expr is not None):
             exprVal =  int(self.expr.eval(env))
        # If expression has an addop then + or - two values
        if(self.addop is not None):
            return self.addop.eval(termVal, exprVal, env)
        return termVal
""" Holds information about a statememt """
class NodeStmt(Node):
    def __init__(self, assn, wr):
        super(NodeStmt, self).__init__()
        self.assn = assn
        self.wr = wr
    # Evaluates a statement
    def eval(self, env):
        # Checks if this is an assignment if it is then evaluate the assignment
        if(self.assn is not None):
            return self.assn.eval(env)
        # If not an assignment should be a wr.
        else:
            return self.wr.eval(env)
""" Holds information about a wr keyword"""
class NodeWr(Node):
    def __init__(self, expr):
        super(NodeWr, self).__init__()
        self.expr = expr
    # prints the result of the expression
    def eval(self, env):
        print(self.expr.eval(env))
