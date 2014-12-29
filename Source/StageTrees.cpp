#include "StageTrees.h"
#include <math.h>
#include <stdint.h>

#include "Logwrapper.h"
#include "RotfaceConstants.def"

static int scales[NUM_SCALES];
static int rrotlut_tree[256][256][NROTS][NUM_SCALES];
static int crotlut_tree[256][256][NROTS][NUM_SCALES];

void precompute_tables() {

	// initialize scales
	for(int s = 0; s < NUM_SCALES; s++){
		scales[s] = (int)(MINFACESIZE * pow(SCALEFACTOR, s));
	}

	const float pi = 3.14159265f;
	uint8_t rcode;
	uint8_t ccode;

//#pragma omp parallel for
	for (int t = 0; t < NROTS; ++t){
		for (int i = 0; i < 256; ++i){
			for (int j = 0; j < 256; ++j) {
				for(int s = 0; s< NUM_SCALES; s++){

					//
					rcode = i;
					ccode = j;

					int r = *(int8_t*) &rcode;
					int c = *(int8_t*) &ccode;

					// -45deg + t * (90deg / rotations)
					float theta = (-pi / 4.0) + t * (pi / 2.0) / NROTS;

					//
					rrotlut_tree[i][j][t][s] = ((int) (+r * cos(theta) + c * sin(theta))) * scales[s];
					crotlut_tree[i][j][t][s] = ((int) (-r * sin(theta) + c * cos(theta))) * scales[s];

				}
			}
		}
	}

}

