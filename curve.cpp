#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    
}
    

Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.

    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    //cerr << "\t>>> Returning empty curve." << endl;

    int seg;			//Determine number of segments if 4 or 
				//more control points
    if (P.size() == 4){
        seg = 1;
    }
    else{
        seg = ((P.size()-1)/3);
    }

    Curve R((seg)*(steps)+1);

    Vector3f Binit;		//Initial Binormal (arbitrary only at beginning)
    Vector3f Blast;		//Most recent Binormal

    bool beginning = true;

    for(unsigned i = 0; i < P.size()-3; i+=3 )
    {
	if (beginning)
	{
	    Binit = Vector3f(0.f, 0.f, 1.f);
        }
        else
        {
            Binit = Blast;
        }

        for(unsigned delta = 0; delta <= steps; ++delta)
        {
            float t = float(delta) / steps;

	    //Point Matrix
	    Matrix4f relativePoints(P[i+0][0], P[i+1][0], P[i+2][0], P[i+3][0], 
				    P[i+0][1], P[i+1][1], P[i+2][1], P[i+3][1], 
				    P[i+0][2], P[i+1][2], P[i+2][2], P[i+3][2], 
				    0.f, 0.f, 0.f, 0.f);
	    //V Matrix
	    Matrix4f MatV(1.f, -3.f, 3.f, -1.f, 
			  0.f, 3.f, -6.f, 3.f, 
			  0.f, 0.f, 3.f , -3.f, 
			  0.f, 0.f, 0.f, 1.f);
	    //T Matrix
	    Matrix4f MatT(-3.f, 6.f, -3.f, 0.f, 
			  3.f, -12.f, 9.f, 0.f, 
			  0.f, 6.f, -9.f , 0.f, 
			  0.f, 0.f, 3.f, 0.f);

	    //polynomial (t) Matrix
	    Vector4f timeT(1, t, t*t, t*t*t);   
	    
	    //Calculate V Vector
	    R[(i*steps) + delta].V = Vector3f((relativePoints * MatV * timeT)[0], 
					       (relativePoints * MatV * timeT)[1],
					       (relativePoints * MatV * timeT)[2]);
            //Calculate Tangent
	    R[(i*steps) + delta].T = Vector3f((relativePoints * MatT * timeT)[0], 
					       (relativePoints * MatT * timeT)[1],
					       (relativePoints * MatT * timeT)[2]).normalized();
	    //Calculate Normal
	    R[(i*steps) + delta].N = Vector3f::cross(Binit, 
						     R[(i*steps) + delta].T).normalized();
	    //Calulate Binormal
	    R[(i*steps) + delta].B = Vector3f::cross(R[(i*steps) + delta].T, 
						     R[(i*steps) + delta].N).normalized();
	    //Keep track of current Binormal
	    Binit = R[(i*steps) + delta].B;

            beginning = false;
            Blast = Binit;    
        }
    }
              
    // Right now this will just return this empty curve.
    //return Curve();
    return R;
}

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    //cerr << "\t>>> Returning empty curve." << endl;

    int seg;			//Determine number of segments if 4 or 
				//more control points
    if (P.size() == 4){
        seg = 1;
    }
    else{
        seg = (P.size()-3);
    }

    Curve R((seg)*(steps)+1);

    Vector3f Binit;		//Initial Binormal (arbitrary only at beginning)
    Vector3f Blast;		//Most recent Binormal

    bool beginning = true;

    for(unsigned i = 0; i < P.size()-3; ++i )
    {
	if (beginning){
	    Binit = Vector3f(0.f, 0.f, 1.f);
        }
	else{
	    Binit = Blast;
        }   

        for(unsigned delta = 0; delta <= steps; ++delta)
        {
            float t = float(delta) / steps;

	    //Point Matrix
	    Matrix4f relativePoints(P[i+0][0], P[i+1][0], P[i+2][0], P[i+3][0], 
				    P[i+0][1], P[i+1][1], P[i+2][1], P[i+3][1], 
				    P[i+0][2], P[i+1][2], P[i+2][2], P[i+3][2], 
				    0.f, 0.f, 0.f, 0.f);
	    //V Matrix
	    Matrix4f MatV(1.f/6, -3.f/6, 3.f/6 , -1.f/6, 
			  4.f/6, 0.f   , -6.f/6, 3.f/6, 
			  1.f/6, 3.f/6 , 3.f/6 , -3.f/6, 
			  0.f  , 0.f   , 0.f   , 1.f/6);
	    //T Matrix
	    Matrix4f MatT(-3.f/6, 6.f/6  , -3.f/6, 0.f/6, 
			   0.f/6, -12.f/6, 9.f/6 , 0.f/6,
			   3.f/6, 6.f/6  , -9.f/6, 0.f/6, 
			   0.f  , 0.f    , 0.f   , 3.f/6);

	    //polynomial (t) Matrix
	    Vector4f timeT(1, t, t*t, t*t*t);   

	    //Calculate V Vector
	    R[(i*steps) + delta].V = Vector3f((relativePoints * MatV * timeT)[0], 
					       (relativePoints * MatV * timeT)[1],
					       (relativePoints * MatV * timeT)[2]);
            //Calculate Tangent
	    R[(i*steps) + delta].T = Vector3f((relativePoints * MatT * timeT)[0], 
					       (relativePoints * MatT * timeT)[1],
					       (relativePoints * MatT * timeT)[2]).normalized();
	    //Calculate Normal
	    R[(i*steps) + delta].N = Vector3f::cross(Binit, 
						     R[(i*steps) + delta].T).normalized();
	    //Calulate Binormal
	    R[(i*steps) + delta].B = Vector3f::cross(R[(i*steps) + delta].T, 
						     R[(i*steps) + delta].N).normalized();
	    //Keep track of current Binormal
	    Binit = R[(i*steps) + delta].B;

	    beginning = false;
            Blast = Binit;
        }
    }

    // Return an empty curve right now.
    //return Curve();
    return R;
}

