#include <ctime>
#include <jni.h>
#include <android/log.h>
#include "DarknetAPI.h"

#define  LOG_TAG    "Darknet"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

DarknetAPI::DarknetAPI(char *name_list, char *cfgfile, char *weightfile)
{
	net = parse_network_cfg_custom(cfgfile, 1); // set batch=1

	if (weightfile) {
		load_weights(&net, weightfile);
	}
	fuse_conv_batchnorm(net);
	srand(2222222);

	names = get_labels(name_list);

}

std::vector<Result> DarknetAPI::detect(image & im, float thresh, float hier_thresh){
	LOGI("JNI: detect");
	float nms = .45;	// 0.4F
	double time;
	char buff[256];
	char *input = buff;

	int letterbox = 0;
	image sized = resize_image(im, net.w, net.h);
	layer l = net.layers[net.n - 1];

	float *X = sized.data;
	LOGI("JNI: begin");
	std::clock_t begin = std::clock();
	network_predict(net, X);
	std::clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	LOGI("JNI: %s: Predicted in %f seconds.\n", std::string(input).c_str(), (elapsed_secs));
	//printf("JNI: %s: Predicted in %f seconds.\n", std::string(input).c_str(), (elapsed_secs));

	int nboxes = 0;
	detection *dets = get_network_boxes(&net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes, letterbox);
	if (nms) {
		do_nms_sort(dets, nboxes, l.classes, nms);
	}

	int i, j;
	std::vector<Result> results;
	for (i = 0; i < nboxes; ++i) {
		std::string label;
		int class_id = -1;
		float prob = 0;
		for (j = 0; j < l.classes; ++j) {
			if (dets[i].prob[j] > thresh) {
				if (class_id < 0) {
					prob = dets[i].prob[j];
					label = std::string(names[j]);
					class_id = j;
					break;
				}
			}
		}
		if (class_id >= 0) {
			Result r;
			box b = dets[i].bbox;
			r.left = (b.x - b.w / 2.)*im.w;
			r.right = (b.x + b.w / 2.)*im.w;
			r.top = (b.y - b.h / 2.)*im.h;
			r.bot = (b.y + b.h / 2.)*im.h;

			if (r.left < 0) r.left = 0;
			if (r.right > im.w - 1) r.right = im.w - 1;
			if (r.top < 0) r.top = 0;
			if (r.bot > im.h - 1) r.bot = im.h - 1;

			r.label = label;
			r.confidence = prob;
			results.push_back(r);
		}
	}
	
	free_detections(dets, nboxes);
	free_image(im);
	free_image(sized);
	return results;
}

DarknetAPI::~DarknetAPI()
{
}
