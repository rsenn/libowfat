#include "../fmt.h"

size_t fmt_pb_type0_sint(char* dest,signed __int64 l) {
  return fmt_varint(dest,(l << 1) ^ (l >> (sizeof(l)*8-1)));
}
