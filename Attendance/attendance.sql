--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- Name: all_attendance(character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION all_attendance(character varying, character varying, character varying) RETURNS TABLE(rollno character varying, attendance_date date, status bit)
    LANGUAGE plpgsql
    AS $_$
DECLARE
from_date date;
to_date date;
BEGIN
from_date := to_timestamp($1,$3)::date;
to_date := to_timestamp($2,$3)::date;
RETURN QUERY (SELECT a.rollno as rr ,a.date as dd,a.present FROM attendance a 
WHERE a.date >= from_date and a.date <= to_date and
a.date NOT IN (SELECT * FROM positive_exceptions) and 
extract (dow from a.date) != 0 and extract(dow from a.date) != 6
UNION
SELECT b.rollno as rr,b.date as dd,b.present from attendance b
WHERE b.date >= from_date AND b.date <= to_date and
b.date IN (SELECT * FROM negative_exceptions))
ORDER BY rr,dd;
END
$_$;


ALTER FUNCTION public.all_attendance(character varying, character varying, character varying) OWNER TO postgres;

--
-- Name: check_exception_present(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION check_exception_present() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
present int;
present2 int;
BEGIN
select count(*) into present FROM positive_exceptions p WHERE p.date = NEW.date;
select count(*) into present2 FROM negative_exceptions n WHERE n.date = NEW.date;
IF present = 1 and present2 < 1 THEN
  DELETE FROM positive_exceptions p WHERE p.date = NEW.date;
  RETURN NEW;
ELSIF present < 1 and present2 = 1 THEN
  DELETE FROM negative_exceptions n WHERE n.date = NEW.date;
  RETURN NEW;
ELSIF present = 0 and present2 = 0 THEN
  RETURN NEW;
END IF;
RETURN NULL;
END;
$$;


ALTER FUNCTION public.check_exception_present() OWNER TO postgres;

--
-- Name: del_exception(text, text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION del_exception(text, text) RETURNS void
    LANGUAGE plpgsql
    AS $_$
DECLARE
del_date date;
BEGIN
del_date := to_timestamp($1,$2)::date;
DELETE FROM positive_exceptions p WHERE p.date = del_date;
DELETE FROM negative_exceptions n WHERE n.date = del_date;
END;
$_$;


ALTER FUNCTION public.del_exception(text, text) OWNER TO postgres;

--
-- Name: del_ta(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION del_ta(character varying) RETURNS void
    LANGUAGE plpgsql
    AS $_$
BEGIN
DELETE FROM attendance WHERE rollno = $1;
DELETE FROM ta_macs WHERE rollno = $1;
DELETE FROM ta_info WHERE rollno = $1;
END
$_$;


ALTER FUNCTION public.del_ta(character varying) OWNER TO postgres;

--
-- Name: del_ta_mac(character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION del_ta_mac(character varying, character varying) RETURNS void
    LANGUAGE plpgsql
    AS $_$
BEGIN
DELETE FROM ta_macs WHERE rollno = $1 and mac = $2;
END
$_$;


ALTER FUNCTION public.del_ta_mac(character varying, character varying) OWNER TO postgres;

--
-- Name: get_attendance_email(character varying, character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_attendance_email(character varying, character varying, character varying, character varying) RETURNS TABLE(attendance_date date, status bit)
    LANGUAGE plpgsql
    AS $_$
DECLARE
from_date date;
to_date date;
p_rollno varchar;
BEGIN
from_date := to_timestamp($1,$3)::date;
to_date := to_timestamp($2,$3)::date;
SELECT i.rollno INTO p_rollno FROM ta_info i WHERE email = lower($4);
RETURN QUERY (SELECT a.date as dd,a.present from attendance a 
WHERE a.rollno = p_rollno and 
a.date >= from_date and a.date <= to_date and 
a.date NOT IN (SELECT * FROM positive_exceptions) and 
extract(dow from a.date) != 0 and extract(dow from a.date) != 6 
UNION
SELECT b.date as dd,b.present from attendance b 
WHERE b.rollno = p_rollno and
b.date >= from_date AND b.date <= to_date and
b.date IN (SELECT * FROM negative_exceptions))
ORDER BY dd;
END
$_$;


ALTER FUNCTION public.get_attendance_email(character varying, character varying, character varying, character varying) OWNER TO postgres;

--
-- Name: get_info_all(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_info_all() RETURNS TABLE(rollno character varying, name character varying, email character varying, batch character varying)
    LANGUAGE plpgsql
    AS $$
DECLARE
BEGIN
RETURN QUERY SELECT i.rollno,i.name,i.email,i.batch FROM ta_info i;
END
$$;


ALTER FUNCTION public.get_info_all() OWNER TO postgres;

--
-- Name: get_info_email(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_info_email(text) RETURNS TABLE(rollno character varying, name character varying, email character varying, batch character varying)
    LANGUAGE plpgsql
    AS $_$
DECLARE
BEGIN
RETURN QUERY SELECT ta_info.rollno,ta_info.name,ta_info.email,ta_info.batch FROM ta_info WHERE ta_info.email = $1;
END
$_$;


ALTER FUNCTION public.get_info_email(text) OWNER TO postgres;

--
-- Name: get_info_rollno(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_info_rollno(text) RETURNS TABLE(rollno character varying, name character varying, email character varying, batch character varying)
    LANGUAGE plpgsql
    AS $_$
DECLARE
BEGIN
RETURN QUERY SELECT i.rollno,i.name,i.email,i.batch FROM ta_info i WHERE  i.rollno = $1;
END
$_$;


ALTER FUNCTION public.get_info_rollno(text) OWNER TO postgres;

--
-- Name: get_mac_email(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_mac_email(text) RETURNS TABLE(macs character varying)
    LANGUAGE plpgsql
    AS $_$
DECLARE
roll_no varchar;
BEGIN
SELECT rollno into roll_no FROM ta_info WHERE email = $1;
RETURN QUERY SELECT mac FROM ta_macs WHERE rollno = roll_no;
END
$_$;


ALTER FUNCTION public.get_mac_email(text) OWNER TO postgres;

--
-- Name: get_mac_rollno(text); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_mac_rollno(text) RETURNS TABLE(macs character varying)
    LANGUAGE plpgsql
    AS $_$
DECLARE
BEGIN
RETURN QUERY SELECT mac FROM ta_macs WHERE rollno = $1;
END
$_$;


ALTER FUNCTION public.get_mac_rollno(text) OWNER TO postgres;

--
-- Name: get_macs_all(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_macs_all() RETURNS TABLE(rollno character varying, mac character varying)
    LANGUAGE plpgsql
    AS $$
DECLARE
BEGIN
RETURN QUERY SELECT m.rollno,m.mac FROM ta_macs m;
END
$$;


ALTER FUNCTION public.get_macs_all() OWNER TO postgres;

--
-- Name: get_negative_exceptions(character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_negative_exceptions(character varying, character varying, character varying) RETURNS TABLE(dates date)
    LANGUAGE plpgsql
    AS $_$
DECLARE
from_date date;
to_date date;
BEGIN
  from_date := to_date($1,$3);
  to_date := to_date($2,$3);
  RETURN QUERY SELECT * FROM negative_exceptions p 
  WHERE p.date <= to_date and p.date >= from_date
  ORDER BY p.date;
END;
$_$;


ALTER FUNCTION public.get_negative_exceptions(character varying, character varying, character varying) OWNER TO postgres;

--
-- Name: get_positive_exceptions(character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_positive_exceptions(character varying, character varying, character varying) RETURNS TABLE(dates date)
    LANGUAGE plpgsql
    AS $_$
DECLARE
from_date date;
to_date date;
BEGIN
  from_date := to_date($1,$3);
  to_date := to_date($2,$3);
  RETURN QUERY SELECT * FROM positive_exceptions p 
  WHERE p.date <= to_date and p.date >= from_date
  ORDER BY p.date;
END;
$_$;


ALTER FUNCTION public.get_positive_exceptions(character varying, character varying, character varying) OWNER TO postgres;

--
-- Name: put_exception(text, text, bit); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION put_exception(text, text, bit) RETURNS void
    LANGUAGE plpgsql
    AS $_$
DECLARE
exception_date date;
BEGIN
exception_date := to_timestamp($1,$2)::date;
IF $3 = '0' THEN
  INSERT INTO positive_exceptions VALUES(exception_date);
ELSE
  INSERT INTO negative_exceptions VALUES(exception_date);
END IF;
END;
$_$;


ALTER FUNCTION public.put_exception(text, text, bit) OWNER TO postgres;

--
-- Name: put_mac(character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION put_mac(character varying, character varying) RETURNS void
    LANGUAGE plpgsql
    AS $_$
BEGIN
INSERT INTO ta_macs(rollno,mac) VALUES ($1,$2);END
$_$;


ALTER FUNCTION public.put_mac(character varying, character varying) OWNER TO postgres;

--
-- Name: put_ta(character varying, character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION put_ta(character varying, character varying, character varying, character varying) RETURNS void
    LANGUAGE plpgsql
    AS $_$
DECLARE
rollno varchar;
BEGIN
SELECT lower(i.rollno) INTO rollno FROM ta_info i WHERE lower(i.rollno) = lower($1);
IF rollno is NULL THEN
INSERT INTO ta_info(rollno,email,name,batch) VALUES(lower($1),lower($2),lower($3),lower($4));
ELSE 
UPDATE ta_info SET email = lower($2) and name = lower($3) and batch = lower($4) WHERE lower(ta_info.rollno) = lower(rollno);
END IF;END
$_$;


ALTER FUNCTION public.put_ta(character varying, character varying, character varying, character varying) OWNER TO postgres;

--
-- Name: update_attendance(character varying, character varying, character varying, bit); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION update_attendance(character varying, character varying, character varying, bit) RETURNS void
    LANGUAGE plpgsql
    AS $_$
DECLARE
attendance_date date;
present int;
BEGIN
attendance_date := to_timestamp($2,$3)::date;
SELECT count(*) INTO present FROM attendance a WHERE a.rollno = $1 AND a.date = attendance_date;
IF present = 0 THEN
  INSERT INTO attendance(rollno,date,present) VALUES($1,attendance_date,$4);
ELSE
  UPDATE attendance a SET present = $4 WHERE a.rollno = $1 and a.date = attendance_date;
END IF;
END
$_$;


ALTER FUNCTION public.update_attendance(character varying, character varying, character varying, bit) OWNER TO postgres;

--
-- Name: update_attendance_timing(character varying, time without time zone, time without time zone); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION update_attendance_timing(character varying, time without time zone, time without time zone) RETURNS void
    LANGUAGE plpgsql
    AS $_$
DECLARE
present int;
BEGIN
SELECT count(*) INTO present FROM attendance_timings WHERE batch = $1;
IF present = 0 THEN
  INSERT INTO attendance_timings(batch,from_time,to_time) VALUES($1,$2,$3);
ELSE
  UPDATE attendance_timings SET from_time = $2 and to_time = $3 WHERE batch = $1;
END IF;
END;
$_$;


ALTER FUNCTION public.update_attendance_timing(character varying, time without time zone, time without time zone) OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: attendance; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE attendance (
    rollno character varying(30),
    date date NOT NULL,
    present bit(1) DEFAULT B'0'::"bit"
);


ALTER TABLE public.attendance OWNER TO postgres;

--
-- Name: attendance_timings; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE attendance_timings (
    batch character varying(20) NOT NULL,
    from_time time without time zone,
    to_time time without time zone
);


ALTER TABLE public.attendance_timings OWNER TO postgres;

--
-- Name: negative_exceptions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE negative_exceptions (
    date date NOT NULL
);


ALTER TABLE public.negative_exceptions OWNER TO postgres;

--
-- Name: positive_exceptions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE positive_exceptions (
    date date NOT NULL
);


ALTER TABLE public.positive_exceptions OWNER TO postgres;

--
-- Name: ta_info; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE ta_info (
    rollno character varying(30) NOT NULL,
    name character varying(80),
    email character varying(80) NOT NULL,
    batch character varying(20)
);


ALTER TABLE public.ta_info OWNER TO postgres;

--
-- Name: ta_macs; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE ta_macs (
    rollno character varying(30) NOT NULL,
    mac character varying(30) NOT NULL
);


ALTER TABLE public.ta_macs OWNER TO postgres;

--
-- Name: attendance_timings_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY attendance_timings
    ADD CONSTRAINT attendance_timings_pkey PRIMARY KEY (batch);


--
-- Name: negative_exceptions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY negative_exceptions
    ADD CONSTRAINT negative_exceptions_pkey PRIMARY KEY (date);


--
-- Name: positive_exceptions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY positive_exceptions
    ADD CONSTRAINT positive_exceptions_pkey PRIMARY KEY (date);


--
-- Name: ta_info_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY ta_info
    ADD CONSTRAINT ta_info_pkey PRIMARY KEY (rollno);


--
-- Name: ta_macs_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY ta_macs
    ADD CONSTRAINT ta_macs_pkey PRIMARY KEY (rollno, mac);


--
-- Name: attendance_date_idx; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX attendance_date_idx ON attendance USING btree (date);


--
-- Name: check_negative_exception_insert; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER check_negative_exception_insert BEFORE INSERT OR UPDATE ON negative_exceptions FOR EACH ROW EXECUTE PROCEDURE check_exception_present();


--
-- Name: check_positive_exception_insert; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER check_positive_exception_insert BEFORE INSERT OR UPDATE ON positive_exceptions FOR EACH ROW EXECUTE PROCEDURE check_exception_present();


--
-- Name: attendance_rollno_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY attendance
    ADD CONSTRAINT attendance_rollno_fkey FOREIGN KEY (rollno) REFERENCES ta_info(rollno);


--
-- Name: ta_macs_rollno_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY ta_macs
    ADD CONSTRAINT ta_macs_rollno_fkey FOREIGN KEY (rollno) REFERENCES ta_info(rollno);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

