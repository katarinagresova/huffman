#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YEL='\033[0;33m'
NC='\033[0m' # No Color
total=0
ok=0

if [ $# -lt 1 ];then 
    refPath="tests/files/";
elif [ "$#" -eq 1 ]; then
    if [ $1 -le 0 ];then 
        echo "ERROR: Number must be greater than 0"
        exit 1
    else
        refPath="tests/random/"
        if [ -d "$refPath" ]; then rm -Rf $refPath; fi
        mkdir "$refPath"
        mkdir "$refPath/original"
        for (( i = 0; i < $1; i++ )); do
          random=$(($RANDOM % 1000 + 10000))
          head -c ${random} /dev/urandom > ${refPath}/original/random_${i}
        done
    fi
fi

# Create folders
if [ -d "$refPath/coded" ]; then rm -Rf $refPath/coded; fi
mkdir "$refPath/coded"
if [ -d "$refPath/decoded" ]; then rm -Rf $refPath/decoded; fi
mkdir "$refPath/decoded"

echo "Starting testing..."
if [ ! -f "./huff_codec" ]
then
  echo -e "./huff_codec ${RED}not found.${NC}"
  exit 1
fi
echo "***********************************************"



for fullfile in `ls -Sr ${refPath}original/*`;
do
  filename="${fullfile##*/}"
  ((total++))

  # static

  ./huff_codec -c -h static -i ${refPath}original/${filename} -o ${refPath}/coded/static-${filename}
  if [ "$?" != "0" ]
    then echo -e "[${RED}Static   code BAD${NC}]"
    exit
  fi
  ./huff_codec -d -h static -i ${refPath}/coded/static-${filename} -o ${refPath}/decoded/decode-static-${filename}
  if [ "$?" != "0" ]
    then echo -e "[${RED}Static   code BAD${NC}]"
    exit
  fi

  diff ${refPath}original/${filename} ${refPath}decoded/decode-static-${filename} 

  if [ "$?" == "0" ]
    then
      origFileSize=$(wc -c <"${refPath}original/${filename}")
      codeFileSize=$(wc -c <"${refPath}/coded/static-${filename}")
      if [ "$origFileSize" -ne 0 ] 
        then
          echo -e "[${filename}\t${GREEN}Static      OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: $((100 - (100*$codeFileSize/$origFileSize)))%]"
        else
          echo -e "[${filename}\t${GREEN}Static      OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: 0]"
      fi
      ((ok++))
    else
      echo -e "[${filename}\t${RED}Static   BAD${NC}]"
  fi
  ((total++))

  # static with model

  # ./huff_codec -c -h static -i ${refPath}original/${filename} -o ${refPath}/coded/static-m-${filename} -m
  # if [ "$?" != "0" ]
  #   then echo -e "[${RED}Static -m   code BAD${NC}]"
  #   exit
  # fi
  # ./huff_codec -d -h static -i ${refPath}/coded/static-m-${filename} -o ${refPath}/decoded/decode-static-m-${filename} -m
  # if [ "$?" != "0" ]
  #   then echo -e "[${RED}Static -m   code BAD${NC}]"
  #   exit
  # fi

  # diff ${refPath}original/${filename} ${refPath}decoded/decode-static-m-${filename} 

  # if [ "$?" == "0" ]
  #   then
  #     origFileSize=$(wc -c <"${refPath}original/${filename}")
  #     codeFileSize=$(wc -c <"${refPath}/coded/static-m-${filename}")
  #     if [ "$origFileSize" -ne 0 ] 
  #       then
  #         echo -e "[${filename}\t${GREEN}Static -m   OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: $((100 - (100*$codeFileSize/$origFileSize)))%]"
  #       else
  #         echo -e "[${filename}\t${GREEN}Static -m   OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: 0]"
  #     fi
  #     ((ok++))
  #   else
  #     echo -e "[${filename}\t${RED}Static -m   BAD${NC}]"
  # fi
  # ((total++))

  # # adaptive

   ./huff_codec -c -h adaptive -i ${refPath}original/${filename} -o ${refPath}/coded/adaptive-${filename}
   if [ "$?" != "0" ]
     then echo -e "[${RED}Adaptive code BAD${NC}]"
     exit
   fi
   ./huff_codec -d -h adaptive -i ${refPath}/coded/adaptive-${filename} -o ${refPath}/decoded/decode-adaptive-${filename}
   if [ "$?" != "0" ]
     then echo -e "[${RED}Adaptive code BAD${NC}]"
     exit
   fi

   diff ${refPath}original/${filename} ${refPath}decoded/decode-adaptive-${filename}

   if [ "$?" == "0" ]
     then
       origFileSize=$(wc -c <"${refPath}original/${filename}")
       codeFileSize=$(wc -c <"${refPath}/coded/adaptive-${filename}")
       if [ "$origFileSize" -ne 0 ]
         then
           echo -e "[${filename}\t${GREEN}Adaptive    OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: $((100 - (100*$codeFileSize/$origFileSize)))%]"
         else
           echo -e "[${filename}\t${GREEN}Adaptive    OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: 0]"
       fi
       ((ok++))
     else
       echo -e "[${filename}\t${RED}Adaptive BAD${NC}]"
   fi
   ((total++))

  # # adaptive model

  # ./huff_codec -c -h adaptive -i ${refPath}original/${filename} -o ${refPath}/coded/adaptive-m-${filename} -m
  # if [ "$?" != "0" ]
  #   then echo -e "[${RED}Adaptive -m code BAD${NC}]"
  #   exit
  # fi
  # ./huff_codec -d -h adaptive -i ${refPath}/coded/adaptive-m-${filename} -o ${refPath}/decoded/decode-adaptive-m-${filename} -m
  # if [ "$?" != "0" ]
  #   then echo -e "[${RED}Adaptive -m code BAD${NC}]"
  #   exit
  # fi

  # diff ${refPath}original/${filename} ${refPath}decoded/decode-adaptive-m-${filename}

  # if [ "$?" == "0" ]
  #   then
  #     origFileSize=$(wc -c <"${refPath}original/${filename}")
  #     codeFileSize=$(wc -c <"${refPath}/coded/adaptive-m-${filename}")
  #     if [ "$origFileSize" -ne 0 ] 
  #       then
  #         echo -e "[${filename}\t${GREEN}Adaptive -m OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: $((100 - (100*$codeFileSize/$origFileSize)))%]"
  #       else
  #         echo -e "[${filename}\t${GREEN}Adaptive -m OK${NC}]\t[origSize: ${origFileSize},\tcodeSize: ${codeFileSize},\tcompression: 0]"
  #     fi
  #     ((ok++))
  #   else
  #     echo -e "[${filename}\t${RED}Adaptive -m BAD${NC}]"
  # fi
  # echo -e

done
    
echo "***********************************************"
echo -e "FINISHED TESTING:"
echo -e "OK/TOTAL: ${ok}/${total}"

if [ "$ok" == "$total" ]
  then
    echo -e "[${GREEN}SUCCESSFULL${NC}]"
  else 
    echo -e "[${RED}FAILED${NC}]"
fi

