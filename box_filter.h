#ifndef BOX_FILTER_HH
#define BOX_FILTER_HH

void box_filter_5x5(unsigned char* in, unsigned char* out, int nx, int ny);

void box_filter_optimized_5x5(unsigned char* in, unsigned char* out, int nx, int ny);


#endif
