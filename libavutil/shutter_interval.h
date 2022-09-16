#ifndef AVUTIL_SHUTTER_INTERVAL_H
#define AVUTIL_SHUTTER_INTERVAL_H

#include "frame.h"
	
typedef struct AVShutterInterval{
	uint32_t sii_time_scale;
	uint8_t  fixed_shutter_interval_within_clvs_flag;
	uint32_t sii_num_units_in_shutter_interval;
	uint8_t  sii_max_sub_layers;
	uint32_t sub_layer_num_units_in_shutter_interval[8];
	
} AVShutterInterval;

AVShutterInterval *av_shutter_interval_alloc(void);

AVShutterInterval *av_shutter_interval_create_side_data(AVFrame *frame);

#endif