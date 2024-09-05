cmake -S ../RayTracing_in_One_Weekend/ -B ../RayTracing_in_One_Weekend/build
make -C build/
./build/main > images/image.ppm
open images/image.ppm