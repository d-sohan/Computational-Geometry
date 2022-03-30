#pragma once

#include <vector>
#include <iostream>


#define BLACK 0
#define RED 1

#define DEVIATION 0.03







struct Event_point {
    const double x;
    const double y;
    const bool null_ep;
    Event_point() : x{}, y{}, null_ep{ true } {}
    Event_point(double x_val, double y_val) : x{ x_val }, y{ y_val }, null_ep{ false } {}
    Event_point(const Event_point& ep) : x{ ep.x }, y{ ep.y }, null_ep{ ep.null_ep } {}
    friend bool operator==(const Event_point& ep1, const Event_point& ep2);
    friend bool operator!=(const Event_point& ep1, const Event_point& ep2);
    friend bool operator<(const Event_point& ep1, const Event_point& ep2);
};





struct Line_segment {
    const int number;
    const Event_point uep;
    const Event_point lep;
    Line_segment(int num = -1) : uep{}, lep{}, number{ num } {}
    Line_segment(int num, const Event_point& uep_val, const Event_point& lep_val) : uep{ uep_val }, lep{ lep_val }, number{ num } {}
    Line_segment(const Line_segment& ls) : uep{ ls.uep }, lep{ ls.lep }, number{ ls.number }{}
    friend bool operator==(const Line_segment& ls1, const Line_segment& ls2) {
        return ls1.number == ls2.number;
    }
};



bool epeq(double x, double y);
bool eplt(double x, double y);
bool epgt(double x, double y);
double epmax(double x, double y);
double epmin(double x, double y);

class Event_queue {
    
    struct Event {
        Event* left;
        Event* right;
        Event* parent;
        bool color;
        Event_point key;
        std::vector<const Line_segment*> starting_segments;
        Event() : left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK } {}
        Event(const Event_point& ep, const Line_segment* lsptr = nullptr) :
            left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK }, key{ ep }
        {
            if (lsptr != nullptr) starting_segments.push_back(lsptr);
        }
    };
    
private:
    Event* nil;
    Event* root;

    
public:
    Event_queue() : nil{ new Event() }, root{ nil } {}
    Event* next() { return tree_minimum(root); }
    Event* search(const Event_point& ep);
    void insert(const Event_point& ep, const Line_segment* lsptr = nullptr);
    void remove(Event*);
    bool empty(Event * ev) { return ev == nil; }
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