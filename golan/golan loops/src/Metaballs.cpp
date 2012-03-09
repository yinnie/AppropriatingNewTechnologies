#include "metaballs.h"
#include "marchingcubes.h"
#include <memory.h>



//=============================================================================
CMetaballs::CMetaballs (int nB)
{
	m_fLevel    = 100.0f;
	m_nNumBalls = nB;

	m_nGridSize = 0;
	m_nGridSizep1 = 0;

	m_nMaxOpenVoxels = 32;
	m_pOpenVoxels    = new int[m_nMaxOpenVoxels*3];

	m_nNumOpenVoxels    = 0;
	m_pfGridEnergy      = 0;
	m_pnGridPointStatus = 0;
	m_pnGridVoxelStatus = 0;

	m_nNumVertices = 0;
	m_nNumIndices  = 0;
	m_pVertices    = 0;
	m_pIndices     = 0;

    srand(ofGetElapsedTimeMillis());

	for( int i = 0; i < m_nNumBalls; i++ ){
		m_Balls[i].p[0] = 0;
		m_Balls[i].p[1] = 0;
		m_Balls[i].p[2] = 0;
		m_Balls[i].v[0] = 0;
		m_Balls[i].v[1] = 0;
		m_Balls[i].v[2] = 0;
		m_Balls[i].a[0] = (float(rand())/RAND_MAX*2-1)/2;
		m_Balls[i].a[1] = (float(rand())/RAND_MAX*2-1)/2;
		m_Balls[i].a[2] = (float(rand())/RAND_MAX*2-1)/2;
		m_Balls[i].t = float(rand())/RAND_MAX;
		m_Balls[i].m = 1;
	}

    bRenderWithTexture = false;
    if (bRenderWithTexture){
        spheremap.loadImage("images/spheremap.jpg");
        texColor.allocate(spheremap.width,spheremap.height,GL_RGB);
        texColor.loadData(spheremap.getPixels(), spheremap.width, spheremap.height, GL_RGB);
    }

}

//=============================================================================
void CMetaballs::SetGridSize(int nSize)
{
	m_fVoxelSize = 2.0/float(nSize);
	m_nGridSize   = nSize;
	m_nGridSizep1 = m_nGridSize+1;

	m_pfGridEnergy      = new float[(nSize+1)*(nSize+1)*(nSize+1)];
	m_pnGridPointStatus = new char [(nSize+1)*(nSize+1)*(nSize+1)];
	m_pnGridVoxelStatus = new char [ nSize*    nSize*    nSize];

    m_nMaxNumVertices = MAX_VERTICES;
    m_nMaxNumIndices  = MAX_INDICES;

	m_pVertices = new SVertex[m_nMaxNumVertices];
	for (int i=0; i<m_nMaxNumVertices; i++){
	    for (int j=0; j<3; j++){
            m_pVertices[i].v[j] = 0;
            m_pVertices[i].n[j] = 0;
	    }
	    for (int j=0; j<2; j++){
            m_pVertices[i].t[j] = 0;
	    }
	}

	m_pIndices = new unsigned short[m_nMaxNumIndices];
	for (int i=0; i<m_nMaxNumIndices; i++){
	    m_pIndices[i] = 0;
	}
}

//=============================================================================
int CMetaballs::getNBalls(){
    return m_nNumBalls;
}
//=============================================================================
void CMetaballs::UpdateBallsFromPoints (int nPoints, ofPoint *points){
    if (nPoints <= m_nNumBalls){
        float lo = -1 + m_fVoxelSize*1.0;
        float hi =  1 - m_fVoxelSize*1.0;

        for (int i=0; i<nPoints; i++){
            float x = ofClamp(points[i].x, lo, hi);
            float y = ofClamp(points[i].y, lo, hi);
            float z = ofClamp(points[i].z, lo, hi);

            m_Balls[i].p[0] = points[i].x;
            m_Balls[i].p[1] = points[i].y;
            m_Balls[i].p[2] = points[i].z;

            m_Balls[i].v[0] = 0;
            m_Balls[i].v[1] = 0;
            m_Balls[i].v[2] = 0;
        }
    }
}


