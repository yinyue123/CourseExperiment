CREATE TABLE account
(
	username varchar(20) NOT NULL ,
	password varchar(20) NOT NULL 
);

CREATE TABLE buy
(
	ONum int NOT NULL ,
	BCount int NULL ,
	BName varchar(20) NOT NULL 
);

CREATE TABLE goods
(
	GName varchar(20) NOT NULL ,
	GPrice float NULL ,
	GCount int NULL 
);

CREATE TABLE orders
(
	ONum int NOT NULL ,
	OTime datetime NULL ,
	username varchar(20) NOT NULL ,
	consignee varchar(20) NULL ,
	location varchar(50) NULL ,
	zipcode varchar(20) NULL ,
	tel varchar(20) NULL ,
	cfm int NOT NULL 
);