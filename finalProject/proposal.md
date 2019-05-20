### Physiacal components:   
- camera connected to the Raspberry Pi   
- Raspberry Pi 3+ with 32GB micro SD card   
- laptop   

### Software:       
#### Raspberry Pi:      
	- python3 program       
	- uses tensorflow object_detection API and 'ssd_mobilenet_v1_coco_11_06_2017' model, trained on coco dataset to recognize certain objects   
	- if desired objects appear in front of the camera then it sends them to the laptop using UDP   
	- uses OpenCV and tensorflow libraries    
#### Laptop:   
	- node.js server   
	- receives data from Raspberry Pi   
	- displays the game website    
	- checks whether Raspberry Pi sends the right labels, at appropriate time   

Both laptop and Raspberry Pi need to be connected to the internet.   