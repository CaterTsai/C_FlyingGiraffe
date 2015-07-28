#include "GeometryFlow.h"

GeometryFlow::GeometryFlow():
	_bIsSetup(false),
	_fYPos(0.0),
	_bAuto(false),
	_fTime(ofRandom(0.5, 1))
{}

//--------------------------------------------------------------
void GeometryFlow::SetupGeometryFlow(float fYPos, float fYScale)
{
	_fYPos = fYPos;
	_fYScale = fYScale;
	_bIsSetup = true;

}

//--------------------------------------------------------------
void GeometryFlow::UpateGeometryFlow(float fDelta)
{
	if(!_bIsSetup)
	{
		return;
	}

	auto Iter_ = _2DGeometryList.begin();

	while(Iter_ != _2DGeometryList.end())
	{
		Iter_->_AnimX.update(fDelta);
		if(Iter_->_AnimX.hasFinishedAnimating())
		{
			Iter_ = _2DGeometryList.erase(Iter_);
		}
		else
		{
			Iter_->_fCosDegree += fDelta * Iter_->_fDegreeVel;
			Iter_->_fSinDegree += fDelta * Iter_->_fDegreeVel;
			Iter_->_fRotateDegree += fDelta * Iter_->_fRotateDegreeVel;

			if(Iter_->_fCosDegree > 360.0)
			{
				Iter_->_fCosDegree -= 360.0;
			}

			if(Iter_->_fSinDegree > 360.0)
			{
				Iter_->_fSinDegree -= 360.0;
			}

			if(Iter_->_fRotateDegree > 360.0)
			{
				Iter_->_fRotateDegree -= 360.0;
			}
			Iter_++;
		}
	}

	if(_bAuto)
	{
		_fTime -= fDelta;
		if(_fTime <= 0.0)
		{
			_fTime = ofRandom(0.5, 1);
			this->AddGeometry();
		}
	}
}

//--------------------------------------------------------------
void GeometryFlow::DrawGeometryFlow()
{
	if(!_bIsSetup)
	{
		return;
	}

	ofPushStyle();

	auto Iter_ = _2DGeometryList.begin();

	while(Iter_ != _2DGeometryList.end())
	{
		float fX_ = Iter_->_AnimX.getCurrentValue();
		float fY_ = (Iter_->_fSinPercentage) * sin(Iter_->_fSinDegree * DEG_TO_RAD) +
					(1 - Iter_->_fSinPercentage) * cos(Iter_->_fCosDegree * DEG_TO_RAD);
		
		ofPoint Pos_(fX_, _fYPos + (fY_ * _fYScale));

		ofNoFill();

		
		ofPushMatrix();
		{
			ofTranslate(Pos_);
			ofRotate(Iter_->_fRotateDegree, Iter_->_RotateAxis.x, Iter_->_RotateAxis.y, Iter_->_RotateAxis.z);

			Iter_->_Shape.draw();
		}
		ofPopMatrix();


		++Iter_;
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void GeometryFlow::AddGeometry()
{
	st2D_GEOMETRY_UNIT	new2DGeometryUnit_(1, 4);
	st2D_GEOMETRY_UNIT	new2DGeometryUnit2_(1, 4);
	st2D_GEOMETRY_UNIT	new2DGeometryUnit3_(1, 4);

	new2DGeometryUnit_._AnimX.animateFromTo(-50, ofGetWindowWidth() + 50);
	new2DGeometryUnit2_._AnimX.animateFromTo(-50, ofGetWindowWidth() + 50);
	new2DGeometryUnit3_._AnimX.animateFromTo(-50, ofGetWindowWidth() + 50);

	_2DGeometryList.push_back(new2DGeometryUnit_);
	_2DGeometryList.push_back(new2DGeometryUnit2_);
	_2DGeometryList.push_back(new2DGeometryUnit3_);
}