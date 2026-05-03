/*
    MyForm_detailed_explained.h
    Detailed commented version for viva / presentation.

    Simple idea:
    - RoadNetwork stores the roads.
    - TrafficSignal controls red/green signal logic.
    - Vehicle stores each car state: moving, waiting, arrived.
    - MetricsCollector calculates delay, congestion, throughput, etc.
    - MyForm only connects these classes with the GUI and draws the simulation.
*/

// This prevents the header file from being included twice by mistake.
#pragma once

// This includes the road network class so the GUI can create and manage roads.
#include "RoadNetwork.h"
// This includes the traffic signal class so the GUI can show red and green lights.
#include "TrafficSignal.h"
// This includes the vehicle class so cars can be created, moved, stopped, and arrived.
#include "Vehicle.h"
// This includes the metrics class so the GUI can calculate delay, congestion, and throughput.
#include "MetricsCollector.h"

// Vector is used to store lists of cars, signals, routes, and other simulation data.
#include <vector>
// String is used for text values coming from the C++ classes.
#include <string>
// cmath is used for mathematical calculations like distance and direction.
#include <cmath>
// algorithm is used for helper functions like max.
#include <algorithm>

// This allows us to use .NET classes like String and Object easily.
using namespace System;
// This allows us to use .NET classes like String and Object easily.
using namespace System::Drawing;
// This allows us to use .NET classes like String and Object easily.
using namespace System::Windows::Forms;

// This groups the whole GUI code under the project namespace.
namespace TrafficFlowGUI {

