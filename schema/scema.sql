																			/*
Mysql only
	--> add cost
    --> add capacity
    */

-- == who wrote is vic ==--

drop schema if exists events;
create schema events;

use events;
set block_encryption_mode = 'aes-256-cbc';
select @@max_allowed_packet;
select @@net_buffer_length;
set global max_allowed_packet = @@max_allowed_packet * 10;
commit;
set global net_buffer_length = @@net_buffer_length *10;

-- =======USERS=================================================================

drop table if exists users;
create table users(
    id binary(16) UNIQUE,
    name varchar(100) NOT NULL,
    username varchar(100) NOT NULL UNIQUE,
    pass_word blob NOT NULL,
    salt binary(16),
    email varchar(100) NOT NULL UNIQUE,
    avater varchar(100) NULL,
    bio varchar(1000) NULL,
    about varchar(50) NULL,
    verify_token binary(16),
    verified boolean default false,
    token_valid_until timestamp not null,
    date_created timestamp default now(),
    date_modified timestamp default now() on update now(),
    

    constraint
		users_pk primary key(id)
);

/*=========PUBLISHED EVENTS================================*/

drop table if exists published;
create table published(
    id binary(16) UNIQUE,
    title varchar(200) NOT NULL,
    description varchar(1000) NOT NULL,
    venue varchar(50) NOT NULL,
    price long NOT null,
    event_date timestamp not null,
    deadline_date timestamp not null,
    publisher_id binary(16)  NOT NULL,
    stars int unsigned default 0,
	date_created timestamp default now(),
    date_modified timestamp default now() on update now(),

    constraint 
		published_pk primary key(id),
    constraint 
		publisher_fk foreign key(publisher_id) references users(id)
);


/*===========STARS===============================*/



