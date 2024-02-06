#include "request_queue.hpp"

RequestQueue::RequestQueue(const SearchServer& search_server) : search_server_(search_server) {
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    std::vector<Document> finded_docs = search_server_.FindTopDocuments(raw_query, status);
    
    if (requests_.size() < min_in_day_) {
        AddResult(finded_docs);
    } else {
        if (requests_.front().request_name.empty()) {
            requests_.pop_front();
            empty_requests_.pop_front();
        } else {
            requests_.pop_front();
        }
    }
    return finded_docs;
    // напишите реализацию
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    
    std::vector<Document> finded_docs = search_server_.FindTopDocuments(raw_query);
    
    if (requests_.size() < min_in_day_) {
        AddResult(finded_docs);
    } else {
        if (requests_.front().request_name.empty()) {
            requests_.pop_front();
            empty_requests_.pop_front();
            AddResult(finded_docs);
        } else {
            requests_.pop_front();
            AddResult(finded_docs);
        }
    }
    return finded_docs;
    // напишите реализацию
}
int RequestQueue::GetNoResultRequests() const {
    // напишите реализацию
    return static_cast<int>(empty_requests_.size());
}

void RequestQueue::AddResult(std::vector<Document>& docs) {
    requests_.push_back(docs);
    if (docs.empty()) {
        empty_requests_.push_back(docs);
    }
}

