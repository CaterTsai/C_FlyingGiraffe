#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxCTShake.h"

#include "GeometryFlow.h"

static const pair<int, int> cLIMIT_OF_SIZE = make_pair(70, 200);
static const pair<float, float> cLIMIT_OF_FALLING_TIME = make_pair(1, 2);
static const pair<float, float> cLIMIT_OF_SWING_TIME = make_pair(0.2, 0.5);
static const pair<int, int> cLIMIT_OF_SWING_DEGREE = make_pair(10, 20);
static const pair<int, int> cLIMIT_OF_FLYING_TIME = make_pair(1.5, 2.5);
static const pair<int, int> cLIMIT_OF_HEIGHT_TIME = make_pair(1, 3);
static const pair<int, int> cLIMIT_OF_FLYING_DEGREE = make_pair(15, 30);

typedef struct _stGIRAFFE
{
	ofPath		_Outline;
	ofPoint		_TongueStart, _TongueEnd;
	float		_fScale;
	ofPoint		_DrawStartPos;
	
	ofPoint		_SpotsStart1, _SpotsEnd1;
	ofPoint		_SpotsStart2, _SpotsEnd2;

	vector<ofPoint>		_CtrlPoint;

	void setup(float fScale = 200)
	{
		_fScale = fScale;
		
		_Outline.clear();
		_Outline.setFilled(false);
		_Outline.setStrokeWidth(2.0);
		_Outline.setStrokeColor(ofColor(0));

		_DrawStartPos.set(-fScale * 0.1, (fScale * 0.3175));
		ofPoint StartPos_ = _DrawStartPos;

		_Outline.moveTo(StartPos_);
				
		//Mouse
		_Outline.bezierTo(	StartPos_.x - (0.4 * fScale), StartPos_.y, 
							StartPos_.x - (0.4 * fScale), StartPos_.y - (0.1 * fScale),
							StartPos_.x, StartPos_.y - (0.2 * fScale));


		//Tongue
		_TongueStart = ofBezierPoint(	StartPos_,
										ofPoint(StartPos_.x - (0.4 * fScale), StartPos_.y),
										ofPoint(StartPos_.x - (0.4 * fScale), StartPos_.y - (0.1 * fScale)),
										ofPoint(StartPos_.x, StartPos_.y - (0.2 * fScale)),
										0.2
										);

		_TongueEnd = ofBezierPoint(	StartPos_,
									ofPoint(StartPos_.x - (0.4 * fScale), StartPos_.y),
									ofPoint(StartPos_.x - (0.4 * fScale), StartPos_.y - (0.1 * fScale)),
									ofPoint(StartPos_.x, StartPos_.y - (0.2 * fScale)),
									0.3
									);
		

		//Horn
		StartPos_.set(StartPos_.x, StartPos_.y - (0.2 * fScale));
		_Outline.bezierTo( StartPos_.x + (0.05 * fScale), StartPos_.y - (0.15 * fScale),
						StartPos_.x + (0.125 * fScale), StartPos_.y - (0.175 * fScale),
						StartPos_.x + (0.125 * fScale), StartPos_.y);

		//Ear
		StartPos_.set(StartPos_.x + (0.125 * fScale), StartPos_.y);
		_Outline.bezierTo( StartPos_.x + (0.15 * fScale), StartPos_.y,
						StartPos_.x + (0.15 * fScale), StartPos_.y + (0.15 * fScale),
						StartPos_.x, StartPos_.y + (0.15 * fScale));

		//Neck
		_Outline.lineTo( StartPos_.x + (0.175 * fScale), StartPos_.y + fScale);
		_Outline.lineTo( StartPos_.x - (0.125 * fScale), StartPos_.y + fScale);
		_Outline.close();

		//Spot
		ofVec2f LinePos_;
		LinePos_.set(StartPos_.x, StartPos_.y + (0.15 * fScale));
		_SpotsStart1 = LinePos_.getInterpolated(ofVec2f(StartPos_.x + (0.175 * fScale), StartPos_.y + fScale), 0.2);
		_SpotsEnd1 = LinePos_.getInterpolated(ofVec2f(StartPos_.x + (0.175 * fScale), StartPos_.y + fScale), 0.5);
		
		LinePos_ = _DrawStartPos;
		_SpotsStart2 = LinePos_.getInterpolated(ofVec2f(StartPos_.x - (0.125 * fScale), StartPos_.y + fScale), 0.6);
		_SpotsEnd2 = LinePos_.getInterpolated(ofVec2f(StartPos_.x - (0.125 * fScale), StartPos_.y + fScale), 0.9);
	}

	void draw()
	{
		ofPushStyle();
		ofSetColor(0);
		ofSetLineWidth(2);

		_Outline.draw();

		//Eyes
		ofCircle(_DrawStartPos.x, _DrawStartPos.y - (0.1 * _fScale), (0.025 * _fScale));

		//Spots
		ofBezier(	_SpotsStart1.x, _SpotsStart1.y,
					_SpotsStart1.x - (0.1 * _fScale), _SpotsStart1.y,
					_SpotsEnd1.x - (0.1 * _fScale), _SpotsEnd1.y,
					_SpotsEnd1.x, _SpotsEnd1.y
					);

		ofBezier(	_SpotsStart2.x, _SpotsStart2.y,
					_SpotsStart2.x + (0.1 * _fScale), _SpotsStart2.y,
					_SpotsEnd2.x + (0.1 * _fScale), _SpotsEnd2.y,
					_SpotsEnd2.x, _SpotsEnd2.y
					);

		ofNoFill();
		//Tongue
		ofBezier(	_TongueStart.x, _TongueStart.y,
					_TongueStart.x - (0.075 * _fScale), _TongueStart.y + (0.05 * _fScale),
					_TongueEnd.x - (0.075 * _fScale), _TongueEnd.y + (0.05 * _fScale),
					_TongueEnd.x, _TongueEnd.y
					);
		
		ofPopStyle();
	}

}stGIRAFFE;

