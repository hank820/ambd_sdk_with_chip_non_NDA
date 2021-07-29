## How to support Matter on AmebaD SDK

0. Quick Summary

	<b>libCHIP.a</b> => lib of Matter core

	<b>lib_main.a</b> => lib of Matter application. AmebaD support lighting-app in this version.

	These can be generated after Step5, by command "make -C asdk light"

	Step1 ~ Step4 are precondicion for build success with Step5

Generate patch by 

Step1
![image](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/6.2c/tools/pic/6.2c_step1.png)

Step2
![image](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/blob/6.2c/tools/pic/6.2c_step2.png)


### 1. Add lwip_v2.1.2 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/048c392ed620c70973e75c6e5a537c9710e98b0b)

<b>Add</b> component/common/network/lwip/lwip_v2.1.2
	
	=> copy full folder to component/common/network/lwip/lwip_v2.1.2

### 2. Revise Makefile to use lwip_2.1.2 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/e20fd7d2ab1ded2400f9c1abe65266dd18ff01d2) 

2.1 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile.include.gen
	
2.2 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/network/sram/Makefile
	
2.3 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/network/xip/Makefile
	
2.4 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/Makefile.include.gen
	
	=> Change include path of lwip, from lwip_2.0.2 to lwip_2.1.2
	=> Change LWIPDIR path from  lwip_2.0.2 to lwip_2.1.2

