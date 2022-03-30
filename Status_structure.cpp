#include <cmath>
#include "Status_structure.h"



bool box_contains(const Line_segment* ls, const Event_point& p) {
    return (p.x >= epmin(ls->uep.x, ls->lep.x) && p.x <= epmax(ls->uep.x, ls->lep.x)) &&
        (p.y >= epmin(ls->uep.y, ls->lep.y) && p.y <= epmax(ls->uep.y, ls->lep.y));
}


Orientation slope(const Line_segment* ls) {
    if (epeq(ls->uep.y , ls->lep.y)) {
        return Orientation{ HORIZONTAL };
    }
    else if (epeq(ls->uep.x , ls->lep.x)) {
        return Orientation{ VERTICAL };
    }
    else {
        double m = (ls->uep.y - ls->lep.y) / (ls->uep.x - ls->lep.x);
        return Orientation{ m };
    }
}


Event_point get_intercept(const Line_segment* ls, const Event_point& p, int type) {
    if (ls->uep == p || ls->lep == p) return p;
    switch (type) {
    case HORIZONTAL:
        return Event_point{ p.x, ls->uep.y };
    case VERTICAL:
        return Event_point{ ls->uep.x, p.y };
    case INCLINED:
        double m = slope(ls).value();
        return Event_point{ (p.y - ls->uep.y) / m + ls->uep.x, p.y };
    }
    return Event_point{};
}

double angle(const Line_segment* ls, const Event_point& ep) {
    double b = ep.x - ls->lep.x;
    double p = ep.y - ls->lep.y;
    double h = sqrt(b * b + p * p);
    return acos(b / h);
}

bool line_segment_lt(const Line_segment* ls1, const Line_segment* ls2, const Event_point& p) {
    auto or1 = slope(ls1), or2 = slope(ls2);
    if (or1.type == HORIZONTAL && or2.type == HORIZONTAL) {
        return eplt(ls1->uep.x , ls2->uep.x);
    }
    else if (or1.type == HORIZONTAL) {
        return eplt(get_intercept(ls1, p, HORIZONTAL).x , get_intercept(ls2, p, or2.type).x);
    }
    else if (or1.type == VERTICAL && or2.type == HORIZONTAL) {
        return epgt(get_intercept(ls2, p, HORIZONTAL).x, get_intercept(ls1, p, VERTICAL).x);
        // return (get_intercept(ls1, p, VERTICAL).x <= get_intercept(ls2, p, HORIZONTAL).x);
    }
    else if (or1.type == VERTICAL && or2.type == VERTICAL) {
        return eplt(get_intercept(ls1, p, VERTICAL).x , get_intercept(ls2, p, VERTICAL).x);
    }
    else if (or1.type == VERTICAL && or2.type == INCLINED) {
        auto p1 = get_intercept(ls1, p, VERTICAL), p2 = get_intercept(ls2, p, INCLINED);
        if (epeq(p1.x, p2.x)) {
            return (angle(ls1, p1) < angle(ls2, p2));
        }
        else if (eplt(p1.x , p2.x)) return true;
        else return false;

    }
    else if (or1.type == INCLINED && or2.type == HORIZONTAL) {
        return eplt(get_intercept(ls1, p, INCLINED).x , ls2->uep.x);
    }
    else if (or1.type == INCLINED) {
        auto p1 = get_intercept(ls1, p, INCLINED), p2 = get_intercept(ls2, p, or2.type);
        if (eplt(p1.x , p2.x)) return true;
        else if (epeq(p1.x, p2.x)) {
            return (angle(ls1, p1) < angle(ls2, p2));
        }
        else return false;
    }
    else {
        std::cerr << "Error ordering line_segments" << std::endl;
        return false;
    }
}

