### This Makefile could also serve as a template, so writing down some thoughts in here
### with the tmpl tag before it

## tmpl: whenever I say parts of the template could just be stored somewhere,
## keep in mind I can also just use a sane format to keep it al together.

## tmpl: the toolchain could just be stored in a seperate makefile and included?
## Setting up PATH variable
arm-toolchain := C:\Program Files\ArmCompilerforEmbedded6.22\bin
tivaware-toolchain := C:\ti\TivaWare_C_Series-2.2.0.295\tools\bin
mingw64-bin := C:\msys64\mingw64\bin
# Cannot really be arsed to find out why this version of find works. Gotta stay focused.
find := C:\Program Files (x86)\GnuWin32\bin\find.exe

## tmpl: Can also go in there?
export PATH:= $(mingw64-bin);$(arm-toolchain);$(tivaware-toolchain);$(PATH)

## tmpl: Also...
target-device := TM4C123GH6PM
startup-code := ./device/$(target-device)
scatter-file := $(startup-code)/scatter.txt
cpu := cortex-m4

## tmpl: Here there are some new inputs. The concept of my Testing Framework (TF) is that I just
## complile single files with tests_

# The test suite that must be added to the source folders.
test-tools-source-files = ./jtest

# Exclude main file from the source files
# Because only one entry point is needed, of course
#TMPL: input as it can change per project
# Path should be relative to ./src
main-source-file-to-exlcude := main.c

## Do I need to build everything? Probably not. Let's see how far I get with a dependency files
## FINDING FILES TO BUILD
## FINDING FILES TO BUILD
# Find all regular source files and create target names pointing to them in the
# build directory.

# Change where to build test-files so things don't get messy. Perhaps this is
# unnecessary, but it is good to separate things until I know what I am doing :)
build-dir := ./build-target-test
source-dir := ./src
sub-source-dirs := $(shell $(find) $(source-dir) -type d) # installed findutils on windows to make this work. There's no "nice" other solution
all-source-dirs := $(sub-source-dirs) $(test-tools-source-files)


# Start with the test file (c is assumned). Is a single source file okay?
# Perhaps it matters relative to what path the test file is given? Not sure what
# will we be more convenient. Let's assume the root folder for now
#tmpl:  everywhere "tests_bsp_led" occurs it should be replaced with a
#       variable. preferably this includes the path? Eventually we can end up
#       with all source files just being build once, instead of multiple times
#       for each tests, with binaries and images for each different test.
c-test-source-file = ./tests/tests_bsp_led.c 

# Define the target dependency file for this single source file
deps-test-source-file = $(c-test-source-file:.c=c.d.)

-include deps-test-source-file


# Find all regular source files with these extensions, put wildcards in them, and use wildcard command to actually match them existing files
source-file-extensions := c s
all-source-wildcards := $(foreach var,$(source-file-extensions), $(addsuffix /*.$(var),$(all-source-dirs)))
all-source-files := $(wildcard $(all-source-wildcards)) $(c-test-source-file)




## Ecl
exclude-files = $(source-dir)/$(main-source-file-to-exlcude)
all-source-files := $(filter-out $(exclude-files),$(all-source-files))


# Find all startup code (located in just one folder preferably)
startup-code-source-files := $(wildcard $(startup-code)/*.s) $(wildcard $(startup-code)/*.c)
startup-code-object-files := $(startup-code-source-files:%=$(build-dir)/%.o)

# Finally generate all the target object file names
all-object-files := $(all-source-files:%=$(build-dir)/%.o) $(startup-code-object-files)







### TARGET TESTS
### The route for tests on the host machine

## HOST TARGET FILES
#tmpl: everywhere "tests_bsp_led" occurs it should be replaced with a
#variable.

target-image := $(build-dir)/image/$(target-device)/tests_bsp_led.axf
target-binary := $(build-dir)/image/$(target-device)/tests_bsp_led.bin
target-linking-info := $(build-dir)/image/$(target-device)/tests_bsp_led-linking-info.txt
target-image-info := $(build-dir)/image/$(target-device)/tests_bsp_led-fromelf-info.txt

# Finding the correct mkdir on windows (by using where). This overrides the default mkdir, which is not an executable. 
# Not sure if this works on Unix systems, too. Also not sure how to handle it if it returns multiple values
MKDIR ?= $(shell where mkdir)

# Two? PHONY targets are needed. The one for testing on the host system and the
# one for testing on the board.

# Include paths
inc_custom_cmsis := C:\code\custom-cmsis
inc_arm_core_cmsis := C:\code\custom-cmsis\CMSIS-arm-default\Core\Include
inc_bsp := ./src/bsp
inc_jlib = ./src/jlib
inc_jtest = ./jtest
inc_runtime_environment := ./src/runtime_environment
inc_os := ./src/jorgOS
INC_PATH := $(inc_bsp) $(inc_jlib) $(inc_jtest) $(inc_runtime_environment) $(inc_os) $(inc_custom_cmsis) $(inc_arm_core_cmsis)
inc_path_string := $(addprefix -I, $(INC_PATH))

# Definitions to pass to compiler
common-defs = -DJSM_ENABLE -D$(target-device)

## TARGET TEST
all: $(target-binary) $(target-image-info)

.PHONY target-test:
target-test: $(target-binary)
	lmflash $^ -v -i ICDI 

$(target-binary): defs = $(common-defs) -DJTEST_TARGET
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
	armclang -MMD -c -g -std=c11 -D$(target-device) $(defs) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) $(inc_path_string) -D__MICROLIB  --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

# Build all assembly s-files
$(build-dir)/%.s.o: %.s
	$(MKDIR) -p $(@D)
	armclang -masm=auto -c -g -std=c11 -D$(target-device) --include-directory=$(inc_custom_cmsis) -I$(inc_arm_core_cmsis) -D:DEF:__MICROLIB --target=arm-arm-none-eabi -mcpu=$(cpu) -mfpu=none -mfloat-abi=soft $< -o $@ 

.PHONY derp:
derp:
	@echo $(all-source-dirs)	
	@echo $(sub-source-dirs)
	@echo $(all-object-files)
	@echo $(all-source-files)
	@echo $(exclude-files)
	@echo $(target-image)
	@echo $(target-binary)

.PHONY: clean
clean:
	touch $(build-dir)
	rm -r $(build-dir)
