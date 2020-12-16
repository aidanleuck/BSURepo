class MemoryException(Exception):

    def __init__(self, msg):
        super(MemoryException, self).__init__()
        self.msg = msg

    def __str__(self):
        return (self.msg)
