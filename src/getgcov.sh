#!/bin/bash
# Copyright (C) 2006-2011  Trevor Woerner


RESULT=`gcov -b "${1}"`
echo $RESULT | grep "No branches" > /dev/null 2>&1
if [ $? -eq 1 ]; then
	COVERAGE="`echo $RESULT | cut -d'%' -f1 | cut -d':' -f2`%"
	BRANCHES="`echo $RESULT | cut -d'%' -f2 | cut -d':' -f2`%"
	TAKEN="`echo $RESULT | cut -d'%' -f3 | cut -d':' -f2`%"
	CALLS="`echo $RESULT | cut -d'%' -f4 | cut -d':' -f2`%"
else
	COVERAGE="`echo $RESULT | cut -d'%' -f1 | cut -d':' -f2`%"
	BRANCHES="(n/a)"
	TAKEN="(n/a)"
	CALLS="`echo $RESULT | cut -d'%' -f2 | cut -d':' -f2`%"
fi

LINES=`wc -l $2 | cut -d' ' -f1`
LINESM1=`expr $LINES - 1`

NEWFILEDATA=`head -n $LINESM1 $2`
NEWFILE=`mktemp tmp.XXXXXXXXXX`

echo $NEWFILEDATA >> $NEWFILE
echo "<coverage>$COVERAGE</coverage>" >> $NEWFILE
echo "<branches>$BRANCHES</branches>" >> $NEWFILE
echo "<taken>$TAKEN</taken>" >> $NEWFILE
echo "<calls>$CALLS</calls>" >> $NEWFILE
echo "</testsuites>" >> $NEWFILE

mv $NEWFILE $2
