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
/* $Id: debug.inc,v 1.12 2005/08/01 16:12:53 greggmc Exp $ */

/* vim: set tabstop=4 shiftwidth=4 expandtab: */

// Written by James Flemer
// <jflemer@alum.rpi.edu>

	$arr = array();
	
	array_push($arr, array('SID', session_id()));
	if (@file_exists(session_save_path()."/sess_".session_id()))
		array_push($arr, array('SESSION', implode('', @file(session_save_path()."/sess_".session_id()))));
	
	if(isset($_SESSION['acl'])) {
		array_push($arr, 'ACL');
		foreach ($_SESSION['acl'] as $key => $val) {
			if(is_array($val))
				$val = '[ '. implode(' ', $val) .' ]';
			array_push($arr, array($key, $val));
		}
	}
	
	foreach (array(
		//'HTTP_SESSION_VARS', 'ESPCONFIG', 'HTTP_GET_VARS',
		'_SESSION', 'ESPCONFIG', '_GET',
		//'HTTP_POST_VARS', 'HTTP_SERVER_VARS', 'HTTP_ENV_VARS') as $v) {
		'_POST', '_SERVER', '_ENV') as $v) {
		if(isset($$v)) {
			array_push($arr, $v);
			foreach ($$v as $key => $val) {
				if(is_array($val))
					$val = '[ '. @implode(' ', $val) .' ]';
				array_push($arr, array($key, $val));
			}
		}
	}
	
	$str = "<table cellspacing=\"0\" cellpadding=\"0\" width=\"590\">\n";
	foreach ($arr as $key) {
		if (is_array($key)) {
			$str .= '<tr><td bgcolor="#ccccff">' .
					htmlspecialchars($key[0]) . '</td>';
			$str .= '<td bgcolor="#cccccc">' .
					@htmlspecialchars($key[1]) . "</td></tr>\n";
		} else {
			$str .= '<tr><th align="left" colspan="2" bgcolor="#ccccff">' .
					htmlspecialchars($key) . "</th></tr>\n";
		}
	}
	$str .= "</table>\n";
?>
<script language="JavaScript">
<!-- // Begin <?php // This should really go into <head> tag ?>

function windowOpener(title,msg) {
  msgWindow=window.open("","displayWindow","menubar=no,alwaysRaised=yes,dependent=yes,width=600,height=500,scrollbars=yes,resizable=yes");
  msgWindow.document.write("<html><head><title>"+title+"</title></head>");
  msgWindow.document.write("<body>"+msg+"</body></html>");
}

function debugWindow () {
 title="Debug Window";
 msg="<?php echo(addcslashes($str, "\0..\31\\\"")); ?>";
 windowOpener(title, msg);
}
// End -->
</script>
<form name="debug"><input type="button" value="debug" onClick="debugWindow()"></form>
