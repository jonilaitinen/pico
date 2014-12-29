#include "Rotface.h"
#include "Logwrapper.h"

#include <omp.h>

#include "facefinder.array"

//#include "StageTrees.h"
float stageThresholds[20] = {-0.714090347290, -0.986888408661, -1.227592945099, -1.580626487732, -1.981800556183,
							 -2.366927623749, -2.575538158417, -2.607631683350, -2.671819686890, -2.719961166382,
							 -2.703913688660, -2.703913688660, -2.687866687775, -2.655773162842, -2.591585159302,
							 -2.575538158417, -2.479256629944, -2.382974147797, -2.286692619324, -2.174364089966};

int Rotface::rrotlut[256][256][NROTS][NUM_SCALES];
int Rotface::crotlut[256][256][NROTS][NUM_SCALES];
int Rotface::ndetections[NROTS];

float Rotface::faceQuality[NROTS][DETECTIONS_PER_ROTATION];
float Rotface::faceYCoords[NROTS][DETECTIONS_PER_ROTATION];
float Rotface::faceXCoords[NROTS][DETECTIONS_PER_ROTATION];
float Rotface::faceDiameters[NROTS][DETECTIONS_PER_ROTATION];

float qcutoff = 7.5f;
float scales[NUM_SCALES];
// 0, 30, -30 degrees  = {0.0f, 0.5235f, -0.5235f};
float rotations[NROTS];

Rotface::Rotface() {

}

void Rotface::precompute_rotluts() {

	const float pi = 3.14159265f;
	uint8_t rcode;
	uint8_t ccode;

	// initialize scales
	for(int s = 0; s < NUM_SCALES; s++){
		scales[s] = MINFACESIZE * std::pow(SCALEFACTOR, s);
	}

#pragma omp parallel for
	for (int t = 0; t < NROTS; ++t){

		// -30deg + t * (60deg / rotations)
		float theta = (-pi / 6.0) + t * (pi / 3.0) / NROTS;
		rotations[t] = theta;

		for (int i = 0; i < 256; ++i){
			for (int j = 0; j < 256; ++j) {
				for(int s = 0; s < NUM_SCALES; s++){
					//
					rcode = i;
					ccode = j;

					int r = *(int8_t*) &rcode;
					int c = *(int8_t*) &ccode;

					//
					rrotlut[i][j][t][s] = ((int) (+r * cos(theta) + c * sin(theta))) * (int)scales[s];
					crotlut[i][j][t][s] = ((int) (-r * sin(theta) + c * cos(theta))) * (int)scales[s];
				}
			}
		}
	}

	//precompute_tables();
    
    pixels = (uint8_t*) malloc(640 * 480);
}

void Rotface::copyFrame(unsigned char* grayFrame, int width, int height){
    frameWidth = width;
    frameHeight = height;
    memcpy(pixels, grayFrame, width * height);
}

std::vector<FaceTrack> Rotface::detectFaces() {

    //LOGD("rows: %i, cols: %i, step: %i \n", frameHeight, frameWidth, frameWidth);

	for(int i = 0; i < NROTS; i++){
		ndetections[i] = 0;
	}

	find_rotated_objects(pixels, frameHeight, frameWidth, frameWidth, STRIDE_FACTOR, frameHeight, qcutoff);

    detectedFaces.clear();
    
	for(int rot = 0; rot < NROTS; rot++){
		for (int i = 0; i < ndetections[rot]; ++i) {
            FaceTrack face;
			face.x = faceXCoords[rot][i];
			face.y = faceYCoords[rot][i];
			face.diameter = faceDiameters[rot][i];
			face.rotation = rotations[rot];
			detectedFaces.push_back(face);
		}
	}
    
	return detectedFaces;
}

