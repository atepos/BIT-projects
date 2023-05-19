DROP TABLE "Travel_Agent";
DROP TABLE "Seat_Configuration";
DROP TABLE "Reservation";
DROP TABLE "Ticket";
DROP TABLE "Flight_Schedule";
DROP TABLE "Class_Price";
DROP TABLE "Flight";
DROP TABLE "Airplane";
DROP TABLE "Routes";
DROP TABLE "Airline";
DROP TABLE "Class";
DROP TABLE "Ticket_Status";
DROP TABLE "Customer";
DROP MATERIALIZED VIEW "Agent_view";

SET SERVEROUTPUT ON;


CREATE TABLE "Customer"(
    "user_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "first_name" VARCHAR(50) NOT NULL,
    "last_name" VARCHAR(50) NOT NULL,
    "email" VARCHAR(255) NOT NULL,
    "date_of_birth" DATE NOT NULL,
    "passport_number" VARCHAR(20) NOT NULL,
    CONSTRAINT valid_passport_number CHECK (REGEXP_LIKE("passport_number", '^[0-9]{8}$')), -- Czech passport format
    CONSTRAINT valid_email CHECK (REGEXP_LIKE("email", '^[a-z]+[a-z0-9\.]*@[a-z0-9\.-]+\.[a-z]{2,}$', 'i'))
);

CREATE TABLE "Travel_Agent"(
    "user_id" INT PRIMARY KEY NOT NULL,
    "agency_name" VARCHAR(50),
    "commision" INT,
    CONSTRAINT "Travel_Agent_Customer_id_fk" FOREIGN KEY ("user_id") REFERENCES "Customer" ("user_id")
);


CREATE TABLE "Ticket_Status"(
    "status_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "status_description" VARCHAR(50) NOT NULL
);

CREATE TABLE "Class"(
    "class_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "name" VARCHAR(50)
);

CREATE TABLE "Airline"(
    "airline_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "name" VARCHAR(50)
);

CREATE TABLE "Routes"(
    "routes_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "departure" VARCHAR(50),
    "destination" VARCHAR(50)
);

CREATE TABLE "Airplane"(
    "airplane_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "type" VARCHAR(50),
    "airline_id"  INT NOT NULL,

    CONSTRAINT "Airplane_Airline_id_fk" FOREIGN KEY ("airline_id") REFERENCES "Airline" ("airline_id")
        ON DELETE SET NULL
);

CREATE TABLE "Flight"(
    "flight_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "airline_id"  INT NOT NULL,
    "routes_id"  INT NOT NULL,

    CONSTRAINT "Flight_Airline_id_fk" FOREIGN KEY ("airline_id") REFERENCES "Airline" ("airline_id")
        ON DELETE SET NULL,

    CONSTRAINT "Flight_Routes_id_fk" FOREIGN KEY ("routes_id") REFERENCES "Routes" ("routes_id")
        ON DELETE SET NULL
);

CREATE TABLE "Class_Price"(
    "class_price_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "price" INT,
    "class_id"  INT NOT NULL,
    "flight_id"  INT NOT NULL,    
    
    CONSTRAINT "Class_Price_Class_id_fk" FOREIGN KEY ("class_id") REFERENCES "Class" ("class_id")
        ON DELETE SET NULL,

    CONSTRAINT "Class_Price_Flight_id_fk" FOREIGN KEY ("flight_id") REFERENCES "Flight" ("flight_id")
        ON DELETE SET NULL
);

CREATE TABLE "Flight_Schedule"(
    "flight_schedule_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "departure_time" DATE,
    "arrival_time" DATE,
    "flight_id"  INT NOT NULL,

    CONSTRAINT "Flight_Schedule_Flight_id_fk"
        FOREIGN KEY ("flight_id") REFERENCES "Flight" ("flight_id")
        ON DELETE SET NULL
);


CREATE TABLE "Ticket"(
    "ticket_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "status_id" INT NOT NULL,
    "user_id" INT NOT NULL,

    CONSTRAINT "Ticket_Ticket_status_id_fk"
		FOREIGN KEY ("status_id") REFERENCES "Ticket_Status" ("status_id")
		ON DELETE SET NULL,
    CONSTRAINT "Ticket_Customer_id_fk" FOREIGN KEY ("user_id") REFERENCES "Customer" ("user_id")
);

