import os
import threading
import datetime
import json
import string
from pathlib import Path

import pandas as pd
from spellchecker import SpellChecker


"""
    Utility file to help with managing documentation, such as spellchecking
"""



with open("docs/.spellcheck_whitelist") as f:
    allowed_words = json.load(f)



def index():    
    directory = "docs/source"

    valid_exts = ["rst", "md"]
    valid_files = []
    for subdir, dirs, files in os.walk(directory):
        for file in files:            
            for ext in valid_exts:
                filepath = Path(subdir).joinpath(file)
                if filepath.suffix == f".{ext}":
                    valid_files.append(filepath)
                    break

    return valid_files


def openfile_mt(file, mode, index):
    global fileptrs       
    fileptrs[index] = open(file, mode)
    

def cleanwords(content) -> list[str]:
    wordlist = content.split() # split on whitespace

    temp = []
    for word in wordlist: # Special character pass
        cleaned = []
        stack = [word.lower()]
        while len(stack) != 0: # Go through a bunch of passes cleaning each word as per the filters
            cap = stack.pop()

            # Word Filters
            bad_chars = set(string.punctuation).difference({'\''})
            broke = False
            for char in bad_chars:
                if char in cap:
                    stack += cap.split(char)
                    broke = True
                    break

            if broke:
                continue

            if cap == '':
                continue
            # end filters

            # Add cleaned word(s) to the list        
            cleaned.append(cap)

        temp += cleaned
        
    wordlist = temp
    temp = []

    return wordlist


fileptrs = []
def amalgamate_mt(filelist):
    threads = []    
    for i, file in enumerate(filelist):
        fileptrs.append(None)
        t = threading.Thread(target=openfile_mt, args=(file, "r", i))
        threads.append(t)

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    amalgam = ""
    dictionary = {}
    for ptr in fileptrs:
        content = ptr.read()
        words = cleanwords(content)
        for word in words:
            word = word.lower()            
            if word not in dictionary:
                dictionary[word] = {"freq": 1, "in": {ptr.name}}
            else:
                dictionary[word]["freq"] += 1
                dictionary[word]["in"].add(ptr.name)
            
        ptr.close()

    return amalgam, dictionary


corrections = []
def checkwords_mt(words, start_index):
    global corrections
    global allowed_words

    s = SpellChecker(distance=2)
    s.word_frequency.load_words(allowed_words)
    misspelled = s.unknown(words)
    
    for i, word in enumerate(misspelled):
        correction = (word, s.correction(word))
        corrections[i+start_index] = correction


def spellcheck():
    s = SpellChecker()

    filelist = index()
    content, metadata = amalgamate_mt(filelist)

    misspelled = s.unknown(list(metadata.keys()))

    skipped = 0
    for word in misspelled:
        correction = s.correction(word)
        if correction == None:
            skipped += 1
            continue


def spellcheck_mt(num_threads: int = -1):
    global corrections

    filelist = index()
    content, metadata = amalgamate_mt(filelist)

    wordlist = list(metadata.keys())
    if num_threads == -1:
        num_threads = len(wordlist)
    assert(0 < num_threads and num_threads <= len(wordlist))
    
    threads = []
    v = []
    subdiv = int(len(wordlist) / num_threads)
    for i in range(0, len(wordlist), subdiv):
        
        slicing = ""
        if (i + subdiv) >= len(wordlist):
            slicing = wordlist[i:]
        else:
            slicing = wordlist[i: (i + subdiv)]
        
        v += slicing
        corrections += [None] * len(slicing)        
        t = threading.Thread(target=checkwords_mt, args=(slicing, i))
        threads.append(t)

    print(f"starting {num_threads} threads")
    for t in threads:
        t.start()

    for t in threads:
        t.join()        
    print("rejoined threads")

    rows = []
    invalid = 0
    skipped = 0
    for i, correction in enumerate(corrections):
        if (correction == None):
            invalid += 1
            continue
        if (correction[1] == None):
            skipped += 1
            continue
        for file in metadata[correction[0]]["in"]:
            row = {
                "file": file,
                "found": correction[0],
                "suggested": correction[1],
            }
            rows.append(row)

    suggestions = pd.DataFrame(rows).set_index("file")
    print(f"Dictionary Size: {len(wordlist)}; Invalid words: {invalid}; Skipped words: {skipped}")
    print(suggestions)

start = datetime.datetime.now()
spellcheck_mt(10)
print(f"task took {datetime.datetime.now() - start}")

