//
// Created by xiangqian5 on 2021/4/8.
//

#include <stdint.h>
#include "src/diaochan/diaochan.h"

extern "C" {
    void* new_diaochan(const char* target, int32_t timeout) {
        auto* diaochan = new CDiaochan(target, timeout);
        return reinterpret_cast<CDiaochan*>(diaochan);
    }
    void delete_diaochan(CDiaochan* diaochan) {
        delete diaochan;
    }
    char* recall(CDiaochan* diaochan, const char* global, const char* context) {
        return strdup(diaochan->Recall(global, context).c_str());
    }

    void* new_async_diaochan(const char* target, int32_t timeout) {
        auto* diaochan = new AsyncDiaochan(target, timeout);
        return reinterpret_cast<AsyncDiaochan*>(diaochan);
    }
    void delete_async_diaochan(AsyncDiaochan* diaochan) {
        delete diaochan;
    }
    char* async_recall(AsyncDiaochan* diaochan, const char* global, const char* context) {
        return strdup(diaochan->AsyncRecall(global, context).c_str());
    }
}