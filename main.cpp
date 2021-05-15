using namespace std;
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <bits/stdc++.h>
// #include <vector>
// #include<queue>
// #include <set>
// #include<limits>
// #include <utility>
// #include <algorithm>
// #include <math.h>
typedef vector<int> vi;
typedef pair<int, int> pi;
#define loop(i, a, b) for (int i = a; i <= b; i++)
#define pb push_back
#define travel(i, a) for (auto i = a.begin(); i != a.end(); i++)

//forward definations of classes and functions
class datapoint;
class grid;
void view_dataset(vector<datapoint> &);
void generate_dataset(vector<datapoint> &);
void read_data_from_file(vector<datapoint> &, string);
datapoint split_line(string);
void add_datapoint_in_bucket(datapoint, string);
void create_bucket(vector<datapoint> &, string);
void create_empty_bucket(string);
void display_grid(grid);
pi find_point_in_grid(datapoint);
void insert(datapoint);
void display_all_grids();
void get_data_from_bucket(vector<datapoint> &, string);
int bucket_size(string);
void split_grid_x(int, int, int);
void split_grid_y(int, int, int);
void erase_element_in_vector(vector<datapoint> &, datapoint);
void view_datapoint(datapoint);
void delete_all_buckets();
bool sortx(datapoint, datapoint);
bool sorty(datapoint, datapoint);
void delete_bucket_using_bucket_name(string);
void add_vector_to_extra_buckets(vector<datapoint> &, grid &);
void rearrange_extra_buckets_x(grid &, grid &, int);
void rearrange_extra_buckets_y(grid &, grid &, int);
void display_all_buckets();
//forward definations of classes and functions

class datapoint
{
public:
    int id, x, y;
    datapoint()
    {
        id = -1;
        x = -1;
        y = -1;
    }
    datapoint(int id, int x, int y)
    {
        this->id = id;
        this->x = x;
        this->y = y;
    }
};

class grid
{
public:
    int xmin, ymin, xmax, ymax;
    string bucket;
    vector<datapoint> v;
    vector<string> extra_buckets;
    grid()
    {
        bucket = "notset";
        xmin = -1;
        ymin = -1;
        xmax = -1;
        ymax = -1;
    }
    grid(int x1, int y1, int x2, int y2)
    {

        bucket = "notset";
        xmin = x1;
        ymin = y1;
        xmax = x2;
        ymax = y2;
    }
};

#define no_of_datapoints 3000
vector<vector<grid>> vg;
int max_bucket_size;
string axis = "x";
int cnt = 0;

int main()
{
    // display_all_buckets();
    delete_all_buckets();
    vector<datapoint> dataset;
    cout << "enter maximum bucket size:";
    int temp = 0;
    cin >> temp;
    if (temp <= 0)
    {
        cout << "max bucket size cannot be 0 or negetive";
        return 0;
    }
    max_bucket_size = temp;
    cout << "\n\n1: use random dataset\n";
    cout << "2: input data from a file\n";
    int ans = 0;
    cin >> ans;
    if (ans == 1)
        generate_dataset(dataset);
    else
    {
        cout << "Enter file name: ";
        string stemp = "";
        cin >> stemp;
        read_data_from_file(dataset, stemp);
    }
    // view_dataset(dataset);
    vg.pb({});
    vg[0].pb(grid(0, 0, 400, 400));
    loop(it, 0, int(dataset.size()) - 1)
    {
        insert(dataset[it]);
    }

    while (1)
    {

        cout << "\n\n1 : DISPLAY GRIDS INFO\n";
        cout << "2 : DISPLAY ALL BUCKETS\n";
        int n = 0;
        cin >> n;
        if (n == 1)
        {
            display_all_grids();
        }
        else if (n == 2)
        {
            display_all_buckets();
        }
    }
    // cout << endl
    //      << "\n\nDISPLAYING ALL GRIDS" << endl
    //      << endl;
    // display_all_grids();
    return 0;
}

