def LCS(stringArray1, stringArray2):
    stringNum = len(stringArray1)
    C = [[0 for x in range(stringNum + 1)] for y in range(stringNum + 1)]
    for i in range(len(C)):
        for j in range(len(C)):
            if(i == 0 or j == 0):
                C[i][j] = 0
            else:

                if(stringArray1[i - 1] == stringArray2[j - 1]):
                    C[i][j] = C[i-1][j-1] + 1
                else:
                    C[i][j] = max(C[i][j-1], C[i-1][j])
    return C[stringNum][stringNum]


stringArray1 = ["Akunstu", "Nukak", "Wauja", "Mbya",
                "Baniwa", "Kubeo", "Piraha", "Witoto", "Tupi", "Mura"]
stringArray2 = ["Baniwa", "Akunstu", "Nukak", "Witoto",
                "Kubeo", "Mura", "Tupi", "Piraha", "Wauja", "Mbya"]

#LCS(stringArray1, stringArray2)

print(LCS(stringArray1, stringArray2))
