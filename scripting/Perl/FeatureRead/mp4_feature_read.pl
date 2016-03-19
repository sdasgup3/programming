#!/usr/bin/perl

use strict;
use warnings;

my $FEATURE_START 	= 1; 
my $FEATURE_END 	= 37; 
my $FILE = "MP4_Grading.csv";

my @features =  ();
my @group_info =  ();
my @points_to_add =  (    
                0,    #group						//0
                2,    #User story coverage(at least 3 aspects)		//1
                1,    #User Story 1: correctness
                1,    #User Story 1: simplicity and clearness
                1,    #User Story 2: correctness
                1,    #User Story 2: simplicity and clearness
                1,    #User Story 3: correctness
                1,    #User Story 3: simplicity and clearness
                1,    #User Story 4: correctness
                1,    #User Story 4: simplicity and clearness
                1,    #User Story 5: correctness
                1,    #User Story 5: simplicity and clearness
                1,    #User Story 6: correctness
                1,    #User Story 6: simplicity and clearness
                1,    #User Story 7: correctness
                1,    #User Story 7: simplicity and clearness
                1,    #User Story 8: correctness
                1,    #User Story 8: simplicity and clearness
                1,    #User Story 9: simplicity and clearness
                1,    #User Story 9: correctness
                1,    #User Story 10: simplicity and clearness
                1,    #User Story 10: correctness
                1,    #User Story 11: simplicity and clearness
                1,    #User Story 11: correctness
                1,    #User Story 12: simplicity and clearness
                1,    #User Story 12: correctness
                1,    #Defining the easiest story 
                2,    #based on easiest story,  defining other difficulties
                1,    #Setting the working hours a week
                2,    #setting hours fixed for each iteration and not exceeding the hours set
                1,    #All the iterations should together cover all the user stories
                1,    #none of them duplicated and assigned according to the priority
                1,    #task1: have a specific goal and related to the story. Also  choose the story from the top five priority.
                1,    #task1: Expressed Clearly
                1,    #task2: have a specific goal and related to the story. Also  choose the story from the top five priority.
                1,    #task2: Expressed Clearly
                1,    #task3: have a specific goal and related to the story. Also  choose the story from the top five priority.
                1,    #task3: Expressed Clearly   //37
                );

open(my $fp, "<", "$FILE") or die "cannot open $FILE: $!";
my @lines = <$fp>;
close ($fp);

my $basepath = "/home/sandeep/CS-PHD/Sem1/TA-427/fa13-cs427/_projects";

my $first_line = 0;
foreach my $line (@lines) {
    chomp ($line);

    if(0 == $first_line) { 
        @features = split (/;/, $line); 
        $first_line = 1;
    } else {

        @group_info = split(/;/, $line);
        
        if(-d "$basepath/$group_info[0]/MP4/") {
		open(my $fp_l, ">", "$basepath/$group_info[0]/MP4/$group_info[0]_mp4_grades.txt") or die "cannot open $basepath/$group_info[0]/MP4/$group_info[0]_mp4_grades.txt: $!";

		print $fp_l "\n\n$features[0] - $group_info[0]\n";
		print $fp_l "==============================\n\n\n";
		#print  "\n\n$features[0] - $group_info[0]\n";
		#print  "==============================\n\n\n";
		
		my $Total = 40;
		my $added_points = 0;
		my $feature_point ;

		print $fp_l "Grading Criteria -> Points Obtained\n\n";
		#print  "Grading Criteria -> Points Obtained\n\n";
		for (my $i = $FEATURE_START; $i <= $FEATURE_END ; ) {
                   
                    $feature_point = 0;
		    if (defined($group_info[$i]) and "Y" eq $group_info[$i]) {
                        $feature_point = $points_to_add[$i];
		    }
		    $added_points += $feature_point;
		    my $index = $i - 1 ;
		    print $fp_l "$index. $features[$i] -> $feature_point\n  ";
		    #print  " $index. $features[$i] -> $feature_point\n";
		    $i = $i + 1;
		}
		print $fp_l "\n\n$group_info[0] Total = $added_points/$Total";
		#print "\n\n$group_info[0] Total = $added_points/$Total";

		close ($fp_l);
	}
    }
}
