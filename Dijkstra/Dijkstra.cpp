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
};
unordered_map<string, vector<Sensor>> sensor_infos;
void test_sensor() {
    for (auto& x : sensor_infos) {
        cout << x.first << endl;
        for (auto& y : x.second) {
            cout << x.first << ' ' << y.data << ' ' << y.value << endl;
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
        istringstream record(line);
        record >> id >> data >> value;
        Sensor sensor;
        sensor.data = data;
        sensor.value = value;
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
    int len = sensor_infos[id].size();
    string value = sensor_infos[id][len - 1].value;
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
void dayth_ofTheYear() {

}
int main(int argc, char* argv[]) {
    read_train_data();
    
    forcast();
    return 0;
}
