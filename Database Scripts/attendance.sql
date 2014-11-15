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
-- Data for Name: attendance; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY attendance (rollno, date, present) FROM stdin;
\.


--
-- Data for Name: ta_info; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY ta_info (rollno, name, email, batch) FROM stdin;
mt13097	juhi pruthi	juhi1397@iiitd.ac.in	mtech13
mt13098	khalique newaz	khalique1398@iiitd.ac.in	mtech13
mt13099	prerna juneja	prerna1399@iiitd.ac.in	mtech13
mt13100	priyanka	priyanka13100@iiitd.ac.in	mtech13
mt13101	rahul kumar singh	rahul13101@iiitd.ac.in	mtech13
mt12004	aritra dhar	1	extended
mt13105	sanya malhotra	sanya13105@iiitd.ac.in	mtech13
mt13106	mahek shah	mahek13106@iiitd.ac.in	mtech13
mt13107	shivam chaturvedi	shivam13107@iiitd.ac.in	mtech13
mt13108	sonam rathore	sonam13108@iiitd.ac.in	mtech13
mt13127	kishor singh rawat	kishor13127@iiitd.ac.in	mtech13
mt13132	priya aggarwal	priya13132@iiitd.ac.in	mtech13
mt13133	priya singhal	priya13133@iiitd.ac.in	mtech13
mt13138	sakshi narula	sakshi13138@iiitd.ac.in	mtech13
mt13153	divya kaushik	divya13153@iiitd.ac.in	mtech13
mt13155	himanshi saini	himanshi13155@iiitd.ac.in	mtech13
mt13157	namrata singh	namrata13157@iiitd.ac.in	mtech13
mt13158	neha jain	neha13158@iiitd.ac.in	mtech13
mt13159	nilesh chander varshney	nilesh13159@iiitd.ac.in	mtech13
mt13162	vandana mittal	vandana13162@iiitd.ac.in	mtech13
mt14001	amandeep	amandeep1401@iiitd.ac.in	mtech14
mt14002	anurag chowdhury	anurag1402@iiitd.ac.in	mtech14
mt14003	ashish bandil	ashish1403@iiitd.ac.in	mtech14
mt14004	avanish kumar singh	avanish1404@iiitd.ac.in	mtech14
mt14005	deepak das	deepak1405@iiitd.ac.in	mtech14
mt14006	garima	garima1406@iiitd.ac.in	mtech14
mt14007	garima mahajan	garima1407@iiitd.ac.in	mtech14
mt14008	gullal singh cheema	gullal1408@iiitd.ac.in	mtech14
mt14009	hitanshu rakeshkumar tiwari	hitanshu1409@iiitd.ac.in	mtech14
mt14010	joy aneja	joy1410@iiitd.ac.in	mtech14
mt14011	kondamadugula raghunath reddy	kondamadugula1411@iiitd.ac.in	mtech14
mt14013	manisha agrawal	manisha1413@iiitd.ac.in	mtech14
mt14014	manisha dubey	manisha1414@iiitd.ac.in	mtech14
mt14015	naman goyal	naman1415@iiitd.ac.in	mtech14
mt14016	niharika gupta	niharika1416@iiitd.ac.in	mtech14
mt14017	nishant adhikari	nishant1417@iiitd.ac.in	mtech14
mt14018	priyanka balotra	priyanka1418@iiitd.ac.in	mtech14
mt14019	rahul sharma	rahul1419@iiitd.ac.in	mtech14
mt14020	saba chaudhary	saba1420@iiitd.ac.in	mtech14
mt14021	sachin negi	sachin1421@iiitd.ac.in	mtech14
mt14022	sanchari deb	sanchari1422@iiitd.ac.in	mtech14
mt14023	shisagnee banerjee	shisagnee1423@iiitd.ac.in	mtech14
mt14024	shreya tripathi	shreya1424@iiitd.ac.in	mtech14
mt14025	sirinam sampath	sirinam1425@iiitd.ac.in	mtech14
mt14026	sonal goel	sonal1426@iiitd.ac.in	mtech14
mt14027	vasudev chatterjee	vasudev1427@iiitd.ac.in	mtech14
mt14028	veronica sharma	veronica1428@iiitd.ac.in	mtech14
mt14029	vikas singh	vikas1429@iiitd.ac.in	mtech14
mt14030	vincent b mathew	vincent1430@iiitd.ac.in	mtech14
mt14031	yash dhingra	yash1431@iiitd.ac.in	mtech14
mt14032	yogesh kumari	yogesh1432@iiitd.ac.in	mtech14
mt14033	ajit pratap singh	ajit1433@iiitd.ac.in	mtech14
mt14034	amitesh	amitesh1434@iiitd.ac.in	mtech14
mt14035	anindya srivastava	anindya1435@iiitd.ac.in	mtech14
mt14036	ankit verma	ankit1436@iiitd.ac.in	mtech14
mt14037	deependra raghuvanshi	deependra1437@iiitd.ac.in	mtech14
mt14038	jagan rv	jagan1438@iiitd.ac.in	mtech14
mt14039	kritika mittal	kritika1439@iiitd.ac.in	mtech14
mt14040	munawar hasan	munawar1440@iiitd.ac.in	mtech14
mt14041	pooja gupta	pooja1441@iiitd.ac.in	mtech14
mt14042	pradyumn nand	pradyumn1442@iiitd.ac.in	mtech14
mt14043	sakshi agarwal	sakshi1443@iiitd.ac.in	mtech14
mt14044	shubham srivastava	shubham1444@iiitd.ac.in	mtech14
mt14045	swati rathi	swati1445@iiitd.ac.in	mtech14
mt14046	aanchal singh	aanchal1446@iiitd.ac.in	mtech14
mt14047	abhishek mitra	abhishek1447@iiitd.ac.in	mtech14
mt14048	anshika agarwal	anshika1448@iiitd.ac.in	mtech14
mt12017	rohit jain	3	extended
mt13007	neha gupta	neha1307@iiitd.ac.in	mtech13
mt13009	nidhi agarwal	nidhi1309@iiitd.ac.in	mtech13
mt13010	payal sharma	payal 1310@iiitd.ac.in	mtech13
mt13011	pooja gupta	pooja1311@iiitd.ac.in	mtech13
mt13014	shariq murtuza	shariq1314@iiitd.ac.in	mtech13
mt13015	shobhita saxena	shobhita1315@iiitd.ac.in	mtech13
mt13016	surabhi gara	surabhi1316@iiitd.ac.in	mtech13
mt13017	suresh r	suresh1317@iiitd.ac.in	mtech13
mt13018	vaishali garg	vaishali1318@iiitd.ac.in	mtech13
mt13019	vibhor	vibhor1319@iiitd.ac.in	mtech13
mt	adarsh dubey	adarsh1331@iiitd.ac.in	mtech13
mt13032	akif khan	akif1332@iiitd.ac.in	mtech13
mt13034	ashta sachdeva	astha1334@iiitd.ac.in	mtech13
mt13036	jeevan joshi	jeevan1336@iiitd.ac.in	mtech13
mt13037	kireet pant	kireet1337@iiitd.ac.in	mtech13
mt13038	kompelly harshavardhan reddy	kompelly1338@iiitd.ac.in	mtech13
mt13039	kritika anabd	kritika1339@iiitd.ac.in	mtech13
mt13040	kunal gupta	kunal1340@iiitd.ac.in	mtech13
mt13041	m mithun bose	mithun1341@iiitd.ac.in	mtech13
mt13043	navin agarwal	navin1343@iiitd.ac.in	mtech13
mt13044	neha	neha1344@iiitd.ac.in	mtech13
mt13045	nisha gupta	nisha1345@iiitd.ac.in	mtech13
mt13049	rahul batra	rahul1349@iiitd.ac.in	mtech13
mt13050	vivek gupta	vivek1350@iiitd.ac.in	mtech13
mt13061	akanksha mishra	akanksha1361@iiitd.ac.in	mtech13
mt13062	anjli chikara	anjli1362@iiitd.ac.in	mtech13
mt13063	dibyendu talukder	dibyendu1363@iiitd.ac.in	mtech13
mt13064	kirti jain	kirti1364@iiitd.ac.in	mtech13
mt13067	nishesh manaktala	nishesh1367@iiitd.ac.in	mtech13
mt13068	nishtha phutela	nishtha1368@iiitd.ac.in	mtech13
mt13070	parul gupta	parul1370@iiitd.ac.in	mtech13
mt13071	pathan khan	pathan1371@iiitd.ac.in	mtech13
mt13072	pooja agarwal	pooja1372@iiitd.ac.in	mtech13
mt13073	prabhat mishra	prabhat1373@iiitd.ac.in	mtech13
mt13076	rohit mehra	rohit1376@iiitd.ac.in	mtech13
mt13077	sakshi gupta	sakshi1377@iiitd.ac.in	mtech13
mt13078	savita dabral	savita1378@iiitd.ac.in	mtech13
mt13079	shruti bansal	shruti1379@iiitd.ac.in	mtech13
mt13080	sukriti mehta	sukriti1380@iiitd.ac.in	mtech13
mt13092	anurag rana	anurag1392@iiitd.ac.in	mtech13
mt13093	chaitanya tanwar	chaitanya1393@iiitd.ac.in	mtech13
mt13095	divya kundra	divya1395@iiitd.ac.in	mtech13
mt13096	jitendra bisht	jitendra1396@iiitd.ac.in	mtech13
mt14049	himanshu varshney	himanshu1449@iiitd.ac.in	mtech14
mt14084	mansi peer	mansi1484@iiitd.ac.in	mtech14
mt14085	mehta janki vrajvihari	mehta1485@iiitd.ac.in	mtech14
mt14086	nidhi gupta	nidhi1486@iiitd.ac.in	mtech14
mt14087	prerna khurana	prerna1487@iiitd.ac.in	mtech14
mt14088	protim bhattacharjee	protim1488@iiitd.ac.in	mtech14
mt14089	saloni mittal	saloni1489@iiitd.ac.in	mtech14
mt14090	shreyata sharma	shreyata1490@iiitd.ac.in	mtech14
mt14091	snigdha tariyal	snigdha1491@iiitd.ac.in	mtech14
mt14092	suman pani	suman1492@iiitd.ac.in	mtech14
mt14093	sushant kumar	sushant1493@iiitd.ac.in	mtech14
mt14094	vibhutesh kumar singh	vibhutesh1494@iiitd.ac.in	mtech14
2010097	yash lamba	yash10097@iiitd.ac.in	dualdegree
2010078	shayan lahiri	shayan10078@iiitd.ac.in	dualdegree
phd1406	soumyadeep ghosh	soumyadeepg@iiitd.ac.in	phd
phd14112	rahul bajpai	rahulbajpai2007@gmail.com	phd
phd14107	parag aggarwal	paraga@iiitd.ac.in	phd
phd14108	syed shammem ahmad	syeds@iiitd.ac.in	phd
phd14002	deepika yadav	deepikay@iiitd.ac.in	phd
phd14004	rohit keshari	rohitk@iiitd.ac.in	phd
phd14106	akshay jain	akshayj@iiitd.ac.in	phd
phd14109	ambuj mehrish	ambujm@iiitd.ac.in	phd
phd14103	tanya shreedhar	tanyas@iiitd.ac.in	phd
phd14101	dhananjay kimothi	dhananjayk@iiitd.ac.in	phd
phd1330	wazir singh	wazirs@iiitd.ac.in	phd
mt12063	anil sharma	anils@iiitd.ac.in	phd
phd1307	haroon rashid	haroonr@iiitd.ac.in	phd
phd1306	venkatesh vinayakarao	venkateshv@iiitd.ac.in	phd
phd1321	rahul singhal	rahuls@iiitd.ac.in	phd
phd1326	anupriya	anupriyag@iiitd.ac.in	phd
phd1324	deepak sharma	deepaks@iiitd.ac.in	phd
phd1325	naveen gupta	naveeng@iiitd.ac.in	phd
phd1323	sneihil gopal	sneihilg@iiitd.ac.in	phd
phd1302	swati agarwal	swatia@iiitd.ac.in	phd
phd1313	megha agrawal	meghaa@iiitd.ac.in	phd
phd1312	monika singh	monikas@iiitd.ac.in	phd
phd1311	tarun kr bansal	tarunb@iiitd.ac.in	phd
phd1225	mohammad a maktoomi	ayatullahm@iiitd.ac.in	phd
phd1224	hemanta kumar mondal	hemantam@iiitd.ac.in	phd
mt14050	megha vij	megha1450@iiitd.ac.in	mtech14
mt14051	neha	neha1451@iiitd.ac.in	mtech14
mt14052	nikita jain	nikita1452@iiitd.ac.in	mtech14
mt14053	ruchika banerjee	ruchika1453@iiitd.ac.in	mtech14
mt14054	rupali jain	rupali1454@iiitd.ac.in	mtech14
mt14055	sangeeth k	sangeeth1455@iiitd.ac.in	mtech14
mt14056	anindya	anindya1456@iiitd.ac.in	mtech14
mt14057	antara ganguly	antara1457@iiitd.ac.in	mtech14
mt14058	disha arora	disha1458@iiitd.ac.in	mtech14
mt14059	k bharath	bharath1459@iiitd.ac.in	mtech14
mt14060	k k praveen kumar	praveen1460@iiitd.ac.in	mtech14
mt14062	nidhi batra	nidhi1462@iiitd.ac.in	mtech14
mt14063	pawan sehgal	pawan1463@iiitd.ac.in	mtech14
mt14064	preet kaur walia	preet1464@iiitd.ac.in	mtech14
mt14065	prince garg	prince1465@iiitd.ac.in	mtech14
mt14066	pulkit sharma	pulkit1466@iiitd.ac.in	mtech14
mt14067	raghav kishore	raghav1467@iiitd.ac.in	mtech14
mt14068	renduchinthala anusha	renduchinthala1468@iiitd.ac.in	mtech14
mt14069	ricktam kundu	ricktam1469@iiitd.ac.in	mtech14
mt14070	sakshi jain	sakshi1470@iiitd.ac.in	mtech14
mt14071	sakshi taneja	sakshi1471@iiitd.ac.in	mtech14
mt14072	sangeeta goyal	sangeeta1472@iiitd.ac.in	mtech14
mt14073	shashwat	shashwat1473@iiitd.ac.in	mtech14
mt14074	sneha bhatia	sneha1474@iiitd.ac.in	mtech14
mt14075	vijay sharma	vijay1475@iiitd.ac.in	mtech14
mt14076	amit mohindru	amit1476@iiitd.ac.in	mtech14
mt14077	ankita raj	ankita1477@iiitd.ac.in	mtech14
mt14078	arpita gang	arpita1478@iiitd.ac.in	mtech14
mt14079	ashwani	ashwani1479@iiitd.ac.in	mtech14
mt14080	himani joshi	himani1480@iiitd.ac.in	mtech14
mt14081	jasmine kaur gulati	jasmine1481@iiitd.ac.in	mtech14
mt14082	kavya gupta	kavya1482@iiitd.ac.in	mtech14
mt14083	manish kumar	manish1483@iiitd.ac.in	mtech14
phd1209	sweta mishra	swetam@iiitd.ac.in	phd
phd1322	naushad ansari	naushada@iiitd.ac.in	phd
phd1308	ankita likhyani	ankital@iiitd.ac.in	phd
phd1309	arpan jati	arpanj@iiitd.ac.in	phd
phd1212	amit kumar chauhan	amitc@iiitd.ac.in	phd
phd1207	rakhi hemani	rakhih@iiitd.ac.in	phd
phd1103	jyoti leeka	jyotil@iiitd.ac.in	phd
mt12020	siddharth dawar	siddharthd@iiitd.ac.in	phd
mt13042	manisha khattar	manishak@iiitd.ac.in	phd
phd14105	prakhar chatterjee	prakharc@iiitd.ac.in	phd
phd14110	gourav chaturvedi	gouravc@iiitd.ac.in	phd
phd14001	akshay agarwal	akshaya@iiitd.ac.in	phd
phd14003	himanshu buckchash	himanshubh@iiitd.ac.in	phd
phd14005	lokender tiwari	lokendert@iiitd.ac.in	phd
phd14111	ankita shukla	ankitas@iiitd.ac.in	phd
phd14104	santhos kumar a	santhoshk@iiitd.ac.in	phd
phd14102	shiju. s	shijus@iiitd.ac.in	phd
phd1317	abhishek kumar	abhishekk@iiitd.ac.in	phd
mt12007	muhammed noufal k	2	extended
mt12034	aniya agarwal	4	extended
mt12074	samit anwer	5	extended
\.


--
-- Data for Name: ta_macs; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY ta_macs (rollno, mac) FROM stdin;
\.


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

