#include <bits/stdc++.h>
#include "core/single_linked_list.h"


int main() {
    SingleLinkedList<int> l;
    l.PushBack(1);
    ++*l.cbegin();
    for (int i : l) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}