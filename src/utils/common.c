#include "utils/common.h"

static S_DATA one_bit_lookup_map[2] = {
    0x00000000,
    0xFFFFFFFF
};

static S_DATA two_bit_lookup_map[4] = {
    0x00000000,
    0x55555555,
    0xAAAAAAAA,
    0xFFFFFFFF
};

S_DATA expand_nibble(S_DATA nibble, NibbleSize size) {
    switch(size) {
        case NIB_1:
            return one_bit_lookup_map[nibble];
        case NIB_2:
            return two_bit_lookup_map[nibble];
        default:
            return 0x00;
    }
}
