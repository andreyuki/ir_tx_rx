# Componente IR TX RX

## Informações gerais

### RMT
O RMT é um periférico do ESP32. É um módulo que pode ser usado para receber e enviar sinal de controle de infra vermelho.


### Esquemático

Para a recepçãod o infravermelho são utilizados: um led sensor de infravermelho (TSOP1838D), um capacitor  de 10uF e um de 100nF, um resistor de 100 Ohm e um de 10k Ohm. 

Para o envio do infravermelho são utilizados: 3 led emissor de infravermelho (TSAL7300), um transistor NPN (BC337-25), um resistor de 22 Ohm e um de 68 Ohm

![Esquemático](./../../../../esquematico.png)

Os capacitores e resistores junto as sensor são utilizados para diminiur o ruído e o transistor junto aos emissores serve para aumentar a corrente nos emissores.

Para o sensor está sendo utilizado a porta 21 e para o emissor a porta 15.

## Como usar
É necessário inicializar os RMTs

### Recibo de comandos

### Envio de comandos

