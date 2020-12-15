#!/bin/bash

echo "Fetching countries from covid19api"
curl --location --request GET 'https://api.covid19api.com/countries'|  tr "," "\n"  | grep "Slug" | awk -F ":" '{print $2}' | grep -v 'Uninted State of America\|united-states' | tr -d  '"' | sort -d > countries
echo "countries file is ready"

for country in $(cat countries);
do
        echo "Fetching data of ${country} into DB..."
        wget "https://api.covid19api.com/total/country/${country}"

        sed  's/},/\n/g' "${country}" | awk -F , '{print $12 $13 $8 $9 $10 $11}' | awk -F '"' '{print $4 $7 $9 $11 $13}' > "${country}.tmp3"
        sed 's/T00:00:00Z//g' "${country}.tmp3" > "${country}.tmp4"
        sed 's/:/,/g' "${country}.tmp4" > "${country}"
        rm "${country}.tmp3" "${country}.tmp4"
    minimumSize=3
    actualSize=$(wc -c <"${country}")
    if [ $minimumSize -ge $actualSize ]; then
      rm "${country}"
      grep -v "${country}" countries > countries.temp
      mv countries.temp countries
      echo "The data for ${country} was deleted from DB"
    else
      echo "Finished successfully to get data for ${country} into DB"
    fi
done

echo "DB is ready, enjoy!"
