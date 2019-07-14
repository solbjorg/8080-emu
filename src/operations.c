#include "operations.h"
#include <stdint.h>

void LXI_pair(uint8_t *first_reg, uint8_t *second_reg, uint8_t *data) {
  *first_reg = data[1];
  *second_reg = data[0];
}
