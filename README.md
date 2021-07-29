# Matter (previously CHIP) on AmebaD

## How to support Matter on AmebaD SDK

https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/6.2c/PORTING.md

## CHIP Ameba-D lighting-app Example

README

    https://github.com/hank820/connectedhomeip/tree/base0705_gn/examples/lighting-app/ambd


## Get AmebaD SDK (Tested on Ubuntu 20.04)

	mkdir ${HOME}/AmebaD_with_CHIP

	cd ${HOME}/AmebaD_with_CHIP

	git clone -b 6.2c https://github.com/hank820/ambd_sdk_with_chip_non_NDA.git

## Get Matter SDK

	cd ${HOME}/AmebaD_with_CHIP

	git clone -b base0705_gn --recurse-submodules https://github.com/hank820/connectedhomeip

## Set Matter Build Environment 

    cd third_party/connectedhomeip

    source scripts/bootstrap.sh

    source scripts/activate.sh

    > Find more details to setup linux build environment
    > https://github.com/hank820/connectedhomeip/blob/master/docs/BUILDING.md


## Make Little CPU
    cd ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp

    make all
    
    output : project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/image/km0_boot_all.bin

## Make CHIP library by gn and Make lib_main.a
### lighting-app

    cd ambd_sdk_with_chip/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp
    make -C asdk light

### CHIP core (generate by GN/ninja in connectedhomeip. Config by [chip/Makefile](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/main/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip/Makefile))

    output : ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/lib/application

    > libCHIP.a, ibCoreTests.a, ibChipCryptoTests.a, ibRawTransportTests.a...

### CHIP application (generate by [chip_main/Makefile](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/main/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip_main/Makefile))

    output : ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/lib/application

    > lib_main.a

## Make Big CPU
    cd ambd_sdk_with_chip_non_NDA/project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp

    make all
    
    output : 

    project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km4_boot_all.bin
    
    project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/image/km0_km4_image2.bin

## Flash Image on AmebaD EVB

Please refer [Application Note](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/master/doc/AN0400%20Ameba-D%20Application%20Note%20v14.pdf) Chapter 8 : Image Tool

    Image Tool Path : $(SDK_ROOT)/tools/AmebaD/Image_Tool/
    

## Run CHIP task on Ameba D (lighting-app example)
    enter command in console

    ATW0=testAP

    ATW1=password

    ATWC

    ATS$ => Run chip task


## Test with [chip-tool](https://github.com/hank820/connectedhomeip/tree/master/examples/chip-tool)
Use standalone chip-tool app(linux) to communicate with the device.

`./chip-tool pairing bypass 192.168.xxx.xxx 11097  (AmebaD IP)`

<b>onoff cluster</b>

Use PB_5 as output, connect a LED to this pin and GND.

`./chip-tool onoff on 1`

`./chip-tool onoff off 1`
