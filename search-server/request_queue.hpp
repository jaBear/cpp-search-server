#pragma once
#include <deque>
#include "search_server.hpp"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        QueryResult(std::vector<Document> request)
            : request_name(request) {
            }
        
        std::vector<Document> request_name;
        // определите, что должно быть в структуре
    };
    std::deque<QueryResult> requests_;
    std::deque<QueryResult> empty_requests_;
    
    const static int min_in_day_ = 1440;
    
    const SearchServer& search_server_;
    
    
    void AddResult(std::vector<Document>& docs);
    // возможно, здесь вам понадобится что-то ещё
};

