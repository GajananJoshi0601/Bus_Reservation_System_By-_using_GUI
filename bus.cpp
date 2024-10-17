#include <wx/wx.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

class User {
public:
    string username;
    string password;

    void registerUser(const string& uname, const string& pass) {
        username = uname;
        password = pass;
        ofstream file("users.txt", ios::app);
        file << username << " " << password << "\n";
        file.close();
    }

    bool loginUser(const string& uname, const string& pass) {
        ifstream file("users.txt");
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string u, p;
            iss >> u >> p;
            if (u == uname && p == pass) {
                return true;
            }
        }
        return false;
    }
};

class Bus {
public:
    vector<pair<string, string>> routes; // Pair of source and destination
    vector<int> timings;
    vector<int> availability;

    void addBus(const string& source, const string& destination, int timing, int available) {
        routes.emplace_back(source, destination);
        timings.push_back(timing);
        availability.push_back(available);
    }

    string displayBuses(const string& source, const string& destination) {
        stringstream ss;
        for (size_t i = 0; i < routes.size(); ++i) {
            if (routes[i].first == source && routes[i].second == destination) {
                ss << "Route: " << routes[i].first << " to " << routes[i].second
                    << ", Timing: " << timings[i] << ", Availability: " << availability[i] << "\n";
            }
        }
        return ss.str();
    }

    int getAvailability(const string& source, const string& destination) {
        for (size_t i = 0; i < routes.size(); ++i) {
            if (routes[i].first == source && routes[i].second == destination) {
                return availability[i];
            }
        }
        return -1; // No route found
    }

    void updateAvailability(const string& source, const string& destination, int avail) {
        for (size_t i = 0; i < routes.size(); ++i) {
            if (routes[i].first == source && routes[i].second == destination) {
                availability[i] = avail;
            }
        }
    }
};

// Main frame for the application
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

    // Event handlers
    void OnRegister(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    void OnViewBuses(wxCommandEvent& event);
    void OnBookSeat(wxCommandEvent& event);

private:
    User user;
    Bus bus;

    wxTextCtrl* txtUsername;
    wxTextCtrl* txtPassword;
    wxTextCtrl* txtSource;
    wxTextCtrl* txtDestination;

    wxDECLARE_EVENT_TABLE();
};

// Event table for the frame
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_BUTTON(1001, MyFrame::OnRegister)
EVT_BUTTON(1002, MyFrame::OnLogin)
EVT_BUTTON(1003, MyFrame::OnExit)
EVT_BUTTON(1004, MyFrame::OnViewBuses)
EVT_BUTTON(1005, MyFrame::OnBookSeat)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Bus Reservation System");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)) {

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText* lblUser = new wxStaticText(panel, wxID_ANY, wxT("Username:"));
    txtUsername = new wxTextCtrl(panel, wxID_ANY);

    wxStaticText* lblPass = new wxStaticText(panel, wxID_ANY, wxT("Password:"));
    txtPassword = new wxTextCtrl(panel, wxID_ANY);

    wxButton* btnRegister = new wxButton(panel, 1001, wxT("Register"));
    wxButton* btnLogin = new wxButton(panel, 1002, wxT("Login"));
    wxButton* btnExit = new wxButton(panel, 1003, wxT("Exit"));

    vbox->Add(lblUser, 0, wxALL, 10);
    vbox->Add(txtUsername, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    vbox->Add(lblPass, 0, wxALL, 10);
    vbox->Add(txtPassword, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    vbox->Add(btnRegister, 0, wxALL | wxALIGN_CENTER, 10);
    vbox->Add(btnLogin, 0, wxALL | wxALIGN_CENTER, 10);
    vbox->Add(btnExit, 0, wxALL | wxALIGN_CENTER, 10);

    panel->SetSizer(vbox);
}

// Register button event handler
void MyFrame::OnRegister(wxCommandEvent& event) {
    wxString uname = txtUsername->GetValue();
    wxString pass = txtPassword->GetValue();
    user.registerUser(uname.ToStdString(), pass.ToStdString());
    wxMessageBox("User registered successfully!");
}

// Login button event handler
void MyFrame::OnLogin(wxCommandEvent& event) {
    wxString uname = txtUsername->GetValue();
    wxString pass = txtPassword->GetValue();
    if (user.loginUser(uname.ToStdString(), pass.ToStdString())) {
        wxMessageBox("Login successful!");
    }
    else {
        wxMessageBox("Invalid username or password!");
    }
}

// Exit button event handler
void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

// View buses button event handler
void MyFrame::OnViewBuses(wxCommandEvent& event) {
    wxString source = txtSource->GetValue();
    wxString destination = txtDestination->GetValue();
    wxString result = bus.displayBuses(source.ToStdString(), destination.ToStdString());
    wxMessageBox(result);
}

// Book seat button event handler
void MyFrame::OnBookSeat(wxCommandEvent& event) {
    // Similar logic for seat booking as in console version
}