CREATE TABLE "Reservation"(
    "reservation_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "number_of_seats" INT,
    "ticket_id"  INT NOT NULL,
    "class_id"  INT NOT NULL,
    "flight_schedule_id"  INT NOT NULL,

    CONSTRAINT "Reservation_Class_id_fk"
        FOREIGN KEY ("class_id") REFERENCES "Class" ("class_id")
        ON DELETE SET NULL,
        
    CONSTRAINT "Reservation_Ticket_id_fk"
        FOREIGN KEY ("ticket_id") REFERENCES "Ticket" ("ticket_id")
        ON DELETE SET NULL,
        
    CONSTRAINT "Reservation_Flight_Schedule_id_fk"
        FOREIGN KEY ("flight_schedule_id") REFERENCES "Flight_Schedule" ("flight_schedule_id")
        ON DELETE SET NULL
);

CREATE TABLE "Seat_Configuration"(
    "seat_configuration_id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    "number_of_seats" INT,
    "airplane_id"  INT NOT NULL,
    "class_id"  INT NOT NULL,

    CONSTRAINT "Seat_Configuration_Airplane_id_fk" FOREIGN KEY ("airplane_id") REFERENCES "Airplane" ("airplane_id")
        ON DELETE SET NULL,

    CONSTRAINT "Seat_Configuration_Class_id_fk" FOREIGN KEY ("class_id") REFERENCES "Class" ("class_id")
        ON DELETE SET NULL
);

----------------------------------- TRIGGERS -----------------------------------

-- Trigger, ktery prevadi ceny jednotlivých tříd z korun na dolary
CREATE OR REPLACE TRIGGER "dolar_convertor" 
    BEFORE INSERT ON "Class_Price"
    FOR EACH ROW
BEGIN
        :NEW."price" := ROUND(:NEW."price" / 21.41);
END;
/

-- Trigger, ktery zkontroluje zda jsou data narozeni jednotlivych klientu spravna (dny v jednotlivych mesicich, ...)
CREATE OR REPLACE TRIGGER "date_of_birth_check"
    BEFORE INSERT OR UPDATE OF "date_of_birth" ON "Customer"
    FOR EACH ROW
DECLARE 
    "date_of_birth" "Customer"."date_of_birth"%TYPE;
    month varchar(2);
    day varchar(2);
    year varchar(4);
BEGIN
    "date_of_birth" := :NEW."date_of_birth";
    year := SUBSTR("date_of_birth", 8, 2);
    day := SUBSTR("date_of_birth", 1, 2);
    month := SUBSTR(TO_CHAR(TO_DATE("date_of_birth", 'DD-MON-YY'), 'MM'),1,2);
    IF( year > 24) THEN
        year := year + 1900;
    ELSE             
        year := year + 2000;
    END IF;    

    IF( year > 2024 OR year < 1900) THEN
        RAISE_APPLICATION_ERROR(-20001, 'Invalid year for date_of_birth: ');
    END IF;
        
    IF((( month = 04) OR ( month = 06) OR ( month = 09) OR ( month = 11)) AND day > 30) THEN
        RAISE_APPLICATION_ERROR(-20001, 'Invalid day');
    END IF;
    
    IF( month = 02 ) THEN
        IF( day > 29 ) THEN
            RAISE_APPLICATION_ERROR(-20001, 'Invalid day');
        ELSIF( day = 29) THEN
            IF( MOD( year, 4) != 0) THEN
                IF( MOD( year, 400) != 0) THEN
                    RAISE_APPLICATION_ERROR(-20001, 'Invalid Year');
                END IF;
            ELSIF((( MOD( year, 4) = 0) OR ( MOD( year, 400) = 0)) AND ( MOD( year,100) = 0)) THEN
                Raise_Application_Error (-20001, 'Invalid Year');
            END IF;
        END IF;
    END IF;          
END;   
/

-- Trigger, ktery pokud si nekdo rezervuje sedadlo v letadle, tak automaticky snizi pocet volnych mist (pokud nekdo rusi rezervaci, tak zvysi pocet volnych mist)
CREATE OR REPLACE TRIGGER "update_seats_available"
    AFTER INSERT OR DELETE OR UPDATE ON "Reservation"
    FOR EACH ROW
