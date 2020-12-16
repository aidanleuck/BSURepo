import ctypes
from memoryException import MemoryException


class MemorySimulator(object):
    def __init__(self):
        self.memory = {}

    def putValue(self, address, addressID):
        self.memory[address] = addressID
        return address

    def getValue(self, address):
        if(address in self.memory):
            return self.memory[address]

    def dereference(self, address, env):
        if(address in self.memory):
            derefID = self.memory[address]
            return env.get(0, derefID)

        raise MemoryException(
            "No pointer exists for address " + hex(int(address)))


if __name__ == '__main__':
    None