void insert(datapoint d)
{
    pi p = find_point_in_grid(d);
    int x = p.first, y = p.second;
    // cout << "***" << y << " " << x << "***";
    // cout << x << y;
    if (vg[0][0].bucket == "notset")
    {
        cout << "\ninserting  ";
        view_datapoint(d);

        string bucket_name = "bucket" + to_string(cnt);
        cnt++;
        add_datapoint_in_bucket(d, bucket_name);
        vg[0][0].bucket = bucket_name;
        vg[0][0].v.pb(d);
        return;
    }
    string bucket_name = vg[y][x].bucket;
    if (int(vg[y][x].v.size()) < max_bucket_size)
    {
        if (bucket_size(bucket_name) < max_bucket_size)
        {
            cout << "\ninserting  ";
            view_datapoint(d);

            vg[y][x].v.pb(d);
            add_datapoint_in_bucket(d, bucket_name);
        }
        else
        {
            //bucket overflow logic
            cout << "\ninserting  ";
            view_datapoint(d);
            cout << "---------------bucket overflow " << y << " " << x;
            vg[y][x].v.pb(d);
            vector<datapoint> vtemp;
            get_data_from_bucket(vtemp, bucket_name);
            string new_bucket_name = "bucket" + to_string(cnt);
            create_bucket(vg[y][x].v, new_bucket_name);
            cnt++;
            vg[y][x].bucket = new_bucket_name;
            loop(it, 0, int(vg[y][x].v.size()) - 1)
            {
                erase_element_in_vector(vtemp, vg[y][x].v[it]);
            }
            create_bucket(vtemp, bucket_name);
            vtemp.clear();
        }
    }
    else
    {
        //split grid logic,grid overflow
        cout << "\ninserting  ";
        view_datapoint(d);
        cout << "-----------------grid overflow";

        vg[y][x].v.pb(d);
        if (axis == "x")
        {
            sort(vg[y][x].v.begin(), vg[y][x].v.end(), sortx);
            int median_index = int(vg[y][x].v.size()) / 2;
            int x_val = vg[y][x].v[median_index].x;
            while (x_val == vg[y][x].v[median_index].x)
            {
                median_index--;
                if (median_index == -1)
                {
                    //handle the exceptional case
                    cout << "\nEXCEPTIONAL CASE: creating extra bucket in the grid";

                    break;
                }
            }
            median_index++;
            // cout << " median index:" << median_index << " grid: " << y << "," << x << " ,x=" << x_val << endl;
            // cout << "before x split" << endl;
            split_grid_x(x, y, median_index);
        }
        else if (axis == "y")
        {
            sort(vg[y][x].v.begin(), vg[y][x].v.end(), sorty);
            // view_dataset(vg[y][x].v);
            int median_index = int(vg[y][x].v.size()) / 2;
            int y_val = vg[y][x].v[median_index].y;
            while (y_val == vg[y][x].v[median_index].y)
            {
                median_index--;
                if (median_index == -1)
                {
                    //handle the exceptional case
                    cout << "\nEXCEPTIONAL CASE: creating extra bucket in the grid";
                    break;
                }
            }
            median_index++;
            // cout << " median index:" << median_index << " grid: " << y << "," << x << " ,y=" << y_val << endl;
            // cout << "before y split" << endl;
            split_grid_y(x, y, median_index);
        }
        //axis change after split
        if (axis == "x")
            axis = "y";
        else if (axis == "y")
            axis = "x";
    }
}

void split_grid_y(int x, int y, int median_index)
{

    if (median_index == 0)
    {
        vector<datapoint> v1(vg[y][x].v.begin(), vg[y][x].v.end() - 1);
        vector<datapoint> v2;
        v2.pb(vg[y][x].v[vg[y][x].v.size() - 1]);
        vg[y][x].v.clear();
        vg[y][x].v = v2;
        string bucket_name = "bucket" + to_string(cnt);
        cnt++;
        create_bucket(v2, vg[y][x].bucket);
        create_bucket(v1, bucket_name);
        vg[y][x].extra_buckets.pb(bucket_name);
        if (axis == "x")
            axis = "y";
        else if (axis == "y")
            axis = "x";
        return;
    }

    int median_val = vg[y][x].v[median_index].y;
    vector<grid> vgrid;
    loop(it, 0, int(vg[0].size()) - 1)
    {
        vector<datapoint> v1 = {};
        vector<datapoint> v2 = {};
        sort(vg[y][it].v.begin(), vg[y][it].v.end(), sorty);
        // cout << (int(vg[y][it].v.size()) - 1);
        // cout << "\ncheckpoint\n";
        loop(it1, 0, (int(vg[y][it].v.size()) - 1))
        {
            // cout << "\ncheckpoint\n";
            if (vg[y][it].v[it1].y < median_val)
                v1.pb(vg[y][it].v[it1]);
            else
                v2.pb(vg[y][it].v[it1]);
        }
        if (it == x)
        {

            string bucket_name = "bucket" + to_string(cnt);
            grid g(vg[y][it].xmin, median_val, vg[y][it].xmax, vg[y][it].ymax);
            g.v = v2;
            g.bucket = bucket_name;
            create_bucket(v2, bucket_name);
            cnt++;
            rearrange_extra_buckets_x(vg[y][it], g, median_val);
            vgrid.pb(g);
            // cout << "checkpoint" << endl;
            // cout << "v1:" << endl;
            // view_dataset(v1);
            // cout << "v2:" << endl;
            // view_dataset(v2);
            // display_grid(g);
            vg[y][it].ymax = median_val;
            vg[y][it].v = v1;
            create_bucket(v1, vg[y][it].bucket);
            // vg[it].insert(vg[it].begin() + x + 1, g);
        }
        else
        {
            grid g(vg[y][it].xmin, median_val, vg[y][it].xmax, vg[y][it].ymax);
            g.v = v2;
            g.bucket = vg[y][it].bucket;
            rearrange_extra_buckets_x(vg[y][it], g, median_val);
            vgrid.pb(g);
            vg[y][it].ymax = median_val;
            vg[y][it].v = v1;
            // vg[it].insert(vg[it].begin() + x + 1, g);
        }
        v1.clear();
        v2.clear();
    }
    // cout << "&&" << vgrid.size();
    // display_grid(vgrid[0]);
    // display_grid(vgrid[1]);
    vg.insert(vg.begin() + y + 1, vgrid);
    vgrid.clear();
}