//=============================================================================
void CMetaballs::UpdateBallsFromPointsAndMasses (int nPoints, ofPoint *points, float *masses){
    if (nPoints <= m_nNumBalls){
        float lo = -1 + m_fVoxelSize*1.0;
        float hi =  1 - m_fVoxelSize*1.0;

        for (int i=0; i<nPoints; i++){
            float x = ofClamp(points[i].x, lo, hi);
            float y = ofClamp(points[i].y, lo, hi);
            float z = ofClamp(points[i].z, lo, hi);
            float m = MAX(0, masses[i]);

            m_Balls[i].p[0] = points[i].x;
            m_Balls[i].p[1] = points[i].y;
            m_Balls[i].p[2] = points[i].z;
            m_Balls[i].m    = m;

            m_Balls[i].v[0] = 0;
            m_Balls[i].v[1] = 0;
            m_Balls[i].v[2] = 0;
        }
    }
}


//=============================================================================
void CMetaballs::Update(float dt)
{
	for( int i = 0; i < m_nNumBalls; i++ )
	{
		m_Balls[i].p[0] += dt*m_Balls[i].v[0];
		m_Balls[i].p[1] += dt*m_Balls[i].v[1];
		m_Balls[i].p[2] += dt*m_Balls[i].v[2];


		m_Balls[i].t -= dt;
		if( m_Balls[i].t < 0 )
		{
			// When is the next time to act?
			m_Balls[i].t = float(rand())/RAND_MAX;

			// Use a new attraction point
			m_Balls[i].a[0] = (float(rand())/RAND_MAX*2-1)/2;
			m_Balls[i].a[1] = (float(rand())/RAND_MAX*2-1)/2;
			m_Balls[i].a[2] = (float(rand())/RAND_MAX*2-1)/2;
		}

		// Accelerate towards the attraction point
		float x = m_Balls[i].a[0] - m_Balls[i].p[0];
		float y = m_Balls[i].a[1] - m_Balls[i].p[1];
		float z = m_Balls[i].a[2] - m_Balls[i].p[2];
		float fDist = 1/sqrtf(x*x + y*y + z*z);

		x *= fDist;
		y *= fDist;
		z *= fDist;

		m_Balls[i].v[0] += 0.1f*x*dt;
		m_Balls[i].v[1] += 0.1f*y*dt;
		m_Balls[i].v[2] += 0.1f*z*dt;

		fDist = m_Balls[i].v[0]*m_Balls[i].v[0] +
		        m_Balls[i].v[1]*m_Balls[i].v[1] +
		        m_Balls[i].v[2]*m_Balls[i].v[2];
		if( fDist > 0.040f )
		{
			fDist = 1/sqrtf(fDist);
			m_Balls[i].v[0] = 0.20f*m_Balls[i].v[0]*fDist;
			m_Balls[i].v[1] = 0.20f*m_Balls[i].v[1]*fDist;
			m_Balls[i].v[2] = 0.20f*m_Balls[i].v[2]*fDist;
		}

		if( m_Balls[i].p[0] < -1+m_fVoxelSize )
		{
			m_Balls[i].p[0] = -1+m_fVoxelSize;
			m_Balls[i].v[0] = 0;
		}
		if( m_Balls[i].p[0] >  1-m_fVoxelSize )
		{
			m_Balls[i].p[0] =  1-m_fVoxelSize;
			m_Balls[i].v[0] = 0;
		}
		if( m_Balls[i].p[1] < -1+m_fVoxelSize )
		{
			m_Balls[i].p[1] = -1+m_fVoxelSize;
			m_Balls[i].v[1] = 0;
		}
		if( m_Balls[i].p[1] >  1-m_fVoxelSize )
		{
			m_Balls[i].p[1] =  1-m_fVoxelSize;
			m_Balls[i].v[1] = 0;
		}
		if( m_Balls[i].p[2] < -1+m_fVoxelSize )
		{
			m_Balls[i].p[2] = -1+m_fVoxelSize;
			m_Balls[i].v[2] = 0;
		}
		if( m_Balls[i].p[2] >  1-m_fVoxelSize )
		{
			m_Balls[i].p[2] =  1-m_fVoxelSize;
			m_Balls[i].v[2] = 0;
		}
	}
}

