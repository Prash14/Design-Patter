// C++ program to demonstrate factory method design pattern
/*
Define an interface for creating an object, but let subclasses decide which class to instantiate. 
Factory Method lets a class defer instantiation to subclasses.
Defining a "virtual" constructor.

Factory Method is to creating objects as Template Method is to implementing an algorithm. 
A superclass specifies all standard and generic behavior (using pure virtual "placeholders" 
for creation steps), and then delegates the creation details to subclasses that are 
supplied by the client.
*/

#include <iostream>
using namespace std;
 
enum VehicleType {
    VT_TwoWheeler,    VT_ThreeWheeler,    VT_FourWheeler
};
 
// Library classes
class Vehicle {
public:
    virtual void printVehicle() = 0;
    static Vehicle* Create(VehicleType type);
};
class TwoWheeler : public Vehicle {
public:
    void printVehicle() {
        cout << "I am two wheeler" << endl;
    }
};
class ThreeWheeler : public Vehicle {
public:
    void printVehicle() {
        cout << "I am three wheeler" << endl;
    }
};
class FourWheeler : public Vehicle {
    public:
    void printVehicle() {
        cout << "I am four wheeler" << endl;
    }
};
 
// Factory method to create objects of different types.
// Change is required only in this function to create a new object type
Vehicle* Vehicle::Create(VehicleType type) {
    if (type == VT_TwoWheeler)
        return new TwoWheeler();
    else if (type == VT_ThreeWheeler)
        return new ThreeWheeler();
    else if (type == VT_FourWheeler)
        return new FourWheeler();
    else return NULL;
}
 
// Client class
class Client {
public:
 
    // Client doesn't explicitly create objects
    // but passes type to factory method "Create()"
    Client(VehicleType type)
    {
        pVehicle = Vehicle::Create(type);
    }
    ~Client() {
        if (pVehicle) {
            delete[] pVehicle;
            pVehicle = NULL;
        }
    }
    Vehicle* getVehicle()  {
        return pVehicle;
    }
 
private:
    Vehicle *pVehicle;
};
 
// Driver program
int main() {	
	VehicleType type = VT_ThreeWheeler;
    Client *pClient = new Client(type);
    Vehicle * pVehicle = pClient->getVehicle();
    pVehicle->printVehicle();
    return 0;
}