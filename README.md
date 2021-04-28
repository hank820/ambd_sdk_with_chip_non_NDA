# ambd_sdk_with_chip_non_NDA

Test on Ubuntu 20.04

# Folder Structure
ambd_sdk/

├── ambd_sdk_with_chip_non_NDA

└── connectedhomeip

# Get amebaD SDK
mkdir ambd_sdk

cd ambd_sdk/

git clone https://github.com/hank820/ambd_sdk_with_chip_non_NDA.git

# Get chip SDK 
cd ambd_sdk/

git clone --recurse-submodules -b base0405_gn https://github.com/hank820/connectedhomeip.git

cd connectedhomeip/src/platform

ln -s ../../../ambd_sdk_with_chip_non_NDA/chip/AMBD

cd connectedhomeip

source scripts/bootstrap.sh

source scripts/activate.sh

 > Find more details to setup linux build environment
 > https://github.com/hank820/connectedhomeip/blob/base0405_gn/docs/BUILDING.md


# Make little CPU
cd ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp

make all


# Make CHIP library by gn and Make lib_main.a

cd ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp

make -C asdk lib_all

output : ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/lib/application

# Make big CPU
cd ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp

make all

# Flash image
ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/image/km0_boot_all.bin

ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km4_boot_all.bin

ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km0_km4_image2.bin

# Run all-cluster-app example
enter "ATS$" in console


# Test with [chip-tool](https://github.com/hank820/connectedhomeip/tree/base0405_gn/examples/chip-tool)
Use standalone chip-tool app to communicate with the device.

./chip-tool pairing bypass 192.168.0.xxx 11097  (AmebaD IP)

./chip-tool onoff on 1

./chip-tool onoff off 1
