/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_H264_SEI_H
#define AVCODEC_H264_SEI_H

#include "get_bits.h"
#include "h264_ps.h"
#include "sei.h"


/**
 * pic_struct in picture timing SEI message
 */
typedef enum {
    H264_SEI_PIC_STRUCT_FRAME             = 0, ///<  0: %frame
    H264_SEI_PIC_STRUCT_TOP_FIELD         = 1, ///<  1: top field
    H264_SEI_PIC_STRUCT_BOTTOM_FIELD      = 2, ///<  2: bottom field
    H264_SEI_PIC_STRUCT_TOP_BOTTOM        = 3, ///<  3: top field, bottom field, in that order
    H264_SEI_PIC_STRUCT_BOTTOM_TOP        = 4, ///<  4: bottom field, top field, in that order
    H264_SEI_PIC_STRUCT_TOP_BOTTOM_TOP    = 5, ///<  5: top field, bottom field, top field repeated, in that order
    H264_SEI_PIC_STRUCT_BOTTOM_TOP_BOTTOM = 6, ///<  6: bottom field, top field, bottom field repeated, in that order
    H264_SEI_PIC_STRUCT_FRAME_DOUBLING    = 7, ///<  7: %frame doubling
    H264_SEI_PIC_STRUCT_FRAME_TRIPLING    = 8  ///<  8: %frame tripling
} H264_SEI_PicStructType;

/**
 * frame_packing_arrangement types
 */
typedef enum {
    H264_SEI_FPA_TYPE_CHECKERBOARD        = 0,
    H264_SEI_FPA_TYPE_INTERLEAVE_COLUMN   = 1,
    H264_SEI_FPA_TYPE_INTERLEAVE_ROW      = 2,
    H264_SEI_FPA_TYPE_SIDE_BY_SIDE        = 3,
    H264_SEI_FPA_TYPE_TOP_BOTTOM          = 4,
    H264_SEI_FPA_TYPE_INTERLEAVE_TEMPORAL = 5,
    H264_SEI_FPA_TYPE_2D                  = 6,
} H264_SEI_FpaType;

typedef struct H264SEITimeCode {
    /* When not continuously receiving full timecodes, we have to reference
       the previous timecode received */
    int full;
    int frame;
    int seconds;
    int minutes;
    int hours;
    int dropframe;
} H264SEITimeCode;

typedef struct H264SEIPictureTiming {
    // maximum size of pic_timing according to the spec should be 274 bits
    uint8_t payload[40];
    int     payload_size_bits;

    int present;
    H264_SEI_PicStructType pic_struct;

    /**
     * Bit set of clock types for fields/frames in picture timing SEI message.
     * For each found ct_type, appropriate bit is set (e.g., bit 1 for
     * interlaced).
     */
    int ct_type;

    /**
     * dpb_output_delay in picture timing SEI message, see H.264 C.2.2
     */
    int dpb_output_delay;

    /**
     * cpb_removal_delay in picture timing SEI message, see H.264 C.1.2
     */
    int cpb_removal_delay;

    /**
     * Maximum three timecodes in a pic_timing SEI.
     */
    H264SEITimeCode timecode[3];

    /**
     * Number of timecode in use
     */
    int timecode_cnt;
} H264SEIPictureTiming;

typedef struct H264SEIAFD {
    int present;
    uint8_t active_format_description;
} H264SEIAFD;

typedef struct H264SEIA53Caption {
    AVBufferRef *buf_ref;
} H264SEIA53Caption;

typedef struct H264SEIUnregistered {
    int x264_build;
    AVBufferRef **buf_ref;
    int nb_buf_ref;
} H264SEIUnregistered;

typedef struct H264SEIRecoveryPoint {
    /**
     * recovery_frame_cnt
     *
     * Set to -1 if no recovery point SEI message found or to number of frames
     * before playback synchronizes. Frames having recovery point are key
     * frames.
     */
    int recovery_frame_cnt;
} H264SEIRecoveryPoint;