    // This is the main Windows Form class that displays and controls the whole simulation GUI.
    public ref class MyForm : public Form
    {
        // Public members can be accessed when the form object is created.
    public:
        // This constructor runs automatically when the GUI window is opened.
        MyForm()
        {
            // This initializes network as empty so the program does not use garbage memory.
            network = nullptr;
            // This initializes metrics as empty so the program does not use garbage memory.
            metrics = nullptr;
            // This initializes vehicles as empty so the program does not use garbage memory.
            vehicles = nullptr;
            // This initializes signals as empty so the program does not use garbage memory.
            signals = nullptr;
            // This initializes paths as empty so the program does not use garbage memory.
            paths = nullptr;
            // This initializes pathIndex as empty so the program does not use garbage memory.
            pathIndex = nullptr;
            // This initializes releaseStep as empty so the program does not use garbage memory.
            releaseStep = nullptr;
            // This initializes startStep as empty so the program does not use garbage memory.
            startStep = nullptr;
            // This initializes freeFlowTime as empty so the program does not use garbage memory.
            freeFlowTime = nullptr;
            // This initializes launched as empty so the program does not use garbage memory.
            launched = nullptr;
            // This initializes visualTravelTotal as empty so the program does not use garbage memory.
            visualTravelTotal = nullptr;

            // This builds all GUI controls such as buttons, panels, grids, labels, and timer.
            InitializeComponent();
            // This prepares the road and vehicle tables before data is shown.
            ConfigureTables();
            // This sets the screen positions of the road network nodes.
            BuildNodePositions();
            // This loads the default roads, signals, and vehicles for the demo.
            LoadPresetScenario();
            // This updates the tables, metrics, and visual display.
            RefreshAllViews();
        }

    protected:
        // This constructor runs automatically when the GUI window is opened.
        ~MyForm()
        {
            // This stops automatic running before resetting or finishing the simulation.
            if (simTimer != nullptr) simTimer->Stop();
            DestroyNativeObjects();
            if (components) delete components;
        }

        // Private members are only used inside this GUI class to keep the code organized.
    private:
        Label^ label1;
        Panel^ panelLeft;
        Panel^ panelMap;
        Panel^ panelRight;
        Panel^ panelBottom;
        Button^ btnLoadPreset;
        Button^ btnStart;
        Button^ btnStep;
        Button^ btnReset;
        Button^ btnGenerateReport;
        Button^ btnAddCar;
        NumericUpDown^ numSteps;
        CheckBox^ chkAutoStep;
        ComboBox^ cmbSpawnNode;
        Label^ label3;
        RichTextBox^ txtLog;
        DataGridView^ dgvRoads;
        DataGridView^ dgvVehicles;
        GroupBox^ grpMetrics;
        Label^ lblStepValue;
        Label^ lblArrivedValue;
        Label^ lblAvgTravelValue;
        Label^ lblDelayValue;
        Label^ lblThroughputValue;
        Label^ lblCongestionValue;
        Label^ lblObjectiveValue;
        Timer^ simTimer;
        System::ComponentModel::Container^ components;

        RoadNetwork* network;
        MetricsCollector* metrics;
        std::vector<Vehicle>* vehicles;
        // This creates a traffic signal object for an intersection.
        std::vector<TrafficSignal>* signals;
        std::vector<std::vector<int> >* paths;
        std::vector<int>* pathIndex;
        std::vector<int>* releaseStep;
        std::vector<int>* startStep;
        std::vector<double>* freeFlowTime;
        std::vector<bool>* launched;
        std::vector<int>* visualTravelTotal;

        System::Collections::Generic::Dictionary<int, Point>^ nodePositions;
        int currentStep;
        bool presetLoaded;

        // This function creates and styles all GUI components on the form.
        void InitializeComponent()
        {
            // This creates a new Windows Forms control or .NET object.
            components = gcnew System::ComponentModel::Container();
            // This creates a new Windows Forms control or .NET object.
            label1 = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            panelLeft = gcnew Panel();
            // This creates a new Windows Forms control or .NET object.
            label3 = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            numSteps = gcnew NumericUpDown();
            // This creates a new Windows Forms control or .NET object.
            chkAutoStep = gcnew CheckBox();
            // This creates a new Windows Forms control or .NET object.
            cmbSpawnNode = gcnew ComboBox();
            // This creates a new Windows Forms control or .NET object.
            btnAddCar = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            btnReset = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            btnStep = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            btnStart = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            btnLoadPreset = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            panelMap = gcnew Panel();
            // This creates a new Windows Forms control or .NET object.
            panelBottom = gcnew Panel();
            // This creates a new Windows Forms control or .NET object.
            panelRight = gcnew Panel();
            // This creates a new Windows Forms control or .NET object.
            dgvVehicles = gcnew DataGridView();
            // This creates a new Windows Forms control or .NET object.
            btnGenerateReport = gcnew Button();
            // This creates a new Windows Forms control or .NET object.
            dgvRoads = gcnew DataGridView();
            // This creates a new Windows Forms control or .NET object.
            grpMetrics = gcnew GroupBox();
            // This creates a new Windows Forms control or .NET object.
            lblObjectiveValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblCongestionValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblThroughputValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblDelayValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblAvgTravelValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblArrivedValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            lblStepValue = gcnew Label();
            // This creates a new Windows Forms control or .NET object.
            txtLog = gcnew RichTextBox();
            // This creates a new Windows Forms control or .NET object.
            simTimer = gcnew Timer(components);

            SuspendLayout();

            // This attaches the control to a side of the form so the layout adjusts properly.
            label1->Dock = DockStyle::Top;
            // This sets the text shown on this control.
            label1->Text = L"Traffic Flow Optimization System";
            // This sets the progress position of the car along the road.
            label1->Height = 40;
            label1->TextAlign = ContentAlignment::MiddleCenter;
            // This creates a new Windows Forms control or .NET object.
            label1->Font = gcnew Drawing::Font(L"Segoe UI", 15, FontStyle::Bold);
            // This sets the text color for better visibility.
            label1->ForeColor = Color::White;
            // This sets the background color to make the GUI clearer and more professional.
            label1->BackColor = Color::FromArgb(18, 30, 45);

            // This attaches the control to a side of the form so the layout adjusts properly.
            panelLeft->Dock = DockStyle::Left;
            panelLeft->Width = 220;
            // This sets the background color to make the GUI clearer and more professional.
            panelLeft->BackColor = Color::FromArgb(232, 238, 245);
            panelLeft->Padding = System::Windows::Forms::Padding(10);

            // This sets the text shown on this control.
            btnLoadPreset->Text = L"Load Preset";
            // This sets the position and size of this control on the form.
            btnLoadPreset->SetBounds(12, 10, 180, 34);
            // This sets the background color to make the GUI clearer and more professional.
            btnLoadPreset->BackColor = Color::LightBlue;
            btnLoadPreset->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnLoadPreset->Click += gcnew EventHandler(this, &MyForm::btnLoadPreset_Click);

            // This sets the text shown on this control.
            btnStart->Text = L"Start / Run";
            // This sets the position and size of this control on the form.
            btnStart->SetBounds(12, 50, 180, 34);
            // This sets the background color to make the GUI clearer and more professional.
            btnStart->BackColor = Color::LightGreen;
            btnStart->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnStart->Click += gcnew EventHandler(this, &MyForm::btnStart_Click);

            // This sets the text shown on this control.
            btnStep->Text = L"Step";
            // This sets the position and size of this control on the form.
            btnStep->SetBounds(12, 90, 180, 90);
            // This sets the background color to make the GUI clearer and more professional.
            btnStep->BackColor = Color::LightYellow;
            btnStep->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnStep->Click += gcnew EventHandler(this, &MyForm::btnStep_Click);

            // This sets the text shown on this control.
            btnReset->Text = L"Reset";
            // This sets the position and size of this control on the form.
            btnReset->SetBounds(12, 190, 180, 90);
            // This sets the background color to make the GUI clearer and more professional.
            btnReset->BackColor = Color::LightCoral;
            btnReset->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnReset->Click += gcnew EventHandler(this, &MyForm::btnReset_Click);

            // This sets the text shown on this control.
            label3->Text = L"Steps to run";
            // This sets the position and size of this control on the form.
            label3->SetBounds(12, 300, 100, 20);

            // This sets the position and size of this control on the form.
            numSteps->SetBounds(129, 298, 60, 26);
            numSteps->Minimum = 1;
            numSteps->Maximum = 1000;
            numSteps->Value = 20;

            // This sets the text shown on this control.
            chkAutoStep->Text = L"Auto Step";
            // This sets the position and size of this control on the form.
            chkAutoStep->SetBounds(12, 335, 120, 24);
            chkAutoStep->Checked = true;

            // This sets the position and size of this control on the form.
            cmbSpawnNode->SetBounds(12, 370, 80, 26);
            cmbSpawnNode->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbSpawnNode->Items->AddRange(gcnew cli::array<Object^>{ L"0", L"1", L"2", L"3", L"4" });
            cmbSpawnNode->SelectedIndex = 0;

            // This sets the text shown on this control.
            btnAddCar->Text = L"Add Car";
            // This sets the position and size of this control on the form.
            btnAddCar->SetBounds(100, 368, 92, 30);
            // This sets the background color to make the GUI clearer and more professional.
            btnAddCar->BackColor = Color::LightSkyBlue;
            btnAddCar->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnAddCar->Click += gcnew EventHandler(this, &MyForm::btnAddCar_Click);

            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(btnLoadPreset);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(btnStart);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(btnStep);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(btnReset);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(label3);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(numSteps);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(chkAutoStep);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(cmbSpawnNode);
            // This adds the control to the panel or form so it becomes visible.
            panelLeft->Controls->Add(btnAddCar);

            // This attaches the control to a side of the form so the layout adjusts properly.
            panelMap->Dock = DockStyle::Fill;
            // This sets the background color to make the GUI clearer and more professional.
            panelMap->BackColor = Color::FromArgb(248, 250, 252);
            // This creates a new Windows Forms control or .NET object.
            panelMap->Paint += gcnew PaintEventHandler(this, &MyForm::panelMap_Paint);

            // This attaches the control to a side of the form so the layout adjusts properly.
            panelRight->Dock = DockStyle::Right;
            panelRight->Width = 320;
            // This sets the background color to make the GUI clearer and more professional.
            panelRight->BackColor = Color::FromArgb(238, 243, 248);
            panelRight->BorderStyle = BorderStyle::FixedSingle;

            // This attaches the control to a side of the form so the layout adjusts properly.
            grpMetrics->Dock = DockStyle::Top;
            // This sets the text shown on this control.
            grpMetrics->Text = L"Performance Metrics";
            // This creates a new Windows Forms control or .NET object.
            grpMetrics->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            // This sets the progress position of the car along the road.
            grpMetrics->Height = 195;

            AddMetricLabel(grpMetrics, L"Step:", 10, 25);        lblStepValue = AddMetricValue(grpMetrics, L"0", 150, 25);
            AddMetricLabel(grpMetrics, L"Arrived:", 10, 48);     lblArrivedValue = AddMetricValue(grpMetrics, L"0/0", 150, 48);
            AddMetricLabel(grpMetrics, L"Avg Travel:", 10, 71);  lblAvgTravelValue = AddMetricValue(grpMetrics, L"0.0", 150, 71);
            AddMetricLabel(grpMetrics, L"Total Delay:", 10, 94); lblDelayValue = AddMetricValue(grpMetrics, L"0", 150, 94);
            AddMetricLabel(grpMetrics, L"Throughput:", 10, 117); lblThroughputValue = AddMetricValue(grpMetrics, L"0.00", 150, 117);
            AddMetricLabel(grpMetrics, L"Avg Congestion:", 10, 140); lblCongestionValue = AddMetricValue(grpMetrics, L"0%", 150, 140);
            AddMetricLabel(grpMetrics, L"Objective:", 10, 163);  lblObjectiveValue = AddMetricValue(grpMetrics, L"0.00", 150, 163);

            // This attaches the control to a side of the form so the layout adjusts properly.
            dgvRoads->Dock = DockStyle::Top;
            // This sets the progress position of the car along the road.
            dgvRoads->Height = 180;
            dgvRoads->AllowUserToAddRows = false;
            dgvRoads->ReadOnly = true;
            dgvRoads->RowHeadersVisible = false;
            dgvRoads->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            // This attaches the control to a side of the form so the layout adjusts properly.
            dgvVehicles->Dock = DockStyle::Fill;
            dgvVehicles->AllowUserToAddRows = false;
            dgvVehicles->ReadOnly = true;
            dgvVehicles->RowHeadersVisible = false;
            dgvVehicles->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            // This sets the text shown on this control.
            btnGenerateReport->Text = L"Generate Final Report";
            // This attaches the control to a side of the form so the layout adjusts properly.
            btnGenerateReport->Dock = DockStyle::Bottom;
            // This sets the progress position of the car along the road.
            btnGenerateReport->Height = 32;
            // This sets the background color to make the GUI clearer and more professional.
            btnGenerateReport->BackColor = Color::LightSteelBlue;
            btnGenerateReport->FlatStyle = FlatStyle::Flat;
            // This creates a new Windows Forms control or .NET object.
            btnGenerateReport->Click += gcnew EventHandler(this, &MyForm::btnGenerateReport_Click);

            // This adds the control to the panel or form so it becomes visible.
            panelRight->Controls->Add(dgvVehicles);
            // This adds the control to the panel or form so it becomes visible.
            panelRight->Controls->Add(btnGenerateReport);
            // This adds the control to the panel or form so it becomes visible.
            panelRight->Controls->Add(dgvRoads);
            // This adds the control to the panel or form so it becomes visible.
            panelRight->Controls->Add(grpMetrics);

            // This attaches the control to a side of the form so the layout adjusts properly.
            panelBottom->Dock = DockStyle::Bottom;
            // This sets the progress position of the car along the road.
            panelBottom->Height = 128;
            // This sets the background color to make the GUI clearer and more professional.
            panelBottom->BackColor = Color::FromArgb(245, 245, 245);
            panelBottom->BorderStyle = BorderStyle::FixedSingle;

            // This attaches the control to a side of the form so the layout adjusts properly.
            txtLog->Dock = DockStyle::Fill;
            txtLog->ReadOnly = true;
            // This sets the background color to make the GUI clearer and more professional.
            txtLog->BackColor = Color::FromArgb(20, 28, 38);
            // This sets the text color for better visibility.
            txtLog->ForeColor = Color::Gainsboro;
            // This creates a new Windows Forms control or .NET object.
            txtLog->Font = gcnew Drawing::Font(L"Consolas", 9);
            // This adds the control to the panel or form so it becomes visible.
            panelBottom->Controls->Add(txtLog);

            // This adds the control to the panel or form so it becomes visible.
            panelMap->Controls->Add(panelBottom);
            // This adds the control to the panel or form so it becomes visible.
            panelMap->Controls->Add(panelRight);

            // This sets the speed of automatic simulation steps.
            simTimer->Interval = 800;
            // This creates a new Windows Forms control or .NET object.
            simTimer->Tick += gcnew EventHandler(this, &MyForm::simTimer_Tick);

            ClientSize = Drawing::Size(1273, 660);
            Controls->Add(panelMap);
            Controls->Add(panelLeft);
            Controls->Add(label1);
            // This sets the progress position of the car along the road.
            Text = L"Traffic Flow Optimization System";
            WindowState = FormWindowState::Maximized;
            ResumeLayout(false);
        }

        // This helper function creates the left-side text labels inside the metrics box.
        Label^ AddMetricLabel(Control^ parent, String^ text, int x, int y)
        {
            // This creates a new Windows Forms control or .NET object.
            Label^ lbl = gcnew Label();
            // This sets the text shown on this control.
            lbl->Text = text;
            lbl->Location = Point(x, y);
            lbl->AutoSize = true;
            // This creates a new Windows Forms control or .NET object.
            lbl->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            // This adds the control to the panel or form so it becomes visible.
            parent->Controls->Add(lbl);
            return lbl;
        }

        // This helper function creates the changing metric value labels inside the metrics box.
        Label^ AddMetricValue(Control^ parent, String^ text, int x, int y)
        {
            // This creates a new Windows Forms control or .NET object.
            Label^ lbl = gcnew Label();
            // This sets the text shown on this control.
            lbl->Text = text;
            lbl->Location = Point(x, y);
            lbl->AutoSize = true;
            // This creates a new Windows Forms control or .NET object.
            lbl->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            // This adds the control to the panel or form so it becomes visible.
            parent->Controls->Add(lbl);
            return lbl;
        }

        // This function fixes where each node/intersection appears on the map panel.
        void BuildNodePositions()
        {
            // This creates a new Windows Forms control or .NET object.
            nodePositions = gcnew System::Collections::Generic::Dictionary<int, Point>();
            // This gives a node its x and y screen position for drawing.
            nodePositions[0] = Point(120, 180);
            // This gives a node its x and y screen position for drawing.
            nodePositions[1] = Point(300, 180);
            // This gives a node its x and y screen position for drawing.
            nodePositions[2] = Point(480, 180);
            // This gives a node its x and y screen position for drawing.
            nodePositions[3] = Point(120, 360);
            // This gives a node its x and y screen position for drawing.
            nodePositions[4] = Point(300, 360);
            // This gives a node its x and y screen position for drawing.
            nodePositions[5] = Point(480, 360);
        }

        // This function creates the native C++ objects used by the simulation logic.
        void CreateNativeObjects()
        {
            network = new RoadNetwork();
            metrics = new MetricsCollector();
            vehicles = new std::vector<Vehicle>();
            // This creates a traffic signal object for an intersection.
            signals = new std::vector<TrafficSignal>();
            paths = new std::vector<std::vector<int> >();
            pathIndex = new std::vector<int>();
            releaseStep = new std::vector<int>();
            startStep = new std::vector<int>();
            freeFlowTime = new std::vector<double>();
            launched = new std::vector<bool>();
            visualTravelTotal = new std::vector<int>();
        }

        // This function deletes dynamic objects to prevent memory leaks.
        void DestroyNativeObjects()
        {
            if (network) { delete network; network = nullptr; }
            if (metrics) { delete metrics; metrics = nullptr; }
            if (vehicles) { delete vehicles; vehicles = nullptr; }
            if (signals) { delete signals; signals = nullptr; }
            if (paths) { delete paths; paths = nullptr; }
            if (pathIndex) { delete pathIndex; pathIndex = nullptr; }
            if (releaseStep) { delete releaseStep; releaseStep = nullptr; }
            if (startStep) { delete startStep; startStep = nullptr; }
            if (freeFlowTime) { delete freeFlowTime; freeFlowTime = nullptr; }
            if (launched) { delete launched; launched = nullptr; }
            if (visualTravelTotal) { delete visualTravelTotal; visualTravelTotal = nullptr; }
        }

        // This converts a normal C++ string into a .NET String for showing in the GUI.
        String^ ToSys(const std::string& s) { return gcnew String(s.c_str()); }

        // This function writes a message in the log box so we can see what happened at each step.
        void AddLog(String^ s)
        {
            // This adds the new log message with the current step number.
            txtLog->AppendText(L"[" + currentStep.ToString() + L"] " + s + Environment::NewLine);
            // This sets the progress position of the car along the road.
            txtLog->SelectionStart = txtLog->TextLength;
            // This automatically scrolls the log to the newest message.
            txtLog->ScrollToCaret();
        }

        // This function searches for a road between two nodes.
        Road* FindRoad(int fromNode, int toNode)
        {
            // This gets all roads from the road network class.
            const auto& allRoads = network->getAllRoads();
            // This sets the progress position of the car along the road.
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
            {
                Road* r = it->second;
                // This returns the road that matches the required condition.
                if (r->getFrom() == fromNode && r->getTo() == toNode) return r;
            }
            // This means no matching object was found.
            return nullptr;
        }

        // This function counts how many vehicles have completed their route.
        int CountArrived()
        {
            int c = 0;
            for (int i = 0; i < (int)vehicles->size(); i++)
                // This checks whether a vehicle has reached its destination.
                if ((*vehicles)[i].isArrived()) c++;
            return c;
        }

        // This function calculates the ideal travel time of a route without traffic delay.
        double ComputeFreeFlowTime(const std::vector<int>& path)
        {
            // This sets the progress position of the car along the road.
            double t = 0.0;
            for (int i = 0; i + 1 < (int)path.size(); i++)
            {
                Road* r = FindRoad(path[i], path[i + 1]);
                // This adds the normal road travel time to the route total.
                if (r) t += r->getFreeTravelTime();
            }
            return t;
        }

        // This function decides which route a newly added car should follow.
        std::vector<int> MakeSpawnPath(int src)
        {
            switch (src)
            {
                // This route is selected when the car starts from this source node.
            case 0: return std::vector<int>{0, 1, 4, 5};
                  // This route is selected when the car starts from this source node.
            case 1: return std::vector<int>{1, 4, 5};
                  // This route is selected when the car starts from this source node.
            case 2: return std::vector<int>{2, 5};
                  // This route is selected when the car starts from this source node.
            case 3: return std::vector<int>{3, 4, 5};
                  // This route is selected when the car starts from this source node.
            case 4: return std::vector<int>{4, 5};
            default: return std::vector<int>();
            }
        }

        // This function adds a new car from the selected starting node.
        void AddOneCarFromSource(int src)
        {
            std::vector<int> path = MakeSpawnPath(src);
            if (path.size() < 2) return;

            int id = (int)vehicles->size() + 1;
            // This sets the progress position of the car along the road.
            int dst = path.back();

            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(id, src, dst));
            // This stores the route/path that this vehicle will follow.
            paths->push_back(path);

            // This stores which part of the route the vehicle is currently on.
            pathIndex->push_back(1);
            // This stores when the vehicle actually started, used for travel time calculation.
            startStep->push_back(-1);
            // This stores ideal travel time, used later to calculate delay.
            freeFlowTime->push_back(ComputeFreeFlowTime(path));
            // This stores whether the vehicle has already entered the road network.
            launched->push_back(false);
            // This stores total visual travel time so the car can be drawn moving smoothly.
            visualTravelTotal->push_back(1);

            ReleaseVehicles();
            // This updates the tables, metrics, and visual display.
            RefreshAllViews();
            // This forces the map panel to redraw with updated car positions.
            panelMap->Invalidate();
            AddLog(L"Vehicle V" + id.ToString() + L" added from node " + src.ToString() + L".");
        }

