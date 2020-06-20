#include "GGMTree.h"

GGMTree::GGMTree(long num_node, uint8_t *msk) {
    this->num_node = num_node;
    this->level = ceil(log2(num_node)) + 1;
    memcpy(this->msk, msk, AES_BLOCK_SIZE);
}

void GGMTree::get_keychain_from_key(uint8_t *current_key, long offset) {
    uint8_t next_key[AES_BLOCK_SIZE];
    // derive tag
    int i = 0, j = 1;
    // iteratively find the key
    while (offset) {
        if(offset & 1) {
            aes_encrypt((uint8_t*) &j, sizeof(int),
                    current_key, iv,
                    next_key);
        } else {
            aes_encrypt((uint8_t*) &i, sizeof(int),
                        current_key, iv,
                        next_key);
        }
        offset >>= 1;
        memcpy(current_key, next_key, AES_BLOCK_SIZE);
    }
}