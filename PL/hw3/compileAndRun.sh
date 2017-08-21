#/bin/sh
if [ ! -d "bin" ]; then
	mkdir bin
fi
javac -sourcepath src -classpath bin -d bin src/Application.java 
java -classpath bin Application
