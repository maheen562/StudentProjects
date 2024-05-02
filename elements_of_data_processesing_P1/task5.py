import json,csv
import pandas as pd
import matplotlib.pyplot as plt
   
def task5():
    rating = []
    with open('/course/data/a1/reviews/HealthStory.json') as json_file:
        review_file = json.load(json_file)
    
    for review_info in review_file:
        rating.append({
                'rating' : review_info["rating"]
        })

    df_rating = pd.DataFrame(rating)

    with open('/course/data/a1/engagements/HealthStory.json') as json_file:
        tweet_file = json.load(json_file)
    
    total_tweets = []
    tweets_per_review = []
    news_id = []

    for each_review in tweet_file:
        news_id.append(each_review)
    
    
    for each_news_id in news_id:
        for each_tweet in tweet_file[each_news_id]["tweets"]:
            tweets_per_review.append(each_tweet)
        for each_reply in tweet_file[each_news_id]["replies"]:
            tweets_per_review.append(each_reply)
        for each_retweet in tweet_file[each_news_id]["retweets"]:
            tweets_per_review.append(each_retweet)
        
        tweets_per_review = list(dict.fromkeys(tweets_per_review))
        total_tweets.append({
            "tweets" : len(tweets_per_review)
           })
        tweets_per_review = []

    total_tweets = pd.DataFrame(total_tweets)
    combined_df = [df_rating,total_tweets]
    result = pd.concat(combined_df, axis=1, join="inner")
    avg_rating = result.groupby("rating")["tweets"].mean().round(2)

    avg_rating.plot(x ='rating', y='Average number of tweets and retweets', kind = 'bar')
    plt.savefig("task5.png")

    
    return
