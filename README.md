# Build
## Windows

1. Download SFML https://www.sfml-dev.org/download/sfml/2.5.1/
2. Install SFML by placing SFML folder in Program Files
3. Clone repository 
   ```
   $ git clone https://github.com/maciej-wieczorek/ProjektPO.git
   ```
4. Configure and generate the build
   ```
   $ cd ProjektPO
   $ mkdir build
   $ cd build

   Use compiler appropriate to installed SFML version

   Default: 
   $ cmake ..

   Or use different generator ($ cmake --help):
   $ cmake -G "CodeBlocks - MinGW Makefiles" ..
   ```
5. Build
   ```
   $ cmake --build .

   Or open generated project in IDE and build
   ```
   