DECLARE
  num_seats_reserved INTEGER;
BEGIN
  IF (INSERTING OR UPDATING) THEN
    num_seats_reserved := :NEW."number_of_seats";
  ELSE
    num_seats_reserved := -1 * :NEW."number_of_seats";
  END IF;
   
  UPDATE "Seat_Configuration"
  SET "number_of_seats" = "number_of_seats" - num_seats_reserved
  WHERE "airplane_id" = (SELECT "airplane_id" FROM "Flight_Schedule" WHERE "flight_schedule_id" = :new."flight_schedule_id")
  AND "class_id" = :NEW."class_id";
END;
/

-- Data insert

INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('John', 'Smith', 'john.smith@example.com', TO_DATE('1980-05-15', 'yyyy-mm-dd'), '12345678');
INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('Jane', 'Doe', 'jane.doe@example.com', TO_DATE('1992-05-25', 'yyyy-mm-dd'), '87654321');
INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('Robert', 'Johnson', 'robert.johnson@example.com', TO_DATE('1975-12-01', 'yyyy-mm-dd'), '23456789');
INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('Lucy', 'Brown', 'lucy.brown@example.com', TO_DATE('1999-09-10', 'yyyy-mm-dd'), '34567890');
INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('David', 'Lee', 'david.lee@example.com', TO_DATE('1988-07-05', 'yyyy-mm-dd'), '45678901');

INSERT INTO "Travel_Agent" ("user_id", "agency_name", "commision") VALUES (1, 'ABC Travel', 10);
INSERT INTO "Travel_Agent" ("user_id", "agency_name", "commision") VALUES (2, 'XYZ Tours', 8);
INSERT INTO "Travel_Agent" ("user_id", "agency_name", "commision") VALUES (3, 'Sunshine Holidays', 12);

INSERT INTO "Ticket_Status" ("status_description") VALUES ('Pending'); -- The ticket has been requested but payment has not been completed.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Paid'); -- The ticket has been paid for and is confirmed.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Cancelled'); -- The ticket has been cancelled by either the passenger or the airline.   
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Refunded'); -- The ticket has been refunded to the passenger.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('On hold'); -- The ticket is being held for the passenger but has not yet been paid for.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Expired'); -- The ticket has expired and cannot be used.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('No show'); -- The passenger did not show up for the flight.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Check-in'); -- The passenger has checked in for the flight.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Boarding'); -- The passenger is currently boarding the flight.
INSERT INTO "Ticket_Status" ("status_description") VALUES ('Completed'); -- The flight has been completed and the passenger has arrived at their destination.

INSERT INTO "Class" ("name") VALUES ('Economy');
INSERT INTO "Class" ("name") VALUES ('Business');
INSERT INTO "Class" ("name") VALUES ('First Class');
INSERT INTO "Class" ("name") VALUES ('Premium Economy');

INSERT INTO "Airline" ("name") VALUES ('American Airlines');
INSERT INTO "Airline" ("name") VALUES ('Delta Air Lines');
INSERT INTO "Airline" ("name") VALUES ('United Airlines');
INSERT INTO "Airline" ("name") VALUES ('Southwest Airlines');

INSERT INTO "Airplane" ("type", "airline_id") VALUES ('Boeing 737', 1);
INSERT INTO "Airplane" ("type", "airline_id") VALUES ('Boeing 767', 2);
INSERT INTO "Airplane" ("type", "airline_id") VALUES ('Airbus A320', 3);
INSERT INTO "Airplane" ("type", "airline_id") VALUES ('Boeing 737', 1);

INSERT INTO "Routes" ("departure", "destination") VALUES ('New York', 'Los Angeles');
INSERT INTO "Routes" ("departure", "destination") VALUES ('Los Angeles', 'Las Vegas');
INSERT INTO "Routes" ("departure", "destination") VALUES ('Las Vegas', 'Denver');
INSERT INTO "Routes" ("departure", "destination") VALUES ('Denver', 'Chicago');
INSERT INTO "Routes" ("departure", "destination") VALUES ('Chicago', 'New York');

