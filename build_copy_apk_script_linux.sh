#!/bin/bash

# The absolute path to VulkanSamples/API-Samples/android/
BLD_BASE_DIR=
# Generally, don't need to modify this line in debug mode.
BLD_DIR=build/outputs/apk/debug
# All the APK compiled will be copied to here.
TGT_DIR=

# The absolute path to Android SDK root. /xxx...xxx/Android/Sdk/
export ANDROID_SDK_ROOT=


PROJECT_ARRAY[1]='01-init_instance'
PROJECT_ARRAY[2]='02-enumerate_devices'
PROJECT_ARRAY[3]='03-init_device'
PROJECT_ARRAY[4]='04-init_command_buffer'
PROJECT_ARRAY[5]='05-init_swapchain'
PROJECT_ARRAY[6]='06-init_depth_buffer'
PROJECT_ARRAY[7]='07-init_uniform_buffer'
PROJECT_ARRAY[8]='08-init_pipeline_layout'
PROJECT_ARRAY[9]='09-init_descriptor_set'
PROJECT_ARRAY[10]='10-init_render_pass'
PROJECT_ARRAY[11]='11-init_shaders'
PROJECT_ARRAY[12]='12-init_frame_buffers'
PROJECT_ARRAY[13]='13-init_vertex_buffer'
PROJECT_ARRAY[14]='14-init_pipeline'
PROJECT_ARRAY[15]='15-draw_cube'
PROJECT_ARRAY[16]='16-vulkan_1_1'
PROJECT_ARRAY[17]='copy_blit_image'
PROJECT_ARRAY[18]='create_debug_report_callback'
PROJECT_ARRAY[19]='draw_subpasses'
PROJECT_ARRAY[20]='draw_textured_cube'
PROJECT_ARRAY[21]='dynamic_uniform'
PROJECT_ARRAY[22]='enable_validation_with_callback'
PROJECT_ARRAY[23]='enumerate_devices_adv'
PROJECT_ARRAY[24]='events'
PROJECT_ARRAY[25]='immutable_sampler'
PROJECT_ARRAY[26]='init_texture'
PROJECT_ARRAY[27]='input_attachment'
PROJECT_ARRAY[28]='instance_extension_properties'
PROJECT_ARRAY[29]='instance_layer_extension_properties'
PROJECT_ARRAY[30]='instance_layer_properties'
PROJECT_ARRAY[31]='memory_barriers'
PROJECT_ARRAY[32]='multiple_sets'
PROJECT_ARRAY[33]='multithreaded_command_buffers'
PROJECT_ARRAY[34]='occlusion_query'
PROJECT_ARRAY[35]='pipeline_cache'
PROJECT_ARRAY[36]='pipeline_derivative'
PROJECT_ARRAY[37]='push_constants'
PROJECT_ARRAY[38]='push_descriptors'
PROJECT_ARRAY[39]='secondary_command_buffer'
PROJECT_ARRAY[40]='separate_image_sampler'
PROJECT_ARRAY[41]='spirv_assembly'
PROJECT_ARRAY[42]='spirv_specialization'
PROJECT_ARRAY[43]='template'
PROJECT_ARRAY[44]='texel_buffer'
PROJECT_ARRAY[45]='validation_cache'
PROJECT_ARRAY[46]='vulkan_1_1_flexible'


PRINT_PROJECTS(){
	for i in $(seq 1 46)
	do
	    echo "${i} : ${PROJECT_ARRAY[i]}"
	done
}


BUILD_AND_COPY(){
    PROJECT_NAME=$1
    cd ${BLD_BASE_DIR}/${PROJECT_NAME} && ../gradlew build
    APK_PATH=${BLD_BASE_DIR}/${PROJECT_NAME}/${BLD_DIR}/${PROJECT_NAME}-debug.apk
    if [[ -f "${APK_PATH}" ]]; then 
        cp ${APK_PATH} ${TGT_DIR}
    else
        echo "${APK_PATH} not exist or BUILD FAILED."
    fi
}


for arg in "$@"; do
    if [ $arg == "-h" ]; then 
        PRINT_PROJECTS
        echo " Pass argument number or project name or '-all'，to build and copy apks."
        exit
    fi

    if [ $arg == "-all" ]; then
        for i in $(seq 1 46); do
            BUILD_AND_COPY ${PROJECT_ARRAY[$i]}
        done
        exit
    fi

    for i in $(seq 1 46); do
        if [ $arg == $i ]; then
            BUILD_AND_COPY ${PROJECT_ARRAY[$i]}
        elif [[ $arg == ${PROJECT_ARRAY[$i]} ]]; then
            BUILD_AND_COPY ${PROJECT_ARRAY[$i]}
        fi
    done

done



