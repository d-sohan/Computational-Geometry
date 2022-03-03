#ifndef EQ_H
#define EQ_H

#include <vector>
#include <iostream>

#define BLACK 0
#define RED 1








struct Event_point {
    const int x;
    const int y;
    Event_point() : x{ -1 }, y{ -1 } {}
    Event_point(int x_val, int y_val) : x{ x_val }, y{ y_val } {}
    Event_point(const Event_point& ep) : x{ ep.x }, y{ ep.y } {}
    friend bool operator==(const Event_point& ep1, const Event_point& ep2);
    friend bool operator!=(const Event_point& ep1, const Event_point& ep2);
    friend bool operator<(const Event_point& ep1, const Event_point& ep2);
};









class Event_queue {
    struct Event {
        Event* left;
        Event* right;
        Event* parent;
        bool color;
        Event_point key;
        std::vector<int> starting_segments;
        Event() : left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK } {}
        Event(const Event_point& ep, int seg = -1) :
            left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK }, key{ ep }
        {
            if (seg != -1) starting_segments.push_back(seg);
        }
    };
private:
    Event* nil;
    Event* root;
public:
    Event_queue() : nil{ new Event() }, root{ nil } {}
    Event* next() { return tree_minimum(root); }
    Event* search(const Event_point& ep);
    void insert(const Event_point& ep, int seg = -1);
    void remove(Event*);
    bool empty() { return root == nil; }
    void show_tree() { preorder(root, 0); }
    ~Event_queue() { delete nilify(root); }
private:
    Event* tree_minimum(Event* x);
    Event* nilify(Event* x);
    void preorder(Event*, int);
    void left_rotate(Event* z);
    void right_rotate(Event* z);
    void transplant(Event* u, Event* v);
    void insert_fixup(Event* z);
    void remove_fixup(Event* x);
};



#endif