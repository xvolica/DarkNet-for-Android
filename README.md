# DarkNet-for-Android
Android version of Darknet Yolo v3 & v2 Neural Networks for object detection http://pjreddie.com/darknet/

# how to use
Open this project by AndroidStudio 3.0, build and run. The default model is yolov3-tiny.

# using other model
If you want to use yolov3 or others, please download weights and cfg files and put them into \app\src\main\assets

Then change the path code in DarknetDao.java

e.g.  You can download yolov3.weights from http://pjreddie.com/darknet/

# something bad
It's very slow to compute an image on phone by darknet. Predict an 640x480 image with yolov3 cast about 7 minutes in my test. 

I speculate because darknet is pure c language, donot use opengl or ndk for acculerate. But maybe there is something wrong in my code.