void Rotface::find_rotated_objects(unsigned char pixels[],
		int nrows, int ncols, int ldim, float stridefactor,
		float smax, float qcutoff) {

#pragma omp parallel for
	for (int t = 0; t < NROTS; ++t){

		for(int scale = 0; scale < NUM_SCALES; scale++) {

			float s = scales[scale];
			float row, col, dr, dc;
			float step = 0.707107f * s;

			dr = dc = MAX(stridefactor * s, 1.0f);

			for (row = step + 1; row <= nrows - step - 1; row += dr){

				for (col = step + 1; col <= ncols - step - 1; col += dc) {
					float q;
					if (odet_classify_region(facefinder, &q, row, col, scale, t, pixels, nrows, ncols, ldim)) {
						if (ndetections[t] < DETECTIONS_PER_ROTATION) {
							faceQuality[t][ndetections[t]] = q;
							faceYCoords[t][ndetections[t]] = row;
							faceXCoords[t][ndetections[t]] = col;
							faceDiameters[t][ndetections[t]] = s;

							++ndetections[t];
						}
					}
				}
			}
		}
	}

	if (CLUSTER_DETECTIONS){
#pragma omp parallel for
		for(int rot = 0; rot < NROTS; rot++){
			ndetections[rot] = cluster_detections(faceYCoords[rot], faceXCoords[rot], faceDiameters[rot], faceQuality[rot], FACE_RATIO, ndetections[rot], qcutoff);
		}
	}

}


int Rotface::bintest(int tcode, int r, int c, int s, int rotind,
		unsigned char pixels[], int ldim, int stage) {

	uint8_t* p = (uint8_t*) &tcode;

	int r1 = ((r << 8) + rrotlut[p[0]][p[1]][rotind][s]) >> 8;
	int c1 = ((c << 8) + crotlut[p[0]][p[1]][rotind][s]) >> 8;

	int r2 = ((r << 8) + rrotlut[p[2]][p[3]][rotind][s]) >> 8;
	int c2 = ((c << 8) + crotlut[p[2]][p[3]][rotind][s]) >> 8;

	//
	bool result = pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2];

	return result;
}

float Rotface::get_dtree_output(int8_t tree[], int r, int c, int s, int rotind,
		unsigned char pixels[], int nrows, int ncols, int ldim, int stage) {

	//toutdim = *(int32*)&tree[0];
	int32_t tdepth = *(int32_t*) &tree[4];
	int32_t* tcodes = (int32_t*) &tree[2 * 4];
	int treeindex = (2 + ((1 << tdepth) - 1)) * 4;
	float* tlut = (float*) &tree[treeindex];

	//
	int idx = 0;

	for (int d = 0; d < tdepth; ++d) {
		if (bintest(tcodes[idx], r, c, s, rotind, pixels, ldim, stage)){
			idx = 2 * idx + 1;
		}
		else {
			idx = 2 * idx + 2;
		}
	}

	//
	int lutidx = idx - ((1 << tdepth) - 1);
	float score = tlut[lutidx];
	return score;
}

int Rotface::get_dtree_size(int8_t dtree[]) {
	int32_t tdepth, toutdim;

	//
	toutdim = *(int32_t*) &dtree[0];
	tdepth = *(int32_t*) &dtree[sizeof(int32_t)];

	//
	return 2 * sizeof(int32_t) + ((1 << tdepth) - 1) * sizeof(int32_t) + (1 << tdepth) * toutdim * sizeof(float);
}

