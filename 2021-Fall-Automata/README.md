# Turing tool on Linux 

## development environment

> GNU LINUX Realease: Ubuntu 18.04.5 LTS on Windows 10 x86_64，5.4.72-microsoft-standard-WSL2
>
> g++ (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

## compile way

1. ```bash
    cd turing-project/
    ```
2. cli
   ```bash
    make
   ```

3. env
   ```bash
    export PATH='pwd':$PATH
    ```
   
4. Example：

   ````bash
   ./turing -v ./programs/gcd.tm 1101 
   ````

5. clear
    ```bash
    make clean
    ```

