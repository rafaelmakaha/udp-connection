# Fundamentos de Redes de Computadores

## Trabalho 1

## Professor Tiago Alves

## Camada de Aplicação: Cliente DNS

### Observações

Gostaria de dizer que este trabalho me ajudou bastante a entender o funcionamento do protocolo UDP. E, também, me ajudou bastante a relembrar o quão trabalhoso é escovar bits em aplicações.
Posso, agora, apreciar bem mais o nível de evolução das tecnologias em que chegamos nos dias atuais.

Estou bem orgulhoso com o resultado deste projeto. Houve grande empenho e esforço. Gostaria de agradecer a dois amigos, mas os mesmo pediram para não serem referenciados. Talvez em um outro momento...

### 1. Introdução

A disciplina de Fundamentos de Redes de Computadores trata de diversos tópicos relativos a
Sistemas Operacionais, sistemas que provêem uma forma intuitiva de se utilizar as funcionalidades
de computadores digitais sem que seja necessário ao usuário ou programador ter profundo
conhecimento das interações entre os diferentes hardwares que compõem um computador.
Para construir ou adicionar funcionalidades a esses sistemas computacionais, é necessário
conhecimento de linguagens de programação e ferramentas de desenvolvimento.

### 2. Objetivos

 - [X] Exercitar conceitos de programação de sistemas operacionais.
 - [X] Estudar a implementação de esquemas de comunicações entre processos (IPC), especificamente a utilização da interface socket.

### 3. Referencial Teórico e Sites

- Mitchell, Mark, Jeffrey Oldham, and Alex Samuel. Advanced linux programming. New Riders, 2001.
- https://gist.github.com/fffaraz/9d9170b57791c28ccda9255b48315168
- https://linux.die.net/man/
- http://www.cis.syr.edu/~wedu/seed/Labs_16.04/Networking/DNS_Remote/udp.c

### 4. Material Utilizado

- Computador com sistema operacional programável
-  Ferramentas de desenvolvimento GNU/Linux ou similares: compilador GCC, depurador, editor de texto.

### 5. Roteiro

1) Revisão de técnicas e ferramentas de desenvolvimento de aplicações para o sistema operacional Linux. Colete o material acompanhante do roteiro do trabalho a partir do Moodle da disciplina e estude os
princípios e técnicas de desenvolvimento de aplicações para o sistema operacional Linux.
2) Realizar as implementações solicitadas no questionário do trabalho

### 6. Implementações e Questões para Estudo

1. Escreva um cliente para o protocolo DNS.
- O cliente receberá entradas do teclado e tentará realizar o envio de uma requisição DNS
(RFC1034 e RFC1035).
    - O primeiro argumento da linha de comando deverá informar o endereço o nome cuja
resolução se procura. O segundo argumento informará o IP do servidor DNS que será consultado.
• O servidor funcionará na porta padrão para o serviço DNS: UDP 53
• Firewalls podem impedir com que pacotes com as consultas alcancem servidores localizados fora da rede de acesso.
- Em relação ao formato do payload UDP da consulta, seguem orientações:
    - O cliente deverá realizar apenas consultas do tipo A (host address)
    - Transaction ID (16 bits): o cliente deverá gerar um número aleatório de 16 bits para identificar a transação/consulta.
    - Flags (16 bits): as flags de consulta deverão indicar uma consulta recursiva. Dessa forma, os 16 bits de deverão estar codificados em 0x0100.
    - Question (16 bits): deverá indicar apenas 1 consulta e, desta forma, deverá estar codificado em 0x0001.
    - Answer RRs, Authority RRs, Additional RRs (todos os três são campos de 16 bits): deverão estar codificados em 0x0000.
    - Query: a query deverá informar uma consulta Type A, Class IN para o Name informado como primeiro parâmetro da linha de comando.
- A implementação do cliente deverá utilizar o protocolo UDP como protocolo de
transporte.
    - Como o protocolo UDP é orientado a mensagem, o cliente deverá aguardar 2 segundos pela chegada da resposta. Caso a resposta não tenha chegado, uma nova tentativa deverá ser realizada.
    - O cliente tentará resolver o nome por até 3 vezes. Depois da terceira vez, uma mensagem de erro deverá ser informada ao usuário.
    - Caso o nome apresentado pelo usuário não exista, 
- O resultado da consulta DNS deverá ser informado na saída padrão (stdout do console) e deverá ter o seguinte formato:
    - nome <> IP.
- Para informar o resultado da consulta, será necessário interpretar a mensagem retornada pelo servidor e extrair a informação do IP de seu payload.
  
- Exemplos de interação:

    Resolução bem sucedida

        $ ./meu_cliente fga.unb.br 8.8.8.8
        fga.unb.br <> 164.41.98.10

    Resolução com falha: nome não existe

        $ ./meu_cliente azertyoiuoiu.unb.br 1.1.1.1
        Host azertyoiuoiu.unb.br não encontrado

    Resolução com falha: servidor não existe/não atendeu

        $ ./meu_cliente fga.unb.br 1.2.3.4
        Nao foi possível resolver fga.unb.br

- Restrições:
    - Para este trabalho, será admitida a utilização de bibliotecas auxiliares para para suporte à implementação. Quando se tratar de serviços de comunicação, apenas bibliotecas de socket UDP da sua linguagem preferida de programação poderão ser utilizadas.
    - Bibliotecas auxiliares serão permitidas

### 7. Desenvolvimento e Comentários

O código está devidamente comentado para fins didáticos e para revisões futuras a respeito do assunto abordado neste projeto.

