# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


add_library( # Sets the name of the library.
             native-lib
             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp
             src/main/cpp/DarknetAPI.cpp
             src/main/cpp/activations.c
             src/main/cpp/activation_layer.c
             src/main/cpp/art.c
             src/main/cpp/avgpool_layer.c
             src/main/cpp/batchnorm_layer.c
             src/main/cpp/blas.c
             src/main/cpp/box.c
             src/main/cpp/captcha.c
             src/main/cpp/cifar.c
             src/main/cpp/classifier.c
             src/main/cpp/coco.c
             src/main/cpp/col2im.c
             src/main/cpp/compare.c
             src/main/cpp/connected_layer.c
             src/main/cpp/convolutional_layer.c
             src/main/cpp/cost_layer.c
             src/main/cpp/cpu_gemm.c
             src/main/cpp/crnn_layer.c
             src/main/cpp/crop_layer.c
             src/main/cpp/cuda.c
             src/main/cpp/darknet.c
             src/main/cpp/data.c
             src/main/cpp/deconvolutional_layer.c
             src/main/cpp/demo.c
             src/main/cpp/detection_layer.c
             src/main/cpp/detector.c
             src/main/cpp/dice.c
             src/main/cpp/dropout_layer.c
             src/main/cpp/gemm.c
             src/main/cpp/getopt.c
             src/main/cpp/go.c
             src/main/cpp/gru_layer.c
             src/main/cpp/im2col.c
             src/main/cpp/image.c
             src/main/cpp/layer.c
             src/main/cpp/list.c
             src/main/cpp/local_layer.c
             src/main/cpp/matrix.c
             src/main/cpp/maxpool_layer.c
             src/main/cpp/network.c
             src/main/cpp/nightmare.c
             src/main/cpp/normalization_layer.c
             src/main/cpp/option_list.c
             src/main/cpp/parser.c
             src/main/cpp/region_layer.c
             src/main/cpp/reorg_layer.c
             src/main/cpp/reorg_old_layer.c
             src/main/cpp/rnn.c
             src/main/cpp/rnn_layer.c
             src/main/cpp/rnn_vid.c
             src/main/cpp/route_layer.c
             src/main/cpp/shortcut_layer.c
             src/main/cpp/softmax_layer.c
             src/main/cpp/super.c
             src/main/cpp/swag.c
             src/main/cpp/tag.c
             src/main/cpp/tree.c
             src/main/cpp/upsample_layer.c
             src/main/cpp/utils.c
             src/main/cpp/voxel.c
             src/main/cpp/writing.c
             src/main/cpp/yolo.c
             src/main/cpp/yolo_layer.c)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )