#include "surf.h"
#include "extra.h"
using namespace std;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    vector<Vector3f> initialVV;
    
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // TODO: Here you should build the surface.  See surf.h for details.

    //cerr << "\t>>> makeSurfRev called (but not implemented).\n\t>>> Returning empty surface." << endl;
 

    for (unsigned i=0; i<profile.size(); i++){

        for (unsigned j=0; j<=steps; j++){
            float t = 2.0f * M_PI * float( j ) / steps;

	    //Matrix operations used to calulate normal
	    Matrix4f rotatM = Matrix4f::rotateY(t);
	    Matrix3f rotatMsub = rotatM.getSubmatrix3x3(0,0);
	    Matrix3f rotatMtrans = rotatMsub.transposed();
	    Matrix3f rotatN = rotatMtrans.inverse();

	    //Calculate surface vertex
	    Vector4f surfaceCalc = Vector4f(profile[i].V[0], profile[i].V[1], profile[i].V[2], 1.f);
	    Vector4f surfaceVecInit = rotatM*surfaceCalc;
	    Vector3f surfaceVec = Vector3f(surfaceVecInit[0], surfaceVecInit[1], surfaceVecInit[2]);

	    //Calculate surface normal
	    Vector3f surfaceVNInit = rotatN*profile[i].N;

	    //Push vectors into surface data
	    surface.VV.push_back(surfaceVec);
	    surface.VN.push_back(-1*surfaceVNInit);

        }
    }

    //Calculate faces once all the vertices are added
    for (unsigned k=0; k<surface.VV.size()-(steps+1);k++){
	Tup3u firstTri;		//faces uses a series of connected triangles
	Tup3u secondTri;

	if ((k+1)%(steps+1) != 0)	//Create triangles (considering edge conditions)
	{
	    //Triangles in counter-clockwise manner
	    firstTri = Tup3u(k+1, k, k+steps+1);
            secondTri = Tup3u(k+1, k+1+steps, k+2+steps);   
        }

	surface.VF.push_back(firstTri);
	surface.VF.push_back(secondTri);  
    }

    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    // TODO: Here you should build the surface.  See surf.h for details.

    //cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." <<endl;
    
    for (unsigned i=0; i<profile.size(); i++){

        for (unsigned j=0; j<sweep.size(); j++){

	    //Matrix from sweep
            Matrix4f coordM(sweep[j].N[0], sweep[j].B[0], sweep[j].T[0], sweep[j].V[0], 
			    sweep[j].N[1], sweep[j].B[1], sweep[j].T[1], sweep[j].V[1], 
			    sweep[j].N[2], sweep[j].B[2], sweep[j].T[2], sweep[j].V[2], 
			    0.f, 0.f, 0.f, 1.f);

	    //Matrix operations to get normal
	    Matrix3f rotatMsub = coordM.getSubmatrix3x3(0,0);
	    Matrix3f rotatMtrans = rotatMsub.transposed();
	    Matrix3f rotatN = rotatMtrans.inverse();

	    //Calculate surface vertex
	    Vector4f surfaceCalc = Vector4f(profile[i].V[0], profile[i].V[1], profile[i].V[2], 1.f);
	    Vector4f surfaceVecInit = coordM*surfaceCalc;
	    Vector3f surfaceVec = Vector3f(surfaceVecInit[0], surfaceVecInit[1], surfaceVecInit[2]);

	    //Calculate surface normal
	    Vector3f surfaceVNInit = rotatN*profile[i].N;

	    //Push vectors into surface data
	    surface.VV.push_back(surfaceVec);
	    surface.VN.push_back(-1*surfaceVNInit);
        }
    }

    //Calculate faces once all the vertices are added
    for (unsigned k=0; k<surface.VV.size()-(sweep.size());k++){

	Tup3u firstTri;		//faces uses a series of connected triangles
	Tup3u secondTri;

	if ((k+1)%(sweep.size()) != 0)	//Create triangles (considering edge conditions)
	{
	    //Triangles in counter-clockwise manner
	    firstTri = Tup3u(k+1, k, k+sweep.size());
            secondTri = Tup3u(k+1, k+sweep.size(), k+1+sweep.size());   
        }

	surface.VF.push_back(firstTri);
	surface.VF.push_back(secondTri);
            
    }

    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
