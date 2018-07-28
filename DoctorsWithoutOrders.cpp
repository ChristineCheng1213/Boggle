/* Name: Xiaoyu Cheng (Christine)
 * Course: CS106B
 * Section: Thurs 3:30pm
 * Description: Finds the schedule of assigning all patients to doctors
 * Resources: Lecture slides & Stanford Library Documentation
 */

#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
#include "vector.h"
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */


bool canAllPatientsBeSeenHelper(Vector<Doctor>& doctors, Vector<Patient> &patients, int seenPatients, Map<string,int>& m, Map<string, Set<string>>& schedule);
bool isValid(Map<string, int>& m);


/*
 * Find whether all patients can be seen.
 * Specifically, it initializes a map that stores the number of hours each doctor have and call its helper function.
 * Accepts vector of doctors and patients, and a map of schedule.
 * Returns true if all patients could be seen and modifies schedule directly, returns false if no solution exists.
 */
bool canAllPatientsBeSeen(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule) {

    Map<string, int> m;

    for (Doctor d : doctors){           //Using extra ADT to store hoursFree might be ineffcient, but if function
        m[d.name] = d.hoursFree;        //directly change each doctor object's hoursFree, at the end we have to
        schedule[d.name] = {};          //restore all the values, which could be even more inefficient and messy.
    }

    return canAllPatientsBeSeenHelper(doctors, patients, 0, m, schedule);
}


/*
 * Finds whether all patients could be assigned to a doctor using recursive backtracking.
 * Accepts all parameters from its parent function, and a int represents the number of patients has been assigned, and a map stores doctors' hoursFree
 * Returns true and modify schedule if solution exitst, otherwise returns false.
 */
bool canAllPatientsBeSeenHelper(Vector<Doctor>& doctors, Vector<Patient> &patients, int seenPatients, Map<string, int>& m, Map<string, Set<string>>& schedule){

    if (!isValid(m)) {
        //base case: some doctor's hours has been exceeded
        return false;
    } else if (seenPatients >= patients.size()) {
        //base case: successfully assigned all patients
        return true;
    } else {
        //recursive: try to assign next patient to every doctor
        Patient p = patients[seenPatients];

        for (Doctor d : doctors) {
                //choose
                m[d.name] -= p.hoursNeeded;

                //explore
                if (canAllPatientsBeSeenHelper(doctors, patients, seenPatients + 1, m, schedule)) {
                    schedule[d.name].add(p.name);
                    return true;
                }

                //un-choose
                m[d.name] += p.hoursNeeded;
        }
        return false;
    }
}


/*
 * Find whether any doctor's time limit has been exceeded.
 * Accepts a map of doctor's hours and returns false if any doctor's hour is negative.
 */
bool isValid(Map<string, int>& m) {

    for (string doctor : m.keys()) {
        if (m[doctor] < 0) {
            return false;
        }
    }
    return true;
}
