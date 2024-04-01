#include "DESKey.h"

DESKey::DESKey() {
    key1 = 0;
    key2 = 0;
    key3 = 0;
}

DESKey::DESKey(unsigned long provided_key1, unsigned long provided_key2, unsigned long provided_key3) {
    key1 = provided_key1;
    key2 = provided_key2;
    key3 = provided_key3;
}
