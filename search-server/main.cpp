#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    
    vector<string> words;
    string word;
    
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
    
    public:
    
    void AddDocument(int document_id, const string& document) {
        
        const vector<string> document_splitted = SplitIntoWordsNoStop(document);
        
        double tf_of_one_word = 1./document_splitted.size();
        
        for (auto& word : document_splitted) {
            documents_[word][document_id] += tf_of_one_word;
        }
        
        document_count_++;
    }
    
    void SetStopWords(const string& text) {
        vector<string> newStopWords = SplitIntoWords(text);
        for (const string& word : newStopWords) {
            stop_words_.insert(word);
        }
        
    }
    
    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<Document> FindTopDocuments(const string& query) const{
        
        Query newQuery = ParseQuery(query);
        vector<Document> matched_documents = FindAllDocuments(newQuery);
        
        sort(matched_documents.begin(), matched_documents.end(), [] (const Document& one,const Document& two) {
            return one.relevance > two.relevance;
        });
        
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        
        return matched_documents;
    }
    
    private:
    
    set<string> stop_words_;
    map<string, map<int, double>> documents_;
    double document_count_ = 0;
    
    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };
 
    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };
    
    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;

        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }
    
    Query ParseQuery(const string& text) const {
        Query query;
        
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            
            if (!query_word.is_stop) {
                
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }
    
    double FindIDF(const double sum_of_docs) const {
        double IDF = log(document_count_/sum_of_docs);
        return IDF;
    }
    
    map<int, double> AddAllPlusWords (const map<string, map<int, double>>& doc_words, const set<string>& query) const{
        map<int, double> plus_docs;
        
        for (const auto& word : query) {
            if (doc_words.count(word)) {
                double IDF = FindIDF(doc_words.at(word).size());
                
            for(const auto& id : doc_words.at(word)) {
                plus_docs[id.first] += IDF * id.second;
                }
            }
            
        }
        return plus_docs;
    }
    
    void SubstractMinusWords (const map<string, map<int, double>>& doc_words, const set<string>& query, map<int, double>& rlv) const {
        
        for (const auto& word : query) {
            
            if (doc_words.count(word)) {
                
                for(const auto& test : doc_words.at(word)) {
                    rlv.erase(test.first);
                }
            }
        }
    }
    
    vector<Document> FindAllDocuments(const Query& query) const {
        
        //adding to relevant documents all plus words
        map<int, double> document_to_relevance = AddAllPlusWords(documents_, query.plus_words);
        
        SubstractMinusWords(documents_, query.minus_words, document_to_relevance);
        
        //from map<int, double> to vector<Document>
        vector<Document> result;
        
        for (const auto& doc : document_to_relevance) {
            result.push_back({doc.first, doc.second});
        }
        
        return result;
    }
    
};

    SearchServer CreateSearchServer() {
        SearchServer search_server;
        
        const string stop_words_joined = ReadLine();
        search_server.SetStopWords(stop_words_joined);
        const int document_count = ReadLineWithNumber();
        
        for (int document_id = 0; document_id < document_count; document_id++) {
            string documents = ReadLine();
            search_server.AddDocument(document_id, documents);
        }
                
        return search_server;
    }

int main() {
    const SearchServer newSearchServer = CreateSearchServer();
    const string query = ReadLine();
    
    vector<Document> top = newSearchServer.FindTopDocuments(query);
    
    for (const auto& word : top) {
        cout << "{ document_id = " << word.id << ", relevance = " << word.relevance << " }" << endl;
    }
}
