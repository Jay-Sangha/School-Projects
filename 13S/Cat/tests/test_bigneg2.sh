# Creates the example output
echo 20 > exp.txt

# Runs the program
./calc 500 -520 > out.txt 

# Ensures exit code is Zero
if [ $? -eq 0 ]; then
    echo "TOO BIG"
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