INSERT INTO "Flight" ("airline_id", "routes_id") VALUES (1, 1);
INSERT INTO "Flight" ("airline_id", "routes_id") VALUES (2, 2);
INSERT INTO "Flight" ("airline_id", "routes_id") VALUES (3, 3);
INSERT INTO "Flight" ("airline_id", "routes_id") VALUES (4, 4);

INSERT INTO "Class_Price" ("price", "class_id", "flight_id") VALUES (150, 1, 1);
INSERT INTO "Class_Price" ("price", "class_id", "flight_id") VALUES (200, 2, 2);
INSERT INTO "Class_Price" ("price", "class_id", "flight_id") VALUES (300, 3, 3);
INSERT INTO "Class_Price" ("price", "class_id", "flight_id") VALUES (150, 4, 4);

INSERT INTO "Flight_Schedule" ("departure_time", "arrival_time", "flight_id") VALUES (TO_DATE('2023-03-15 08:00:00', 'yyyy-mm-dd hh24:mi:ss'), TO_DATE('2023-03-15 10:00:00', 'yyyy-mm-dd hh24:mi:ss'), 1);
INSERT INTO "Flight_Schedule" ("departure_time", "arrival_time", "flight_id") VALUES (TO_DATE('2023-03-16 10:00:00', 'yyyy-mm-dd hh24:mi:ss'), TO_DATE('2023-03-16 12:00:00', 'yyyy-mm-dd hh24:mi:ss'), 2);
INSERT INTO "Flight_Schedule" ("departure_time", "arrival_time", "flight_id") VALUES (TO_DATE('2023-03-17 12:00:00', 'yyyy-mm-dd hh24:mi:ss'), TO_DATE('2023-03-17 14:00:00', 'yyyy-mm-dd hh24:mi:ss'), 3);
INSERT INTO "Flight_Schedule" ("departure_time", "arrival_time", "flight_id") VALUES (TO_DATE('2023-03-18 14:00:00', 'yyyy-mm-dd hh24:mi:ss'), TO_DATE('2023-03-18 16:00:00', 'yyyy-mm-dd hh24:mi:ss'), 4);

INSERT INTO "Ticket" ("status_id", "user_id") VALUES (2, 1);
INSERT INTO "Ticket" ("status_id", "user_id") VALUES (1, 2);
INSERT INTO "Ticket" ("status_id", "user_id") VALUES (2, 3);
INSERT INTO "Ticket" ("status_id", "user_id") VALUES (4, 4);


INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (2, 1, 1, 1);
INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (1, 2, 2, 2);
INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (6, 3, 1, 3);
INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (4, 4, 2, 4);

INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(100, 1, 1);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(200, 2, 2);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(150, 3, 3);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(50, 4, 4);

-- 4. part of project

-- Trigger [1] - Ceny jednotlivych trid jsou prevedeny na dolary (jednodussi trigger na seznameni)
SELECT "C"."name", "CK"."price" as "price_in_dolars"
FROM "Class" "C"
JOIN "Class_Price" "CK" ON "CK"."class_id" = "C"."class_id"
ORDER BY "price_in_dolars";

-- Trigger [2] - kontrola spravnosti dat. narozeni (prestupne roky, pocty dnu v mesici, ...)
SELECT "C"."first_name", "C"."last_name", "C"."date_of_birth"
FROM "Customer" "C";

