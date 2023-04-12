#include <bits/stdc++.h>
#include <iostream>
#include <queue>
#include <random>
#include <fstream>
#include <string>

using namespace std;

class Point {
    public:
        Point() {}
        Point(const vector<double>& coords) : _coords(coords) {}
        virtual ~Point() {}
        double getDist(const Point &other) const {
            double sum = 0.0;
            for (int i = 0; i < _coords.size(); i++) {
                double diff = _coords[i] - other._coords[i];
                sum += diff * diff;
            }
            return sqrt(sum);
        }
        const vector<double>& getCoords() const {
            return _coords;
        }
        bool operator<(const Point& other) const {
            return _coords[0] < other._coords[0];
        }
    private:
        vector<double> _coords;
};



class NearestPointsContainer{
    public:
        NearestPointsContainer() {}
        virtual ~NearestPointsContainer() {}
        
        void addPoints(const Point &points){
            _points.push_back(points);
        }

        bool comparePoints(const Point &p1, const Point &p2, const Point &target_point) {
            return p1.getDist(target_point) < p2.getDist(target_point);
        }

        
        bool getNearestPoints(const Point &target_point,vector<Point> &nearest_points,int K){
            // PriorityQueue untuk menyimpan k titik terdekat
            priority_queue<pair<double, Point>> nearest_points_pqueue;

            if(K>_points.size()){ // Jika nilai K lebih besar dari jumlah titik yang ada, maka tidak ada titik terdekat yang dapat ditemukan.
                return false;
            }

            ofstream near_points_file("near_points_file.txt"); // Membuat file untuk menyimpan titik terdekat
            near_points_file << "Kumpulan titik :"<<endl;
            for(auto itr = _points.begin(); itr != _points.end(); ++itr){
                double current_point_distance= itr->getDist(target_point);
                // buatkan fungsi untuk menulis near_points_file ke txt
                for(int i=0; i<itr->getCoords().size(); i++){
                    near_points_file << itr->getCoords()[i]<<" ";
                }
                near_points_file << endl;
                if(nearest_points_pqueue.size()<K){
                    nearest_points_pqueue.push({current_point_distance, *itr});
                }else{
                    double highest_distance = nearest_points_pqueue.top().first;
                    if(comparePoints(*itr, nearest_points_pqueue.top().second, target_point)){
                        nearest_points_pqueue.pop();
                        nearest_points_pqueue.push({current_point_distance, *itr});
                    }
                }
            }
            near_points_file << endl;
            near_points_file << "Kumpulan titik terdekat :"<<endl;
            while(!nearest_points_pqueue.empty()){
                Point close_point = nearest_points_pqueue.top().second;
                nearest_points.push_back(close_point);
                for(int i=0; i<close_point.getCoords().size(); i++){
                    near_points_file << close_point.getCoords()[i]<<" ";
                }
                near_points_file << endl;
                nearest_points_pqueue.pop();
            }
            near_points_file << endl;
            near_points_file << "Target Point :" << endl;
            for(int i=0; i<target_point.getCoords().size(); i++){
                near_points_file << target_point.getCoords()[i]<<" ";
            }
            near_points_file << endl;
            near_points_file.close();       
            return true;
        }
    private:
        typedef vector<Point> Points;
        Points _points;
};

int main(int argc, const char * argv[]) {
    const int RANDOM_POINTS_MAX_VALUE = 100; // Nilai maksimal dari titik yang akan dihasilkan secara random
    const int RANDOM_POINTS_COUNT = 1000; // Nilai jumlah titik yang akan dihasilkan secara random
    std::mt19937 eng; // Deklarasi variabel eng untuk menyimpan nilai random 
    eng.seed(std::random_device{}());
    // Membuat distribusi random untuk nilai X dan Y dari titik yang akan dihasilkan secara random
    std::uniform_int_distribution<> dist(-RANDOM_POINTS_MAX_VALUE, RANDOM_POINTS_MAX_VALUE);
    int dimensi;
    vector<double> pointContainer;
    int K;
    // berikan warna pada terminal
    cout << "\033[1;32m" << "====================================================="<< endl;
    cout << "Masukkan nilai dimensi\t\t\t: ";
    cin >> dimensi;
    cout << "Masukkan nilai Poin\t\t\t: ";
    for(int i=0; i<dimensi; i++){
        double point;
        cin >> point;
        pointContainer.push_back(point);
    }
    cout << "Masukkan nilai K(1 s.d. 1000)\t\t: ";
    cin >> K; 
    cout << "====================================================="<< "\033[0m" << endl;

    NearestPointsContainer points_container;
    for(int i=0; i<RANDOM_POINTS_COUNT; i++){
        vector<double> point;
        for(int j=0; j<dimensi; j++){
            point.push_back(dist(eng));
        }
        points_container.addPoints(Point(point));
    }
    Point target_point(pointContainer);
    vector<Point> nearest_points;
    points_container.getNearestPoints(target_point, nearest_points, K);
    cout << "\033[1;33m"; 
    int i=1; // Deklarasi untuk menampilkan nomor titik terdekat
    for( auto itr = nearest_points.begin(); itr != nearest_points.end(); ++itr){
        cout << "Titik Terdekat ke-"<<i<<" : ";
        for(int j=0; j<dimensi; j++){
            cout << itr->getCoords()[j] << " ";
        }
        cout << endl;
        i++;
    }
    cout << "\033[0m";
    cout<<endl;
    return 0;
}