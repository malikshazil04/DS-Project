#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace TrafficFlowGUI;

[STAThread]
int main(cli::array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm());
    return 0;
}

