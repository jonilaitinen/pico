/*
 *	Copyright (c) 2013, Nenad Markus
 *	All rights reserved.
 *
 *	This is an implementation of the algorithm described in the following paper:
 *		N. Markus, M. Frljak, I. S. Pandzic, J. Ahlberg and R. Forchheimer,
 *		A method for object detection based on pixel intensity comparisons,
 *		http://arxiv.org/abs/1305.4537
 *
 *	Redistribution and use of this program as source code or in binary form, with or without modifications, are permitted provided that the following conditions are met:
 *		1. Redistributions may not be sold, nor may they be used in a commercial product or activity without prior permission from the copyright holder (contact him at nenad.markus@fer.hr).
 *		2. Redistributions may not be used for military purposes.
 *		3. Any published work which utilizes this program shall include the reference to the paper available at http://arxiv.org/abs/1305.4537
 *		4. Redistributions must retain the above copyright notice and the reference to the algorithm on which the implementation is based on, this list of conditions and the following disclaimer.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
	
*/

#include <stdint.h>

/*
	
*/

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))

#define BINTEST(f, r, c, sr, sc, pixels, nrows, ncols, ldim) \
		(	\
			((pixels)[((256*(r)+((int8_t*)&(f))[0]*(sr))/256)*(ldim)+((256*(c)+((int8_t*)&(f))[1]*(sc))/256)]<=(pixels)[((256*(r)+((int8_t*)&(f))[2]*(sr))/256)*(ldim)+((256*(c)+((int8_t*)&(f))[3]*(sc))/256)])	\
		)

/*
	
*/
	float get_dtree_output(int8_t tree[], int r, int c, int sr, int sc, uint8_t pixels[], int nrows, int ncols, int ldim)
	{
		int d, idx;

		int32_t tdepth;
		int32_t* tcodes;
		float* tlut;

		//
		tdepth = *(int32_t*)&tree[0];
		tcodes = (int32_t*)&tree[sizeof(int32_t)];
		tlut = (float*)&tree[(1+((1<<tdepth)-1))*sizeof(int32_t)];

		//
		idx = 0;

		for(d=0; d<tdepth; ++d)
		{
			if( BINTEST(tcodes[idx], r, c, sr, sc, pixels, nrows, ncols, ldim) )
				idx = 2*idx + 2;
			else
				idx = 2*idx + 1;
		}

		//
		return tlut[idx - ((1<<tdepth)-1)];
	}

	int get_dtree_size(int8_t dtree[])
	{
		int32_t tdepth;

		//
		tdepth = *(int32_t*)&dtree[0];

		//
		return 1*sizeof(int32_t) + ((1<<tdepth)-1)*sizeof(int32_t) + (1<<tdepth)*sizeof(float);
	}

/*
	
*/
	int classify_region(void* od, float* o, float r, float c, float s, uint8_t pixels[], int nrows, int ncols, int ldim)
	{
		int8_t* ptr;
		int loc;

		int32_t nstages;

		int i, j;
		int ir, ic, isr, isc;
		float threshold;

		//
		ptr = (int8_t*)od;
		loc = 0;

		*o = 0.0f;

		//
		ir = (int)( r + s**(float*)&ptr[0*sizeof(float)] );
		ic = (int)( c + s**(float*)&ptr[1*sizeof(float)] );
		isr = (int)( s**(float*)&ptr[2*sizeof(float)] );
		isc = (int)( s**(float*)&ptr[3*sizeof(float)] );

		loc += 4*sizeof(float);

		//
		nstages = *(int32_t*)&ptr[loc];
		loc += sizeof(int32_t);

		if(!nstages)
			return 0;

		//
		i = 0;

		while(i<nstages)
		{
			int numtrees;

			//
			numtrees = *(int32_t*)&ptr[loc];
			loc += sizeof(int32_t);

			//
			for(j=0; j<numtrees; ++j)
			{
				//
				*o += get_dtree_output(&ptr[loc], ir, ic, isr, isc, pixels, nrows, ncols, ldim);

				loc += get_dtree_size(&ptr[loc]);
			}

			//
			threshold = *(float*)&ptr[loc];
			loc += sizeof(float);

			//
			if(*o <= threshold)
				return -1;

			//
			++i;
		}

		//
		*o = *o - threshold;

		//
		return 1;
	}

