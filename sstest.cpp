#include "status_structure.h"
#include <iostream>

int main(){

    Status_structure ss;
    // std::cout << "No of lines? ";
    // int n;
    // std::cin >> n;
    // for (int i = 0; i < n; ++i) {
    //     std::cout << "line " << i << " :\n";
    //     int ux, uy, lx, ly;
    //     std::cout << "Upper endpoint? ";
    //     std::cin >> ux >> uy;
    //     std::cout << "Lower endpoint? ";
    //     std::cin >> lx >> ly;
    //     Event_point uep{ ux, uy };
    //     Event_point lep{ lx, ly };
    //     Line_segment ls{uep, lep};
    //     if (lep == uep || lep < uep) {
    //         std::cerr << "<<Lower endpoint must be below upper endpoint>>\n";
    //         return 0;
    //     }
    //     ss.insert(ls, i);
    // }

    Event_point uep1{ 0, 2 };
    Event_point lep1{ 3, 0 };
    Line_segment ls1{uep1, lep1};
    std::cout << "********Inserting 1******";
    ss.insert(ls1);
    std::cout << "**************\n\n\n";

    Event_point uep2{ 5, 1 };
    Event_point lep2{ 4, -1};
    Line_segment ls2{uep2, lep2};
    std::cout << "********Inserting 2******";
    ss.insert(ls2);
    std::cout << "**************\n\n\n";

    Event_point uep3{ 6, 3 };
    Event_point lep3{ 7, 0 };
    Line_segment ls3{uep3, lep3};
    std::cout << "********Inserting 3******";
    ss.insert(ls3);
    std::cout << "**************\n\n\n";

    Event_point uep4{ 9, 1 };
    Event_point lep4{ 6, 0 };
    Line_segment ls4{uep4, lep4};
    std::cout << "********Inserting 4******";
    ss.insert(ls4);
    std::cout << "**************\n\n\n";

    Event_point uep5{ 12, 2 };
    Event_point lep5{ 11, 0 };
    Line_segment ls5{uep5, lep5};
    std::cout << "********Inserting 4******";
    ss.insert(ls5);
    std::cout << "**************\n\n\n";

    ss.show_tree();
    std::cout << "**************\n\n\n";

    ss.remove(ss.search(ls1));
    ss.remove(ss.search(ls3));
    ss.remove(ss.search(ls5));
    ss.remove(ss.search(ls4));
    std::cout << "********Removing 1,3,5,4******\n";
    ss.show_tree();
    std::cout << "**************\n\n\n";

    ss.remove(ss.search(ls2));
    std::cout << "********Removing 2******\n";
    ss.show_tree();
    std::cout << "**************\n\n\n";

    return 0;
}