-- Trigger [3] - se spusti pokazde, kdyz vkladame nebo mazeme rezervaci referencni (v tabulce Seat_configuration z 200 na 189 [class_id, airplane_id, seat_configuration_id]
UPDATE "Reservation"
SET "number_of_seats" = 11
WHERE "ticket_id" = 4 AND "class_id" = 2 AND "flight_schedule_id" = 4;

----------------------------------- PROCEDURY -----------------------------------
-- procedura, ktery vypise jmena klientu, kteri maji listek na prislusne trase show_customers_on_route(odletova destinace, priletova destinace)
CREATE OR REPLACE PROCEDURE show_customers_on_route(p_departure VARCHAR2, p_destination VARCHAR2) IS
  -- deklarace kurzoru
  CURSOR customers_on_route IS
    SELECT "c"."first_name", "c"."last_name"
    FROM "Customer" "c"
    INNER JOIN "Ticket" "t" ON "c"."user_id" = "t"."user_id"
    INNER JOIN "Reservation" "r" ON "t"."ticket_id" = "r"."ticket_id"
    INNER JOIN "Flight_Schedule" "fs" ON "r"."flight_schedule_id" = "fs"."flight_schedule_id"
    INNER JOIN "Flight" "f" ON "fs"."flight_id" = "f"."flight_id"
    INNER JOIN "Routes" "rt" ON "f"."routes_id" = "rt"."routes_id"
    WHERE "rt"."departure" = p_departure AND "rt"."destination" = p_destination;
    
  -- deklarace proměnných pro uložení jmen a příjmení
  v_first_name "Customer"."first_name"%TYPE;
  v_last_name "Customer"."last_name"%TYPE;
  
BEGIN
  -- otevření kurzoru
  OPEN customers_on_route;
  DBMS_OUTPUT.PUT_LINE('Cestujici, kteri leti na vybrane trase:');
  
  -- čtení kurzoru a uložení jmen a příjmení do proměnných
  LOOP
    FETCH customers_on_route INTO v_first_name, v_last_name;
    EXIT WHEN customers_on_route%NOTFOUND;
    
    -- výpis jmen a příjmení
    DBMS_OUTPUT.PUT_LINE(v_first_name || ' ' || v_last_name);
  END LOOP;
  
  -- uzavření kurzoru
  CLOSE customers_on_route;
  
EXCEPTION
  -- ošetření výjimky
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('Error: ' || SQLERRM);
    CLOSE customers_on_route;
END;
/

-- volani procedury vypise cestujiciho, ktery leti z Denveru(src) do Chicaga(dest) 
BEGIN
  show_customers_on_route('Denver', 'Chicago');
END;
/
-- procedura, ktera vypise vsechna jmena zakazniku, kteri jsou momentalne v databazi
CREATE OR REPLACE PROCEDURE print_customer_names
IS
  CURSOR customer_cursor IS SELECT "first_name", "last_name" FROM "Customer";
  customer_rec customer_cursor%ROWTYPE;
BEGIN
  OPEN customer_cursor;
   DBMS_OUTPUT.PUT_LINE('Jmena vsech cestujicih, kteri jsou v databazi:');
  LOOP
    FETCH customer_cursor INTO customer_rec;
    EXIT WHEN customer_cursor%NOTFOUND;
    DBMS_OUTPUT.PUT_LINE(customer_rec."first_name" || ' ' || customer_rec."last_name");
  END LOOP;
  CLOSE customer_cursor;
EXCEPTION
  WHEN OTHERS THEN
    DBMS_OUTPUT.PUT_LINE('An error occurred: ' || SQLERRM);
END;
/
-- ukazka procedury, ktera vytiskne jmena vsech cestujicich, kteri jsou momentalne v databazi
BEGIN
  print_customer_names;
END;
/

----------------------------------- EXPLAIN PLAN -----------------------------------
-- Kolik prumerne utratil zakaznik s id 2 za letenky 
-- bez optimalizace pomoci indexu
EXPLAIN PLAN FOR
    SELECT "c"."first_name", "c"."last_name", AVG("cp"."price") AS "avg"
FROM "Customer" "c"
    JOIN "Ticket" "t" ON "c"."user_id" = "t"."user_id"
    JOIN "Reservation" "r" ON "t"."ticket_id" = "r"."ticket_id"
    JOIN "Class_Price" "cp" ON "r"."class_id" = "cp"."class_id" AND "r"."flight_schedule_id" = "cp"."flight_id"
    WHERE "c"."user_id" = 2
    GROUP BY "c"."first_name", "c"."last_name"
    ORDER BY "avg" DESC;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());


-- Kolik prumerne utratil zakaznik s id 2 za letenky 
-- s optimalizaci pomoci indexu
CREATE INDEX idx_ticket_user_id ON "Ticket" ("user_id");
CREATE INDEX idx_class_price_class_id ON "Class_Price" ("class_id");


EXPLAIN PLAN FOR
    SELECT "c"."first_name", "c"."last_name", AVG("cp"."price") AS "avg"
