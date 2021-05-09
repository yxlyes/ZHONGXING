#include "Dijkstra.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
using namespace std;
struct Sensor {
    string data;
    string value;
    int dayNum;
    bool weekend;
};
struct Forcast {
    bool can_forcast;
    double rate_of_increase;
    string last_value;
};
struct canUse {
    string weekend_value;
    string no_weekend_value;
};
unordered_map<string, vector<Sensor>> sensor_infos;
unordered_map<string, canUse> can_use;
unordered_map<int, int> day_of_month_notLeapYear {
    {1, 31}, {2, 28}, {3, 31}, {4, 30}, {5, 31}, {6, 30},
    {7, 31}, {8, 31}, {9, 30}, {10, 31}, {11, 30}, {12, 31}
};
unordered_map<int, int> daysNum_hasSpend_byMonth_notLeapYear {
    {1, 0}, {2, 31}, {3, 59}, {4, 90}, {5, 120}, {6, 151},
    {7, 181}, {8, 212}, {9, 243}, {10, 273}, {11, 304}, {12, 334}
};
unordered_map<int, int> daysNum_hasSpend_byYear {
    {2017, 0}, {2018, 365}, {2019, 730}
};
void test_sensor() {
    for (auto& x : sensor_infos) {
        cout << x.first << endl;
        for (auto& y : x.second) {
            cout << x.first << ' ' << y.data << ' ' << y.value << ' ' << y.dayNum << ' ' << y.weekend << endl;
        }
    }
}
void read_train_data() {
    string train_file_name = "train_step2.csv";
    string train_file_name2 = "test.csv";
    ifstream in_train(train_file_name);
    string line;
    getline(in_train, line);
    // cout << line << endl;
    string id, data;
    string value;
    while (getline(in_train, line)) {
        int line_len = line.size();
        for (auto& c : line) {
            if (c == ',')   c = ' ';
        }
        istringstream record1(line);
        record1 >> id >> data >> value;
        Sensor sensor;
        sensor.data = data;
        sensor.value = value;
        for (auto& c : data) {
            if (c == '-') c = ' ';
        }
        istringstream record2(data);
        int year, month, day;
        record2 >> year >> month >> day;
        sensor.dayNum = daysNum_hasSpend_byYear[year] + daysNum_hasSpend_byMonth_notLeapYear[month] + day;
        if (sensor.dayNum % 7 == 1 || sensor.dayNum % 7 == 0) {
            sensor.weekend = true;
        } else {
            sensor.weekend = false;
        }
        sensor_infos[id].emplace_back(sensor);
    }
    // test_sensor();
}

void revise_name(string& line) {
    line += ",value";    
}
void revise_data(string& line) {
    string temp_line = line;
    string id, data;
    for (auto& c : temp_line) {
        if (c == ',')   c = ' ';
    }
    istringstream record(temp_line);
    record >> id >> data;
    for (auto& c : data) {
        if (c == '-') c = ' ';
    }
    istringstream record3(data);
    int year, month, day;
    record3 >> year >> month >> day;
    int dayNum = daysNum_hasSpend_byYear[year] + daysNum_hasSpend_byMonth_notLeapYear[month] + day;
    if (dayNum % 7 == 1 || dayNum % 7 == 0) {
        line += ',';
        line += can_use[id].weekend_value;
    } else {
        line += ',';
        line += can_use[id].no_weekend_value;
    }
}
void forcast() {
    string forcast_file_name = "test_step2.csv";
    string output_file_name = "result_step2.txt";
    ifstream in_test(forcast_file_name);
    ofstream out_result(output_file_name);
    string line;
    getline(in_test, line);
    revise_name(line);
    out_result << line << endl;
    while (getline(in_test, line)) {
        revise_data(line);
        out_result << line << endl;
    }
}
void preforcast() {
    for (auto& sensor : sensor_infos) {
        can_use[sensor.first].no_weekend_value = "nan";
        can_use[sensor.first].weekend_value = "nan";
        int len = sensor.second.size();
        bool find_weekend = false;
        bool find_no_weekend = false;
        int weekend_num = 0;
        int no_weekend_num = 0;
        double weekend_value = 0;
        double no_weekend_value = 0;
        int cnt = 0;
        for (int i = len - 1; i >= 0; --i) {
            cnt++;
            if (cnt > len / 5) break;
            if (!find_weekend) {
                if (sensor.second[i].weekend && sensor.second[i].value != "nan") {
                    find_weekend = true;

                    can_use[sensor.first].weekend_value = sensor.second[i].value;
                }
            }
            if (!find_no_weekend) {
                if (!sensor.second[i].weekend && sensor.second[i].value != "nan") {
                    find_no_weekend = true;
                    can_use[sensor.first].no_weekend_value = sensor.second[i].value;
                }
            }
            if (find_weekend && find_no_weekend) {
                break;
            }
        }
    }
}
int main(int argc, char* argv[]) {
    read_train_data();
    cout << "read over" << endl;
    preforcast();
    cout << "preforcast over" << endl;
    forcast();
    return 0;
}
