#pragma once
#include <iostream>
#include "document.hpp"
#include "paginator.hpp"



template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& IR);

template <typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& IR) {
    for (auto page = IR.begin(); page != IR.end(); ++page) {
        out << *page;
    }
    return out;
}
