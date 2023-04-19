#define BYTES(n) (n+7)/8

#define EXTRACT_COLOR(n, m, k) (n >> m % BIT(k))
