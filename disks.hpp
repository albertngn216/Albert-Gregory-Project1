///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once



#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

enum disk_color { DISK_LIGHT, DISK_DARK};

class disk_state {
private:
  std::vector<disk_color> _colors;

public:
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

    assert(light_count > 0);

    for (size_t i = 1; i < _colors.size(); i += 2) {
      _colors[i] = DISK_DARK;
    }
  }

  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t light_count() const {
    return total_count() / 2;
  }

  size_t dark_count() const {
    return light_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_initialized() const {
      for (size_t i = 0; i < total_count(); i++){     //check each position before function
        if (i % 2 == 0){                              //check even position --> should be light
            if (_colors[i] == DISK_DARK) {
              return false;
            }
        } else {                                      //check odd position --> should be dark
            if (_colors[i] == DISK_LIGHT) {
              return false;
            }
        }
      }

      return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks on
  // the left (low indices) and all dark disks on the right (high indices).
  bool is_sorted() const {

    //iterate through each disk in vector
    for(size_t i = 0; i < total_count(); i++){

      //check first half n/2
      if(i < total_count() / 2){
        
        //if any dark disks found return false
        if(_colors[i] == DISK_DARK){
          return false;

        }
        //check second half
      } else {

        //check if any light disks remain in second half
        if(_colors[i] == DISK_LIGHT){
          return false;

        }
      }
    }
      
      return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the alternate algorithm.
sorted_disks sort_alternate(const disk_state& before) {
	int numOfSwap = 0;                                                                      //record # of step swap

  disk_state state = before;
    for(int i = 0; i < state.total_count() + 1; i++){
      if(i % 2 == 0){
          for(int index = 0; index < state.total_count() - 1; index = index + 2){
              if(state.get(index) != state.get(index +1)){
                  if(state.get(index) == DISK_DARK && state.get(index + 1) == DISK_LIGHT){
                      state.swap(index);
                      numOfSwap++;
                  }
              }
          }
      }
      else{
        for(int index = 1; index < state.total_count() - 2; index = index + 2){
            if (state.get(index) != state.get(index + 1)){
                if(state.get(index) == DISK_DARK && state.get(index+1) == DISK_LIGHT){
                    state.swap(index);
                    numOfSwap++;
                }
            }
          }
        }
      }

  return sorted_disks(disk_state(state), numOfSwap);
}


// Algorithm that sorts disks using the lawnmower algorithm.
// Return a sorted_disk(disk_state(<state>), numofSwap)
sorted_disks sort_lawnmower(const disk_state& before) {

  //copy before to new variable to sort properly
  disk_state state = before;
  //initialize counter
  int numOfSwap = 0;

  for(int i = 0; i < state.total_count() / 2; i++) { //iterate left to right
    int index = 0;
    while (index + 1 < state.total_count()) {  //sentinal to not go past n/2
      if(state.get(index) != state.get(index + 1)) {  //comparing two disks next to each other
        if(state.get(index) == DISK_DARK && state.get(index + 1) == DISK_LIGHT) { //if a dark disk is to the left of a light 
          //swap disks and increment swap counter
          state.swap(index);
          numOfSwap++;
        }
      }
      //increment index in while loop until i+1 < n/2
      index++;
    }
    while (index > 0) {
      if(state.get(index-1) != state.get(index)) {
        if(state.get(index-1) == DISK_DARK && state.get(index) == DISK_LIGHT) {
          state.swap(index - 1);
          numOfSwap++;
        }
      }
      index--;
    }
  }

  return sorted_disks(disk_state(state), numOfSwap);
}
