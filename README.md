#erasus

file eraser

modes:British HMG IS5 (1 pass) | US Airforce 5020 (3 passes) | German VSITR (7 passes)

compile:
g++ *.cpp -std=c++11 -o erasus -no-pie -Wall \`pkg-config gtkmm-3.0 --cflags --libs\`

![alt tag](https://github.com/bitspalter/erasus/blob/master/erasus.png "erasus")
