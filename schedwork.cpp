#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool fillWorkers(const AvailabilityMatrix& avail, size_t n, size_t k, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, size_t idx, vector<size_t>& shiftsUsed, vector<vector<bool>>& alreadyInDay);
//this is the backtracking function for filling one spot with a worker

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t n = avail.size();
    size_t k = (avail[0]).size(); //setup n and k

    sched = DailySchedule(n, vector<Worker_T>(dailyNeed,INVALID_ID)); // I previously used nested loop to initialize, but then I segfaulted cuz after clear() sched has nothing. I finally figured out that I can use the same initializing trick like the two vector<> a (b,c) below to do the work
    vector<size_t> shiftsUsed (k,0); //set up a vector to record the status of usage of shifts for each worker
    vector<vector<bool>> alreadyInDay (n,vector<bool>(k,false)); //tracking if for a day (a row) a worker has already been used 

    if(fillWorkers(avail, n, k, dailyNeed, maxShifts, sched, 0, shiftsUsed, alreadyInDay)==true){
      return true; //processed successfully
    }
    return false;
}

bool fillWorkers(const AvailabilityMatrix& avail, size_t n, size_t k, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, size_t idx, vector<size_t>& shiftsUsed, vector<vector<bool>>& alreadyInDay){
    if(idx==n*dailyNeed){
      return true; //we've finished successfully
    }
    size_t idxRow = idx/dailyNeed;
    size_t idxCol = idx%dailyNeed; //small math trick to determine the row and col positions

    for(size_t i=0; i<k; i++){ //iterate through all workers
      if(avail[idxRow][i]==true && shiftsUsed[i]<maxShifts && alreadyInDay[idxRow][i]==false){ //good for putting the value in
        sched[idxRow][idxCol] = i;
        shiftsUsed[i]++;
        alreadyInDay[idxRow][i] = true; //assign and update accordingly

        if(fillWorkers(avail, n, k, dailyNeed, maxShifts, sched, idx+1, shiftsUsed, alreadyInDay)==true){
          return true; //recursive returning
        }
        else{
          sched[idxRow][idxCol] = INVALID_ID;
          shiftsUsed[i]--;
          alreadyInDay[idxRow][i] = false; //we retreive all the assignings back; backtrack
        }
      }
    }
    return false; //if we end up here it should be false
}