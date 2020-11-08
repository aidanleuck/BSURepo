#!/usr/bin/env python
""" generated source for module Environment """
#  (C) 2013 Jim Buffenbarger
#  All rights reserved.
from pl_evalexception import EvalException

class Environment(object):
    def __init__(self):
        self.environmentDict = {} # Dictionary to store environment variables
        
    # Puts key = var and val = value into dicitonary
    def put(self, var, val):
        self.environmentDict[var] = val
    # Gets a value in dictionary, if that value is not in dictionary throw EvalException
    def get(self, pos, var):
        if(var not in self.environmentDict):
            raise EvalException(pos, "There is no instance of " + var + " in the environment")
        val = self.environmentDict[var]
        
        return val