FROM "Customer" "c"
    JOIN "Ticket" "t" ON "c"."user_id" = "t"."user_id"
    JOIN "Reservation" "r" ON "t"."ticket_id" = "r"."ticket_id"
    JOIN "Class_Price" "cp" ON "r"."class_id" = "cp"."class_id" AND "r"."flight_schedule_id" = "cp"."flight_id"
    WHERE "c"."user_id" = 2
    GROUP BY "c"."first_name", "c"."last_name"
    ORDER BY "avg" DESC;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP INDEX idx_ticket_user_id;
DROP INDEX idx_class_price_class_id;

----------------------------------- CASE WHEN  -----------------------------------
-- case when (1)
-- rozdeli pocty rezervaci do 3 trid (small, medium a large) podle poctu a vypise jmeno cestujiciho s poctem rezervaci a prislusnou tridou
SELECT "c"."first_name", "c"."last_name", "r"."number_of_seats","cl"."name",
    CASE 
        WHEN "r"."number_of_seats" <= 5 THEN 'Small'
        WHEN "r"."number_of_seats" > 5 AND "r"."number_of_seats" <= 10 THEN 'Medium'
        ELSE 'Large'
    END AS "Group Size"
FROM "Customer" "c"
    INNER JOIN "Ticket" "t" ON "c"."user_id" = "t"."user_id"
    INNER JOIN "Reservation" "r" ON "t"."ticket_id" = "r"."ticket_id"
    INNER JOIN "Class" "cl" ON "r"."class_id" = "cl"."class_id"
ORDER BY "Group Size" DESC;


-- case when (2)
-- vrati cislo rezervace, pocet sedadel, tridu letu, id letu, nazev letecke spolecnosti, cenu tridy, pocet dostupnych sedadel (nerezervovanych) a celkovou cenu
-- komplikovanejsi case -> cena za rezervaci (stanovili jsme si ze rezervace 1 tridy je 2* drazsi a rezervace business tridy je 1.5* drazsi nez v ekonomicke tride)
-- celkova cena se tedy stanovi v case na zaklade poctu rezervovanych sedadel, ceny tridy * konstanta (poplate za rezervaci))
-- sub query vypoceta pocet volnych mist
SELECT "r"."reservation_id", "r"."number_of_seats", "c"."name" as "class_name", "f"."flight_id", "a"."name" as "airline_name", "rp"."price" as "class_price", "ac"."number_of_seats" as "seats_available",
    CASE
        WHEN "r"."class_id" = 1 THEN "rp"."price" * "r"."number_of_seats"
        WHEN "r"."class_id" = 2 THEN ("rp"."price" * "r"."number_of_seats") * 1.5
        WHEN "r"."class_id" = 3 THEN ("rp"."price" * "r"."number_of_seats") * 2
    END AS "total_price"
FROM "Reservation" "r"
    JOIN "Flight_Schedule" "fs" ON "r"."flight_schedule_id" = "fs"."flight_schedule_id"
    JOIN "Class" "c" ON "r"."class_id" = "c"."class_id"
    JOIN "Flight" "f" ON "fs"."flight_id" = "f"."flight_id"
    JOIN "Airline" "a" ON "f"."airline_id" = "a"."airline_id"
    JOIN "Class_Price" "rp" ON "r"."flight_schedule_id" = "rp"."flight_id" AND "r"."class_id" = "rp"."class_id"
    JOIN (SELECT "sc"."airplane_id", "sc"."class_id", SUM("sc"."number_of_seats") AS "number_of_seats"
        FROM "Seat_Configuration" "sc"
        GROUP BY "sc"."airplane_id", "sc"."class_id"
    ) "ac" ON "f"."airline_id" = "ac"."airplane_id" AND "r"."class_id" = "ac"."class_id"
ORDER BY "r"."reservation_id";

----------------------------------- MATERIALIZOVANY POHLED -----------------------------------
CREATE MATERIALIZED VIEW "Agent_view" REFRESH ON COMMIT AS
SELECT "C"."first_name", "C"."last_name", "T"."commision"
FROM "Customer" "C"
INNER JOIN "Travel_Agent" "T" ON "T"."user_id" = "C"."user_id";

-- vypis materializovaneho pohledu
SELECT * FROM "Agent_view";

