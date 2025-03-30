# Creates the example output
echo 513 > exp.txt

# Runs the program
./calc 1 512 > out.txt 

# Ensures exit code is Zero
if [ $? -ne 0 ]; then
    echo "TOO BIG"
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
