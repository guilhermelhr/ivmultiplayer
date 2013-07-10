SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE IF NOT EXISTS `bans` (
  `address` varchar(15) NOT NULL,
  `reason` varchar(64) NOT NULL DEFAULT 'Not specified',
  `ban_date` datetime NOT NULL,
  UNIQUE KEY `address` (`address`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `servers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `address` varchar(15) NOT NULL,
  `port` int(6) NOT NULL,
  `version` varchar(8) NOT NULL,
  `last_post` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=3 ;
