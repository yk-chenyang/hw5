#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generate(std::set<std::string>& result, int sizeWord, const std::string& in, std::string currentStr, std::string floatingRemainder, int index, const std::set<std::string>& dictSameLength, const std::set<std::string>& prefixDict);
// this helper function essentially generates all possible combinations of the word shaped by the fixed letters given and the dataset of floating letters at the free locations

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> result;
    int sizeWord = in.size(); //length of the word
    std::string floatingRemainder = floating; //make a copy cuz we may want to change it

    //two pruning tools

    std::set<std::string> dictSameLength;
    for(std::set<std::string>::const_iterator it=dict.begin();it!=dict.end();++it){
      if((*it).size()==in.size()){
        dictSameLength.insert(*it); //we build a new (much shorter dictionary with the same length as the desired word)
      }
    }

    std::set<std::string> prefixDict;
    for(std::set<std::string>::const_iterator it=dictSameLength.begin();it!=dictSameLength.end();++it){
      int tempSize = (*it).size();
      for(int i=1; i<=tempSize;i++){
        prefixDict.insert((*it).substr(0,i)); //prefixes with 1 letter, 2 letters, 3... essentially all prefixes of all possible words 
      }
    }
    generate(result, sizeWord, in, "", floatingRemainder, 0, dictSameLength, prefixDict); //0-indexed; we passed in an empty string to be filled and put in result
    return result; //should have been processed already
}

// Define any helper functions here
void generate(std::set<std::string>& result, int sizeWord, const std::string& in, std::string currentStr, std::string floatingRemainder, int index, const std::set<std::string>& dictSameLength, const std::set<std::string>& prefixDict){
    int spotsLeft = sizeWord - index; //how many spots are left, from now on, for letters to be filled in
    int floatingLeft = floatingRemainder.size();
    if(spotsLeft < floatingLeft){
      return; //early pruning if we have no spaces for all the floating letters to be filled 
    }
    if(index == sizeWord){ //we've reached the end
      if(floatingLeft==0 && dictSameLength.find(currentStr)!=dictSameLength.end()){ //we're done with all the floatings, and the word generated satisfies the dictionary
        result.insert(currentStr); //put it in result
        return;
      }
      else{
        return; //the word does not satisfy requirement. Return/backtrack
      }
    }
    //from now on, either fixed or try all
    char c = in[index]; //our current letter to be examined
    if(c!='-'){ //fixed
      if(prefixDict.find(currentStr+c)==prefixDict.end()){ //nothing starts with this
        return; //prune early
      }
      generate(result,sizeWord,in,currentStr+c,floatingRemainder,index+1,dictSameLength,prefixDict); //add the current letter to currentStr because we have no other choices but to use the fixed letters
    }
    else{ //not determined yet
      for(char newChar = 'a'; newChar <= 'z'; newChar++){ //iterate through all possible letters
        if(prefixDict.find(currentStr+newChar)==prefixDict.end()){ //nothing starts with this
          continue; //prune this branch early
        }
        std::string temp = floatingRemainder;
        for(int i=0; i<floatingLeft; i++){
          if(newChar==floatingRemainder[i]){ //if this is the letter that we need for floating
            temp = floatingRemainder.substr(0,i) + floatingRemainder.substr(i+1,(floatingLeft-(i+1))); //delete this floating letter because we've used it. we delete by setting the new floatingRemainder to a temp string, and then pass the editted string to the next recursive case
          }
        }
        generate(result,sizeWord,in,currentStr+newChar,temp,index+1,dictSameLength,prefixDict); //recurse
      }
    }
}