/*
	
*/

	float get_overlap(float r1, float c1, float s1, float r2, float c2, float s2)
	{
		float overr, overc;

		//
		overr = MAX(0, MIN(r1+s1/2, r2+s2/2) - MAX(r1-s1/2, r2-s2/2));
		overc = MAX(0, MIN(c1+s1/2, c2+s2/2) - MAX(c1-s1/2, c2-s2/2));

		//
		return overr*overc/(s1*s1+s2*s2-overr*overc);
	}

	void ccdfs(int a[], int i, float rs[], float cs[], float ss[], int n)
	{
		int j;

		//
		for(j=0; j<n; ++j)
			if(a[j]==0 && get_overlap(rs[i], cs[i], ss[i], rs[j], cs[j], ss[j])>0.3f)
			{
				//
				a[j] = a[i];

				//
				ccdfs(a, j, rs, cs, ss, n);
			}
	}

	int find_connected_components(int a[], float rs[], float cs[], float ss[], int n)
	{
		int i, ncc, cc;

		//
		if(!n)
			return 0;

		//
		for(i=0; i<n; ++i)
			a[i] = 0;

		//
		ncc = 0;
		cc = 1;

		for(i=0; i<n; ++i)
			if(a[i] == 0)
			{
				//
				a[i] = cc;

				//
				ccdfs(a, i, rs, cs, ss, n);

				//
				++ncc;
				++cc;
			}

		//
		return ncc;
	}

	int cluster_detections(float rs[], float cs[], float ss[], float qs[], int n)
	{
		int idx, ncc, cc;
		int a[4096];

		//
		ncc = find_connected_components(a, rs, cs, ss, n);

		if(!ncc)
			return 0;

		//
		idx = 0;

		for(cc=1; cc<=ncc; ++cc)
		{
			int i, k;

			float sumqs=0.0f, sumrs=0.0f, sumcs=0.0f, sumss=0.0f;

			//
			k = 0;

			for(i=0; i<n; ++i)
				if(a[i] == cc)
				{
					sumqs += qs[i];
					sumrs += rs[i];
					sumcs += cs[i];
					sumss += ss[i];

					++k;
				}

			//
			qs[idx] = sumqs; // accumulated confidence measure

			//
			rs[idx] = sumrs/k;
			cs[idx] = sumcs/k;
			ss[idx] = sumss/k;

			//
			++idx;
		}

		//
		return idx;
	}

	int find_objects(float rs[], float cs[], float ss[], float qs[], int maxndetections,
						void* od,
						void* pixels, int nrows, int ncols, int ldim,
						float scalefactor, float stridefactor, float minsize, float maxsize,
						int clusterdetections)
	{
		float s;
		int ndetections;

		//
		ndetections = 0;

		s = minsize;

		while(s<=maxsize)
		{
			float r, c, dr, dc;

			//
			dr = MAX(stridefactor*s, 1.0f);
			dc = MAX(stridefactor*s, 1.0f);

			//
			for(r=s/2+1; r<=nrows-s/2-1; r+=dr)
				for(c=s/2+1; c<=ncols-s/2-1; c+=dc)
				{
					float q;

					if(classify_region(od, &q, r, c, s, pixels, nrows, ncols, ldim)>0)
					{
						if(ndetections < maxndetections)
						{
							qs[ndetections] = q;
							rs[ndetections] = r;
							cs[ndetections] = c;
							ss[ndetections] = s;

							++ndetections;
						}
					}
				}

			//
			s = scalefactor*s;
		}

		//
		if(clusterdetections)
			ndetections = cluster_detections(rs, cs, ss, qs, ndetections);

		//
		return ndetections;
	}