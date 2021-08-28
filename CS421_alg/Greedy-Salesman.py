def greedyTravellingSalesman(startIndex, adjMatrix):

    numCities = len(adjMatrix)
    visited = [0, 0, 0, 0, 0]
    visited[startIndex] = 1
    routeLength = 0
    startRow = adjMatrix[startIndex]

    for i in range(numCities - 1, 0, -1):
        minimum = float('inf')
        minimumIndex = 0
        for j in range(numCities):
            if(i != j and startRow[j] < minimum and visited[j] != 1):
                minimumIndex = j
                minimum = startRow[j]

        startRow = adjMatrix[minimumIndex]
        routeLength = routeLength + minimum
        visited[minimumIndex] = 1

    return routeLength + startRow[startIndex]


Matrix = [[0 for x in range(5)] for y in range(5)]

Matrix[0][0] = 0
Matrix[0][1] = 1059
Matrix[0][2] = 1704
Matrix[0][3] = 328
Matrix[0][4] = 414
Matrix[1][0] = 1059
Matrix[1][1] = 0
Matrix[1][2] = 2527
Matrix[1][3] = 655
Matrix[1][4] = 1094
Matrix[2][0] = 1704
Matrix[2][1] = 2527
Matrix[2][2] = 0
Matrix[2][3] = 1562
Matrix[2][4] = 1268
Matrix[3][0] = 328
Matrix[3][1] = 655
Matrix[3][2] = 1562
Matrix[3][3] = 0
Matrix[3][4] = 294
Matrix[4][0] = 414
Matrix[4][1] = 1094
Matrix[4][2] = 1268
Matrix[4][3] = 294
Matrix[4][4] = 0

print(greedyTravellingSalesman(4, Matrix))
