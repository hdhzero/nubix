#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "utils.h"

FILE* iso;
uint8_t buffer[2048];
int lba;

void ls() {
    uint32_t i;
    uint32_t sk = 0;
    Directory* dir;

    fill_block(iso, buffer);

    while (1) {
        if (sk >= 2048) break;

        dir = (Directory*) &buffer[sk];
        if (dir->dir_length == 0) break;

        for (i = 0; i < dir->id_length; ++i) {
            printf("%c", buffer[sk + 33 + i]);
        }

        printf("    %u\n", dir->LBA[0]);
        sk += dir->dir_length;
    }
}

void cd() {
    fseek(iso, 2048 * lba, SEEK_SET);
}

void pwd() {
    printf("current_dir: %u\n", lba);
}

void cat() {
    int i;

    fill_block(iso, buffer);

    for (i = 0; i < 2048; ++i) {
        printf("%c", buffer[i]);
    }    
}

int main(int argc, char** argv) {
    int option;
    Directory* dir;
    PVD* pvd;

    iso = fopen(argv[1], "rb");
    skip(iso, 16);
    fill_block(iso, buffer);

    pvd = (PVD*) buffer;
    dir = (Directory*) &pvd->dir_root_entry;

    lba = dir->LBA[0];
    skip(iso, lba);

    while (1) {
        printf("exit: 0   cd: 1 (<lba>|-1)     pwd: 2     ls: 3     cat: 4\n");
        scanf("%i", &option);

        if (option == 0) {
            break;
        } else if (option == 1) {
            scanf("%i", &lba);
            cd();
        } else if (option == 2) {
            pwd();
        } else if (option == 3) {
            ls();
        } else if (option == 4) {
            cat();
        }
    }

    return 0;
}