drop table if exists stars;
create table stars
(
    id binary(16)  UNIQUE,
    user_id binary(16) ,
    published_id binary(16) ,
	date_created timestamp default now(),

    constraint 
		stars_pk primary key(id),
        
    constraint 
		stars_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    
    constraint 
		stars_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

-- =========PAYMENTS========================================================================

drop table if exists payments;
create table payments
(
    id  binary(16)  UNIQUE,
    user_id binary(16) ,
    published_id binary(16) ,
	date_created timestamp default now(),
    date_modified timestamp default now() on update now(),
    merchant_request_id varchar(256),
    checkout_request_id varchar(256),
    transaction_id varchar(256) null,
    amount int,
    

    constraint 
		payments_pk primary key(id),
        
    constraint 
		pay_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    
    constraint
		pay_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

-- ==============FOLLOWERS=============================================================

drop table if exists followers;
create table followers
(
    id binary(16) unique,
    user_id binary(16) NOT NULL,
    follower_id binary(16) NOT NULL,
    date_created timestamp default now(),

    constraint 
		followers_pk primary key(id),
        
    constraint 
		follow_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    
    constraint 
		follow_follower_fk foreign key(follower_id) references users(id)
    on delete cascade
);

-- =========SUBSCRIPTIONS==================================================================

drop table if exists subscriptions;
create table subscriptions
(
    id binary(16) UNIQUE,
    user_id binary(16)  not null,
    published_id binary(16) not null,
    paid bool default false,
    date_created timestamp default now(),
    date_modified timestamp default now() on update now(),

    constraint 
		subscriptions_pk primary key(id),
        
    constraint 
		sub_user_fk foreign key(user_id) references users(id)
    on delete cascade,
    
    constraint 
		sub_events_fk foreign key(published_id) references published(id)
    on delete cascade
);

-- ========================================================================

drop table if exists user_images;
create table user_images
(
	id binary(16) not null,
    user_id binary(16),
    file_path varchar(256),
    purpose enum('avater') default 'avater',
	date_created timestamp default now(),
    date_modified timestamp default now() on update now(),
    
    
	constraint 
		user_img_pk primary key(id),
	constraint 
		img_user_fk foreign key(user_id) references users(id)
    on delete cascade
);

-- ===========================================================================

drop table if exists pub_images;
create table pub_images
(
	id binary(16) not null,
    pub_id binary(16) not null,
    file_path varchar(256) not null,
    purpose enum('banner') default 'banner',
	date_created timestamp default now(),
    date_modified timestamp default now() on update now(),
    
    
	constraint 
		pub_img_pk primary key(id),
	constraint 
		img_pub_fk foreign key(pub_id) references published(id)
    on delete cascade
);

-- ===========================================================================
-- ===============procedures==================================================
-- ===========================================================================


-- ===========password enrypt ===========
delimiter #
drop function if exists encrypt_password #
create function encrypt_password ( in_password varchar(50), salt binary(16) ) returns blob
deterministic
begin
	set block_encryption_mode = 'aes-256-cbc';
    return aes_encrypt(in_password,sha2('key',512),salt);
end #
delimiter ;

-- select cast(encrypt_password('hello') as char);

-- select random_bytes(16);

-- 0773753482

-- select (aes_encrypt('yoh','key',random_bytes(16)))

-- =========password decrypt =========== ---

delimiter #
drop function if exists decrypt_password #
create function decrypt_password ( in_password blob, salt binary(16) ) returns varchar(50)
deterministic
begin
	set block_encryption_mode = 'aes-256-cbc';
	return aes_decrypt(in_password,sha2('key',512),salt);
end #
delimiter ;

-- ========== validate user ============ ---

delimiter #
drop procedure if exists validate_user #
create procedure validate_user (
	in_password varchar(50),
    in_username varchar(50),
    in_email varchar(50)
)

begin
	declare pass blob;
    declare v_salt binary(16);
    declare nf int default 0;
    
   declare continue handler for not found
		set nf = 1;
    
    if isnull(in_email) then
			select
				pass_word,salt
			into pass,v_salt
           -- email
			from users
			where username = in_username;
	else
			select
				pass_word,salt
			into pass,v_salt
            from users
            where email = in_email;
	end if;
	
		
    if nf = 0 then
		if decrypt_password(pass,v_salt) = in_password then
			select 1 as valid;
		end if;
	end if;
end #
delimiter ;


-- =============== add user id ========== ---



delimiter #
drop trigger if exists add_users_id #
create trigger add_users_id
before insert on users
for each row
begin
    set new.id = uuid_to_bin(uuid());
    set new.verify_token = uuid_to_bin(uuid());
    set new.token_valid_until = date_add(now(),interval 1 hour);
end #
delimiter ;


-- ========= add published id ===== ---

delimiter #
drop trigger if exists add_published_id #
create trigger add_published_id
before insert on published
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ;


delimiter #
drop trigger if exists add_star_id #
create trigger add_star_id
before insert on stars
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ; 


delimiter #
drop trigger if exists add_follower_id #
create trigger add_follower_id
before insert on followers
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ; 


drop trigger if exists add_u_img_id;
/*
delimiter #
drop trigger if exists add_u_img_id #
create trigger add_u_img_id
before insert on user_images
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ; */

drop trigger if exists add_p_img_id;
/*
delimiter #
drop trigger if exists add_p_img_id #
create trigger add_p_img_id
before insert on pub_images
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ; */


delimiter #
drop trigger if exists add_payment_id #
create trigger add_payment_id
before insert on payments
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ;


delimiter #
drop trigger if exists add_sub_id #
create trigger add_sub_id
before insert on subscriptions
for each row
begin
    set new.id = uuid_to_bin(uuid());
end #
delimiter ; 

-- select cast(bin_to_uuid(id) as char) into @muuid from users where username = 'my';

-- ==========get one user by id=========== --

delimiter #
drop procedure if exists get_user #
create procedure get_user(
	in_id varchar(256)
)
begin
	declare this_id binary(16) default uuid_to_bin(in_id);
	select
    cast( bin_to_uuid(id) as char) as id,
    name,
    username,
    email,
    (select file_path from user_images where user_id = this_id and purpose = 'avater' limit 1) as avater,
    about,
    bio,
    date_created as join_date,
    (select count(*) from published where publisher_id = this_id) as published,
	(select count(*) from followers where  follower_id = this_id) as followers,
    (select count(*) from followers where  user_id = this_id) as followed,
    (select count(*) from stars where  user_id = this_id) as starred,
    (select count(*) from subscriptions where user_id = this_id) as subscribed,
    ifnull((select sum(stars) from published where  publisher_id = this_id),0) as stars_earned
    from users
    where id = this_id;
end #
delimiter ;



delimiter #
drop trigger if exists add_stars #
create trigger add_stars
before insert on stars
for each row
begin
	update published p
    set stars = p.stars + 1
    where p.id = new.published_id;
end #
delimiter ;


delimiter #
drop trigger if exists sub_stars #
create trigger sub_stars
before delete on stars
for each row
begin
	update published p
    set stars = p.stars - 1
    where p.id = old.published_id;
end #
delimiter ;


delimiter #
drop procedure if exists get_one_published #
create procedure get_one_published(
	in_id varchar(256)
)
begin
	declare this_id binary(16) default uuid_to_bin(in_id);
    select 
		cast(bin_to_uuid(id) as char) as id,
        title,
        description,
        venue,
        (select file_path from pub_images where pub_id = this_id and purpose = 'banner' limit 1) as avater,
        cast(bin_to_uuid(publisher_id) as char) as publisher_id,
        date_created,
        event_date,
        deadline_date,
        now() as time_queried,
        (select count(*) from subscriptions where published_id = this_id) as subscriptions,
        stars
	from published where id = this_id ;
end #
delimiter ;

delimiter #
drop procedure if exists get_many_published #
create procedure get_many_published(
    in_user_id varchar(256),
    last_query_time timestamp,
    last_query_last_time timestamp
)
begin
	declare this_user_id binary(16) default uuid_to_bin(in_user_id);
    select 
		cast(bin_to_uuid(id) as char) as id,
        title,
        -- description,
        -- venue,
        -- cast(bin_to_uuid(publisher_id) as char) as publisher_id,
        -- date_created,
        event_date,
        -- deadline_date,
		now() as time_queried,
        (select count(*) from subscriptions s where s.published_id = p.id) as subscriptions,
        stars
	from published p
    where
    deadline_date > now()
    -- and publisher_id in (select follower_id from followers where user_id = in_user_id)
    or p.publisher_id = in_user_id
    or p.date_created >= last_query_time
    or p.date_created <= last_query_last_time
    or p.id in 
    (select id from stars where user_id in (select follower_id from followers where user_id = in_user_id))
    limit 20;
end #
delimiter ;


delimiter #
drop procedure if exists get_followers #
create procedure get_followers(
	in_user_id varchar(256),
    last_time timestamp
)
begin
	declare in_user_id_bin binary(16) default uuid_to_bin(in_user_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from users s where f.user_id = s.id)) as char) as user_id,
        (select name from users s where f.user_id = s.id) as name,
        (select username from users s where f.user_id = s.id) as username,
        date_created
    from
		followers f
    where
		f.date_created < last_time
	and f.follower_id = in_user_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;


