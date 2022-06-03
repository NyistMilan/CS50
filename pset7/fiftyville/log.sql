-- Keep a log of any SQL queries you execute as you solve the mystery.

--Looking through the Crime Scene Reports to read some useful information
--#######################################################################
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time
-- – each of their interview transcripts mentions the bakery.
--#######################################################################
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street = "Humphrey Street";

--Looking for Interviews that mention "bakery" on the date of the theft
--#######################################################################
--| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.|
--| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.|
--| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.|
--| Emma    | I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.
--#######################################################################
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";

--Looking at bakery security logs because Ruth said we could get the car's info
--+----------+---------------+--------+
--| activity | license_plate | minute |
--+----------+---------------+--------+
--| exit     | 5P2BI95       | 16     |
--| exit     | 94KL13X       | 18     |
--| exit     | 6P58WS2       | 18     |
--| exit     | 4328GD8       | 19     |
--| exit     | G412CB7       | 20     |
--| exit     | L93JTIZ       | 21     |
--| exit     | 322W7JE       | 23     |
--| exit     | 0NTHK55       | 23     |
--+----------+---------------+--------+
SELECT activity, license_plate, minute FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;

--Looking for clues at ATMs from Eugene's Interview
--+----------------+--------+
--| account_number | amount |
--+----------------+--------+
--| 28500762       | 48     |
--| 28296815       | 20     |
--| 76054385       | 60     |
--| 49610011       | 50     |
--| 16153065       | 80     |
--| 25506511       | 20     |
--| 81061156       | 30     |
--| 26013199       | 35     |
--+----------------+--------+
SELECT account_number, amount FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

--Looking for short phone calls
--+----------------+----------------+
--|     caller     |    receiver    |
--+----------------+----------------+
--| (130) 555-0289 | (996) 555-8899 |
--| (499) 555-9472 | (892) 555-8872 |
--| (367) 555-5533 | (375) 555-8161 |
--| (499) 555-9472 | (717) 555-1342 |
--| (286) 555-6063 | (676) 555-6554 |
--| (770) 555-1861 | (725) 555-3243 |
--| (031) 555-6622 | (910) 555-3251 |
--| (826) 555-1652 | (066) 555-9701 |
--| (338) 555-6650 | (704) 555-2131 |
--+----------------+----------------+
SELECT caller, receiver FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;

--Looking for passport number for flight
SELECT passport_number FROM passengers, flights, airports
WHERE passengers.flight_id = flights.id AND airports.id = flights.origin_airport_id AND year = 2021 AND month = 7 AND day = 29 AND hour IN (SELECT MIN(hour) FROM flights WHERE year = 2021 AND month = 7 AND day = 29) AND minute IN (SELECT MIN(minute) FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND hour IN (SELECT MIN(hour) FROM flights WHERE year = 2021 AND month = 7 AND day = 29));

--Looking at bank_accounts
SELECT person_id FROM bank_accounts
WHERE account_number IN (SELECT account_number FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw");

--Looking for people with those informations
--+--------+-------+----------------+-----------------+---------------+
--|   id   | name  |  phone_number  | passport_number | license_plate |
--+--------+-------+----------------+-----------------+---------------+
--| 686048 | Bruce | (367) 555-5533 | 5773159633      | 94KL13X       |
--+--------+-------+----------------+-----------------+---------------+
SELECT id, name, phone_number, passport_number, license_plate FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60) AND license_plate IN (SELECT license_plate FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25) AND passport_number IN (SELECT passport_number FROM passengers, flights, airports
WHERE passengers.flight_id = flights.id AND airports.id = flights.origin_airport_id AND year = 2021 AND month = 7 AND day = 29 AND hour IN (SELECT MIN(hour) FROM flights WHERE year = 2021 AND month = 7 AND day = 29) AND minute IN (SELECT MIN(minute) FROM flights WHERE year = 2021 AND month = 7 AND day = 29 AND hour IN (SELECT MIN(hour) FROM flights WHERE year = 2021 AND month = 7 AND day = 29))) AND people.id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"));

--Looking for where Bruce is going
--He is going to New York City
SELECT abbreviation, full_name, city FROM airports, flights, passengers
WHERE passengers.flight_id = flights.id AND passengers.passport_number = 5773159633 AND flights.destination_airport_id = airports.id;

--Looking for Bruce's ACCOMPLICE's phone
--+----------------+
--|    receiver    |
--+----------------+
--| (375) 555-8161 |
--+----------------+
SELECT receiver FROM phone_calls
WHERE caller = "(367) 555-5533" AND year = 2021 AND month = 7 AND day = 28 AND duration < 60;

--Looking for the accomplice's name
--+-------+
--| name  |
--+-------+
--| Robin |
--+-------+
SELECT name FROM people
WHERE phone_number IN (SELECT receiver FROM phone_calls
WHERE caller = "(367) 555-5533" AND year = 2021 AND month = 7 AND day = 28 AND duration < 60);



