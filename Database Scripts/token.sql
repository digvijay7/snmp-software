--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: tokentable; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tokentable (
    username text NOT NULL,
    expiration integer NOT NULL,
    token bytea NOT NULL
);


ALTER TABLE public.tokentable OWNER TO postgres;

--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE users (
    username text NOT NULL,
    password bytea NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: tokentable_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY tokentable
    ADD CONSTRAINT tokentable_pkey PRIMARY KEY (username);


--
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_pkey PRIMARY KEY (username);


--
-- PostgreSQL database dump complete
--

