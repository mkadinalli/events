/*Mysql only*/

--== who wrote is vic ==--

drop table if exists users;
create table users(
    id int auto_increment UNIQUE,
    name varchar(100) NOT NULL,
    username varchar(100) NOT NULL UNIQUE,
    password varchar(100) NOT NULL,
    email varchar(100) NOT NULL UNIQUE,
    avater varchar(100) NULL,
    bio varchar(1000) NULL,
    about varchar(50) NULL,

    constraint users_pk primary key(id)
);

/*=========================================*/

drop table if exists published;
create table published(
    id int auto_increment UNIQUE,
    title varchar(200) NOT NULL,
    description varchar(1000) NOT NULL,
    venue varchar(50) NOT NULL,
    publisher int NOT NULL,

    constraint published_pk primary key(id),
    constraint publisher_fk foreign key(publisher) references users(id)
);

/*==========================================*/

drop table if exists subscriptions;
create table subscriptions
(
    id int auto_increment UNIQUE,
    user_id int,
    published_id int,

    constraint subscriptions_pk primary key(id),
    constraint sub_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    constraint sub_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

drop table if exists stars;
create table stars
(
    id int auto_increment UNIQUE,
    user_id int,
    published_id int,

    constraint stars_pk primary key(id),
    constraint stars_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    constraint stars_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

drop table if exists payments;
create table payments
(
    id int auto_increment UNIQUE,
    user_id int,
    published_id int,

    constraint payments_pk primary key(id),
    constraint pay_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    constraint pay_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

drop table if exists followers;
create table followers
(
    id int auto_increment UNIQUE,
    user_id int,
    follower_id int,

    constraint followers_pk primary key(id),
    constraint follow_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    constraint follow_follower_fk foreign key(follower_id) references users(id)
    on delete cascade
);



/*

insert into users 
(
	name,username,email,password
)
values
(
	'my name','my username','myemail,'my passwrd'
);


insert into published
(
	title,description,venue,publisher
)
values
(
	'title','description',venue',
);


insert into subscriptions
(
	user_id,published_id
)
values
(
	''0','0'
);


insert into stars
(
	user_id,published_id
)
values
(
	''0','0'
);


insert into payments
(
	user_id,published_id
)
values
(
	''0','0'
);


insert into followers
(
	user_id,published_id
)
values
(
	''0','0'
);

==========================================================
insert into users 
(
	name,username,email,password
)
values
('vic','myuser1','myemail1@gmail.com','my passwrd'),
('vic','myuser2','myemail2@gmail.com','my passwrd'),
('vic','myuser3','myemail3@gmail.com','my passwrd'),
('vic','myuser4','myemail4@gmail.com','my passwrd'),
('vic','myuser5','myemail5@gmail.com','my passwrd'),
('vic','myuser6','myemail6@gmail.com','my passwrd'),
('vic','myuser7','myemail7@gmail.com','my passwrd'),
('vic','myuser8','myemail8@gmail.com','my passwrd'),
('vic','myuser9','myemail9@gmail.com','my passwrd'),
('vic','myuser10','myemail10@gmail.com','my passwrd')
;

===========================================================

insert into published
(
	title,description,venue,publisher
)
values
('title1','description1','venue1',31),
('title2','description2','venue2',31),
('title3','description3','venue3',33),
('title4','description4','venue4',34),
('title5','description5','venue5',35),
('title6','description6','venue6',36),
('title7','description7','venue7',37),
('title8','description8','venue8',38),
('title9','description9','venue9',39),
('title10','description10','venue10',40)
;

===========================================================


insert into subscriptions
(
	user_id,published_id
)
values
(31,1),
(32,2),
(33,3),
(34,4),
(35,5),
(36,6),
(37,7),
(38,8),
(39,9),
(40,10)
;

================================================================

insert into stars
(
	user_id,published_id
)
values
(31,1),
(32,2),
(33,3),
(34,4),
(35,5),
(36,6),
(37,7),
(38,8),
(39,9),
(40,10)
;

==================================================================

insert into followers
(
	user_id,follower_id
)
values
(31,40),
(32,31),
(33,32),
(34,33),
(35,34),
(36,35),
(37,36),
(38,37),
(39,38),
(40,39)
;

===================================================================
*/

create procedure select_users
delimeter//
begin//
	select * from users//
end//
delimeter;



