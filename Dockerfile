FROM ubuntu:22.04

WORKDIR /build
COPY ./                     ./VulkanSamples


RUN apt-get update && apt-get install -y cmake git wget unzip g++ python3 openjdk-8-jdk

# The ABI could be (arm64-v8a，armeabi-v7a, x86, x86_64)
RUN cd /build/VulkanSamples/API-Samples && \
    cmake -DANDROID=ON -DABI_NAME=arm64-v8a && \
    cd /build/VulkanSamples/API-Samples/android && \
    python3 compile_shaders.py

RUN chmod a+x /build/VulkanSamples/docker/build_copy_script.sh && \
    /build/VulkanSamples/docker/build_copy_script.sh -all


#### Sdk
#RUN wget https://dl.google.com/android/repository/commandlinetools-linux-9123335_latest.zip && \
#    unzip commandlinetools-linux-9123335_latest.zip
#### Deps spirv-tools prebuild
#RUN wget https://storage.googleapis.com/spirv-tools/artifacts/prod/graphics_shader_compiler/spirv-tools/linux-gcc-debug/continuous/1889/20221128-102508/install.tgz


#### Vulkan-Repo
#RUN git clone https://github.com/LunarG/VulkanSamples.git && \
#    cd /build/VulkanSamples/API-Samples/
#    cmake -DANDROID=ON -DABI_NAME=arm64-v8 && \
#    cd /build/VulkanSamples/API-Samples/android && \
#    python3 compile_shaders.py
    
# RUN apt install -y openjdk-8-jdk 

# RUN chmod a+x build_copy_script.sh && /build/build_copy_script.sh -all


