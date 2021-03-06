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

/* $Id: questions_options.inc,v 1.18 2007/11/26 16:03:56 liedekef Exp $ */

/* vim: set tabstop=4 shiftwidth=4 expandtab: */

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

// New Interface Design by Kon Angelopoulos
// for the phpESP project.
// <angekproductions@iprimus.com.au>

	// do not need to import session vars here
	// it is done by questions.inc before this code executes

	// this type of question has answer options
	if ($updated && $curr_q) {
		$sql = "SELECT id,content FROM ".$GLOBALS['ESPCONFIG']['question_choice_table']." WHERE question_id=${curr_q} ORDER BY id";
		$result = execute_sql($sql);
		$c = record_count($result);
	} else {
		if ($curr_q && isset($_POST['num_choices']))
			$c = intval($_POST['num_choices']);
		else
			$c = $GLOBALS['ESPCONFIG']['default_num_choices'];
	}
	if(isset($_POST['extra_choices']))
		$num_choices = max($c, $_POST['num_choices']) + 1;
	else
		$num_choices = $c;
?>
</div>
<div id="multichoice" class="fl-container-650" style="padding:15px;display:none;">
    <div class="fl-container-flex fl-col-mixed content">
        <div id="answers_input" class="fl-container-flex header">
			<?php printf(_('Enter the possible <strong>answers</strong> (if applicable). Enter
%s on an line by itself to create a
fill-in-the-blank answer at the end of this question.
Any blank lines will be suppressed.'), '<tt>!other</tt>'); ?>
			<input type="hidden" id="num_choices" name="num_choices" value="<?php echo($num_choices); ?>" />
        </div>
        <div class="fl-container content">
            <ol>
<?php
	for($i=1; $i<$num_choices+1; ++$i) {
            echo('<div style="padding:5px;"><li>');
		if ($updated && $curr_q) {
			list($choice_id, $choice_content) = fetch_row($result);
			$result->MoveNext();
		} else {
			if ($curr_q) {
			   $choice_id = intval($_POST["choice_id_$i"]);
			   if(isset($_POST["choice_content_$i"]))
			      $choice_content = _stripslashes($_POST["choice_content_$i"]);
			}
		}
?>
	  
			<input type="hidden" name="choice_id_<?php echo($i); ?>"
				value="<?php if(isset($choice_id)) echo($choice_id); ?>" />
			<input type="text" size="60" name="choice_content_<?php echo($i); ?>"
				value="<?php if(isset($choice_content)) echo(htmlspecialchars($choice_content)); ?>" />
		
            </li></div>
<?php	} ?>
            </ol>
        </div>
    <div class="footer">
	
		&nbsp;
		<input type="button" name="extra_choices" onclick="addAnswerLine()" value="<?php echo(_('Add another answer line')); ?>" />
	
    </div>
    </div>
</div>