//=============================================================================
void CMetaballs::Render()
{
	int nCase,x,y,z;
	bool bComputed;

	m_nNumIndices  = 0;
	m_nNumVertices = 0;

	// Clear status grids
	memset(m_pnGridPointStatus, 0, (m_nGridSizep1)*(m_nGridSizep1)*(m_nGridSizep1));
	memset(m_pnGridVoxelStatus, 0, (m_nGridSize  )*(m_nGridSize  )*(m_nGridSize  ));

	for( int i = 0; i < m_nNumBalls; i++ ){
	    if (m_Balls[i].m > 0){

            x = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[0]);
            y = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[1]);
            z = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[2]);

            // Work our way out from the center of the ball until the surface is
            // reached. If the voxel at the surface is already computed then this
            // ball share surface with a previous ball.
            bComputed = false;
            while(1){
                if( IsGridVoxelComputed(x,y,z) ){
                    bComputed = true;
                    break;
                }

                nCase = ComputeGridVoxel(x,y,z);
                if( nCase < 255 )
                    break;

                z--;
            }

            if( bComputed )
                continue;

            // Compute all voxels on the surface by computing neighbouring voxels
            // if the surface goes into them.
            AddNeighborsToList(nCase,x,y,z);

            while( m_nNumOpenVoxels ){
                m_nNumOpenVoxels--;
                x = m_pOpenVoxels[m_nNumOpenVoxels*3    ];
                y = m_pOpenVoxels[m_nNumOpenVoxels*3 + 1];
                z = m_pOpenVoxels[m_nNumOpenVoxels*3 + 2];

                nCase = ComputeGridVoxel(x,y,z);
                AddNeighborsToList(nCase,x,y,z);
            }
        }
	}


    // Render the computed triangles!
    if (bRenderWithTexture){

        glColor3f(1,1,1);
        texColor.bind();
        texColor.texData.bFlipTexture = false;
        GLfloat txw = texColor.texData.tex_t;
        GLfloat tyh = texColor.texData.tex_u;

        glBegin(GL_TRIANGLES);
        for (int i=0; i<m_nNumIndices; i++){
            int index0 = (int) m_pIndices[i];
            SVertex V = m_pVertices[index0];
            glNormal3fv (V.n);
            glTexCoord2f(V.t[0]*txw, (1-V.t[1])*tyh);
            glVertex3fv (V.v);
        }
        glEnd();
        texColor.unbind();

    } else {
        glBegin(GL_TRIANGLES);
        for (int i=0; i<m_nNumIndices; i++){
            int index0 = (int) m_pIndices[i];
            SVertex V = m_pVertices[index0];
            glNormal3fv(V.n);
            glVertex3fv(V.v);
        }
        glEnd();
    }



}

//=============================================================================
void CMetaballs::AddNeighborsToList(int nCase, int x, int y, int z)
{
	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<0) )
		AddNeighbor(x+1, y, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<1) )
		AddNeighbor(x-1, y, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<2) )
		AddNeighbor(x, y+1, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<3) )
		AddNeighbor(x, y-1, z);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<4) )
		AddNeighbor(x, y, z+1);

	if( CMarchingCubes::m_CubeNeighbors[nCase] & (1<<5) )
		AddNeighbor(x, y, z-1);
}

//=============================================================================
void CMetaballs::AddNeighbor(int x, int y, int z)
{
	if( IsGridVoxelComputed(x,y,z) || IsGridVoxelInList(x,y,z) )
		return;

	// Make sure the array is large enough
	if( m_nMaxOpenVoxels == m_nNumOpenVoxels ){
		m_nMaxOpenVoxels *= 2;
		int *pTmp = new int[m_nMaxOpenVoxels*3];
		memcpy(pTmp, m_pOpenVoxels, m_nNumOpenVoxels*3*sizeof(int));
		delete[] m_pOpenVoxels;
		m_pOpenVoxels = pTmp;
	}

    int n3 = m_nNumOpenVoxels*3;
	m_pOpenVoxels[n3  ] = x;
	m_pOpenVoxels[n3+1] = y;
	m_pOpenVoxels[n3+2] = z;

	SetGridVoxelInList(x,y,z);
	m_nNumOpenVoxels++;
}

//=============================================================================
float CMetaballs::ComputeEnergy(float x, float y, float z)
{
	float fEnergy = 0;
	float fSqDist;
	float *p;
	float dx,dy,dz;

	for( int i = 0; i < m_nNumBalls; i++ ){
		// The formula for the energy is
		//   e += mass/distance^2

        p = m_Balls[i].p;
        dx = p[0] - x;
        dy = p[1] - y;
        dz = p[2] - z;
        fSqDist = dx*dx + dy*dy + dz*dz;
		if( fSqDist < 0.0001f ) fSqDist = 0.0001f;
		fEnergy += m_Balls[i].m / fSqDist;
	}

	return fEnergy;
}

