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
};
unordered_map<string, vector<Sensor>> sensor_infos;
void test_sensor() {
    for (auto& x : sensor_infos) {
        cout << x.first << endl;
        for (auto& y : x.second) {
            cout << x.first << ' ' << y.data << ' ' << y.value << ' ' << y.dayNum << endl;
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
    string id, data, value;
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
        sensor.dayNum = (month - 1) * 30 + day;
        sensor_infos[id].emplace_back(sensor);
    }
    // test_sensor();
}
string find_best_value(int dayNum, string id) {
    string best_value = "nan";
    int best_gap = 999;
    for (auto& sensor : sensor_infos[id]) {
        int gap = abs(sensor.dayNum - dayNum);
        if (gap <= best_gap && sensor.value != "nan") {
            best_gap = gap;
            best_value = sensor.value;
        }
    }
    if (best_gap >= 30) {
        return "nan";
    } else {
        return best_value;
    }
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
    int dayNum = (month - 1) * 30 + day;
    string value = find_best_value(dayNum, id);
    line += ',';
    line += value;
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

int main(int argc, char* argv[]) {
    read_train_data();
    cout << "read over" << endl;
    forcast();
    return 0;
}
