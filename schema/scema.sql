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

    constraint published_pk primary key(id)
);

/*==========================================*/

drop table if exists subscriptions;
create table subscriptions
(
    id int auto_increment UNIQUE,
    user_id int,
    published_id int,

    constraint subscriptions_pk primary key(id),
    constraint sub_user_fk foreign key(user_id) references users(id),
    constraint sub_events_fk foreign key(published_id) references published(id)
);