typedef struct H264SEIBufferingPeriod {
    int present;   ///< Buffering period SEI flag
    int initial_cpb_removal_delay[32];  ///< Initial timestamps for CPBs
} H264SEIBufferingPeriod;

typedef struct H264SEIFramePacking {
    int present;
    int arrangement_id;
    int arrangement_cancel_flag;  ///< is previous arrangement canceled, -1 if never received
    H264_SEI_FpaType arrangement_type;
    int arrangement_repetition_period;
    int content_interpretation_type;
    int quincunx_sampling_flag;
    int current_frame_is_frame0_flag;
} H264SEIFramePacking;

typedef struct H264SEIDisplayOrientation {
    int present;
    int anticlockwise_rotation;
    int hflip, vflip;
} H264SEIDisplayOrientation;

typedef struct H264SEIGreenMetaData {
    uint8_t green_metadata_type;
    uint8_t period_type;
    uint16_t num_seconds;
    uint16_t num_pictures;
    uint8_t percent_non_zero_macroblocks;
    uint8_t percent_intra_coded_macroblocks;
    uint8_t percent_six_tap_filtering;
    uint8_t percent_alpha_point_deblocking_instance;
    uint8_t xsd_metric_type;
    uint16_t xsd_metric_value;
} H264SEIGreenMetaData;

typedef struct H264SEIAlternativeTransfer {
    int present;
    int preferred_transfer_characteristics;
} H264SEIAlternativeTransfer;

typedef struct H264SEIFilmGrainCharacteristics {
    int present;
    int model_id;
    int separate_colour_description_present_flag;
    int bit_depth_luma;
    int bit_depth_chroma;
    int full_range;
    int color_primaries;
    int transfer_characteristics;
    int matrix_coeffs;
    int blending_mode_id;
    int log2_scale_factor;
    int comp_model_present_flag[3];
    uint16_t num_intensity_intervals[3];
    uint8_t num_model_values[3];
    uint8_t intensity_interval_lower_bound[3][256];
    uint8_t intensity_interval_upper_bound[3][256];
    int16_t comp_model_value[3][256][6];
    int repetition_period;
} H264SEIFilmGrainCharacteristics;

typedef struct H264SEIMasteringDisplay {
    int present;
    uint16_t display_primaries[3][2];
    uint16_t white_point[2];
    uint32_t max_luminance;
    uint32_t min_luminance;
} H264SEIMasteringDisplay;

typedef struct H264SEIContentLight {
    int present;
    uint16_t max_content_light_level;
    uint16_t max_pic_average_light_level;
} H264SEIContentLight;

typedef struct H264SEIContext {
    H264SEIPictureTiming picture_timing;
    H264SEIAFD afd;
    H264SEIA53Caption a53_caption;
    H264SEIUnregistered unregistered;
    H264SEIRecoveryPoint recovery_point;
    H264SEIBufferingPeriod buffering_period;
    H264SEIFramePacking frame_packing;
    H264SEIDisplayOrientation display_orientation;
    H264SEIGreenMetaData green_metadata;
    H264SEIAlternativeTransfer alternative_transfer;
    H264SEIFilmGrainCharacteristics film_grain_characteristics;
	H264SEIMasteringDisplay mastering_display;
	H264SEIContentLight content_light;
	
} H264SEIContext;

struct H264ParamSets;

int ff_h264_sei_decode(H264SEIContext *h, GetBitContext *gb,
                       const struct H264ParamSets *ps, void *logctx);

/**
 * Reset SEI values at the beginning of the frame.
 */
void ff_h264_sei_uninit(H264SEIContext *h);

/**
 * Get stereo_mode string from the h264 frame_packing_arrangement
 */
const char *ff_h264_sei_stereo_mode(const H264SEIFramePacking *h);

/**
 * Parse the contents of a picture timing message given an active SPS.
 */
int ff_h264_sei_process_picture_timing(H264SEIPictureTiming *h, const SPS *sps,
                                       void *logctx);

#endif /* AVCODEC_H264_SEI_H */
