/*
 * Source: txt2bin.c
 * Author: Scott Linder <scott.b.linder@wmich.edu>
 * Description: Convert textual (ASCII) representation of B-Tree index into a
 * binary representation.
 *
 * Input: Text from Standard Input. All integral values are expected to fit in
 * a signed byte (-127 to 127 inclusive) and all character data is expected to
 * be in a single byte encoding (ASCII). The first line contains three integral
 * values (M, rootPtr, N) separated by whitespace. The remaining N lines have M
 * triplets of values (TP, KV, DRP) separated by whitespace, followed by one
 * trailing TP. The TP and DRP are all integral values, and KV are 3 ASCII
 * characters each.
 *
 * Output: Binary data to Standard Output. There is no padding between any
 * values. The first three bytes are M, rootPtr, N, in that order. Following
 * are N groups of (M + (M - 1) * 3 + (M - 1)) bytes each. These are arranged
 * as:
 *  [M TP bytes][(M - 1) * 3 KV bytes][M - 1 DRP bytes]
 * in the order in which they were read in, but now grouped together by type
 * rather than being separated into triplets. To be completely clear, here are
 * the sizes for the various parts, keeping in mind there will be _no_ padding
 * between these values.
 *  Value       Bytes
 *  -----------------
 *  M            1
 *  rootPtr      1
 *  N            1
 *  TP           1
 *  KV           3
 *  DRP          1
 * Thus, the header is in total 3 bytes, followed by N groups of (M * 5 + 1)
 * bytes each.
 *
 * Example Usage: txt2bin <input.txt >output.bin
 *
 * Errata: I'm sure I messed up somewhere; email me if I did, or you have
 * questions!
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define KV_LEN 3
#define STR_(X) #X
#define STR(X) STR_(X)

int main(int argc, char *argv[]) {
    int i, j;
    int nscan;
    signed char m, r, n;

    nscan = scanf("%hhu %hhu %hhu", &m, &r, &n);
    assert(nscan == 3);

    fwrite(&m, sizeof(signed char), 1, stdout);
    fwrite(&r, sizeof(signed char), 1, stdout);
    fwrite(&n, sizeof(signed char), 1, stdout);

    signed char *tps = (signed char *) calloc(m, sizeof(signed char));
    char *kvs = (char *) calloc((m - 1) * KV_LEN, sizeof(char));
    signed char *drps = (signed char *) calloc(m - 1, sizeof(signed char));

    for (i = 0; i < n; ++i) {
        for (j = 0; j < m - 1; ++j) {
            nscan = scanf("%hhu %" STR(KV_LEN) "c %hhu",
                            tps + j,
                            kvs + (j * KV_LEN),
                            drps + j);
            assert(nscan == 3);
        }
        scanf("%hhu", tps + j);
        fwrite(tps, sizeof(signed char), m, stdout);
        fwrite(kvs, sizeof(char), (m - 1) * KV_LEN, stdout);
        fwrite(drps, sizeof(signed char), m - 1, stdout);
    }

    free(tps);
    free(kvs);
    free(drps);

    return EXIT_SUCCESS;
}
