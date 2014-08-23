-- -----------------------------------------------------
-- Table `openbboxmanager`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `openbboxmanager` (
  `id` INTEGER NOT NULL,
  `timestart` LONG NULL ,
  PRIMARY KEY (`id`) )
;


-- -----------------------------------------------------
-- Table `openbboxnode`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `openbboxnode` (
  `id` INTEGER NOT NULL  ,
  `idmanager` INTEGER NULL ,
  `timestart` LONG NULL ,
  `timeend` LONG NULL ,
  `label` VARCHAR(100) NULL ,
  `mac` VARCHAR(20) NULL ,
  `ip` VARCHAR(20) NULL ,
  `port` INTEGER NULL ,
  `numcameras` INTEGER NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_manager_node`
    FOREIGN KEY (`idmanager` )
    REFERENCES `openbboxmanager` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `subject`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `subject` (
  `id` INTEGER NOT NULL  ,
  `tag` VARCHAR(255) NULL ,
  `type` VARCHAR(45) NULL ,
  `label` VARCHAR(45) NULL ,
  `protocol` VARCHAR(45) NULL ,
  `timeserver` LONG NULL ,
  `birthdate` LONG NULL ,
  `deathdate` LONG NULL ,
  `arrivaldate` LONG NULL ,
  PRIMARY KEY (`id`) )
;


-- -----------------------------------------------------
-- Table `taskfile`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `taskfile` (
  `id` INTEGER NOT NULL  ,
  `filename` VARCHAR(100) NULL ,
  `timeserver` LONG NULL ,
  `taskname` VARCHAR(100) NULL ,
  `type` VARCHAR(20) NULL ,
  `file` BLOB NULL ,
  `hash` VARCHAR(255) NULL ,
  PRIMARY KEY (`id`) )
;


-- -----------------------------------------------------
-- Table `behaviortask`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `behaviortask` (
  `id` INTEGER NOT NULL  ,
  `idconn` INTEGER NULL ,
  `idsubject` INTEGER NULL ,
  `idtaskfile` INTEGER NULL ,
  `timestart` LONG NULL ,
  `timeend` LONG NULL ,
  `label` VARCHAR(100) NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_idconn_behaviortask`
    FOREIGN KEY (`idconn` )
    REFERENCES `openbboxnode` (`id` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_behaviortask_1`
    FOREIGN KEY (`idsubject` )
    REFERENCES `subject` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  CONSTRAINT `fk_behaviortask_2`
    FOREIGN KEY (`idtaskfile` )
    REFERENCES `taskfile` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `videostreampacket`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `videostreampacket` (
  `id` INTEGER NOT NULL  ,
  `idtask` INTEGER NULL ,
  `port` INTEGER NULL ,
  `idpacket` INTEGER NULL ,
  `timeserver` LONG NULL ,
  `timesec` LONG NULL ,
  `timeusec` LONG NULL ,
  `sizeexpected` INTEGER NULL ,
  `size` INTEGER NULL ,
  `width` INTEGER NULL ,
  `height` INTEGER NULL ,
  `format` INTEGER NULL ,
  `filename` VARCHAR(100) NULL ,
  `headerframe` INTEGER NULL ,
  `checksum` INTEGER NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_idconn_video`
    FOREIGN KEY (`idtask` )
    REFERENCES `behaviortask` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `behavioreventpacket`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `behavioreventpacket` (
  `id` INTEGER NOT NULL  ,
  `idtask` INTEGER NULL ,
  `port` INTEGER NULL ,
  `idpacket` INTEGER NULL ,
  `timeserver` LONG NULL ,
  `timesec` LONG NULL ,
  `timeusec` LONG NULL ,
  `pinscontext` INTEGER NULL ,
  `pinevent` INTEGER NULL ,
  `pineventlabel` VARCHAR(20) NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_idconn_event`
    FOREIGN KEY (`idtask` )
    REFERENCES `behaviortask` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `behaviortable`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `behaviortable` (
  `id` INTEGER NOT NULL  ,
  `idtask` INTEGER NULL ,
  `port` INTEGER NULL ,
  `idpacket` INTEGER NULL ,
  `timeserver` LONG NULL ,
  `event` VARCHAR(20) NULL ,
  `motortime` FLOAT NULL ,
  `rewardtime` FLOAT NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_idconn_event`
    FOREIGN KEY (`idtask` )
    REFERENCES `behaviortask` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `behaviorinfo`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `behaviorinfo` (
  `id` INTEGER NOT NULL  ,
  `idtask` INTEGER NULL ,
  `idpacket` INTEGER NULL ,
  `type` VARCHAR(45) NULL ,
  `timeserver` LONG NULL ,
  `timesec` LONG NULL ,
  `timeusec` LONG NULL ,
  `info` VARCHAR(255) NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_idconn`
    FOREIGN KEY (`idtask` )
    REFERENCES `behaviortask` (`id` )
    ON DELETE SET NULL
    ON UPDATE CASCADE)
;


-- -----------------------------------------------------
-- Table `controlcommand`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `controlcommand` (
  `id` INTEGER NOT NULL  ,
  `idconn` INTEGER NULL ,
  `timesent` LONG NULL ,
  `timeanswered` LONG NULL ,
  `command` INTEGER NULL ,
  `answer` INTEGER NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_conn_control`
    FOREIGN KEY (`idconn` )
    REFERENCES `openbboxnode` (`id` )
    ON DELETE CASCADE
    ON UPDATE CASCADE)
;

