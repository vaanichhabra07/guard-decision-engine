# Guard Decision Engine

A C++ decision system that selects the safest outcome in a secured facility by evaluating signals and applying priority-based logic.

---

## 🧠 The Idea

Instead of treating this as a simple “pick the correct door” problem,  
this solution models how a system makes decisions under uncertainty.

The focus is not on *which door is correct*,  
but on *how a system should think when it cannot see everything directly*.

---

## 🏢 Think of It Like This

Imagine sitting at a control desk inside a secured facility.

In front of you, there are three possible actions:
- Open Exit  
- Trigger Containment  
- Reset System  

You don’t see what’s behind the doors.  
You only observe signals:

- Power status  
- Alarm activity  
- Threat level  
- Override commands  
- Number of people inside  

---

## 📊 The Key Shift

At first, the problem looks like:
> “Which door should I choose?”

But in a real system, the question is:
> “Given these signals, what is the safest action right now?”

---

## ⚙️ Decision Model

The system follows a strict priority-based approach:

1. **System failure (power loss)**  
   → Reset (system cannot be trusted)

2. **Manual override**  
   → Safe Exit (authority takes control)

3. **Alarm + critical threat**  
   → Safe Exit (immediate evacuation)

4. **Alarm (uncertain state)**  
   → Reset (hold and reassess)

5. **Normal conditions with personnel**  
   → Safe Exit (standard operation)

6. **Normal conditions without personnel**  
   → Reset (facility lockdown)

7. **Critical state without alarm (silent anomaly)**  
   → Trap (unknown unsafe path)

8. **Default case**  
   → Reset (safety fallback)

---

## ⚡ How It Thinks

The system does not try to guess the “correct” door.

Instead, it:
- evaluates signals  
- prioritizes them  
- eliminates unsafe options first  
- acts on the most critical condition  

---

## 🔍 Why This Approach

This design reflects how real systems behave:

- Critical signals override normal flow  
- Safety is prioritized over optimization  
- Uncertainty leads to fallback, not risk  

It’s closer to how operating systems, safety systems, and control systems make decisions.

---

## 🧩 In One Line

A simple door-selection problem becomes a model of how systems make safe decisions by responding to signals in the right order.

---

## 🚀 How to Run

```bash
g++ main.cpp -o run
./run
