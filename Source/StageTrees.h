#ifndef STAGETREES_H
#define STAGETREES_H

void precompute_tables();

float stage0tree0(int r, int c, int s, int rotind, unsigned char pixels[], int ldim);
float stage1tree0(int r, int c, int s, int rotind, unsigned char pixels[], int ldim);
float stage1tree1(int r, int c, int s, int rotind, unsigned char pixels[], int ldim);

#endif
