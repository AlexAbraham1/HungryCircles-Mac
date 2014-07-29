#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"

#include "Circle.h"

class ofApp : public ofBaseApp{
    
public:
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void cleanupCircles();
    bool isBlankSpace(int x, int y);
    void switchCamera();
    void switchInvert();
    void fixZIndex();
    void touchCircle(int x, int y);
    
    shared_ptr<ofVideoGrabber> grabber;
    
    ofImage image;
    ofxCvColorImage colorCv;
    ofxCvColorImage colorCvSmall;
    ofxCvGrayscaleImage grayCv;
    ofxCvHaarFinder haarFinder;
    float haarScaleFactor;
    
    bool firstRun;
    
    
    
    std::vector<Circle*> circles;
    std::vector<ofxCvBlob> eyes;
    
    std::list<Circle*> added_circles;
    
    int minRadius;
    int maxRadius;

    int DEVICE_ID;
    
    int screenWidth;
    int screenHeight;
    int haarWidth;
    int haarHeight;
    
    ofTrueTypeFont font;
};
