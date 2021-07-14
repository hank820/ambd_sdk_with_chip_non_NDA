# Matter (previously CHIP) on AmebaD

## How to support Matter on AmebaD SDK

1. Add lwip_v2.1.2 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/048c392ed620c70973e75c6e5a537c9710e98b0b)

2. Revise Makefile to use lwip_2.1.2 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/e20fd7d2ab1ded2400f9c1abe65266dd18ff01d2) 

3. Use mbedtls in connectedhomeip/third_party/mbedtls/repo [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/337b61475f3ab7ff59ea8d0981c618d36701dd2e)

       Folder structure
	
       ${HOME}/AmebaD_with_CHIP
	  
         ---> ambd_sdk_with_chip_non_NDA (branch 6.2c)
	  
         ---> connectedhomeip (branch base0705_gn https://github.com/hank820/connectedhomeip/tree/base0705_gn )
	   
	   mbedtls config path : connectedhomeip/config/ambd/mbedtls/mbedtls_config.h

4. Update toolchain from 6.4.1 to 9.3.0 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/73758f273ce095e7fc1d182546302231b6f3e806)

       Upgrade gcc to fix C++14 build error. 

5. Add Makeflie for libCHIP.a/lib_main.a and fix compile error [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/b9504185f4727332ff7d88277958b803429d3d57)

       5.1 Enable LWIP_IPV6
	   5.2 Enable STD_PRINTF
	   5.3 Specific setting in component/common/api/platform/platform_stdlib_rtl8721d.h to compatible Matter SDK
	   5.4 [libCHIP.a] Add .../GCC-RELEASE/project_hp/asdk/make/chip/lighting_app/Makefile
	   5.5 [lib_main.a] Add .../GCC-RELEASE/project_hp/asdk/make/chip_main/lighting_app/Makefile
	   
6. Add Matter task. Trigger by ATS$ [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/ee1bc3d68325db329d09ee52c3b6392957cb764d)

       6.1 Add ATS$ to start Matter task
	   6.2 Fix "Failed to initialize Udp transport: LwIP Error" by define LWIP_TCPIP_CORE_LOCKING, LWIP_COMPAT_MUTEX_ALLOWE... in lwipopts.h
	   6.3 Fix "chip[CSL] PacketBuffer: allocation too large" by Enlarge PBUF_POOL_BUFSIZE

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
