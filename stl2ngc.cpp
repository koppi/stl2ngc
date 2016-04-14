#include <omp.h>

#include <opencamlib/stlsurf.hpp>
#include <opencamlib/stlreader.hpp>
#include <opencamlib/cylcutter.hpp>
#include <opencamlib/adaptivepathdropcutter.hpp>

#include <locale>
#include <string>
#include <vector>

using namespace std;
using namespace ocl;

wstring wstr(const char *str) { wstring wstr (str, str+strlen(str)); return wstr; }

class APDC : public AdaptivePathDropCutter {
public:
    APDC() : AdaptivePathDropCutter() {}
    virtual ~APDC()  {}
    vector<CLPoint> getPoints() {
        return clpoints;
    }
};

class GCodeWriter {
public:
    GCodeWriter() {};
    /// destructor
    virtual ~GCodeWriter() {};
    void g1(const double x, const double y, const double z) {
        cout << "G1 X" << x << " Y" << y << " Z" << z << endl;
    }
    void g0(const double x, const double y, const double z) {
        cout << "G0 X" << x << " Y" << y << " Z" << z << endl;
    }
};

Path zigzag_x(double minx, double dx, double maxx,
              double miny, double dy, double maxy, double z) {
    Path p;

    int rev = 0;
    for (double i = miny; i < maxy; i+= dy) {
        if (rev == 0) {
            p.append(Line(Point(minx, i, z), Point(maxx, i, z)));
            rev = 1;
        } else {
            p.append(Line(Point(maxx, i, z), Point(minx, i, z)));
            rev = 0;
        }
    }

    return p;
}

Path zigzag_y(double minx, double dx, double maxx,
              double miny, double dy, double maxy, double z) {
    Path p;

    int rev = 0;
    for (double i = minx; i < maxx; i+= dx) {
        if (rev == 0) {
            p.append(Line(Point(i, miny, z), Point(i, maxy, z)));
            rev = 1;
        } else {
            p.append(Line(Point(i, maxy, z), Point(i, miny, z)));
            rev = 0;
        }
    }

    return p;
}

bool isNearlyEqual(double a, double b) {
    int factor = 0.00001;

    double min_a = a - (a - std::nextafter(a, std::numeric_limits<double>::lowest())) * factor;
    double max_a = a + (std::nextafter(a, std::numeric_limits<double>::max()) - a) * factor;

    return min_a <= b && max_a >= b;
}

int main(int argc, char* argv[]) {
    double zsafe = 5;
    double zstep = 3;
    
    double d = 2.0;
    double d_overlap = 1 - 0.75; // step percentage

    double corner = 0; // d

    locale::global(locale("C"));

    //assert(argc == 2);
    //assert(argv[1]);
    
    STLSurf s;
    STLReader r(wstr("/dev/stdin"), s);
    //STLReader r(wstr(argv[1]), s);
    // s.rotate(0,0,0);
    cerr << s << endl;
    cerr << s.bb << endl;

    double zdim = s.bb.maxpt.z - s.bb.minpt.z;
    cerr << "zdim " << zdim << endl;

    double zstart = s.bb.maxpt.z - zstep;
    cerr << "zstart " << zstart << endl;

    CylCutter* c = new CylCutter(d, 6);
    cerr << *c << endl;

    APDC apdc;
    apdc.setSTL(s);
    apdc.setCutter(c);
    apdc.setSampling(d*d_overlap);
    apdc.setMinSampling(d*d_overlap / 100);
    
    double minx, miny, maxx, maxy, z;
    minx = s.bb.minpt.x - corner;
    miny = s.bb.minpt.y - corner;
    maxx = s.bb.maxpt.x + corner;
    maxy = s.bb.maxpt.y + corner;
    z = s.bb.minpt.z - zsafe;

    double dx = d * d_overlap, dy = d * d_overlap;

    Path p = zigzag_x(minx, dx, maxx, miny, dy, maxy, z);
    apdc.setPath(&p);
    apdc.setZ(z);

    cerr << "calculating.. " << flush;

    apdc.setThreads(4);
    apdc.run();

    cerr << "done." << endl;

    GCodeWriter w;

    cout << "G21 F900"   << endl;
    cout << "G64 P0.001" << endl; // path control mode : constant velocity
    cout << "M03 S13500" << endl; // start spindle

    cout << "G0" <<
        " X" << s.bb.minpt.x <<
        " Y" << s.bb.minpt.y <<
        " Z" << zsafe << endl;

    double zcurr = zstart;

    vector<CLPoint> pts = apdc.getPoints();

    bool fst = true;
    while (zcurr > s.bb.minpt.z - zstep) {

        cerr << "zcurr " << zcurr << endl;

        BOOST_FOREACH(Point cp, pts) {
            double z = - fmin(-cp.z, -zcurr) - s.bb.maxpt.z;
            if (!isNearlyEqual(z, 0)) {
                if (fst) {
                    w.g0(cp.x, cp.y, zsafe);
                    w.g0(cp.x, cp.y, 0);
                    fst = false;
                }
                w.g1(cp.x, cp.y, z);
            }
        }
        
        zcurr -= zstep;
        reverse(pts.begin(), pts.end());
    }
    
    cout << "G0Z" << zsafe << endl;
    cout << "M05" << endl; // stop spindle
    cout << "G0X" << s.bb.minpt.x << " Y" << s.bb.minpt.y << endl;
    cout << "M2" << endl;
    cout << "%" << endl;
    
    return 0;
}
