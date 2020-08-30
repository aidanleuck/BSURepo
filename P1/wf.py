import string 

file = open("Gettysburg-Address.txt")
speechDict = {}

for line in file:
    words = line.split()
    for x in words:
        removePunc = x.translate(str.maketrans('', '', string.punctuation))
        finalWord = removePunc.lower()
        if(finalWord != ''):
            if finalWord in speechDict:
                speechDict[finalWord] = speechDict[finalWord] + 1.0
            else:
                speechDict[finalWord] = 1.0

reverseSpeechDict = dict(sorted(speechDict.items(), key = lambda keyVal:keyVal[1], reverse = True))


with open("Gettysburg-Address.output.txt", "w") as fileOut:
    for i in reverseSpeechDict:
        print(i + " " + str(reverseSpeechDict[i]), file=fileOut)
