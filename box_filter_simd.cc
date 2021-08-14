#include <box_filter_simd.hh>
#include <simd.hh>
#include <new>

static const int edge_offset = 2;
static const int filter_size = 5;

int get_id(int x, int y, int nx) {
    return y * nx + x;
}

// calculate line sum of 5 near elements from {y} line of {in} to {buffer_id} line of {buffer}
void update_buffer(unsigned char* in, u_int16_t* buffer, int y, int buffer_id, int nx) {
    const int buffer_size = 16*((nx - edge_offset * 2) / 16) + 16;
    uint16_t_16 acc;
    uint16_t_16 temp;
    uint8_t_16 acc8;
    for (int x = edge_offset; x <= (nx - edge_offset); x += 16) {
        acc8.read(&in[get_id(x - edge_offset, y, nx)]);
        convert_u8_u16_16(acc8, acc);
         
        for (int i = -edge_offset + 1; i <= edge_offset; i++) {
            convert_u8_u16_16(uint8_t_16 {&in[get_id(x - i, y, nx)]}, temp);
            acc += temp;
        }
        acc.write(&buffer[get_id(x - edge_offset, buffer_id, buffer_size)]);
    }
}

// calculate sum of 5 lines in {buffer} and write sum of 4 lines for next filtered
// result to last line of {buffer}
void first_buffer_sum(unsigned char* out, u_int16_t* buffer, int nx) {
    const int buffer_size = 16*((nx - edge_offset * 2) / 16) + 16;
    uint16_t_16 sum;
    uint16_t_16 buf;
    uint8_t_16 res;
    for (int x = 0; x <= (nx - edge_offset*2); x += 16) {
        sum.read(&buffer[get_id(x, 0, buffer_size)]);
        buf.read(&buffer[get_id(x, 1, buffer_size)]);
        for (int i = 2; i != filter_size; i++) {
            buf += uint16_t_16{&buffer[get_id(x, i, buffer_size)]};
        }
        sum += buf;
        
        sum /= 25;
        convert_u16_u8_16(sum, res);
        res.write(&out[get_id(x + edge_offset, edge_offset, nx)]);
        buf.write(&buffer[get_id(x, filter_size, buffer_size)]);
    }
}

// calculate sum as saved sum of previous 4 lines from the last line of {buffer}
// and one new line with id {buffer_add_id}, updates last line of {buffer}
void write_buffer_sum(unsigned char* out, u_int16_t* buffer, int y, int buffer_add_id, int nx) {
    const int buffer_size = 16*((nx - edge_offset * 2) / 16) + 16;
    int buffer_delete_id = buffer_add_id + 1;
    buffer_delete_id = buffer_delete_id % filter_size;
    uint16_t_16 sum;
    uint16_t_16 buffer_sum;
    uint8_t_16 res;
    for (int x = 0; x <= (nx - edge_offset*2); x+=16) {
        sum.read(&buffer[get_id(x, filter_size, buffer_size)]);
        sum += uint16_t_16{&buffer[get_id(x, buffer_add_id, buffer_size)]};
        buffer_sum = sum - uint16_t_16{&buffer[get_id(x, buffer_delete_id, buffer_size)]};
        sum /= 25;
        convert_u16_u8_16(sum, res);
        res.write(&out[get_id(x + edge_offset, y, nx)]);
        buffer_sum.write(&buffer[get_id(x, filter_size, buffer_size)]);
    }
}

void box_filter_simd_5x5(unsigned char* in, unsigned char* out, int nx, int ny) {
    const int buffer_size = 16*((nx - edge_offset * 2) / 16) + 16;
    u_int16_t* buffer = new (std::align_val_t(32)) u_int16_t[
        buffer_size * (filter_size + 1)
    ];
    
    int buffer_start_id = 0;

    for (int y = 0; y != filter_size; y++) {
        update_buffer(in, buffer, y, y, nx);
    }
    
    first_buffer_sum(out, buffer, nx);

    for (int y = edge_offset + 1; y != ny - edge_offset; y++) {
        update_buffer(in, buffer, y, buffer_start_id, nx);
        write_buffer_sum(out, buffer, y, buffer_start_id, nx);
        
        buffer_start_id++;
        buffer_start_id %= filter_size;
    }

    free(buffer);
}
