#include "SSEClient.h"

int main() {
    SSEClient client;

    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 200000; ++i) {
        client.update(INS, "test", i);
    }
    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 100; ++i) {
        client.update(DEL, "test", i);
    }

    vector<int> results = client.search("test");
    return 0;
}
