#ifndef _C_2D_GEOMETRY_FLOW_
#define _C_2D_GEOMETRY_FLOW_

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

typedef enum _e2DGEOMETRY_TYPE
{
	e2D_GEOMETRY_TRIANGLE	=	0,
	e2D_GEOMETRY_QUADRILATERAL,	
	e2D_GEOMETRY_STAR,
	
	e2D_GEOMETRY_MAX,
}e2D_GEOMETRY_TYPE;

typedef struct _st2D_GEOMETRY_UNIT
{
	ofxAnimatableFloat		_AnimX;
	e2D_GEOMETRY_TYPE		_eType;

	ofColor					_Color;

	float					_fSize;

	float					_fSinDegree;
	float					_fCosDegree;
	float					_fDegreeVel;
	float					_fSinPercentage;
	
	float					_fRotateDegree;
	float					_fRotateDegreeVel;
	ofVec3f					_RotateAxis;

	ofPath					_Shape;

	_st2D_GEOMETRY_UNIT(float fMaxDuration, float fMinDuration)
	{
		_AnimX.setCurve(AnimCurve::LINEAR);
		_AnimX.setDuration(ofRandom(fMinDuration, fMaxDuration));
		
		_fSize	= ofRandom(30, 50);

		_fDegreeVel = ofRandom(90, 360);
		_fSinDegree = ofRandom(0, 360);
		_fCosDegree = ofRandom(0, 360);
		_fSinPercentage = ofRandom(0, 1);

		_RotateAxis.set(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		_RotateAxis.normalize();
		_fRotateDegreeVel = ofRandom(90, 360);
		_fRotateDegree = ofRandom(0, 360);

		_eType	= (e2D_GEOMETRY_TYPE)static_cast<int>(ofRandom(0, e2D_GEOMETRY_MAX));

		_Shape.setFilled(false);
		_Shape.setStrokeColor(ofColor(0));
		_Shape.setStrokeWidth(2);
		float fHalfSize_ = _fSize / 2.0;
		if(_eType == e2D_GEOMETRY_TRIANGLE)
		{
			_Shape.moveTo(ofRandom(-fHalfSize_ * 0.2, -fHalfSize_), ofRandom(fHalfSize_ * 0.2, fHalfSize_));
			_Shape.lineTo(ofRandom(-fHalfSize_, fHalfSize_), ofRandom(-fHalfSize_ * 0.2, -fHalfSize_));
			_Shape.lineTo(ofRandom(fHalfSize_ * 0.2, fHalfSize_), ofRandom(fHalfSize_ * 0.2, fHalfSize_));
			_Shape.close();
		}
		else if(_eType == e2D_GEOMETRY_QUADRILATERAL)
		{
			_Shape.moveTo(ofRandom(-fHalfSize_ * 0.2, -fHalfSize_), ofRandom(fHalfSize_ * 0.2, fHalfSize_));
			_Shape.lineTo(ofRandom(-fHalfSize_ * 0.2, -fHalfSize_), ofRandom(-fHalfSize_ * 0.2, -fHalfSize_));
			_Shape.lineTo(ofRandom(fHalfSize_ * 0.2, fHalfSize_), ofRandom(-fHalfSize_ * 0.2, -fHalfSize_));
			_Shape.lineTo(ofRandom(fHalfSize_ * 0.2, fHalfSize_), ofRandom(fHalfSize_ * 0.2, fHalfSize_));
			_Shape.close();
		}
		else
		{
			ofVec2f aStarPos_[5];
			for(int idx_ = 0; idx_ < 5; idx_++)
			{
				float fDegree_ = idx_ * 72 + ofRandom(-18, 18);
				aStarPos_[idx_] = ofVec2f(	sin(fDegree_ * DEG_TO_RAD) * ofRandom(fHalfSize_ * 0.7, fHalfSize_), 
											-cos(fDegree_ * DEG_TO_RAD) * ofRandom(fHalfSize_ * 0.7, fHalfSize_)
											);
			}

			_Shape.moveTo(aStarPos_[0]);
			_Shape.lineTo(aStarPos_[2]);
			_Shape.lineTo(aStarPos_[4]);
			_Shape.lineTo(aStarPos_[1]);
			_Shape.lineTo(aStarPos_[3]);
			_Shape.close();
		}
	}

}st2D_GEOMETRY_UNIT;

class GeometryFlow
{
public:
	GeometryFlow();
	void SetupGeometryFlow(float fYPos, float fYScale = 100);
	void UpateGeometryFlow(float fDelta);
	void DrawGeometryFlow();
	void AddGeometry();

public:
	inline void setAuto(bool value)
	{
		_bAuto = value;
	}

private:
	bool					_bIsSetup;
	float					_fYPos;
	float					_fYScale;
	list<st2D_GEOMETRY_UNIT>	_2DGeometryList;

	bool					_bAuto;
	float					_fTime;
};


#endif // !_C_2D_GEOMETRY_FLOW_
