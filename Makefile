## Setting up PATH variable
arm-toolchain := C:\Program Files\ArmCompilerforEmbedded6.22\bin
tivaware-toolchain := C:\ti\TivaWare_C_Series-2.2.0.295\tools\bin
gnuwin32-bin := C:\Program Files (x86)\GnuWin32\bin
# Hardcode in which version of "find" to use, so we can easily swap uit gnuwin32
# for mingw etc.... Only the gnuwin32 version seems to work in PowerShell, can't
# really be bothered to find out why.
find := C:\Program Files (x86)\GnuWin32\bin\find.exe


export PATH:=$(arm-toolchain);$(tivaware-toolchain);$(gnuwin32-bin);$(PATH)

target-device := TM4C123GH6PM
startup-code := ./device/$(target-device)
scatter-file := $(startup-code)/scatter.txt
cpu := cortex-m4

## FINDING FILES TO BUILD
# Find all regular source files and create target names pointing to them in the build directory.
build-dir := ./build
source-dir := ./src
sub-source-dirs := $(shell $(find) $(source-dir) -type d) # installed findutils on windows to make this work. There's no "nice" other solution
all-source-dirs := $(source-dir) $(sub-source-dirs) #$(addprefix $(source-dir)/, $(sub-source-dirs))

# Find all regular source files with these extensions, put wildcards in them, and use wildcard command to actually match them existing files
source-file-extensions := c s
all-source-wildcards := $(foreach var,$(source-file-extensions), $(addsuffix /*.$(var),$(all-source-dirs)))
all-source-files := $(wildcard $(all-source-wildcards))

# Find all startup code (located in just one folder preferably)
startup-code-source-files := $(wildcard $(startup-code)/*.s) $(wildcard $(startup-code)/*.c)
startup-code-object-files := $(startup-code-source-files:%=$(build-dir)/%.o)

# Finally generate all the target object file names
all-object-files := $(all-source-files:%=$(build-dir)/%.o) $(startup-code-object-files)

## DEPENDENCY FILES
# Dependency files are needed to make sure make recognizes changes to headers.
# I thought I was being clever with the source-file-extentions above, but now I
# need to filter for c object files, in order to find the dependency files that are
# going to be generated. 

# The next first filters for .c.o files and then turns them to .c.d. files.
all-c-object-files := $(filter %c.o, $(all-object-files))
all-dependency-files := $(all-c-object-files:.c.o=.c.d)

## TARGET FILES
target-image := $(build-dir)/image/$(target-device).axf
target-binary := $(build-dir)/image/$(target-device).bin
target-linking-info := $(build-dir)/image/$(target-device)-linking-info.txt
target-image-info := $(build-dir)/image/$(target-device)-fromelf-info.txt

# Finding the correct mkdir on windows (by using where). This overrides the default mkdir, which is not an executable. 
# Not sure if this works on Unix systems, too. Also not sure how to handle it if it returns multiple values
MKDIR ?= $(shell where mkdir)



# Include paths
inc_custom_cmsis := C:\code\custom-cmsis
inc_arm_core_cmsis := C:\code\custom-cmsis\CMSIS-arm-default\Core\Include
inc_bsp := ./src/bsp
inc_jlib = ./src/jlib
inc_features := ./src/features
inc_runtime_environment := ./src/runtime_environment
inc_os := ./src/jorgOS
INC_PATH := $(inc_bsp) $(inc_jlib) $(inc_features) $(inc_runtime_environment) $(inc_os)
inc_path_string := $(addprefix -I, $(INC_PATH))

# Definitions to pass to compiler
defs = -DJSM_ENABLE

#.PHONY: all
#all: source-files startup-code

all: $(target-binary) $(target-image-info)

.PHONY: flash
flash: $(target-binary)
	lmflash $^ -r -v -i ICDI 
$(target-binary): $(target-image)
	$(MKDIR) -p $(@D)
	fromelf --bin -o $@ $^

$(target-image-info): $(target-image)
	$(MKDIR) -p $(@D)
	fromelf --text -s -o $@ $^

# Link all object files into target image
$(target-image) : $(all-object-files)
	$(MKDIR) -p $(@D)
	armlink --info sizes,stack --map --list $(notdir target-linking-info.txt) --library_type=microlib --scatter=$(scatter-file) $^ -o $@

# Build all c-files
$(build-dir)/%.c.o: %.c
	$(MKDIR) -p $(@D)
	armclang -MMD -c -g -std=c11 -D$(target-device) $(defs) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) $(inc_path_string) -D__MICROLIB  --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

# Include all dependency files 
-include $(all-dependency-files)

# Build all assembly s-files
$(build-dir)/%.s.o: %.s
	$(MKDIR) -p $(@D)
	armclang -masm=auto -c -g -std=c11 -D$(target-device) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) -D:DEF:__MICROLIB --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

#startup-code:$(wildcard $(startup-code)/*.s) $(wildcard $(startup-code)/*.c) 
#	armclang -masm=auto -Wa,armasm,--diag_suppress=A1950W -c -g -D$(target-device) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) --target=arm-arm-none-eabi -mcpu=$(cpu) $?


# Phony target for debugging.
.PHONY: perp
perp:
	where find
	find $(source-dir) -type d
#	@echo $(all-source-wildcards)
	@echo $(sub-source-dirs)
	@echo $(all-source-files)
#	@echo $(all-object-files)

#	@echo $(all-c-object-files)

#	@echo $(all-dependency-files)

# Testing dependency file creation
.PHONY: dep
dep:
	armclang -c -MMD -g -std=c11 -D$(target-device) $(defs) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) $(inc_path_string) -D__MICROLIB  --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft src/bsp/bsp.c -o bsp.test.o



.PHONY: clean
clean:
	rm -r $(build-dir)