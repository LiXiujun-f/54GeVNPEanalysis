cp pico.list tmp

for badrun in `cat badrunlist.txt`; do
  # sed -i -e "/_${badrun}_/d" tmp 
  sed "/_${badrun}_/d" tmp > tmp.rm
  echo $badrun
  cp tmp.rm tmp
  rm tmp.rm
done

