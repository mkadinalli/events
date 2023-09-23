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


