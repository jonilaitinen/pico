#ifndef STAGESELECTION_H
#define STAGESELECTION_H

void initTree();
int classify_region(float* o, float r_f, float c_f, float s_f, int rotind, unsigned char pixels[], int nrows, int ncols, int ldim);

#endif
