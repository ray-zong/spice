/*
Navicat SQLite Data Transfer

Source Server         : spice
Source Server Version : 30808
Source Host           : :0

Target Server Type    : SQLite
Target Server Version : 30808
File Encoding         : 65001

Date: 2016-09-09 14:25:31
*/

PRAGMA foreign_keys = OFF;

-- ----------------------------
-- Table structure for content
-- ----------------------------
DROP TABLE IF EXISTS "main"."content";
CREATE TABLE "content" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"chineseName"  TEXT NOT NULL,
"englishName"  TEXT NOT NULL,
CONSTRAINT "chineseName" UNIQUE ("chineseName", "englishName")
);

-- ----------------------------
-- Records of content
-- ----------------------------

-- ----------------------------
-- Table structure for spice_content
-- ----------------------------
DROP TABLE IF EXISTS "main"."spice_content";
CREATE TABLE "spice_content" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"retentionTime"  REAL NOT NULL,
"absoluteValue"  REAL NOT NULL,
"relativeValue"  REAL NOT NULL,
"spiceId"  INTEGER NOT NULL,
"contentId"  INTEGER NOT NULL,
CONSTRAINT "spiceId" FOREIGN KEY ("spiceId") REFERENCES "spice_information" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "contentId" FOREIGN KEY ("contentId") REFERENCES "content" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
CONSTRAINT "value" UNIQUE ("spiceId" ASC, "contentId" ASC)
);

-- ----------------------------
-- Records of spice_content
-- ----------------------------

-- ----------------------------
-- Table structure for spice_information
-- ----------------------------
DROP TABLE IF EXISTS "main"."spice_information";
CREATE TABLE "spice_information" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"englishName"  TEXT,
"chineseName"  TEXT NOT NULL,
"typeId"  INTEGER NOT NULL,
"managementId"  INTEGER NOT NULL,
"property"  TEXT,
"sense"  TEXT,
"physicsId"  INTEGER NOT NULL,
"extract"  TEXT,
"origin"  TEXT,
"purpose"  TEXT,
"imagePath"  TEXT,
CONSTRAINT "managementId" FOREIGN KEY ("managementId") REFERENCES "spice_management" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "physicsId" FOREIGN KEY ("physicsId") REFERENCES "spice_physics" ("id") ON DELETE CASCADE ON UPDATE CASCADE,
CONSTRAINT "typeId" FOREIGN KEY ("typeId") REFERENCES "spice_type" ("id") ON DELETE RESTRICT ON UPDATE CASCADE,
CONSTRAINT "name" UNIQUE ("englishName" ASC, "chineseName" ASC)
);

-- ----------------------------
-- Records of spice_information
-- ----------------------------

-- ----------------------------
-- Table structure for spice_management
-- ----------------------------
DROP TABLE IF EXISTS "main"."spice_management";
CREATE TABLE "spice_management" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ON CONFLICT ROLLBACK,
"FEMA"  TEXT,
"FDA"  TEXT,
"COE"  TEXT,
"GB"  TEXT,
CONSTRAINT "value" UNIQUE ("FEMA", "FDA", "COE", "GB")
);

-- ----------------------------
-- Records of spice_management
-- ----------------------------

-- ----------------------------
-- Table structure for spice_physics
-- ----------------------------
DROP TABLE IF EXISTS "main"."spice_physics";
CREATE TABLE "spice_physics" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"density"  TEXT,
"refractive"  TEXT,
"solubility"  TEXT,
CONSTRAINT "value" UNIQUE ("density", "refractive", "solubility")
);

-- ----------------------------
-- Records of spice_physics
-- ----------------------------

-- ----------------------------
-- Table structure for spice_type
-- ----------------------------
DROP TABLE IF EXISTS "main"."spice_type";
CREATE TABLE "spice_type" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"name"  TEXT NOT NULL,
CONSTRAINT "name" UNIQUE ("name")
);

-- ----------------------------
-- Records of spice_type
-- ----------------------------
INSERT INTO "main"."spice_type" VALUES (0, '油类');
INSERT INTO "main"."spice_type" VALUES (1, '酊剂类');
INSERT INTO "main"."spice_type" VALUES (2, '浸膏类');
INSERT INTO "main"."spice_type" VALUES (3, '其他');

-- ----------------------------
-- Table structure for sqlite_sequence
-- ----------------------------
DROP TABLE IF EXISTS "main"."sqlite_sequence";
CREATE TABLE sqlite_sequence(name,seq);

-- ----------------------------
-- Records of sqlite_sequence
-- ----------------------------
INSERT INTO "main"."sqlite_sequence" VALUES ('spice_type', 3);
INSERT INTO "main"."sqlite_sequence" VALUES ('user', 1);
INSERT INTO "main"."sqlite_sequence" VALUES ('spice_information', 0);
INSERT INTO "main"."sqlite_sequence" VALUES ('spice_content', 0);

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS "main"."user";
CREATE TABLE "user" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"name"  TEXT NOT NULL,
"password"  TEXT NOT NULL ON CONFLICT FAIL,
"type"  INTEGER NOT NULL ON CONFLICT FAIL DEFAULT 1,
CONSTRAINT "name" UNIQUE ("name")
);

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO "main"."user" VALUES (1, 'admin', 123456, 0);

-- ----------------------------
-- Indexes structure for table spice_type
-- ----------------------------
CREATE UNIQUE INDEX "main"."id"
ON "spice_type" ("id" ASC);
