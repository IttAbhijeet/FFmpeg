#include <stdint.h>
#include <string.h>

#include "shutter_interval.h"
#include "mem.h"

AVShutterInterval *av_shutter_interval_alloc(void)
{
    return av_mallocz(sizeof(AVShutterInterval));
}

AVShutterInterval *av_shutter_interval_create_side_data(AVFrame *frame)
{
    AVFrameSideData *side_data = av_frame_new_side_data(frame,
                                                        AV_FRAME_DATA_SHUTTER_INTERVAL,
                                                        sizeof(AVShutterInterval));
    if (!side_data)
        return NULL;

    memset(side_data->data, 0, sizeof(AVShutterInterval));

    return (AVShutterInterval *)side_data->data;
}
