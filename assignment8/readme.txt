mutex lock과 condition variable을 사용시에는
condition queue에 들어가기 전에 signal을 보낼 상황이 생길 수 있어서
새로운 전역변수 flag를 이용해서 구현하였다.
그리고 mutex lock을 critical section보호에 사용되었고
flag에 따라서 thread가 wait으로 상태가 변경된다.

semaphore로 구현할 때는 위 처럼 먼저 실행되는 것을 고려하지 않고 
flag 변수 없이 단순히 binary semaphore2개를 생성하여
서로 전송했다는 신호를 줄 수 있다.
받는 쪽이 먼저 실행되었으면 0으로 wait로 상태가 되고
보내는 쪽이 먼저 실행되었으면 이미 1이므로 바로 실행될 수 있다.
그리고 논리적으로 교차적으로 실행될 수 밖에 없기 떄문에
critical section을 보호하였다.
