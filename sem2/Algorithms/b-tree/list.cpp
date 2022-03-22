#include "List.h"

List::List() {
    beg = nullptr;
    end = nullptr;
}


void List::delete_first() {
    if (beg != nullptr && beg->next != nullptr) {
        beg = beg->next;
        delete (beg->prev);
        beg->prev = nullptr;
    }
    else if (beg->next == nullptr) {
        delete (beg);
        beg = nullptr;
    }
}

void List::insert_first(char arg) {
    element* temp = beg;
    if (temp != nullptr) {
        if (temp->next == nullptr)
            end = temp;
    }
    beg = new element;
    beg->content = arg;
    beg->prev = nullptr;
    if (temp != nullptr) {
        temp->prev = beg;
    }
    beg->next = temp;

}

void List::insert_last(char arg) {
    if (beg == nullptr) {
        insert_first(arg);
    }
    else if (end == nullptr) {
        end = new element;
        end->content = arg;
        end->next = nullptr;
        end->prev = beg;
        beg->next = end;
    }
    else {
        element* temp = end;
        end = new element;
        end->prev = temp;
        end->content = arg;
        end->next = nullptr;
        temp->next = end;

    }
   
}

List::~List() {
    while (beg != nullptr)
        delete_first();
}

