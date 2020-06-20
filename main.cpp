#include "SSEClient.h"

int main() {
    SSEClient client;

    for (int i = 0; i < 10; ++i) {
        client.update(INS, "test", i);
    }
    for (int i = 0; i < 1; ++i) {
        client.update(DEL, "test", i);
    }

    vector<int> results = client.search("test");
    return 0;
}
