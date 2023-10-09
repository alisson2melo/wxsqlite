//Name:     Application.cpp

//Purpose:    Application with GUI for interacting with an SQLite        database.

//Author:      Alisson Melo

//Created:     06-10-2023

//Copyright:   (c) 2023 Alisson Melo

//License:     MIT license



#include <wx/sqlite3.h>
#include <wx/wx.h>
#include <wx/grid.h>


class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        // Create a frame
        MyFrame *frame = new MyFrame("Lanches Database", wxPoint(50, 50), wxSize(800, 600));
        frame->Show(true);

        // Initialize the wxSQLite3 library
        if (wxSQLite3Database::Initialize() != wxSQLite3_OK) {
            wxLogError("Failed to initialize wxSQLite3 library");
            return false;
        }

        // Open the database connection
        wxSQLite3Database db;
        if (!db.Open("example.db")) {
            wxLogError("Failed to open database");
            return false;
        }

        // Create the Lanches table if it doesn't exist
        wxString query = "CREATE TABLE IF NOT EXISTS Lanches ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "name TEXT, "
                         "price REAL)";
        wxSQLite3ResultSet result = db.ExecuteQuery(query);
        result.Finalize();

        return true;
    }

    virtual int OnExit() {
        // Cleanup the wxSQLite3 library
        wxSQLite3Database::Shutdown();
        return wxApp::OnExit();
    }
};

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size), grid(nullptr) {
        // Create a panel
        wxPanel *panel = new wxPanel(this, wxID_ANY);

        // Create a grid for displaying data
        grid = new wxGrid(panel, wxID_ANY);
        grid->CreateGrid(0, 2);  // Two columns (name and price)

        // Set column labels
        grid->SetColLabelValue(0, "Name");
        grid->SetColLabelValue(1, "Price");

        // Create a sizer to manage the layout
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(grid, 1, wxEXPAND | wxALL, 5);

        // Set the sizer for the panel
        panel->SetSizer(sizer);

        // Load data into the grid
        LoadData();
    }

    void LoadData() {
        // Open the database connection
        wxSQLite3Database db;
        if (!db.Open("example.db")) {
            wxLogError("Failed to open database");
            return;
        }

        // Query the data from the Lanches table
        wxString query = "SELECT name, price FROM Lanches";
        wxSQLite3ResultSet result = db.ExecuteQuery(query);

        // Clear the grid
        grid->ClearGrid();

        // Populate the grid with data
        int row = 0;
        while (result.NextRow()) {
            wxString name = result.GetAsString(0);
            double price = result.GetDouble(1);

            grid->AppendRows();
            grid->SetCellValue(row, 0, name);
            grid->SetCellValue(row, 1, wxString::Format("%.2f", price));

            row++;
        }

        result.Finalize();
    }

private:
    wxGrid *grid;
};

wxIMPLEMENT_APP(MyApp);
