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
-- Name: get_uid(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION get_uid(thash character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
declare
res integer;
begin
select uid into res from uid where hash = decode(thash,'hex');
if not found then
select insert_uid(thash) into res;
end if;
return res;
end
;$$;


ALTER FUNCTION public.get_uid(thash character varying) OWNER TO postgres;

--
-- Name: FUNCTION get_uid(thash character varying); Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON FUNCTION get_uid(thash character varying) IS 'Return uid for a given MAC address.
If MAC not present, then call insert_uid and return the uid assigned.';


--
-- Name: insert_uid(character varying); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION insert_uid(thash character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$declare
id integer;
begin
insert into uid(hash) values (decode(thash,'hex'));
select uid into id from uid where hash = decode(thash,'hex');
return id;
end;$$;


ALTER FUNCTION public.insert_uid(thash character varying) OWNER TO postgres;

--
-- Name: FUNCTION insert_uid(thash character varying); Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON FUNCTION insert_uid(thash character varying) IS 'Insert a new record for a given mac hash.
Returns new uid.
To be called only when the hash is not already present.';


--
-- Name: log_insert(character varying, character varying, timestamp without time zone, character varying, integer); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION log_insert(tdevice character varying, tclient character varying, tts timestamp without time zone, tlabel character varying, ttype integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
declare
id1 integer;
id2 integer;
begin
select get_uid(tdevice) into id1;
select get_uid(tclient) into id2;
insert into logs(device_id,client_id,ts,label,type)
values (id1,id2,tts,tlabel,ttype);
return 1;
end
;
$$;


ALTER FUNCTION public.log_insert(tdevice character varying, tclient character varying, tts timestamp without time zone, tlabel character varying, ttype integer) OWNER TO postgres;