        // This function sets the columns of the road and vehicle tables.
        void ConfigureTables()
        {
            // This initializes dgvRoads->DataSource as empty so the program does not use garbage memory.
            dgvRoads->DataSource = nullptr;
            // This initializes dgvVehicles->DataSource as empty so the program does not use garbage memory.
            dgvVehicles->DataSource = nullptr;
            dgvRoads->AutoGenerateColumns = false;
            dgvVehicles->AutoGenerateColumns = false;

            dgvRoads->Columns->Clear();
            // This adds a table column to show simulation information.
            dgvRoads->Columns->Add("road", "Road");
            // This adds a table column to show simulation information.
            dgvRoads->Columns->Add("flow", "Flow");
            // This adds a table column to show simulation information.
            dgvRoads->Columns->Add("queue", "Queue");
            // This adds a table column to show simulation information.
            dgvRoads->Columns->Add("signal", "Signal");

            dgvVehicles->Columns->Clear();
            // This adds a table column to show simulation information.
            dgvVehicles->Columns->Add("id", "Vehicle");
            // This adds a table column to show simulation information.
            dgvVehicles->Columns->Add("route", "Route");
            // This adds a table column to show simulation information.
            dgvVehicles->Columns->Add("state", "State");
            // This adds a table column to show simulation information.
            dgvVehicles->Columns->Add("road", "Road");
        }

