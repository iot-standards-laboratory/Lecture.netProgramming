# 개요

리눅스 환경에서 네트워크 프로그래밍을 위해 우리는 **<u>커널, 컴파일러, 에디터</u>** 3개를 준비해야한다. 



# 커널 준비

리눅스 환경에서 작동하는 프로그램을 만들기 위해서는 네트워크 리눅스 커널을 준비해야한다. 엄밀히 따지면 커널 준비라기 보다는 POSIX 기반 소프트웨어 개발 환경을 준비하는 것이며, mac os 환경을 사용 중이라면 별도의 준비없이 다음 절차로 넘어가도 된다. 

본 절에서는 Windows 사용자를 위해 리눅스 커널을 준비하는 방법에 대해 소개한다. Windows 사용자라면 Windows에서 POSIX기반 소프트웨어를 구동 및 개발할 수 있는 환경을 제공하는 Cygwin을 사용할 수 있지만 우리는 WSL과 Virtual Machine을 사용하는 2가지 방식을 소개한다. 

- [Virtual Machine 을 통해 리눅스 커널 준비하기](./vm/README.md)
  - `.ova` 파일을 통해 리눅스 커널을 준비한 경우 **<u>컴파일러 및 에디터를 사전에 준비해놓은 상태이니 아래의 절차를 생략하고 바로 코딩을 하실 수 있습니다.</u>**
  - [ova 파일 다운로드](https://drive.google.com/file/d/136On8uGjdzVvn94-zPnoxAqfUx_l-31F/view)
- [WSL을 통해 리눅스 커널 준비하기](./wsl/README.md)



# 컴파일러 준비

C언어는 대표적인 컴파일 언어이며, gcc, clang과 같은 컴파일러를 준비하여야 한다. 아래의 명령어는 clang 컴파일러를 설치하는 명령어이며, gcc 사용을 원하는 사용자는 아래 명령어에서 clang을 gcc로 대체하여 설치할 수 있다. 

```zsh
sudo apt install clang llvm
```

(VMware에서 .ova 파일을 사용하는 경우 바로 vscode를 실행하여 간단한 코드를 작성하고, clang 또는 gcc를 통해 컴파일을 진행하면 된다.)



# 에디터 준비

에디터는 코딩 중 가장 많이 사용하는 도구로, 코딩을 위해서 자신에게 맞는 에디터를 찾는 것도 매우 중요하다. 본 포스트에서는 vim과 vscode 2가지를 이용해 c언어 코딩을 수행하는 방식을 설명하고 있다. 



## vim 설치

vim은 매우 오랜 역사를 가진 에디터로, CLI에서 바로 사용할 수 있기 때문에 SSH를 통해 원격으로 접속하여 코딩을 하고자 하는 사용자에게 매우 적합한 에디터이다. 하지만 GUI 환경에 익숙한 사용자에게는 추천하지 않는다. 

Windows 환경에서 WSL을 통해 프로그래밍을 하려는 사용자는 vscode를 사용하는 것을 추천하며, vm환경에서 vim을 사용하고 싶은 사용자는 아래 명령어를 통해 바로 설치할 수 있다. 

```zsh
sudo apt install vim
```



## vscode 설치 및 설정

vscode는 마이크로소프트에서 제작한 code editor이자 개발도구로 아래의 사이트에서 자세한 사항을 확인할 수 있으며, 다운로드 받을 수도 있다. vm 사용자는 virtual machine에 설치되어 있는 운영체제에서 vscode를 설치하면 되고, wsl 사용자는 windows 환경에서 vscode를 설치하면 된다.

* https://code.visualstudio.com/

* vscode 설치가 완료되면 c/c++ 개발을 위한 설정을 수행한다.

  * plugin 설치 - 다음 2개의 plugin을 설치한다.

    * Remote-WSL (WSL 사용자의 경우에만 설치)	
    * c/c++

    ![1566889467495](.README.assets/1566889467495.png)

  * plugin 설치가 완료되면 vscode를 재실행 합니다.



### 프로젝트 생성

* project를 수행할 폴더를 생성한다.

* vscode를 수행한 후 해당 폴더를 연다. 

  ![image-20201221104226685](.README.assets/image-20201221104226685.png)

* WSL 사용자의 경우 `해당 폴더를 wsl로 열기` 를 수행한다.

  ![image-20201221110721733](.README.assets/image-20201221110721733.png)

* main.c 소스코드를 생성한 후 아래의 소스코드를 입력한다.

  ```c++
  #include <stdio.h>
  
  int main(){
      printf("Hello world\n");
      
      return 0;
  }
  ```

  ![image-20201221105217447](.README.assets/image-20201221105217447.png)

* 해당 폴더에서 터미널을 실행한다.

  ![image-20201221110742393](.README.assets/image-20201221110742393.png)

* 아래의 명령으로 컴파일을 실행한 후 실행파일이 생성되는 것을 확인한다.

  * clang 대신에 gcc를 사용할 수 있다.

  ```zsh
  clang main.c -o main
  ```

* 생성한 파일을 실행한다.

  ```zsh
  ./main
  ```

  ![image-20201221110755602](.README.assets/image-20201221110755602.png)



# ref

* https://github.com/Microsoft/vscode-cpptools/releases

* https://code.visualstudio.com/docs/cpp/config-wsl

* https://codeday.me/ko/qa/20190627/905798.html

* <https://m.blog.naver.com/PostView.nhn?blogId=stupidus85&logNo=70133129470&proxyReferer=https%3A%2F%2Fwww.google.com%2F>

* <https://pkgpl.org/2014/07/27/sconstruct_basic/>

* <https://clee.tistory.com/88>

* <https://wiki.iotivity.org/build_iotivity_with_ubuntu_build_machine>

* <http://egloos.zum.com/bruceKIM/v/3399880>

* <http://egloos.zum.com/bruceKIM/v/3399880>

* <https://hongku.tistory.com/70>

* <https://scons.org/doc/0.97/HTML/scons-user/x628.html>

