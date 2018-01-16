SC Analyzer
===========
This readme contains all information to start using the SC Analyzer.

Prerequirements
===============
Software
--------
The following software must be installed to compile and execute SC Analyzer.
Other versions might work, but are not tested.

Visual Studio Community 2015 (https://www.visualstudio.com/downloads/)

ArrayFire v3.5.1 (https://arrayfire.com/download/)

CUDA Toolkit 8.0 (https://developer.nvidia.com/cuda-80-ga2-download-archive)

SQL Server 2017 Express Edition (https://www.microsoft.com/de-ch/sql-server/sql-server-editions-express)

Environment Variables
---------------------
SC Analyzer relies on environment variables to find external libraries and the Yolo configuration files.
The following environment variables must be set to compile and execute SC Analyzer.

AF_PATH => Path to the ArrayFire directory (for example: "C:\Program Files\ArrayFire\v3\")

CV_PATH => Path to the OpenCV directory (for example "C:\Program Files\opencv\")

CURL_PATH => Path to curl directory (for example "C:\libcurl\")

YOLO_PATH => Path to Yolo directory (for example "C:\yolo\")

NANOBCD_PATH => Path to nanobcd (for example "C:\nanobcd\")

System Path
-----------
The path which contains the required Yolo files (equal to YOLO_PATH environment variable) must be added to the system path.

SQL Server Installation
-----------------------
The MS SQL server must be installed on any system reachable from the system that is executing SC Analyzer.

A database named "sc_analyer" must exist on the database server. To create the appropriate database schema, use the script "create_db_scheme.sql" provided in the sql folder of this project.

In order to reach the SQL server, create a system odbc connection. Configure the odbc connection name and credentials in the project configuration located under dto/Configuration.h.

Execution
=========
After compilation of the solution, you can execute console_initializer.exe. By default, console_initializer will be compiled to accept the following arguments:

```
console_initializer.exe scene, prefix, fps, videoFilePath
```

Scene describes a unique scene id. Each scene id should only be used once.  
Prefix describes a prefix to be used. A prefix currently describes one camera.  
Fps describes the frames per second that should be evaluated by the pipeline.  
VideoFilePath describes the path to the MKV file. 

SC Analyzer supports several input types. To use another type, you will have to modify the main method in the console_initializer project.