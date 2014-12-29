#include "StageSelection.h"
#include "StageTrees.h"
#include "Logwrapper.h"

#define STAGES 20

float stageThresholds[20] = {-0.71409035, -0.9868884, -1.227593, -1.5806265, -1.9818006,
							 -2.3669276, -2.5755382, -2.6076317, -2.6718197, -2.7199612,
							 -2.7039137, -2.7039137, -2.6878667, -2.6557732, -2.5915852,
							 -2.5755382, -2.4792566, -2.3829741, -2.2866926, -2.174364};

float processStage(int stage, int r, int c, int s, int rotind, unsigned char pixels[], int ldim);

void initTree(){
	precompute_tables();
}

int classify_region(float* o, float r_f, float c_f, float s_f, int rotind, unsigned char pixels[], int nrows, int ncols, int ldim){

	*o = 0.0f;

	for(int stage = 0; stage < STAGES; stage++){

		*o += processStage(stage, r_f, c_f, s_f, rotind, pixels, ldim);

		//LOGD("score: %f threshold: %f stage: %i", *o, stageThresholds[stage], stage);
		if (*o <= stageThresholds[stage]){
			//LOGD("stage: %i", stage);
			return -1;
		}
	}

	//LOGD("stage: %i", 19);
	//*o = *o - finalThreshold;
	return 1;

}

