<div align="center">

# **GiroMed: Um Dispensador Automatizado de Medicamentos com Interface Web para o Cuidado de Idosos**

<br>

**Giovani Bellini**

1 Engenharia de Computação – Pontifícia Universidade Católica de Campinas (PUC-Campinas)<br>
Campinas – SP – Brasil

`giovanibelliniestudos@gmail.com`

</div>

<br>

> **Abstract.** *This paper presents GiroMed, an automated medicine dispenser designed to assist elderly individuals in managing their medication schedules. Built with an ESP32 microcontroller, stepper motors, and magnetic sensors, the system dispenses pills into 21 slots based on a pre-programmed calendar. Caregivers can manage schedules and receive email alerts through a local web interface. The solution aims to reduce medication errors and provide a reliable, automated assistance tool for home care.*

> **Resumo.** *Este artigo apresenta o GiroMed, um dispensador automático de medicamentos desenvolvido para auxiliar idosos no gerenciamento de seus horários de medicação. Construído com um microcontrolador ESP32, motores de passo e sensores magnéticos, o sistema dispensa comprimidos em 21 gavetas com base em um calendário pré-programado. Cuidadores podem gerenciar horários e receber alertas por e-mail através de uma interface web local. A solução visa reduzir erros de medicação e aumentar a segurança.*

<br>

## **1. Introdução**

O envelhecimento populacional traz desafios significativos para a saúde pública e familiar, especialmente no que tange à adesão a tratamentos medicamentosos. O esquecimento de horários ou a dosagem incorreta são problemas recorrentes na rotina do público alvo idoso, o que compromete a eficácia dos tratamentos.

&nbsp;&nbsp;&nbsp;&nbsp;Nesse contexto, a aplicação de sistemas embarcados oferece uma oportunidade viável para automação domiciliar. O projeto GiroMed surge como uma máquina inteligente capaz de armazenar remédios e dispensá-los para consumo no momento exato, notificando os responsáveis sobre as rotinas diárias.

## **2. Arquitetura do Sistema e Casos de Uso**

A concepção do projeto baseia-se na separação das responsabilidades de gerenciamento lógico e controle físico. Para guiar o desenvolvimento, foram estabelecidos casos de uso focados nas interações entre a máquina e seus dois usuários principais: o Cuidador e o Paciente.

### **2.1. Casos de Uso**

O escopo operacional do sistema atende às interações fundamentais ilustradas na Figura 1, detalhando o acesso à rede local, a retirada da medicação e o retorno dos alertas.

```mermaid
flowchart TD
    %% Definição de Cores Claras
    classDef actor fill:#fdf1e6,stroke:#f5a623,stroke-width:2px,color:#333
    classDef system fill:#f0f8ff,stroke:#87cefa,stroke-width:2px,color:#333
    classDef action fill:#e6ffe6,stroke:#90ee90,stroke-width:1px,color:#333

    %% Atores
    Cuidador(("fa:fa-user-md Cuidador")):::actor
    Paciente(("fa:fa-blind Paciente")):::actor
    
    %% Sistema Interno
    subgraph GiroMed [fa:fa-cogs Sistema Interno GiroMed]
        direction TB
        UC1("fa:fa-calendar-alt Cadastrar Horários e Gavetas"):::action
        UC2("fa:fa-at Configurar E-mail de Destino"):::action
        UC3("fa:fa-volume-up Dispensar Medicação e Tocar Alarme"):::action
        UC4("fa:fa-door-open Abrir Porta e Retirar Remédio"):::action
        UC5("fa:fa-sync Realinhar e Enviar E-mail"):::action
    end
    
    %% Relacionamentos
    Cuidador -->|Acessa Web local| UC1
    Cuidador -->|Acessa Web local| UC2
    UC5 -.->|Notificação SMTP fa:fa-envelope| Cuidador
    
    UC3 -.->|Aviso Sonoro/Visual| Paciente
    Paciente -->|Interação Física fa:fa-hand-paper| UC4
    UC4 -->|Gatilho Magnético| UC5
```

<div align="center">
  <span style="font-family: Helvetica; font-size: 10pt; font-weight: bold;">
  Figura 1. Diagrama de Casos de Uso ilustrando as interações dos atores com o sistema.
  </span>
</div>
<br>

### **2.2. Design de Alto Nível (HLD)**

A arquitetura de alto nível do software (Figura 2) foi estruturada para suportar as necessidades assíncronas do hardware e da rede. Os módulos lógicos isolam o controle eletromecânico das requisições web e da sincronização temporal externa, favorecendo a estabilidade da máquina.

