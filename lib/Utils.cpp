#include "Utils.h"

/* parse the json file */
VRP Utils::InitParameters(char **argv) {
    /* error string */
    std::string s;
    VRP v;
    Graph g;
    /* open the file */
    FILE* fp = fopen(argv[1], "r");
    if (fp == NULL) {
        s = "The file " + std::string(argv[1]) +  " doesn't exist.";
        throw s;
    }else {
        /* little buf more fread, big buff less fread big access time */
        char readBuffer[65536];
        rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        this->d.ParseStream(is);
        fclose(fp);
        /* check error */
        if (this->d.HasParseError()) {
            s = "Error(offset" + std::string((char*)this->d.GetErrorOffset()) +
                "): " + GetParseError_En(this->d.GetParseError());
            throw s;
        }else {
            s = "Invalid file format!";
            int numVertices = this->d["vertices"].Size();
            Customer *customers = new Customer[numVertices];
            /* inserting the depot = v0 */
            customers[0] = Customer(this->d["vertices"][0]["name"].GetString(),
                                    this->d["vertices"][0]["x"].GetInt(),
                                    this->d["vertices"][0]["y"].GetInt());
            g.InsertVertex(customers[0]);
            int x, y, request, serviceTime;
            /* parsing all customers */
            for (int i = 1; i < numVertices; i++) {
                auto& item = d["vertices"][i];
                if (item["name"].IsString() && item["x"].IsInt() &&
                    item["y"].IsInt() && item["request"].IsInt() &&
                    item["time"].IsInt()) {
                        /* name, x, y, request, service time */
                        std::string n = this->d["vertices"][i]["name"].GetString();
                        x = this->d["vertices"][i]["x"].GetInt();
                        y = this->d["vertices"][i]["y"].GetInt();
                        request = this->d["vertices"][i]["request"].GetInt();
                        serviceTime = this->d["vertices"][i]["time"].GetInt();
                        customers[i] = Customer(n, x, y, request, serviceTime);
                        /* add vertex */
                        g.InsertVertex(customers[i]);
                }else {
                    throw s;
                }
            }
            /* parsing all costs, graph edge */
            int numCosts = this->d["costs"].Size();
            int from, to, value;
            for (int i = 0; i < numCosts; i++) {
                for (int j = 0; j < numCosts-1; j++) {
                    /* start, end, ammount */
                    from = this->d["costs"][i][j]["from"].GetInt();
                    to = this->d["costs"][i][j]["to"].GetInt();
                    value = this->d["costs"][i][j]["value"].GetInt();
                    /* add egde with weight */
                    g.InsertEdge(customers[from], customers[to], value);
                }
            }
            delete [] customers;
            if (this->d["vehicles"].IsInt() &&
                this->d["capacity"].IsInt() && this->d["worktime"].IsInt()) {
                /* creating the VRP */
                VRP v(g,
                      numVertices,
                      this->d["vehicles"].GetInt(),
                      this->d["capacity"].GetInt(),
                      this->d["worktime"].GetInt());
            }else {
                throw s;
            }
        }
    }
    return v;
}

/* save the result into a file json */
FILE* Utils::SaveResult() {
    FILE* fp = fopen("output.json", "w");
    if (fp != NULL) {
        char writeBuffer[65536];
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
        rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
        rapidjson::Document::AllocatorType& allocator = this->d.GetAllocator();
        rapidjson::Value route(rapidjson::kArrayType);
        for (int i = 0; i < 3; i++) {
            rapidjson::Value r(rapidjson::kArrayType);
            for (int k = 0; k < 10; k++) {
                int c = 1 + (rand() % (int)(8));
                r.PushBack(c, allocator);
            }
            route.PushBack(r, allocator);
        }
        d.AddMember("routes", route, allocator);
        d.Accept(writer);
        fclose(fp);
    }else {
        throw "Error writing file! (Bad permissions)\n";
    }
    return fp;
}

/* pretty print for logging */
void Utils::logger(std::string s, int c) const {
    switch(c) {
        case SUCCESS:
            std::cout << ANSI_GREEN << s << ANSI_RESET << std::endl;
        break;
        case WARNING:
            std::cout << ANSI_YELLOW << s << ANSI_RESET << std::endl;
        break;
        case ERROR:
            std::cout << ANSI_RED << s << ANSI_RESET << std::endl;
        break;
    }
}