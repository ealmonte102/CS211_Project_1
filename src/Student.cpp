// Student.cpp
// Created by Evan Almonte on 10/18/2015.
//
#include "Student.hpp"
#include "Utilities.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::istream;
using std::string;
using std::ostream;

string idToInt(int num) {
	string createdID = "000000";
	int location = createdID.length() - 1;
	while (num > 0) {
		createdID[location--] = '0' + num % 10;
		num /= 10;
	}
	return createdID;
}

const double Student::MIN_GPA = 0.0;
const double Student::MAX_GPA = 4.0;

// Constructors
Student::Student( ) : lastName(""), firstName(""), id(-1),
                      standing(""), credits(-1), gpa(-1.0) { }

Student::Student(string first, string last, int id, int creds, double gpa, Date dob, Date matric)
	: lastName(last), firstName(first), id(id), credits(creds),
	  gpa(gpa), dob(dob), matriculation(matric) {
	if (credits < 0) {
		this->credits = 0;
	}
	if (gpa < MIN_GPA || gpa > MAX_GPA) {
		this->gpa = 0;
	}
	setStanding();
}

// Accessors
int Student::getCredits( ) const {
	return credits;
}

std::string Student::getFirstName( ) const {
	return firstName;
}

const Date& Student::getDob( ) const {
	return dob;
}

// Returns the first name and last name of the student seperated by a space.
std::string Student::getFullName( ) const {
	return firstName + " " + lastName;
}

double Student::getGpa( ) const {
	return gpa;
}

int Student::getId( ) const {
	return id;
}

std::string Student::getLastName( ) const {
	return lastName;
}

const Date& Student::getMatric( ) const {
	return matriculation;
}

std::string Student::getStanding( ) const {
	return standing;
}

// Mutators
// If the number pass is negative, the default value of 0 will be used.
void Student::setCredits(int creds) {
	creds < 0 ? credits = 0 : credits = creds;
	setStanding();
}

void Student::setDob(const Date& birthday) {
	dob = birthday;
}

void Student::setFirstName(std::string first) {
	firstName = first;
}

void Student::setGpa(double gradeAvg) {
	if (gradeAvg < MIN_GPA || gradeAvg > MAX_GPA) {
		cout << "Invalid GPA entered, the default value of 0 will be used.\n";
		gpa = 0;
	} else {
		gpa = gradeAvg;
	}
}

void Student::setId(int id) {
	this->id = id;
}

void Student::setLastName(std::string last) {
	lastName = last;
}

void Student::setMatric(const Date& matric) {
	matriculation = matric;
}

void Student::setStanding( ) {
	if (credits > 105) {
		standing = "Graduate";
	} else if (credits > 90) {
		standing = "Lower Senior";
	} else if (credits > 75) {
		standing = "Upper Junior";
	} else if (credits > 60) {
		standing = "Lower Junior";
	} else if (credits > 45) {
		standing = "Upper Sophmore";
	} else if (credits > 30) {
		standing = "Lower Sophomore";
	} else if (credits > 15) {
		standing = "Upper Freshman";
	} else {
		standing = "Lower Freshman";
	}
}

// Overloaded Operators
bool Student::operator==(const Student& otherStudent) const {
	return (lastName == otherStudent.lastName &&
		firstName == otherStudent.firstName &&
		id == otherStudent.id);
}

bool Student::operator!=(const Student& otherStudent) const {
	return !operator==(otherStudent);
}

bool Student::operator<(const Student& otherStudent) const {
	string myLast = upperConvert(lastName);
	string otherLast = upperConvert(otherStudent.lastName);
	string myFirst = upperConvert(firstName);
	string otherFirst = upperConvert(otherStudent.firstName);

	if (myLast < otherLast) {
		return true;
	}
	if (myLast > otherLast) {
		return false;
	}
	if (myFirst < otherFirst) {
		return true;
	}
	if (myFirst > otherFirst) {
		return false;
	}
	return id < otherStudent.id;
}

bool Student::operator<=(const Student& otherStudent) const {
	return !(*this > otherStudent);
}

bool Student::operator>(const Student& otherStudent) const {
	return otherStudent < *this;
}

bool Student::operator>=(const Student& otherStudent) const {
	return !(*this < otherStudent);
}

istream& operator>>(istream& input, Student& currentStudent) {
	cout << "Change First Name(Y/N)? ";
	if (getYesOrNo()) {
		cout << "First Name: ";
		input >> currentStudent.firstName;
		currentStudent.firstName = currentStudent.firstName;
	}
	cout << "Change Last Name(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Last Name: ";
		input >> currentStudent.lastName;
		currentStudent.lastName = currentStudent.lastName;
	}
	cout << "Change Student ID(Y/N)? ";
	if (getYesOrNo()) {
		cout << "ID: ";
		input >> currentStudent.id;
	}
	cout << "Change credits earned(Y/N)? ";
	if (getYesOrNo()) {
		int creds;
		cout << "Credits Earned: ";
		input >> creds;
		currentStudent.setCredits(creds);
	}
	cout << "Change GPA(Y/N)? ";
	if (getYesOrNo()) {
		double gradeAvg;
		cout << "GPA: ";
		input >> gradeAvg;
		currentStudent.setGpa(gradeAvg);
	}
	cout << "Change Date of Birth?(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Date of Birth:\n";
		cin >> currentStudent.dob;
	}
	cout << "Change Matriculation Date(Y/N)? ";
	if (getYesOrNo()) {
		cout << "Matriculation Date:\n";
		cin >> currentStudent.matriculation;
	}
	return input;
}

ostream& operator<<(ostream& output, const Student& currentStudent) {
	output << "First Name: " << currentStudent.firstName << "\n";
	output << "Last Name: " << currentStudent.lastName << "\n";
	output << "ID: " << currentStudent.id << "\n";
	output << "GPA: " << currentStudent.gpa << "\n";
	output << "Credits Earned: " << currentStudent.credits << "\n";
	output << "Standing: " << currentStudent.standing << "\n";
	output << "Date of Birth: " << currentStudent.dob << "\n";
	output << "Matriculation Date: " << currentStudent.matriculation << "\n";
	return output;
}