void split_grid_x(int x, int y, int median_index)
{
    if (median_index == 0)
    {
        sort(vg[y][x].v.begin(), vg[y][x].v.end(), sortx);
        vector<datapoint> v1(vg[y][x].v.begin(), vg[y][x].v.end() - 1);
        vector<datapoint> v2;
        v2.pb(vg[y][x].v[vg[y][x].v.size() - 1]);
        vg[y][x].v.clear();
        vg[y][x].v = v2;
        string bucket_name = "bucket" + to_string(cnt);
        cnt++;
        create_bucket(v2, vg[y][x].bucket);
        create_bucket(v1, bucket_name);
        vg[y][x].extra_buckets.pb(bucket_name);
        if (axis == "x")
            axis = "y";
        else if (axis == "y")
            axis = "x";
        return;
    }
    //implement split logic
    // cout << "before";
    int median_val = vg[y][x].v[median_index].x;
    // cout << median_val;

    loop(it, 0, int(vg.size()) - 1)
    {
        vector<datapoint> v1;
        vector<datapoint> v2;
        sort(vg[it][x].v.begin(), vg[it][x].v.end(), sortx);
        // view_dataset(vg[it][x].v);
        loop(it1, 0, int(vg[it][x].v.size()) - 1)
        {
            if (vg[it][x].v[it1].x < median_val)
                v1.pb(vg[it][x].v[it1]);
            else
                v2.pb(vg[it][x].v[it1]);
        }
        // view_dataset(v1);
        // cout << "##################";
        // view_dataset(v2);
        if (it == y)
        {
            string bucket_name = "bucket" + to_string(cnt);
            grid g(median_val, vg[it][x].ymin, vg[it][x].xmax, vg[it][x].ymax);
            cnt++;
            g.v = v2;
            // view_dataset(g.v);
            // cout << endl;
            g.bucket = bucket_name;
            create_bucket(v2, bucket_name);
            vg[it][x].xmax = median_val;
            vg[it][x].v = v1;
            create_bucket(v1, vg[it][x].bucket);
            rearrange_extra_buckets_x(vg[it][x], g, median_val);
            vg[it].insert(vg[it].begin() + x + 1, g);
        }
        else
        {
            grid g(median_val, vg[it][x].ymin, vg[it][x].xmax, vg[it][x].ymax);
            g.v = v2;
            g.bucket = vg[it][x].bucket;
            vg[it][x].xmax = median_val;
            vg[it][x].v = v1;
            rearrange_extra_buckets_x(vg[it][x], g, median_val);
            vg[it].insert(vg[it].begin() + x + 1, g);
        }
        v1.clear();
        v2.clear();
    }
}

void rearrange_extra_buckets_y(grid &g1, grid &g2, int median_val)
{
    if (g1.extra_buckets.empty())
        return;

    vector<datapoint> v;
    vector<datapoint> v1;
    vector<datapoint> v2;
    for (int i = 0; i < g1.extra_buckets.size(); i++)
    {
        vector<datapoint> temp;
        get_data_from_bucket(temp, g1.extra_buckets[i]);
        for (int it = 0; it < temp.size(); it++)
        {
            v.pb(temp[it]);
        }
        temp.clear();
    }
    loop(it1, 0, int(v.size()) - 1)
    {
        if (v[it1].y < median_val)
            v1.pb(v[it1]);
        else
            v2.pb(v[it1]);
    }
    v.clear();

    add_vector_to_extra_buckets(v1, g1);
    add_vector_to_extra_buckets(v2, g2);
}

