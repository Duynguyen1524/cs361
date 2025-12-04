#ifndef HUMAN_H
#define HUMAN_H
#include <string>
#include "Boat.h"
/**
 * @class Human
 * Represents an adult or child participating in the Island Crossing simulation.
 *
 * A Human object stores a unique ID, a type (adult or child), and a pointer to the
 * shared Boat object after boarding. 
 */
class Human {
    public:
    /** Constructs a Human with a role and ID.*/ 
        Human(int id, int type){
            myid = id;
            mytype = type;
        }
        /** Returns the human's integer ID.*/
        int get_id(){ return myid;};
        /**Stored when the thread boards the boat. */
        Boat* myboat;
        /**Adds the human to the specified boat. */
        void addBoat(Boat* boat){
            myboat = boat;
        }
        /** Returns the human's type (0 = adult, 1 = child)s */
        int gettype(){
            return mytype;
        }


         
    private:
        int myid; //id for human. 
        int mytype; // 0 for adult , 1 for child


};
#endif // HUMAN_H