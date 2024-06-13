## Setting up PATH variable
arm-toolchain := C:\Program Files\ArmCompilerforEmbedded6.22\bin
tivaware-toolchain := C:\ti\TivaWare_C_Series-2.2.0.295\tools\bin
gnuwin32-bin := C:\Program Files (x86)\GnuWin32\bin

export PATH:=$(arm-toolchain);$(tivaware-toolchain);$(gnuwin32-bin);$(PATH)

target-device := TM4C123GH6PM
startup-code := ./device/$(target-device)
scatter-file := $(startup-code)/scatter.txt
cpu := cortex-m4

## FINDING FILES TO BUILD
# Find all regular source files and create target names pointing to them in the build directory.
build-dir := ./build
source-dir := ./src
sub-source-dirs := $(shell find $(source-dir) -type d) # installed findutils on windows to make this work. There's no "nice" other solution
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
inc_runtime_environment := ./src/runtime_environment
inc_os := ./src/jorgOS
INC_PATH := $(inc_bsp) $(inc_runtime_environment) $(inc_os)
inc_path_string := $(addprefix -I, $(INC_PATH))

# Definitions to pass to compiler
defs = -DJSM_ENABLE

#.PHONY: all
#all: source-files startup-code

all: $(target-binary) $(target-image-info)

.PHONY: flash
flash: $(target-binary)
	lmflash $^ -v -i ICDI 

$(target-binary): $(target-image)
	$(MKDIR) -p $(@D)
	fromelf --bin -o $@ $^

$(target-image-info): $(target-image)
	$(MKDIR) -p $(@D)
	fromelf --text -s -o $@ $^

# Link all object files into target image
$(target-image) : $(all-object-files)
	$(MKDIR) -p $(@D)
	armlink --info sizes --map --list $(notdir target-linking-info) --library_type=microlib --scatter=$(scatter-file) $^ -o $@

# Build all c-files
$(build-dir)/%.c.o: %.c
	$(MKDIR) -p $(@D)
	armclang -c -g -std=c11 -D$(target-device) $(defs) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) $(inc_path_string) -D__MICROLIB  --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

# Build all assembly s-files
$(build-dir)/%.s.o: %.s
	$(MKDIR) -p $(@D)
	armclang  -masm=auto -c -g -std=c11 -D$(target-device) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) -D:DEF:__MICROLIB --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

#startup-code:$(wildcard $(startup-code)/*.s) $(wildcard $(startup-code)/*.c) 
#	armclang -masm=auto -Wa,armasm,--diag_suppress=A1950W -c -g -D$(target-device) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) --target=arm-arm-none-eabi -mcpu=$(cpu) $?


# Phony target for debugging.
.PHONY: perp
perp:
	find $(source-dir) -type d
	@echo $(all-source-files)
	@echo $(all-object-files)



.PHONY: clean
clean:
	rm -r $(build-dir)