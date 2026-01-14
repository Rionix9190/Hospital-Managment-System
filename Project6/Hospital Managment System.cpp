#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

// ---------------- FUNCTION PROTOTYPES ----------------
void displayMenu();
void addPatient(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity);
void autoGrow(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& capacity, int size);
void displayAllPatients(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size);
void searchPatient(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size);
void assignRoom(int* id, int* floor, int* room, int size, int** rooms, int floors, int roomsPerFloor);
void dischargePatient(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity, int** rooms);
void autoShrink(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& capacity, int size);
void saveToFile(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size);
void loadFromFile(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity, int** rooms, int floors, int roomsPerFloor);
int** initializeRooms(int floors, int roomsPerFloor);
void freeRooms(int** rooms, int floors);

// ---------------- MAIN ----------------
int main() {
    int size = 0, capacity = 2;
    int* id = new int[capacity];
    int* age = new int[capacity];
    char* gender = new char[capacity];
    int* floor = new int[capacity]; // Changed from ward
    int* room = new int[capacity];
    char** name = new char* [capacity];
    char** disease = new char* [capacity];

    int totalFloors = 3, roomsPerFloor = 4;
    int** rooms = initializeRooms(totalFloors, roomsPerFloor);

    loadFromFile(id, age, gender, name, disease, floor, room, size, capacity, rooms, totalFloors, roomsPerFloor);

    int choice;
    do {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (choice) {
        case 1: addPatient(id, age, gender, name, disease, floor, room, size, capacity); break;
        case 2: assignRoom(id, floor, room, size, rooms, totalFloors, roomsPerFloor); break;
        case 3: dischargePatient(id, age, gender, name, disease, floor, room, size, capacity, rooms); break;
        case 4: searchPatient(id, age, gender, name, disease, floor, room, size); break;
        case 5: displayAllPatients(id, age, gender, name, disease, floor, room, size); break;
        case 6: saveToFile(id, age, gender, name, disease, floor, room, size); break;
        case 0: saveToFile(id, age, gender, name, disease, floor, room, size); break;
        default: cout << "Invalid Choice!\n";
        }
    } while (choice != 0);

    for (int i = 0; i < size; i++) {
        delete[] name[i];
        delete[] disease[i];
    }
    delete[] id; delete[] age; delete[] gender;
    delete[] floor; delete[] room;
    delete[] name; delete[] disease;
    freeRooms(rooms, totalFloors);

    return 0;
}

// ---------------- FUNCTIONS ----------------

void displayMenu() {
    cout << "\n===== HOSPITAL SYSTEM (FLOOR-BASED) =====\n";
    cout << "1. Add Patient\n2. Assign Floor & Room\n3. Discharge Patient\n4. Search\n5. Display All\n6. Save\n0. Exit\nChoice: ";
}

void autoGrow(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& capacity, int size) {
    int newCap = capacity * 2;
    if (newCap <= size) newCap = size + 1;

    int* nid = new int[newCap];
    int* nage = new int[newCap];
    char* ng = new char[newCap];
    int* nf = new int[newCap];
    int* nr = new int[newCap];
    char** nn = new char* [newCap];
    char** nd = new char* [newCap];

    for (int i = 0; i < size; i++) {
        nid[i] = id[i]; nage[i] = age[i]; ng[i] = gender[i];
        nf[i] = floor[i]; nr[i] = room[i];
        nn[i] = name[i]; nd[i] = disease[i];
    }

    delete[] id; delete[] age; delete[] gender;
    delete[] floor; delete[] room;
    delete[] name; delete[] disease;

    id = nid; age = nage; gender = ng; floor = nf; room = nr; name = nn; disease = nd;
    capacity = newCap;
}

void addPatient(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity) {
    if (size >= capacity) autoGrow(id, age, gender, name, disease, floor, room, capacity, size);

    cout << "ID: "; cin >> id[size];
    cout << "Age: "; cin >> age[size];
    cout << "Gender (M/F): "; cin >> gender[size];
    cin.ignore(1000, '\n');

    char temp[100];
    cout << "Name: "; cin.getline(temp, 100);
    name[size] = new char[strlen(temp) + 1]; strcpy(name[size], temp);
    cout << "Disease: "; cin.getline(temp, 100);
    disease[size] = new char[strlen(temp) + 1]; strcpy(disease[size], temp);

    floor[size] = -1; room[size] = -1;
    size++;
}