bool Rotface::odet_classify_region(void* od, float* o, float r_f, float c_f,
		float s_f, int rotind, unsigned char pixels[], int nrows, int ncols, int ldim) {

	int32_t nstages;

	int i, j;
	int row, col, scale;
	float treshold;

	//
	int8_t* ptr = (int8_t*) od;
	int loc = 0;

	*o = 0.0f;

	// ratio seems to be not used in detection..
	//float ratio = *(float*) &ptr[loc];
	loc += 4;

	//
	nstages = *(int32_t*) &ptr[loc];
	loc += 4;

	if (!nstages){
		return false;
	}

	//
	row = (int) r_f;
	col = (int) c_f;
	scale = (int) s_f;

	//
	i = 0;

	while (i < nstages) {
		int numtrees;

		numtrees = *(int32_t*) &ptr[loc];
		loc += 4;
        /*
		// use unrolled tree for first stage..
		if(i == 0){
			*o += stage0tree0(row, col, scale, rotind, pixels, ldim);
		}

		// and second stage
		if(i == 1){
			*o += stage1tree0(row, col, scale, rotind, pixels, ldim);
			*o += stage1tree1(row, col, scale, rotind, pixels, ldim);
		}
        */
		for (j = 0; j < numtrees; ++j) {
			//if(i > 1){ // use trees from the table for other levels
				*o += get_dtree_output(&ptr[loc], row, col, scale, rotind, pixels, nrows, ncols, ldim, i);
			//}
			loc += get_dtree_size(&ptr[loc]);
		}

		treshold = stageThresholds[i];
		loc += 4;

		if (*o <= treshold){
			return false;
		}

		++i;
	}

	return true;
}

#ifndef MAX
	#define MAX(a, b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
	#define MIN(a, b) ((a)<(b)?(a):(b))
#endif

float Rotface::get_overlap(float r1, float c1, float s1, float r2, float c2, float s2,
		float ratio) {

	float overr = MAX(0, MIN(r1 + s1/2, r2 + s2/2) - MAX(r1 - s1/2, r2 - s2/2));
	float overc = MAX(0, MIN(c1 + ratio * s1/2, c2 + ratio * s2/2) - MAX(c1 - ratio * s1/2, c2 - ratio * s2/2));

	return overr * overc / (ratio * s1 * s1 + ratio * s2 * s2 - overr * overc);
}

void Rotface::ccdfs(int a[], int i, float rs[], float cs[], float ss[],
		float ratio, int n) {
	int j;

	for (j = 0; j < n; ++j){
		if (a[j] == 0 && get_overlap(rs[i], cs[i], ss[i], rs[j], cs[j], ss[j], ratio) > 0.3f) {
			//
			a[j] = a[i];
			ccdfs(a, j, rs, cs, ss, ratio, n);
		}
	}
}

int Rotface::find_connected_components(int a[], float rs[], float cs[], float ss[], float ratio, int n) {

	int i, ncc, cc;

	if (!n){
		return 0;
	}

	//
	for (i = 0; i < n; ++i){
		a[i] = 0;
	}

	//
	ncc = 0;
	cc = 1;

	for (i = 0; i < n; ++i){
		if (a[i] == 0) {

			a[i] = cc;

			ccdfs(a, i, rs, cs, ss, ratio, n);

			++ncc;
			++cc;
		}
	}

	return ncc;
}

int Rotface::cluster_detections(float rs[], float cs[], float ss[], float qs[], float ratio, int n, float qcutoff) {
	int idx, ncc, cc;
	int a[4096];

	//
	ncc = find_connected_components(a, rs, cs, ss, ratio, n);

	if (!ncc){
		return 0;
	}

	//
	idx = 0;

	for (cc = 1; cc <= ncc; ++cc) {
		int i, k;

		float sumqs = 0.0f, sumrs = 0.0f, sumcs = 0.0f, sumss = 0.0f;

		//
		k = 0;

		for (i = 0; i < n; ++i){
			if (a[i] == cc) {
				sumqs += qs[i];
				sumrs += rs[i];
				sumcs += cs[i];
				sumss += ss[i];

				++k;
			}
		}
		if (sumqs >= qcutoff) {
			//
			qs[idx] = sumqs; // accumulated confidence measure
			rs[idx] = sumrs / k;
			cs[idx] = sumcs / k;
			ss[idx] = sumss / k;

			//
			++idx;
		}
	}

	//
	return idx;
}


