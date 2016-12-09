# For program to work correctly setenv III_BASE to 
# the directory where all iii stuff is.
#
# Remark: if III is running on lvrhost remove 'rsh lvrhost' and 
# quotes from lvrShow to improve animation performance

# The machine that has wire to lvr
lvrhost=sloth

# The machine that is used to display cd image 
disphost=`env | grep DISPLAY | cut -f2 -d= | cut -f1 -d:`

# The ultrix machine to decompress 
ultrixhost=stupid

# The machine that has a cd player for Vol 4
cdhost4=abulia

# The machine that has a cd player for Vol 5
cdhost5=pestilence

# The directory where all the iii stuff is.
iii_base=/INSANE/usr/u1/audris/planets




#HP CC FLAGS
flags_c = -c  -O -I/usr/include/X11R5
link_f =  -L/usr/lib/X11R5 -lXaw -lXmu -lXt -lXext -lX11 -lm 
CC = cc

# gcc flags and compiler
#flags_c = -c -O -I/usr/local/include
#link_f =  -L/usr/local/lib -lXaw -lXmu -lXt -lXext -lX11 -lm 
#CC = gcc


OBJECT = images.o graph.o layout.o order.o \
action.o menu.o help.o


.SUFFIXES:
.SUFFIXES: .C .def .h  .cc .c .o 
.c.o: ; $(CC)  $(flags_c) $*.c

all : iii.bin #lvrShow cd lvrShowBin

images.o : iii.h 
graph.o : iii.h action.h layout.h menu.h order.h 
action.o: iii.h action.h layout.h
menu.o :  iii.h menu.h order.h action.h
order.o : iii.h order.h
help.o : iii.h

iii.bin :  $(OBJECT)
	$(CC)  $(OBJECT) $(link_f)  -o iii.bin 

#lvrShowBin : lvrShow.o
#	rsh $(lvrhost) "cd $(iii_base); $(CC)  lvrShow.o  -o lvrShowBin"

lvrShow: lvrShow.templ
	sed 's"lvrhost"$(lvrhost)"' lvrShow.templ | \
	sed 's"iii_base"$(iii_base)"g' > lvrShow
	chmod +x lvrShow

#cd : decomp d1 cdShow cdGet
#	rsh $(ultrixhost) "cd $(iii_base); cc  decomp.c  -o decomp"
#	$(CC)  d1.c  -o d1

cdShow	: cdShow.templ
	sed 's"cdhost4"$(cdhost4)"' cdShow.templ | \
	sed 's"cdhost5"$(cdhost5)"' |  \
	sed 's"ultrixhost"$(ultrixhost)"' |  \
	sed 's"disphost"$(disphost)"' |  \
	sed 's"iii_base"$(iii_base)"g' \
	> cdShow 
	chmod +x cdShow

cdGet	: cdGet.templ
	sed 's"cdhost4"$(cdhost4)"' cdGet.templ | \
	sed 's"cdhost5"$(cdhost5)"' |  \
	sed 's"ultrixhost"$(ultrixhost)"' |  \
	sed 's"disphost"$(disphost)"' |  \
	sed 's"iii_base"$(iii_base)"g' \
	> cdGet
	chmod +x cdGet


clean:
	rm -f iii.bin lvrShow *.o cdShow cdGet	d1 decomp