delimiter #
drop procedure if exists get_following #
create procedure get_following(
	in_user_id varchar(256),
    last_time timestamp
)
begin
	declare in_user_id_bin binary(16) default uuid_to_bin(in_user_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from users s where f.user_id = s.id)) as char) as user_id,
        (select name from users s where f.user_id = s.id) as name,
        (select username from users s where f.user_id = s.id) as username,
        date_created
    from
		followers f
    where
		f.date_created < last_time
	and f.user_id = in_user_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;


delimiter #
drop procedure if exists get_pub_for_user #
create procedure get_pub_for_user(
	in_user_id varchar(256),
    last_time timestamp
)
begin
	declare in_user_id_bin binary(16) default uuid_to_bin(in_user_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        title,
        event_date,
        (select count(*) from subscriptions s where s.id = p.id) as subscriptions,
        stars,
        date_created
    from
		published p
    where
		p.date_created < last_time
	and p.deadline_date > now()
	and p.publisher_id = in_user_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;


delimiter #
drop procedure if exists get_stars_for_pub #
create procedure get_stars_for_pub(
	in_publish_id varchar(256),
    last_time timestamp
)
begin
	declare in_publish_id_bin binary(16) default uuid_to_bin(in_publish_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from users s where f.user_id = s.id)) as char) as user_id,
        (select name from users s where f.user_id = s.id) as name,
        (select username from users s where f.user_id = s.id) as username,
        date_created
    from
		stars f
    where
		f.date_created < last_time
	and f.published_id = in_publish_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;





delimiter #
drop procedure if exists get_stars_for_user #
create procedure get_stars_for_user(
	in_user_id varchar(256),
    last_time timestamp
)
begin
	declare in_user_id_bin binary(16) default uuid_to_bin(in_user_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from published s where f.published_id = s.id)) as char) as event_id,
        (select title from published s where f.published_id = s.id) as title,
        date_created
    from
		stars f
    where
		f.date_created < last_time
	and f.user_id = in_user_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;



delimiter #
drop procedure if exists get_subs_for_user #
create procedure get_subs_for_user(
	in_user_id varchar(256),
    last_time timestamp
)
begin
	declare in_user_id_bin binary(16) default uuid_to_bin(in_user_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from published s where f.published_id = s.id)) as char) as event_id,
        (select title from published s where f.published_id = s.id) as title,
        paid,
        date_created
    from
		subscriptions f
    where
		f.date_created < last_time
	and f.user_id = in_user_id_bin
    -- and f.paid = true
    order by date_created desc
    limit 20;
end #
delimiter ;



