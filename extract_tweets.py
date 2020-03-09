import tweepy
import re
import sys
import os
from os import path

# Fill the X's with the credentials obtained
#FILL IN YOUR OWN KEYS HERE!!!!!!!! 
#You need to create a twitter developer account to obtain these!
consumer_key = "" 
consumer_secret = ""
access_key = ""
access_secret = ""
  

# Function to extract tweets. Takes in a username and sets up authenticatino and tweet pulling
def get_tweets(username): 
          
        # Authorization to consumer key and consumer secret 
        auth = tweepy.OAuthHandler(consumer_key, consumer_secret) 
  
        # Access to user's access key and access secret 
        auth.set_access_token(access_key, access_secret) 
  
        # Calling api 
        api = tweepy.API(auth) 
  
        # Number tweets to be extracted, can be changed depending on how large you want data to be. The larger the number of tweets, the more options in the markov chains
        number_of_tweets=3200
        #Pull tweets from timeline, excludes retweets and uses extended mode to retrieve full text of tweets
        tweets = api.user_timeline(screen_name=username, count = number_of_tweets, tweet_mode = "extended", include_rts = False) 
  
        # Empty Array to load tweets into
        tmp=[]  
  
        # create array of tweet information: username,  
        # tweet id, date/time, text 
        tweets_for_csv = [tweet.full_text for tweet in tweets] # CSV file created  
        for j in tweets_for_csv: 
  
            # Appending tweets to the empty array tmp 
            tmp.append(j)  
  
        # Printing the tweets 
        #print(tmp)
       # for i in tmp:
        #    print(i)
        return tmp

#Function that takes in text (tweet) and cleans it by removing newlines, urls, and leading or ending whitespace. Also filters out empty lines.
def cleanTweet(tweet):
    cleanTweet = []
    for i in tweet:
        cleanURL = re.sub(r"http\S+", "", i)
        clean = re.sub(r"@\S+", "", cleanURL)
        clean = clean.strip()
        clean = clean.replace("\n"," ")
        if(not clean):
            continue
        cleanTweet.append(clean)
    return cleanTweet

#Function that takes in text and removes unnecessary characters and newlines
def cleanText(path):
    file = open(path, "r")
    cleanText = []
    for line in file:
        clean = re.sub(r"@\S+", "", line)
        clean = clean.strip()
        clean = clean.replace("\n"," ")
        if(not clean):
            continue
        cleanText.append(clean)
    return cleanText

#Function that takes cleaned tweets or texts and writes them line by line to a file to be used by the markov_chain.cpp file
def writeToFile(array, name):
    if(len(array) == 0):
        writeFile = open("invalid.txt", "w")
        writeFile.close()
        return
    
    if(name[0] == "@"):
        filename = "text" + name + ".txt"
    else:
        filename = "markov.txt"
    if(path.exists("markov.txt")):
        os.remove("markov.txt")
    if(path.exists(filename)):
        os.remove(filename)

    writeFile = open(filename, "w+")
    for i in array:
        phrase = i + "\n"
        writeFile.write(phrase)
    writeFile.close()
  
# Driver code 
if __name__ == '__main__': 
  
    # Here goes the twitter handle for the user 
    # whose tweets are to be extracted. 
    
    #If there is an empty, invalid file. Remove.
    if(path.exists("invalid.txt")):
        os.remove("invalid.txt")
    
    #Text source (tweet or filepath) to be taken in
    source = str(sys.argv[1])

    #If text source is a tweet, send to API to retrieve tweet.
    if(source[0] == "@"):
        source = source.strip('\'')
        try:
            rawTweets = get_tweets(source)
            finalTweets = cleanTweet(rawTweets)
        except:
            print("Invalid Twitter handle")
            finalTweets = []
        writeToFile(finalTweets, source)
    #If text source is a file, go open filepath to retrieve text
    else:
        try:
            textArray = cleanText(source)
        except:
            print("Invalid file path")
            textArray = []
        writeToFile(textArray, source)


    #rawTweets = get_tweets(handle)
    #finalTweets = cleanText(rawTweets)
    #for i in finalTweets:
    #    print(i)
