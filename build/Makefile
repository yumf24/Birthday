.PHONY: all clean

ROOT := $(shell cd .. && pwd) # 获取根目录
ROOT := $(strip $(ROOT))
export ROOT # 将子make能够使用ROOT变量

# 要运行make的子目录
# 需要注意的时，可执行文件依赖于前面两个
DIRS = $(ROOT)/source/birthclass/src \
	   $(ROOT)/source/myTime/src \
	   $(ROOT)/source/main/src

RM = rm
RMFLAGS = -rf

RMS = $(ROOT)/build/libs $(ROOT)/build/exes # 当前Makefile clean的时候要删除的目录


# 因为shell的问题，for循环还是写成一行算了
all: 
	@set -e; for dir in $(DIRS); do cd $$dir && $(MAKE); done
	@echo ""
	@echo ":) make completed"
	@echo ""

clean:
	@set -e; for dir in $(DIRS); do cd $$dir && $(MAKE) clean; done
	$(RM) $(RMFLAGS) $(RMS)
	@echo ""
	@echo ":) clean completed"
	@echo ""

