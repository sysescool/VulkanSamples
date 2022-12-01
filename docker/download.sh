# download sdkmanager to download android sdk
# and copy them to docker images
wget https://dl.google.com/android/repository/commandlinetools-linux-9123335_latest.zip
unzip commandlinetools-linux-9123335_latest.zip
rm commandlinetools-linux-9123335_latest.zip
# yes | ./cmdline-tools/bin/sdkmanager --sdk_root=./Sdk --install "platforms;android-33"
./cmdline-tools/bin/sdkmanager --sdk_root=./Sdk --install "platforms;android-33"
./cmdline-tools/bin/sdkmanager --sdk_root=./Sdk --install "ndk;21.3.6528147"
./cmdline-tools/bin/sdkmanager --sdk_root=./Sdk --install "cmake;3.10.2.4988404"
./cmdline-tools/bin/sdkmanager --sdk_root=./Sdk --install "emulator"