void assignRoom(int* id, int* floor, int* room, int size, int** rooms, int totalFloors, int roomsPerFloor) {
    int pid;
    cout << "Enter Patient ID: "; cin >> pid;
    for (int i = 0; i < size; i++) {
        if (id[i] == pid) {
            cout << "Available Rooms by Floor: \n";
            for (int f = 0; f < totalFloors; f++) {
                cout << "Floor " << f << ": ";
                for (int r = 0; r < roomsPerFloor; r++)
                    if (rooms[f][r] == 0) cout << r << " ";
                cout << endl;
            }
            int f_choice, r_choice;
            cout << "Enter Floor & Room: "; cin >> f_choice >> r_choice;
            if (f_choice < 0 || f_choice >= totalFloors || r_choice < 0 || r_choice >= roomsPerFloor || rooms[f_choice][r_choice] == 1) {
                cout << "Invalid Selection or Room Occupied!\n"; return;
            }
            if (floor[i] != -1) rooms[floor[i]][room[i]] = 0;
            floor[i] = f_choice; room[i] = r_choice; rooms[f_choice][r_choice] = 1;
            cout << "Assigned Successfully.\n"; return;
        }
    }
    cout << "Patient ID Not Found.\n";
}

void dischargePatient(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity, int** rooms) {
    int pid;
    cout << "Enter ID: "; cin >> pid;
    for (int i = 0; i < size; i++) {
        if (id[i] == pid) {
            if (floor[i] != -1 && room[i] != -1) rooms[floor[i]][room[i]] = 0;
            delete[] name[i]; delete[] disease[i];
            for (int j = i; j < size - 1; j++) {
                id[j] = id[j + 1]; age[j] = age[j + 1]; gender[j] = gender[j + 1];
                floor[j] = floor[j + 1]; room[j] = room[j + 1];
                name[j] = name[j + 1]; disease[j] = disease[j + 1];
            }
            size--;
            autoShrink(id, age, gender, name, disease, floor, room, capacity, size);
            cout << "Patient Discharged.\n"; return;
        }
    }
}

void autoShrink(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& capacity, int size) {
    if (capacity > 2 && size <= capacity / 4) {
        int newCap = capacity / 2;
        int* nid = new int[newCap]; int* nage = new int[newCap]; char* ng = new char[newCap];
        int* nf = new int[newCap]; int* nr = new int[newCap];
        char** nn = new char* [newCap]; char** nd = new char* [newCap];

        for (int i = 0; i < size; i++) {
            nid[i] = id[i]; nage[i] = age[i]; ng[i] = gender[i];
            nf[i] = floor[i]; nr[i] = room[i];
            nn[i] = name[i]; nd[i] = disease[i];
        }
        delete[] id; delete[] age; delete[] gender; delete[] floor; delete[] room; delete[] name; delete[] disease;
        id = nid; age = nage; gender = ng; floor = nf; room = nr; name = nn; disease = nd;
        capacity = newCap;
    }
}

void displayAllPatients(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size) {
    for (int i = 0; i < size; i++)
        cout << "ID: " << id[i] << " | Name: " << name[i] << " | Floor: " << floor[i] << " Room: " << room[i] << endl;
}

void searchPatient(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size) {
    int pid; cout << "Search ID: "; cin >> pid;
    for (int i = 0; i < size; i++)
        if (id[i] == pid) { cout << "Found: " << name[i] << " on Floor: " << floor[i] << endl; return; }
    cout << "Not Found.\n";
}

void saveToFile(int* id, int* age, char* gender, char** name, char** disease, int* floor, int* room, int size) {
    ofstream f("patients.txt");
    if (!f) return;
    f << size << endl;
    for (int i = 0; i < size; i++)
        f << id[i] << "\n" << age[i] << "\n" << gender[i] << "\n" << name[i] << "\n" << disease[i] << "\n" << floor[i] << " " << room[i] << endl;
    f.close();
}

void loadFromFile(int*& id, int*& age, char*& gender, char**& name, char**& disease, int*& floor, int*& room, int& size, int& capacity, int** rooms, int totalFloors, int roomsPerFloor) {
    ifstream fin("patients.txt");
    if (!fin) return;
    int fSize; fin >> fSize;
    while (capacity < fSize) autoGrow(id, age, gender, name, disease, floor, room, capacity, size);

    char temp[100];
    for (int i = 0; i < fSize; i++) {
        fin >> id[i] >> age[i] >> gender[i]; fin.ignore();
        fin.getline(temp, 100); name[i] = new char[strlen(temp) + 1]; strcpy(name[i], temp);
        fin.getline(temp, 100); disease[i] = new char[strlen(temp) + 1]; strcpy(disease[i], temp);
        fin >> floor[i] >> room[i];
        if (floor[i] >= 0 && floor[i] < totalFloors && room[i] >= 0 && room[i] < roomsPerFloor)
            rooms[floor[i]][room[i]] = 1;
        size++;
    }
    fin.close();
}

int** initializeRooms(int floors, int roomsPerFloor) {
    int** rooms = new int* [floors];
    for (int i = 0; i < floors; i++) {
        rooms[i] = new int[roomsPerFloor];
        for (int j = 0; j < roomsPerFloor; j++) rooms[i][j] = 0;
    }
    return rooms;
}

void freeRooms(int** rooms, int floors) {
    for (int i = 0; i < floors; i++) delete[] rooms[i];
    delete[] rooms;
}