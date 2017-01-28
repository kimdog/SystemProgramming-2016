
LKM 관련 파일

aa.c
aa.ko
Makefile

- write는 /proc/myproc/myproc 에 echo 10000 > myproc 형식으로 써주면 됨.


각 port별 결과 로그 파일

1111.txt 2222.txt 3333.txt 4444.txt 5555.txt

-----------------------

수신 프로그램은 전 warm-up 과제에 이미 제출하였다.
이 LKM은 다음의 PORT를 static하게 인지하기 때문에
통신 시 다음과 같이 PORT를 설정해 주어야한다.

1111 2222 3333 4444 5555


ip의 경우는 확인 코드를 넣어줬으면 더 확실했겠지만
단순히 port만 확인하여 packet drop 처리를 하기 때문에
특정하게 지정하지 않아도 됨.