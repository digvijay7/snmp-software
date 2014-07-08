#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main(int argc, char* argv[])
{
	char *sql,*sql2,*sql3;
   
	try{
		connection C("dbname=mydb user=postgres password=admin \
		hostaddr=127.0.0.1 port=5432");
		if (C.is_open()) {
			cout << "Opened database successfully: " << C.dbname() << endl;
      		} else {
			cout << "Can't open database" << endl;
         		return 1;
      		}

	sql = (char*)"CREATE TABLE live_table ( client_id integer not null, device_id integer not null, ts timestamp not null);";
	work w(C);
	w.exec(sql);
	w.commit();

	sql2 = (char*)"CREATE OR REPLACE FUNCTION function_live_logs() RETURNS TRIGGER AS $trigger_live_logs$ \
		DECLARE \
			res RECORD; \
			foundFlag INTEGER :=0; \
    		BEGIN \
			IF (NEW.type =1) THEN \
				FOR res IN SELECT * FROM live_table where live_table.client_id = NEW.client_id LOOP \
					foundFlag :=1; \
					IF (NEW.device_id != res.device_id ) THEN \
					 UPDATE live_table SET device_id = NEW.device_id,ts = NEW.ts where client_id = NEW.client_id; \
					END IF; \
				END LOOP; \
				IF (foundFlag =0) THEN \
					INSERT INTO live_table VALUES(NEW.client_id, NEW.device_id, NEW.ts);\
				END IF; \
			END IF; \
			IF (NEW.type =2) THEN \
				FOR res IN SELECT * FROM live_table where live_table.client_id = NEW.client_id LOOP \
					IF(NEW.device_id = res.device_id) THEN \
						DELETE FROM live_table where client_id = NEW.client_id; \
					END IF; \
				END LOOP; \
			END IF; \
			RETURN NULL; \
		END; \
		$trigger_live_logs$ LANGUAGE plpgsql;" ;

	work w2(C);
	w2.exec(sql2);
	w2.commit();
	
	sql3 = (char*)
		"CREATE TRIGGER trigger_live_logs \
		AFTER INSERT ON logs \
			FOR EACH ROW EXECUTE PROCEDURE function_live_logs();";
	
	work w3(C);
	w3.exec(sql3);
	w3.commit();

	C.disconnect ();
	}catch (const std::exception &e){
		cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
