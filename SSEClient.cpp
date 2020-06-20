#include "SSEClient.h"

SSEClient::SSEClient() {
    server = new SSEServer();
    // compute all derive key
    for(long i = 0; i < 191702; i++) {
        derived_keys[i] = (uint8_t*) malloc(AES_BLOCK_SIZE);
        key_derivation((uint8_t *) &i, sizeof(long),
                       key, AES_BLOCK_SIZE,
                       derived_keys[i]);
    }
    // init the GGM Tree
    tree = new GGMTree(GGM_SIZE, key);
}

SSEClient::~SSEClient() {
    delete_bf.reset();
    delete server;
}

void SSEClient::update(OP op, const string& keyword, int ind) {
    // compute the tag
    uint8_t pair[keyword.size() + sizeof(int)];
    memcpy(pair, keyword.c_str(), keyword.size());
    memcpy(pair + keyword.size(), (uint8_t*) &ind, sizeof(int));
    // generate the digest of tag
    uint8_t tag[DIGEST_SIZE];
    sha256_digest(pair, keyword.size() + sizeof(int), tag);
    // process the operator
    if(op == INS) {
        // token
        uint8_t token[DIGEST_SIZE];
        hmac_digest((uint8_t*) keyword.c_str(), keyword.size(),
                key, AES_BLOCK_SIZE,
                token);
        // label
        int counter = C[keyword];
        uint8_t label[DIGEST_SIZE];
        hmac_digest((uint8_t*) &counter, sizeof(int),
                token, DIGEST_SIZE,
                label);
        C[keyword]++;
        // get all offsets in BF
        vector<long> indexes = BloomFilter<32, 191702, HASH_SIZE>::get_index(tag);
        sort(indexes.begin(), indexes.end());

        vector<string> ciphertext_list;
        for(long index : indexes) {
            // derive a key from the offset
            uint8_t derived_key[AES_BLOCK_SIZE];
            key_derivation((uint8_t*) &index, sizeof(long),
                    key, AES_BLOCK_SIZE,
                    derived_key);
            // use the key to encrypt the id
            uint8_t encrypted_id[AES_BLOCK_SIZE + sizeof(int)];
            memcpy(encrypted_id, iv, AES_BLOCK_SIZE);
            aes_encrypt((uint8_t*) &ind, sizeof(int),
                        derived_key, encrypted_id,
                        encrypted_id + AES_BLOCK_SIZE);
            // save the encrypted id in the list
            ciphertext_list.emplace_back(string((char*) encrypted_id, AES_BLOCK_SIZE + sizeof(int)));
        }
        // convert tag/label to string
        string tag_str((char*) tag, DIGEST_SIZE);
        string label_str((char*) label, DIGEST_SIZE);
        // save the list on the server
        server->add_entries(label_str, tag_str, ciphertext_list);
    } else {
        // insert the tag into BF
        delete_bf.add_tag(tag);
    }
}

vector<int> SSEClient::search(const string& keyword) {
    // token
    uint8_t token[DIGEST_SIZE];
    hmac_digest((uint8_t*) keyword.c_str(), keyword.size(),
                key, AES_BLOCK_SIZE,
                token);
    // search all deleted positions
    vector<long> delete_pos = delete_bf.search();
    unordered_map<long, uint8_t *> keys = derived_keys;
    for (long & pos : delete_pos) {
        free(keys[pos]);
        keys.erase(pos);
    }
    // give all results to the server for search
    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    vector<int> res = server->search(token, keys);
    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    return res;
}