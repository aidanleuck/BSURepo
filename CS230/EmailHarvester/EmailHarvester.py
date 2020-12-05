import urllib.request
import sys
import re

# Finds and prints address once input has been sanitized


def plainEmail(scrapedString):
    regexStr = r'[\w][\w._%+-]+@[\w\.-]+\.[\w\.-]+'
    addresses = set(re.findall(regexStr, scrapedString))

    return addresses


# Advanced technique to check if there are embedded information in html attributes
def advancedSanitization(scrapedString):

    scrapedString = scrapedString.replace(" ", "")
    regexReplaceAt = r'(\[|\(|\")at(\]|\)|\")'
    scrapedString = re.sub(regexReplaceAt, "\"@\"", scrapedString)
    # Removes most html tags except attributes
    regexRemoveTags = r'</[\w]+>|<\w+>|<\w+/>'
    # Removes unnecessary attribute information
    regexRemoveAttributes = r'<?\"?\w+=\"([\w.`~!#$%^&*()_-a+=]+\")?/?>?'

    # Removes Html tags
    scrapedString = re.sub(regexRemoveTags, "", scrapedString)
    # Removes attributes and returns the new sanitized string
    return re.sub(regexRemoveAttributes, "", scrapedString)

    # Removes extraneous information and find cases where there are (at's)


def simpleSanitizeInput(scrapedString):
    # Remove all spaces
    scrapedString = scrapedString.replace(" ", "")

    # Replaces instances of at
    regexReplaceAt = r'(\[|\(|\")at(\]|\)|\")'
    # Any time we have an (at) [at] or "at" replace it with "@"
    return re.sub(regexReplaceAt, "@", scrapedString)


if __name__ == "__main__":
    if (len(sys.argv) == 2):
        # Gets HTML data
        html = urllib.request.urlopen(sys.argv[1])
        htmlData = html.read()

        # Converts to string
        scrapedString = htmlData.decode("utf8")

        # Sanitize and print input
        simpleResult = simpleSanitizeInput(scrapedString)
        addresses = plainEmail(simpleResult)
        if(len(addresses) == 0):
            scrapedString = advancedSanitization(scrapedString)
            addresses = plainEmail(scrapedString)
            for address in addresses:
                print(address)

            if(len(addresses) == 0):
                print("No addresses were found")
        else:
            for address in addresses:
                print(address)

    else:
        # Error messages if input is incorrect
        sys.stderr.write("Error: you did not provide valid arguments")
        sys.stderr.write("\nUsage: EmailHarvester.py URL")