//====================================================================================
//EXTRA CREDIT
//====================================================================================

Curve evalCatMullRom( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalCatMullRom must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    // TODO:
    // Implement CatMull Rom splines that connect lines through the control points

    cerr << "\t>>> evalCatMullRom has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;
    //cerr << "\t>>> Returning empty curve." << endl;

    int seg;			//Determine number of segments if 4 or 
				//more control points
    if (P.size() == 4){
        seg = 1;
    }
    else{
        seg = (P.size()-3);
    }

    Curve R((seg)*(steps)+1);

    Vector3f Binit;		//Initial Binormal (arbitrary only at beginning)
    Vector3f Blast;		//Most recent Binormal

    bool beginning = true;

    for(unsigned i = 0; i < P.size()-3; ++i )
    {
	if (beginning){
	    Binit = Vector3f(0.f, 0.f, 1.f);
        }
	else{
	    Binit = Blast;
        }   

        for(unsigned delta = 0; delta <= steps; ++delta)
        {
            float t = float(delta) / steps;

	    //Point Matrix
	    Matrix4f relativePoints(P[i+0][0], P[i+1][0], P[i+2][0], P[i+3][0], 
				    P[i+0][1], P[i+1][1], P[i+2][1], P[i+3][1], 
				    P[i+0][2], P[i+1][2], P[i+2][2], P[i+3][2], 
				    0.f, 0.f, 0.f, 0.f);
	    //V Matrix
	    Matrix4f MatV(0.f/2, -1.f/2, 2.f/2 , -1.f/2, 
			  2.f/2, 0.f   , -5.f/2, 3.f/2, 
			  0.f/2, 1.f/2 , 4.f/2 , -3.f/2, 
			  0.f  , 0.f   , -1.f/2, 1.f/2);
	    //T Matrix
	    Matrix4f MatT(-1.f/2, 4.f/2  , -3.f/2, 0.f/2, 
			   0.f/2, -10.f/2, 9.f/2 , 0.f/2,
			   1.f/2, 8.f/2  , -9.f/2, 0.f/2, 
			   0.f  , -2.f/2 , 3.f/2 , 0.f/2);

	    //polynomial (t) Matrix
	    Vector4f timeT(1, t, t*t, t*t*t);   

	    //Calculate V Vector
	    R[(i*steps) + delta].V = Vector3f((relativePoints * MatV * timeT)[0], 
					       (relativePoints * MatV * timeT)[1],
					       (relativePoints * MatV * timeT)[2]);
            //Calculate Tangent
	    R[(i*steps) + delta].T = Vector3f((relativePoints * MatT * timeT)[0], 
					       (relativePoints * MatT * timeT)[1],
					       (relativePoints * MatT * timeT)[2]).normalized();
	    //Calculate Normal
	    R[(i*steps) + delta].N = Vector3f::cross(Binit, 
						     R[(i*steps) + delta].T).normalized();
	    //Calulate Binormal
	    R[(i*steps) + delta].B = Vector3f::cross(R[(i*steps) + delta].T, 
						     R[(i*steps) + delta].N).normalized();
	    //Keep track of current Binormal
	    Binit = R[(i*steps) + delta].B;

	    beginning = false;
            Blast = Binit;
        }
    }

    // Return an empty curve right now.
    //return Curve();
    return R;
}

//====================================================================================
//====================================================================================

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

