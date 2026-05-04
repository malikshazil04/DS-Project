/*
    MyForm_stopline_fixed.h
    Backend stays on your real headers.
    Fix added:
    - vehicles stop at the edge / stop line when signal is red
    - waiting vehicles queue before the node
    - more than one car can be shown on the same edge
*/

#pragma once

#include "RoadNetwork.h"
#include "TrafficSignal.h"
#include "Vehicle.h"
#include "MetricsCollector.h"

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace TrafficFlowGUI {

    public ref class MyForm : public Form
    {
    public:
        MyForm()
        {
            network = nullptr;
            metrics = nullptr;
            vehicles = nullptr;
            signals = nullptr;
            paths = nullptr;
            pathIndex = nullptr;
            releaseStep = nullptr;
            startStep = nullptr;
            freeFlowTime = nullptr;
            launched = nullptr;
            visualTravelTotal = nullptr;

            InitializeComponent();
            ConfigureTables();
            BuildNodePositions();
            LoadPresetScenario();
            RefreshAllViews();
        }

    protected:
        ~MyForm()
        {
            if (simTimer != nullptr) simTimer->Stop();
            DestroyNativeObjects();
            if (components) delete components;
        }

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

        void InitializeComponent()
        {
            components = gcnew System::ComponentModel::Container();
            label1 = gcnew Label();
            panelLeft = gcnew Panel();
            label3 = gcnew Label();
            numSteps = gcnew NumericUpDown();
            chkAutoStep = gcnew CheckBox();
            cmbSpawnNode = gcnew ComboBox();
            btnAddCar = gcnew Button();
            btnReset = gcnew Button();
            btnStep = gcnew Button();
            btnStart = gcnew Button();
            btnLoadPreset = gcnew Button();
            panelMap = gcnew Panel();
            panelBottom = gcnew Panel();
            panelRight = gcnew Panel();
            dgvVehicles = gcnew DataGridView();
            btnGenerateReport = gcnew Button();
            dgvRoads = gcnew DataGridView();
            grpMetrics = gcnew GroupBox();
            lblObjectiveValue = gcnew Label();
            lblCongestionValue = gcnew Label();
            lblThroughputValue = gcnew Label();
            lblDelayValue = gcnew Label();
            lblAvgTravelValue = gcnew Label();
            lblArrivedValue = gcnew Label();
            lblStepValue = gcnew Label();
            txtLog = gcnew RichTextBox();
            simTimer = gcnew Timer(components);

            SuspendLayout();

            label1->Dock = DockStyle::Top;
            label1->Text = L"Traffic Flow Optimization System";
            label1->Height = 40;
            label1->TextAlign = ContentAlignment::MiddleCenter;
            label1->Font = gcnew Drawing::Font(L"Segoe UI", 15, FontStyle::Bold);
            label1->ForeColor = Color::White;
            label1->BackColor = Color::FromArgb(18, 30, 45);

            panelLeft->Dock = DockStyle::Left;
            panelLeft->Width = 220;
            panelLeft->BackColor = Color::FromArgb(232, 238, 245);
            panelLeft->Padding = System::Windows::Forms::Padding(10);

            btnLoadPreset->Text = L"Load Preset";
            btnLoadPreset->SetBounds(12, 10, 180, 34);
            btnLoadPreset->BackColor = Color::LightBlue;
            btnLoadPreset->FlatStyle = FlatStyle::Flat;
            btnLoadPreset->Click += gcnew EventHandler(this, &MyForm::btnLoadPreset_Click);

            btnStart->Text = L"Start / Run";
            btnStart->SetBounds(12, 50, 180, 34);
            btnStart->BackColor = Color::LightGreen;
            btnStart->FlatStyle = FlatStyle::Flat;
            btnStart->Click += gcnew EventHandler(this, &MyForm::btnStart_Click);

            btnStep->Text = L"Step";
            btnStep->SetBounds(12, 90, 180, 90);
            btnStep->BackColor = Color::LightYellow;
            btnStep->FlatStyle = FlatStyle::Flat;
            btnStep->Click += gcnew EventHandler(this, &MyForm::btnStep_Click);

            btnReset->Text = L"Reset";
            btnReset->SetBounds(12, 190, 180, 90);
            btnReset->BackColor = Color::LightCoral;
            btnReset->FlatStyle = FlatStyle::Flat;
            btnReset->Click += gcnew EventHandler(this, &MyForm::btnReset_Click);

            label3->Text = L"Steps to run";
            label3->SetBounds(12, 300, 100, 20);

            numSteps->SetBounds(129, 298, 60, 26);
            numSteps->Minimum = 1;
            numSteps->Maximum = 1000;
            numSteps->Value = 20;

            chkAutoStep->Text = L"Auto Step";
            chkAutoStep->SetBounds(12, 335, 120, 24);
            chkAutoStep->Checked = true;

            cmbSpawnNode->SetBounds(12, 370, 80, 26);
            cmbSpawnNode->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbSpawnNode->Items->AddRange(gcnew cli::array<Object^>{ L"0", L"1", L"2", L"3", L"4" });
            cmbSpawnNode->SelectedIndex = 0;

            btnAddCar->Text = L"Add Car";
            btnAddCar->SetBounds(100, 368, 92, 30);
            btnAddCar->BackColor = Color::LightSkyBlue;
            btnAddCar->FlatStyle = FlatStyle::Flat;
            btnAddCar->Click += gcnew EventHandler(this, &MyForm::btnAddCar_Click);

            panelLeft->Controls->Add(btnLoadPreset);
            panelLeft->Controls->Add(btnStart);
            panelLeft->Controls->Add(btnStep);
            panelLeft->Controls->Add(btnReset);
            panelLeft->Controls->Add(label3);
            panelLeft->Controls->Add(numSteps);
            panelLeft->Controls->Add(chkAutoStep);
            panelLeft->Controls->Add(cmbSpawnNode);
            panelLeft->Controls->Add(btnAddCar);

            panelMap->Dock = DockStyle::Fill;
            panelMap->BackColor = Color::FromArgb(248, 250, 252);
            panelMap->Paint += gcnew PaintEventHandler(this, &MyForm::panelMap_Paint);

            panelRight->Dock = DockStyle::Right;
            panelRight->Width = 320;
            panelRight->BackColor = Color::FromArgb(238, 243, 248);
            panelRight->BorderStyle = BorderStyle::FixedSingle;

            grpMetrics->Dock = DockStyle::Top;
            grpMetrics->Text = L"Performance Metrics";
            grpMetrics->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
            grpMetrics->Height = 195;

            AddMetricLabel(grpMetrics, L"Step:", 10, 25);        lblStepValue = AddMetricValue(grpMetrics, L"0", 150, 25);
            AddMetricLabel(grpMetrics, L"Arrived:", 10, 48);     lblArrivedValue = AddMetricValue(grpMetrics, L"0/0", 150, 48);
            AddMetricLabel(grpMetrics, L"Avg Travel:", 10, 71);  lblAvgTravelValue = AddMetricValue(grpMetrics, L"0.0", 150, 71);
            AddMetricLabel(grpMetrics, L"Total Delay:", 10, 94); lblDelayValue = AddMetricValue(grpMetrics, L"0", 150, 94);
            AddMetricLabel(grpMetrics, L"Throughput:", 10, 117); lblThroughputValue = AddMetricValue(grpMetrics, L"0.00", 150, 117);
            AddMetricLabel(grpMetrics, L"Avg Congestion:", 10, 140); lblCongestionValue = AddMetricValue(grpMetrics, L"0%", 150, 140);
            AddMetricLabel(grpMetrics, L"Objective:", 10, 163);  lblObjectiveValue = AddMetricValue(grpMetrics, L"0.00", 150, 163);

            dgvRoads->Dock = DockStyle::Top;
            dgvRoads->Height = 180;
            dgvRoads->AllowUserToAddRows = false;
            dgvRoads->ReadOnly = true;
            dgvRoads->RowHeadersVisible = false;
            dgvRoads->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            dgvVehicles->Dock = DockStyle::Fill;
            dgvVehicles->AllowUserToAddRows = false;
            dgvVehicles->ReadOnly = true;
            dgvVehicles->RowHeadersVisible = false;
            dgvVehicles->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;

            btnGenerateReport->Text = L"Generate Final Report";
            btnGenerateReport->Dock = DockStyle::Bottom;
            btnGenerateReport->Height = 32;
            btnGenerateReport->BackColor = Color::LightSteelBlue;
            btnGenerateReport->FlatStyle = FlatStyle::Flat;
            btnGenerateReport->Click += gcnew EventHandler(this, &MyForm::btnGenerateReport_Click);

            panelRight->Controls->Add(dgvVehicles);
            panelRight->Controls->Add(btnGenerateReport);
            panelRight->Controls->Add(dgvRoads);
            panelRight->Controls->Add(grpMetrics);

            panelBottom->Dock = DockStyle::Bottom;
            panelBottom->Height = 128;
            panelBottom->BackColor = Color::FromArgb(245, 245, 245);
            panelBottom->BorderStyle = BorderStyle::FixedSingle;

            txtLog->Dock = DockStyle::Fill;
            txtLog->ReadOnly = true;
            txtLog->BackColor = Color::FromArgb(20, 28, 38);
            txtLog->ForeColor = Color::Gainsboro;
            txtLog->Font = gcnew Drawing::Font(L"Consolas", 9);
            panelBottom->Controls->Add(txtLog);

            panelMap->Controls->Add(panelBottom);
            panelMap->Controls->Add(panelRight);

            simTimer->Interval = 800;
            simTimer->Tick += gcnew EventHandler(this, &MyForm::simTimer_Tick);

            ClientSize = Drawing::Size(1273, 660);
            Controls->Add(panelMap);
            Controls->Add(panelLeft);
            Controls->Add(label1);
            Text = L"Traffic Flow Optimization System";
            WindowState = FormWindowState::Maximized;
            ResumeLayout(false);
        }

        Label^ AddMetricLabel(Control^ parent, String^ text, int x, int y)
        {
            Label^ lbl = gcnew Label();
            lbl->Text = text;
            lbl->Location = Point(x, y);
            lbl->AutoSize = true;
            lbl->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            parent->Controls->Add(lbl);
            return lbl;
        }

        Label^ AddMetricValue(Control^ parent, String^ text, int x, int y)
        {
            Label^ lbl = gcnew Label();
            lbl->Text = text;
            lbl->Location = Point(x, y);
            lbl->AutoSize = true;
            lbl->Font = gcnew Drawing::Font(L"Segoe UI", 9);
            parent->Controls->Add(lbl);
            return lbl;
        }

        void BuildNodePositions()
        {
            nodePositions = gcnew System::Collections::Generic::Dictionary<int, Point>();
            nodePositions[0] = Point(120, 180);
            nodePositions[1] = Point(300, 180);
            nodePositions[2] = Point(480, 180);
            nodePositions[3] = Point(120, 360);
            nodePositions[4] = Point(300, 360);
            nodePositions[5] = Point(480, 360);
        }

        void CreateNativeObjects()
        {
            network = new RoadNetwork();
            metrics = new MetricsCollector();
            vehicles = new std::vector<Vehicle>();
            signals = new std::vector<TrafficSignal>();
            paths = new std::vector<std::vector<int> >();
            pathIndex = new std::vector<int>();
            releaseStep = new std::vector<int>();
            startStep = new std::vector<int>();
            freeFlowTime = new std::vector<double>();
            launched = new std::vector<bool>();
            visualTravelTotal = new std::vector<int>();
        }

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

        String^ ToSys(const std::string& s) { return gcnew String(s.c_str()); }

        void AddLog(String^ s)
        {
            txtLog->AppendText(L"[" + currentStep.ToString() + L"] " + s + Environment::NewLine);
            txtLog->SelectionStart = txtLog->TextLength;
            txtLog->ScrollToCaret();
        }

        Road* FindRoad(int fromNode, int toNode)
        {
            const auto& allRoads = network->getAllRoads();
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
            {
                Road* r = it->second;
                if (r->getFrom() == fromNode && r->getTo() == toNode) return r;
            }
            return nullptr;
        }

        int CountArrived()
        {
            int c = 0;
            for (int i = 0; i < (int)vehicles->size(); i++)
                if ((*vehicles)[i].isArrived()) c++;
            return c;
        }

        double ComputeFreeFlowTime(const std::vector<int>& path)
        {
            double t = 0.0;
            for (int i = 0; i + 1 < (int)path.size(); i++)
            {
                Road* r = FindRoad(path[i], path[i + 1]);
                if (r) t += r->getFreeTravelTime();
            }
            return t;
        }

        std::vector<int> MakeSpawnPath(int src)
        {
            switch (src)
            {
            case 0: return std::vector<int>{0, 1, 4, 5};
            case 1: return std::vector<int>{1, 4, 5};
            case 2: return std::vector<int>{2, 5};
            case 3: return std::vector<int>{3, 4, 5};
            case 4: return std::vector<int>{4, 5};
            default: return std::vector<int>();
            }
        }

        void AddOneCarFromSource(int src)
        {
            std::vector<int> path = MakeSpawnPath(src);
            if (path.size() < 2) return;

            int id = (int)vehicles->size() + 1;
            int dst = path.back();

            vehicles->push_back(Vehicle(id, src, dst));
            paths->push_back(path);
           
            pathIndex->push_back(1);
            startStep->push_back(-1);
            freeFlowTime->push_back(ComputeFreeFlowTime(path));
            launched->push_back(false);
            visualTravelTotal->push_back(1);

            ReleaseVehicles();
            RefreshAllViews();
            panelMap->Invalidate();
            AddLog(L"Vehicle V" + id.ToString() + L" added from node " + src.ToString() + L".");
        }

        void ConfigureTables()
        {
            dgvRoads->DataSource = nullptr;
            dgvVehicles->DataSource = nullptr;
            dgvRoads->AutoGenerateColumns = false;
            dgvVehicles->AutoGenerateColumns = false;

            dgvRoads->Columns->Clear();
            dgvRoads->Columns->Add("road", "Road");
            dgvRoads->Columns->Add("flow", "Flow");
            dgvRoads->Columns->Add("queue", "Queue");
            dgvRoads->Columns->Add("signal", "Signal");

            dgvVehicles->Columns->Clear();
            dgvVehicles->Columns->Add("id", "Vehicle");
            dgvVehicles->Columns->Add("route", "Route");
            dgvVehicles->Columns->Add("state", "State");
            dgvVehicles->Columns->Add("road", "Road");
        }

        void LoadPresetScenario()
        {
            simTimer->Stop();
            DestroyNativeObjects();
            CreateNativeObjects();

            currentStep = 0;
            presetLoaded = true;
            txtLog->Clear();

            network->addRoad(1, 0, 1, 180, 60, 5, 1);
            network->addRoad(2, 1, 2, 180, 60, 4, 1);
            network->addRoad(3, 0, 3, 180, 60, 5, 1);
            network->addRoad(4, 1, 4, 180, 60, 3, 1);
            network->addRoad(5, 2, 5, 180, 60, 4, 1);
            network->addRoad(6, 3, 4, 180, 60, 3, 1);
            network->addRoad(7, 4, 5, 180, 60, 3, 1);
            network->addRoad(8, 3, 0, 180, 60, 2, 1);

            TrafficSignal s0(0, 12, 0); s0.addIncomingRoad(8); signals->push_back(s0);
            TrafficSignal s1(1, 12, 0); s1.addIncomingRoad(1); signals->push_back(s1);
            TrafficSignal s2(2, 12, 0); s2.addIncomingRoad(2); signals->push_back(s2);
            TrafficSignal s3(3, 12, 0); s3.addIncomingRoad(3); signals->push_back(s3);
            TrafficSignal s4(4, 12, 1); s4.addIncomingRoad(4); s4.addIncomingRoad(6); signals->push_back(s4);
            TrafficSignal s5(5, 12, 1); s5.addIncomingRoad(5); s5.addIncomingRoad(7); signals->push_back(s5);

            vehicles->push_back(Vehicle(1, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(0);
            vehicles->push_back(Vehicle(2, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(3);
            vehicles->push_back(Vehicle(3, 0, 5)); paths->push_back(std::vector<int>{0, 1, 2, 5}); releaseStep->push_back(6);
            vehicles->push_back(Vehicle(4, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(9);
            vehicles->push_back(Vehicle(5, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(12);
            vehicles->push_back(Vehicle(6, 3, 5)); paths->push_back(std::vector<int>{3, 0, 1, 2, 5}); releaseStep->push_back(15);
            vehicles->push_back(Vehicle(7, 0, 5)); paths->push_back(std::vector<int>{0, 1, 4, 5}); releaseStep->push_back(18);
            vehicles->push_back(Vehicle(8, 0, 5)); paths->push_back(std::vector<int>{0, 3, 4, 5}); releaseStep->push_back(21);

            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                pathIndex->push_back(1);
                startStep->push_back(-1);
                freeFlowTime->push_back(ComputeFreeFlowTime((*paths)[i]));
                launched->push_back(false);
                visualTravelTotal->push_back(1);
            }

            AddLog(L"Preset network loaded.");
            RefreshAllViews();
        }

        int GetRoadQueue(int roadId) { return network->getRoad(roadId)->getQueueSize(); }

        void UpdateSignals()
        {
            for (int i = 0; i < (int)signals->size(); i++)
            {
                std::vector<int> q;
                std::vector<int> in = (*signals)[i].getIncomingRoadIds();
                for (int j = 0; j < (int)in.size(); j++)
                    q.push_back(GetRoadQueue(in[j]));
                (*signals)[i].updateSignal(q);
            }
        }

        bool HasGreenAtNode(int nodeId, int incomingRoadId)
        {
            for (int i = 0; i < (int)signals->size(); i++)
                if ((*signals)[i].getIntersectionId() == nodeId)
                    return (*signals)[i].isGreen(incomingRoadId);
            return true;
        }

        void ReleaseVehicles()
        {
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                if ((*launched)[i] || (*releaseStep)[i] > currentStep || (*paths)[i].size() < 2) continue;

                Road* firstRoad = FindRoad((*paths)[i][0], (*paths)[i][1]);
                if (!firstRoad || firstRoad->getCurrentFlow() >= firstRoad->getCapacity()) continue;

                (*launched)[i] = true;
                (*startStep)[i] = currentStep;
                firstRoad->addVehicle(&(*vehicles)[i]);
                int tt = std::max(1, (int)firstRoad->getCurrentTravelTime());
                (*vehicles)[i].enterEdge(firstRoad->getRoadID(), tt);
                (*visualTravelTotal)[i] = tt;
                AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" entered road " + firstRoad->getRoadID().ToString() + L".");
            }
        }

        // FIXED: stop at edge / stop line when signal is red
        void UpdateVehicleTimes()
        {
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                if (!(*vehicles)[i].isMoving()) continue;

                Vehicle& v = (*vehicles)[i];
                Road* currRoad = network->getRoad(v.getCurrentEdge());
                if (!currRoad) continue;

                int p = (*pathIndex)[i];
                int destNode = currRoad->getTo();
                if (p > 0 && p < (int)(*paths)[i].size())
                    destNode = (*paths)[i][p];

                if (v.getRemainingTravelTime() <= 1)
                {
                    bool green = HasGreenAtNode(destNode, currRoad->getRoadID());

                    if (!green)
                    {
                        currRoad->removeVehicle(&v);
                        currRoad->enqueueVehicle(&v);
                        v.waitAtIntersection();
                        continue;
                    }
                }

                v.updateRemainingTime();

                if (v.hasReachedIntersection())
                {
                    currRoad->removeVehicle(&v);
                    currRoad->enqueueVehicle(&v);
                    v.waitAtIntersection();
                }
            }
        }

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
                    currRoad->dischargeVehicle(&(*vehicles)[i]);
                    (*vehicles)[i].arrive();
                    metrics->recordArrival(currentStep - (*startStep)[i], (*freeFlowTime)[i]);
                    arrivedThisStep++;
                    AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" arrived.");
                    continue;
                }

                Road* nextRoad = FindRoad((*paths)[i][p], (*paths)[i][p + 1]);
                if (!nextRoad) continue;

                bool green = HasGreenAtNode((*paths)[i][p], currRoad->getRoadID());
                int allowed = currRoad->calculateAllowedDischarge(green, nextRoad->getCurrentFlow(), nextRoad->getCapacity());

                if (allowed > 0 && nextRoad->getCurrentFlow() < nextRoad->getCapacity())
                {
                    currRoad->dischargeVehicle(&(*vehicles)[i]);
                    nextRoad->addVehicle(&(*vehicles)[i]);
                    int tt = std::max(1, (int)nextRoad->getCurrentTravelTime());
                    (*vehicles)[i].enterEdge(nextRoad->getRoadID(), tt);
                    (*visualTravelTotal)[i] = tt;
                    (*pathIndex)[i]++;
                    AddLog(L"Vehicle V" + (*vehicles)[i].getVehicleId().ToString() + L" entered road " + nextRoad->getRoadID().ToString() + L".");
                }
                else
                {
                    (*vehicles)[i].waitAtIntersection();
                }
            }

            return arrivedThisStep;
        }

        void UpdateRoadTravelTimes()
        {
            const auto& allRoads = network->getAllRoads();
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
                it->second->updateTravelTime();
        }

        void StepSimulation()
        {
            if (!presetLoaded) return;

            currentStep++;
            UpdateSignals();
            ReleaseVehicles();
            UpdateVehicleTimes();
            int arrivedThisStep = ProcessWaitingVehicles();
            UpdateRoadTravelTimes();
            metrics->recordStepMetrics(*network, arrivedThisStep);  //updates average travel time and delay.

            RefreshAllViews();
            panelMap->Invalidate();

            if (CountArrived() == (int)vehicles->size())
            {
                simTimer->Stop();
                AddLog(L"Simulation completed.");
            }
        }

        void RefreshRoadGrid()
        {
            dgvRoads->Rows->Clear();
            const auto& allRoads = network->getAllRoads();
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it)
            {
                Road* r = it->second;
                String^ sig = L"-";
                for (int i = 0; i < (int)signals->size(); i++)
                    if ((*signals)[i].getIntersectionId() == r->getTo())
                        sig = (*signals)[i].isGreen(r->getRoadID()) ? L"GREEN" : L"RED";

                int row = dgvRoads->Rows->Add();
                dgvRoads->Rows[row]->Cells[0]->Value = r->getFrom().ToString() + L"->" + r->getTo().ToString();
                dgvRoads->Rows[row]->Cells[1]->Value = r->getCurrentFlow().ToString();
                dgvRoads->Rows[row]->Cells[2]->Value = r->getQueueSize().ToString();
                dgvRoads->Rows[row]->Cells[3]->Value = sig;
            }
        }

        String^ PathToString(int idx)
        {
            String^ s = L"";
            for (int i = 0; i < (int)(*paths)[idx].size(); i++)
            {
                if (i > 0) s += L"-";
                s += (*paths)[idx][i].ToString();
            }
            return s;
        }

        void RefreshVehicleGrid()
        {
            dgvVehicles->Rows->Clear();
            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                String^ road = (*vehicles)[i].getCurrentEdge() < 0 ? L"-" : (*vehicles)[i].getCurrentEdge().ToString();
                int row = dgvVehicles->Rows->Add();
                dgvVehicles->Rows[row]->Cells[0]->Value = L"V" + (*vehicles)[i].getVehicleId().ToString();
                dgvVehicles->Rows[row]->Cells[1]->Value = PathToString(i);
                dgvVehicles->Rows[row]->Cells[2]->Value = ToSys((*vehicles)[i].getStatus());
                dgvVehicles->Rows[row]->Cells[3]->Value = road;
            }
        }

        void RefreshMetrics()
        {
            lblStepValue->Text = currentStep.ToString();
            lblArrivedValue->Text = metrics->getTotalArrived().ToString() + L"/" + vehicles->size().ToString();
            lblAvgTravelValue->Text = metrics->getAverageTravelTime().ToString("F2");
            lblDelayValue->Text = metrics->getTotalDelay().ToString("F0");
            lblThroughputValue->Text = metrics->getThroughput().ToString("F3");
            lblCongestionValue->Text = (metrics->getAverageCongestion() * 100.0).ToString("F1") + L"%";
            lblObjectiveValue->Text = metrics->computeObjectiveFunction(*network).ToString("F2");
        }

        void RefreshAllViews()
        {
            RefreshRoadGrid();
            RefreshVehicleGrid();
            RefreshMetrics();
        }

        int GetVehicleRankOnRoad(Road* r, Vehicle* v, bool waiting)
        {
            const std::vector<Vehicle*>& list = waiting ? r->getWaitingQueue() : r->getMovingVehicles();
            for (int i = 0; i < (int)list.size(); i++)
                if (list[i] == v) return i;
            return 0;
        }

        // FIXED: queue before node and multiple cars on same edge
        PointF GetVehiclePoint(int idx)
        {
            Vehicle& v = (*vehicles)[idx];
            if (v.getCurrentEdge() < 0) return PointF(0.0f, 0.0f);

            Road* r = network->getRoad(v.getCurrentEdge());
            Point a = nodePositions[r->getFrom()];
            Point b = nodePositions[r->getTo()];

            float dx = (float)(b.X - a.X), dy = (float)(b.Y - a.Y);
            float len = (float)Math::Sqrt(dx * dx + dy * dy);
            if (len < 1.0f) len = 1.0f;

            float nx = -(dy / len), ny = (dx / len);
            int rank = GetVehicleRankOnRoad(r, &v, v.isWaiting());
            float lane = 10.0f + (rank % 3) * 9.0f;

            float t;
            if (v.isWaiting())
            {
                t = 0.82f - 0.08f * rank;
                if (t < 0.18f) t = 0.18f;
            }
            else
            {
                int total = (*visualTravelTotal)[idx];
                if (total < 1) total = 1;
                t = 1.0f - ((float)v.getRemainingTravelTime() / (float)total);
                if (t < 0.08f) t = 0.08f;
                if (t > 0.76f) t = 0.76f;
                t -= 0.06f * rank;
                if (t < 0.08f) t = 0.08f;
            }

            return PointF((float)a.X + dx * t + nx * lane,
                (float)a.Y + dy * t + ny * lane);
        }

        Color GetRoadColor(Road* r)
        {
            double c = r->getCongestion();
            if (c > 0.66) return Color::Red;
            if (c > 0.33) return Color::Orange;
            return Color::Green;
        }

        void DrawSignal(Graphics^ g, TrafficSignal& s)
        {
            Point p = nodePositions[s.getIntersectionId()];
            std::vector<int> incoming = s.getIncomingRoadIds();

            for (int i = 0; i < (int)incoming.size(); i++)
            {
                Road* inRoad = network->getRoad(incoming[i]);
                if (!inRoad) continue;

                Point a = nodePositions[inRoad->getFrom()];
                Point b = nodePositions[inRoad->getTo()];
                float dx = (float)(b.X - a.X), dy = (float)(b.Y - a.Y);
                float len = (float)Math::Sqrt(dx * dx + dy * dy);
                if (len < 1.0f) len = 1.0f;

                float ux = dx / len, uy = dy / len;
                float nx = -uy, ny = ux;

                PointF c((float)b.X - ux * 28.0f, (float)b.Y - uy * 28.0f);
                Pen^ stopPen = gcnew Pen(s.isGreen(incoming[i]) ? Color::LimeGreen : Color::Red, 5);
                g->DrawLine(stopPen,
                    PointF(c.X - nx * 15.0f, c.Y - ny * 15.0f),
                    PointF(c.X + nx * 15.0f, c.Y + ny * 15.0f));
                delete stopPen;

                Brush^ bulb = s.isGreen(incoming[i]) ? Brushes::LimeGreen : Brushes::Red;
                g->FillEllipse(bulb, Rectangle((int)c.X - 7, (int)c.Y - 22, 14, 14));
                g->DrawEllipse(Pens::Black, Rectangle((int)c.X - 7, (int)c.Y - 22, 14, 14));
            }
        }

        void DrawRoad(Graphics^ g, Road* r)
        {
            Point a = nodePositions[r->getFrom()];
            Point b = nodePositions[r->getTo()];

            Pen^ shadow = gcnew Pen(Color::FromArgb(210, 215, 220), 13);
            shadow->StartCap = Drawing::Drawing2D::LineCap::Round;
            shadow->EndCap = Drawing::Drawing2D::LineCap::Round;
            g->DrawLine(shadow, a, b);
            delete shadow;

            Pen^ roadPen = gcnew Pen(GetRoadColor(r), 7);
            roadPen->StartCap = Drawing::Drawing2D::LineCap::Round;
            roadPen->EndCap = Drawing::Drawing2D::LineCap::Round;
            g->DrawLine(roadPen, a, b);
            delete roadPen;

            int mx = (a.X + b.X) / 2, my = (a.Y + b.Y) / 2;
            g->DrawString(L"R" + r->getRoadID().ToString(), gcnew Drawing::Font(L"Segoe UI", 8, FontStyle::Bold), Brushes::Black, (float)mx - 8, (float)my - 18);

            int q = r->getQueueSize();
            if (q > 0)
            {
                g->FillEllipse(Brushes::Orange, Rectangle(mx - 12, my + 3, 24, 18));
                g->DrawString(q.ToString(), gcnew Drawing::Font(L"Segoe UI", 8, FontStyle::Bold), Brushes::Black, (float)mx - 4, (float)my + 3);
            }
        }

        void DrawNode(Graphics^ g, int id, Point p)
        {
            g->FillEllipse(Brushes::White, Rectangle(p.X - 18, p.Y - 18, 36, 36));
            Pen^ nodePen = gcnew Pen(Color::FromArgb(40, 80, 120), 2);
            g->DrawEllipse(nodePen, Rectangle(p.X - 18, p.Y - 18, 36, 36));
            delete nodePen;
            g->DrawString(id.ToString(), gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold), Brushes::Black, (float)p.X - 6, (float)p.Y - 9);
        }

        void DrawCar(Graphics^ g, int idx)
        {
            PointF pt = GetVehiclePoint(idx);
            bool wait = (*vehicles)[idx].isWaiting();
            Brush^ body = wait ? Brushes::OrangeRed : Brushes::RoyalBlue;
            Rectangle bodyRect((int)pt.X - 8, (int)pt.Y - 5, 16, 10);
            g->FillRectangle(body, bodyRect);
            g->DrawRectangle(Pens::Black, bodyRect);
            g->DrawString(L"V" + (*vehicles)[idx].getVehicleId().ToString(), gcnew Drawing::Font(L"Segoe UI", 7, FontStyle::Bold), Brushes::Black, pt.X - 8, pt.Y - 18);
        }

        void panelMap_Paint(System::Object^, PaintEventArgs^ e)
        {
            if (!network || !vehicles || !signals) return;

            Graphics^ g = e->Graphics;
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
            g->Clear(Color::FromArgb(248, 250, 252));

            System::Drawing::Font^ titleFont = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            g->DrawString(L"Live Road Network  |  Green = clear, Orange = medium, Red = congested / stop", titleFont, Brushes::DimGray, 20.0f, 20.0f);

            const auto& allRoads = network->getAllRoads();
            for (auto it = allRoads.begin(); it != allRoads.end(); ++it) DrawRoad(g, it->second);

            for (int i = 0; i < (int)signals->size(); i++) DrawSignal(g, (*signals)[i]);

            for each (System::Collections::Generic::KeyValuePair<int, Point> kv in nodePositions)
                DrawNode(g, kv.Key, kv.Value);

            for (int i = 0; i < (int)vehicles->size(); i++)
            {
                if (!(*launched)[i] || (*vehicles)[i].isArrived()) continue;
                DrawCar(g, i);
            }
        }

        String^ BuildFinalReport()
        {
            String^ s = L"FINAL REPORT\r\n\r\n";
            s += L"Step: " + currentStep.ToString() + L"\r\n";
            s += L"Arrived: " + metrics->getTotalArrived().ToString() + L"/" + vehicles->size().ToString() + L"\r\n";
            s += L"Avg Travel Time: " + metrics->getAverageTravelTime().ToString("F2") + L" steps\r\n";
            s += L"Total Delay: " + metrics->getTotalDelay().ToString("F0") + L"\r\n";
            s += L"Throughput: " + metrics->getThroughput().ToString("F3") + L"\r\n";
            s += L"Avg Network Congestion: " + (metrics->getAverageCongestion() * 100.0).ToString("F1") + L"%\r\n";
            s += L"Objective: " + metrics->computeObjectiveFunction(*network).ToString("F2") + L"\r\n";
            return s;
        }

        void btnLoadPreset_Click(System::Object^, System::EventArgs^) { LoadPresetScenario(); }

        void btnStart_Click(System::Object^, System::EventArgs^)
        {
            if (!presetLoaded) LoadPresetScenario();

            if (chkAutoStep->Checked)
            {
                if (simTimer->Enabled) { simTimer->Stop(); AddLog(L"Auto run paused."); }
                else { simTimer->Start(); AddLog(L"Auto run started."); }
            }
            else
            {
                int n = (int)numSteps->Value;
                for (int i = 0; i < n; i++) StepSimulation();
            }
        }

        void btnStep_Click(System::Object^, System::EventArgs^) { StepSimulation(); }
        void btnReset_Click(System::Object^, System::EventArgs^) { LoadPresetScenario(); }
        void btnGenerateReport_Click(System::Object^, System::EventArgs^) { MessageBox::Show(BuildFinalReport(), L"Final Simulation Report", MessageBoxButtons::OK, MessageBoxIcon::Information); }
        void btnAddCar_Click(System::Object^, System::EventArgs^) { AddOneCarFromSource(Int32::Parse(cmbSpawnNode->Text)); }
        void simTimer_Tick(System::Object^, System::EventArgs^) { StepSimulation(); }
    };

}
