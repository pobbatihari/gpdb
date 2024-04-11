-----------------------------------------------
-- Testcases regarding non scalar subqueries --
-----------------------------------------------

-- Setup
-- start_ignore
drop table if exists t1_subq;
drop table if exists t2_subq;
drop table if exists t3_subq;
drop table if exists comp_table;
create table t1_subq (a int, b int, c int) distributed by (a);
create table t2_subq (a int, b int, c int) distributed by (a);
create table t3_subq (a int, b int, c int) distributed by (a);

create type comp_type as ( a int, b int);
create table comp_table(id int, item comp_type) distributed by (id);
-- end_ignore

insert into t1_subq values (1, 1, null), (2, 1, 1), (1, null, 1), (2, null, 1), (1,1,1), (1,1,0), (0,0,0);
insert into t2_subq values (1, null, 1), ('123', 456, null), (1, 1, null), (1, 1, 1), (1, 1, 0), (0,0,0);
insert into t3_subq values (1, 1, null), ('123', 456, null), (1, 1, null);
insert into comp_table values (1, ROW(1, 10)), (2, ROW(2, 20));

analyze t1_subq, t2_subq, t3_subq;
set optimizer_trace_fallback to on;
-------------------------------------------
------- Non-Scalar IN subqueries ----------
-------------------------------------------

-- Two columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.b, t1_subq.c from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.b, t1_subq.c from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a, t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a, t1_subq.b from t2_subq);
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.b, t1_subq.c from t2_subq) AND t1_subq.b=1;
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.b, t1_subq.c from t2_subq) AND t1_subq.b=1;
explain (costs off)  select * from t1_subq where (a, b) IN (select (item).a, (item).b from comp_table);
select * from t1_subq where (a, b) IN (select (item).a, (item).b from comp_table);
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off)  select ((a, b) IN (select (item).a, (item).b from comp_table)) from t1_subq;
select ((a, b) IN (select (item).a, (item).b from comp_table)) from t1_subq;

-- Three columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
explain (costs off) select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq) AND t1_subq.b=1;
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq) AND t1_subq.b=1;
select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b, t1_subq.c) IN (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;

-- Adding const to projection column
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t1_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t1_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.c+1, t2_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.c+1, t2_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+1, t2_subq.b+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+1, t2_subq.b+1 from t2_subq);
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) IN (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;

-- Adding two columns
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ANY(select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ANY(select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ANY(select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ANY(select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ANY(select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ANY(select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;

-- Nested subquery
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
explain (costs off) select (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
explain (costs off) select (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) IN (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;

-- Set returning function in projection
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select generate_series(1, 10), generate_series(1, 10));
select * from t1_subq where (t1_subq.a, t1_subq.b) IN (select generate_series(1, 10), generate_series(1, 10));
explain (costs off) select  ((t1_subq.a, t1_subq.b) IN (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;
select  ((t1_subq.a, t1_subq.b) IN (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;

-------------------------------------------
------- Non-Scalar ALL subqueries ----------
-------------------------------------------

-- Two columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a, t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t2_subq.b from t2_subq);
explain (costs off)  select * from t1_subq where (a, b) = ALL (select (item).a, (item).b from comp_table);
select * from t1_subq where (a, b) = ALL (select (item).a, (item).b from comp_table);
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((a, b) = ALL (select (item).a, (item).b from comp_table)) from t1_subq;
select ((a, b) = ALL (select (item).a, (item).b from comp_table)) from t1_subq;

-- Three columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) = ALL (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;

-- Adding const to projection column
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t1_subq.c+1, t1_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t1_subq.c+1, t1_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t2_subq.c+1, t2_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t2_subq.c+1, t2_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t1_subq.a+1, t2_subq.b+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL(select t1_subq.a+1, t2_subq.b+1 from t2_subq);
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;

-- Adding two columns
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ALL (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ALL (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;

-- Nested subquery
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
explain (costs off) select (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
explain (costs off) select (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) = ALL (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;

-- Set returning function in projection
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select generate_series(1, 10), generate_series(1, 10));
select * from t1_subq where (t1_subq.a, t1_subq.b) = ALL (select generate_series(1, 10), generate_series(1, 10));
explain (costs off) select  ((t1_subq.a, t1_subq.b) = ALL (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;
select  ((t1_subq.a, t1_subq.b) = ALL (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;

-------------------------------------------
------- Non-Scalar != ANY subqueries ----------
-------------------------------------------

-- Two columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a, t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t2_subq.b from t2_subq);
explain (costs off)  select * from t1_subq where (a, b) != any(select (item).a, (item).b from comp_table);
select * from t1_subq where (a, b) != any(select (item).a, (item).b from comp_table);
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a, t2_subq.b from t2_subq)) from t1_subq;
explain (costs off)  select ((a, b) != ANY (select (item).a, (item).b from comp_table)) from t1_subq;
select ((a, b) != ANY (select (item).a, (item).b from comp_table)) from t1_subq;

-- Three columns composite key
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq);
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t2_subq.a, t2_subq.b, t2_subq.c from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b, t1_subq.c) != ANY (select t1_subq.b, t1_subq.c, t2_subq.b from t2_subq)) from t1_subq;

-- Adding const to projection column
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t1_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t1_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.c+1, t2_subq.c+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.c+1, t2_subq.c+1 from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+1, t2_subq.b+1 from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+1, t2_subq.b+1 from t2_subq);
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t1_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
explain (costs off) select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;
select ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.c+1, t2_subq.c+1 from t2_subq)) from t1_subq;

-- Adding two columns
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq);
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq);
explain (costs off) select  ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t1_subq.a, t1_subq.b+t1_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) != ANY (select t2_subq.a+t2_subq.a, t2_subq.b+t2_subq.b from t2_subq)) from t1_subq;
explain (costs off) select  ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;
select  ((t1_subq.a, t1_subq.b) != ANY (select t1_subq.a+t2_subq.a, t1_subq.b+t2_subq.b from t2_subq)) from t1_subq;

-- Nested subquery
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t1_subq.a,t1_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq));
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq));
explain (costs off) select (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t3_subq.b from t3_subq)) from t1_subq;
explain (costs off) select (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;
select (t1_subq.a, t1_subq.b) != ANY (select t2_subq.a,t2_subq.b from t2_subq where (t2_subq.a, t2_subq.b) IN (select t3_subq.a, t1_subq.b from t3_subq)) from t1_subq;

-- Set returning function in projection
explain (costs off) select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select generate_series(1, 10), generate_series(1, 10));
select * from t1_subq where (t1_subq.a, t1_subq.b) != ANY (select generate_series(1, 10), generate_series(1, 10));
explain (costs off) select  ((t1_subq.a, t1_subq.b) != ANY (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;
select  ((t1_subq.a, t1_subq.b) != ANY (select generate_series(1, 10), generate_series(1, 10))) from t1_subq;

reset optimizer_trace_fallback;
drop table t1_subq, t2_subq, t3_subq, comp_table;
drop type comp_type;