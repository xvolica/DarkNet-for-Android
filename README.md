# DarkNet-for-Android
Android version of Darknet Yolo v3 & v2 Neural Networks for object detection http://pjreddie.com/darknet/

# How to use
Open this project by AndroidStudio 3.0, build and run. The default model is yolov3-tiny.

# How to use other model
If you want to use yolov3 or others, please download weights and cfg files and put them into \app\src\main\assets

Then change the PATH in DarknetDao.java

e.g.  You can download yolov3.weights from http://pjreddie.com/darknet/

# Something bad
It's very slow to compute an image on phone by darknet. Predicting an 640x480 image with yolov3 casts about 7 minutes in my test. 

Maybe because that darknet is coded by c language and not using any acculerate structure like openblas.

Or there is maybe something wrong in my code.

