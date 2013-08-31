#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*int8 	Unsigned 8-bit integer.
sint8 	Signed 8-bit integer.
int16_LSB 	Little-endian encoded unsigned 16-bit integer.
int16_MSB 	Big-endian encoded unsigned 16-bit integer.
int16_LSB-MSB 	Little-endian followed by big-endian encoded unsigned 16-bit integer.
sint16_LSB 	Little-endian encoded signed 16-bit integer.
sint16_MSB 	Big-endian encoded signed 16-bit integer.
sint16_LSB-MSB 	Little-endian followed by big-endian encoded signed 16-bit integer.
int32_LSB 	Little-endian encoded unsigned 32-bit integer.
int32_MSB 	Big-endian encoded unsigned 32-bit integer.
int32_LSB-MSB 	Little-endian followed by big-endian encoded unsigned 32-bit integer.
sint32_LSB 	Little-endian encoded signed 32-bit integer.
sint32_MSB 	Big-endian encoded signed 32-bit integer.
sint32_LSB-MSB 	Little-endian followed by big-endian encoded signed 32-bit integer. 
*/

typedef struct Directory {
    uint8_t dir_length;
    uint8_t extended_attr_record_sz;
    uint32_t LBA[2];
    uint32_t data_length[2];
    uint8_t date_and_time[7];
    uint8_t file_flags;
    uint8_t interleaved;
    uint8_t interleaved_gap;
    uint16_t vol_seq_nb[2];
    uint8_t id_length;
    uint8_t identifier;
} __attribute((packed)) Directory;

typedef struct PVD { /* Primary Volume Descriptor */
    uint8_t type;
    uint8_t identifier[5];
    uint8_t version;
    uint8_t unused1;
    uint8_t sys_id[32];
    uint8_t vol_id[32];
    uint8_t unused2[8];
    uint32_t vol_space_sz[2];
    uint8_t unused3[32];
    uint16_t vol_set_sz[2];
    uint16_t vol_seq_nb[2];
    uint16_t logical_block_sz[2];
    uint32_t path_table_sz[2];
    uint32_t LPath_location;
    uint32_t LPath_optional;
    uint32_t MPath_location;
    uint32_t MPath_optional;
    uint8_t  dir_root_entry[34];
    uint8_t  vol_set_id[128];
    uint8_t  publisher_id[128];
    uint8_t  data_preparer_id[128];
    uint8_t  app_id[128];
    uint8_t  copyright_file_id[38];
    uint8_t  abstract_file_id[36];
    uint8_t  bibliographic_file_id[37];
    uint8_t  vol_create_date_time[17];
    uint8_t  vol_modification_date_time[17];
    uint8_t  vol_expiration_date_time[17];
    uint8_t  vol_effective_date_time[17];
    uint8_t  file_struct_ver;
    uint8_t  unused4;
    uint8_t  app_used[512];
    uint8_t  reserved[653];
} __attribute((packed)) PVD;

void skip(FILE* ptr, int lba) {
    fseek(ptr, 2048 * lba, SEEK_SET);
}

void fill_block(FILE* iso, uint8_t* ptr) {
    int i = 0;

    while (i < 2048) {
        fscanf(iso, "%c", &ptr[i]);
        ++i;
    }
}

void disp_block(uint8_t* ptr) {
    int i;

    for (i = 0; i < 2048; ++i) {
        printf("%c", ptr[i]);
    }
}

void get_file_name(uint8_t* ptr) {
    uint32_t i;
    int k;
    int sk = 0;
    Directory* p;

    for (k = 0; k < 10; ++k) {
        p = (Directory*) &ptr[sk];

        printf("sk: %i\n", sk);
        printf("dir_length: %u\n", p->dir_length);
        printf("id_length: %u\n", p->id_length);
        printf("lba: %u\n", p->LBA[0]);
        printf("data_length: %u\n", p->data_length[0]);


        for (i = 0; i < (uint32_t) p->id_length; ++i) {
            printf("%c", ptr[sk+33+i]);
        }

        sk += p->dir_length;
        printf("\n");
    }
}

int main(int argc, char** argv) {
    FILE* iso;
    uint8_t block[2048];
    Directory* dir;
    PVD* pvd;
    char* p;
    int i;
    int lba;

    iso = fopen(argv[1], "rb");
    skip(iso, 16);
    fill_block(iso, &block[0]);

    pvd = (PVD*) block;
    dir = (Directory*) &pvd->dir_root_entry;

    lba = dir->LBA[0];
    skip(iso, lba);
    fill_block(iso, &block[0]);

    get_file_name(&block[0]);

    skip(iso, 746);
    fill_block(iso, &block[0]);
    disp_block(&block[0]);


    return 0;
}