class ctUmbrella
{
public:
	void setup(float fSize = 200)
	{
		_fsize = fSize;

		_Head.clear();
		_Handle.clear();

		//---------------------------
		//Set the body
		float fSize_ = 20;
		float fWidth_ = _fPosW = (fSize * 1.2)/fSize_;
		float fStart_ = -fSize * 0.6;

		_BodyP1.set(fStart_, 0);
		_BodyP2.set(-fStart_, 0);
		_BodyP3.set(0, -fSize * 0.5);
		for(int idx_ = 1; idx_ <= fSize_; idx_++)
		{
			float fXValue_ = fStart_ + fWidth_ * idx_;
			float fYValue_ = (idx_ % 2 == 0)?0:fSize * 0.05;

			_BodyPosList.push_back(ofVec2f(fXValue_, fYValue_));
		}
		
		_fPosH = fSize * 0.05;

		//---------------------------
		//Set the head
		_Head.setFillColor(ofColor(0));
		_Head.setStrokeColor(ofColor(0));
		_Head.setStrokeWidth(2);

		_Head.moveTo(0, -fSize * 0.39);
		_Head.lineTo(-fSize * 0.2, -fSize * 0.4);
		_Head.lineTo(-fSize * 0.1, -fSize * 0.44);
		_Head.lineTo(-fSize * 0.12, -fSize * 0.54);
		_Head.lineTo(fSize * 0.12, -fSize * 0.54);
		_Head.lineTo(fSize * 0.1, -fSize * 0.44);
		_Head.lineTo(fSize * 0.2, -fSize * 0.4);
		_Head.close();

		//---------------------------
		//Handle
		_Handle.setFilled(false);
		_Handle.setStrokeColor(ofColor(0));
		_Handle.setStrokeWidth(2);

		_Handle.moveTo(-fSize * 0.01, 0);
		_Handle.lineTo(-fSize * 0.01, fSize * 0.45);
		_Handle.bezierTo(	-fSize * 0.02, fSize * 0.45,
							-fSize * 0.04, fSize * 0.47,
							-fSize * 0.04, fSize * 0.5	);

		_Handle.lineTo(fSize * 0.04, fSize * 0.5);
		_Handle.bezierTo(	fSize * 0.04, fSize * 0.47,
							fSize * 0.02, fSize * 0.45,
							fSize * 0.01, fSize * 0.45	);
		_Handle.lineTo(fSize * 0.01, fSize * 0.45);
		_Handle.lineTo(fSize * 0.01, 0);
		_Handle.close();

		//---------------------------
		//Rotate
		_AnimPos.setDuration(0.5);
		_AnimPos.setCurve(AnimCurve::LINEAR);
		_AnimPos.setRepeatType(AnimRepeat::PLAY_ONCE);
		_AnimPos.animateFromTo(0.0, 1.0);
		_bUp2Down = true;

		_AnimTonuge.setDuration(1.0);
		_AnimTonuge.setCurve(AnimCurve::EASE_IN_EASE_OUT);
		_AnimTonuge.setRepeatType(AnimRepeat::LOOP_BACK_AND_FORTH);
		_AnimTonuge.animateFromTo(0.0, 1.0);
		
	}

	//---------------------------
	void update(float fDelta)
	{
		_AnimPos.update(fDelta);
		_AnimTonuge.update(fDelta);

		float fAnimValue_ = _AnimPos.getCurrentValue();

		_BodyP1.y = _BodyP2.y = fAnimValue_ * _fPosH;
		
		if(_bUp2Down)
		{
			_fShiftW = fAnimValue_ * _fPosW;
		}
		else
		{
			_fShiftW = (1 - fAnimValue_)  * _fPosW;
		}
		
		if(_AnimPos.hasFinishedAnimating() && _AnimPos.getPercentDone() == 1.0)
		{
			if(_bUp2Down)
			{
				_AnimPos.animateFromTo(1.0, 0.0);
			}
			else
			{
				_AnimPos.animateFromTo(0.0, 1.0);
			}

			for(auto& Iter_ : _BodyPosList)
			{
				Iter_.x -= _fPosW;
			}

			auto Tmp_ = (*_BodyPosList.begin());
			_BodyPosList.pop_front();
			_BodyPosList.push_back(ofVec2f(_BodyP2.x, Tmp_.y));

			_bUp2Down = !_bUp2Down;
			_fShiftW = 0;
		}
	}

