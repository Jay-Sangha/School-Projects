# Creates the example output
echo 6.5 > exp.txt

# Runs the program
./calc 2.5 4 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "Can not compute negative numbers"
    rm out.txt
    rm exp.txt
    exit 0
fi



# Prints a message on success
echo "PASS"

# Cleans up files created
rm out.txt
rm exp.txt
exit 1
