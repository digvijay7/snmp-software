snmp-software
=============

Software to parse snmp log file, push it to a database and release it using a web API
Test.

SNMP is a popular protocol used in Enterprise Networks to get important information about the network itself. Though old, it can be used to performa number of different tasks keep a check on the health of the network.

This project uses data called using the SNMP protocol in an Enterprise Network, to enable different ubiquitous services for users using the network. Though the software has been primarily made for a wifi netowrk (and has more use cases in it), it can work just as well with SNMP data from ethernet networks.

The title of the project is SNMP "Software". The term "Software" is used instead of program, because the project actually contains a suite of programs to be used in the system.

The project as of yet is not intended nor ready for deployment in *any* Enterprise Network. However it can be a future possibility.

The project is divided into 5 folders:

DB-Alter/ - It primarily contains programs to change/alter data inside the database.
Database Scripts/ - It contains .sql files which can be used to setup a database.
REST/ - This folder contains programs to expose the database using a REST API.
SNMP/ - Contains programs used for reading and parsing SNMP trap dump files. Trasnferring them to a server, which then finally pushes it into the database.
lib/ - Contains files which are required accross different folders.

Features of the Project:
1) Open Source!
2) Use GNU's libmicrohttpd library to setup a REST service.
3) Uses HTTPS to securely transfer data.
4) Stores and uses hashed MAC addresses so that user privacy can be maintained.
5) Provides privacy and security centric web services - without vioalting any user's privacy
6) Users can set their access level according to their preference.
7) Can be used to get information about who was connected at which Access Point, from what time to what time - if the user has given access.
8) Similarly, to get information about usage pattern of Enterprise Networks the Count API can be used.
9) Supports Apps - the app will soon be supporting different aplications, such as live occupancy monitoring of the campus it is deployed in, taking attendance of students, finding last place of use of a wifi device, etc.

TBD: Here is a quickly made, informal list of things which need to be added to the system.
1) Fix security holes
2) Add heartbeat to the system to detect whether system is functioning or not
3) Add/Supplement/Migrate REST APIs using popular web frameworks like Django
4) Git add scripts used to run programs round the clock, on linux machines using cronjobs
