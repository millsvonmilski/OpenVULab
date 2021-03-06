/*
Copyright 2007-2010 University of Cambridge
Copyright 2007-2009 University of Toronto
Copyright 2010 Lucendo Development Ltd.

Licensed under the Educational Community License (ECL), Version 2.0 or the New
BSD license. You may not use this file except in compliance with one these
Licenses.

You may obtain a copy of the ECL 2.0 License and BSD License at
https://source.fluidproject.org/svn/LICENSE.txt
*/

/*global fluid, jQuery, jqUnit, $*/

fluid.registerNamespace("fluid.testUtils");

/*
 * A number of utility functions for creating "duck-type" events for testing various key
 * stroke combinations.
 */

fluid.testUtils.ctrlKeyEvent = function (keyCode, target) {
    return fluid.testUtils.modKeyEvent("CTRL", keyCode, target);
};

fluid.testUtils.keyEvent = function (keyCode, target) {
    return {
        keyCode: fluid.reorderer.keys[keyCode],
        target: fluid.unwrap(target),
        preventDefault: function () {}, 
        stopPropagation: function () {} 
    };
};

fluid.testUtils.modKeyEvent = function (modifier, keyCode, target) {
    var togo = fluid.testUtils.keyEvent(keyCode, target);
    modifier = jQuery.makeArray(modifier);
    for (var i = 0; i < modifier.length; ++ i) {
        var mod = modifier[i];
        if (mod === "CTRL") {
            togo.ctrlKey = true;
        }
        else if (mod === "SHIFT") {
            togo.shiftKey = true;
        }
        else if (mod === "ALT") {
            togo.altKey = true;
        }
    }
    return togo;
};

/** Sort a component tree into canonical order, to facilitate comparison with
 * deepEq */

fluid.testUtils.sortTree = function (tree) {
    function comparator(ela, elb) {
        var ida = ela.ID || "";
        var idb = elb.ID || "";
        var cola = ida.indexOf(":") === -1;
        var colb = idb.indexOf(":") === -1;
        if (cola && colb) { // if neither has a colon, compare by IDs if they have IDs
            return ida.localeCompare(idb);
        }
        else {
            return cola - colb; 
        }
    }
    if (fluid.isArrayable(tree)) {
        tree.sort(comparator);
    }
    fluid.transform(tree, function (value) {
        if (!fluid.isPrimitive(value)) {
            fluid.testUtils.sortTree(value);
        }
    });
      
};

fluid.testUtils.assertTree = function (message, expected, actual) {
    fluid.testUtils.sortTree(expected);
    fluid.testUtils.sortTree(actual);
    jqUnit.assertDeepEq(message, expected, actual);
};

/** Condense a DOM node into a plain Javascript object, to facilitate testing against
 * a trial, with the use of assertDeepEq or similar
 */
fluid.testUtils.assertNode = function (message, expected, node) {
    var togo = {};
    if (!node.nodeType) { // Some types of DOM nodes (e.g. select) have a valid "length" property
        if (node.length === 1 && expected.length === undefined) {
            node = node[0];
        }
        else if (node.length !== undefined) {
            jqUnit.assertEquals("Expected number of nodes " + message, expected.length, node.length);
            for (var i = 0; i < node.length; ++ i) {
                fluid.testUtils.assertNode(message + ": node " + i + ": ", expected[i], node[i]);
            }
            return;
        }
    }
    for (var key in expected) {
        // mustn't use DOM getAttribute because of numerous bugs (in particular http://www.quirksmode.org/bugreports/archives/2007/03/getAttributefor_is_always_null_in_Internet_Explore.html )
        var attr = jQuery.attr(node, key);
        var messageExt = " - attribute " + key + "";
        if (key === "nodeName") {
            attr = node.tagName.toLowerCase();
            messageExt = " - node name";
        }
        else if (key === "nodeText") {
            attr = jQuery.trim(fluid.dom.getElementText(node));
        }
        else if (key === "nodeHTML") {
            attr = $(node).html();
        }
        var evalue = expected[key];
        var pass = evalue === attr;
        if (attr === false || attr === true) { // support for IE refusing to honour XHTML values
            pass = !!evalue === attr;
        }
        if (key !== "children") {
            jqUnit.assertTrue(message + messageExt + " expected value: " + evalue + " actual: " + attr, pass);
        }
        else {
            var children = $("> *", node);
            fluid.testUtils.assertNode("> " + message, evalue, children);
        }
    }
  
};
