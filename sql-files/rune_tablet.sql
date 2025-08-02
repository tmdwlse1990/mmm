CREATE TABLE IF NOT EXISTS `runes` (
    `char_id` int(11) UNSIGNED NOT NULL default '0',
    `rune_id` int(11) UNSIGNED NOT NULL default '0',
    `set_id` int(11) UNSIGNED NOT NULL default '0',
    `selected` tinyint(6) NOT NULL DEFAULT '0',
    `upgrade` smallint(11) UNSIGNED NOT NULL DEFAULT '0',
    `failcount` tinyint(11) UNSIGNED NOT NULL DEFAULT '0',
    `reward` tinyint(1) unsigned DEFAULT '0',
    UNIQUE KEY `char_id` (`char_id`,`rune_id`,`set_id`)
)ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS `runes_book` (
    `char_id` int(11) UNSIGNED NOT NULL DEFAULT '0',
    `rune_id` int(11) UNSIGNED NOT NULL DEFAULT '0',
    `book_id` int(11) UNSIGNED NOT NULL DEFAULT '0',
    UNIQUE KEY `char_id` (`char_id`,`rune_id`,`book_id`)
)ENGINE=MyISAM;