Event_point get_intersection(const Line_segment* ls1, const Line_segment* ls2) {
    auto or1 = slope(ls1), or2 = slope(ls2);
    if (or1.type == HORIZONTAL && or2.type == HORIZONTAL || or1.type == VERTICAL && or2.type == VERTICAL) {
        return Event_point{};
    }
    if (or1.type == HORIZONTAL) {
        auto ip = get_intercept(ls2, ls1->uep, or2.type);
        if (box_contains(ls1, ip) && box_contains(ls2, ip)) return ip;
        else return Event_point{};
    }
    else if (or1.type == VERTICAL && or2.type == INCLINED) {
        auto ip = Event_point{ ls1->uep.x, or2.value() * (ls1->uep.x - ls2->uep.x) + ls2->uep.y };
        if (box_contains(ls1, ip) && box_contains(ls2, ip)) return ip;
        else return Event_point{};
    }
    else if (or1.type == INCLINED && or2.type == INCLINED) {
        double m1 = or1.value(), m2 = or2.value();
        double c1 = ls1->uep.y - m1 * ls1->uep.x, c2 = ls2->uep.y - m2 * ls2->uep.x;
        double ip_x = (c1 - c2) / (m2 - m1);
        double ip_y = m1 * ip_x + c1;
        auto ip = Event_point{ ip_x, ip_y };
        if (box_contains(ls1, ip) && box_contains(ls2, ip)) return ip;
        else return Event_point{};
    }
    else {
        return get_intersection(ls2, ls1);
    }
}

int ls_position(const Line_segment* ls, const Event_point& p) {
    auto or1 = slope(ls);
    if (or1.type != HORIZONTAL) {
        auto ic = get_intercept(ls, p, or1.type);
        if (epeq(ic.x, p.x)) return 0;
        if (eplt(ic.x , p.x)) return -1;
        return 1;
    }
    else {
        if (box_contains(ls, p)) return 0;
        if (eplt(ls->lep.x , p.x)) return -1;
        return 1;
    }
}



















const Status_structure::Segment* Status_structure::find_left_neighbor(const Segment* s) {
    if (s == nil) return s;
    if (s->left != nil) return tree_maximum(s->left);
    const Segment* y = s->parent;
    while (y != nil && s == y->left) {
        s = y;
        y = y->parent;
    }
    return y;
}


const Status_structure::Segment* Status_structure::find_right_neighbor(const Segment* s) {
    if (s == nil) return s;
    if (s->right != nil) return tree_minimum(s->right);
    const Segment* y = s->parent;
    while (y != nil && s == y->right) {
        s = y;
        y = y->parent;
    }
    return y;
}


const Status_structure::Segment* Status_structure::search_leftmost(std::vector<const Segment*>& vls1, std::vector<const Segment*>& vls2) {
    int i = 0, j = 0;
    while (i < vls1.size() && j < vls2.size() && vls1[i] == vls2[j]) {
        ++i;
        ++j;
    }
    if (j < vls2.size()) return vls2[j];
    return nil;
}
const Status_structure::Segment* Status_structure::search_rightmost(std::vector<const Segment*>& vls1, std::vector<const Segment*>& vls2) {
    int i = vls1.size()-1, j = vls2.size()-1;
    while (i >= 0 && j >= 0 && vls1[i] == vls2[j]) {
        --i;
        --j;
    }
    if (j >= 0) return vls2[j];
    return nil;
}

std::vector<const Status_structure::Segment*> Status_structure::get_lp(const Event_point& ep) {
    std::vector<const Segment*> vls, lp;
    inorder(root, vls);
    for (const auto& x : vls) {
        if (x->key->lep == ep) lp.push_back(x);
    }
    return lp;
}
std::vector<const Status_structure::Segment*> Status_structure::get_cp(const Event_point& ep) {
    std::vector<const Segment*> vls, cp;
    inorder(root, vls);
    for (const auto& x : vls) {
        if (ls_position(x->key, ep) == 0 && x->key->lep != ep && x->key->uep != ep) cp.push_back(x);
    }
    return cp;
}










