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
insert into logs(device,client,ts,label,type)
values (id1,id2,tts,tlabel,ttype);
return 1;
end
;
$$;


ALTER FUNCTION public.log_insert(tdevice character varying, tclient character varying, tts timestamp without time zone, tlabel character varying, ttype integer) OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: logs; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE logs (
    device integer NOT NULL,
    ts timestamp without time zone NOT NULL,
    label character varying(30),
    type integer,
    client_id integer DEFAULT 2
);


ALTER TABLE public.logs OWNER TO postgres;

--
-- Name: tokentable; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tokentable (
    username text NOT NULL,
    token bytea NOT NULL,
    expiration integer NOT NULL
);


ALTER TABLE public.tokentable OWNER TO postgres;

--
-- Name: uid; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE uid (
    uid integer NOT NULL,
    hash bytea NOT NULL,
    rollno integer,
    batch character varying,
    access integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.uid OWNER TO postgres;

--
-- Name: uid_uid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE uid_uid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.uid_uid_seq OWNER TO postgres;

--
-- Name: uid_uid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE uid_uid_seq OWNED BY uid.uid;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE users (
    username text NOT NULL,
    password bytea NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: uid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY uid ALTER COLUMN uid SET DEFAULT nextval('uid_uid_seq'::regclass);


--
-- Data for Name: logs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY logs (device, ts, label, type, client_id) FROM stdin;
1	2012-12-23 01:01:01	abc	1	2
\.


--
-- Data for Name: tokentable; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tokentable (username, token, expiration) FROM stdin;
\.


--
-- Data for Name: uid; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY uid (uid, hash, rollno, batch, access) FROM stdin;
\.


--
-- Name: uid_uid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('uid_uid_seq', 4125, true);


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY users (username, password) FROM stdin;
harkirat	\\xc62f2143e0a623f0287c2945ef26d9ca119607be548998d2320e0089224c3db0
garvita	\\xc62f2143e0a623f0287c2945ef26d9ca119607be548998d2320e0089224c3db0
digvijay	\\xc62f2143e0a623f0287c2945ef26d9ca119607be548998d2320e0089224c3db0
\.


--
-- Name: tokentable_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tokentable
    ADD CONSTRAINT tokentable_pkey PRIMARY KEY (username);


--
-- Name: uid_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY uid
    ADD CONSTRAINT uid_pkey PRIMARY KEY (hash);


--
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_pkey PRIMARY KEY (username);


--
-- Name: uid_hash_idx; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX uid_hash_idx ON uid USING btree (hash);


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
