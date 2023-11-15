-- CREATE TABLE cars ( make varchar(100), model varchar(100), year int, value decimal(10, 2) );
-- INSERT INTO cars VALUES ('Porsche', '911 GT3', 2020, 169700), ('Porsche', 'Cayman GT4', 2018, 118000), ('Porsche', 'Panamera', 2022, 113200), ('Porsche', 'Macan', 2019, 27400), ('Porsche', '718 Boxster', 2017, 48880), ('Ferrari', '488 GTB', 2015, 254750), ('Ferrari', 'F8 Tributo', 2019, 375000), ('Ferrari', 'SF90 Stradale', 2020, 627000), ('Ferrari', '812 Superfast', 2017, 335300), ('Ferrari', 'GTC4Lusso', 2016, 268000);


-- select * from  cars order by make, value desc;
 -- Macan
delimiter //
create procedure get_all_cars(
	in year_filter int,
    out cars_number int
)
begin 
	select * from  cars where year = year_filter order by make, value desc;
    select count(*) from cars where year = year_filter into cars_number;
end //
delimiter ;


drop procedure get_all_cars;

-- call get_all_cars(2017,);

select * from cars;

delimiter #
create procedure example1()
begin
	declare cnt int;
    
    select count(*)
    into cnt
    from cars
    where model like '%Macan%';
    
    select concat('There is ',cnt,' car(s)');
end #
delimiter ;

drop procedure example1;

call example1;

select date_format(now(),'%Y/%M/%d') - date_format(date('2002/12/9'),'%Y/%M/%d') as age;

select date('2002/12/9/10');


select timestamp(date('2002/12/9 10:0:0'));

select date_format(now(),'00/%M/%d');

delimiter #
drop procedure get_age #
create procedure get_age()
begin 
	if date_format(now(),'00/%M/%d') <= date_format(date('2002/12/9'),'00/%M/%d') then
		select date_format(now(),'%Y/%M/%d') - date_format(date('2002/12/9'),'%Y/%M/%d') as age;
	else
		select date_format(now(),'%Y/%M/%d') - date_format(date('2002/12/9'),'%Y/%M/%d') -1 as age;
	end if;
    -- return 1;
end #
delimiter ;


call get_age();


delimiter #
drop procedure if exists get_age #
create procedure get_age(
	number_p int,
    out number_in float
)
begin 
	declare a int default(10);
    declare b varchar(20) default 'hello world';
    
    set a = 900;
    
    set number_in = sqrt(number_p);
end #
delimiter ;
call get_age(4, @sqrt_n);
select @sqrt_n;


delimiter #
drop procedure if exists get_age #
create procedure get_age(
	price int,
    out discount float
)

reads sql data

begin 
	declare make varchar(50);
    
    declare done int default 0;
    
    declare cur1 cursor for
		select model
        from cars;
        
	declare continue handler for not found
		set done = 1;
        
	create temporary table if not exists temp_table
    ( make_r varchar(200));
	
    open cur1;
    
    while done = 0 do
		fetch cur1 into make;
        insert into temp_table (make_r) values (make);
	end while;
    
    close cur1;
    
    select * from temp_table;
    drop temporary table temp_table;
end #
delimiter ;
call get_age(200,@dis);
select * from temp_table;

delimiter #
drop function if exists discount #
create function discount (price int)
returns int
deterministic
begin
	return 90;
end #
delimiter ;
select discount (4);


-- =====triggers====== --- 

alter table cars 
add column vic date null;

alter table cars
modify column vic timestamp null;


select * from cars;

delimiter #
drop trigger if exists dt #
create trigger dt
before insert on cars
for each row
begin
	-- if new.vic = null then
		set new.vic = now();
   -- end if;
end #
delimiter ;

insert into cars (make , model , year , value) values ('Mercedes','s550s',2020,150000);

delimiter #
drop procedure if exists demo #
create procedure demo( out p_in int)
begin
	select p_in;
    select 900 into p_in;
    select p_in;
end #
delimiter ;
set @p = 700;
call demo(@p);

select 5&6;

set @mytime = now();

select @mytime;

select curdate();

select now() - @mytime;

select format(400000,2);

select if(not true,'true','false');

select ifnull('not NULL','hello word');

set @email = 'murimimlvictor@gmail.com';

select insert(@email,2,5,'***');

select isnull(null);

select lpad('hel','o',4);

select version();

select date_add(now(),interval 20 year);

select bin_to_uuid(uuid());

select current_timestamp();

select encrypt();

select sha('hello world');

set @pass = aes_decrypt(aes_encrypt('hello world',unhex(sha2('1234567812345678',512))),unhex(sha2('1234567812345678',512)));

set block_encryption_mode = 'aes-256-cbc';

set @key_ = sha2('my password',512);
set @init_v = random_bytes(16);

set @encr = aes_encrypt('text',@key_,@init_v);
set @decr = aes_decrypt(@encr,@key_,@init_v);

select cast(@decr as char);

select unhex(56);

select cast(@pass as char);

