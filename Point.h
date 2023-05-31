using namespace std;

class Point 
{
private:
    string measurement;
    stringstream tags;
    stringstream fields;
    chrono::system_clock::time_point timestamp;

public:
    Point(const string& measurement)
        : measurement(measurement) {
        }

    Point& tag(const string& key, const string& value) {
        if( !tags.str().empty() ) {
            tags << ",";
        }
        tags << key << "=" << value;
        return *this;
    }

    Point& field(const string& key, double value ) {
        if( !fields.str().empty() ) {
            fields << ",";
        }
        fields << key << "=" << value;
        return *this;
    }

    Point& time(const std::chrono::system_clock::time_point& time) {
        timestamp = time;
        return *this;
    }

    string toLineProtocol() const {
        stringstream line;
        line << measurement;
        if( !tags.str().empty() ) {
            line << "," << tags.str();
        }
        line << " " << fields.str();
        line << " " << chrono::duration_cast<chrono::nanoseconds>(timestamp.time_since_epoch()).count();
    
        return line.str();
    }
    
};
