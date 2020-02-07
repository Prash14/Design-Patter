/*  Observer Design Pattern:
============================
Model the "independent" functionality with a "subject" abstraction
Model the "dependent" functionality with "observer" hierarchy
The Subject is coupled only to the Observer base class
Observers register themselves with the Subject
The Subject broadcasts events to all registered Observers
Observers "pull" the information they need from the Subject
Client configures the number and type of Observers
*/

#include "iostream"
#include "vector"
#include "algorithm"
using namespace std;

class Observer; // Forward declaration
/*
Subject represents the core (or independent) abstraction.
It contains View
1) function to attach the view with model (model is present in observer)
2) Publisher Function to notify all the views
*/
class Subject
{
	vector <Observer *> view; 
	
	int value;
	
	public:	
	void attach(Observer *obs) {
		view.push_back(obs);
	}
	
	void notify();
	
	//helper function
	void set_value(int val){
		value = val;
		notify();
	}
	
	int get_value() { return value;}
	
};


/*
The Observer defines a one-to-many relationship so that when one object changes state, 
the others are notified and updated automatically. 
Observers register themselves with the Subject when the object is create.
It contains model
& Update function to update the view.
*/
class Observer
{
	int deamon;
	Subject *model_;
	
	public:
	virtual void update() = 0;
	
	Observer(Subject *model,int div): deamon(div),model_(model)
	{
		 // 4. Observers register themselves with the Subject
        model_->attach(this);
	}
	
	//helper function
	protected:
    Subject *getSubject() {
        return model_;
    }
    int getDivisor() {
        return deamon;
    }
	
};

void Subject::notify()
{
	  for_each(view.begin(), view.end(), [](Observer *obs) { obs->update(); }); //Lambda Expression
	  //for (int i = 0; i < view.size(); i++)
	 //  view[i]->update();
}

/*
divObserver : Divisior View
*/
class divObserver : public Observer
{
	public:
	divObserver(Subject *model,int div):Observer(model,div)
	{
	}
	
	void update()
	{
		// 6. "Pull" information of interest
        int v = getSubject()->get_value(), d = getDivisor();
        cout << v << " Div " << d << " is " << v / d << '\n';
	}
};



/*
modObserver : Modulus View
*/
class modObserver : public Observer
{
	public:
	modObserver(Subject *model,int div):Observer(model,div)
	{
	}
	
	void update()
	{
		// 6. "Pull" information of interest
        int v = getSubject()->get_value(), d = getDivisor();
        cout << v << " Mod " << d << " is " << v % d << '\n';
	}
};

int main() {
  Subject subj;
  divObserver divObs1(&subj, 4); // 7. Client configures the number and
  divObserver divObs2(&subj, 3); //    type of Observers
  modObserver modObs3(&subj, 3);
  subj.set_value(14);
}