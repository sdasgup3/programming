#!/usr/bin/perl

use strict;
use warnings;


my @features =  ();
my @group_info =  ();
my @points_to_deduct =  (    
                0,        #group
                0,        #completed    
                1,    #Implemented Element::addChild() but no exception thrown
                1,    #Implemented Element::addChild() but not called
                2,    #Unimplemented Element::addChild() or any of its alternative
                2,    #Unimplemented Element::getChildren()
                1,    #Unimplemented Book::AddToCollection (1) 
                1,    #Unimplemented Book::removeFromCollectionToCollection or any of its alternative 
                1,    #Unimplemented Book::GetStringRepresentation
                1,    #Unimplemented Book:: Constructor
                2,    #Unimplemented Collection:: getStringRepresentation 
                2,    #Unimplemented Collection::RestoreCollection
                3,    #Unimplemented Library::SaveLibraryToFile
                3,    #Unimplemented Library:RecreateLibraryFromFile or any alternative
                3,    #Missing/improper Test Scenario 1: Add Book
                3,    #Missing/Improper Test Scenario 2: Remove Book
                3,    #Missing/Improper Test Scenario 3: Recreate Book from String
                3,    #Missing/Improper Test Scenario 4: Save the book in string
                3,    #Missing/Improper Test Scenario 5: Recreate Collection from String
                3,    #Missing/Improper Test Scenario 6: Save collection to string
                3,    #Missing/Improper Test Scenario 7: Save library to file
                3,    #Missing/Improper Test Scenario 8: Restore library from file
                3,    #Missing/Improper Test Scenario 9: Collection Inclusion
                1,    #Style(too long)
                3,    #Style  (Newly added methods not documented) 
                5,    #Style  (bad variable name)
                1,    #Style  (bad data structures)
                5,    #Missing/Inconsistent indentation 
                3,    #If/Else on same line 
                5,    #History Not submitted
                1,    #History short
                1,    #Wrong submissions/ project classpath issues
                0,    #Special
                1,    #CompileError
                1,    #Many scenarios in One test	
                1,    #Test are put In wrong directory
                1,    #RunTimeError
                );

open(my $fp, "<", "mp1_grades.csv") or die "cannot open mp1_grades.csv: $!";
my @lines = <$fp>;
close ($fp);

my $basepath = "/home/sandeep/CS-PHD/Sem1/TA-427/fa13-cs427/_projects";

my $first_line = 0;
foreach my $line (@lines) {
    chomp ($line);

    if(0 == $first_line) { 
        @features = split (/,/, $line); 
        $first_line = 1;
    } else {

        @group_info = split(/,/, $line);
        
        # Print the grading info
	open(my $fp_l, ">", "$basepath/$group_info[0]/MP1/$group_info[0]_mp1_grades.txt") or die "cannot open $basepath/$group_info[0]/MP1/$group_info[0]_mp1_grades.txt: $!";

        print $fp_l "\n\n$features[0] - $group_info[0]\n";
        print $fp_l "==============================\n\n\n";
        
        my $Total = 60;
        my $special_switch_on = 0;

        print $fp_l "Grading Criteria -> Points Deducted\n\n";
        for (my $i = 2; $i <= 36 ; ) {
            my $deducted_points = 0;
            if (defined($group_info[$i]) and "Y" eq $group_info[$i]) {
                $deducted_points = $points_to_deduct[$i];
                $Total = $Total - $deducted_points;

                if(defined ($features[$i]) and ( "Special" eq $features[$i] )) {
                    $special_switch_on = 1;
                }
            }
            if("Special" ne $features[$i] ) {
		my $index = $i - 1 ;	
                print $fp_l "$index. $features[$i] -> $deducted_points\n  ";
            }		
            $i = $i + 1;
        }
        if(1 == $special_switch_on) {
            $Total = 0;
	}
        print $fp_l "\n\n$group_info[0] Total = (60 - (total deduction)) = $Total";

       close ($fp_l);
    }
}

#foreach my $line (@features) {
#print "$line".  "\n";
#}

