#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(255, 255, 255);
    ofSetLogLevel (OF_LOG_VERBOSE);
    font.loadFont("verdana.ttf",72);
    
    screenWidth = 640;
    screenHeight = 480;
    
    grabber = shared_ptr<ofVideoGrabber>(new ofVideoGrabber());
    grabber->setPixelFormat(OF_PIXELS_RGB);
    DEVICE_ID = grabber->listDevices().size() - 1;
    grabber->setDeviceID(DEVICE_ID);
    grabber->initGrabber(screenWidth, screenHeight);
    image.allocate(screenWidth, screenHeight, OF_IMAGE_COLOR);
    
    haarScaleFactor = 2;
    
    haarWidth = int(screenWidth / haarScaleFactor);
    haarHeight = int(screenHeight / haarScaleFactor);
    
    haarFinder.setup("right_eye.xml");
    haarFinder.setNeighbors(2);
    haarFinder.setScaleHaar(1.33);
    
    colorCv.allocate(screenWidth, screenHeight);
    colorCvSmall.allocate(haarWidth, haarHeight);
    grayCv.allocate(haarWidth, haarHeight);
    
    firstRun = true;
    invertColors = false;
    
    minRadius = 5;
    maxRadius = 15;
}
//--------------------------------------------------------------
void ofApp::update(){
    
    grabber->update();
    if (grabber->isFrameNew()) {
        
        //Detect Eyes
        image.setFromPixels(grabber->getPixelsRef());
        image.mirror(false, true);
        
        colorCv = image.getPixels();
        colorCvSmall.scaleIntoMe(colorCv, CV_INTER_NN);
        grayCv = colorCvSmall;
        haarFinder.findHaarObjects(grayCv);
        eyes = haarFinder.blobs;
        
        if (firstRun) {
            int x = 0;
            int y = 0;
            
            while (y <= screenHeight) {
                
                int radius = int(minRadius+ static_cast<float>(rand())/ (static_cast<float>(RAND_MAX/ (maxRadius - minRadius))));
                
                if (isBlankSpace(x, y)) {
                    Circle * circle = new Circle();
                    
                    circle->x = x;
                    circle->y = y;
                    
                    circle->radius = radius;
                    
                    circles.push_back(circle);
                }
                
                x += minRadius;
                
                if (x > screenWidth) {
                    x = 0;
                    y += minRadius;
                }
                
            }
            firstRun = false;
            fixZIndex();
            cleanupCircles();
            
        }
        
        
    }

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xFFFFFF);
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        Circle * circle = circles[i];
        ofColor color = image.getPixelsRef().getColor(circle->x, circle->y);
        if (invertColors) {
            color.invert();
        }
        circle->color = color;
        circle->drawCircle();
    }
    
    
    ofPushStyle();
    
    for (std::vector<ofxCvBlob>::size_type i = 0; i != eyes.size(); i++) {
        ofxCvBlob& eye = eyes[i];
        
        int x;
        int y;
        
        x = (eye.boundingRect.x + (eye.boundingRect.width/2)) * haarScaleFactor;
        y = (eye.boundingRect.y + (eye.boundingRect.height/2)) * haarScaleFactor;
            
        
        
        int radius1 = eye.boundingRect.width/3;
        
        ofSetColor(0, 0, 255);
        ofCircle(x, y, 100, radius1);
    }
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    switch (key){
        case OF_KEY_LEFT:
            if (maxRadius >= 15) {
                maxRadius -= 5;
                circles.clear();
                firstRun = true;
            }
            break;
        case OF_KEY_RIGHT:
            maxRadius += 5;
            circles.clear();
            firstRun = true;
            break;
        case OF_KEY_UP:
            invertColors = !invertColors;
            break;
        case 'c':
            maxRadius = 15;
            circles.clear();
            invertColors = false;
            firstRun = true;
            break;
        case ' ':
            saveImage();
            break;
        
    }
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

void ofApp::cleanupCircles() {
    ofLog() << "cleanupCircles() started";
    stringstream ss;
    ss << circles.size();
    ofLog() << "NUMBER OF CIRCLES: " + ss.str();
    std::random_shuffle(circles.begin(), circles.end());
    ofLog() << "cleanupCircles() ended";
    ss.str("");
    ss << circles.size();
    ofLog() << "NUMBER OF CIRCLES: " + ss.str();
}

bool ofApp::isBlankSpace(int x, int y) {
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        
        Circle * circle = circles[i];
        
        float x2 = static_cast<int>(pow(static_cast<double>(circle->x - x), 2)+.005);
        float y2 = static_cast<int>(pow(static_cast<double>(circle->y - y), 2)+.005);
        float radius2 = static_cast<int>(pow(static_cast<double>(circle->radius), 2)+.005);
        
        if ((x2 + y2) < (radius2)/2) {
            return false;
        }
        
    }
    return true;
    
}

void ofApp::fixZIndex() {
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        Circle * circle = circles[i];
        
        float smallRadius = (minRadius + ((minRadius + maxRadius)/2))/2;
        if (circle->radius <= smallRadius) {
            circle->z = 50;
        }
    }
}

void ofApp::touchCircle(int x, int y) {
    int red = int(0+ static_cast<float>(rand())/ (static_cast<float>(RAND_MAX/ (255 - 0))));
    int green = int(0+ static_cast<float>(rand())/ (static_cast<float>(RAND_MAX/ (255 - 0))));
    int blue = int(0+ static_cast<float>(rand())/ (static_cast<float>(RAND_MAX/ (255 - 0))));
    int radius = int(minRadius+ static_cast<float>(rand())/ (static_cast<float>(RAND_MAX/ (maxRadius - minRadius))));
    ofColor color = ofColor(red, green, blue);
    
    Circle * circle = new Circle();
    
    circle->x = x;
    circle->y = y;
    circle->radius = radius;
    circle->z = 200;
    circle->color = color;
    
    added_circles.push_back(circle);
}

void ofApp::saveImage() {
    stringstream ss;
    ss << ofGetElapsedTimef();
    ofSaveScreen(ofFilePath::getUserHomeDir() + "/Desktop/" + ss.str() + ".jpg");
}