void rearrange_extra_buckets_x(grid &g1, grid &g2, int median_val)
{
    if (g1.extra_buckets.empty())
        return;

    vector<datapoint> v;
    vector<datapoint> v1;
    vector<datapoint> v2;
    for (int i = 0; i < g1.extra_buckets.size(); i++)
    {
        vector<datapoint> temp;
        get_data_from_bucket(temp, g1.extra_buckets[i]);
        for (int it = 0; it < temp.size(); it++)
        {
            v.pb(temp[it]);
        }
        temp.clear();
    }
    loop(it1, 0, int(v.size()) - 1)
    {
        if (v[it1].x < median_val)
            v1.pb(v[it1]);
        else
            v2.pb(v[it1]);
    }
    v.clear();

    add_vector_to_extra_buckets(v1, g1);
    add_vector_to_extra_buckets(v2, g2);
}

void add_vector_to_extra_buckets(vector<datapoint> &v, grid &g)
{
    for (int k = 0; k < g.extra_buckets.size(); k++)
    {
        delete_bucket_using_bucket_name(g.extra_buckets[k]);
        create_empty_bucket(g.extra_buckets[k]);
    }
    g.extra_buckets.clear();
    if (v.size() == 0)
        return;

    int point_cnt = 0;
    vector<datapoint> v1;
    for (int i = 0; i < v.size(); i++)
    {
        v1.pb(v[i]);
        point_cnt++;
        if (point_cnt == max_bucket_size)
        {
            string bucket_name = "bucket" + to_string(cnt);
            create_bucket(v1, bucket_name);
            cnt++;
            g.extra_buckets.pb(bucket_name);
            v1.clear();
            point_cnt = 0;
        }
    }
    if (!v1.empty())
    {
        string bucket_name = "bucket" + to_string(cnt);
        create_bucket(v1, bucket_name);
        cnt++;
        g.extra_buckets.pb(bucket_name);
        v1.clear();
        point_cnt = 0;
    }
}

bool sortx(datapoint d1, datapoint d2)
{
    return (d1.x < d2.x);
}

bool sorty(datapoint d1, datapoint d2)
{
    return (d1.y < d2.y);
}

pi find_point_in_grid(datapoint d)
{
    int j = 0, i = 0;
    for (i = 0; i < int(vg[0].size()); i++)
    {
        if (d.x >= vg[0][i].xmin && d.x < vg[0][i].xmax)
            break;
    }
    for (j = 0; j < int(vg.size()); j++)
    {
        if (d.y >= vg[j][i].ymin && d.y < vg[j][i].ymax)
            break;
    }

    return make_pair(i, j);
}
void display_grid(grid g)
{
    cout << "########################START#############################" << endl
         << endl;
    cout << "grid AREA : (" << g.xmin << "," << g.ymin << ")->"
         << "(" << g.xmax << "," << g.ymax << ")" << endl
         << endl;
    cout << "POINTS TO : " << endl
         << g.bucket;
    for (int i = 0; i < g.extra_buckets.size(); i++)
    {
        cout << ", " << g.extra_buckets[i];
    }
    cout << endl
         << endl;
    cout << "CONTAINS POINTS : " << endl;
    view_dataset(g.v);
    for (int i = 0; i < g.extra_buckets.size(); i++)
    {
        // cout << ", " << g.extra_buckets[i];
        vector<datapoint> v1;
        v1.clear();
        get_data_from_bucket(v1, g.extra_buckets[i]);
        view_dataset(v1);
    }
    cout << endl
         << endl;
    cout << "#########################END##############################" << endl
         << endl;
}

void display_all_grids()
{
    for (int i = 0; i < int(vg.size()); i++)
    {
        for (int j = 0; j < int(vg[0].size()); j++)
        {
            cout << "grid no : (" << i << "," << j << ")" << endl;
            display_grid(vg[i][j]);
        }
    }
}

