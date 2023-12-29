

DB_Param{
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;

    DB_Param& DB_ParamSetDBName(string_view db_name) {
        this->db_name = db_name;
        return *this;
    }
    DB_Param& SetDBConnectionTimeout(int db_connection_timeout) {
        this->db_connection_timeout = db_connection_timeout;
        return *this;
    }
    DB_Param& SetDBAllowExceptions(bool db_allow_exceptions) {
        this->db_allow_exceptions = db_allow_exceptions;
        return *this;
    }
    DB_Param& SetDBLogLevel(DBLogLevel db_log_level) {
        this->db_log_level = db_log_level;
        return *this;
    }
}

PersonsParam{
    int min_age;
    int max_age;

    PersonsParam& SetMinAge(int min_age) {
        this->min_age = min_age;
        return *this;
    }

    PersonsParam& SetMaxAge(int max_age) {
        this->max_age = max_age;
        return *this;
    }
}

DBHandler CreateDBHandler(DB_param& db_param) {
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    }
    else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    return db;
}

DBQuery CreateDBQuery(DBHandler& db, PersonsParam& persons_param) {

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << persons_param.min_age << " and "s << persons_param.max_age << " "s
        << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    return DBQuery{query_str.str()};
}

vector<Person> LoadPersons(DB_param db_param, PersonsParam persons_param, string_view name_filter) {

    DBHandler db = CreateDBHandler(db_param);
    if (!db_param.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    DBQuery query = CreateDBQuery(db, persons_param);

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}