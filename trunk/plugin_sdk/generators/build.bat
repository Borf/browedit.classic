..\mingw\bin\g++ ../mymath.cpp *.cpp -I.. -I../base -DPLUGIN_EXPORTS -D_USRDLL -L.. -lplugin_base -shared -o generators.dll
@pause