//=============================================================================
void CMetaballs::ComputeNormal(SVertex *pVertex)
{
	float fSqDist;
	float K;

	for( int i = 0; i < m_nNumBalls; i ++ ){
		// To compute the normal we derive the energy formula and get
		//   n += 2 * mass * vector / distance^4
        float *p = m_Balls[i].p;
        float *v = pVertex->v;
		float x = v[0] - p[0];
		float y = v[1] - p[1];
		float z = v[2] - p[2];

		fSqDist = x*x + y*y + z*z;
		K = 2.0 * m_Balls[i].m / (fSqDist * fSqDist);
		pVertex->n[0] += K * x;
		pVertex->n[1] += K * y;
		pVertex->n[2] += K * z;
	}

	float vx = pVertex->n[0];
	float vy = pVertex->n[1];
	float vz = pVertex->n[2];
	float vh = sqrt(vx*vx + vy*vy + vz*vz);
	if (vh > 0){
        pVertex->n[0] /= vh;
        pVertex->n[1] /= vh;
        pVertex->n[2] /= vh;
	}

	// Compute the sphere-map texture coordinate
	// Note: The normal used here should be transformed to camera space first
	// for correct result. In this application no transformation is needed
	// since the camera is fixed.
	pVertex->t[0] =  pVertex->n[0]/2 + 0.5f;
	pVertex->t[1] = -pVertex->n[1]/2 + 0.5f;

}

//=============================================================================
inline float CMetaballs::ComputeGridPointEnergy(int x, int y, int z)
{
	if( IsGridPointComputed(x,y,z) )
		return m_pfGridEnergy[x +
		                      y*(m_nGridSizep1) +
		                      z*(m_nGridSizep1)*(m_nGridSizep1)];

	// The energy on the edges are always zero to make sure the isosurface is
	// always closed.
	if( x <= 1 || y <= 1 || z <= 1 ||
	    x >= m_nGridSize || y >= m_nGridSize || z >= m_nGridSize ){
		m_pfGridEnergy[x +
		               y*(m_nGridSizep1) +
		               z*(m_nGridSizep1)*(m_nGridSizep1)] = 0;
		SetGridPointComputed(x,y,z);
		return 0;
	}

	float fx = ConvertGridPointToWorldCoordinate(x);
	float fy = ConvertGridPointToWorldCoordinate(y);
	float fz = ConvertGridPointToWorldCoordinate(z);

    float out = m_pfGridEnergy[x +
                   y*(m_nGridSizep1) +
	               z*(m_nGridSizep1)*(m_nGridSizep1)] = ComputeEnergy(fx,fy,fz);

	SetGridPointComputed(x,y,z);
	return out;
}

