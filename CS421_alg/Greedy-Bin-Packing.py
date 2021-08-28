def GreedyBinPacking(W, C):
    binCount = 0
    currLength = len(W)
    d = {}
    while(currLength > 0):
        currCapacity = C
     

       
        for w in W:
            if(w <= currCapacity and not d.__contains__(w)):
                currCapacity -= w
                d[w] = w
                currLength = currLength - 1

                

        binCount = binCount + 1
    
    return binCount

W = [2,5,4,7,1,3,8]
GreedyBinPacking(W, 10)