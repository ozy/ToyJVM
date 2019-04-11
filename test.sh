#!/bin/sh
cd test
rm *.class
rm test/*.out
javac *.java
cd ..

for classFile in test/*.class; do
    echo "Running ${classFile}"
    ./bin/main "${classFile%.class}" > "${classFile}.out"
    
    if diff "${classFile}.txt" "${classFile}.out"; then
        echo "succeeded"
    else
        echo "${classFile} failed"
        exit 1
    fi

done