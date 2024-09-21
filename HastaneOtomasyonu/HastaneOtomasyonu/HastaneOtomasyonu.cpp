#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

// New Folder
static void newFail(string mevcutHastaListesi)
{
    ofstream myFile;
    myFile.open(mevcutHastaListesi);
    myFile.close();
}

// Interface
static void head()
{
    cout << "************ 4VAST HASTANE OTOMASYONU *************" << endl; // 1.satir -> 0
    cout << "Use the 'w' and 's' keys to move the cursor up and down, and the 'q' key to make a selection." << endl;//2.satir -> 1
    cout << endl;//3.satir -> 2
}

static void menu()
{
    cout << "    *New patient record" << endl;
    cout << "    *Listing existing patients." << endl;
    cout << "    *Discharge patient." << endl;
    cout << "    *Exet." << endl;
}

// Clear Console
void ConsoleClaer()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Coordinate
static void positionXY(int x, int y)
{
    COORD coordinates{};
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

static int inputControl(string input, int y)
{
    if (input == "s" || input == "S")
    {
        y++;
        if (y > 6) y = 3;
    }
    else if (input == "w" || input == "W")
    {
        y--;
        if (y < 3) y = 6;
    }
    return y;
}

// Patient structure and operations
struct Hasta
{
    string PatientNo;
    string Name;
    string Lastname;
    string Age;
    string BloodType;
    string PhoneNumber;
};

vector<Hasta> hastalar;

static void newAddPationt(string currentPatientList)
{
    string name, lastname, bloodType, phone, age;
    string patientNo = "Patient No : " + to_string(rand() % 1000);

    ofstream myFolder(currentPatientList, ios::app);

    // kulanýcýdan veri alma
    cout << "Patient Name : "; cin >> name; cout << endl;
    cout << "Patient Lastname : "; cin >> lastname; cout << endl;
    cout << "Patient Age : "; cin >> age; cout << endl;
    cout << "Patient Blood Type : "; cin >> bloodType; cout << endl;
    cout << "Patient Phone : "; cin >> phone; cout << endl;

    // veriyi dosyaya yazma
    myFolder << endl << "*************************************" << "\n"
        << patientNo << "\n"
        << "Name : " << name << "\n"
        << "Lastname : " << lastname << "\n"
        << "Age : " << age << "\n"
        << "Blood Type : " << bloodType << "\n"
        << "Phone Number : " << phone << "\n";

    myFolder.close();

    // Hastayý güncelle
    Hasta yeniHasta;
    yeniHasta.PatientNo = patientNo;
    yeniHasta.Name = name;
    yeniHasta.Lastname = lastname;
    yeniHasta.Age = age;
    yeniHasta.BloodType = bloodType;
    yeniHasta.PhoneNumber = phone;

    hastalar.push_back(yeniHasta);
}

void ReadPatientList(string currentPatientList)
{
    ifstream myFaile(currentPatientList);
    string satir;
    Hasta hasta;

    while (getline(myFaile, satir))
    {
        if (satir.find("Patient No : ") != string::npos) {
            hasta.PatientNo = satir.substr(satir.find(":") + 2);
        }
        else if (satir.find("Name : ") != string::npos) {
            hasta.Name = satir.substr(satir.find(":") + 2);
        }
        else if (satir.find("Lastname : ") != string::npos) {
            hasta.Lastname = satir.substr(satir.find(":") + 2);
        }
        else if (satir.find("Age :") != string::npos) {
            hasta.Age = satir.substr(satir.find(":") + 2);
        }
        else if (satir.find("Blood Type :") != string::npos) {
            hasta.BloodType = satir.substr(satir.find(":") + 2);
        }
        else if (satir.find("Phone Number :") != string::npos) {
            hasta.PhoneNumber = satir.substr(satir.find(":") + 2);
            hastalar.push_back(hasta);
        }
    }
    myFaile.close();
}

void PrintPatientList()
{
    for (const auto& hasta : hastalar) {
        cout << "Patient No : " << hasta.PatientNo
            << ", Name : " << hasta.Name
            << ", Lastname : " << hasta.Lastname
            << ", Age : " << hasta.Age
            << ", Blood Type : " << hasta.BloodType
            << ", Phone Number : " << hasta.PhoneNumber << endl;
    }
    cout << "Press the Q key to continue: " << endl;
    _getch();
}

void HastaTaburcuEt(string currentPatientList)
{
    ofstream myFolder(currentPatientList);
    string _patientNo;
    bool hastaVar = false;

    ReadPatientList(currentPatientList);

    cout << "Please enter the patient number to be discharged: "; cin >> _patientNo; cout << endl;

    for (auto it = hastalar.begin(); it != hastalar.end(); ) {
        if (it->PatientNo == _patientNo) {
            it = hastalar.erase(it);
            hastaVar = true;
        }
        else {
            ++it;
        }
    }

    if (!hastaVar)
    {
        cout << "Patient not found" << endl;
    }
    else
    {
        myFolder.is_open();
        for (auto& hasta : hastalar) {
            myFolder << endl << "*************************************" << "\n"
                << hasta.PatientNo << "\n"
                << "Name : " << hasta.Name << "\n"
                << "Lastname : " << hasta.Lastname << "\n"
                << "Age : " << hasta.Age << "\n"
                << "Blood Type : " << hasta.BloodType << "\n"
                << "Phone Number : " << hasta.PhoneNumber << "\n";
        }
        cout << "Patient has been successfully discharged." << endl;
        cout << "Press the Q key to continue: " << endl;
        _getch();
    }

    myFolder.close();
}

static void secim(int y, string currentPatientList)
{
    if (y == 3) {
        newAddPationt(currentPatientList);
        ReadPatientList(currentPatientList);
    }
    if (y == 4) {
        PrintPatientList();
    }
    if (y == 5) {
        HastaTaburcuEt(currentPatientList);
    }
}

int main()
{
    string currentPatientList = "currentPatientList.txt";
    ReadPatientList(currentPatientList);

    string input;
    int y = 2, x = 3;

    head();
    menu();

    while (true)
    {
        input = _getch();
        y = inputControl(input, y);
        positionXY(x, y);

        if (input == "q" || input == "Q")
        {
            if (y < 6)
            {
                ConsoleClaer();
                secim(y, currentPatientList);
                ConsoleClaer();
                head();
                menu();
            }
            else if (y == 6)
            {
                break;
            }
        }
    }
}
