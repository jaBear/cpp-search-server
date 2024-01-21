#pragma once
#include <vector>
#include "document.hpp"

template <typename Iterator>
class IteratorRange {
public:
    
    IteratorRange(Iterator iter_begin, Iterator iter_end, const size_t& page_size, Iterator& end_itr_for_IR) {
        for (int i = 0; i < page_size; i++) {
            if (iter_begin == iter_end) {
                break;
            }
            docs_.push_back(*iter_begin);
            iter_begin++;
        }
        end_itr_for_IR = iter_begin;
    }
    
    auto begin() const {
        return docs_.begin();
    }
    
    auto end() const {
        return docs_.end();
    }
    
private:
    std::vector<Document> docs_;
};

template <typename Iterator>
class Paginator {
public:
    
    Paginator(Iterator iter_begin, Iterator iter_end, const size_t& page_size) {
        auto end_itr_for_IR = iter_begin;
        for (int i = 0; i < page_size; i++) {
            itrs_.push_back(IteratorRange(iter_begin, iter_end, page_size, end_itr_for_IR));
            iter_begin = end_itr_for_IR;
        }
    }
    
    auto begin() const {
        return itrs_.begin();
    }
    
    auto end() const {
        return itrs_.end();
    }
    
private:
    std::vector<IteratorRange<Iterator>> itrs_;
};
