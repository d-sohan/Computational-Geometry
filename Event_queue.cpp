#include "Event_queue.h"






bool operator==(const Event_point& ep1, const Event_point& ep2)
{
    return (ep1.x == ep2.x && ep1.y == ep2.y);
}

bool operator!=(const Event_point& ep1, const Event_point& ep2)
{
    return (ep1.x != ep2.x || ep1.y != ep2.y);
}


bool operator<(const Event_point& ep1, const Event_point& ep2)
{
    return (ep1.y > ep2.y || (ep1.y == ep2.y && ep1.x < ep2.x));
}









Event_queue::Event* Event_queue::search(const Event_point& ep)
{
    Event* x = root;
    while (x != nil && x->key != ep) {
        if (ep < x->key) {
            x = x->left;
        }
        else x = x->right;
    }
    return x;
}


void Event_queue::preorder(Event* x, int indent)
{
    if (x != nil) {
        for (int i = 0; i < indent; ++i) std::cout << "    ";
        
        if (x->color == RED) std::cout << "r";
        else std::cout << "b";

        std::cout << "(" << x->key.x << ", " << x->key.y << ")";

        std::cout << ": [";
        auto& segments = x->starting_segments;
        for (const auto& segment : segments) {
            std::cout << segment << ", ";
        }
        std::cout << "]" << std::endl;
        
        preorder(x->left, indent + 1);

        preorder(x->right, indent + 1);
    }
    else {
        for (int i = 0; i < indent; ++i) std::cout << "    ";
        std::cout << "nil" << std::endl;
    }
}






void Event_queue::insert(const Event_point& ep, int seg)
{
    Event* y = search(ep);
    if (y != nil) {
        if (seg != -1) {
            auto& segments = y->starting_segments;
            for (const auto& segment : segments) {
                if (segment == seg) return;
            }
            segments.push_back(seg);
        }
        return;
    }
    Event* x = root;
    while (x != nil) {
        y = x;
        if (ep < x->key) x = x->left;
        else x = x->right;
    }
    Event* z = new Event(ep, seg);
    z->left = nil;
    z->right = nil;
    z->parent = y;
    z->color = RED;
    if (y == nil) root = z;
    else if (ep < y->key) y->left = z;
    else y->right = z;
    insert_fixup(z);
}




void Event_queue::remove(Event* z)
{
    if (z == nil) return;
    Event* y = z;
    Event* x = z;
    bool y_org_color = y->color;
    if (z->left = nil) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = tree_minimum(z->right);
        y_org_color = y->color;
        x = y->right;
        if (y->parent == z) x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (y_org_color == BLACK)
        remove_fixup(x);
}





Event_queue::Event* Event_queue::tree_minimum(Event* x)
{
    while (x->left != nil) {
        x = x->left;
    }
    return x;
}






void Event_queue::left_rotate(Event* x)
{
    Event* y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}






void Event_queue::right_rotate(Event* y)
{
    Event* x = y->left;
    y->left = x->right;
    if (x->right != nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil) root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}




void Event_queue::transplant(Event* u, Event* v)
{
    if (u->parent == nil) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}







void Event_queue::insert_fixup(Event* z)
{
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Event* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(z->parent->parent);
            }
        }
        else {
            Event* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}








void Event_queue::remove_fixup(Event* x)
{
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Event* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->parent);
                x = root;
            }
        }
        else {
            Event* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

Event_queue::Event* Event_queue::nilify(Event* x)
{
    if (x != nil) {
        x->left = nilify(x->left);
        x->right = nilify(x->right);
        delete x;
    }
    return nil;
}
