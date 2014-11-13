CPP=g++ -g -fPIC
RM=rm -f
all:
	$(CPP) -c simulator.cc -o simulator.o
	$(CPP) -c event.cc -o event.o
	$(CPP) -c addr.cc -o addr.o
	$(CPP) -c packet.cc -o packet.o
	$(CPP) -c node.cc -o node.o
	$(CPP) -c link.cc -o link.o
	$(CPP) -c host.cc -o host.o
	$(CPP) -c router.cc -o router.o
	$(CPP) -c cache.cc -o cache.o
	$(CPP) -c itr.cc -o itr.o
	$(CPP) -c etr.cc -o etr.o
	$(CPP) -c xtr.cc -o xtr.o
	$(CPP) -c mapdb.cc -o mapdb.o
	$(CPP) -c logger.cc -o logger.o
	$(CPP) -c lisp-sim.cc -o lisp-sim.o
	#$(CPP) simulator.o event.o addr.o packet.o node.o link.o host.o router.o itr.o mapdb.o logger.o lisp-sim.o -o lisp-sim
	swig -python -c++ lisp-sim.i
	$(CPP) -c lisp-sim_wrap.cxx `pkg-config --cflags --libs python2`
	$(CPP) -shared simulator.o event.o addr.o packet.o node.o link.o host.o router.o cache.o itr.o etr.o xtr.o mapdb.o logger.o lisp-sim.o lisp-sim_wrap.o -o _lispsim.so
clean:
	$(RM) *.o *.so *.cxx lispsim.py lispsim.pyc
	$(RM) *.nam
	$(RM) -r __pycache__
