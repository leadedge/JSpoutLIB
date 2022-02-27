# JSpoutLIB
A JNI (Java Native Interface) library for use with the [SpoutProcessing](https://github.com/leadedge/SpoutProcessing) project.

This library is an interface between C++ and Java to create JNI (Java Native Interface) dll files for use with the [SpoutProcessing](https://github.com/leadedge/SpoutProcessing) library project.

The Visual Studio 2017 project produces "JSpoutLIB.dll". 64 bit and 32 bit output files are copied as "JNISpout_64.lib" and "JNISpout_32.lib" respectively to the "Binaries" folder. These should be copied	to the "eclipse-workspace\spout\lib\" folder. Refer to "SETUP.md" in the the SpoutProcessing repository.

The header file "spout.JNISpout.h" is produced by the Eclipse project from definitions in "JNISpout.java". If "JNISpout.java" is ever changed, the modified file must be used.

Spout source files are reproduced in this repository and can be updated as necessary. 
   o The "SpoutGL" folder contains all the source files from the Spout SDK repository "SpoutGL" folder. 
   o The "Spout" folder contains older files that are still used by the Processing library. 

The project also depends on the Java 8 Development toolkit. 
		
1) Download the Java 8 Development Kit 
   https://www.oracle.com/java/technologies/downloads/#java8-windows 
   jdk-8u321-windows-x64.exe]
   
2) Install to\: 
   C:\Program Files\Java\jdk1.8.0_321  
   
3) Confirm the correct additional include directories for the project. 
   C:\Program Files\Java\jdk1.8.0_321\include\win32 
   C:\Program Files\Java\jdk1.8.0_321\include 
