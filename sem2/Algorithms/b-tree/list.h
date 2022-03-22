#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
struct element {
    element* next;
    element* prev;
    char content;
};

class List {
private:
   

public:
    bool get_beg() const {
        return beg->content;
    }
    element* end;
    element* beg;
  

    List();
    ~List();
    void delete_first();
    void insert_first(char arg);
    bool is_empty() const {
        if (beg == nullptr) {
            return true;
        }
        else return false;
    }   
    void insert_last(char arg);
    

};



