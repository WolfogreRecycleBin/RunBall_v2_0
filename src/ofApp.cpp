#include "ofApp.h"
#define num 20
struct runBall
{
    float x=0,y=0;
    float vx=0,vy=0;
    float r=0;
    ofColor color;
};
runBall ball[num];
void drawBall(runBall ball);
void upBallDate(runBall *ball);
void computeSpeed(float x1,float y1,float x2,float y2,float *vx1,float *vy1,float *vx2,float *vy2);
int ballNum=0;
int stiction[num][num];
float sumvx=0,sumvy=0;
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255,255,255);
	ballNum=20;
    for(int i=0;i<ballNum;i++)
    {
        ball[i].x=ofRandom(0.0,1024.0);
        ball[i].y=ofRandom(0.0,768.0);
        ball[i].vx=ofRandom(-10.0,10.0);
        ball[i].vy=ofRandom(-10.0,10.0);
        ball[i].r=30;
        ball[i].color.set(
        	round(ofRandom(0, 255)),
        	round(ofRandom(0, 255)),
        	round(ofRandom(0, 255))
        				 );
    }
    for(int i=0;i<ballNum;i++)
        for(int j=i+1;j<ballNum;j++)
        {
            stiction[i][j]=0;
        }

}

//--------------------------------------------------------------
void ofApp::update(){
	for(int i=0;i<ballNum;i++)
    {
        ball[i].x+=ball[i].vx;
        ball[i].y+=ball[i].vy;
        if(ball[i].x>=1024-ball[i].r)
        {
            ball[i].x=1024-ball[i].r;
            ball[i].vx=- ball[i].vx;
        }
        if(ball[i].x<=0+ball[i].r)
        {
            ball[i].x=0+ball[i].r;
            ball[i].vx=- ball[i].vx;
        }
        if(ball[i].y>=768-ball[i].r)
        {
            ball[i].y=768-ball[i].r;
            ball[i].vy=-ball[i].vy;
        }
        if(ball[i].y<=0+ball[i].r)
        {
            ball[i].y=0+ball[i].r;
            ball[i].vy=-ball[i].vy;
        }
    }

    for(int i=0;i<ballNum;i++)
        for(int j=i+1;j<ballNum;j++)
    {
        /*if (((ball[i].x-ball[j].x)*(ball[i].x-ball[j].x)+(ball[i].y-ball[j].y)*(ball[i].y-ball[j].y))<=4*ball[i].r*ball[i].r)
            computeSpeed(ball[i].x,ball[i].y,ball[j].x,ball[j].y,&ball[i].vx,&ball[i].vy,&ball[j].vx,&ball[j].vy);*/
        if (((ball[i].x-ball[j].x)*(ball[i].x-ball[j].x)+(ball[i].y-ball[j].y)*(ball[i].y-ball[j].y))<=4*ball[i].r*ball[i].r)
        {
            if(stiction[i][j]==0)
            {
                computeSpeed(ball[i].x,ball[i].y,ball[j].x,ball[j].y,&ball[i].vx,&ball[i].vy,&ball[j].vx,&ball[j].vy);
                stiction[i][j]=1;
            }

        }
        else
        {
            if(stiction[i][j]==1) stiction[i][j]=0;
        }
    }
    sumvx=0;
    for(int i=0;i<ballNum;i++) sumvx+=abs(ball[i].vx);
    sumvy=0;
    for(int i=0;i<ballNum;i++) sumvy+=abs(ball[i].vy);
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0,255,0);
    ofDrawBitmapString("FPS:"+ofToString(ofGetFrameRate()), 10, 15);
    ofDrawBitmapString("sumvx:"+ofToString(sumvx), 10, 25);
    ofDrawBitmapString("sumvy:"+ofToString(sumvy), 10, 35);
    for(int i=0;i<ballNum;i++) drawBall(ball[i]);
}

//--------------------------------------------------------------
void drawBall(runBall ball)
{
    int i;
    for(i=0;i<=5;i++)
    {
        ofSetColor(ball.color.r-(30-6*i),ball.color.g-(30-6*i),ball.color.b-(30-6*i));
        ofCircle(ball.x,ball.y,ball.r-i);
    }

    ofSetColor(ball.color);
    ofCircle(ball.x,ball.y,ball.r-i);


    {
        float ox,oy,r=ball.r/2;
        ofColor oColor=ball.color;
        oy=ball.y-(ball.y/(sqrt(ball.x*ball.x+ball.y*ball.y)))*r;
        ox=ball.x-(ball.x/(sqrt(ball.x*ball.x+ball.y*ball.y)))*r;
        while(r>=0)
        {
            ofSetColor(oColor.r,oColor.g,oColor.b);
            ofCircle(ox,oy,r--);
            if(oColor.r<=250) oColor.r+=5;
            if(oColor.g<=250) oColor.g+=5;
            if(oColor.b<=250) oColor.b+=5;

        }
    }


}

//--------------------------------------------------------------
void computeSpeed(float x1,float y1,float x2,float y2,float *vx1,float *vy1,float *vx2,float *vy2)
{
	struct vector
	{
		float x;
		float y;
	};
	vector r,e,a,b,aa,bb;
	r.x=x1-x2;
	r.y=y1-y2;
	e.x=r.x/sqrt(r.x*r.x+r.y*r.y);
	e.y=r.y/sqrt(r.x*r.x+r.y*r.y);
	a.x=*vx1;
	a.y=*vy1;
	b.x=*vx2;
	b.y=*vy2;
	aa.x=e.x*(a.x*e.x+a.y*e.y);
	aa.y=e.y*(a.x*e.x+a.y*e.y);
	bb.x=e.x*(b.x*e.x+b.y*e.y);
	bb.y=e.y*(b.x*e.x+b.y*e.y);
	a.x=a.x-aa.x+bb.x;
	a.y=a.y-aa.y+bb.y;
	b.x=b.x-bb.x+aa.x;
	b.y=b.y-bb.y+aa.y;
	*vx1=a.x;
	*vy1=a.y;
	*vx2=b.x;
	*vy2=b.y;
	return;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
