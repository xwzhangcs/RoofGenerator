#pragma once

#include <vector>
class Config {
public:
	double center_x_ratio;
	double center_y_ratio;
	double roofWidth_ratio;
	double roofAspect;
	double rotate;
	int selected_roof_type;
	bool bRidgeDis;
	double ridgeDisRatio;
	double ridgeRatio;

public:
	Config();
	Config(double center_x_ratio, double center_y_ratio, double roofWidth_ratio, double roofAspect, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, double ridgeRatio);
};