void view_dataset(vector<datapoint> &v)
{
    if (v.empty())
        return;
    loop(i, 0, int(v.size()) - 1)
    {
        cout << "ID : " << v[i].id << ", point : ( " << v[i].x << " , " << v[i].y << " )" << endl;
    }
}
void view_datapoint(datapoint d)
{

    cout << "ID : " << d.id << ", point : ( " << d.x << " , " << d.y << " )";
}

void generate_dataset(vector<datapoint> &v)
{
    // ofstream MyFile("dataset.txt");
    v.clear();
    loop(i, 1, no_of_datapoints)
    {
        datapoint temp;
        temp.id = i;
        temp.x = rand() % 399 + 1;
        temp.y = rand() % 399 + 1;
        v.pb(temp);
        // MyFile << temp.id << " " << temp.x << " " << temp.y << endl;
    }
    // MyFile.close();
}

void read_data_from_file(vector<datapoint> &v, string filename)
{
    string line;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            //use line here
            // cout<<line<<endl;
            v.pb(split_line(line));
        }
        myfile.close();
    }
    else
        cout << "Unable to open file";
}

void get_data_from_bucket(vector<datapoint> &v, string filename)
{
    string line;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            //use line here
            // cout<<line<<endl;
            v.pb(split_line(line));
        }
        myfile.close();
    }
    else
        cout << "Unable to open bucket";
}
int bucket_size(string filename)
{
    string line;
    ifstream myfile(filename);
    int cnt = 0;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            //use line here
            // cout<<line<<endl;
            // v.pb(split_line(line));
            cnt++;
        }
        myfile.close();
    }
    else
        cout << "Unable to open bucket";

    return cnt;
}
datapoint split_line(string str)
{
    datapoint d1;
    vi v;
    string word = "";
    for (auto x : str)
    {
        if (x == ' ')
        {
            // cout << word << "#";
            v.pb(std::stoi(word));
            word = "";
        }
        else
        {
            word = word + x;
        }
    }
    // cout << word << endl;
    v.pb(std::stoi(word));
    d1.id = v[0];
    d1.x = v[1];
    d1.y = v[2];
    v.clear();
    return d1;
}

void add_datapoint_in_bucket(datapoint temp, string filename)
{
    // ofstream MyFile(filename);
    // MyFile << temp.id << " " << temp.x << " " << temp.y << endl;
    // MyFile.close();
    ofstream outfile;
    outfile.open(filename, std::ios_base::app); // append instead of overwrite
    outfile << temp.id << " " << temp.x << " " << temp.y << endl;
}
void create_bucket(vector<datapoint> &v, string filename)
{
    ofstream MyFile(filename);
    // MyFile << "Files can be tricky, but it is fun enough!";
    loop(i, 0, int(v.size()) - 1)
    {
        MyFile << v[i].id << " " << v[i].x << " " << v[i].y << endl;
    }
    MyFile.close();
}

void erase_element_in_vector(vector<datapoint> &v, datapoint d)
{
    int index = -1;
    // cout << d.id << endl;
    loop(i, 0, int(v.size()) - 1)
    {
        if (v[i].id == d.id)
            index = i;
    }
    if (index != -1)
        v.erase(v.begin() + index);
}

void delete_all_buckets()
{
    int i = 0;
    while (1)
    {
        // string s = "bucket" + to_string(i);
        char *char_arr;
        string str_obj("bucket" + to_string(i));
        char_arr = &str_obj[0];
        if (remove(char_arr) == 0)
        {
            i++;
            continue;
        }
        else
            break;
    }
}

void delete_bucket_using_bucket_name(string bucket_name)
{

    char *char_arr;
    string str_obj(bucket_name);
    char_arr = &str_obj[0];
    if (remove(char_arr) == 0)
    {
        return;
    }
    else
        return;
}

void create_empty_bucket(string filename)
{
    ofstream MyFile(filename);
    // MyFile << "Files can be tricky, but it is fun enough!";
    MyFile.close();
}

void display_all_buckets()
{
    int i = 0;
    while (1)
    {
        // string s = "bucket" + to_string(i);
        // char *char_arr;
        // string str_obj("bucket" + to_string(i));
        // char_arr = &str_obj[0];
        vector<datapoint> v;
        string bucket_name = "bucket" + to_string(i);
        ifstream ifile;
        ifile.open(bucket_name);
        if (ifile)
        {
            get_data_from_bucket(v, bucket_name);
            cout << "***********START*************" << endl
                 << endl;
            cout << bucket_name << ": " << endl;
            view_dataset(v);
            cout << endl
                 << "************END**************" << endl
                 << endl;
            v.clear();
            ifile.close();
        }
        else
            break;

        i++;
    }
}