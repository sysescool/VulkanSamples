[TOC]

# android build in docker

This folder is used to build APK in docker on ubuntu or linux.

all you should do in ubuntu is:

```bash
# download spirv-tools and glslang
cd VulkanSamples
./download.sh

# download android Sdk for docker.
cd VulkanSamples/docker
./download.sh
cd VulkanSamples
docker build -t vksample .
docker run -it vksample /bin/bash
$ cd /build/VulkanSamples/docker/BUILD_APKs && ls
### The output below:
# 01-init_instance-debug.apk                 events-debug.apk
# 02-enumerate_devices-debug.apk             immutable_sampler-debug.apk
# 03-init_device-debug.apk                   init_texture-debug.apk
# 04-init_command_buffer-debug.apk           input_attachment-debug.apk
# 05-init_swapchain-debug.apk                instance_extension_properties-debug.apk
# 06-init_depth_buffer-debug.apk             instance_layer_extension_properties-debug.apk
# 07-init_uniform_buffer-debug.apk           instance_layer_properties-debug.apk
# 08-init_pipeline_layout-debug.apk          memory_barriers-debug.apk
# 09-init_descriptor_set-debug.apk           multiple_sets-debug.apk
# 10-init_render_pass-debug.apk              multithreaded_command_buffers-debug.apk
# 11-init_shaders-debug.apk                  occlusion_query-debug.apk
# 12-init_frame_buffers-debug.apk            pipeline_cache-debug.apk
# 13-init_vertex_buffer-debug.apk            pipeline_derivative-debug.apk
# 14-init_pipeline-debug.apk                 push_constants-debug.apk
# 15-draw_cube-debug.apk                     push_descriptors-debug.apk
# 16-vulkan_1_1-debug.apk                    secondary_command_buffer-debug.apk
# copy_blit_image-debug.apk                  separate_image_sampler-debug.apk
# create_debug_report_callback-debug.apk     spirv_assembly-debug.apk
# draw_subpasses-debug.apk                   spirv_specialization-debug.apk
# draw_textured_cube-debug.apk               template-debug.apk
# dynamic_uniform-debug.apk                  texel_buffer-debug.apk
# enable_validation_with_callback-debug.apk  validation_cache-debug.apk
# enumerate_devices_adv-debug.apk            vulkan_1_1_flexible-debug.apk
```

## Copy APKs from running containers

Don't close above console, and open another console:

```bash
cd VulkanSamples/docker
docker ps
docker cp [CONTAINER ID]:/build/VulkanSamples/docker/BUILD_APKs/  ./
# for example:
# docker cp b4f7c253e69e:/build/VulkanSamples/docker/BUILD_APKs/  ./
# Then all the APKs are in the folder VulkanSamples/docker/BUILD_APKs/
```