void Status_structure::insert(const Line_segment* ls, const Event_point& p)
{
    Segment* y = nil;
    Segment* x = root;
    while (x != nil) {
        y = x;
        if (line_segment_lt(ls, (x->key), p)) x = x->left;
        else x = x->right;
    }
    Segment* z = new Segment(ls);
    z->left = nil;
    z->right = nil;
    z->parent = y;
    z->color = RED;
    if (y == nil) root = z;
    else if (line_segment_lt(ls, (y->key), p)) y->left = z;
    else y->right = z;
    insert_fixup(z);
}




void Status_structure::remove(const Segment* zee)
{
    Segment* z = deconst(zee);

    if (z == nil) return;
    Segment* y = z;
    Segment* x = z;
    bool y_org_color = y->color;
    if (z->left == nil) {
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















void Status_structure::preorder(Segment* x, int indent)
{
    if (x != nil) {
        for (int i = 0; i < indent; ++i) std::cout << "    ";

        if (x->color == RED) std::cout << "r";
        else std::cout << "b";

        std::cout << "(" << x->key->number << ")" << std::endl;


        preorder(x->left, indent + 1);

        preorder(x->right, indent + 1);
    }
    else {
        for (int i = 0; i < indent; ++i) std::cout << "    ";
        std::cout << "nil" << std::endl;
    }
}



Status_structure::Segment* Status_structure::fln(const Event_point& p, Segment* s) {
    if (s != nil && ls_position(s->key, p) == -1) {
        auto ans = fln(p, s->right);
        return (ans == nil) ? s : ans;
    }
    else if (s != nil) {
        return fln(p, s->left);
    }
    else return s;
}

Status_structure::Segment* Status_structure::frn(const Event_point& p, Segment* s) {
    if (s != nil && ls_position(s->key, p) == 1) {
        auto ans = frn(p, s->left);
        return (ans == nil) ? s : ans;
    }
    else if (s != nil) {
        return frn(p, s->right);
    }
    else return s;
}

Status_structure::Segment* Status_structure::deconst(const Segment* zee) {
    if (zee == nil) return nil;
    Segment* z;
    if (zee->parent == nil) z = root;
    else if (zee == zee->parent->left) z = zee->parent->left;
    else z = zee->parent->right;
    return z;
}

Status_structure::Segment* Status_structure::search(const Line_segment* ls, Segment* x)
{
    if (x != nil && x->key != ls) {
        auto res_left = search(ls, x->left);
        if (res_left != nil) return res_left;
        auto res_right = search(ls, x->right);
        if (res_right == nil) return res_right;
        return nil;
    }
    else return x;
}

void Status_structure::inorder(Segment* x, std::vector<const Segment*>& vls) {
    if (x != nil) {
        inorder(x->left, vls);
        vls.push_back(x);
        inorder(x->right, vls);
    }
}




















Status_structure::Segment* Status_structure::tree_minimum(Segment* x)
{
    while (x->left != nil) {
        x = x->left;
    }
    return x;
}


Status_structure::Segment* Status_structure::tree_maximum(Segment* x)
{
    while (x->right != nil) {
        x = x->right;
    }
    return x;
}



void Status_structure::left_rotate(Segment* x)
{
    Segment* y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}






void Status_structure::right_rotate(Segment* y)
{
    Segment* x = y->left;
    y->left = x->right;
    if (x->right != nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil) root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}




void Status_structure::transplant(Segment* u, Segment* v)
{
    if (u->parent == nil) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}







void Status_structure::insert_fixup(Segment* z)
{
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Segment* y = z->parent->parent->right;
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
            Segment* y = z->parent->parent->left;
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








void Status_structure::remove_fixup(Segment* x)
{
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Segment* w = x->parent->right;
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
            Segment* w = x->parent->left;
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

Status_structure::Segment* Status_structure::nilify(Segment* x)
{
    if (x != nil) {
        x->left = nilify(x->left);
        x->right = nilify(x->right);
        delete x;
    }
    return nil;
}