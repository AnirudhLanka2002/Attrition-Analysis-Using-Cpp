#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <conio.h>

class Employee {
public:
    int id;
    int age;
    char gender;
    std::string department;
    int yearsOfExperience;
    int monthOfLeave;
    double salary;
    bool attrition;

    Employee(int id, int age, char gender, const std::string& department,
             int yearsOfExperience, int monthOfLeave, double salary, bool attrition)
        : id(id), age(age), gender(gender), department(department),
          yearsOfExperience(yearsOfExperience), monthOfLeave(monthOfLeave),
          salary(salary), attrition(attrition) {}

    // Static data members to count attritions
    static int totalEmployees;
    static int totalAttrition;

    // Static member function to calculate attrition rate
    static double getAttritionRate() {
        return (totalEmployees > 0) ? (double(totalAttrition) / totalEmployees) * 100 : 0.0;
    }
};

// Initialize static data members
int Employee::totalEmployees = 0;
int Employee::totalAttrition = 0;

class Department {
public:
    int totalEmployees;
    int totalAttrition;

    Department() : totalEmployees(0), totalAttrition(0) {}

    double getAttritionRate() const {
        return (totalEmployees > 0) ? (double(totalAttrition) / totalEmployees) * 100 : 0.0;
    }
};

class HRDepartment : public Department {
public:
    int totalFemaleEmployees;
    int totalFemaleAttrition;

    HRDepartment() : totalFemaleEmployees(0), totalFemaleAttrition(0) {}

    double getFemaleAttritionRate() const {
        return (totalFemaleEmployees > 0) ? (double(totalFemaleAttrition) / totalFemaleEmployees) * 100 : 0.0;
    }
};

class FinanceDepartment : public Department {
public:
    int totalMaleEmployees;
    int totalMaleAttrition;

    FinanceDepartment() : totalMaleEmployees(0), totalMaleAttrition(0) {}

    double getMaleAttritionRate() const {
        return (totalMaleEmployees > 0) ? (double(totalMaleAttrition) / totalMaleEmployees) * 100 : 0.0;
    }
};

class AttritionAnalysis {
private:
    double overallAttritionRate;
    double avgExperience;
    std::map<std::string, Department> departmentData;
    std::map<int, Department> experienceData;
    std::map<int, Department> monthOfLeaveData;
    std::map<char, Department> genderData;
    std::map<int, Department> ageRangeData;
    HRDepartment hrDept;
    FinanceDepartment financeDept;

public:
    AttritionAnalysis() : overallAttritionRate(0), avgExperience(0) {}

    void calculate(const std::vector<Employee>& employees) {
        Employee::totalEmployees = employees.size();

        for (const auto& employee : employees) {
            if (employee.attrition) {
                Employee::totalAttrition++;
                monthOfLeaveData[employee.monthOfLeave].totalAttrition++;
            }

            avgExperience += employee.yearsOfExperience;
            departmentData[employee.department].totalEmployees++;
            experienceData[(employee.yearsOfExperience - 1) / 3 * 3].totalEmployees++;
            genderData[employee.gender].totalEmployees++;
            ageRangeData[(employee.age / 5) * 5].totalEmployees++;

            if (employee.attrition) {
                departmentData[employee.department].totalAttrition++;
                experienceData[(employee.yearsOfExperience - 1) / 3 * 3].totalAttrition++;
                genderData[employee.gender].totalAttrition++;
                ageRangeData[(employee.age / 5) * 5].totalAttrition++;

                // Additional HR Department specific calculations
                if (employee.department == "HR") {
                    hrDept.totalFemaleEmployees += (employee.gender == 'F');
                    hrDept.totalFemaleAttrition += (employee.gender == 'F' && employee.attrition);
                }

                // Additional Finance Department specific calculations
                if (employee.department == "Finance") {
                    financeDept.totalMaleEmployees += (employee.gender == 'M');
                    financeDept.totalMaleAttrition += (employee.gender == 'M' && employee.attrition);
                }
            }
        }

        overallAttritionRate = (Employee::totalAttrition / double(Employee::totalEmployees)) * 100;
        avgExperience /= double(Employee::totalEmployees);
    }

