#Compile the code
# if the file calculate exits then remove it
programname="queue"
hwnum=4

for i in `seq 1 1 3`; do
    if [ -f output${hwnum}${i}.txt ]; then
        rm output${hwnum}${i}.txt
    fi
done

if [ -f ${programname} ]; then
    rm ${programname}
fi
g++ -std=c++11 *.cpp -o ${programname}
# if the file densemult does not exit then exit the test
if [ ! -f ${programname} ]; then
    echo -e "\033[1;91mCompile FAILED.\033[0m"
    exit
fi
# clean
for i in `seq 1 1 3`; do
    if [ -f ${i}.out ]; then
        rm ${i}.out
    fi
    if [ -f ${i}.stderr ]; then
        rm ${i}.stderr
    fi
    if [ -f ${i}.stdcout ]; then
        rm ${i}.stdcout
    fi
done

GREEN='\033[0;32m' 
RED='\033[0;31m'
NC='\033[0m' 

correct=true

# For test case 1
# input 1.txt and output 1.out
timeout -k 2s 2s ./${programname} "input=input${hwnum}1.txt;command=command${hwnum}1.txt;output=output${hwnum}1.txt" 1>1.stdcout 2>1.stderr
# compare 1.out with 1.ans, output the difference to 1.diff
diff -iEBwu ans${hwnum}1.txt output${hwnum}1.txt > 1.diff
# if diff returns nothing, it means you pass the test case (Your ourput file 1.out is correct)
if [ $? -ne 0 ]; then
    # display "test case 1 FAILED" to the screen. The word "FAILED" is highlighted in Red color
    echo -e "Test case 1    ${RED}FAILED.${NC}" 
    correct=false
else
    # display "test case 1 PASSED" to the screen. The word "PASSED" is highlighted in Green color
    echo -e "Test case 1    ${GREEN}PASSED.${NC}"
    # remove the file 1.diff
    rm -f 1.diff
fi
# For test case 2
# this line intentionally use different method to call the program
timeout -k 5s 5s ./${programname} input=input${hwnum}2.txt command=command${hwnum}2.txt output=output${hwnum}2.txt 1>1.stdcout 2>1.stderr
diff -iEBwu ans${hwnum}2.txt output${hwnum}2.txt > 2.diff
if [ $? -ne 0 ]; then
    echo -e "Test case 2    ${RED}FAILED.${NC}"
    correct=false
else
    echo -e "Test case 2    ${GREEN}PASSED.${NC}"
    rm -f 2.diff
fi
# For test case 3
timeout -k 5s 5s ./${programname} "input=input${hwnum}3.txt;command=command${hwnum}3.txt;output=output${hwnum}3.txt" 1>1.stdcout 2>1.stderr
diff -iEBwu ans${hwnum}3.txt output${hwnum}3.txt > 3.diff
if [ $? -ne 0 ]; then
    echo -e "Test case 3    ${RED}FAILED.${NC}"
    correct=false
else
    echo -e "Test case 3    ${GREEN}PASSED.${NC}"
    rm -f 3.diff
fi
