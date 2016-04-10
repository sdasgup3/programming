#!/bin/bash  

mod_files=`git status | grep modified | cut -f 2 -d : | xargs`;
new_files=`git status | grep "new file" | cut -f 2 -d : | xargs`;
file_to_commit="$mod_files $new_files" 

echo $file_to_commit
echo "Are U OK?"

read R;
if [ $R = "y" ] || [ $R = "Y" ] ; then
  #This also works; the command line arg should be in quotes
  #M="$1"; 
  echo "Enter Message (with quotes)?";
  read M;
  git pull origin master && git commit -m "$M"  $file_to_commit && git push origin master;
fi