        // This function resets everything and loads the demo network for the project.
        void LoadPresetScenario()
        {
            // This stops automatic running before resetting or finishing the simulation.
            simTimer->Stop();
            DestroyNativeObjects();
            CreateNativeObjects();

            currentStep = 0;
            presetLoaded = true;
            // This clears the old map drawing before drawing the new frame.
            txtLog->Clear();

            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(1, 0, 1, 180, 60, 5, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(2, 1, 2, 180, 60, 4, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(3, 0, 3, 180, 60, 5, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(4, 1, 4, 180, 60, 3, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(5, 2, 5, 180, 60, 4, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(6, 3, 4, 180, 60, 3, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(7, 4, 5, 180, 60, 3, 1);
            // This creates a road with id, from-node, to-node, length, speed, capacity, and lane data.
            network->addRoad(8, 3, 0, 180, 60, 2, 1);

            // This tells the signal which road is entering that intersection.
            TrafficSignal s0(0, 12, 0); s0.addIncomingRoad(8); signals->push_back(s0);
            // This tells the signal which road is entering that intersection.
            TrafficSignal s1(1, 12, 0); s1.addIncomingRoad(1); signals->push_back(s1);
            // This tells the signal which road is entering that intersection.
            TrafficSignal s2(2, 12, 0); s2.addIncomingRoad(2); signals->push_back(s2);
            // This tells the signal which road is entering that intersection.
            TrafficSignal s3(3, 12, 0); s3.addIncomingRoad(3); signals->push_back(s3);
            // This tells the signal which road is entering that intersection.
            TrafficSignal s4(4, 12, 1); s4.addIncomingRoad(4); s4.addIncomingRoad(6); signals->push_back(s4);
            // This tells the signal which road is entering that intersection.
            TrafficSignal s5(5, 12, 1); s5.addIncomingRoad(5); s5.addIncomingRoad(7); signals->push_back(s5);

            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(1, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(0);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(2, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(3);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(3, 0, 5)); paths->push_back(std::vector<int>{0, 1, 2, 5}); releaseStep->push_back(6);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(4, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(9);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(5, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(12);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(6, 3, 5)); paths->push_back(std::vector<int>{3, 0, 1, 2, 5}); releaseStep->push_back(15);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(7, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(18);
            // This adds a new vehicle object into the vehicle list.
            vehicles->push_back(Vehicle(8, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(21);

            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                // This stores which part of the route the vehicle is currently on.
                pathIndex->push_back(1);
                // This stores when the vehicle actually started, used for travel time calculation.
                startStep->push_back(-1);
                // This stores ideal travel time, used later to calculate delay.
                freeFlowTime->push_back(ComputeFreeFlowTime((*paths)[i]));
                // This stores whether the vehicle has already entered the road network.
                launched->push_back(false);
                // This stores total visual travel time so the car can be drawn moving smoothly.
                visualTravelTotal->push_back(1);
            }

            AddLog(L"Preset network loaded.");
            // This updates the tables, metrics, and visual display.
            RefreshAllViews();
        }

        // This function gets the number of vehicles waiting on a road.
        int GetRoadQueue(int roadId) { return network->getRoad(roadId)->getQueueSize(); }

        // This function updates each signal according to road queue sizes.
        void UpdateSignals()
        {
            for (int i = 0; i < (int)signals->size(); i++)
            {
                std::vector<int> q;
                // This gets the roads that enter the current signal/intersection.
                std::vector<int> in = (*signals)[i].getIncomingRoadIds();
                for (int j = 0; j < (int)in.size(); j++)
                    q.push_back(GetRoadQueue(in[j]));
                // This applies the signal logic from your TrafficSignal class.
                (*signals)[i].updateSignal(q);
            }
        }

        // This function checks whether a vehicle has green light at the next intersection.
        bool HasGreenAtNode(int nodeId, int incomingRoadId)
        {
            for (int i = 0; i < (int)signals->size(); i++)
                if ((*signals)[i].getIntersectionId() == nodeId)
                    // This checks if the selected incoming road currently has green signal.
                    return (*signals)[i].isGreen(incomingRoadId);
            return true;
        }

        // This function puts vehicles onto their first road when their release step arrives.
        void ReleaseVehicles()
        {
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                if ((*launched)[i] || (*releaseStep)[i] > currentStep || (*paths)[i].size() < 2) continue;

                Road* firstRoad = FindRoad((*paths)[i][0], (*paths)[i][1]);
                if (!firstRoad || firstRoad->getCurrentFlow() >= firstRoad->getCapacity()) continue;

                (*launched)[i] = true;
                (*startStep)[i] = currentStep;
                // This places the vehicle on its first road as a moving vehicle.
                firstRoad->addVehicle(&(*vehicles)[i]);
                // This sets the progress position of the car along the road.
                int tt = std::max(1, (int)firstRoad->getCurrentTravelTime());
                // This tells the vehicle which road it is entering and how much time it needs.
                (*vehicles)[i].enterEdge(firstRoad->getRoadID(), tt);
                (*visualTravelTotal)[i] = tt;
                AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" entered road " + firstRoad->getRoadID().ToString() + L".");
            }
        }

        // This function moves vehicles forward and stops them at red lights.
        void UpdateVehicleTimes()
        {
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                // This checks that only moving vehicles are updated here.
                if (!(*vehicles)[i].isMoving()) continue;

                Vehicle& v = (*vehicles)[i];
                // This gets the road on which the vehicle is currently moving.
                Road* currRoad = network->getRoad(v.getCurrentEdge());
                if (!currRoad) continue;

                int p = (*pathIndex)[i];
                int destNode = currRoad->getTo();

                if (p > 0 && p < (int)(*paths)[i].size())
                    destNode = (*paths)[i][p];

                // STOP-LINE RULE:
                // if vehicle is about to reach the intersection and signal is red,
                // stop it at the edge and place it in the waiting queue
                // This checks whether the car is close to the intersection/stop line.
                if (v.getRemainingTravelTime() <= 1)
                {
                    // This checks the signal before allowing the car to cross.
                    bool green = HasGreenAtNode(destNode, currRoad->getRoadID());

                    if (!green)
                    {
                        // This removes the car from the moving list of the road.
                        currRoad->removeVehicle(&v);
                        // This puts the car in the waiting queue at the signal.
                        currRoad->enqueueVehicle(&v);
                        // This changes the vehicle state to waiting at the red light/intersection.
                        v.waitAtIntersection();
                        // This skips the current loop case when the condition is not suitable.
                        continue;
                    }
                }

                // This reduces the vehicle travel time by one step, meaning the car moves forward.
                v.updateRemainingTime();

                // This checks if the car has reached the end of the current road.
                if (v.hasReachedIntersection())
                {
                    // This removes the car from the moving list of the road.
                    currRoad->removeVehicle(&v);
                    // This puts the car in the waiting queue at the signal.
                    currRoad->enqueueVehicle(&v);
                    // This changes the vehicle state to waiting at the red light/intersection.
                    v.waitAtIntersection();
                }
            }
        }

        // This function decides which waiting cars can move forward or arrive.
        int ProcessWaitingVehicles()
        {
            int arrivedThisStep = 0;

            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                if (!(*launched)[i] || !(*vehicles)[i].isWaiting()) continue;

                int p = (*pathIndex)[i];
                int pathLen = (int)(*paths)[i].size();
                if (p <= 0 || p >= pathLen) continue;

                Road* currRoad = FindRoad((*paths)[i][p - 1], (*paths)[i][p]);
                if (!currRoad) continue;

                if (p == pathLen - 1)
                {
                    // This removes the vehicle from the waiting queue when it is allowed to leave.
                    currRoad->dischargeVehicle(&(*vehicles)[i]);
                    // This marks the vehicle as arrived at its destination.
                    (*vehicles)[i].arrive();
                    // This sends travel time and delay data to the metrics collector.
                    metrics->recordArrival(currentStep - (*startStep)[i], (*freeFlowTime)[i]);
                    arrivedThisStep++;
                    AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" arrived.");
                    // This skips the current loop case when the condition is not suitable.
                    continue;
                }

                Road* nextRoad = FindRoad((*paths)[i][p], (*paths)[i][p + 1]);
                if (!nextRoad) continue;

                // This checks the signal before allowing the car to cross.
                bool green = HasGreenAtNode((*paths)[i][p], currRoad->getRoadID());
                // This uses your Road class logic to decide if a waiting car may leave.
                int allowed = currRoad->calculateAllowedDischarge(green, nextRoad->getCurrentFlow(), nextRoad->getCapacity());

                if (allowed > 0 && nextRoad->getCurrentFlow() < nextRoad->getCapacity())
                {
                    // This removes the vehicle from the waiting queue when it is allowed to leave.
                    currRoad->dischargeVehicle(&(*vehicles)[i]);
                    nextRoad->addVehicle(&(*vehicles)[i]);
                    // This sets the progress position of the car along the road.
                    int tt = std::max(1, (int)nextRoad->getCurrentTravelTime());
                    // This tells the vehicle which road it is entering and how much time it needs.
                    (*vehicles)[i].enterEdge(nextRoad->getRoadID(), tt);
                    (*visualTravelTotal)[i] = tt;
                    (*pathIndex)[i]++;
                    AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" entered road " + nextRoad->getRoadID().ToString() + L".");
                }
                else
                {
                    // This changes the vehicle state to waiting at the red light/intersection.
                    (*vehicles)[i].waitAtIntersection();
                }
            }

            return arrivedThisStep;
        }

        // This updates road travel times based on current congestion.
        void UpdateRoadTravelTimes()
        {
            // This gets all roads from the road network class.
            const auto& allRoads = network->getAllRoads();
            // This sets the progress position of the car along the road.
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
                // This recalculates travel time for this road.
                it->second->updateTravelTime();
        }

        // This is the main one-step function; the Step button and timer both call this.
        void StepSimulation()
        {
            if (!presetLoaded) return;

            // This increases the simulation time by one step.
            currentStep++;
            UpdateSignals();
            ReleaseVehicles();
            UpdateVehicleTimes();
            int arrivedThisStep = ProcessWaitingVehicles();
            UpdateRoadTravelTimes();
            // This records congestion, throughput, and other metrics for this step.
            metrics->recordStepMetrics(*network, arrivedThisStep);

            // This updates the tables, metrics, and visual display.
            RefreshAllViews();
            // This forces the map panel to redraw with updated car positions.
            panelMap->Invalidate();

            // This checks if all vehicles have reached the destination.
            if (CountArrived() == (int)vehicles->size())
            {
                // This stops automatic running before resetting or finishing the simulation.
                simTimer->Stop();
                AddLog(L"Simulation completed.");
            }
        }

        // This function updates the road table with flow, queue, and signal values.
        void RefreshRoadGrid()
        {
            // This clears old table rows before adding updated data.
            dgvRoads->Rows->Clear();
            // This gets all roads from the road network class.
            const auto& allRoads = network->getAllRoads();
            // This sets the progress position of the car along the road.
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
            {
                Road* r = it->second;
                String^ sig = L"-";
                for (int i = 0; i < (int)signals->size(); i++)
                    if ((*signals)[i].getIntersectionId() == r->getTo())
                        // This checks if the selected incoming road currently has green signal.
                        sig = (*signals)[i].isGreen(r->getRoadID()) ? L"GREEN" : L"RED";

                // This adds a new row in the table.
                int row = dgvRoads->Rows->Add();
                // This fills a table cell with updated simulation information.
                dgvRoads->Rows[row]->Cells[0]->Value = r->getFrom().ToString() + L"->" + r->getTo().ToString();
                // This fills a table cell with updated simulation information.
                dgvRoads->Rows[row]->Cells[1]->Value = r->getCurrentFlow().ToString();
                // This fills a table cell with updated simulation information.
                dgvRoads->Rows[row]->Cells[2]->Value = r->getQueueSize().ToString();
                // This fills a table cell with updated simulation information.
                dgvRoads->Rows[row]->Cells[3]->Value = sig;
            }
        }

        // This converts a vehicle route into readable text like 0-1-4-5.
        String^ PathToString(int idx)
        {
            String^ s = L"";
            for (int i = 0; i < (int)(*paths)[idx].size(); i++)
            {
                // This adds one line of information to the final report text.
                if (i > 0) s += L"-";
                // This adds one line of information to the final report text.
                s += (*paths)[idx][i].ToString();
            }
            return s;
        }

        // This function updates the vehicle table with route, status, and road number.
        void RefreshVehicleGrid()
        {
            // This clears old table rows before adding updated data.
            dgvVehicles->Rows->Clear();
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                // This gets the road on which the vehicle is currently moving.
                String^ road = (*vehicles)[i].getCurrentEdge() < 0 ? L"-" : (*vehicles)[i].getCurrentEdge().ToString();
                // This adds a new row in the table.
                int row = dgvVehicles->Rows->Add();
                // This fills a table cell with updated simulation information.
                dgvVehicles->Rows[row]->Cells[0]->Value = L"V" + (*vehicles)[i].getVehicleId().ToString();
                // This fills a table cell with updated simulation information.
                dgvVehicles->Rows[row]->Cells[1]->Value = PathToString(i);
                // This fills a table cell with updated simulation information.
                dgvVehicles->Rows[row]->Cells[2]->Value = ToSys((*vehicles)[i].getStatus());
                // This fills a table cell with updated simulation information.
                dgvVehicles->Rows[row]->Cells[3]->Value = road;
            }
        }

        // This function updates the metric labels shown on the right panel.
        void RefreshMetrics()
        {
            // This sets the text shown on this control.
            lblStepValue->Text = currentStep.ToString();
            // This sets the text shown on this control.
            lblArrivedValue->Text = metrics->getTotalArrived().ToString() + L"/" + vehicles->size().ToString();
            // This sets the text shown on this control.
            lblAvgTravelValue->Text = metrics->getAverageTravelTime().ToString("F2");
            // This sets the text shown on this control.
            lblDelayValue->Text = metrics->getTotalDelay().ToString("F0");
            // This sets the text shown on this control.
            lblThroughputValue->Text = metrics->getThroughput().ToString("F3");
            // This sets the text shown on this control.
            lblCongestionValue->Text = (metrics->getAverageCongestion() * 100.0).ToString("F1") + L"%";
            // This sets the text shown on this control.
            lblObjectiveValue->Text = metrics->computeObjectiveFunction(*network).ToString("F2");
        }

        // This function refreshes all GUI views together after each simulation change.
        void RefreshAllViews()
        {
            RefreshRoadGrid();
            RefreshVehicleGrid();
            RefreshMetrics();
        }

        // This function calculates the exact screen position where a car should be drawn.
        PointF GetVehiclePoint(int idx)
        {
            Vehicle& v = (*vehicles)[idx];
            // This gets the road on which the vehicle is currently moving.
            if (v.getCurrentEdge() < 0) return PointF(0.0f, 0.0f);

            // This gets the road on which the vehicle is currently moving.
            Road* r = network->getRoad(v.getCurrentEdge());
            // This gives a node its x and y screen position for drawing.
            Point a = nodePositions[r->getFrom()];
            // This gives a node its x and y screen position for drawing.
            Point b = nodePositions[r->getTo()];

            // This calculates the road direction in x and y.
            float dx = (float)(b.X - a.X), dy = (float)(b.Y - a.Y);
            // This calculates the road length on the screen.
            float len = (float)Math::Sqrt(dx * dx + dy * dy);
            if (len < 1.0f) len = 1.0f;

            // normal vector gives a visible lane offset, so multiple cars can exist on one edge
            // This creates a side offset so cars do not overlap directly on the road.
            float nx = -(dy / len), ny = (dx / len);
            int rank = GetVehicleRankOnRoad(r, &v, v.isWaiting());
            // This changes lane spacing so multiple cars can be seen on one road.
            float lane = 10.0f + (rank % 3) * 9.0f;

            float t;
            if (v.isWaiting())
            {
                // red-light cars stay behind the stop line, not inside the node
                // This sets the progress position of the car along the road.
                t = 0.86f - 0.075f * rank;
                // This sets the progress position of the car along the road.
                if (t < 0.18f) t = 0.18f;
            }
            else
            {
                int total = (*visualTravelTotal)[idx];
                if (total < 1) total = 1;
                // This checks whether the car is close to the intersection/stop line.
                t = 1.0f - ((float)v.getRemainingTravelTime() / (float)total);
                // This sets the progress position of the car along the road.
                if (t < 0.08f) t = 0.08f;
                // This sets the progress position of the car along the road.
                if (t > 0.78f) t = 0.78f; // keeps car before stop line
                t -= 0.055f * rank;
                // This sets the progress position of the car along the road.
                if (t < 0.08f) t = 0.08f;
            }

            // This returns the final x and y point where the car will be drawn.
            return PointF((float)a.X + dx * t + nx * lane,
                (float)a.Y + dy * t + ny * lane);
        }

        // This function finds the order of a car on a road so cars can be spaced properly.
        int GetVehicleRankOnRoad(Road* r, Vehicle* v, bool waiting)
        {
            // This sets the progress position of the car along the road.
            const std::vector<Vehicle*>& list = waiting ? r->getWaitingQueue() : r->getMovingVehicles();
            for (int i = 0; i < (int)list.size(); i++)
                if (list[i] == v) return i;
            return 0;
        }

        // This function chooses road color based on congestion level.
        Color GetRoadColor(Road* r)
        {
            // This gets the congestion value from the Road class.
            double c = r->getCongestion();
            if (c > 0.66) return Color::Red;
            if (c > 0.33) return Color::Orange;
            return Color::Green;
        }

        // This creates a traffic signal object for an intersection.
        void DrawSignal(Graphics^ g, TrafficSignal& s)
        {
            // This gives a node its x and y screen position for drawing.
            Point p = nodePositions[s.getIntersectionId()];
            // This gets the roads that enter the current signal/intersection.
            std::vector<int> incoming = s.getIncomingRoadIds();

            for (int i = 0; i < (int)incoming.size(); i++)
            {
                Road* inRoad = network->getRoad(incoming[i]);
                if (!inRoad) continue;

                // This gives a node its x and y screen position for drawing.
                Point a = nodePositions[inRoad->getFrom()];
                // This gives a node its x and y screen position for drawing.
                Point b = nodePositions[inRoad->getTo()];
                // This calculates the road direction in x and y.
                float dx = (float)(b.X - a.X), dy = (float)(b.Y - a.Y);
                // This calculates the road length on the screen.
                float len = (float)Math::Sqrt(dx * dx + dy * dy);
                if (len < 1.0f) len = 1.0f;
                float ux = dx / len, uy = dy / len;
                // This creates a side offset so cars do not overlap directly on the road.
                float nx = -uy, ny = ux;

                // Stop-line segment: this is the red/green bar before each intersection.
                // This calculates the stop-line position before the intersection.
                PointF c((float)b.X - ux * 28.0f, (float)b.Y - uy * 28.0f);
                // This creates a new Windows Forms control or .NET object.
                Pen^ stopPen = gcnew Pen(s.isGreen(incoming[i]) ? Color::LimeGreen : Color::Red, 5);
                // This creates the red/green stop-line pen.
                g->DrawLine(stopPen,
                    PointF(c.X - nx * 15.0f, c.Y - ny * 15.0f),
                    PointF(c.X + nx * 15.0f, c.Y + ny * 15.0f));
                // This creates the red/green stop-line pen.
                delete stopPen;

                // This checks if the selected incoming road currently has green signal.
                Brush^ bulb = s.isGreen(incoming[i]) ? Brushes::LimeGreen : Brushes::Red;
                // This draws a filled circle, used for nodes, queues, or signal lights.
                g->FillEllipse(bulb, Rectangle((int)c.X - 7, (int)c.Y - 22, 14, 14));
                // This draws the border of a circle.
                g->DrawEllipse(Pens::Black, Rectangle((int)c.X - 7, (int)c.Y - 22, 14, 14));
            }
        }

        // This function draws one road with color and road label.
        void DrawRoad(Graphics^ g, Road* r)
        {
            // This gives a node its x and y screen position for drawing.
            Point a = nodePositions[r->getFrom()];
            // This gives a node its x and y screen position for drawing.
            Point b = nodePositions[r->getTo()];

            // This creates a new Windows Forms control or .NET object.
            Pen^ shadow = gcnew Pen(Color::FromArgb(210, 215, 220), 13);
            // This makes road ends rounded for a cleaner GUI look.
            shadow->StartCap = Drawing::Drawing2D::LineCap::Round;
            // This makes road ends rounded for a cleaner GUI look.
            shadow->EndCap = Drawing::Drawing2D::LineCap::Round;
            // This draws a line on the map, used for roads or stop-line segments.
            g->DrawLine(shadow, a, b);
            delete shadow;

            // This creates a new Windows Forms control or .NET object.
            Pen^ roadPen = gcnew Pen(GetRoadColor(r), 7);
            // This makes road ends rounded for a cleaner GUI look.
            roadPen->StartCap = Drawing::Drawing2D::LineCap::Round;
            // This makes road ends rounded for a cleaner GUI look.
            roadPen->EndCap = Drawing::Drawing2D::LineCap::Round;
            // This draws a line on the map, used for roads or stop-line segments.
            g->DrawLine(roadPen, a, b);
            delete roadPen;

            int mx = (a.X + b.X) / 2, my = (a.Y + b.Y) / 2;
            // This writes labels such as road id, node id, or vehicle id on the map.
            g->DrawString(L"R" + r->getRoadID().ToString(), gcnew Drawing::Font(L"Segoe UI", 8, FontStyle::Bold), Brushes::Black, (float)mx - 8, (float)my - 18);

            int q = r->getQueueSize();
            if (q > 0)
            {
                // This draws a filled circle, used for nodes, queues, or signal lights.
                g->FillEllipse(Brushes::Orange, Rectangle(mx - 12, my + 3, 24, 18));
                // This writes labels such as road id, node id, or vehicle id on the map.
                g->DrawString(q.ToString(), gcnew Drawing::Font(L"Segoe UI", 8, FontStyle::Bold), Brushes::Black, (float)mx - 4, (float)my + 3);
            }
        }

        // This function draws an intersection/node on the map.
        void DrawNode(Graphics^ g, int id, Point p)
        {
            // This draws a filled circle, used for nodes, queues, or signal lights.
            g->FillEllipse(Brushes::White, Rectangle(p.X - 18, p.Y - 18, 36, 36));
            // This creates a new Windows Forms control or .NET object.
            Pen^ nodePen = gcnew Pen(Color::FromArgb(40, 80, 120), 2);
            // This draws the border of a circle.
            g->DrawEllipse(nodePen, Rectangle(p.X - 18, p.Y - 18, 36, 36));
            delete nodePen;
            // This writes labels such as road id, node id, or vehicle id on the map.
            g->DrawString(id.ToString(), gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold), Brushes::Black, (float)p.X - 6, (float)p.Y - 9);
        }

        // This function draws one vehicle on the map.
        void DrawCar(Graphics^ g, int idx)
        {
            // This sets the progress position of the car along the road.
            PointF pt = GetVehiclePoint(idx);
            // This sets the progress position of the car along the road.
            bool wait = (*vehicles)[idx].isWaiting();
            Brush^ body = wait ? Brushes::OrangeRed : Brushes::RoyalBlue;
            // This creates the small rectangle shape used as the car body.
            Rectangle bodyRect((int)pt.X - 8, (int)pt.Y - 5, 16, 10);
            // This creates the small rectangle shape used as the car body.
            g->FillRectangle(body, bodyRect);
            // This creates the small rectangle shape used as the car body.
            g->DrawRectangle(Pens::Black, bodyRect);
            // This writes labels such as road id, node id, or vehicle id on the map.
            g->DrawString(L"V" + (*vehicles)[idx].getVehicleId().ToString(), gcnew Drawing::Font(L"Segoe UI", 7, FontStyle::Bold), Brushes::Black, pt.X - 8, pt.Y - 18);
        }

        // This is the main drawing function for the road network map.
        void panelMap_Paint(System::Object^, PaintEventArgs^ e)
        {
            // This safety check avoids drawing before simulation objects are created.
            if (!network || !vehicles || !signals) return;

            Graphics^ g = e->Graphics;
            // This makes the drawing smoother and less rough.
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
            // This clears the old map drawing before drawing the new frame.
            g->Clear(Color::FromArgb(248, 250, 252));

            // This creates a new Windows Forms control or .NET object.
            System::Drawing::Font^ titleFont = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            // This writes labels such as road id, node id, or vehicle id on the map.
            g->DrawString(L"Live Road Network  |  Green = clear, Orange = medium, Red = congested/stop", titleFont, Brushes::DimGray, 20.0f, 20.0f);

            // This gets all roads from the road network class.
            const auto& allRoads = network->getAllRoads();
            // This sets the progress position of the car along the road.
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it) DrawRoad(g, it->second);

            for (int i = 0; i < (int)signals->size(); i++) DrawSignal(g, (*signals)[i]);

            for each(System::Collections::Generic::KeyValuePair<int, Point> kv in nodePositions)
                DrawNode(g, kv.Key, kv.Value);

            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                // This checks whether a vehicle has reached its destination.
                if (!(*launched)[i] || (*vehicles)[i].isArrived()) continue;
                DrawCar(g, i);
            }
        }

