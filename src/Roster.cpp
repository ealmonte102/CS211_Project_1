// Roster.cpp
// Created by Evan Almonte on 10/18/2015.
//
#include "Roster.hpp"
#include "Student.hpp"
#include "Utilities.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::istream;
using std::ostream;
using std::string;

namespace RosterUtils {
	void printSpikeDesign(int length);
	void printSpikeDesignRev(int length);
	void printCharacter(char character, int length);
}

//Constructors
Roster::Roster( ) : courseName(""), courseCode(""), numEnrolled(0), instructor(""),
                    credits(0), capacity(5), studentList(new Student*[capacity]) {
	for (int i = 0; i < capacity; ++i) {
		studentList[i] = nullptr;
	}
}

Roster::Roster(std::string course, std::string courseNumber, std::string instructor, int credits, int maxCapacity) :
	courseName(course), courseCode(courseNumber), numEnrolled(0),
	instructor(instructor), credits(credits), capacity(maxCapacity) {
	if (credits <= 0 || credits > 4) {
		cout << "Invalid number of credits given: " << credits << "\n";
		cout << "Please try again: ";
		this->credits = getValidInt();
	}
	if (capacity < 0) {
		capacity = 0;
	}
	studentList = new Student*[capacity];
	for (int i = 0; i < capacity; ++i) {
		studentList[i] = nullptr;
	}
}

Roster::Roster(const Roster& rhs) :
	courseName(rhs.courseName), courseCode(rhs.courseCode), instructor(rhs.instructor),
	credits(rhs.credits), capacity(rhs.capacity), studentList(new Student*[capacity]) {
	for (numEnrolled = 0; numEnrolled < rhs.numEnrolled; ++numEnrolled) {
		studentList[numEnrolled] = rhs.studentList[numEnrolled];
	}
}

Roster::~Roster( ) {
	delete[] studentList;
}

//Adds a student to the roster. Capacity increased if reached by size.
void Roster::addStudent(Student* aStudent) {
	if (numEnrolled == capacity) {
		grow();
	}
	studentList[numEnrolled++] = aStudent;
}

//Adds an array of students to the roster. If the capacity is reached while adding students
//it will be increased.
void Roster::addStudent(Student* newStudents[], int numOfStudents) {
	for (int i = 0; i < numOfStudents; ++i , ++numEnrolled) {
		if (numEnrolled == capacity) {
			grow();
		}
		studentList[numEnrolled] = newStudents[i];
	}
}

void Roster::editStudent(std::string lastName) const {
	clearScreen();
	int location = findStudent(lastName);
	if (location != STUDENT_NOT_FOUND && location != NONE_CHOSEN) {
		clearScreen();
		cout << "********************************\n";
		cout << "         Editing Student        \n";
		cout << "********************************\n";
		cout << *studentList[location] << "\n";
		cin >> *studentList[location];
	}
}

//Removes all students from the roster. Capacity is left unchanged.
void Roster::removeAll( ) {
	while (numEnrolled > 0) {
		studentList[--numEnrolled] = nullptr;
	}
}

void Roster::setCourseName(std::string cN) {
	courseName = cN;
}

void Roster::setCourseCode(std::string cC) {
	courseCode = cC;
}

void Roster::setInstructorName(std::string iN) {
	instructor = iN;
}

void Roster::setCredits(int creds) {
	credits = creds;
}

string Roster::getCourseCode( ) const {
	return courseCode;
}

string Roster::getCourseName( ) const {
	return courseName;
}

string Roster::getInstructorName( ) const {
	return instructor;
}

int Roster::getNumEnrolled( ) const {
	return numEnrolled;
}

int Roster::getNumOfCredits( ) const {
	return credits;
}

void Roster::displayInfo( ) const {
	int maxLength = courseName.length();
	if (instructor.length() > maxLength) {
		maxLength = instructor.length() + string("Instructor: ").length();
	} else {
		maxLength += string("Course: ").length();
	}
	RosterUtils::printSpikeDesign(maxLength);
	cout << "Course: " << courseName << "\n";
	cout << "Course Code: " << courseCode << "\n";
	cout << "Instructor: " << instructor << "\n";
	cout << "Credits: " << credits << "\n";
	cout << "Students Enrolled: " << numEnrolled << "/" << capacity << "\n";
	RosterUtils::printSpikeDesignRev(maxLength);
}

void Roster::listAllStudents( ) const {
	sortUp();
	if (numEnrolled != 0) {
		cout << "______________________________\n";
		for (int i = 0; i < numEnrolled; ++i) {
			cout << studentList[i]->getLastName() + ", " + studentList[i]->getFirstName();
			cout << ": " << studentList[i]->getId() << "\n";
		}
		cout << "______________________________\n";
	} else {
		cout << "The current roster is empty.\n";
	}
}

//Removes student from the current roster.
void Roster::removeStudent(string lastName) {
	int location = findStudent(lastName);
	if (location != STUDENT_NOT_FOUND && location != NONE_CHOSEN) {
		//Shift all the students over to the left in order to avoid empty gaps.
		for (int i = location; i < numEnrolled; ++i) {
			studentList[i] = studentList[i + 1];
		}
		--numEnrolled;
	}
}

