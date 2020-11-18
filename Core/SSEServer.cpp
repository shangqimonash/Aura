//
// Created by shangqi on 2020/11/18.
//

#include "SSEServer.h"

SSEServer::SSEServer() {
    this->handler = new SSEServerHandler();
}

void SSEServer::add_entries(const string& label, const string& tag, const vector<string>& ciphertext_list) {
    handler->add_entries(label, tag, ciphertext_list);
}

void SSEServer::search(std::vector<int> &_return, const string &token, const std::vector<SerialisedNode> &node_list,
                       const int32_t level) {
    // convert serialised node to GGM nodes
    vector<GGMNode> received_nodes;
    received_nodes.reserve(node_list.size());
    for(const SerialisedNode& node : node_list) {
        received_nodes.emplace_back(node.index, node.level, (uint8_t*) node.key.c_str());
    }
    // search for the result
    _return = handler->search((uint8_t*) token.c_str(), received_nodes, level);
}

int main() {
    TThreadedServer server(
            make_shared<SSEServiceProcessor>(make_shared<SSEServer>()),
            make_shared<TServerSocket>("localhost", 9090),
            make_shared<TBufferedTransportFactory>(),
            make_shared<TBinaryProtocolFactory>()
    );
    // start service
    server.serve();
}