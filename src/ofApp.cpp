#include "ofApp.h"

#pragma region Flying

//--------------------------------------------------------------
void Flying::setup()
{
	_Umbrella.setup(_fSize);
	_Giraffe.setup(_fSize * 0.6);

	_Falling.setCurve(AnimCurve::LINEAR);
	_Falling.setDuration(ofRandom(cLIMIT_OF_FALLING_TIME.first, cLIMIT_OF_FALLING_TIME.second));
	
	_GiraffeSwing.setCurve(AnimCurve::EASE_IN_EASE_OUT);
	_GiraffeSwing.setDuration(ofRandom(cLIMIT_OF_SWING_TIME.first, cLIMIT_OF_SWING_TIME.second));
	_GiraffeSwing.setRepeatType(AnimRepeat::LOOP_BACK_AND_FORTH);
	
	_FlyingPosX.setCurve(AnimCurve::EASE_OUT);
	_FlyingPosX.setDuration(ofRandom(cLIMIT_OF_FLYING_TIME.first, cLIMIT_OF_FLYING_TIME.second));
	
	_FlyingSinDegree.setCurve(AnimCurve::LINEAR);
	_FlyingSinDegree.setRepeatType(AnimRepeat::LOOP);
	_FlyingSinDegree.setDuration(ofRandom(cLIMIT_OF_HEIGHT_TIME.first, cLIMIT_OF_HEIGHT_TIME.second));

	_FlyingCosDegree.setCurve(AnimCurve::LINEAR);
	_FlyingCosDegree.setRepeatType(AnimRepeat::LOOP);
	_FlyingCosDegree.setDuration(ofRandom(cLIMIT_OF_HEIGHT_TIME.first, cLIMIT_OF_HEIGHT_TIME.second));
}

//--------------------------------------------------------------
void Flying::update(float fDelta)
{
	if(!_bAlive)
	{
		return;
	}

	_Umbrella.update(fDelta);
	_Falling.update(fDelta);
	_GiraffeSwing.update(fDelta);
	_FlyingPosX.update(fDelta);
	_FlyingSinDegree.update(fDelta);
	_FlyingCosDegree.update(fDelta);

	switch(_State)
	{
	case eState_Falling:
		{
			_FlyingPosY = _Falling.getCurrentValue();

			if(_Falling.getPercentDone() == 1.0 && _Falling.hasFinishedAnimating())
			{
				float fDegree_ = ofRandom(cLIMIT_OF_SWING_DEGREE.first, cLIMIT_OF_SWING_DEGREE.second);
				_GiraffeSwing.animateFromTo(-fDegree_, fDegree_);
				_FlyingSinDegree.animateFromTo(0, 2 * PI);
				_FlyingCosDegree.animateFromTo(0, 2 * PI);
				_FlyingPosX.animateTo(ofGetWidth() + _fSize);

				_fFlyDegree = ofRandom(cLIMIT_OF_FLYING_DEGREE.first, cLIMIT_OF_FLYING_DEGREE.second);

				_State = eState_Flying;
			}
		}
		break;
	case eState_Flying:
		{
			_FlyingPosY = ( 0.6 * sin(_FlyingSinDegree.getCurrentValue()) + 
							0.4 * cos( _FlyingCosDegree.getCurrentValue())
						  )	* 100;
			_FlyingPosY += ofGetHeight()/2;
		}
		break;
	}

	if(_FlyingPosX.getPercentDone() == 1.0 && _FlyingPosX.hasFinishedAnimating())
	{
		_bAlive = false;
	}
}

//--------------------------------------------------------------
void Flying::draw()
{
	if(!_bAlive)
	{
		return;
	}


	ofPushStyle();
	ofPushMatrix();
	ofTranslate(_FlyingPosX.getCurrentValue(), _FlyingPosY);
	ofRotateZ(_fFlyDegree);
	{
		_Umbrella.draw(ofVec2f(0, 0));

		ofPushMatrix();
		ofTranslate(0, _fSize * 0.5);
		ofRotateZ(_GiraffeSwing.getCurrentValue());
		{
			_Giraffe.draw();
		}
		ofPopMatrix();
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void Flying::play()
{
	_Falling.animateFromTo( -_fSize, ofGetHeight()/2);
	_FlyingPosX.reset(ofRandom(ofGetWidth() * 0.3, ofGetWidth() * 0.5));
	_FlyingPosY = -_fSize;
	_bAlive = true;
}

#pragma endregion

#pragma region ofApp
//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(255);
	ofSetVerticalSync(true);

	//flying
	_fAutoAddTime = ofRandom(1, 3);
	_bAutoAdd = false;

	//Geometry
	_GFlow.SetupGeometryFlow(ofGetHeight()/2);

	//Webcam
	_Webcam.initGrabber(ofGetWidth(), ofGetHeight());

	_MainTimer = ofGetElapsedTimef();

	_ShakeCanvas.setup();
	_ShakeCanvas.startShake();
}

//--------------------------------------------------------------
void ofApp::update()
{
	float fDelta_ = ofGetElapsedTimef() - _MainTimer;
	_MainTimer += fDelta_;

	auto Iter_ = _FlyingList.begin();
	while(Iter_ != _FlyingList.end())
	{
		Iter_->update(fDelta_);

		if(Iter_->getAlive())
		{
			++Iter_;
		}
		else
		{
			//remove die giraffe
			Iter_ = _FlyingList.erase(Iter_);
		}
	}

	_GFlow.UpateGeometryFlow(fDelta_);

	//Auto add flying
	if(_bAutoAdd)
	{
		_fAutoAddTime -= fDelta_;
		if(_fAutoAddTime < 0.0)
		{
			_fAutoAddTime = ofRandom(1, 3);
			this->addFlying();
		}
	}

	_Webcam.update();

	_ShakeCanvas.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofSetColor(255);
	_Webcam.draw(0, 0);

	_ShakeCanvas.begin();
	for(auto& Iter_ : _FlyingList)
	{
		Iter_.draw();
	}

	_GFlow.DrawGeometryFlow();
	_ShakeCanvas.end();

	_ShakeCanvas.draw();		
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch(key)
	{
	case 's':
		{	
			_bAutoAdd = true;
		}
		break;
	case 'w':
		{
			_GFlow.setAuto(true);
		}
		break;
	case 'c':
		{
			_Webcam.videoSettings();
		}
		break;
	}	
}

//--------------------------------------------------------------
void ofApp::addFlying()
{
	Flying newFlying_;
	newFlying_.setup();
	newFlying_.play();

	_FlyingList.push_back(newFlying_);
}

#pragma endregion