float stage0tree0(int r, int c, int s, int rotind, unsigned char pixels[],
		int ldim) {

	// depth 1
	int rs = (r << 8);
	int cs = (c << 8);
	int r1 = (rs + rrotlut_tree[240][43][rotind][s]) >> 8;
	int c1 = (cs + crotlut_tree[240][43][rotind][s]) >> 8;
	int r2 = (rs + rrotlut_tree[183][249][rotind][s]) >> 8;
	int c2 = (cs + crotlut_tree[183][249][rotind][s]) >> 8;

	if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
		// depth 2t
		r1 = (rs + rrotlut_tree[251][2][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[251][2][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[215][209][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[215][209][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3t
			r1 = (rs + rrotlut_tree[21][17][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[21][17][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[193][98][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[193][98][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4t
				r1 = (rs + rrotlut_tree[39][225][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[39][225][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[242][211][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[242][211][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5t
					r1 = (rs + rrotlut_tree[42][206][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[42][206][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[187][172][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[187][172][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6t
						r1 = (rs + rrotlut_tree[73][239][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[73][239][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[229][222][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[229][222][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7t
							return -0.974334597588;
						} else {
							// depth 7
							return -0.826380789280;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[32][47][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[32][47][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[128][202][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[128][202][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.854893088341;
						} else {
							// depth 7
							return -0.385955691338;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[18][207][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[18][207][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[237][219][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[237][219][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[204][10][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[204][10][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[180][188][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[180][188][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.950656354427;
						} else {
							// depth 7
							return -0.765583872795;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[217][6][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[217][6][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[75][23][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[75][23][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.604332089424;
						} else {
							// depth 7
							return 0.113412730396;
						}
					}

				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[204][166][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[204][166][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[8][56][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[8][56][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[188][12][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[188][12][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[51][1][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[51][1][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[190][245][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[190][245][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[99][177][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[99][177][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.674130260944;
						} else {
							// depth 7
							return 0.206060811877;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[190][251][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[190][251][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[132][222][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[132][222][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.089949846268;
						} else {
							// depth 7
							return 0.831169724464;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[178][24][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[178][24][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[245][224][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[245][224][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[167][228][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[167][228][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[229][221][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[229][221][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.911433219910;
						} else {
							// depth 7
							return -0.592331171036;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[133][42][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[133][42][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[14][205][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[14][205][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.173702031374;
						} else {
							// depth 7
							return -0.550832927227;
						}
					}

				}

			}

		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[189][3][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[189][3][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[221][221][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[221][221][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[4][252][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[4][252][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[236][231][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[236][231][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[3][59][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[3][59][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[175][186][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[175][186][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[25][211][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[25][211][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[241][2][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[241][2][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.971638441086;
						} else {
							// depth 7
							return -0.878741025925;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[230][5][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[230][5][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[0][251][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[0][251][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.841427862644;
						} else {
							// depth 7
							return -0.405906617641;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[21][53][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[21][53][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[235][22][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[235][22][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[30][205][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[30][205][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[90][180][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[90][180][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.861078441143;
						} else {
							// depth 7
							return -0.565645873547;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[207][5][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[207][5][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[65][172][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[65][172][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.341678798199;
						} else {
							// depth 7
							return 0.436785817146;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[30][221][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[30][221][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[129][82][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[129][82][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[232][52][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[232][52][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[49][54][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[49][54][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[9][62][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[9][62][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[241][49][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[241][49][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.277599900961;
						} else {
							// depth 7
							return 0.663319706917;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[1][31][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[1][31][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[235][22][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[235][22][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.659056663513;
						} else {
							// depth 7
							return 0.259162902832;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[8][60][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[8][60][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[150][94][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[150][94][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[168][242][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[168][242][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[85][14][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[85][14][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.381809026003;
						} else {
							// depth 7
							return 0.498613029718;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[109][56][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[109][56][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[193][21][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[193][21][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.952885448933;
						} else {
							// depth 7
							return 0.525480091572;
						}
					}

				}
			}

		}

	} else {
		// depth 2
		r1 = (rs + rrotlut_tree[240][7][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[240][7][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[226][59][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[226][59][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3
			r1 = (rs + rrotlut_tree[237][9][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[237][9][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[233][29][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[233][29][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[221][86][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[221][86][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[191][4][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[191][4][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[188][250][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[188][250][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[186][191][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[186][191][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[219][247][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[219][247][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[248][10][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[248][10][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.966683268547;
						} else {
							// depth 7
							return -0.872131705284;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[26][65][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[26][65][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[194][76][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[194][76][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.892893433571;
						} else {
							// depth 7
							return -0.531749784946;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[223][255][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[223][255][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[240][46][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[240][46][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[214][8][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[214][8][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[240][118][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[240][118][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.973446309566;
						} else {
							// depth 7
							return -0.925384104252;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[226][231][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[226][231][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[28][52][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[28][52][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.828240394592;
						} else {
							// depth 7
							return -0.970087885857;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[110][200][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[110][200][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[13][191][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[13][191][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[102][248][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[102][248][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[183][189][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[183][189][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[207][13][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[207][13][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[222][34][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[222][34][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.932922661304;
						} else {
							// depth 7
							return -0.647062838078;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[12][156][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[12][156][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[213][8][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[213][8][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.052182585001;
						} else {
							// depth 7
							return -0.698983013630;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[6][67][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[6][67][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[112][83][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[112][83][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[249][13][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[249][13][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[123][244][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[123][244][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.956757247448;
						} else {
							// depth 7
							return -0.824457705021;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[201][7][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[201][7][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[174][103][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[174][103][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.879487514496;
						} else {
							// depth 7
							return -0.592595696449;
						}
					}
				}
			}
		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[195][248][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[195][248][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[236][156][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[236][156][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[225][5][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[225][5][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[232][27][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[232][27][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[238][51][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[238][51][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[136][144][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[136][144][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[9][10][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[9][10][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[210][191][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[210][191][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.941427469254;
						} else {
							// depth 7
							return -0.773953557014;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[167][39][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[167][39][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[204][155][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[204][155][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.972676694393;
						} else {
							// depth 7
							return -0.915933609009;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[7][207][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[7][207][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[222][204][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[222][204][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[13][44][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[13][44][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[236][48][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[236][48][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.936830639839;
						} else {
							// depth 7
							return -0.748399853706;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[241][223][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[241][223][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[203][30][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[203][30][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.014226661064;
						} else {
							// depth 7
							return -0.584033548832;
						}
					}
				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[11][54][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[11][54][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[230][217][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[230][217][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[7][213][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[7][213][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[217][218][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[217][218][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[7][207][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[7][207][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[213][165][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[213][165][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.981993734837;
						} else {
							// depth 7
							return -0.792417347431;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[30][65][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[30][65][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[232][90][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[232][90][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.809621870518;
						} else {
							// depth 7
							return -0.344009965658;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[186][249][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[186][249][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[231][44][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[231][44][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[210][250][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[210][250][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[230][119][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[230][119][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.683100402355;
						} else {
							// depth 7
							return -0.017119366676;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[190][167][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[190][167][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[216][250][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[216][250][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.572113513947;
						} else {
							// depth 7
							return -0.415778398514;
						}
					}

				}

			}

		}

	}

}

float stage1tree0(int r, int c, int s, int rotind, unsigned char pixels[],
		int ldim) {

	// depth 1
	int rs = (r << 8);
	int cs = (c << 8);
	int r1 = (rs + rrotlut_tree[249][253][rotind][s]) >> 8;
	int c1 = (cs + crotlut_tree[249][253][rotind][s]) >> 8;
	int r2 = (rs + rrotlut_tree[238][228][rotind][s]) >> 8;
	int c2 = (cs + crotlut_tree[238][228][rotind][s]) >> 8;

	if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
		// depth 2t
		r1 = (rs + rrotlut_tree[7][98][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[7][98][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[11][96][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[11][96][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3t
			r1 = (rs + rrotlut_tree[60][116][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[60][116][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[63][117][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[63][117][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4t
				r1 = (rs + rrotlut_tree[174][236][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[174][236][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[175][231][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[175][231][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5t
					r1 = (rs + rrotlut_tree[160][36][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[160][36][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[140][44][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[140][44][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6t
						r1 = (rs + rrotlut_tree[114][218][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[114][218][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[119][215][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[119][215][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7t
							return -0.975477993488;
						} else {
							// depth 7
							return -0.852814257145;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[206][168][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[206][168][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[205][165][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[205][165][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.895339071751;
						} else {
							// depth 7
							return -0.349560678005;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[200][15][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[200][15][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[137][233][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[137][233][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[192][13][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[192][13][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[144][151][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[144][151][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.869919717312;
						} else {
							// depth 7
							return -0.634551942348;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[214][176][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[214][176][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[9][201][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[9][201][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.074232570827;
						} else {
							// depth 7
							return -0.664460897446;
						}
					}

				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[146][0][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[146][0][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[141][4][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[141][4][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[21][45][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[21][45][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[231][213][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[231][213][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[156][249][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[156][249][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[139][188][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[139][188][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.948739349842;
						} else {
							// depth 7
							return -0.850960135460;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[252][84][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[252][84][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[241][81][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[241][81][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.663039147854;
						} else {
							// depth 7
							return -0.090133666992;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[10][47][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[10][47][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[146][199][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[146][199][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[160][248][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[160][248][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[233][215][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[233][215][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.680266737938;
						} else {
							// depth 7
							return 0.000659679063;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[213][11][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[213][11][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[225][172][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[225][172][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.001295386115;
						} else {
							// depth 7
							return 0.726975500584;
						}
					}

				}

			}

		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[234][213][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[234][213][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[199][255][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[199][255][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[24][211][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[24][211][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[228][206][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[228][206][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[255][250][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[255][250][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[12][209][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[12][209][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[29][36][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[29][36][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[172][165][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[172][165][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.661668419838;
						} else {
							// depth 7
							return -0.212543666363;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[66][154][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[66][154][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[64][157][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[64][157][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.886499464512;
						} else {
							// depth 7
							return -0.429299235344;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[180][214][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[180][214][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[144][190][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[144][190][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[3][219][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[3][219][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[0][217][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[0][217][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.468944370747;
						} else {
							// depth 7
							return 0.469276309013;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[6][43][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[6][43][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[191][159][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[191][159][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.102824412286;
						} else {
							// depth 7
							return 0.616635382175;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[171][191][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[171][191][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[211][8][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[211][8][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[8][13][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[8][13][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[228][97][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[228][97][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[7][17][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[7][17][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[5][14][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[5][14][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.876669168472;
						} else {
							// depth 7
							return -0.177054643631;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[20][54][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[20][54][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[122][22][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[122][22][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.721834957600;
						} else {
							// depth 7
							return -0.150998339057;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[217][7][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[217][7][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[142][149][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[142][149][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[180][21][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[180][21][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[220][228][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[220][228][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.937341570854;
						} else {
							// depth 7
							return -0.855403661728;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[198][22][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[198][22][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[162][156][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[162][156][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.897110462189;
						} else {
							// depth 7
							return -0.666558265686;
						}
					}

				}
			}

		}

	} else {
		// depth 2
		r1 = (rs + rrotlut_tree[19][65][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[19][65][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[48][18][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[48][18][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3
			r1 = (rs + rrotlut_tree[8][212][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[8][212][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[125][50][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[125][50][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[13][202][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[13][202][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[62][10][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[62][10][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[36][195][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[36][195][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[78][14][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[78][14][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[30][51][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[30][51][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[85][11][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[85][11][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.933248460293;
						} else {
							// depth 7
							return -0.716899812222;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[2][185][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[2][185][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[1][191][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[1][191][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.754336833954;
						} else {
							// depth 7
							return -0.325810760260;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[192][190][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[192][190][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[194][175][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[194][175][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[12][247][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[12][247][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[106][209][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[106][209][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.785708546638;
						} else {
							// depth 7
							return -0.459146887064;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[211][241][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[211][241][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[160][165][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[160][165][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.490862727165;
						} else {
							// depth 7
							return 0.134068429470;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[20][49][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[20][49][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[23][24][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[23][24][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[24][57][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[24][57][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[248][224][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[248][224][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[31][47][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[31][47][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[181][168][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[181][168][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.784718811512;
						} else {
							// depth 7
							return -0.400187611580;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[9][214][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[9][214][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[71][14][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[71][14][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.554918289185;
						} else {
							// depth 7
							return 0.050002548844;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[16][221][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[16][221][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[231][23][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[231][23][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[113][250][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[113][250][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[208][110][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[208][110][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.715542137623;
						} else {
							// depth 7
							return -0.191850274801;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[216][2][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[216][2][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[224][241][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[224][241][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.126960545778;
						} else {
							// depth 7
							return 0.579422533512;
						}
					}
				}
			}
		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[16][204][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[16][204][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[129][26][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[129][26][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[185][223][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[185][223][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[217][39][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[217][39][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[229][39][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[229][39][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[150][219][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[150][219][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[22][45][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[22][45][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[2][37][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[2][37][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.676412999630;
						} else {
							// depth 7
							return -0.134757399559;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[85][6][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[85][6][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[84][2][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[84][2][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.882178604603;
						} else {
							// depth 7
							return 0.480735719204;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[14][211][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[14][211][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[222][198][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[222][198][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[27][217][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[27][217][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[236][64][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[236][64][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.649167239666;
						} else {
							// depth 7
							return 0.041268289089;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[198][22][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[198][22][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[173][123][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[173][123][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.016997646540;
						} else {
							// depth 7
							return 0.559891641140;
						}
					}
				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[198][0][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[198][0][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[189][98][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[189][98][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[191][6][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[191][6][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[242][223][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[242][223][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[214][18][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[214][18][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[187][222][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[187][222][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.636393189430;
						} else {
							// depth 7
							return -0.145660221577;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[191][225][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[191][225][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[116][215][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[116][215][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.219853803515;
						} else {
							// depth 7
							return 0.337155669928;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[239][232][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[239][232][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[174][30][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[174][30][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[188][239][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[188][239][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[154][84][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[154][84][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.253562778234;
						} else {
							// depth 7
							return 0.846079945564;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[181][85][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[181][85][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[102][23][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[102][23][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.446490526199;
						} else {
							// depth 7
							return -0.474858999252;
						}
					}

				}

			}

		}

	}

}

float stage1tree1(int r, int c, int s, int rotind, unsigned char pixels[],
		int ldim) {

	// depth 1
	int rs = (r << 8);
	int cs = (c << 8);
	int r1 = (rs + rrotlut_tree[11][61][rotind][s]) >> 8;
	int c1 = (cs + crotlut_tree[11][61][rotind][s]) >> 8;
	int r2 = (rs + rrotlut_tree[62][9][rotind][s]) >> 8;
	int c2 = (cs + crotlut_tree[62][9][rotind][s]) >> 8;

	if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
		// depth 2t
		r1 = (rs + rrotlut_tree[254][181][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[254][181][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[243][187][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[243][187][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3t
			r1 = (rs + rrotlut_tree[16][215][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[16][215][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[98][237][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[98][237][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4t
				r1 = (rs + rrotlut_tree[13][60][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[13][60][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[68][12][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[68][12][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5t
					r1 = (rs + rrotlut_tree[245][184][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[245][184][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[83][250][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[83][250][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6t
						r1 = (rs + rrotlut_tree[11][213][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[11][213][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[67][223][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[67][223][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7t
							return -0.902199447155;
						} else {
							// depth 7
							return -0.706977844238;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[225][148][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[225][148][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[9][201][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[9][201][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.470123350620;
						} else {
							// depth 7
							return -0.790496885777;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[27][211][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[27][211][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[239][59][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[239][59][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[9][115][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[9][115][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[8][120][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[8][120][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.677114009857;
						} else {
							// depth 7
							return -0.091747671366;
						}

					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[196][49][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[196][49][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[199][57][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[199][57][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.400468617678;
						} else {
							// depth 7
							return 0.421801954508;
						}
					}

				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[97][228][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[97][228][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[148][183][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[148][183][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[185][0][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[185][0][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[45][11][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[45][11][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[16][32][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[16][32][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[1][25][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[1][25][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.864519298077;
						} else {
							// depth 7
							return -0.644038558006;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[57][16][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[57][16][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[54][21][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[54][21][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.699232459068;
						} else {
							// depth 7
							return -0.215942010283;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[16][54][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[16][54][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[70][36][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[70][36][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[178][220][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[178][220][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[29][170][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[29][170][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.705655395985;
						} else {
							// depth 7
							return -0.221160084009;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[5][153][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[5][153][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[206][5][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[206][5][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.415612071753;
						} else {
							// depth 7
							return -0.217693626881;
						}
					}

				}

			}

		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[12][50][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[12][50][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[52][10][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[52][10][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[189][229][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[189][229][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[102][14][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[102][14][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[10][230][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[10][230][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[75][243][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[75][243][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[36][44][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[36][44][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[83][238][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[83][238][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.787978947163;
						} else {
							// depth 7
							return -0.385377585888;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[200][215][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[200][215][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[185][160][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[185][160][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.474431246519;
						} else {
							// depth 7
							return -0.066287674010;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[166][207][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[166][207][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[178][139][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[178][139][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[9][58][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[9][58][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[3][59][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[3][59][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.599859535694;
						} else {
							// depth 7
							return 0.287700414658;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[216][173][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[216][173][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[168][145][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[168][145][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.337295353413;
						} else {
							// depth 7
							return 0.320837259293;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[126][27][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[126][27][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[200][249][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[200][249][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[26][212][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[26][212][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[245][28][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[245][28][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[7][201][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[7][201][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[203][182][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[203][182][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.414592176676;
						} else {
							// depth 7
							return 0.255377858877;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[179][21][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[179][21][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[179][62][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[179][62][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.117075204849;
						} else {
							// depth 7
							return 0.734245538712;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[16][185][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[16][185][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[115][201][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[115][201][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[164][99][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[164][99][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[166][100][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[166][100][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.554289340973;
						} else {
							// depth 7
							return 0.112419269979;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[5][187][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[5][187][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[27][153][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[27][153][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.177964657545;
						} else {
							// depth 7
							return 0.310518503189;
						}
					}

				}
			}

		}

	} else {
		// depth 2
		r1 = (rs + rrotlut_tree[225][33][rotind][s]) >> 8;
		c1 = (cs + crotlut_tree[225][33][rotind][s]) >> 8;
		r2 = (rs + rrotlut_tree[97][29][rotind][s]) >> 8;
		c2 = (cs + crotlut_tree[97][29][rotind][s]) >> 8;

		if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
			// depth 3
			r1 = (rs + rrotlut_tree[18][209][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[18][209][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[96][57][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[96][57][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[18][200][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[18][200][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[92][179][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[92][179][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[183][42][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[183][42][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[111][240][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[111][240][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[29][206][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[29][206][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[224][215][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[224][215][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.818760216236;
						} else {
							// depth 7
							return -0.488692671061;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[11][204][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[11][204][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[224][231][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[224][231][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.323849469423;
						} else {
							// depth 7
							return 0.260142445564;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[199][5][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[199][5][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[86][102][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[86][102][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[17][4][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[17][4][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[87][22][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[87][22][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.331430763006;
						} else {
							// depth 7
							return 0.184700861573;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[21][54][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[21][54][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[188][167][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[188][167][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.092600792646;
						} else {
							// depth 7
							return 0.592089653015;
						}
					}
				}
			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[197][241][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[197][241][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[164][172][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[164][172][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[56][40][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[56][40][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[238][226][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[238][226][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[204][15][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[204][15][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[225][235][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[225][235][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.408500522375;
						} else {
							// depth 7
							return 0.141405522823;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[254][184][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[254][184][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[247][184][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[247][184][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.134414598346;
						} else {
							// depth 7
							return 0.843505859375;
						}
					}
				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[222][1][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[222][1][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[235][211][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[235][211][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[181][5][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[181][5][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[16][95][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[16][95][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.304444938898;
						} else {
							// depth 7
							return 0.328749328852;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[221][30][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[221][30][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[221][34][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[221][34][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.695842981339;
						} else {
							// depth 7
							return 0.932765781879;
						}
					}
				}
			}
		} else {
			// depth 3
			r1 = (rs + rrotlut_tree[17][179][rotind][s]) >> 8;
			c1 = (cs + crotlut_tree[17][179][rotind][s]) >> 8;
			r2 = (rs + rrotlut_tree[22][173][rotind][s]) >> 8;
			c2 = (cs + crotlut_tree[22][173][rotind][s]) >> 8;

			if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
				// depth 4
				r1 = (rs + rrotlut_tree[239][45][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[239][45][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[113][239][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[113][239][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[202][94][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[202][94][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[202][97][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[202][97][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[16][2][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[16][2][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[217][67][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[217][67][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.732744634151;
						} else {
							// depth 7
							return -0.259727418423;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[232][28][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[232][28][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[36][39][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[36][39][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.634609222412;
						} else {
							// depth 7
							return 0.078541472554;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[90][18][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[90][18][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[233][31][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[233][31][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[235][51][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[235][51][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[107][9][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[107][9][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.464937388897;
						} else {
							// depth 7
							return -0.862183094025;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[15][43][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[15][43][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[230][33][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[230][33][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.598266005516;
						} else {
							// depth 7
							return 0.033121552318;
						}
					}
				}

			} else {
				// depth 4
				r1 = (rs + rrotlut_tree[230][228][rotind][s]) >> 8;
				c1 = (cs + crotlut_tree[230][228][rotind][s]) >> 8;
				r2 = (rs + rrotlut_tree[22][186][rotind][s]) >> 8;
				c2 = (cs + crotlut_tree[22][186][rotind][s]) >> 8;

				if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
					// depth 5
					r1 = (rs + rrotlut_tree[90][201][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[90][201][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[234][34][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[234][34][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[90][3][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[90][3][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[176][160][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[176][160][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.479300022125;
						} else {
							// depth 7
							return 0.241974070668;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[198][234][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[198][234][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[193][170][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[193][170][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return 0.152431309223;
						} else {
							// depth 7
							return 0.737960398197;
						}
					}

				} else {
					// depth 5
					r1 = (rs + rrotlut_tree[102][253][rotind][s]) >> 8;
					c1 = (cs + crotlut_tree[102][253][rotind][s]) >> 8;
					r2 = (rs + rrotlut_tree[198][185][rotind][s]) >> 8;
					c2 = (cs + crotlut_tree[198][185][rotind][s]) >> 8;

					if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
						// depth 6
						r1 = (rs + rrotlut_tree[2][179][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[2][179][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[3][180][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[3][180][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.738606393337;
						} else {
							// depth 7
							return 0.437300384045;
						}
					} else {
						// depth 6
						r1 = (rs + rrotlut_tree[21][217][rotind][s]) >> 8;
						c1 = (cs + crotlut_tree[21][217][rotind][s]) >> 8;
						r2 = (rs + rrotlut_tree[228][74][rotind][s]) >> 8;
						c2 = (cs + crotlut_tree[228][74][rotind][s]) >> 8;

						if (pixels[r1 * ldim + c1] <= pixels[r2 * ldim + c2]) {
							// depth 7
							return -0.535599291325;
						} else {
							// depth 7
							return 0.350604712963;
						}
					}

				}

			}

		}

	}

}