	//---------------------------
	void draw(ofVec2f DrawPos)
	{
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(DrawPos);
		{
			//Draw Handle
			_Handle.draw();

			//Draw Body

			ofPath Body_;
			Body_.setFilled(false);
			Body_.setStrokeColor(ofColor(0));
			Body_.setStrokeWidth(2);

			Body_.moveTo(_BodyP1);
			for(auto Iter_ : _BodyPosList)
			{
				Body_.lineTo(Iter_.x - _fShiftW, Iter_.y);
			}
			Body_.lineTo(_BodyP2);
			Body_.lineTo(_BodyP3);
			Body_.close();
						
			Body_.draw();

			ofSetColor(0);
			ofSetLineWidth(1);
			for(auto Iter_ : _BodyPosList)
			{
				ofLine(_BodyP3, ofVec2f(Iter_.x - _fShiftW, Iter_.y));
			}

			//Draw Head
			_Head.draw();

			//Draw Eye
			ofFill();
			ofSetColor(255);
			ofEllipse(0, -_fsize * 0.2, _fsize * 0.18, _fsize * 0.21);
			
			ofSetColor(0);
			ofEllipse(0, -_fsize * 0.2, _fsize * 0.13, _fsize * 0.16);
			
			ofNoFill();
			ofSetLineWidth(2);
			ofEllipse(0, -_fsize * 0.2, _fsize * 0.18, _fsize * 0.21);

			//Draw Tonuge
			ofFill();
			ofSetColor(255);
			ofSetLineWidth(2);

			float fTonugeShift_ = (_AnimTonuge.getCurrentValue() - 0.5) * _fsize * 0.05;
			ofFill();
			ofSetColor(255);
			ofBezier(	_fsize * 0.05, -_fsize * 0.05,
						_fsize * 0.15 + fTonugeShift_, _fsize * 0.12,
						-_fsize * 0.15 + fTonugeShift_, _fsize * 0.12,
						-_fsize * 0.05, -_fsize * 0.05);

			ofNoFill();
			ofSetColor(0);
			ofSetLineWidth(2);
			ofBezier(	_fsize * 0.05, -_fsize * 0.05,
						_fsize * 0.15 + fTonugeShift_, _fsize * 0.12,
						-_fsize * 0.15 + fTonugeShift_, _fsize * 0.12,
						-_fsize * 0.05, -_fsize * 0.05);

			ofLine(0, _fsize * -0.05, fTonugeShift_, _fsize * 0.05);
			ofLine(-_fsize * 0.08,  _fsize * -0.05, _fsize * 0.08,  _fsize * -0.05);

		}
		ofPopMatrix();
		ofPopStyle();
	}

private:
	float _fsize;
	
	ofPath	_Head, _Handle, _Tonuge;

	bool _bUp2Down;
	ofVec2f _BodyP1, _BodyP2, _BodyP3;
	float _fPosH, _fPosW, _fShiftW;
	deque<ofVec2f> _BodyPosList;

	
	ofxAnimatableFloat	_AnimPos;
	ofxAnimatableFloat	_AnimTonuge;
};

enum eFlyingState
{
	eState_Falling,
	eState_Flying
};

class Flying
{
public:
	Flying()
		:_fSize(ofRandom(cLIMIT_OF_SIZE.first, cLIMIT_OF_SIZE.second))
		,_bAlive(false)
		,_State(eState_Falling)
		,_fFlyDegree(0)
		,_FlyingBaseX(ofRandom(0, 2 * PI))
		,_FlyingBaseY(ofRandom(0, 2 * PI))
	{}
	void setup();
	void update(float fDelta);
	void draw();

	void play();

	inline bool getAlive()
	{
		return _bAlive;
	}
private:
	ctUmbrella		_Umbrella;
	stGIRAFFE		_Giraffe;
	eFlyingState	_State;
	float			_fFlyDegree;

	float			_fSize;
	bool			_bAlive;


	ofxAnimatableFloat	_GiraffeSwing;
	
	ofxAnimatableFloat	_Falling;
	ofxAnimatableFloat	_FlyingPosX;
	float				_FlyingPosY, _FlyingBaseX, _FlyingBaseY;
	ofxAnimatableFloat	_FlyingSinDegree, _FlyingCosDegree;
	
};

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);

private:
	void addFlying();

private:
	float	_MainTimer;

	bool			_bAutoAdd;
	float			_fAutoAddTime;
	list<Flying>	_FlyingList;

	GeometryFlow	_GFlow;

	ofVideoGrabber		_Webcam;
	ofxCTShakeCanvas	_ShakeCanvas;
};
