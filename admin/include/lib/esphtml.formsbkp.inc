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

/* $Id: esphtml.forms.inc,v 1.19 2007/12/10 12:15:55 liedekef Exp $ */

/* vim: set tabstop=4 shiftwidth=4 expandtab: */

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

// string	mkwarn(char *warning);
// string	mkerror(char *error);

// string	mkradio(char *name, char *value);
// string	mkcheckbox(char *name, char *value);
// string	mktext(char *name);
// string   mkhidden(char *name, char *$value)
// string	mktextarea(char *name, int rows, int cols, char *wordwrap);
// string	mkselect(char *name, char *options[]);
// string	mkfile(char *name);

/* {{{ proto string mkwarn(string message)
   Returns HTML format for an warning message. */
function mkwarn ($msg) {
	return("<span class=\"warn\">${msg}</span>\n");
}
/* }}} */

/* {{{ proto string mkerror(string message)
   Returns HTML format for an error message. */
function mkerror ($msg) {
	return("<span class=\"error\">[ ${msg} ]</span>\n");
}
/* }}} */

	
/* {{{ proto string mkradio(string name, string value)
   Returns HTML format for a radio button. */
function mkradio ($_name, $value, $varr = null) {
    	if ($varr == null)
        	$varr =& $_POST;
	$str = '<input type="radio" name="' . htmlspecialchars($_name) .'" value="' . htmlspecialchars($value) .'"';
	if ( isset($varr[$_name]) && $varr[$_name] == $value )
		$str .= " checked=\"checked\"";
	$str .= ' />';
	return($str);
}
/* }}} */
	
/* {{{ proto string mkcheckbox(string name, string value)
   Returns HTML format for a check box. */
function mkcheckbox ($_name, $value, $varr = null) {
    	if ($varr == null)
        	$varr =& $_POST;
	$str = '<input type="checkbox" name="' . htmlspecialchars($_name) .'[]" value="' . htmlspecialchars($value) .'"';
	if (isset($varr) && is_array($varr) && in_array($value, $varr))
		$str .= " checked=\"checked\"";
	$str .= ' />';
	return($str);
}
/* }}} */

/* {{{ proto string mktext(string name, int size)
   Returns HTML format for a text entry line. */
function mktext ($_name, $size = 20, $max = 0, $varr = null) {
        if ($varr == null)
            $varr =& $_POST;
	$size = intval($size);
	$max  = intval($max);
	$str = "size=\"$size\"";
	if ($max)	$str .= " maxlength=\"$max\"";
	if (isset($varr[$_name]))
		return('<input type="text" '. $str .' name="'. htmlspecialchars($_name) .'" value="'. _stripslashes(htmlspecialchars($varr[$_name])) .'" />');
	return('<input type="text" '. $str .' name="'. htmlspecialchars($_name) .'" />');
}
/* }}} */

/* {{{ proto string mkpass(string name)
   Returns HTML format for a password entry line. */
function mkpass ($_name) {
	return('<input type="password" name="'. htmlspecialchars($_name) .'" />');
}
/* }}} */

/* {{{ proto string mkhidden(string name)
   Returns HTML format for a hidden form field. */
function mkhidden ($_name, $varr = null) {
    	if ($varr == null)
        	$varr =& $_POST;
	if (isset($varr[$_name]))
		return('<input type="hidden" name="'. htmlspecialchars($_name) .'" value="'. _stripslashes(htmlspecialchars($varr[$_name])) .'" />');
	return('<input type="hidden" name="'. htmlspecialchars($_name) .'" />');
}
/* }}} */

/* {{{ proto string mktextarea(string name, int rows, int cols, string wrap_type)
   Returns HTML format for a text entry box. */
function mktextarea ($_name, $rows, $cols, $wrap, $varr = null) {
    	if ($varr == null)
        	$varr =& $_POST;
	$str = '<textarea name="' . htmlspecialchars($_name) .'"';
	if($rows > 0)
		$str .= ' rows="' . $rows . '"';
	if($cols > 0)
		$str .= ' cols="' . $cols . '"';
	//if($wrap != '')
	//	$str .= ' wrap="' . strtolower($wrap) . '"';
	$str .= '>';
	if (isset($varr[$_name]))
		 $str .= _stripslashes(htmlspecialchars($varr[$_name]));
	$str .= '</textarea>';
	return($str);
}
/* }}} */

/* {{{ proto string mkselect(string name, array options)
   Returns HTML format for a select box (dropdown). */
