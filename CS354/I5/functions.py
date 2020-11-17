class Function(object):
    def __init__(self, param, expr):
        self.param = param
        self.expr = expr
    def call(self, expr, env):
        return env.put(self.param, self.expr.eval(env))
    
    