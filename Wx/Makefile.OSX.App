#

G = -g
#VFRDIR=/usr/local/vfr
VFRDIR=/Users/yoh/Works/Vtools/vfr
TMPLDIR=$(VFRDIR)/osxAppTmpl
OPTNAME=Wx

CXX = `wx-config --cxx`
INC = -I. -I$(VFRDIR)/sg -I$(VFRDIR)/$(OPTNAME) -I$(VFRDIR)/util
CFLAGS = $(INC) $(G)
CXXFLAGS = $(CFLAGS) `wx-config --cxxflags base,core,gl`
LDLIBS = `wx-config --libs base,core,gl`
LDFLAGS = $(CFLAGS)
INSTALL = /usr/bin/install -c

VFR_DLIB = -L$(VFRDIR)/sg -lvfr -L$(VFRDIR)/$(OPTNAME) -lvfr$(OPTNAME)
VFR_SLIB = $(VFRDIR)/sg/libvfr.a $(VFRDIR)/$(OPTNAME)/libvfr$(OPTNAME).a

PROGRAM = Test
OBJS = TestApp.o
VERSION = 1.0.0
COPYRIGHT = 2015 FEAST LTD

BUNDLE = $(PROGRAM).app/Contents

all : bundle

#
# Targets to build a Mac OS X application bundle
#
bundle: $(BUNDLE)/MacOS/$(PROGRAM) \
	$(BUNDLE)/Info.plist \
	$(BUNDLE)/PkgInfo \
	$(BUNDLE)/Resources/$(PROGRAM).rsrc \
	$(BUNDLE)/Resources/$(PROGRAM).icns

$(BUNDLE)/Info.plist: $(TMPLDIR)/Info.plist.in 
	@$(INSTALL) -d `dirname $@`
	sed -e "s/IDENTIFIER/`echo $(program_dir) | sed 's,/,.,g'`/" \
	    -e "s/EXECUTABLE/$(PROGRAM)/" \
	    -e "s/ICONFILE/$(PROGRAM)\.icns/" \
	    -e "s/COPYRIGHT/$(COPYRIGHT)/" \
	    -e "s/VERSION/$(VERSION)/" $< > $@

$(BUNDLE)/PkgInfo:
	@$(INSTALL) -d `dirname $@`
	echo -n "APPL????" > $@

$(BUNDLE)/MacOS/$(PROGRAM): $(PROGRAM)
	@$(INSTALL) -d `dirname $@`
	cp $< $@

$(BUNDLE)/Resources/$(PROGRAM).rsrc: $(TMPLDIR)/tmpl.rsrc
	@$(INSTALL) -d `dirname $@`
	cp $< $@

$(BUNDLE)/Resources/$(PROGRAM).icns: $(TMPLDIR)/tmpl.icns
	@$(INSTALL) -d `dirname $@`
	cp $< $@


#$(PROGRAM) : $(OBJS) 
#	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(VFR_DLIB) $(LDLIBS)


$(PROGRAM) : $(OBJS) 
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(VFR_SLIB) $(LDLIBS)

clean :
	$(RM) *.o *~ a.out core

distclean :
	$(RM) -r $(PROGRAM).app $(PROGRAM)

depend:
	$(DEPEND) vfr*.[hC]

# DO NOT DELETE
