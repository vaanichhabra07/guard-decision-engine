
//  Secured Facility Guard Engine
//
//  Three doors. One correct choice.
//
//  [ ALPHA ]  Safe Exit      the goal
//  [ BETA  ]  Trap           containment route (danger zone)
//  [ GAMMA ]  Reset          hold position
//
//  Decision rules:
//
//  1. Power out              →  Reset     electronics unreliable
//  2. Control override       →  Safe Exit operator takes charge  
//  3. RED alarm              →  Safe Exit: Evacuate immediately
//  4. Any alarm              →  Reset     hold and reassess
//  5. Clear + staff inside   →  Safe Exit normal exit
//  6. Clear + building empty →  Reset lockdown
//  7. Silent RED anomaly     →  Trap      unsafe unknown path
//  8. Anything else          →  Reset     default safety
// ============================================================

#include <iostream>
#include <string>
#include <array>
#include <vector>

using namespace std;

//  Domain types


enum class Outcome { SAFE_EXIT, TRAP, RESET };
enum class Threat  { GREEN = 1, YELLOW = 2, RED = 3 };

struct Door {
    int     id;
    string  name;
    string  location;
    Outcome outcome;
};

struct Situation {
    string  label;
    Threat  threat;
    bool    alarm;
    bool    override;
    bool    power_out;
    int     staff;
};

struct Decision {
    string  scenario;
    string  reason;
    int     door_id;
    Outcome outcome;
};

//  Guard — decision engine

namespace Guard {

    vector<Decision> log;

    int find(const array<Door, 3>& doors, Outcome want) {
        for (int i = 0; i < 3; ++i)
            if (doors[i].outcome == want) return i;
        return 0;
    }

    int decide(const array<Door, 3>& doors, const Situation& s) {

        Outcome go;
        string  why;

        // 1. System failure -
        if (s.power_out) {
            go  = Outcome::RESET;
            why = "Power failure — fallback to reset.";
        }

        // 2. Manual override - obey immediately
        else if (s.override) {
            go  = Outcome::SAFE_EXIT;
            why = "Override active — safe exit authorised.";
        }

        // 3. Confirmed danger - evacuate
        else if (s.alarm && s.threat == Threat::RED) {
            go  = Outcome::SAFE_EXIT;
            why = "Critical alarm — immediate evacuation.";
        }

        // 4. Alarm but unclear - hold
        else if (s.alarm) {
            go  = Outcome::RESET;
            why = "Alarm active — holding position for reassessment.";
        }

        // 5. Normal operation - allow exit
        else if (s.threat == Threat::GREEN && s.staff > 0) {
            go  = Outcome::SAFE_EXIT;
            why = "All clear — normal exit permitted.";
        }

        // 6. Empty building - lock down
        else if (s.threat == Threat::GREEN && s.staff == 0) {
            go  = Outcome::RESET;
            why = "No personnel — facility locked down.";
        }

        // 7. Silent RED - unknown critical risk
        else if (s.threat == Threat::RED && !s.alarm) {
            go  = Outcome::TRAP;
            why = "Critical anomaly without alarm — unsafe path detected.";
        }

        // 8. Default safety
        else {
            go  = Outcome::RESET;
            why = "Uncertain state — defaulting to reset.";
        }

        int i = find(doors, go);
        log.push_back({ s.label, why, doors[i].id, go });
        return i;
    }

} // namespace Guard


namespace Show {

    string str(Outcome o) {
        if (o == Outcome::SAFE_EXIT) return "SAFE EXIT";
        if (o == Outcome::TRAP)      return "TRAP";
        return "RESET";
    }

    string str(Threat t) {
        if (t == Threat::GREEN)  return "GREEN";
        if (t == Threat::YELLOW) return "YELLOW";
        return "RED";
    }

    void line(char c = '-', int n = 60) {
        cout << string(n, c) << "\n";
    }

    void scenario(const Situation& s,
                  const array<Door, 3>& doors,
                  int chosen,
                  const Decision& d)
    {
        line();
        cout << "  " << s.label << "\n\n";

        cout << "  Threat " << str(s.threat)
             << " | Alarm "    << (s.alarm ? "YES" : "no")
             << " | Override " << (s.override ? "YES" : "no")
             << " | Power "    << (s.power_out ? "OUT" : "ok")
             << " | Staff "    << s.staff << "\n\n";

        for (int i = 0; i < 3; ++i) {
            bool picked = (i == chosen);
            cout << (picked ? "  => " : "     ")
                 << "Door " << doors[i].id
                 << " [" << doors[i].name << "] "
                 << doors[i].location;

            if (picked)
                cout << " --> " << str(doors[i].outcome);

     
        }

        cout << "\n  " << d.reason << "\n";
    }

    void audit(const vector<Decision>& log) {
        line('=');
        cout << "  AUDIT TRAIL\n";
        line('=');

        for (size_t i = 0; i < log.size(); ++i) {
            const auto& d = log[i];
            cout << "\n  [" << i + 1 << "] " << d.scenario
                 << "\n     Door    " << d.door_id
                 << "\n     Outcome " << str(d.outcome)
                 << "\n     Why     " << d.reason << "\n";
        }
        line('=');
    }

} // namespace Show


int main() {

    const array<Door, 3> doors = {{
        {1, "ALPHA", "North corridor", Outcome::SAFE_EXIT},
        {2, "BETA",  "East passage",   Outcome::TRAP},
        {3, "GAMMA", "West stairwell", Outcome::RESET},
    }};

    const array<Situation, 3> runs {{
        {"Routine",        Threat::GREEN, false, false, false, 4},
        {"Critical Alarm", Threat::RED,   true,  false, false, 4},
        {"Silent Anomaly", Threat::RED,   false, false, false, 3},
    }};

    for (const auto& s : runs) {
        int chosen = Guard::decide(doors, s);
        Show::scenario(s, doors, chosen, Guard::log.back());
    }

    Show::audit(Guard::log);

    return 0;
}