float processStage(int stage, int r, int c, int s, int rotind, unsigned char pixels[], int ldim) {

	float sum = 0;

	if (stage == 0) {
		return stage0tree0(r, c, s, rotind, pixels, ldim);
	} else if (stage == 1) {
		sum += stage1tree0(r, c, s, rotind, pixels, ldim);
		sum += stage1tree1(r, c, s, rotind, pixels, ldim);
	}
	/*
	else if (stage == 2) {
		sum += stage2tree0(r, c, s, rotind, pixels, ldim);
		sum += stage2tree1(r, c, s, rotind, pixels, ldim);
		sum += stage2tree2(r, c, s, rotind, pixels, ldim);
	} else if (stage == 3) {
		sum += stage3tree0(r, c, s, rotind, pixels, ldim);
		sum += stage3tree1(r, c, s, rotind, pixels, ldim);
		sum += stage3tree2(r, c, s, rotind, pixels, ldim);
		sum += stage3tree3(r, c, s, rotind, pixels, ldim);
	} else if (stage == 4) {
		sum += stage4tree0(r, c, s, rotind, pixels, ldim);
		sum += stage4tree1(r, c, s, rotind, pixels, ldim);
		sum += stage4tree2(r, c, s, rotind, pixels, ldim);
		sum += stage4tree3(r, c, s, rotind, pixels, ldim);
		sum += stage4tree4(r, c, s, rotind, pixels, ldim);
	} else if (stage == 5) {
		sum += stage5tree0(r, c, s, rotind, pixels, ldim);
		sum += stage5tree1(r, c, s, rotind, pixels, ldim);
		sum += stage5tree2(r, c, s, rotind, pixels, ldim);
		sum += stage5tree3(r, c, s, rotind, pixels, ldim);
		sum += stage5tree4(r, c, s, rotind, pixels, ldim);
		sum += stage5tree5(r, c, s, rotind, pixels, ldim);
		sum += stage5tree6(r, c, s, rotind, pixels, ldim);
		sum += stage5tree7(r, c, s, rotind, pixels, ldim);
		sum += stage5tree8(r, c, s, rotind, pixels, ldim);
		sum += stage5tree9(r, c, s, rotind, pixels, ldim);
	} else if (stage == 6) {
		sum += stage6tree0(r, c, s, rotind, pixels, ldim);
		sum += stage6tree1(r, c, s, rotind, pixels, ldim);
		sum += stage6tree2(r, c, s, rotind, pixels, ldim);
		sum += stage6tree3(r, c, s, rotind, pixels, ldim);
		sum += stage6tree4(r, c, s, rotind, pixels, ldim);
		sum += stage6tree5(r, c, s, rotind, pixels, ldim);
		sum += stage6tree6(r, c, s, rotind, pixels, ldim);
		sum += stage6tree7(r, c, s, rotind, pixels, ldim);
		sum += stage6tree8(r, c, s, rotind, pixels, ldim);
		sum += stage6tree9(r, c, s, rotind, pixels, ldim);
		sum += stage6tree10(r, c, s, rotind, pixels, ldim);
		sum += stage6tree11(r, c, s, rotind, pixels, ldim);
		sum += stage6tree12(r, c, s, rotind, pixels, ldim);
		sum += stage6tree13(r, c, s, rotind, pixels, ldim);
		sum += stage6tree14(r, c, s, rotind, pixels, ldim);
		sum += stage6tree15(r, c, s, rotind, pixels, ldim);
		sum += stage6tree16(r, c, s, rotind, pixels, ldim);
		sum += stage6tree17(r, c, s, rotind, pixels, ldim);
		sum += stage6tree18(r, c, s, rotind, pixels, ldim);
		sum += stage6tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 7) {
		sum += stage7tree0(r, c, s, rotind, pixels, ldim);
		sum += stage7tree1(r, c, s, rotind, pixels, ldim);
		sum += stage7tree2(r, c, s, rotind, pixels, ldim);
		sum += stage7tree3(r, c, s, rotind, pixels, ldim);
		sum += stage7tree4(r, c, s, rotind, pixels, ldim);
		sum += stage7tree5(r, c, s, rotind, pixels, ldim);
		sum += stage7tree6(r, c, s, rotind, pixels, ldim);
		sum += stage7tree7(r, c, s, rotind, pixels, ldim);
		sum += stage7tree8(r, c, s, rotind, pixels, ldim);
		sum += stage7tree9(r, c, s, rotind, pixels, ldim);
		sum += stage7tree10(r, c, s, rotind, pixels, ldim);
		sum += stage7tree11(r, c, s, rotind, pixels, ldim);
		sum += stage7tree12(r, c, s, rotind, pixels, ldim);
		sum += stage7tree13(r, c, s, rotind, pixels, ldim);
		sum += stage7tree14(r, c, s, rotind, pixels, ldim);
		sum += stage7tree15(r, c, s, rotind, pixels, ldim);
		sum += stage7tree16(r, c, s, rotind, pixels, ldim);
		sum += stage7tree17(r, c, s, rotind, pixels, ldim);
		sum += stage7tree18(r, c, s, rotind, pixels, ldim);
		sum += stage7tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 8) {
		sum += stage8tree0(r, c, s, rotind, pixels, ldim);
		sum += stage8tree1(r, c, s, rotind, pixels, ldim);
		sum += stage8tree2(r, c, s, rotind, pixels, ldim);
		sum += stage8tree3(r, c, s, rotind, pixels, ldim);
		sum += stage8tree4(r, c, s, rotind, pixels, ldim);
		sum += stage8tree5(r, c, s, rotind, pixels, ldim);
		sum += stage8tree6(r, c, s, rotind, pixels, ldim);
		sum += stage8tree7(r, c, s, rotind, pixels, ldim);
		sum += stage8tree8(r, c, s, rotind, pixels, ldim);
		sum += stage8tree9(r, c, s, rotind, pixels, ldim);
		sum += stage8tree10(r, c, s, rotind, pixels, ldim);
		sum += stage8tree11(r, c, s, rotind, pixels, ldim);
		sum += stage8tree12(r, c, s, rotind, pixels, ldim);
		sum += stage8tree13(r, c, s, rotind, pixels, ldim);
		sum += stage8tree14(r, c, s, rotind, pixels, ldim);
		sum += stage8tree15(r, c, s, rotind, pixels, ldim);
		sum += stage8tree16(r, c, s, rotind, pixels, ldim);
		sum += stage8tree17(r, c, s, rotind, pixels, ldim);
		sum += stage8tree18(r, c, s, rotind, pixels, ldim);
		sum += stage8tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 9) {
		sum += stage9tree0(r, c, s, rotind, pixels, ldim);
		sum += stage9tree1(r, c, s, rotind, pixels, ldim);
		sum += stage9tree2(r, c, s, rotind, pixels, ldim);
		sum += stage9tree3(r, c, s, rotind, pixels, ldim);
		sum += stage9tree4(r, c, s, rotind, pixels, ldim);
		sum += stage9tree5(r, c, s, rotind, pixels, ldim);
		sum += stage9tree6(r, c, s, rotind, pixels, ldim);
		sum += stage9tree7(r, c, s, rotind, pixels, ldim);
		sum += stage9tree8(r, c, s, rotind, pixels, ldim);
		sum += stage9tree9(r, c, s, rotind, pixels, ldim);
		sum += stage9tree10(r, c, s, rotind, pixels, ldim);
		sum += stage9tree11(r, c, s, rotind, pixels, ldim);
		sum += stage9tree12(r, c, s, rotind, pixels, ldim);
		sum += stage9tree13(r, c, s, rotind, pixels, ldim);
		sum += stage9tree14(r, c, s, rotind, pixels, ldim);
		sum += stage9tree15(r, c, s, rotind, pixels, ldim);
		sum += stage9tree16(r, c, s, rotind, pixels, ldim);
		sum += stage9tree17(r, c, s, rotind, pixels, ldim);
		sum += stage9tree18(r, c, s, rotind, pixels, ldim);
		sum += stage9tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 10) {
		sum += stage10tree0(r, c, s, rotind, pixels, ldim);
		sum += stage10tree1(r, c, s, rotind, pixels, ldim);
		sum += stage10tree2(r, c, s, rotind, pixels, ldim);
		sum += stage10tree3(r, c, s, rotind, pixels, ldim);
		sum += stage10tree4(r, c, s, rotind, pixels, ldim);
		sum += stage10tree5(r, c, s, rotind, pixels, ldim);
		sum += stage10tree6(r, c, s, rotind, pixels, ldim);
		sum += stage10tree7(r, c, s, rotind, pixels, ldim);
		sum += stage10tree8(r, c, s, rotind, pixels, ldim);
		sum += stage10tree9(r, c, s, rotind, pixels, ldim);
		sum += stage10tree10(r, c, s, rotind, pixels, ldim);
		sum += stage10tree11(r, c, s, rotind, pixels, ldim);
		sum += stage10tree12(r, c, s, rotind, pixels, ldim);
		sum += stage10tree13(r, c, s, rotind, pixels, ldim);
		sum += stage10tree14(r, c, s, rotind, pixels, ldim);
		sum += stage10tree15(r, c, s, rotind, pixels, ldim);
		sum += stage10tree16(r, c, s, rotind, pixels, ldim);
		sum += stage10tree17(r, c, s, rotind, pixels, ldim);
		sum += stage10tree18(r, c, s, rotind, pixels, ldim);
		sum += stage10tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 11) {
		sum += stage11tree0(r, c, s, rotind, pixels, ldim);
		sum += stage11tree1(r, c, s, rotind, pixels, ldim);
		sum += stage11tree2(r, c, s, rotind, pixels, ldim);
		sum += stage11tree3(r, c, s, rotind, pixels, ldim);
		sum += stage11tree4(r, c, s, rotind, pixels, ldim);
		sum += stage11tree5(r, c, s, rotind, pixels, ldim);
		sum += stage11tree6(r, c, s, rotind, pixels, ldim);
		sum += stage11tree7(r, c, s, rotind, pixels, ldim);
		sum += stage11tree8(r, c, s, rotind, pixels, ldim);
		sum += stage11tree9(r, c, s, rotind, pixels, ldim);
		sum += stage11tree10(r, c, s, rotind, pixels, ldim);
		sum += stage11tree11(r, c, s, rotind, pixels, ldim);
		sum += stage11tree12(r, c, s, rotind, pixels, ldim);
		sum += stage11tree13(r, c, s, rotind, pixels, ldim);
		sum += stage11tree14(r, c, s, rotind, pixels, ldim);
		sum += stage11tree15(r, c, s, rotind, pixels, ldim);
		sum += stage11tree16(r, c, s, rotind, pixels, ldim);
		sum += stage11tree17(r, c, s, rotind, pixels, ldim);
		sum += stage11tree18(r, c, s, rotind, pixels, ldim);
		sum += stage11tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 12) {
		sum += stage12tree0(r, c, s, rotind, pixels, ldim);
		sum += stage12tree1(r, c, s, rotind, pixels, ldim);
		sum += stage12tree2(r, c, s, rotind, pixels, ldim);
		sum += stage12tree3(r, c, s, rotind, pixels, ldim);
		sum += stage12tree4(r, c, s, rotind, pixels, ldim);
		sum += stage12tree5(r, c, s, rotind, pixels, ldim);
		sum += stage12tree6(r, c, s, rotind, pixels, ldim);
		sum += stage12tree7(r, c, s, rotind, pixels, ldim);
		sum += stage12tree8(r, c, s, rotind, pixels, ldim);
		sum += stage12tree9(r, c, s, rotind, pixels, ldim);
		sum += stage12tree10(r, c, s, rotind, pixels, ldim);
		sum += stage12tree11(r, c, s, rotind, pixels, ldim);
		sum += stage12tree12(r, c, s, rotind, pixels, ldim);
		sum += stage12tree13(r, c, s, rotind, pixels, ldim);
		sum += stage12tree14(r, c, s, rotind, pixels, ldim);
		sum += stage12tree15(r, c, s, rotind, pixels, ldim);
		sum += stage12tree16(r, c, s, rotind, pixels, ldim);
		sum += stage12tree17(r, c, s, rotind, pixels, ldim);
		sum += stage12tree18(r, c, s, rotind, pixels, ldim);
		sum += stage12tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 13) {
		sum += stage13tree0(r, c, s, rotind, pixels, ldim);
		sum += stage13tree1(r, c, s, rotind, pixels, ldim);
		sum += stage13tree2(r, c, s, rotind, pixels, ldim);
		sum += stage13tree3(r, c, s, rotind, pixels, ldim);
		sum += stage13tree4(r, c, s, rotind, pixels, ldim);
		sum += stage13tree5(r, c, s, rotind, pixels, ldim);
		sum += stage13tree6(r, c, s, rotind, pixels, ldim);
		sum += stage13tree7(r, c, s, rotind, pixels, ldim);
		sum += stage13tree8(r, c, s, rotind, pixels, ldim);
		sum += stage13tree9(r, c, s, rotind, pixels, ldim);
		sum += stage13tree10(r, c, s, rotind, pixels, ldim);
		sum += stage13tree11(r, c, s, rotind, pixels, ldim);
		sum += stage13tree12(r, c, s, rotind, pixels, ldim);
		sum += stage13tree13(r, c, s, rotind, pixels, ldim);
		sum += stage13tree14(r, c, s, rotind, pixels, ldim);
		sum += stage13tree15(r, c, s, rotind, pixels, ldim);
		sum += stage13tree16(r, c, s, rotind, pixels, ldim);
		sum += stage13tree17(r, c, s, rotind, pixels, ldim);
		sum += stage13tree18(r, c, s, rotind, pixels, ldim);
		sum += stage13tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 14) {
		sum += stage14tree0(r, c, s, rotind, pixels, ldim);
		sum += stage14tree1(r, c, s, rotind, pixels, ldim);
		sum += stage14tree2(r, c, s, rotind, pixels, ldim);
		sum += stage14tree3(r, c, s, rotind, pixels, ldim);
		sum += stage14tree4(r, c, s, rotind, pixels, ldim);
		sum += stage14tree5(r, c, s, rotind, pixels, ldim);
		sum += stage14tree6(r, c, s, rotind, pixels, ldim);
		sum += stage14tree7(r, c, s, rotind, pixels, ldim);
		sum += stage14tree8(r, c, s, rotind, pixels, ldim);
		sum += stage14tree9(r, c, s, rotind, pixels, ldim);
		sum += stage14tree10(r, c, s, rotind, pixels, ldim);
		sum += stage14tree11(r, c, s, rotind, pixels, ldim);
		sum += stage14tree12(r, c, s, rotind, pixels, ldim);
		sum += stage14tree13(r, c, s, rotind, pixels, ldim);
		sum += stage14tree14(r, c, s, rotind, pixels, ldim);
		sum += stage14tree15(r, c, s, rotind, pixels, ldim);
		sum += stage14tree16(r, c, s, rotind, pixels, ldim);
		sum += stage14tree17(r, c, s, rotind, pixels, ldim);
		sum += stage14tree18(r, c, s, rotind, pixels, ldim);
		sum += stage14tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 15) {
		sum += stage15tree0(r, c, s, rotind, pixels, ldim);
		sum += stage15tree1(r, c, s, rotind, pixels, ldim);
		sum += stage15tree2(r, c, s, rotind, pixels, ldim);
		sum += stage15tree3(r, c, s, rotind, pixels, ldim);
		sum += stage15tree4(r, c, s, rotind, pixels, ldim);
		sum += stage15tree5(r, c, s, rotind, pixels, ldim);
		sum += stage15tree6(r, c, s, rotind, pixels, ldim);
		sum += stage15tree7(r, c, s, rotind, pixels, ldim);
		sum += stage15tree8(r, c, s, rotind, pixels, ldim);
		sum += stage15tree9(r, c, s, rotind, pixels, ldim);
		sum += stage15tree10(r, c, s, rotind, pixels, ldim);
		sum += stage15tree11(r, c, s, rotind, pixels, ldim);
		sum += stage15tree12(r, c, s, rotind, pixels, ldim);
		sum += stage15tree13(r, c, s, rotind, pixels, ldim);
		sum += stage15tree14(r, c, s, rotind, pixels, ldim);
		sum += stage15tree15(r, c, s, rotind, pixels, ldim);
		sum += stage15tree16(r, c, s, rotind, pixels, ldim);
		sum += stage15tree17(r, c, s, rotind, pixels, ldim);
		sum += stage15tree18(r, c, s, rotind, pixels, ldim);
		sum += stage15tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 16) {
		sum += stage16tree0(r, c, s, rotind, pixels, ldim);
		sum += stage16tree1(r, c, s, rotind, pixels, ldim);
		sum += stage16tree2(r, c, s, rotind, pixels, ldim);
		sum += stage16tree3(r, c, s, rotind, pixels, ldim);
		sum += stage16tree4(r, c, s, rotind, pixels, ldim);
		sum += stage16tree5(r, c, s, rotind, pixels, ldim);
		sum += stage16tree6(r, c, s, rotind, pixels, ldim);
		sum += stage16tree7(r, c, s, rotind, pixels, ldim);
		sum += stage16tree8(r, c, s, rotind, pixels, ldim);
		sum += stage16tree9(r, c, s, rotind, pixels, ldim);
		sum += stage16tree10(r, c, s, rotind, pixels, ldim);
		sum += stage16tree11(r, c, s, rotind, pixels, ldim);
		sum += stage16tree12(r, c, s, rotind, pixels, ldim);
		sum += stage16tree13(r, c, s, rotind, pixels, ldim);
		sum += stage16tree14(r, c, s, rotind, pixels, ldim);
		sum += stage16tree15(r, c, s, rotind, pixels, ldim);
		sum += stage16tree16(r, c, s, rotind, pixels, ldim);
		sum += stage16tree17(r, c, s, rotind, pixels, ldim);
		sum += stage16tree18(r, c, s, rotind, pixels, ldim);
		sum += stage16tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 17) {
		sum += stage17tree0(r, c, s, rotind, pixels, ldim);
		sum += stage17tree1(r, c, s, rotind, pixels, ldim);
		sum += stage17tree2(r, c, s, rotind, pixels, ldim);
		sum += stage17tree3(r, c, s, rotind, pixels, ldim);
		sum += stage17tree4(r, c, s, rotind, pixels, ldim);
		sum += stage17tree5(r, c, s, rotind, pixels, ldim);
		sum += stage17tree6(r, c, s, rotind, pixels, ldim);
		sum += stage17tree7(r, c, s, rotind, pixels, ldim);
		sum += stage17tree8(r, c, s, rotind, pixels, ldim);
		sum += stage17tree9(r, c, s, rotind, pixels, ldim);
		sum += stage17tree10(r, c, s, rotind, pixels, ldim);
		sum += stage17tree11(r, c, s, rotind, pixels, ldim);
		sum += stage17tree12(r, c, s, rotind, pixels, ldim);
		sum += stage17tree13(r, c, s, rotind, pixels, ldim);
		sum += stage17tree14(r, c, s, rotind, pixels, ldim);
		sum += stage17tree15(r, c, s, rotind, pixels, ldim);
		sum += stage17tree16(r, c, s, rotind, pixels, ldim);
		sum += stage17tree17(r, c, s, rotind, pixels, ldim);
		sum += stage17tree18(r, c, s, rotind, pixels, ldim);
		sum += stage17tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 18) {
		sum += stage18tree0(r, c, s, rotind, pixels, ldim);
		sum += stage18tree1(r, c, s, rotind, pixels, ldim);
		sum += stage18tree2(r, c, s, rotind, pixels, ldim);
		sum += stage18tree3(r, c, s, rotind, pixels, ldim);
		sum += stage18tree4(r, c, s, rotind, pixels, ldim);
		sum += stage18tree5(r, c, s, rotind, pixels, ldim);
		sum += stage18tree6(r, c, s, rotind, pixels, ldim);
		sum += stage18tree7(r, c, s, rotind, pixels, ldim);
		sum += stage18tree8(r, c, s, rotind, pixels, ldim);
		sum += stage18tree9(r, c, s, rotind, pixels, ldim);
		sum += stage18tree10(r, c, s, rotind, pixels, ldim);
		sum += stage18tree11(r, c, s, rotind, pixels, ldim);
		sum += stage18tree12(r, c, s, rotind, pixels, ldim);
		sum += stage18tree13(r, c, s, rotind, pixels, ldim);
		sum += stage18tree14(r, c, s, rotind, pixels, ldim);
		sum += stage18tree15(r, c, s, rotind, pixels, ldim);
		sum += stage18tree16(r, c, s, rotind, pixels, ldim);
		sum += stage18tree17(r, c, s, rotind, pixels, ldim);
		sum += stage18tree18(r, c, s, rotind, pixels, ldim);
		sum += stage18tree19(r, c, s, rotind, pixels, ldim);
	} else if (stage == 19) {
		sum += stage19tree0(r, c, s, rotind, pixels, ldim);
		sum += stage19tree1(r, c, s, rotind, pixels, ldim);
		sum += stage19tree2(r, c, s, rotind, pixels, ldim);
		sum += stage19tree3(r, c, s, rotind, pixels, ldim);
		sum += stage19tree4(r, c, s, rotind, pixels, ldim);
		sum += stage19tree5(r, c, s, rotind, pixels, ldim);
		sum += stage19tree6(r, c, s, rotind, pixels, ldim);
		sum += stage19tree7(r, c, s, rotind, pixels, ldim);
		sum += stage19tree8(r, c, s, rotind, pixels, ldim);
		sum += stage19tree9(r, c, s, rotind, pixels, ldim);
		sum += stage19tree10(r, c, s, rotind, pixels, ldim);
		sum += stage19tree11(r, c, s, rotind, pixels, ldim);
		sum += stage19tree12(r, c, s, rotind, pixels, ldim);
		sum += stage19tree13(r, c, s, rotind, pixels, ldim);
		sum += stage19tree14(r, c, s, rotind, pixels, ldim);
		sum += stage19tree15(r, c, s, rotind, pixels, ldim);
		sum += stage19tree16(r, c, s, rotind, pixels, ldim);
		sum += stage19tree17(r, c, s, rotind, pixels, ldim);
		sum += stage19tree18(r, c, s, rotind, pixels, ldim);
		sum += stage19tree19(r, c, s, rotind, pixels, ldim);
	}
	*/
	LOGD("sum: %f", sum);
	return sum;
}