//=============================================================================
int CMetaballs::ComputeGridVoxel(int x, int y, int z)
{
	float b[8];

	b[0] = ComputeGridPointEnergy(x  , y  , z  );
	b[1] = ComputeGridPointEnergy(x+1, y  , z  );
	b[2] = ComputeGridPointEnergy(x+1, y  , z+1);
	b[3] = ComputeGridPointEnergy(x  , y  , z+1);
	b[4] = ComputeGridPointEnergy(x  , y+1, z  );
	b[5] = ComputeGridPointEnergy(x+1, y+1, z  );
	b[6] = ComputeGridPointEnergy(x+1, y+1, z+1);
	b[7] = ComputeGridPointEnergy(x  , y+1, z+1);

	float fx = ConvertGridPointToWorldCoordinate(x) + m_fVoxelSize/2;
	float fy = ConvertGridPointToWorldCoordinate(y) + m_fVoxelSize/2;
	float fz = ConvertGridPointToWorldCoordinate(z) + m_fVoxelSize/2;

	int c = 0;
	c |= b[0] > m_fLevel ?   1 : 0;
	c |= b[1] > m_fLevel ?   2 : 0;
	c |= b[2] > m_fLevel ?   4 : 0;
	c |= b[3] > m_fLevel ?   8 : 0;
	c |= b[4] > m_fLevel ?  16 : 0;
	c |= b[5] > m_fLevel ?  32 : 0;
	c |= b[6] > m_fLevel ?  64 : 0;
	c |= b[7] > m_fLevel ? 128 : 0;

	// Compute vertices from marching pyramid case
	fx = ConvertGridPointToWorldCoordinate(x);
	fy = ConvertGridPointToWorldCoordinate(y);
	fz = ConvertGridPointToWorldCoordinate(z);

	int i = 0;
	float t, omt;
	unsigned short EdgeIndices[12];
	memset(EdgeIndices, 0xFF, 12*sizeof(unsigned short));
	while(1){
		int nEdge =	CMarchingCubes::m_CubeTriangles[c][i];
		if( nEdge == -1 )
			break;

		if( EdgeIndices[nEdge] == 0xFFFF )
		{
            EdgeIndices[nEdge] = m_nNumVertices;
            if (m_nNumVertices < m_nMaxNumVertices){

                // Optimization: It's possible that the non-interior edges
                // have been computed already in neighbouring voxels

                // Compute the vertex by interpolating between the two points
                int nIndex0 = CMarchingCubes::m_CubeEdges[nEdge][0];
                int nIndex1 = CMarchingCubes::m_CubeEdges[nEdge][1];

                float *mcvn0 = CMarchingCubes::m_CubeVertices[nIndex0];
                float *mcvn1 = CMarchingCubes::m_CubeVertices[nIndex1];

                t = (m_fLevel - b[nIndex0])/(b[nIndex1] - b[nIndex0]);
                omt = 1.0f - t;

                m_pVertices[m_nNumVertices].v[0] = mcvn0[0]*omt + mcvn1[0]*t;
                m_pVertices[m_nNumVertices].v[1] = mcvn0[1]*omt + mcvn1[1]*t;
                m_pVertices[m_nNumVertices].v[2] = mcvn0[2]*omt + mcvn1[2]*t;

                m_pVertices[m_nNumVertices].v[0] = fx +
                    m_pVertices[m_nNumVertices].v[0]*m_fVoxelSize;
                m_pVertices[m_nNumVertices].v[1] = fy +
                    m_pVertices[m_nNumVertices].v[1]*m_fVoxelSize;
                m_pVertices[m_nNumVertices].v[2] = fz +
                    m_pVertices[m_nNumVertices].v[2]*m_fVoxelSize;

                // Compute the normal at the vertex
                ComputeNormal(&m_pVertices[m_nNumVertices]);

                m_nNumVertices++;
            }
		}

		// Add the edge's vertex index to the index list
		if (m_nNumIndices < MAX_INDICES){
            m_pIndices[m_nNumIndices] = EdgeIndices[nEdge];
            m_nNumIndices++;
		}
		i++;
	}

	SetGridVoxelComputed (x,y,z);


    // Render the computed triangles!
    if (bRenderWithTexture){

        glColor3f(1,1,1);
        texColor.bind();
        texColor.texData.bFlipTexture = false;
        GLfloat txw = texColor.texData.tex_t;
        GLfloat tyh = texColor.texData.tex_u;

        glBegin(GL_TRIANGLES);
        for (int i=0; i<m_nNumIndices; i++){
            int index0 = (int) m_pIndices[i];
            SVertex V = m_pVertices[index0];
            glNormal3fv(V.n);
            glTexCoord2f(V.t[0]*txw, (1-V.t[1])*tyh);
            glVertex3fv(V.v);
        }
        glEnd();
        texColor.unbind();

    } else {

        glBegin(GL_TRIANGLES);
        for (int i=0; i<m_nNumIndices; i++){
            SVertex V = m_pVertices[m_pIndices[i]];
            glNormal3fv(V.n);
            glVertex3fv(V.v);
        }
        glEnd();
    }



    m_nNumVertices = 0;
    m_nNumIndices = 0;

	return c;
}




//=============================================================================
inline float CMetaballs::ConvertGridPointToWorldCoordinate(int x)
{
	return float(x)*m_fVoxelSize - 1.0f;
}

//=============================================================================
int CMetaballs::ConvertWorldCoordinateToGridPoint(float x)
{
	return int((x + 1.0f)/m_fVoxelSize + 0.5f);
}


//=============================================================================
inline bool CMetaballs::IsGridPointComputed(int x, int y, int z)
{
	if( m_pnGridPointStatus[x +
	                        y*(m_nGridSize+1) +
	                        z*(m_nGridSize+1)*(m_nGridSize+1)] == 1 )
		return true;
	else
		return false;
}

//=============================================================================
inline bool CMetaballs::IsGridVoxelComputed(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 1 )
		return true;
	else
		return false;
}

//=============================================================================
inline bool CMetaballs::IsGridVoxelInList(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 2 )
		return true;
	else
		return false;
}

//=============================================================================
inline void CMetaballs::SetGridPointComputed(int x, int y, int z)
{
	m_pnGridPointStatus[x +
	                    y*(m_nGridSizep1) +
	                    z*(m_nGridSizep1)*(m_nGridSizep1)] = 1;
}

//=============================================================================
inline void CMetaballs::SetGridVoxelComputed(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 1;
}

//=============================================================================
inline void CMetaballs::SetGridVoxelInList(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 2;
}
