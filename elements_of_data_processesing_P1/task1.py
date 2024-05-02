"""
This task outputs a json file telling us the number of articles, reviews, and tweets in our sample
"""
import json
import os
import pandas as pd


def task1():

    article_files_loc = '/course/data/a1/content/HealthStory'
    article_files = [pos_json for pos_json in os.listdir(article_files_loc) if pos_json.endswith('.json')]
    
    X = len(article_files)    
    
    review_file = pd.read_json(r'/course/data/a1/reviews/HealthStory.json') 
    Y = len(review_file)
    
    news_id = []
    tweets_per_review = []
    with open('/course/data/a1/engagements/HealthStory.json') as json_file:
        tweet_file = json.load(json_file)
    for each_review in tweet_file:
        news_id.append(each_review)
    
    
    for each_news_id in news_id:
        for each_tweet in tweet_file[each_news_id]["tweets"]:
            tweets_per_review.append(each_tweet)
        for each_reply in tweet_file[each_news_id]["replies"]:
            tweets_per_review.append(each_reply)
        for each_retweet in tweet_file[each_news_id]["retweets"]:
            tweets_per_review.append(each_retweet)
        
    total_tweets = list(dict.fromkeys(tweets_per_review))    
    Z = len(total_tweets)
    

    dictionary = {
        "Total number of articles": X,
        "Total number of reviews": Y,
        "Total number of tweets": Z
    }
    
    with open("task1.json", "w") as outfile:
        json.dump(dictionary,outfile)

    return
