//
// Created by shangqi on 2020/6/17.
//

#ifndef AURA_SSESERVER_H
#define AURA_SSESERVER_H

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "BloomFilter.h"
extern "C" {
#include "CommonUtil.h"
}
#include "GGMTree.h"

using namespace chrono;
using namespace std;

class SSEServer {
private:
    unordered_map<string, string> tags;
    unordered_map<string, vector<string>> dict;
    unordered_map<long, uint8_t*> keys;

    void compute_leaf_keys(const vector<GGMNode>& node_list, int level);
public:
    SSEServer();
    void add_entries(const string& label, const string& tag, vector<string> ciphertext_list);
    vector<int> search(uint8_t *token, vector<GGMNode> node_list, int level);
};


#endif //AURA_SSESERVER_H
