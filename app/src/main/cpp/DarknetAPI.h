#pragma once

#include <string>
#include <vector>

extern "C" {
#include "network.h"
#include "parser.h"
#include "utils.h"
}

struct Result {
	int left;
	int right;
	int top;
	int bot;
	float confidence;
	std::string label;
};

class DarknetAPI
{

private:
	network net;
	char **names;

public:
	DarknetAPI(char *name_list ,char *cfgfile, char *weightfile);
	~DarknetAPI();


	std::vector<Result> detect(image & im, float thresh=0.5, float hier_thresh=0.5);
};

