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
			this->panelMap->SuspendLayout();
			this->panelBottom->SuspendLayout();
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
			this->panelMap->ResumeLayout(false);
			this->panelBottom->ResumeLayout(false);
			this->panelBottom->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