delimiter #
drop procedure if exists get_subscribers_for_pub #
create procedure get_subscribers_for_pub(
	in_publish_id varchar(256),
    last_time timestamp
)
begin
	declare in_publish_id_bin binary(16) default uuid_to_bin(in_publish_id);
    
	select
		cast(bin_to_uuid(id) as char) as id,
        cast(bin_to_uuid((select id from users s where f.user_id = s.id)) as char) as user_id,
        (select name from users s where f.user_id = s.id) as name,
        (select username from users s where f.user_id = s.id) as username,
        date_created
    from
		subscriptions f
    where
		f.date_created < last_time
	and f.paid = true
	and f.published_id = in_publish_id_bin
    order by date_created desc
    limit 20;
end #
delimiter ;


delimiter #
drop procedure if exists search_publish #
create procedure search_publish(
	str varchar(50)
)
begin
	select
		cast(bin_to_uuid(id) as char) as id,
        title
	from 
		published
	where
		title like '%str%'
	order by stars desc;
    
end #
delimiter ;


delimiter #
drop procedure if exists insert_user_image #
create procedure insert_user_image(
	user_id varchar(256),
    file_n varchar(256)
)
begin
	declare im_id varchar(256) default uuid();
	insert into user_images (id,user_id,file_path)
    values (uuid_to_bin(im_id),uuid_to_bin(user_id),file_n);
    
    select im_id as id;
end #
delimiter ;


delimiter #
drop procedure if exists insert_pub_image #
create procedure insert_pub_image(
	pub_id varchar(256),
    file_n varchar(256)
)
begin
	declare im_id varchar(256) default uuid();
	insert into pub_images (id,user_id,file_path)
    values (uuid_to_bin(im_id),uuid_to_bin(pub_id),file_n);
    
    select im_id as id;
end #
delimiter ;


delimiter #
drop procedure if exists insert_user #
create procedure insert_user(
	in_name varchar(256),
    in_username varchar(256),
    in_email varchar(256)
)
begin
	insert into users 
	(
	name,username,email
	)
	values
	(
	in_name,in_username,in_email
	);
    
    select bin_to_uuid(id) as id,
    in_email as email,
    bin_to_uuid(verify_token) as token
    from users where email = in_email;
end #
delimiter ;


-- Make password null
alter table users 
modify pass_word blob null;




delimiter #
drop procedure if exists verify_user_email #
create procedure verify_user_email(
	in_user_id varchar(256),
    in_tok varchar(256),
    in_password varchar(256)
)
begin
    declare exp_time timestamp default null;
    declare success boolean default false;
    declare p_salt binary(16) default random_bytes(16);
    
    declare user_id binary(16) default uuid_to_bin(in_user_id);
    declare token binary(16) default uuid_to_bin(in_tok);
		
	select token_valid_until
    into exp_time 
    from users 
    where id = user_id
    and verify_token = token
    and verified = false;
    
    if exp_time is not null then
        if exp_time < now() then
			delete from users where id = user_id;
		else
			    update users 
				set pass_word = encrypt_password(in_password,p_salt),
					salt = p_salt,
					verified = true
				where
					verify_token = token
				and id = user_id
				and verified = false;
			set success = true;
		end if;
	end if;
    
    select success;
    
end #
delimiter ;


delimiter #
drop procedure if exists check_username_validity #
create procedure check_username_validity(
	in_username varchar(256)
)
begin
    select count(*) as number from users where username = in_username;
end #
delimiter ;


delimiter #
drop procedure if exists add_user_password #
create procedure add_user_password(
	in_password varchar(256),
    in_vtoken varchar(256),
    in_id varchar(256)
)
begin
	declare p_salt binary(16) default random_bytes(16);
    declare tok binary(16) default uuid_to_bin(in_vtoken);
    declare id_ binary(16) default uuid_to_bin(in_id);
    
    update users 
    set pass_word = encrypt_password(in_password,p_salt),
		salt = p_salt,
        verified = true
	where
		verify_token = tok 
	and token_valid_until > now()
    and id = id_
    and verified = false;
end #
delimiter ;






delete from users where email = 'murimimlvictor@gmail.com';

select bin_to_uuid(id) as id, bin_to_uuid(verify_token) as tok from users;

call add_user_password('hello','62282cf3-7cbf-11ee-864c-dc215ca11a9e','62282c42-7cbf-11ee-864c-dc215ca11a9e');

select *from users;
and verify_token = uuid_to_bin('92750f21-7cbb-11ee-864c-dc215ca11a9e');

