
# Runs the program
./calc 4 'a' > out.txt 

# Ensures exit code is Zero
if [ $? -eq 0 ]; then
    echo "BAD INPUT"
    rm out.txt
    exit 1
fi




# Prints a message on success
echo "PASS"

# Cleans up files created
rm out.txt
exit 0
