import math


def coinAlgorithm(denominations, value):
    denominationsCount = [0] * len(denominations)

    for i in range(len(denominations)):
        count = 0
        if value < .009:
            break
        while(value >= denominations[i]):
            value = value - denominations[i]
            count = count + 1

        denominationsCount[i] = count

    if value < .009:
        return denominationsCount
    else:
        return "No solution"


den = (.25, .15, .10, .05, .02, .01)

print(coinAlgorithm(den, .63))
