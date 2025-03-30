# Creates the example output
echo 1 > exp.txt

# Runs the program
./calc -3 4 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "Can not compute negative numbers"
    rm out.txt
    rm exp.txt
    exit 1
fi

# Ensures differences *are* found
diff out.txt exp.txt
if [ $? -ne 0 ]; then
    echo "Incorrect Awnser"
    rm out.txt
    rm exp.txt
    exit 1
fi

# Prints a message on success
echo "PASS"

# Cleans up files created
rm out.txt
rm exp.txt
exit 0