-- update comisi agenta s id 1
UPDATE "Travel_Agent" SET "commision" = 19 WHERE "user_id" = 1;
COMMIT; 

-- ukazka, ze aktualizace probehne i v materializovanem pohledu
SELECT * FROM "Agent_view";
----------------------------------- PRAVA A ROLE-----------------------------------
-- udeleni vsech prav danemu uzivateli
-- udeleni prav (SELECT, INSERT, UPDATE, DELETE, ...) 
GRANT ALL ON "Travel_Agent" TO xhemza05;
GRANT ALL ON "Seat_Configuration" TO xhemza05;
GRANT ALL ON "Reservation" TO xhemza05;
GRANT ALL ON "Ticket" TO xhemza05;
GRANT ALL ON "Flight_Schedule" TO xhemza05;
GRANT ALL ON "Class_Price" TO xhemza05;
GRANT ALL ON "Flight" TO xhemza05;
GRANT ALL ON "Airplane" TO xhemza05;
GRANT ALL ON "Routes" TO xhemza05;
GRANT ALL ON "Airline" TO xhemza05;
GRANT ALL ON "Class" TO xhemza05;
GRANT ALL ON "Ticket_Status" TO xhemza05;
GRANT ALL ON "Customer" TO xhemza05;
GRANT ALL ON "Agent_view" TO xhemza05; -- materializovany pohled druheho clena tymu

-- udeleni prav ke spousteni procedur
GRANT EXECUTE ON print_customer_names TO xhemza05;
GRANT EXECUTE ON show_customers_on_route TO xhemza05;

----------------------------------- TRANSAKCE -----------------------------------
-- transakce(1)
-- tato transakce vlozi noveho zakaznika "Johna Doe" a pote vytvori jeho listek se statusem "unbooked" a vytvori mu 1 rezervaci do ekonomicke tridy a ticket status nastavi na booked 
-- atomicnost - BEGIN(zajistit provadeni kodu jako celku), COMMIT(dokonci transakci) a ROLLBACK(v pripade nedokonceni transakce vrati zpet zmeny a udrzi konzistenci databaze)
DECLARE
  v_ticket_id INT;
  v_reservation_id INT;
BEGIN
  -- Insert a new customer
  INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number")
  VALUES ('John', 'Doe', 'johndoe@example.com', TO_DATE('1990-01-01', 'YYYY-MM-DD'), '12545678');

  -- Create a new ticket
  INSERT INTO "Ticket" ("status_id", "user_id")
  VALUES (1, (SELECT MAX("user_id") FROM "Customer"))
  RETURNING "ticket_id" INTO v_ticket_id;

  -- Reserve a seat on a flight
  INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id")
  VALUES (1, v_ticket_id, 1, 1)
  RETURNING "reservation_id" INTO v_reservation_id;

  -- Update the ticket status to "booked"
  UPDATE "Ticket" SET "status_id" = 2 WHERE "ticket_id" = v_ticket_id;

  COMMIT;
EXCEPTION
  WHEN OTHERS THEN
    ROLLBACK;
    RAISE;
END;
/


-- transakce(2)
-- vybereme radek se zakaznikem, ktery ma ID 1 a ten uzamkneme (Pote zmenime jmeno zakaznika na Jirka a vypiseme zakaznika do Dbms Outputu a commitneme zmeny)
-- FOR UPDATE (uzamkene vybrany radek, aby s nim mohl pracovat pouze 1 uzivatel)
DECLARE
  v_customer_id INT;
  v_customer_name VARCHAR2(50);
BEGIN
  -- Find a customer to update
  SELECT "user_id", "first_name" || ' ' || "last_name" INTO v_customer_id, v_customer_name
  FROM "Customer"
  WHERE "user_id" = 1
  FOR UPDATE;

  -- Update the customer's name
  UPDATE "Customer" SET "first_name" = 'Jirka' WHERE "user_id" = v_customer_id;

 
  COMMIT;
    -- Print the updated customer name
   DBMS_OUTPUT.PUT_LINE('zakaznikovi s ID ' || v_customer_id || 'byolo zmeneno krestni jmeno  ' || v_customer_name);
EXCEPTION
  WHEN OTHERS THEN
    ROLLBACK;
    RAISE;
END;
/