    void displayOverall() const {
        std::cout << "Overall Attrition Analysis\n";
        std::cout << "-------------------------\n";
        std::cout << "Total Employees: " << Employee::totalEmployees << "\n";
        std::cout << "Total Attrition: " << Employee::totalAttrition << "\n";
        std::cout << "Overall Attrition Rate: " << overallAttritionRate << "%\n";
        std::cout << "Average Years of Experience: " << avgExperience << "\n";
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }


    void displayByDepartment() const {
        std::cout << "\nAttrition Analysis by Department\n";
        std::cout << "-------------------------------\n";
        for (const auto& pair : departmentData) {
            const std::string& department = pair.first;
            const Department& deptData = pair.second;
            double deptAttritionRate = deptData.getAttritionRate();

            std::cout << "Department: " << department << "\n";
            std::cout << "Total Employees: " << deptData.totalEmployees << "\n";
            std::cout << "Total Attrition: " << deptData.totalAttrition << "\n";
            std::cout << "Attrition Rate: " << deptAttritionRate << "%\n";
            std::cout << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    void displayByExperience() const {
        std::cout << "\nAttrition Analysis by Years of Experience Range\n";
        std::cout << "--------------------------------------------\n";
        for (const auto& pair : experienceData) {
            int experienceRange = pair.first;
            int rangeStart = experienceRange;
            int rangeEnd = experienceRange + 2;

            const Department& expData = pair.second;
            double attritionRateWithExp = expData.getAttritionRate();

            std::cout << "Years of Experience Range: " << rangeStart << " to " << rangeEnd << "\n";
            std::cout << "Total Employees: " << expData.totalEmployees << "\n";
            std::cout << "Total Attrition: " << expData.totalAttrition << "\n";
            std::cout << "Attrition Rate: " << attritionRateWithExp << "%\n";
            std::cout << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    void displayByMonthOfLeave() const {
        std::cout << "\nAttrition Analysis by Month of Leave\n";
        std::cout << "----------------------------------\n";
        for (const auto& pair : monthOfLeaveData) {
            int monthOfLeave = pair.first;
            const Department& monthData = pair.second;
            int totalAttritionForMonth = monthData.totalAttrition;
            int totalAttrition = Employee::totalAttrition;

            double attritionRateWithMonth = (totalAttrition > 0) ? (double(totalAttritionForMonth) / totalAttrition) * 100 : 0.0;

            std::cout << "Month of Leave: " << (monthOfLeave != -1 ? std::to_string(monthOfLeave) : "Not Left") << "\n";
            std::cout << "Total Employees: " << monthData.totalEmployees << "\n";
            std::cout << "Total Attrition: " << totalAttritionForMonth << "\n";
            std::cout << "Attrition Rate: " << attritionRateWithMonth << "%\n";
            std::cout << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    void displayByGender() const {
        std::cout << "\nAttrition Analysis by Gender\n";
        std::cout << "----------------------------\n";
        for (const auto& pair : genderData) {
            char gender = pair.first;
            const Department& genderDeptData = pair.second;
            double genderAttritionRate = genderDeptData.getAttritionRate();

            std::cout << "Gender: " << gender << "\n";
            std::cout << "Total Employees: " << genderDeptData.totalEmployees << "\n";
            std::cout << "Total Attrition: " << genderDeptData.totalAttrition << "\n";
            std::cout << "Attrition Rate: " << genderAttritionRate << "%\n";
            std::cout << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    void displayByAgeRange() const {
        std::cout << "\nAttrition Analysis by Age Range (5 years)\n";
        std::cout << "---------------------------------------\n";
        for (const auto& pair : ageRangeData) {
            int ageRange = pair.first;
            int rangeStart = ageRange;
            int rangeEnd = ageRange + 4;

            const Department& ageRangeDeptData = pair.second;
            double ageRangeAttritionRate = ageRangeDeptData.getAttritionRate();

            std::cout << "Age Range: " << rangeStart << " to " << rangeEnd << "\n";
            std::cout << "Total Employees: " << ageRangeDeptData.totalEmployees << "\n";
            std::cout << "Total Attrition: " << ageRangeDeptData.totalAttrition << "\n";
            std::cout << "Attrition Rate: " << ageRangeAttritionRate << "%\n";
            std::cout << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    // Friend function to access private members of EmployeeDatabase
    friend class EmployeeDatabase;
};

class EmployeeDatabase {
private:
    std::vector<Employee> employees;

public:
    EmployeeDatabase() {
        // std::cout<<"hrllo"<<std::endl;
        const char* env_p = std::getenv("ResourceFile");
        if(env_p == NULL){
            std::cout<< "Accessing Local File..." << std::endl;
            std::string filename = "DataSet.csv";
            loadFromCSV(filename);
        }
        else{
            // std::cout << "Your PATH is: " << env_p << std::endl<<std::endl;
            std::string filename = env_p;
            filename = filename + "\\DataSet.csv";
            std::cout << "Your PATH is: " << filename << std::endl<<std::endl;
            // std::cout << filename << std::endl << std::endl;
            loadFromCSV(filename);
        }
        
    }

    void loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening the file: " << filename << std::endl;
            return;
        }

        employees.clear(); // Clear existing data

        std::string line;
        // Skip the header line
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            int id, age, yearsOfExperience, monthOfLeave;
            char gender;
            std::string department;
            double salary;
            bool attrition;

            std::getline(iss, token, ',');
            id = std::stoi(token);

            std::getline(iss, token, ',');
            age = std::stoi(token);

            std::getline(iss, token, ',');
            gender = token[0];

            std::getline(iss, department, ',');

            std::getline(iss, token, ',');
            yearsOfExperience = std::stoi(token);

            std::getline(iss, token, ',');
            monthOfLeave = std::stoi(token);

            std::getline(iss, token, ',');
            salary = std::stod(token);

            std::getline(iss, token, ',');
            attrition = (std::stoi(token) == 1);

            employees.emplace_back(id, age, gender, department, yearsOfExperience, monthOfLeave, salary, attrition);
        }

        file.close();
    }

    void showEmployeeData() const {
        // system("cls");---------------------------------
        if (employees.empty()) {
            std::cout << "No employee data to display.\n";
            return;
        }

        std::cout << "Employee Data Table\n";
        std::cout << "-------------------\n";
        std::cout << std::left << std::setw(15) << "Employee ID"
                  << std::setw(5) << "Age"
                  << std::setw(6) << "Gender"
                  << std::setw(12) << "Department"
                  << std::setw(20) << "Years of Experience"
                  << std::setw(15) << "Month of Leave"
                  << std::setw(10) << "Salary"
                  << std::setw(10) << "Attrition\n";

        for (const auto& employee : employees) {
            std::cout << std::left << std::setw(15) << employee.id
                      << std::setw(5) << employee.age
                      << std::setw(6) << employee.gender
                      << std::setw(12) << employee.department
                      << std::setw(20) << employee.yearsOfExperience
                      << std::setw(15) << (employee.attrition ? std::to_string(employee.monthOfLeave) : "Not Left")
                      << std::setw(10) << employee.salary
                      << std::setw(10) << employee.attrition << "\n";
        }
        std::cout<< "press any key to continue..." << std::endl;
        _getch();
    }

    void performAttritionAnalysis() {
        if (employees.empty()) {
            std::cout << "No employee data for attrition analysis.\n";
            return;
        }

        int choice;
        AttritionAnalysis analysis;
        analysis.calculate(employees);

        while (true) {
            // system("cls");
            std::cout << "\nAttrition Analysis Options\n";
            std::cout << "--------------------------\n";
            std::cout << "1. Display Overall Attrition Analysis\n";
            std::cout << "2. Display Attrition Analysis by Department\n";
            std::cout << "3. Display Attrition Analysis by Years of Experience Range\n";
            std::cout << "4. Display Attrition Analysis by Month of Leave\n";
            std::cout << "5. Display Attrition Analysis by Gender\n";
            std::cout << "6. Display Attrition Analysis by Age Range (5 years)\n";
            std::cout << "7. Back to Main Menu\n";
            std::cout << "Enter your choice (1/2/3/4/5/6/7): ";
            std::cin >> choice;
            // system("cls");
            if(choice<1 and choice>7){
                std::cout<<"wrong input do it again"<<std::endl;
                break;
            }
            else{
                switch (choice) {
                case 1:
                    analysis.displayOverall();
                    break;
                case 2:
                    analysis.displayByDepartment();
                    break;
                case 3:
                    analysis.displayByExperience();
                    break;
                case 4:
                    analysis.displayByMonthOfLeave();
                    break;
                case 5:
                    analysis.displayByGender();
                    break;
                case 6:
                    analysis.displayByAgeRange();
                    break;
                case 7:
                    return;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
            }
            
        }
    }
};

class Gender {
public:
    int totalEmployees;
    int totalAttrition;

    Gender() : totalEmployees(0), totalAttrition(0) {}
};

class AgeRange {
public:
    int totalEmployees;
    int totalAttrition;

    AgeRange() : totalEmployees(0), totalAttrition(0) {}
};

// void generateSampleCSV(const std::string& filename) {
//     std::ofstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Error creating the file: " << filename << std::endl;
//         return;
//     }

//     file << "Employee ID,Age,Gender,Department,Years of Experience,Month of Leave,Salary,Attrition\n";
//     file << "1,30,M,HR,5,6,50000,0\n";
//     file << "2,35,F,Finance,7,8,60000,0\n";
//     file << "3,25,M,IT,3,3,45000,1\n";
//     file << "4,40,F,Sales,10,12,70000,0\n";
//     file << "5,28,F,IT,4,9,52000,1\n";
//     file << "6,32,M,Finance,8,7,55000,0\n";
//     file << "7,27,M,HR,2,5,48000,1\n";
//     file << "8,45,F,Sales,12,10,75000,0\n";
//     file << "9,29,F,IT,6,11,58000,0\n";
//     file << "10,38,M,Sales,9,4,63000,1\n";

//     file.close();
// }

int main(int argc, char *argv[]) {

    // if(const char* env_p = std::getenv("ResourceFile")){
    //     // std::cout << "Your PATH is: " << env_p << std::endl<<std::endl;
    //     std::string filename = env_p;
    //     filename = filename + "\\DataSet.csv";
    //     std::cout << filename << std::endl << std::endl;
    // }

    EmployeeDatabase employeeDB;
    int choice=1;
    // system("cls");

    Home:
        std::cout << "\t\t\t\t==================== Welcome User ====================" << std::endl;
        std::cout << "\t\t\t\t\tPress Any Key To Continue to App..." << std::endl; _getch();
        goto mainbase;

    mainbase:
        while (true) {
            // system("cls");--------------------------------------
            std::cout << "\nEmployee Attrition Analysis\n";
            std::cout << "--------------------------\n";
            std::cout << "1. Show Employee Data\n";
            std::cout << "2. Perform Attrition Analysis\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter your choice (1/2/3): ";
            std::cin >> choice;
            // system("cls");
            if(choice==1 or choice==2 or choice==3){
                    switch (choice) {
                    case 1:
                        employeeDB.showEmployeeData();
                        break;
                    case 2:
                        employeeDB.performAttritionAnalysis();
                        break;
                    case 3:
                        std::cout << "Exiting the program. Goodbye!\n";
                        return 0;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                        break;
                }
            }
            else{
                std::cout<<"wrong input do it again"<<std::endl;
                break;
            }
            
            
    }
    return 0;
}