        // This function creates the final text report from the simulation metrics.
        String^ BuildFinalReport()
        {
            String^ s = L"FINAL REPORT\r\n\r\n";
            // This adds one line of information to the final report text.
            s += L"Step: " + currentStep.ToString() + L"\r\n";
            // This adds one line of information to the final report text.
            s += L"Arrived: " + metrics->getTotalArrived().ToString() + L"/" + vehicles->size().ToString() + L"\r\n";
            // This adds one line of information to the final report text.
            s += L"Avg Travel Time: " + metrics->getAverageTravelTime().ToString("F2") + L" steps\r\n";
            // This adds one line of information to the final report text.
            s += L"Total Delay: " + metrics->getTotalDelay().ToString("F0") + L"\r\n";
            // This adds one line of information to the final report text.
            s += L"Throughput: " + metrics->getThroughput().ToString("F3") + L"\r\n";
            // This adds one line of information to the final report text.
            s += L"Avg Network Congestion: " + (metrics->getAverageCongestion() * 100.0).ToString("F1") + L"%\r\n";
            // This adds one line of information to the final report text.
            s += L"Objective: " + metrics->computeObjectiveFunction(*network).ToString("F2") + L"\r\n";
            return s;
        }

        // This is an event function that runs when the related button/timer is used.
        void btnLoadPreset_Click(System::Object^, System::EventArgs^) { LoadPresetScenario(); }

