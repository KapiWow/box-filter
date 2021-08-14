#include <box_filter.h>
#include <stdlib.h>

static const int edge_offset = 2;
static const int filter_size = 5;
static const int simd_size = 16;

int get_id(int x, int y, int nx) {
    return y * nx + x;
}

int get_size(int nx) {
    return simd_size * ((nx - edge_offset * 2) / simd_size);
}

void box_filter_5x5(unsigned char* in, unsigned char* out, int nx, int ny) {
    //skip 2 first and 2 last pixels
    for (int y = edge_offset; y != ny - edge_offset; y++) {
        //for (int x = edge_offset; x != nx - edge_offset; x++) {
        for (int x = edge_offset; x != get_size(nx) + edge_offset; x++) {
            u_int16_t sum = 0;
            for (int y_offset = -edge_offset; y_offset != edge_offset + 1; y_offset++) {
                for (int x_offset = -edge_offset; x_offset != edge_offset + 1; x_offset++) {
                    sum += (u_int16_t)in[get_id(x + x_offset, y + y_offset, nx)];
                }
            }
            sum /= filter_size * filter_size;
            out[get_id(x, y, nx)] = (u_int8_t)sum;
        }
    }
}

void update_buffer(unsigned char* in, u_int16_t* buffer, int y, int buffer_id, int nx) {
    u_int16_t acc = 0;
    for (int x = 0; x != filter_size; x++) {
        acc += in[get_id(x, y, nx)];
    }
    buffer[get_id(0, buffer_id, nx - edge_offset * 2)] = acc;

    int id_prev = get_id(0, y, nx);
    int id_next = get_id(filter_size, y, nx);
    int id_buf = get_id(1, buffer_id, nx - edge_offset * 2);
    for (int x = 1; x != get_size(nx); x++) {
        acc += (u_int16_t)(in[id_next++]);
        acc -= (u_int16_t)(in[id_prev++]);
        buffer[id_buf++] = acc;
    }
}

void write_buffer_sum(unsigned char* out, u_int16_t* buffer, int y, int nx) {
    for (int x = 0; x != get_size(nx); x++) {
        u_int16_t sum = 0;
        for (int i = 0; i != filter_size; i++) {
            sum += buffer[get_id(x, i, nx - edge_offset * 2)];
        }
        sum /= filter_size * filter_size;
        out[get_id(x + edge_offset, y, nx)] = (u_int8_t)sum;
    }
}

void box_filter_optimized_5x5(unsigned char* in, unsigned char* out, int nx, int ny) {
    u_int16_t* buffer = (u_int16_t*)malloc((nx - edge_offset * 2) * filter_size * sizeof(u_int16_t));
    
    int buffer_start_id = 0;

    for (int y = 0; y != filter_size; y++) {
        update_buffer(in, buffer, y, y, nx);
    }
    
    write_buffer_sum(out, buffer, edge_offset, nx);

    for (int y = edge_offset + 1; y != ny - edge_offset; y++) {
        update_buffer(in, buffer, y + edge_offset, buffer_start_id, nx);
        write_buffer_sum(out, buffer, y, nx);
        
        if (buffer_start_id == 4) {
            buffer_start_id = 0;
        } else {
            buffer_start_id++;
        }
    }

    free(buffer);
}
