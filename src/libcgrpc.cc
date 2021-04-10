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
}