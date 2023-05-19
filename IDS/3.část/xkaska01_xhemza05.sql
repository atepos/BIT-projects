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

-- Data insert

INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('John', 'Smith', 'john.smith@example.com', TO_DATE('1980-05-15', 'yyyy-mm-dd'), '12345678');
INSERT INTO "Customer" ("first_name", "last_name", "email", "date_of_birth", "passport_number") VALUES ('Jane', 'Doe', 'jane.doe@example.com', TO_DATE('1992-02-22', 'yyyy-mm-dd'), '87654321');
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

INSERT INTO "Class_Price" ("price", "class_id", "flight_id") VALUES (100, 1, 1);
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
INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (3, 3, 1, 3);
INSERT INTO "Reservation" ("number_of_seats", "ticket_id", "class_id", "flight_schedule_id") VALUES (4, 4, 2, 4);

INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(300, 1, 2);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(100, 1, 1);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(200, 2, 2);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(150, 3, 3);
INSERT INTO "Seat_Configuration"("number_of_seats", "airplane_id", "class_id") VALUES(50, 4, 4);

-- 3. part of the project 

-- spojeni dvou tabulek
-- Ktere typy letadel letaji spolecnosti American Airlines?
-- type 
SELECT "Ap"."type" AS "Airplane"
FROM "Airplane" "Ap"
JOIN "Airline" "Ar" ON "Ar"."airline_id" = "Ap"."airline_id"
WHERE "Ar"."name" = 'American Airlines'
ORDER BY "Airplane";

-- spojeni dvou tabulek
-- Které třídy mají 100 sedadel?
-- name 
SELECT "C"."name" AS "name"
FROM "Class" "C"
JOIN "Seat_Configuration" "S" ON "S"."seat_configuration_id" = "C"."class_id"
WHERE "S"."number_of_seats" = 100
ORDER BY "name";

-- spojeni tří tabulek
-- Jaký je status lístků, které jsou zarezervovány v jednotlivých třídách?
-- (name, status)
SELECT
    "T"."status_id" AS "ticket_status",
    "C"."name" AS "class_name"
FROM "Reservation" "R"
JOIN "Ticket" "T" ON "T"."ticket_id" = "R"."ticket_id"
JOIN "Class" "C" ON "C"."class_id" = "R"."class_id"
ORDER BY "class_name", "ticket_status";

-- GROUP BY s agregacni funkci
-- Které aerolinky mají více než dvě letadla určitého typu?
-- (pocet, name, type)
SELECT 
    "AP"."type" AS "type",
    COUNT ("AP"."airplane_id") AS "count",
    "AR"."name" AS "name"
FROM "Airline" "AR"
JOIN "Airplane" "AP" ON "AP"."airline_id" = "AR"."airline_id"
GROUP BY "AR"."airline_id", "AP"."type", "AR"."name"
HAVING  COUNT ("AP"."airplane_id") > 1 
ORDER BY "AR"."name";

-- GROUP BY s agregacni funkci
-- Který typ letadla má nejvíce tříd a kolik?
-- plane_type
SELECT 
    "A"."type" AS "plane_type",
    COUNT("S"."seat_configuration_id") AS "count"
FROM "Airplane" "A"
JOIN "Seat_Configuration" "S" ON "S"."airplane_id" = "A"."airplane_id"
GROUP BY "A"."airplane_id", "A"."type"
HAVING COUNT ("S"."seat_configuration_id") >= ALL (
    SELECT COUNT ("S"."seat_configuration_id")
    FROM "Seat_Configuration" "S"
    GROUP BY "S"."airplane_id"
)
ORDER BY "plane_type";

-- Dotaz s predikatorem EXISTS
-- existuje v databázi zákazník, který má již zakoupený lístek?
-- krestni jmeno a id zakaznika
SELECT 
    "C"."first_name" as "first_name",
    "C"."user_id" as "user_id"
FROM "Customer" "C"
WHERE EXISTS (
    SELECT *
    FROM "Ticket" "T"
    WHERE "T"."user_id" = "C"."user_id" AND "T"."status_id" = '2'
)
ORDER BY "first_name";

-- Dotaz s predikatorem IN s vnorenym select
-- Kteri zakaznici jsou zaroven i Cestovni agenti?
-- last_name, first_name

SELECT 
    "C"."first_name" AS "first_name",
    "C"."last_name" AS "last_name"
FROM "Customer" "C"
WHERE "C"."user_id" IN(
    SELECT "T"."user_id" 
    FROM "Travel_Agent" "T"
)
ORDER BY "last_name";
