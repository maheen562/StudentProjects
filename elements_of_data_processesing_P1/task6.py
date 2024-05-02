import json,csv,re,os,nltk
import pandas as pd
from nltk.corpus import stopwords
import time

def task6():
    

    # Step - Access direcotry to build list of files
    article_files_loc = '/course/data/a1/content/HealthStory'
    article_files = [pos_json for pos_json in os.listdir(article_files_loc) if pos_json.endswith('.json')]
    insert_dictionary(article_files_loc,article_files)

def insert_dictionary(article_files_loc,article_files):
    start_time = time.time()
    word_dictionay={}
    for each_article in article_files:
        build_dictionary_from_file(article_files_loc,each_article, word_dictionay)
    
    with open("task6.json", "w") as outfile:
        json.dump(word_dictionay, outfile,sort_keys=True) 

def build_dictionary_from_file(directory,each_article, word_dictionay):
    
    ascii_letters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    dict_words_in_file = []
    articles = []

    with open(directory + '/' + each_article) as json_file:
        article_file_info = json.load(json_file)
    json_file.close()
    text = article_file_info["text"]
        
    no_punct_text = re.sub(r'[^A-Za-z]+',' ',text)
    no_spaces = re.sub(r'\n|\t' , ' ',no_punct_text)
    lowercase_text = no_spaces.lower()
    tokens = nltk.word_tokenize(lowercase_text)
    stop_words = set(stopwords.words('english'))
    #print(stop_words)
    no_stopwords = [w for w in tokens if not w in stop_words]
    no_stopwords = [m for m in no_stopwords if not m in ascii_letters]
    # remove english words
    # remove single character words


    for each_word in no_stopwords:
        if each_word in word_dictionay:
            file_list_collection = word_dictionay[each_word]            
            if not(each_article in file_list_collection):
                file_list_collection.append(each_article)
                word_dictionay[each_word]=file_list_collection
        else:
            word_dictionay[each_word]=[(each_article)]

        word_dictionay[each_word] = re.findall(r'story_reviews_\d{5}',str(word_dictionay[each_word]))
        word_dictionay[each_word] = list(dict.fromkeys(word_dictionay[each_word]))
        word_dictionay[each_word].sort()
        
    return

