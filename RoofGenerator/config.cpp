#include "Config.h"

Config::Config() {

}
Config::Config(double center_x_ratio, double center_y_ratio, double roofWidth_ratio, double roofAspect, double rotate, int selected_roof_type, bool bRidgeDis, double ridgeDisRatio, double ridgeRatio){
	this->center_x_ratio = center_x_ratio;
	this->center_y_ratio = center_y_ratio;
	this->roofWidth_ratio = roofWidth_ratio;
	this->roofAspect = roofAspect;
	this->rotate = rotate;
	this->selected_roof_type = selected_roof_type;
	this->bRidgeDis = bRidgeDis;
	this->ridgeDisRatio = ridgeDisRatio;
	this->ridgeRatio = ridgeRatio;
}