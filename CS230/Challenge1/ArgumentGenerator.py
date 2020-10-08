import random
class ArgumentGenerator:
    
    manual = ""
    argument = ""
    subjectList = ["Mom", "Dad", "My sister", "Eric", "A random person", "A beggar", "A homeless man", "A soldier", "A priest", "A random celebrity", "A dog", "A cat", "A mouse", "A rat", "A local musician"]
    verbList = ["told us", "explained to us", "explained to me", "says", "told me", "preached to me", "mansplained to me", "scolded me"]

    internalList = [{"noun":"birds", "p":"can sing", "notP":"can't sing", "q": "know music theory", "notQ": "do not know music theory", "r":"can fly"}, 
    {"noun":"pigs", "p":"have ears", "notP":"do not have ears", "q":"can fly", "notQ": "can not fly", "r": "have a tail"}, 
    {"noun":"mice", "p": "have tails", "notP":"do not have tails", "q":"are smarter than humans", "notQ":"are not smarter than humans", "r":"are furry"}, 
    {"noun":"people", "p": "have feet" ,"notP":"do not have feet", "q":"can walk on water", "notQ":"can not walk on water", "r":"have legs"}, 
    {"noun":"spiders", "p": "have eight legs", "notP": "do not have eight legs", "q":"can jump 8 times higher than other bugs", "notQ":"can not jump 8 times higher than other bugs", "r":"have eight eyes"} , 
    {"noun" :"chocolate", "p":"is brown", "notP": "is not brown", "q":"is made of feces", "notQ":"is not made of feces", "r":"is made with sugar"}, 
    {"noun":"pizza" , "p":"is made of bread", "notP": "is not made of bread", "q":"is blue", "notQ":"is not blue", "r":"is brown"}, 
    {"noun":"dogs", "p":"have eyes", "notP":"do not have eyes", "q":"have wings", "notQ":"do not have wings", "r":"have paws"}, 
    {"noun":"cats", "p":"have noses", "notP":"do not have noses", "q":"can speak", "notQ": "can't speak", "r":"have whiskers"}, 
    {"noun":"gorillas" ,"p": "are big", "notP":"are not big", "q":"can sing", "notQ":"can not sing", "r":"are black"}, 
    {"noun":"people" , "p":"use phones", "notP":"do not use phones", "q":"can lay an egg", "notQ":"can not lay an egg", "r":"can smile"}, 
    {"noun":"flowers", "p":"have petals", "notP":"do not have petals", "q": "produce honey", "notQ": "do not produce honey", "r":"smell good"}, 
    {"noun":"priests", "p": "use a bible", "notP":"do not use a bible" ,"q":"know all", "notQ":"do not know all", "r":"wear robes"}, 
    {"noun":"sheep", "p":"have wool", "notP":"do not have wool", "q":"can not die", "notQ":"can die", "r":"smell bad"}, 
    {"noun":"bears", "p": "eat berries", "notP": "do not eat berries", "q":"are aquatic", "notQ":"are not aquatic", "r":"are black"}]

    begList = ["Paranormal activity is real because I have seen it with my own two eyes",
    "Everyone wants a new hat because it is the hottest hat ever made!",
    "humans have feet because god gave us feet",
    "God exists because it says so in the bible. The bible contains God's word",
    "Destiny is real. Everything in your life has already been decided for you", 
    "Bigfoot is real, I saw a documentary on the History Channel with proof",
    "The best phone on the planet is a Samsung because no one can make as good as a phone as them",
    "Mulan is a horrible movie because it did not earn a lot of money in the box office",
    ]


    def __init__(self, argumentType, inputManual):
        self.manual = inputManual
        self.argument = argumentType

    def generate(self):
        if self.argument == "1":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseTwoFromList(self.internalList)
                self.modusPonens(subject, verb, facts)
                
            else:
                subject = input("Please enter a noun: (speaker)")
                noun = input("Please enter a second noun: ")
                fact1 = input("Please enter a random fact about that noun: ")
                verb = self.chooseFromList(self.verbList)
                noun2 = input("Please enter a third noun: ")
                fact2 = input("Please enter a second random fact about that noun: ")

                facts = [{"noun": noun, "p":fact1}, {"noun":noun2, "q":fact2}]

                self.modusPonens(subject, verb, facts)
        elif self.argument == "2":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseFromList(self.internalList)

                self.modusTollens(subject, verb, facts)
            else:
                subject = input("Choose a noun: ")
                noun = input("Choose a second noun: ")
                verb = self.chooseFromList(self.verbList)
                fact1 = input("Choose a fact about the noun: ")
                notFact1 = input("Please write the opposite of the previously inserted fact: ")
                fact2 = input("Choose a second fact about the noun: ")
                notFact2 = input("Please write the opposite of the previously inserted fact: ")

                facts = {"noun":noun,"p": fact1, "notP":notFact1, "q":fact2, "notQ": notFact2}

                self.modusTollens(subject, verb, facts)
        elif self.argument == "3":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseFromList(self.internalList)

                self.processElim(subject, verb, facts)

        
            else:
                subject = input("Choose a speaker (noun): ")
                verb = self.chooseFromList(self.verbList)
                noun = input("Write a noun:")
                factF = input("Write a false fact about the noun: ")
                notFactF = input("Write the opposite of the given fact: ")
                factT = input("Write a true fact about the noun: ")
                facts = {"noun": noun, "q":factF, "notQ": notFactF, "p": factT}
                self.processElim(subject, verb, facts)

        elif self.argument == "4":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseFromList(self.internalList)

                self.chainRule(subject, verb, facts)
            else:
                subject = input("Choose a speaker (noun): ")
                verb = self.chooseFromList(self.verbList)
                noun = input("Write a noun: ")
                fact1 = input("Give a random fact about the noun: ")
                fact2= input("Give another random fact about the noun: ")
                fact3 = input("Give another random fact about the noun: ")

                facts = {"noun": noun, "p":fact1, "q": fact2, "r": fact3}

                self.chainRule(subject, verb, facts)

        elif self.argument == "5":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseFromList(self.internalList)

                self.affirmCons(subject, verb, facts)

            else:
                subject = input("Choose a noun (speaker): ")
                verb = self.chooseFromList(self.verbList)
                noun = input("Choose a noun: ")
                fact1 = input("Choose a fact about the noun: ")
                fact2 = input("Choose a second fact about the noun: ")

                facts = {"noun": noun, "p": fact1, "q": fact2}
                self.affirmCons(subject, verb, facts)

        elif self.argument == "6":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)
                facts = self.chooseFromList(self.internalList)

                self.denyAnt(subject, verb, facts)
            else:
                subject = input("Choose a noun (Speaker): ")
                verb = self.chooseFromList(self.verbList)
                noun = input("Choose a noun: ")
                fact1 = input("Write a fact about the " + noun + ": ")
                notfact2 = input("Write the opposite fact given previously: ")
                fact3 = input("Write a fact about the " + noun + ": ")
                notfact4 = input("Write the opposite fact given previously: ")

                facts = {"noun": noun, "p":fact1, "notP":notfact2, "q":fact3, "notQ":notfact4}
                self.denyAnt(subject, verb, facts)




        elif self.argument == "7":
            if self.manual == "1":
                subject = self.chooseFromList(self.subjectList)
                verb = self.chooseFromList(self.verbList)

                phrase = self.chooseFromList(self.begList)

                self.begQuest(subject, verb, phrase)
            else:
                subject = input("Choose a speaker (Noun): ")
                verb = self.chooseFromList(self.verbList)

                phrase2 = input("Write a true claim about a noun: ")
                phrase3 = input("Give an explanation about why that fact is true: ")

                phrase = phrase2 + " because " +phrase3

                self.begQuest(subject,verb,phrase)
                

            

    def chooseTwoFromList(self, inputList):
        listLength = len(inputList)
        randIndex = random.randint(0, listLength - 1)
        randIndex2 = random.randint(0, listLength - 1)

        if(randIndex == randIndex2):
            self.chooseTwoFromList(inputList)
        else:
            item1 = inputList[randIndex]
            item2 = inputList[randIndex2]
            return (item1, item2)

    def chooseFromList(self, inputList):
        listLength = len(inputList)
        randIndex = random.randint(0, listLength - 1)
    
        internalList = inputList[randIndex]
        return internalList

    def modusPonens(self, subject, verb, facts):
       print(subject + " " + verb + " if " + facts[0]["noun"] + " " + facts[0]["p"] +  " then " + facts[1]["noun"] + " "+ facts[1]["q"] + ". " + facts[0]["noun"] + " "+facts[0]["p"] + ", therefore" + " " + facts[1]["noun"]+ " "+facts[1]["q"])
           
            
    def modusTollens(self, subject, verb, facts):
        print(subject + " " + verb + " if " + facts["noun"] + " "+ facts["p"] + " then " + facts["noun"] + " " + facts["q"] + ". " + facts["noun"] + " "+facts["notQ"] + ". Therefore, " + facts["noun"]+ " "+ facts["notP"] )
    def processElim(self, subject, verb, facts):
        print(subject + " " + verb + " either "+ facts["noun"] + " " + facts["q"] + " or " + facts["noun"] + " " +facts["p"] + ". " + facts["noun"] + " " + facts["notQ"] + " so " + facts["noun"] + " " +facts["p"] )
    def chainRule(self, subject, verb, facts):
        print(subject + " " + verb + " if " + facts["noun"] + " " + facts["p"] + " then " + facts["noun"] + " " + facts["q"] + " and if " +facts["noun"] + " " + facts["q"] + " then " +facts["noun"] + " " + facts["r"] + " (using chain rule we can say) if " + facts["noun"] + " " + facts["p"] + " then " + facts["noun"] + " "+facts["r"] )
    def affirmCons(self, subject, verb, facts): 
        print(subject + " " + verb + " if " + facts["noun"] + " " + facts["p"] + " then " + facts["noun"] + " " + facts["q"] +". Therefore, if " + facts["noun"] + " " + facts["q"] + " then " + facts["noun"] + " " + facts["p"])
    def denyAnt(self, subject, verb, facts):
        print(subject + " " + verb + " if " + facts["noun"] + " " + facts["p"] + " then " + facts["noun"] + " " + facts["q"] + ". Therefore, if " + facts["noun"] + " " + facts["notP"] + " then " + facts["noun"] + " " + facts["notQ"])
    def begQuest(self, subject, verb, phrase):
        print(subject + " " + verb + " " + phrase)
    
