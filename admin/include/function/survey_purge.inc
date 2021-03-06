<?php
/*****************************************************************************
Copyright 2008 York University

Licensed under the Educational Community License (ECL), Version 2.0 or the New
BSD license. You may not use this file except in compliance with one these
Licenses.

You may obtain a copy of the ECL 2.0 License and BSD License at
https://source.fluidproject.org/svn/LICENSE.txt
*******************************************************************************/
?>
<?php

/* $Id: survey_purge.inc,v 1.9 2007/11/26 11:36:17 liedekef Exp $ */

/* vim: set tabstop=4 shiftwidth=4 expandtab: */

// Written by James Flemer
// <jflemer@alum.rpi.edu>

/* {{{ proto void survey_purge(array survey_ids)
   Purges all traces of survey(s) from the database.
   Returns void. */
function survey_purge($sids) {
	if(is_int($sids)) {
		$sidstr = '='.$sids;
	} else if (is_string($sids)) {
		$sids = split(" ",$sids);
	}
	if(is_array($sids)) {
		$sidstr = array_to_insql($sids);
	}

	// make a list of question IDs
	$sql = "SELECT id FROM ".$GLOBALS['ESPCONFIG']['question_table']." WHERE survey_id ${sidstr}";
	$result = execute_sql($sql);
	$qids = array();
	while(list($qid) = fetch_row($result)) {
        $result->MoveNext();
		array_push($qids, $qid);
	}
	db_close($result);
	$qidstr = array_to_insql($qids);
	
	// work from the bottom up...
	// start with the survey results
	$tables = array('response_bool','response_date','response_multiple','response_other','response_rank','response_single','response_text');
	foreach($tables as $table) {
		$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG'][$table.'_table']." WHERE question_id ${qidstr}";
		$result = execute_sql($sql);
	}
	// then responses
	$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG']['response_table']." WHERE survey_id ${sidstr}";
	execute_sql($sql);

	// then question choices
	$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG']['question_choice_table']." WHERE question_id ${qidstr}";
	execute_sql($sql);

	// then questions
	$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG']['question_table']." WHERE survey_id ${sidstr}";
	execute_sql($sql);

	// then conditions
	$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG']['condition_table']." WHERE survey_id ${sidstr}";
	execute_sql($sql);

	// and finally the survey(s)
	$sql = "DELETE FROM ".$GLOBALS['ESPCONFIG']['survey_table']." WHERE id ${sidstr}";
	execute_sql($sql);

	return;
}
/* }}} */

?>