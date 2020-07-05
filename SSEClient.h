//
// Created by shangqi on 2020/6/17.
//

#ifndef AURA_SSECLIENT_H
#define AURA_SSECLIENT_H

#include "GGMTree.h"
#include "SSEServer.h"

enum OP {
    INS, DEL
};

class SSEClient {
private:
    uint8_t *key = (unsigned char*) "0123456789123456";
    uint8_t *iv = (unsigned char*) "0123456789123456";

    GGMTree *tree;
    BloomFilter<32, GGM_SIZE, HASH_SIZE> delete_bf;
    unordered_map<string, int> C;       // search time

    SSEServer *server;
public:
    SSEClient();
    ~SSEClient();
    void update(OP op, const string& keyword, int ind);
    vector<int> search(const string& keyword);
};


#endif //AURA_SSECLIENT_H
