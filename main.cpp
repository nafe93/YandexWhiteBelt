#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class Date
{
public:
    Date (int new_year, int new_month, int new_day)
    {
        if (new_year < 0) {
            throw out_of_range("year value is invalid: " + to_string(new_month));
        }
        if (new_month < 1 || new_month > 12)
        {
            throw out_of_range("Month value is invalid: " + to_string(new_month));
        } else if (new_day < 1 || new_day > 31)
        {
            throw out_of_range("Day value is invalid: " + to_string(new_day));
        }
        year = new_year;
        month = new_month;
        day = new_day;
    }

    int GetYear() const
    {
        return year;
    };

    int GetMonth() const
    {
        return month;
    };

    int GetDay() const
    {
        return day;
    };

private:
    int year;
    int month;
    int day;
};

ostream& operator << (ostream& stream, const Date& date)
{
    stream << setfill('0') << setw(4) << to_string(date.GetYear());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetMonth());
    stream << "-";
    stream << setfill('0') << setw(2) << to_string(date.GetDay());
    return stream;
}

bool operator < (const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() != rhs.GetYear()){
        return lhs.GetYear() < rhs.GetYear();
    }
    else if (lhs.GetMonth() != rhs.GetMonth()){
        return lhs.GetMonth() < rhs.GetMonth();
    }
    else {
        return lhs.GetDay() < rhs.GetDay();
    }
}

Date ParseDate(const string& date)
{
    istringstream date_stream(date);
    bool flag = true;

    int year;
    flag = flag && (date_stream >> year);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int month;
    flag = flag && (date_stream >> month);
    flag = flag && (date_stream.peek() == '-');
    date_stream.ignore(1);

    int day;
    flag = flag && (date_stream >> day);
    flag = flag && date_stream.eof();

    if (!flag)
    {
        throw logic_error("Wrong date format: " + date);
    }
    return Date(year, month, day);
}

class Database
{
public:
    void AddEvent(const Date& date, const string& event)
    {
        if (!event.empty())
        {
            records[date].insert(event);
        }
    }

    bool DeleteEvent(const Date& date, const string& event)
    {
        if (records.count(date) > 0 && records[date].count(event) > 0)
        {
            records[date].erase(event);
            return true;
        }
        return false;
    }

    int  DeleteDate(const Date& date)
    {
        int del = 0;
        if (records.count(date) > 0)
        {
            del = records.at(date).size();
            records.erase(date);
        }
        return del;
    }

    set<string> Find(const Date& date) const
    {
        set<string> result;
        if (records.count(date) > 0)
        {
            result = records.at(date);
        }
        return result;
    }


    void Print() const
    {
        for (const auto& r : records)
        {
            for (auto s : r.second)
            {
                cout << r.first << ' ' << s << endl;
            }
        }
    }

private:
    map<Date, set<string>> records;
};

int main()
{
    try
    {
        Database db;
        string command;

        while (getline(cin, command))
        {
            if(command == "") continue;

            stringstream input(command);

            string operation;
            input >> operation;

            map<string, char> operations_codes = {{"Add", 'A'}, {"Del",'D'}, {"Find", 'F'}, {"Print",'P'}};

            char operation_code = operations_codes[operation];

            switch(operation_code)
            {
                case 'A' :
                {
                    string date_string, event;
                    input >> date_string >> event;
                    const Date date = ParseDate(date_string);
                    if (event != "" || event != " ") {
                        db.AddEvent(date, event);
                    }
                    break;
                }
                case 'D' :
                {
                    string date_string, event;
                    input >> date_string;
                    if (!input.eof()) {
                        input >> event;
                    }

                    const Date date = ParseDate(date_string);

                    if (event.empty()) {
                        const int num_of_events_del = db.DeleteDate(date);
                        cout << "Deleted " << num_of_events_del << " events" << endl;
                    } else {
                        if (db.DeleteEvent(date, event)) {
                            cout << "Deleted successfully" << endl;
                        } else {
                            cout << "Event not found" << endl;
                        }
                    }
                    break;
                }
                case 'F' :
                {
                    string date_string, event;
                    input >> date_string >> event;
                    const Date date = ParseDate(date_string);
                    set<string> events = db.Find(date);

                    for (string e : events) {
                        cout << e << endl;
                    }
                    break;
                }
                case 'P' :
                {
                    db.Print();
                    break;
                }
                default:
                    cout << "Unknown command: " << operation << endl;
                    return 0;
            }
        }
    }
    catch (exception& e)
    {
        cout << e.what() << endl;
        return 0;
    }
    return 0;
}
