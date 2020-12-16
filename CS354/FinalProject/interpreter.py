#!/usr/bin/env python
""" generated source for module Interpreter """
#  (C) 2013 Jim Buffenbarger
#  All rights reserved.
from pl_parser import Parser
from pl_environment import Environment
from pl_syntaxexception import SyntaxException
from pl_evalexception import EvalException
from memoryAccess import MemorySimulator
from memoryException import MemoryException


class Interpreter(object):
    """ generated source for class Interpreter """
    @classmethod
    def main(cls, args):
        """ generated source for method main """
        parser = Parser()
        env = Environment()
        memory = MemorySimulator()
        for prog in args:
            try:
                node = parser.parse(prog)
                node.eval(env, memory)
            except SyntaxException as e:
                print(e)
            except EvalException as e:
                print(e)
            except MemoryException as e:
                print(e)


if __name__ == '__main__':
    import sys
    Interpreter.main(sys.argv[1:])
