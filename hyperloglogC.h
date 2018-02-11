#ifndef HYPERLOGLOG_H
#define HYPERLOGLOG_H
typedef struct _hyperloglog HyperloglogC;
HyperloglogC * new_hll(const char b, const size_t elements_size);
void add_hll(HyperloglogC *hll, const void *element);
int size_hll(const HyperloglogC *hll);
void free_hll(HyperloglogC *hll);
void add_hash(HyperloglogC *hll, const uint64_t hash);
#endif