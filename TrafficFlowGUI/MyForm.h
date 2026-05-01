#pragma once

namespace TrafficFlowGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Panel^ panelLeft;
	private: System::Windows::Forms::Panel^ panelMap;
	private: System::Windows::Forms::Panel^ panelRight;
	private: System::Windows::Forms::Panel^ panelBottom;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ btnStep;
	private: System::Windows::Forms::Button^ btnStart;
	private: System::Windows::Forms::Button^ btnLoadPreset;
	private: System::Windows::Forms::NumericUpDown^ numSteps;
	private: System::Windows::Forms::CheckBox^ chkAutoStep;
	private: System::Windows::Forms::Button^ btnReset;
	private: System::Windows::Forms::Label^ label3;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panelLeft = (gcnew System::Windows::Forms::Panel());
			this->panelMap = (gcnew System::Windows::Forms::Panel());
			this->panelRight = (gcnew System::Windows::Forms::Panel());
			this->panelBottom = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnLoadPreset = (gcnew System::Windows::Forms::Button());
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->btnStep = (gcnew System::Windows::Forms::Button());
			this->btnReset = (gcnew System::Windows::Forms::Button());
			this->chkAutoStep = (gcnew System::Windows::Forms::CheckBox());
			this->numSteps = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->panelLeft->SuspendLayout();
			this->panelMap->SuspendLayout();
			this->panelBottom->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numSteps))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Dock = System::Windows::Forms::DockStyle::Top;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(0, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(460, 32);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Traffic Flow Optimization System";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// panelLeft
			// 
			this->panelLeft->BackColor = System::Drawing::SystemColors::ControlLight;
			this->panelLeft->Controls->Add(this->label3);
			this->panelLeft->Controls->Add(this->numSteps);
			this->panelLeft->Controls->Add(this->chkAutoStep);
			this->panelLeft->Controls->Add(this->btnReset);
			this->panelLeft->Controls->Add(this->btnStep);
			this->panelLeft->Controls->Add(this->btnStart);
			this->panelLeft->Controls->Add(this->btnLoadPreset);
			this->panelLeft->Dock = System::Windows::Forms::DockStyle::Left;
			this->panelLeft->Location = System::Drawing::Point(0, 32);
			this->panelLeft->Name = L"panelLeft";
			this->panelLeft->Size = System::Drawing::Size(220, 628);
			this->panelLeft->TabIndex = 1;
			// 
			// panelMap
			// 
			this->panelMap->BackColor = System::Drawing::Color::White;
			this->panelMap->Controls->Add(this->panelBottom);
			this->panelMap->Controls->Add(this->panelRight);
			this->panelMap->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panelMap->Location = System::Drawing::Point(220, 32);
			this->panelMap->Name = L"panelMap";
			this->panelMap->Size = System::Drawing::Size(1053, 628);
			this->panelMap->TabIndex = 2;
			// 
			// panelRight
			// 
			this->panelRight->BackColor = System::Drawing::SystemColors::Control;
			this->panelRight->Dock = System::Windows::Forms::DockStyle::Right;
			this->panelRight->Location = System::Drawing::Point(803, 0);
			this->panelRight->Name = L"panelRight";
			this->panelRight->Size = System::Drawing::Size(250, 628);
			this->panelRight->TabIndex = 0;
			// 
			// panelBottom
			// 
			this->panelBottom->BackColor = System::Drawing::Color::LightGray;
			this->panelBottom->Controls->Add(this->label2);
			this->panelBottom->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->panelBottom->Location = System::Drawing::Point(0, 508);
			this->panelBottom->Name = L"panelBottom";
			this->panelBottom->Size = System::Drawing::Size(803, 120);
			this->panelBottom->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Dock = System::Windows::Forms::DockStyle::Top;
			this->label2->Location = System::Drawing::Point(0, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(81, 20);
			this->label2->TabIndex = 0;
			this->label2->Text = L"Event Log";
			// 
			// btnLoadPreset
			// 
			this->btnLoadPreset->Location = System::Drawing::Point(10, 10);
			this->btnLoadPreset->Name = L"btnLoadPreset";
			this->btnLoadPreset->Size = System::Drawing::Size(180, 34);
			this->btnLoadPreset->TabIndex = 0;
			this->btnLoadPreset->Text = L"Load Preset";
			this->btnLoadPreset->UseVisualStyleBackColor = true;
			// 
			// btnStart
			// 
			this->btnStart->Location = System::Drawing::Point(10, 50);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(75, 31);
			this->btnStart->TabIndex = 1;
			this->btnStart->Text = L"Start";
			this->btnStart->UseVisualStyleBackColor = true;
			// 
			// btnStep
			// 
			this->btnStep->Location = System::Drawing::Point(12, 87);
			this->btnStep->Name = L"btnStep";
			this->btnStep->Size = System::Drawing::Size(105, 130);
			this->btnStep->TabIndex = 2;
			this->btnStep->Text = L"Step";
			this->btnStep->UseVisualStyleBackColor = true;
			// 
			// btnReset
			// 
			this->btnReset->Location = System::Drawing::Point(12, 223);
			this->btnReset->Name = L"btnReset";
			this->btnReset->Size = System::Drawing::Size(91, 170);
			this->btnReset->TabIndex = 3;
			this->btnReset->Text = L"Reset";
			this->btnReset->UseVisualStyleBackColor = true;
			// 
			// chkAutoStep
			// 
			this->chkAutoStep->AutoSize = true;
			this->chkAutoStep->Location = System::Drawing::Point(16, 440);
			this->chkAutoStep->Name = L"chkAutoStep";
			this->chkAutoStep->Size = System::Drawing::Size(107, 24);
			this->chkAutoStep->TabIndex = 4;
			this->chkAutoStep->Text = L"Auto Step";
			this->chkAutoStep->UseVisualStyleBackColor = true;
			// 
			// numSteps
			// 
			this->numSteps->Location = System::Drawing::Point(129, 405);
			this->numSteps->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->numSteps->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numSteps->Name = L"numSteps";
			this->numSteps->Size = System::Drawing::Size(60, 26);
			this->numSteps->TabIndex = 5;
			this->numSteps->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100, 0, 0, 0 });
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 407);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(96, 20);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Steps to run";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1273, 660);
			this->Controls->Add(this->panelMap);
			this->Controls->Add(this->panelLeft);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->panelLeft->ResumeLayout(false);
			this->panelLeft->PerformLayout();
			this->panelMap->ResumeLayout(false);
			this->panelBottom->ResumeLayout(false);
			this->panelBottom->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numSteps))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
