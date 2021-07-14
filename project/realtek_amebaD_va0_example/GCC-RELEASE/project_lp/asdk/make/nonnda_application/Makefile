
include $(MAKE_INCLUDE_GEN)

.PHONY: lib clean

#*****************************************************************************#
#                               VARIABLES	                              #
#*****************************************************************************#
DIR_RAM_LP = $(BASEDIR)/component/soc/realtek/amebad/fwlib/ram_lp
DIR_MISC = $(BASEDIR)/component/soc/realtek/amebad/misc
DIR_USRCFG = $(BASEDIR)/component/soc/realtek/amebad/fwlib/usrcfg


vpath %.c $(DIR_RAM_LP) $(DIR_MISC) $(DIR_USRCFG) 

#*****************************************************************************#
#                               Source FILE LIST                              #
#*****************************************************************************#
CSRC =	$(DIR_RAM_LP)/rtl8721dlp_pmc.c \
	$(DIR_RAM_LP)/rtl8721dlp_pmc_ram.c \
	$(DIR_MISC)/rtl8721d_freertos_pmu.c \
	$(DIR_USRCFG)/rtl8721dlp_sleepcfg.c \


#*****************************************************************************#
#                               Object FILE LIST                              #
#*****************************************************************************#
OBJS = $(notdir $(CSRC:.c=.o))

#*****************************************************************************#
#                        RULES TO GENERATE TARGETS                            #
#*****************************************************************************#

lib: CORE_TARGETS COPY_RAM_OBJS
	$(AR) rvs lib_application_lp.a $(OBJS)
	$(MOVE) -f lib_application_lp.a $(ROOTDIR)/lib/application
CORE_TARGETS: $(OBJS)

#*****************************************************************************#
#                        RULES TO CLEAN TARGETS                               #
#*****************************************************************************#
clean: CLEAN_OBJS 
	$(REMOVE) *.o
	$(REMOVE) *.i
	$(REMOVE) *.s
	$(REMOVE) *.d
	$(REMOVE) *.su

-include $(DEPS)
