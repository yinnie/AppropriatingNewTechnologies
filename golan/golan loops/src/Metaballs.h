#ifndef METABALLS_H
#define METABALLS_H

#define MAX_BALLS    64
#define MAX_VERTICES 60000
#define MAX_INDICES  60000

#include "ofMain.h"

struct SBall
{
	float p[3];
	float v[3];
	float a[3];
	float t;
	float m;
};

#define FVF_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
struct SVertex
{
	float v[3];
	float n[3];
	float t[2];
};

class CMetaballs
{
public:
	CMetaballs (int nB);

	void  Compute();
	void  Update(float fDeltaTime);
	void  Render();
	void  UpdateBallsFromPoints (int nPoints, ofPoint *points);
	void  UpdateBallsFromPointsAndMasses (int nPoints, ofPoint *points, float *masses);

	void  SetGridSize(int nSize);
	int   getNBalls();

    bool            bRenderWithTexture;
	ofImage         spheremap;
	ofTexture 		texColor;

protected:
	float ComputeEnergy(float x, float y, float z);
	void  ComputeNormal(SVertex *pVertex);
	inline float ComputeGridPointEnergy(int x, int y, int z);
	int   ComputeGridVoxel(int x, int y, int z);

	bool  IsGridPointComputed(int x, int y, int z);
	bool  IsGridVoxelComputed(int x, int y, int z);
	bool  IsGridVoxelInList(int x, int y, int z);
	void  SetGridPointComputed(int x, int y, int z);
	void  SetGridVoxelComputed(int x, int y, int z);
	void  SetGridVoxelInList(int x, int y, int z);

	inline float ConvertGridPointToWorldCoordinate(int x);
	int   ConvertWorldCoordinateToGridPoint(float x);
	void  AddNeighborsToList(int nCase, int x, int y, int z);
	void  AddNeighbor(int x, int y, int z);

	float  m_fLevel;

	int    m_nNumBalls;
	SBall  m_Balls[MAX_BALLS];

	int    m_nNumOpenVoxels;
	int    m_nMaxOpenVoxels;
	int   *m_pOpenVoxels;

	int    m_nGridSize;
	int    m_nGridSizep1;
	float  m_fVoxelSize;

	float *m_pfGridEnergy;
	char  *m_pnGridPointStatus;
	char  *m_pnGridVoxelStatus;

    int             m_nMaxNumVertices;
    int             m_nMaxNumIndices;

	int             m_nNumVertices;
	int             m_nNumIndices;
	SVertex        *m_pVertices;
	unsigned short *m_pIndices;
};

#endif