```mermaid
flowchart TD
    %% Definição de Cores Claras
    classDef cloud fill:#f8f9fa,stroke:#ced4da,stroke-width:2px,color:#333
    classDef esp fill:#fff4e6,stroke:#ffc107,stroke-width:2px,color:#333
    classDef hardware fill:#e8f5e9,stroke:#66bb6a,stroke-width:2px,color:#333
    classDef user fill:#e3f2fd,stroke:#4fc3f7,stroke-width:2px,color:#333

    Usuario(("fa:fa-mobile-alt Usuário")):::user

    subgraph Nuvem [fa:fa-cloud Serviços Externos]
        direction TB
        NTP["fa:fa-clock Servidor NTP Público"]:::cloud
        SMTP["fa:fa-envelope Servidor SMTP do Google"]:::cloud
    end

    subgraph ESP32 [fa:fa-microchip Microcontrolador ESP32]
        direction TB
        Web["fa:fa-wifi Servidor Web e mDNS"]:::esp
        RTC["fa:fa-hourglass-half Relógio Interno e Gatilhos"]:::esp
        Core["fa:fa-brain Controle Lógico Central"]:::esp
        Mail["fa:fa-paper-plane Cliente ESP Mail"]:::esp
    end

    subgraph Hardware [fa:fa-plug Componentes Físicos]
        direction TB
        Motor["fa:fa-sync-alt Motor de Passo ULN2003"]:::hardware
        Sensores["fa:fa-magnet Sensores Magnéticos Hall"]:::hardware
        IO["fa:fa-desktop Display LCD I2C e Buzzer"]:::hardware
    end

    Usuario <-->|HTTP| Web
    Web -->|Dados Cadastrais| Core
    RTC <-->|Sincronismo| NTP
    RTC -->|Verificação de Minutos| Core
    Core -->|Disparo Assíncrono| Mail
    Mail -->|TLS/SSL| SMTP
    Core -->|Pulsos Lógicos| Motor
    Sensores -->|Leitura de Estado| Core
    Core -->|Status| IO
```

<div align="center">
  <span style="font-family: Helvetica; font-size: 10pt; font-weight: bold;">
  Figura 2. Diagrama de Arquitetura de Alto Nível (HLD) detalhando o fluxo de dados e controle.
  </span>
</div>
<br>

## **3. Desenvolvimento de Hardware**

O projeto em sua camada física é orquestrado pelo microcontrolador ESP32. Para ilustrar o roteamento dos sinais lógicos, a distribuição de alimentação nas trilhas de 5V e 3.3V, e o aterramento unificado, a Figura 3 detalha o diagrama esquemático.

<br>
<div align="center">
  <img src="Pasted image 20260823160713.png" alt="Diagrama Elétrico do GiroMed" width="600">
  <br>
  <br>
  <span style="font-family: Helvetica; font-size: 10pt; font-weight: bold;">
  Figura 3. Diagrama elétrico detalhando as conexões do microcontrolador, display I2C, motor de passo e sensores magnéticos.
  </span>
</div>
<br>

*(Anotação para revisão: As legendas das figuras devem ser centralizadas caso tenham menos de uma linha, ou justificadas e recuadas em 0,8 cm caso sejam mais longas, sempre utilizando a fonte Helvetica, tamanho 10 e em negrito[cite: 2].)*

O maquinário principal utiliza o motor de passo com 2048 passos por revolução, promovendo a movimentação cilíndrica do carrossel. Os sensores magnéticos atuam como chaves limitadoras de alta precisão, evitando desgastes mecânicos que ocorreriam caso fossem utilizadas chaves fim-de-curso físicas tradicionais.

## **4. Conclusões**

O desenvolvimento do GiroMed atesta que a integração entre componentes microcontrolados de baixo custo e interfaces web acessíveis gera soluções de alto impacto na saúde. A arquitetura de software dividida em módulos, o tratamento de eventos sensoriais da porta e os protocolos de notificação online resultam em um sistema robusto. Dessa forma, proporciona-se mais autonomia e qualidade de vida ao paciente idoso e tranquilidade ao cuidador responsável.

## **Referências**

Bellini, G. (2026) "Projeto GiroMed: Automação na Dispensação de Medicamentos", PUC Campinas, Trabalho de Conclusão de Curso.

Espressif Systems (2026) "ESP32 Technical Reference Manual", Disponível em plataformas oficiais.