        // This is an event function that runs when the related button/timer is used.
        void btnStart_Click(System::Object^, System::EventArgs^)
        {
            if (!presetLoaded) LoadPresetScenario();

            if (chkAutoStep->Checked)
            {
                // This stops automatic running before resetting or finishing the simulation.
                if (simTimer->Enabled) { simTimer->Stop(); AddLog(L"Auto run paused."); }
                else { simTimer->Start(); AddLog(L"Auto run started."); }
            }
            else
            {
                int n = (int)numSteps->Value;
                // This runs one complete simulation step.
                for (int i = 0; i < n; i++) StepSimulation();
            }
        }

        // This is an event function that runs when the related button/timer is used.
        void btnStep_Click(System::Object^, System::EventArgs^) { StepSimulation(); }
        // This is an event function that runs when the related button/timer is used.
        void btnReset_Click(System::Object^, System::EventArgs^) { LoadPresetScenario(); }
        // This is an event function that runs when the related button/timer is used.
        void btnGenerateReport_Click(System::Object^, System::EventArgs^) { MessageBox::Show(BuildFinalReport(), L"Final Simulation Report", MessageBoxButtons::OK, MessageBoxIcon::Information); }
        // This is an event function that runs when the related button/timer is used.
        void btnAddCar_Click(System::Object^, System::EventArgs^) { AddOneCarFromSource(Int32::Parse(cmbSpawnNode->Text)); }
        // This is an event function that runs when the related button/timer is used.
        void simTimer_Tick(System::Object^, System::EventArgs^) { StepSimulation(); }
    };

}