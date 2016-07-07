#include "utils/common.h"

S_DATA expand_nibble(S_DATA nibble, uint8_t nibble_width, uint8_t num_expansions) {
    S_DATA new_nibble = 0;
    for(int i = 0; i < num_expansions; ++i) {
        new_nibble |= nibble << (i * nibble_width);
    }
    return new_nibble;
}
