# Physiotherapy Center Simulation System

A simulation project developed as part of the _Data Structures and Algorithms_ course at Cairo University's Faculty of Engineering. The goal is to simulate and manage a physiotherapy center's workflow including patient appointments, treatment sessions, and resource allocation.

## 📌 Objective

Design and implement a system that simulates:

- Patient arrival and treatment scheduling
- Therapist schedules
- Utilization of limited treatment resources

Emphasis is placed on applying efficient data structures and algorithms.

---

## 🧠 Core Concepts

- **Time Simulation**: Actions occur in discrete timesteps.
- **Patient Types**:
  - `N-Patient`: Must follow treatment order as provided.
  - `R-Patient`: Can undergo treatments in any order.
- **Treatment Types**:
  - `E-therapy`: Requires electro devices
  - `U-therapy`: Requires ultrasound devices
  - `X-therapy`: Requires gym rooms (shared resource with capacity)

---

## 📋 System Components

### 🧾 Main Lists

- **All Patients**
- **Early Patients** (`VT < PT`)
- **Late Patients** (`VT > PT`)
- **Waiting Patients**
- **In-treatment Patients**
- **Finished Patients**

### 📦 Resources

- Electro Devices (`E`)
- Ultrasound Devices (`U`)
- Gym Rooms (`X`, capacity-loaded)

---

## ⚙️ Features

- Prioritized and time-sensitive resource allocation
- Support for treatment **rescheduling** and **cancellation** with probabilities
- Simulation runs in **interactive** or **silent** mode
- Input and output follow strict formatting

---

## 📁 File Formats

### Input

Includes:

- Number and capacity of devices/rooms
- Patients with:
  - Type (`N` or `R`)
  - Appointment and arrival time
  - Required treatments with durations

### Output

Includes for each patient:

- ID, Type, PT, VT, FT
- Total Waiting and Treatment Time
- Cancellation/Reschedule flags

Also includes overall statistics like:

- Total time, averages, and percentages of early/late/cancelled/rescheduled patients

---

## ✅ Example Patient Record

```
N 50 40 2 U 10 X 5
```

> Normal patient, PT=50, VT=40, needs 10 units of U-therapy then 5 units of X-therapy.

---

## 🖥️ Simulation Modes

- **Interactive Mode**: Step-by-step output with user input
- **Silent Mode**: Fast execution, only generates the output file

---