//If found, returns the index of a student in studentList otherwise returns STUDENT_NOT_FOUND
//if none are found, or NONE_CHOSEN if user wishes to quit without choosing a student.
int Roster::findStudent(string lastName) const {
	//foundStudents: Holds the indices of where the students are located.
	//numFound: Holds the a count referring to the number of students found.
	int* foundStudents = new int[numEnrolled];
	cout << "Students Found:\n";
	int numFound = findStudentHelper(foundStudents, lastName);

	//If no students are found, return to the caller.
	if (numFound == 0) {
		clearScreen();
		cout << "********************************\n";
		cout << "             ERROR!             \n";
		cout << "********************************\n";
		cout << "No students with a last name of \"" << lastName << "\" were found.\n";
		return STUDENT_NOT_FOUND;
	}

	int studentIndex = NONE_CHOSEN;
	int userChoice = -1;
	cout << "Please choose a student(0 to quit): ";
	while (userChoice < 0 || userChoice > numFound) {
		userChoice = getValidInt();
	}
	if (userChoice != 0) {
		studentIndex = foundStudents[userChoice - 1];
	}

	delete[] foundStudents;
	return studentIndex;
}

//Returns the number of students found in the Roster with the specified last name.
//Also populates foundStudents with the each found students index in studentList.
int Roster::findStudentHelper(int* foundStudents, string lastName) const {
	//numFound refers to the number of students found with the same last name.
	int numFound = 0;
	for (int i = 0; i < numEnrolled; ++i) {
		if (upperConvert(studentList[i]->getLastName()) == upperConvert(lastName)) {
			cout << numFound + 1 << ". " + studentList[i]->getFullName() + ", " << studentList[i]->getId() << "\n";
			foundStudents[numFound] = i;
			numFound++;
		}
	}
	return numFound;
}

//Sorts the roster in reverse alphebetical order.
void Roster::sortDown( ) const {
	selectionSortDown(studentList, numEnrolled);
}

//Sorts the roster in alphabetical order.
void Roster::sortUp( ) const {
	selectionSortUp(studentList, numEnrolled);
}

//Used to increase the capacity of the roster if it's size surpasses the current capacity.
void Roster::grow( ) {
	int newCapacity = 2 * capacity + 1;
	Student** tempList = new Student*[newCapacity];
	capacity = newCapacity;
	//Fill the array with the current student list.
	for (int i = 0; i < numEnrolled; ++i) {
		tempList[i] = studentList[i];
	}
	//Fill the rest of the array with null pointers.
	for (int i = numEnrolled; i < capacity; ++i) {
		tempList[i] = nullptr;
	}
	delete[] studentList;
	studentList = tempList;
}

//Overloaded Operators
ostream& operator<<(ostream& output, const Roster& currentRoster) {
	int maxLength = currentRoster.getCourseName().length();
	if (currentRoster.getInstructorName().length() > maxLength) {
		maxLength = currentRoster.getInstructorName().length() + string("Instructor: ").length();
	} else {
		maxLength += string("Course: ").length();
	}
	RosterUtils::printCharacter('=', maxLength);
	output << "Course: " << currentRoster.courseName << "\n";
	output << "Course Code: " << currentRoster.courseCode << "\n";
	output << "Instructor: " << currentRoster.instructor << "\n";
	output << "Credits: " << currentRoster.credits << "\n";
	output << "Students Enrolled: " << currentRoster.numEnrolled << "/" << currentRoster.capacity << "\n";
	RosterUtils::printCharacter('=', maxLength);
	for (int i = 0; i < currentRoster.numEnrolled; ++i) {
		output << currentRoster.studentList[i]->getLastName() + ", " + currentRoster.studentList[i]->getFirstName();
		output << ": " << currentRoster.studentList[i]->getId() << "\n";
	}
	RosterUtils::printCharacter('_', maxLength);
	return output;
}

std::istream& operator>>(std::istream& input, Roster& currentRoster) {
	cout << "Change the course name(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Please enter the course name: ";
		getline(input, currentRoster.courseName);
	}
	cout << "Change the course code(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Please enter the course code: ";
		getline(input, currentRoster.courseCode);
	}
	cout << "Change the course instructor?(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Please enter the course instructors name: ";
		getline(input, currentRoster.instructor);
	}
	cout << "Change the credits awarded upon completion(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Please enter the credits awarded upon completion: ";
		do {
			currentRoster.credits = getValidInt();
		} while (currentRoster.credits < 1 || currentRoster.credits > 4);
	}
	return input;
}

//Returns an immutable Student object. Bounds are checked.
const Student& Roster::operator[](int index) const {
	if (index < 0 || index >= numEnrolled) {
		cout << "Array index out of bounds: " << index << "\n";
		exit(1);
	}
	return *studentList[index];
}

Roster& Roster::operator=(const Roster& rhs) {
	if (this != &rhs) {
		delete[] studentList;

		courseName = rhs.courseName;
		courseCode = rhs.courseCode;
		instructor = rhs.instructor;
		credits = rhs.credits;
		capacity = rhs.capacity;
		studentList = new Student*[capacity];
		for (numEnrolled = 0; numEnrolled < rhs.numEnrolled; ++numEnrolled) {
			studentList[numEnrolled] = rhs.studentList[numEnrolled];
		}
	}
	return *this;
}


namespace RosterUtils {
	void printSpikeDesign(int length) {
		for (int i = 0; i < length; ++i) {
			i % 2 == 0 ? cout << "/" : cout << "\\";
		}
		cout << "\n";
	}

	void printSpikeDesignRev(int length) {
		for (int i = 0; i < length; ++i) {
			i % 2 == 0 ? cout << "\\" : cout << "/";
		}
		cout << "\n";
	}

	void printCharacter(char character, int length) {
		for (int i = 0; i < length; ++i) {
			cout << character;
		}
		cout << "\n";
	}
}

