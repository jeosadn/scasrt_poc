#!/bin/env zsh

if [[ `uname` == Darwin ]]; then
    MAX_MEMORY_UNITS=KB
else
    MAX_MEMORY_UNITS=MB
fi

TIMEFMT="%U user| %S system| %P cpu| %*E total| %X KB avg shared (code)| %D KB avg unshared (data/stack)| %K KB total (sum)| %M $MAX_MEMORY_UNITS max memory| %F page faults from disk| %R other page faults|"

declare -a Tests=("asrt" "simple-sensor" "basic-dma")
declare -a Iterations=(
    "1"     "2"     "3"     "4"     "5"     "6"     "7"     "8"     "9"
    "10"    "20"    "30"    "40"    "50"    "60"    "70"    "80"    "90"
    "100"   "200"   "300"   "400"   "500"   "600"   "700"   "800"   "900"
    "1000"  "2000"  "3000"  "4000"  "5000"  "6000"  "7000"  "8000"  "9000"
    "10000"
    "11000" "12000" "13000" "14000" "15000" "16000" "17000" "18000" "19000"
    "20000"
    #"21000" "22000" "23000" "24000" "25000" "26000" "27000" "28000" "29000"
)

for test in ${Tests[@]}; do
    cd "${test}"
    for iter in ${Iterations[@]}; do
        echo "${test}_${iter}"
        make clean 2&>1 >/dev/null
        #echo "${test}_${iter}_clean|" >> ../log
        #{ time ITER=${iter} make sim 1>/dev/null } >> ../log
        #make clean 2&>1 >/dev/null
        #echo "${test}_${iter}_asrt|" >> ../log
        #{ time ITER=${iter} make asrt 1>/dev/null } >> ../log
        #tail -1 sca.log >> ../log
        #make clean 2&>1 >/dev/null
        #echo "${test}_${iter}_asrt-fast|" >> ../fast
        #{ time ITER=${iter} make fast 1>/dev/null } >> ../fast
        #tail -1 sca.log >> ../log
        #make clean 2&>1 >/dev/null
        echo "${test}_${iter}_asrt-bad|" >> ../bad
        { time ITER=${iter} make bad 1>/dev/null } >> ../bad
        tail -1 sca.log >> ../bad
        make clean 2&>1 >/dev/null
    done
    cd ..
done