function mkselect ($_name, $options, $varr = null,$onclick = null) {
    	if ($varr == null)
        	$varr =& $_POST;
	$str  = "<select name=\"${_name}\"";
	if ($onclick != null && $onclick != '')
		$str .= " onChange=\"${onclick}(this);\" ";
	$str .= ">\n";
	$str .= "<option></option>\n";
	while(list($cid, $content) = each($options)) {
		$checked = '';
		if (isset($varr[$_name]) && $varr[$_name] == $cid)
			$checked = " selected=\"selected\"";
		$str .= "<option value=\"${cid}\"${checked}>${content}</option>\n";
	}
	$str .= "</select>\n";
	return($str);
}
/* }}} */

/* {{{ proto string mkfile(string name)
   Returns HTML format for a file selection button. */
function mkfile ($_name) {
	return('<input type="file" name="'.htmlspecialchars($_name) .'" />');
}
/* }}} */

/* {{{ proto string mksubmit(string name, string value)
   Returns HTML format for a submit button. */
function mksubmit($_name, $_value = null) {
    if ($_value == null)
        $_value = _('Submit');
    if (!empty($_value))
        $_value = ' value="'.htmlspecialchars($_value).'"';
    return '<input type="submit" name="'.htmlspecialchars($_name).'"'.$_value.' />';
}
/* }}} */

/* {{{ proto string show_login(string surveyname, string message, string css, string title)
    Returns HTML login form */
function show_login($_surveyname = null, $_message = null, $_css = null, $_title = "Survey Login") {
	ob_start();
	//error_reporting(E_ALL);
	//ini_set('display_errors', '1');

	//include(realpath('.')."/include/head.inc");
	include realpath(('.')."/include/head.inc");
	$header = ob_get_contents();
	ob_end_flush();
#	$str = $head;
    $str = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
    $str .="<head>\n<title>OpenVULab</title>\n";
    $str .= '    <script type="text/javascript" src="http://vulab.fluidproject.org/js/default.js"></script>
	<script type="text/javascript" src="http://vulab.fluidproject.org/js/jquery-1.2.6.pack.js"></script>
    <script type="text/javascript" src="http://vulab.fluidproject.org/js/jquery.ajaxContent.js"></script>
    <script type="text/javascript" src="http://vulab.fluidproject.org/js/vulab.admin.js"></script>

    <script type="text/javascript" src="http://vulab.fluidproject.org/js/jquery.form.js"></script>
    <script type="text/javascript" src="http://vulab.fluidproject.org/js/thickbox.js"></script>

    <link rel="stylesheet" href="http://vulab.fluidproject.org/js/thickbox.css" type="text/css" media="screen" />
    
<link href="http://vulab.fluidproject.org/admin/style.css" rel="stylesheet" type="text/css" />
';

    if (empty($_css)) {
        $_css = "default.css";
    }
    #$str .= '<link rel="stylesheet" href="'. $GLOBALS['ESPCONFIG']['css_url'].$_css .'" type="text/css">';
    $str .=  "\n</head>\n<body id=\"auth\">\n";
    
    $str .= '<div class="headerGraphic"></div>';
    $str .= '<div class="login2" style="padding:20px; text-align:left;">';
    if ($_title) {
        $str .= "<h1>Login</h1>\n";
    }
    if ($_message) {
        $str .= mkerror($_message);
    }

    $str .= '<form name="loginform" id="loginform1"  method="post">';
    $str .= "\n<fieldset>\n";
    $str .= '<div class="row" style="margin:5px;">';
    $str .= '';
    $str .= "Username: ";
    $str .= '';
    $str .= mktext("username");
    $str .= "\n".'</div><div class="row" style="margin:5px;">'."\n";
    $str .= '';
    $str .= "Password: ";
    $str .= '';
    $str .= mkpass("password");
    $str .= "\n".'</div><div class="row">'."\n";
//    # if the variable $lang is set, we assume we don't need to ask for the language
//    if (!isset($_REQUEST['lang']) && !isset($lang)) {
//       $str .= "Language:";
//       $_languages=array();
//       foreach (esp_getlocales() as $_lang) {
//	   $_languages[$_lang]=$_lang;
//       }
//       $str .= mkselect("lang",$_languages);
//       $str .= "\n".'</div><div class="row">'."\n";
//    }
#    $str .= mksubmit("Login");
	$str .= '<input type="submit" name="Login" value="Login" />';
    $str .= "<br/><a href='signup.php'>Sign up for account</a>";
    # | <a href='lostpassword.php'>Lost Password</a> | <a href='changepassword.php'>Change my Password</a>
    if ($_surveyname != "") {
        $str .= '<input type="hidden" name="'."$_surveyname".'"';
    }
    $str .= "\n</div></fieldset>\n</form>\n";
    $str .= "</div>";
    $str .= "\n</body>\n</html>";

    return $str;
}
/* }}} */
?>