call insert_user('vic','user2355455','email2786455');
/*
select * from published;

select * from payments;

select cast(bin_to_uuid(id) as char) as id from users;

insert into published
(
	title,description,venue,publisher_id,event_date,deadline_date
)
values
('title12','description1','venue1',uuid_to_bin('24df0b8b-71ad-11ee-b82b-dc215ca11a9e'),'2024-1-1','2024-1-1');



call verify_user_email('24df0b8b-71ad-11ee-b82b-dc215ca11a9e','24e0909f-71ad-11ee-b82b-dc215ca11a9e');


call insert_user('vic','user2','email2','1234');

delete from users where email != 'hello';

select * from users;

select * from payments;

-- so you read code ???????????????????????? :D

call get_many_published(@muuid,now(),now());

call get_user('2272f2a3-5a0e-11ee-b6f2-1f05bb9bd55d');

call get_following('2272f2a3-5a0e-11ee-b6f2-1f05bb9bd55d','1990-2-2 00:00:00');

call get_followers('7df5c89c-5aef-11ee-b6f2-1f05bb9bd55d',now());

call get_pub_for_user('58a7e1d2-5df5-11ee-8af4-ce9687e2d584','2030-1-1');

rollback;

call get_stars(@muuid2,now());

call get_subscribers(@muuid2,now());

select * from stars;

select * from users;



select * from published;

-- /home/vic/Desktop/ev2/events/files/image1695926754.jpg

select * from subscriptions;

select * from user_images;

select cast(bin_to_uuid(id) as char) as id from users;

insert into users 
(
	name,username,email,pass_word
)
values
(
	'my27','my27','myemail272','my passwrd'
);

insert into published
(
	title,description,venue,publisher_id,event_date,deadline_date
)
values
('title12','description1','venue1',uuid_to_bin('24df0b8b-71ad-11ee-b82b-dc215ca11a9e'),'2024-1-1','2024-1-1');

select cast(bin_to_uuid(id) as char) as id from published;

select cast(bin_to_uuid(id) as char) as id from subscriptions;

select cast(bin_to_uuid(id) as char) as id from followers;

delete from published where id = uuid_to_bin('7e3512ea-72fc-11ee-b82b-dc215ca11a9e');

call get_one_published('3df7354d-5a1b-11ee-b6f2-1f05bb9bd55d');

call get_subs_for_user('7df5c89c-5aef-11ee-b6f2-1f05bb9bd55d','2030-2-2 00:00:00');

insert into subscriptions
(
	user_id,published_id
)
values
(uuid_to_bin('24df0b8b-71ad-11ee-b82b-dc215ca11a9e'),uuid_to_bin('7e3512ea-72fc-11ee-b82b-dc215ca11a9e'));

insert into subscriptions
(
	user_id,published_id
)
values
(
	uuid_to_bin('7df5c89c-5aef-11ee-b6f2-1f05bb9bd55d'),uuid_to_bin('4cb1a2e9-5b77-11ee-b6f2-1f05bb9bd55d')
);

insert into followers
(
	user_id,follower_id
)
values
(uuid_to_bin('24df0b8b-71ad-11ee-b82b-dc215ca11a9e'),uuid_to_bin('24df0b8b-71ad-11ee-b82b-dc215ca11a9e'));



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

/*
            switch (result_bind_get_at(i, params)->type_name)
            {

            case MYSQL_TYPE_SHORT:
                int b_int = result_bind_get_int(i, params);
                p_bind[i].buffer = &b_int;
                break;

            case MYSQL_TYPE_LONG:
                long b_long = result_bind_get_i32(i, params);
                p_bind[i].buffer = &b_long;
                break;

            case MYSQL_TYPE_LONGLONG:
                long long b_long_long = result_bind_get_i64(i, params);
                p_bind[i].buffer = b_long_long;
                break;

            case MYSQL_TYPE_BOOL:
                bool b_bool = result_bind_get_bool(i, params);
                p_bind[i].buffer = &b_bool;
                break;

            case MYSQL_TYPE_FLOAT:
                double b_float = result_bind_get_float(i, params);
                p_bind[i].buffer = &b_float;
                break;

            case MYSQL_TYPE_DOUBLE:
                double b_double = result_bind_get_double(i, params);
                p_bind[i].buffer = &b_double;
                break;

            case MYSQL_TYPE_DECIMAL:
                double b_decimal = result_bind_get_double(i, params);
                p_bind[i].buffer = &b_decimal;
                break;

            default:
                char *b_str = result_bind_get_string(i, params);
                p_bind[i].buffer = b_str;
                p_bind[i].buffer_length = strlen(b_str);
                break;
            }

*/




