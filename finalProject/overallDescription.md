# Concept

-MEMORY GAME
-user remembers a random sequence of objects appearing on the screen
-user recreates the sequence by placing physical pictures of those objects in front of the camera, in the same order as previously seen on the screen
-sequence length starts at 1 and increments by 1 with every additional round
-placing wrong picture in front of the camera ends the game
-user can restart the game and try again


# Description

## Raspberry Pi
### Preparation:
-installed Raspian Stretch   
-followed [these instructions](https://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/) to install OpenCV for python3 (from step #2)   
-changed the appropriate parts to install OpenCV 4.1.0 instead of 3.1.0   
-installed tensorflow using the precompiled binaries from this repository (pip install didn't work) from [here](https://github.com/lhelontra/tensorflow-on-arm/releases)   
-installed remaining dependencies and cloned tensorflow models from [here](https://pythonprogramming.net/introduction-use-tensorflow-object-detection-api-tutorial/)    
### Code:
- modified the [following code](https://pythonprogramming.net/video-tensorflow-object-detection-api-tutorial/?completed=/introduction-use-tensorflow-object-detection-api-tutorial/), which in turn is a modified version of tensorflow 'object_detection-tutorial' file     
-removed or commented out redundant parts    
-the program downloads the 'ssd_mobilenet_v1_coco_11_06_2017' model' every time it runs so I have commented out the appropriate parts in my code after the first run    
-removed part which displayed the webcam stream    
-made the camera turn on/off every 1s which descreased each detection run from ~6s to ~1s    
-changed the code so that if one of the 5 labels I am using (bear, zebra, horse, airplane, donut) is detected then it is sent using UDP to my laptop    

## Laptop    
### Preparation:    
-installed node, express and socket.io    
-very slightly modified the node.js server code from class    
-build the game website    
### Code:    
-game generates a random sequence of objects and the next object is always different from the current one    
-every object is first shown at the center of the screen and its small icon is appended at the bottom until all the object are displayed    
-in the guessing phase user sees number of remaining objects 'to guess'    
-game checks if the user puts the right picture in front of Pi's camera -> if not then the game is over    
-the next object is always different from the current one so the game ignores the current object until other object is put in front of the camera    
-for this reason and also because Pi's recognition is quite slow the game has no time limit and ends after the user makes a mistake    

