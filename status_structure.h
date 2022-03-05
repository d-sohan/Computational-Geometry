#ifndef STS_H
#define STS_H


#include <vector>
#include <iostream>
#include "Event_queue.h"

#define BLACK 0
#define RED 1


struct Line_segment {
    const Event_point uep;
    const Event_point lep;
    Line_segment() : uep{ -1,-1 }, lep{ -1,-1 } {}
    Line_segment(Event_point uep_val, Event_point lep_val) : uep{ uep_val }, lep{ lep_val } {}
    Line_segment(const Line_segment& ls) : uep{ ls.uep }, lep{ ls.lep } {}
    friend float slope(const Line_segment& ls) {return (ls.uep.y - ls.lep.y)/(ls.uep.x - ls.lep.x);}
    friend bool operator==(const Line_segment& ls1, const Line_segment& ls2);
    friend bool operator!=(const Line_segment& ls1, const Line_segment& ls2);
    friend bool operator<(const Line_segment& ls1, const Line_segment& ls2);
};


class Status_structure {
    struct Segment {
        Segment* left;
        Segment* right;
        Segment* parent;
        bool color;
        Line_segment key;
        std::vector<int> starting_segments;
        Segment() : left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK } {}
        Segment(const Line_segment& ls, int seg = -1) :
            left{ nullptr }, right{ nullptr }, parent{ nullptr }, color{ BLACK }, key{ ls }
        {
            if (seg != -1) starting_segments.push_back(seg);
        }
    };
private:
    Segment* nil;
    Segment* root;
public:
    Status_structure() : nil{ new Segment() }, root{ nil } {}
    Segment* next() { return tree_minimum(root); }
    Segment* search(const Line_segment& ls);
    void insert(const Line_segment& ls, int seg = -1);
    void remove(Segment*);
    bool empty() { return root == nil; }
    void show_tree() { preorder(root, 0); }
    ~Status_structure() { delete nilify(root); }
private:
    Segment* tree_minimum(Segment* x);
    Segment* nilify(Segment* x);
    void preorder(Segment*, int);
    void left_rotate(Segment* z);
    void right_rotate(Segment* z);
    void transplant(Segment* u, Segment* v);
    void insert_fixup(Segment* z);
    void remove_fixup(Segment* x);
};

#endif