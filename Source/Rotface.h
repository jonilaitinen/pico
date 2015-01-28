#ifndef ROTFACE_H
#define ROTFACE_H

#include "opencv2/opencv.hpp"
#include <stdint.h>

#include "RotfaceConstants.def"

struct FaceTrack {
	int x;
	int y;
	int diameter;
	float rotation;
    float quality;
};

class Rotface {
public:

	Rotface();
    
    void precompute_rotluts();
	std::vector<FaceTrack> detectFaces(unsigned char* greyData,
                                       int frameWidth, int frameHeight);

private:
	static int rrotlut[256][256][NROTS][NUM_SCALES];
	static int crotlut[256][256][NROTS][NUM_SCALES];
	// contains number of detections for each rotation
	static int ndetections[NROTS];

	static float faceQuality[NROTS][DETECTIONS_PER_ROTATION];
	static float faceYCoords[NROTS][DETECTIONS_PER_ROTATION];
	static float faceXCoords[NROTS][DETECTIONS_PER_ROTATION];
	static float faceDiameters[NROTS][DETECTIONS_PER_ROTATION];

	std::vector<FaceTrack> detectedFaces;


	void find_rotated_objects(unsigned char pixels[], int nrows, int ncols, int ldim,
							  float stridefactor, float smax, float qcutoff);
	int bintest(int tcode, int r, int c, int s, int rotind, unsigned char pixels[], int ldim, int stage);
	float get_dtree_output(int8_t tree[], int r, int c, int s, int rotind, unsigned char pixels[], int nrows, int ncols, int ldim, int stage);
	int get_dtree_size(int8_t dtree[]);
	bool odet_classify_region(void* od, float* o, float r_f, float c_f, float s_f, int rotind, unsigned char pixels[], int nrows, int ncols, int ldim);
	float get_overlap(float r1, float c1, float s1, float r2, float c2, float s2, float ratio);
	void ccdfs(int a[], int i, float rs[], float cs[], float ss[], float ratio, int n);
	int find_connected_components(int a[], float rs[], float cs[], float ss[], float ratio, int n);
	int cluster_detections(float rs[], float cs[], float ss[], float qs[], float ratio, int n, float qcutoff);

};

#endif
