//
// Created by shangqi on 2020/6/20.
//

#ifndef AURA_GGMTREE_H
#define AURA_GGMTREE_H

#include <bitset>
#include <cmath>
#include <cstring>

extern "C" {
#include "../Util/CommonUtil.h"
}

using namespace std;

class GGMTree {
private:
    long num_node;
    int level;
    uint8_t msk[AES_BLOCK_SIZE];

    uint8_t *iv = (unsigned char*) "0123456789123456";

public:
    GGMTree(long num_node, uint8_t *msk);
    void get_keychain_from_key(uint8_t *current_key, long offset);
};


#endif //AURA_GGMTREE_H
