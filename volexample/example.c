#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "paulslib.h"

/* 
	Simple test of the Marching Cubes code found in paulslib found here
   http://local.wasp.uwa.edu.au/~pbourke/libraries/
	Very poorly written with lots of assumptions, designed to give the
   basic idea of how to call PolygoniseCube().
	http://local.wasp.uwa.edu.au/~pbourke/geometry/polygonise/
	ps: code formateed for tab stops of 3 characters.
	pps: One would normally want to calculate normals as well.
*/

#define NX 200
#define NY 160
#define NZ 160

int main(int argc,char **argv)
{
	int i,j,k,l,n,c;
	short int ***data = NULL;
	short int isolevel = 128,themax = 0,themin = 255;
	GRIDCELL grid;
	TRIANGLE triangles[10];
	TRIANGLE *tri = NULL;
	int ntri = 0;
	FILE *fptr;

	// Check the command line, must at least contain the name of a raw volume file
	if (argc < 2) {
		fprintf(stderr,"Usage: %s [options] volumetricfilename\n",argv[0]);
		fprintf(stderr,"Options\n");
		fprintf(stderr,"    -i n    choose an isosurface value\n");
		exit(-1);
	}

	// Parse the command line
	for (i=1;i<argc;i++) {
		if (strcmp(argv[i],"-i") == 0)
			isolevel = atof(argv[i+1]);
	}

	// Malloc the volumetric data, hardwired size!
	data = malloc(NX*sizeof(short int **));
	for (i=0;i<NX;i++)
		data[i] = malloc(NY*sizeof(short int *));
	for (i=0;i<NX;i++)
		for (j=0;j<NY;j++)
			data[i][j] = malloc(NZ*sizeof(short int));

	// Open and read the raw data
	fprintf(stderr,"Reading data ...\n");
	if ((fptr = fopen(argv[argc-1],"rb")) == NULL) {
		fprintf(stderr,"File open failed\n");
		exit(-1);
	}
	for (k=0;k<NZ;k++) {
		for (j=0;j<NY;j++) {
			for (i=0;i<NX;i++) {
				if ((c = fgetc(fptr)) == EOF) {
					fprintf(stderr,"Unexpected end of file\n");
					exit(-1);
				}
				data[i][j][k] = c;
				if (c > themax)
					themax = c;
				if (c < themin)
					themin = c;
			}
		}
	}
	fclose(fptr);
	fprintf(stderr,"Volumetric data range: %d -> %d\n",themin,themax);

	// Polygonise the grid 
	fprintf(stderr,"Polygonising data ...\n");
	for (i=0;i<NX-1;i++) {
		if (i % (NX/10) == 0)
			fprintf(stderr,"   Slice %d of %d\n",i,NX);
		for (j=0;j<NY-1;j++) {
			for (k=0;k<NZ-1;k++) {
				grid.p[0].x = i;
				grid.p[0].y = j;
         	grid.p[0].z = k;
				grid.val[0] = data[i][j][k];
            grid.p[1].x = i+1;
            grid.p[1].y = j;
            grid.p[1].z = k; 
				grid.val[1] = data[i+1][j][k];
            grid.p[2].x = i+1;
            grid.p[2].y = j+1;
            grid.p[2].z = k;
				grid.val[2] = data[i+1][j+1][k];
            grid.p[3].x = i;
            grid.p[3].y = j+1;
            grid.p[3].z = k;
				grid.val[3] = data[i][j+1][k];
            grid.p[4].x = i;
            grid.p[4].y = j;
            grid.p[4].z = k+1;
				grid.val[4] = data[i][j][k+1];
            grid.p[5].x = i+1;
            grid.p[5].y = j;
            grid.p[5].z = k+1;
				grid.val[5] = data[i+1][j][k+1];
            grid.p[6].x = i+1;
            grid.p[6].y = j+1;
            grid.p[6].z = k+1;
				grid.val[6] = data[i+1][j+1][k+1];
            grid.p[7].x = i;
            grid.p[7].y = j+1;
            grid.p[7].z = k+1;
				grid.val[7] = data[i][j+1][k+1];
				n = PolygoniseCube(grid,isolevel,triangles);
				tri = realloc(tri,(ntri+n)*sizeof(TRIANGLE));
				for (l=0;l<n;l++)
					tri[ntri+l] = triangles[l];
				ntri += n;
			}
		}
	}
	fprintf(stderr,"Total of %d triangles\n",ntri);

	// Now do something with the triangles ....
	// Here I just write them to a geom file
	// http://local.wasp.uwa.edu.au/~pbourke/dataformats/geom/
	fprintf(stderr,"Writing triangles ...\n");
	if ((fptr = fopen("output.geom","w")) == NULL) {
		fprintf(stderr,"Failed to open output file\n");
		exit(-1);
	}
	for (i=0;i<ntri;i++) {
      fprintf(fptr,"f3 ");
      for (k=0;k<3;k++)  {
         fprintf(fptr,"%g %g %g ",tri[i].p[k].x,tri[i].p[k].y,tri[i].p[k].z);
      }
		fprintf(fptr,"0.5 0.5 0.5\n"); // colour
	}
	fclose(fptr);

	exit(0);
}



