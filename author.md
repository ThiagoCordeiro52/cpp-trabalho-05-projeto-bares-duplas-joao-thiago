# Identificação Pessoal
 
- Nome: João Guilherme Lopes Alves da Costa - Thiago de Oliveira Cordeiro
- Email: joguicosta@hotmail.com - thiagoty52@gmail.com
- Turma: DIM0120 - T01

# TODO dos autores

- [X] Implementar os parênteses
- [X] Tratar erros
- [X] Modelar uma classe (BaresManager?)
- [ ] Documentação
- [ ] Implementar itens extras (pilha, filha, vector...)
- [X] Colocar exemplo de teste do pdf no data/

# Indique quais itens da avaliação foram realizados

- [X] Lê expressões do `std::cin` e cria corretamente a lista de tokens (20 pontos)
- [X] Converte corretamente expressões do formato infixo para posfixo (15 pontos)
- [X] Trata corretamente o uso de parênteses e `-` unário (5 pontos)
- [X] Avalia corretamente expressão no formato posfixo (15 créditos)
- [X] Detecta corretamente o conjunto de erros solicitados (20 créditos)
- [X] Gera a saída conforma solicitado (15 créditos)
- [X] Código é organizado em classes (10 créditos)

- [ ] **Extra**: utilizou pilha feito pelos autores
- [ ] **Extra**: utilizou fila feito pelos autores
- [ ] **Extra**: utilizou vector feito pelos autores

# Compiling and Runnig

<!-- ## Manualmente com g++

Para compilar e executar o programa manualmente basta digitar os seguintes comandos em seu terminal:

```
$ git clone https://github.com/selan-active-classes/trabalho-05-projeto-bares-duplas-joao-thiago.git (faz o clone do repositório em algum diretório do seu pc)
$ cd trabalho-05-projeto-bares-individual-joaoguilac/ (vai até a pasta do repositório clonado)
$ mkdir bin (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ cd bin/ (vá para a pasta dos executáveis criada para compilar e executar seu programa)
$ g++ -Wall -std=c++11 -g ../source/src/bcr.cpp ../source/src/animation_mgr.cpp ../source/src/bar_chart.cpp -I../source/include -o bcr (compilar)
$ ./bares (executar)
``` -->

## Cmake

Para compilar e executar o programa com o Cmake você precisa ter instalado pelo menos a versão 3.5. Em seguida, abra seu terminal e execute os seguintes comandos:
```
$ git clone https://github.com/selan-active-classes/trabalho-05-projeto-bares-duplas-joao-thiago.git (faz o clone do repositório em algum diretório do seu pc)
$ cd trabalho-05-projeto-bares-individual-joaoguilac/ (vai até a pasta do repositório clonado)
$ mkdir bin (caso não tenha uma pasta para os executáveis, você deve criá-la com esse comando)
$ cd bin/ (vá para a pasta dos executáveis criada para compilar e executar seu programa)
$ cmake ../EBNF_basic/source/ (criar o cmake do projeto)
$ cmake --build . (compilar)
$ ./bares (executar)
$ Digite a expressão a ser calculada
```

--------
&copy; DIMAp/UFRN 2021.
