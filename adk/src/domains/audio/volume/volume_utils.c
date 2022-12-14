/*!
\copyright  Copyright (c) 2018-2021 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\brief      volume_t related utility functions.
*/

#include "volume_utils.h"
#include <kymera_volume.h>
#include <kymera_config.h>

static volume_config_t volumeUtils_GetOutputVolumeConfig(void)
{
    volume_config_t config =
    {
        .range = { .min = appConfigMinVolumedB(), .max = appConfigMaxVolumedB() },
        .number_of_steps = ((appConfigMaxVolumedB() - appConfigMinVolumedB())+1)
    };
    return config;
}

int VolumeUtils_LimitVolumeToRange(int volume, range_t range)
{
    int volume_result = volume;

    if(volume >= range.max)
    {
        volume_result = range.max;
    }
    if(volume <= range.min)
    {
        volume_result = range.min;
    }

    return volume_result;
}

int VolumeUtils_GetStepSize(volume_config_t config)
{
    unsigned steps = ((config.number_of_steps > 1) ? (config.number_of_steps-1) : 1);
    return ((config.range.max - config.range.min) / steps);
}

static inline int16 volumeUtils_GetRange(volume_config_t config)
{
    return config.range.max - config.range.min;
}

int16 VolumeUtils_ConvertToVolumeConfig(volume_t volume_to_convert, volume_config_t output_format)
{
    int16 input_range = volumeUtils_GetRange(volume_to_convert.config);
    int16 output_range = volumeUtils_GetRange(output_format);
    /* Add 1/2 the input range so arithmetic rounding is used.
       Without this "simple" conversions fail. In that case 80/127 becomes 160/25 becomes 79/127 */
    int16 converted_value = ((volume_to_convert.value - volume_to_convert.config.range.min) * output_range + input_range / 2) 
                            / input_range;
    int16 output_step_size = VolumeUtils_GetStepSize(output_format);

    if ((converted_value % output_step_size) != 0)
    {
        int16 output_step_level = (converted_value / output_step_size);
        if ((2 * (converted_value % output_step_size)) > output_step_size)
        {
            output_step_level++;
        }
        converted_value = output_step_level * output_step_size;
    }
    
    converted_value += output_format.range.min;
    
    return VolumeUtils_LimitVolumeToRange(converted_value, output_format.range);
}

bool VolumeUtils_VolumeIsEqual(volume_t volume_1, volume_t volume_2)
{
    return ((volume_1.value == volume_2.value) &&
            (volume_1.config.number_of_steps == volume_2.config.number_of_steps) &&
            (volume_1.config.range.min == volume_2.config.range.min) &&
            (volume_1.config.range.max == volume_2.config.range.max));
}

int VolumeUtils_IncrementVolume(volume_t volume)
{
    return VolumeUtils_LimitVolumeToRange((volume.value + VolumeUtils_GetStepSize(volume.config)), volume.config.range);
}

int VolumeUtils_DecrementVolume(volume_t volume)
{
    return VolumeUtils_LimitVolumeToRange((volume.value - VolumeUtils_GetStepSize(volume.config)), volume.config.range);
}

unsigned VolumeUtils_GetVolumeInPercent(volume_t volume)
{
    return (((volume.value - volume.config.range.min) * 100) / (volume.config.range.max - volume.config.range.min));
}

int VolumeUtils_GetLimitedVolume(volume_t volume, volume_t limit)
{
    int value = volume.value;
    int limited_value = VolumeUtils_ConvertToVolumeConfig(limit, volume.config);
    if(volume.value > limited_value)
    {
        value = limited_value;
    }
    return value;
}

int16 VolumeUtils_GetVolumeInDb(volume_t volume)
{
    int32 gain = VOLUME_MUTE_IN_DB;
    if(volume.value > volume.config.range.min)
    {
        gain = VolumeUtils_ConvertToVolumeConfig(volume, volumeUtils_GetOutputVolumeConfig());
    }
    return gain;
}
