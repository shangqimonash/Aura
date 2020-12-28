#include "Core/SSEClient.h"

int main() {
    SSEClient client;

//    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 200000; ++i) {
        client.update(INS, "test", i);
    }
//    cout << duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() << endl;
    for (int i = 0; i < 10000; ++i) {
        client.update(DEL, "test", i);
    }

    vector<int> results = client.search("test");
    for (int res : results) {
        cout << res << endl;
    }
    return 0;
}
