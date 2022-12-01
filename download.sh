##### build all APKs need below deps:

##### spirv-gcc-debug-linux
wget https://storage.googleapis.com/spirv-tools/artifacts/prod/graphics_shader_compiler/spirv-tools/linux-gcc-debug/continuous/1889/20221128-102508/install.tgz
tar zxvf install.tgz
mv -T ./install ./spirv-tools
rm install.tgz


##### glslang-master-linux-Release.zip
wget https://github.com/KhronosGroup/glslang/releases/download/7.9.2888/glslang-master-linux-Release.zip
sudo apt-get install -y unzip
unzip -o glslang-master-linux-Release.zip -d ./glslang
rm glslang-master-linux-Release.zip
