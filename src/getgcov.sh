#!/bin/bash
# Copyright (C) 2006-2011  Trevor Woerner

ALLOW=0
GCOVOUT=`mktemp gcovout.XXXXXXXXXX`
GCOVOUT1=`mktemp gcovout1.XXXXXXXXXX`
gcov -b "${1}" > $GCOVOUT
GOLINES=`wc -l $GCOVOUT | cut -d' ' -f1`
LINECNT=1
while [ $GOLINES -gt 0 ]; do
	line=`cat $GCOVOUT | head -n $LINECNT | tail -n1`
	LINECNT=`expr $LINECNT + 1`

	echo $line | grep "File" > /dev/null
	if [ $? -eq 0 ]; then
		echo $line | grep "ngr" > /dev/null
		if [ $? -eq 0 ]; then
			ALLOW=1
		else
			ALLOW=0
		fi
	fi

	if [ "x$ALLOW" = "x1" ]; then
		echo $line >> $GCOVOUT1
	fi

	GOLINES=`expr $GOLINES - 1`
done

RESULT=`cat $GCOVOUT1`
rm -f $GCOVOUT $GCOVOUT1

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
LINESM2=`expr $LINES - 2`
LINESM3=`expr $LINES - 3`

NEWFILEDATA=`tail -n $LINESM2 $2 | head -n $LINESM3`
NEWFILE=`mktemp tmp.XXXXXXXXXX`

COVFILE=`basename $1 .o`
COVFILE="${COVFILE}.c.gcov"

echo "<?xml version=\"1.0\"?>" >> $NEWFILE
echo "<testsuites xmlns=\"http://check.sourceforge.net/ns\">" >> $NEWFILE
echo $NEWFILEDATA >> $NEWFILE
echo "<coverage>$COVERAGE</coverage>" >> $NEWFILE
echo "<branches>$BRANCHES</branches>" >> $NEWFILE
echo "<taken>$TAKEN</taken>" >> $NEWFILE
echo "<calls>$CALLS</calls>" >> $NEWFILE
echo "</testsuites>" >> $NEWFILE

mv $NEWFILE $2
