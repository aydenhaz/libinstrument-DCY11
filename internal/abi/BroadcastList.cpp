#include "BroadcastList.h"
namespace IPL_ABI {
namespace {
struct Node : BroadcastLink {
    GPI::BroadcastCallable callable;
    uint32_t channel;
    Node(uint32_t channel, const GPI::BroadcastCallable& callable)
        : callable(callable), channel(channel) {}
};
#if defined(__arm__) && !defined(__aarch64__)
static_assert(sizeof(Node)==32,"V21 broadcast node");
static_assert(offsetof(Node,callable)==8,"V21 stored Callable offset");
static_assert(offsetof(Node,channel)==28,"V21 stored channel offset");
#endif
using Lock = std::lock_guard<std::mutex>;
Node* find(BroadcastListState& state, uint32_t channel, const GPI::BroadcastCallable& callable) {
    // V21 0x1527c searches backwards, comparing identity before channel.
    for (BroadcastLink* p=state.sentinel.previous; p!=&state.sentinel; p=p->previous) {
        Node* node=static_cast<Node*>(p);
        if (node->callable.equals(callable) && node->channel==channel) return node;
    }
    return nullptr;
}
}
void addBroadcastCallable(BroadcastListState& state, uint32_t channel, const GPI::BroadcastCallable& callable) {
    Lock lock(state.mutex);
    if (Node* node=find(state,channel,callable)) {
        node->callable=callable;
        return;
    }
    Node* node=new Node(channel,callable);
    node->previous=&state.sentinel;
    node->next=state.sentinel.next;
    state.sentinel.next->previous=node;
    state.sentinel.next=node;
    ++state.count;
}
void removeBroadcastCallable(BroadcastListState& state, uint32_t channel, const GPI::BroadcastCallable& callable) {
    GPI::BroadcastCallable removed;
    {
        Lock lock(state.mutex);
        if (Node* node=find(state,channel,callable)) {
            // Retain one copy while unlinking/destructing under the lock;
            // final release follows unlock, as in original 0x1675c.
            removed=node->callable;
            node->next->previous=node->previous;
            node->previous->next=node->next;
            --state.count;
            delete node;
        }
    }
}
}