### 3. Use mbedtls in connectedhomeip/third_party/mbedtls/repo [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/337b61475f3ab7ff59ea8d0981c618d36701dd2e)

	Folder structure
	
	   ${HOME}/AmebaD_with_CHIP
	  
         ---> ambd_sdk_with_chip_non_NDA (branch 6.2c)
	  
         ---> connectedhomeip (branch base0705_gn https://github.com/hank820/connectedhomeip/tree/base0705_gn )
	   
	   mbedtls config path : connectedhomeip/config/ambd/mbedtls/mbedtls_config.h

3.1 <b>Modified</b> component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/wifi_eap_config.c
	   
	=> Fix compile error by change MBEDTLS_SSL_BUFFER_LEN to MBEDTLS_SSL_PAYLOAD_OVERHEAD
	   
3.2 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile.include.gen
	
	=> Add $(CHIPDIR), path of connectedhomeip
	=> Change include path of mbedtls, from .../mbedtls-2.4.0 to $(CHIPDIR)/third_party/mbedtls/repo/
	=> Change include path of mbedtls, from .../mbedtls-2.4.0/include to $(CHIPDIR)/third_party/mbedtls/repo/include
	=> Add MBEDTLS config path $(CHIPDIR)/config/ambd
	=> Add MBEDTLS config define GLOBAL_CFLAGS += -DMBEDTLS_CONFIG_FILE=\"mbedtls/mbedtls_config.h\"
	=> mbedtls config path : connectedhomeip/config/ambd/mbedtls/mbedtls_config.h
	   
3.3 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/ld/rlx8721d_rom_symbol_acut.ld
	
	=> Delete mbedlt related in ld file. Link without mbedtls symbol in ROM because mbedtls upgrade
	   
3.4 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/mbedtls/Makefile
	
	=> Add "make -C mbedtls-chip all" for new Makefile of Matter mbedtls

3.5 <b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/mbedtls/mbedtls-chip/Makefile

	=> New Makefile for Matter mbedtls

### 4. Update toolchain from 6.4.1 to 9.3.0 [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/73758f273ce095e7fc1d182546302231b6f3e806)

Upgrade gcc to fix C++14 build error. 
	   
4.1 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile.include.gen

4.2 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/Makefile
	
4.3 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/asdk/Makefile.include.gen
	
4.4 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_lp/toolchain/Makefile
		
	=> Change setting for toolchain 9.3.0
	
4.5 <b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/asdk/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partaa
	
<b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/asdk/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partab

<b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/asdk/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partac
	
<b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/asdk/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partad
	
<b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/toolchain/asdk/asdk-9.3.0-linux-newlib-build-3483-x86_64.tar.bz2.partae

	=> Toolchain files

### 5. Add Makeflie for libCHIP.a/lib_main.a and fix compile error [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/b9504185f4727332ff7d88277958b803429d3d57)

5.1 <b>Add</b> component/common/utilities/chip_porting.c
	
5.2 <b>Add</b> component/common/utilities/chip_porting.h
		
	=> Porting hal function for Matter

5.3 <b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip/lighting_app/Makefile
	
	=> Makefile for Matter lighting app, OUTPUT : .../GCC-RELEASE/project_hp/asdk/lib/application/lib_main.a
		
5.4 <b>Add</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/chip_main/lighting_app/Makefile

	=> Makefile for Matter core, OUTPUT : .../GCC-RELEASE/project_hp/asdk/lib/application/libCHIP.a

5.5 <b>Modified</b> component/common/api/network/include/lwipopts.h
	
	=> Enable LWIP_IPV6
		
	=> Add #ifdef __cplusplus extern "C" { #endif ...
		
5.6 <b>Modified</b> component/common/api/platform/platform_stdlib_rtl8721d.h
	
	=> Compare and merge manually
		
	=> Add "#define STD_PRINTF 1" to enable standard function
	=> Add #ifndef __cplusplus to separate "rand" between C++ and C.
	   #ifndef __cplusplus
		   #undef rand
	   #endif
	=> Merge #if CHIP_PROJECT ... 

5.7 <b>Modified</b> component/soc/realtek/amebad/app/monitor/include/shell.h

	=> Add space for "\r" to fix compile error

5.8 <b>Modified</b> component/soc/realtek/amebad/fwlib/include/rtl8721d_ram_libc.h
	
	=> Add extern function for Matter SDK.

5.9 <b>Modified</b> component/soc/realtek/amebad/fwlib/ram_common/rtl8721d_ram_libc.c
	
	=> Add new function for Matter SDK
	   strcasecmp
	   strncasecmp
	   strnlen
		
5.10 <b>Modified</b> component/soc/realtek/amebad/swlib/string/rand.c
	
5.11 <b>Modified</b> component/soc/realtek/amebad/swlib/string/rand.h
	
	=> Add "#ifndef __cplusplus" to separate "rand" between C++ and C. 
	
5.12 <b>Modified</b> component/soc/realtek/amebad/swlib/string/strproc.h
	
	=> Change "#ifndef isprint" to "#ifndef __cplusplus" to fix C++ compatibility.
		
5.13 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile
	
	=> Add "make_subdirs_lib_light" for "make -C asdk light"
		
5.14 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile.include.gen
	
	=> Add CPP, g++ for support C++
	=> Add C++ option
		 CPPFLAGS += -std=gnu++11
		 CPPFLAGS += -std=c++14
		 CPPFLAGS += -fno-rtti
	=> Add "-DLWIP_IPV6_ROUTE_TABLE_SUPPORT=1" for Matter SDK
		
5.15 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/make/utilities/Makefile

	=> Add chip_porting.c
   
### 6. Add Matter task. Trigger by ATS$ [[ref commit]](https://github.com/hank820/ambd_sdk_with_chip_non_NDA/commit/ee1bc3d68325db329d09ee52c3b6392957cb764d)

6.1 <b>Modified</b> component/common/api/at_cmd/atcmd_sys.c
	
	=> Add "fATchipapp" for ATS$. Trigger Matter task.
		
6.2 <b>Modified</b> component/common/api/network/include/lwipopts.h
		
	=> Enlarge PBUF_POOL_BUFSIZE from 508 to 1280
		Fixed Matter run time error "chip[CSL] PacketBuffer: allocation too large"
	
	=> Add lwip options to fix "Failed to initialize Udp transport: LwIP Error"
		#define LWIP_TCPIP_CORE_LOCKING         1
		#define LWIP_COMPAT_MUTEX_ALLOWED       1
		#define LWIP_IPV6_ND                    0
		#define LWIP_IPV6_SCOPES                0
		#define LWIP_PBUF_FROM_CUSTOM_POOLS     0
	
6.3 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/Makefile

	=> Add "make -C asdk light"
	
6.4 <b>Modified</b> project/realtek_amebaD_va0_example/GCC-RELEASE/project_hp/asdk/Makefile
	
	=> Link libCHIP.a and lib_main.a
		LINK_APP_LIB += $(ROOTDIR)/lib/application/lib_main.a
		LINK_APP_LIB += $(ROOTDIR)/lib/application/libCHIP.a
	
6.5 <b>Modified</b> project/realtek_amebaD_va0_example/inc/inc_hp/FreeRTOSConfig.h
	
	=> Reduce "configTOTAL_HEAP_SIZE" from 250 to 200 because code size become large.
