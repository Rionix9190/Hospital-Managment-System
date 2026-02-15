A simple C++ console program to manage patients in a hospital. You can add patients, assign rooms, discharge them, search, and save their information. The system uses floors and rooms to organize patients.

Features

Add new patients (ID, Name, Age, Gender, Disease)

Assign floors and rooms to patients

Discharge patients and free rooms

Search patients by ID

Display all patients with their room and floor

Save and load patient data from a file

Automatic memory management for patient records

How to Use

Compile the program

g++ -o hospital_system main.cpp


Run the program

./hospital_system


Menu Options

===== HOSPITAL MANAGEMENT SYSTEM =====
1. Add Patient
2. Assign Floor & Room
3. Discharge Patient
4. Search
5. Display All
6. Save
0. Exit


Choose the option by typing its number.

Rooms are assigned only if they are available.

Data is saved automatically when exiting or using “Save”.

Project Structure

main.cpp – All code is in one file.

Key functions:

addPatient() – Add patient info

assignRoom() – Assign floor and room

dischargePatient() – Remove a patient

searchPatient() – Find a patient by ID

displayAllPatients() – Show all patients

saveToFile() / loadFromFile() – Save/load data

autoGrow() / autoShrink() – Handle memory automatically

Future Improvements

Add a graphical interface

Search by name or disease

Support multiple diseases per patient

Reports like floor occupancy

Use a database instead of text files
