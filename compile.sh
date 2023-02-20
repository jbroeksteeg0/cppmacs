set -e

cd build
ninja
cp compile_commands.json ../
cp cppmacs ../

cd ../
printf "\nSuccessfully Compiled!\n"
