TEMPLATE = subdirs

SUBDIRS += libminijava \
    minijava_compiler \
    minijava_unittest

libminijava.subdir = libminijava
minijava_compiler.subdir = minijava-compiler
minijava_compiler.depends = libminijava
minijava_unittest.subdir = minijava-unittest
minijava_unittest.depends = libminijava
