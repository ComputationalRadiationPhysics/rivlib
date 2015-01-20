
Remote Interactive Visualization
Copyright TUD 2013-2014, VICCI (http://www.vicci.eu/)
Alle Rechte vorbehalten.
All rights reserved.
cf. license.txt

http://www.vicci.eu/projektergebnisse/projektergebnisse-ap4-interaktive-echtzeitvisualisierung-und-analyse/?lang=en

Setup:
Linux:
	Use CMake!
	> mkdir build
	> cd build
	> cmake ..
	
Windows:
	Use Visual Studio 2012
	1. Copy "extlibs - *.props" to "extlibs.props" and edit corresponding paths

Projects:
  rivlib
    Library for RIV providers (and more)
  tests/c++sensor
    VICCI SeMiWa Sensore written in c++ with JNI
	Prototype for rivlib/vicci_middleware_broker
  tests/rivclnttest
    Client Test Application (c++)
  tests/rivprovtest
    Provider Test Application (c++)

Future extensions:
  "offers": published via 'brokers' in addition to 'providers', 'offers'
            define possible providers, i.e. the possibility of a visualization
			server to instantiate new providers on demand for specific tasks.
