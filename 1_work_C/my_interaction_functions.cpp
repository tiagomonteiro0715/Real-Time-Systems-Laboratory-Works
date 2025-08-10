#include "my_interaction_functions.h"
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <interface.h>
#include <timers.h>
#include <interrupts.h>
#include <conio.h>
#include <time.h>

time_t tempo;

int getBitValue(uInt8 value, uInt8 bit_n)
// given a byte value, returns the value of its bit n
{
    return (value & (1 << bit_n));
}

void setBitValue(uInt8* variable, int n_bit, int new_value_bit) {
    uInt8 mask_on = (uInt8)(1 << n_bit);
    uInt8 mask_off = ~mask_on;
    if (new_value_bit) *variable |= mask_on;
    else *variable &= mask_off;
}

void moveXLeft() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 6, 1); // set bit 6 to high level
    setBitValue(&p, 7, 0); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveXRight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 6, 0); // set bit 6 to low level
    setBitValue(&p, 7, 1); // set bit 7 to high level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();

}

void stopX() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 6, 0); // set bit 6 to low level
    setBitValue(&p, 7, 0); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();

}

int getXPos() {
    uInt8 p = readDigitalU8(0);
    if (!getBitValue(p, 2))
        return 1;
    if (!getBitValue(p, 1))
        return 2;
    if (!getBitValue(p, 0))
        return 3;
    return (-1);
}

int getYPos() {
    uInt8 p = readDigitalU8(0);
    if (!getBitValue(p, 5))
        return 1;
    if (!getBitValue(p, 4))
        return 2;
    if (!getBitValue(p, 3))
        return 3;
    return (-1);

}

int getZPos() {
    uInt8 p0 = readDigitalU8(0);
    uInt8 p1 = readDigitalU8(1);
    if (!getBitValue(p1, 3))
        return 1;
    if (!getBitValue(p1, 1))
        return 2;
    if (!getBitValue(p0, 7))
        return 3;
    return (-1);
}

void gotoX(int x) {
    int posX = getXPos();
    // Se a posição atual for maior que o destino, move para a esquerda
    if (posX > x || posX == -1) {
        moveXLeft();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posX > x || posX == -1) {
            posX = getXPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }

    // Se a posição atual for menor que o destino, move para a direita
    if (posX < x) {
        moveXRight();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posX < x) {
            posX = getXPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }

    stopX();
}

void gotoY(int y) {
    int posY = getYPos();
    // Se a posição atual for maior que o destino, move para fora
    if (posY > y || posY == -1) {
        moveYOutside();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posY > y || posY == -1) {
            posY = getYPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }

    // Se a posição atual for menor que o destino, move para dentro
    if (posY < y || posY == -1) {
        moveYInside();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posY < y) {
            posY = getYPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }
    stopY();
}

void gotoZ(int z) {
    int posZ = getZPos();
    // Se a posição atual for maior que o destino, move para baixo
    if (posZ > z || posZ == -1) {
        moveZDown();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posZ > z || posZ == -1) {
            posZ = getZPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }

    // Se a posição atual for menor que o destino, move para cima
    if (posZ < z) {
        moveZUp();

        // Verifica a posição enquanto ainda não chegamos ao destino
        while (posZ < z) {
            posZ = getZPos();  // Atualiza a posição atual
            vTaskDelay(100 / portTICK_PERIOD_MS);  // Pequeno atraso para não sobrecarregar o processador
        }
    }

    stopZ();
}

void moveYInside() {

    uInt8 p = readDigitalU8(2); // read port 2

    setBitValue(&p, 5, 1); // set bit 6 to high level
    setBitValue(&p, 4, 0); // set bit 7 to low level

    writeDigitalU8(2, p); // update port 2
}

void moveYOutside() {

    uInt8 p = readDigitalU8(2); // read port 2

    setBitValue(&p, 5, 0); // set bit 6 to high level
    setBitValue(&p, 4, 1); // set bit 7 to low level

    writeDigitalU8(2, p); // update port 2

}

void stopY() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 5, 0); // set bit 6 to low level
    setBitValue(&p, 4, 0); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveZUp() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 3, 1); // set bit 6 to low level
    setBitValue(&p, 2, 0); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void moveZDown() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 3, 0); // set bit 6 to low level
    setBitValue(&p, 2, 1); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void stopZ() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 3, 0); // set bit 6 to low level
    setBitValue(&p, 2, 0); // set bit 7 to low level

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}


uInt8 getEmergencyP2() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2);
    taskEXIT_CRITICAL();

    return p;
}


void updateEmergencyP2(uInt8 p) {
    taskENTER_CRITICAL();
    writeDigitalU8(2, p);
    taskEXIT_CRITICAL();
}


//rever melhor qual o bit certo

void turnOnGreenUpLight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 0, 1);

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void turnOffGreenUpLight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 0, 0);

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}


void turnOnGreenDownLight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 1, 1);

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

void turnOffGreenDownLight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 1, 0);

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}


int Platecount() {

    int tempo1 = time(&tempo);

    int switch1, switch1_prev;
    int contador = 0;
    uInt8 p1 = readDigitalU8(1);
    switch1 = getBitValue(p1, 5);
    switch1_prev = switch1;
    printf("Pode comecar a clicar\n");

    while (difftime(time(&tempo), tempo1) < 5) {
        p1 = readDigitalU8(1);
        switch1 = getBitValue(p1, 5);
        if ((switch1 != 0) && (switch1 != switch1_prev)) {
            contador++;
        }
        switch1_prev = switch1;
    }
    return contador;
}



void SemiAutoCalibration() {
    int tecla = 0;
    int posZ = 0, posX = 0;
    int PosZAtual = 0, PosXAtual = 0;

    system("cls");
    printf("->Presine tecla *W* para ir para cima\n");
    printf("->Presine tecla *S* para ir para baixo\n");
    printf("->Presine tecla *D* para ir para a direita\n");
    printf("->Presine tecla *A* para ir para a esquerda\n");
    printf("->Pressine tecla *ESC* para sair\n");
    while (tecla != 27) {
        tecla = _getch();
        switch (tecla) {

        case 'w':
            PosZAtual = getZPos();
            posZ = getZPos();
            moveZUp();
            while (posZ == -1 || posZ == PosZAtual) {
                posZ = getZPos();
            }
            stopZ();
            printf("->ZCALIBRADO<-\n");
            break;
        case 's':
            PosZAtual = getZPos();
            posZ = getZPos();
            moveZDown();
            while (posZ == -1 || posZ == PosZAtual) {
                posZ = getZPos();
            }
            stopZ();
            printf("->ZCALIBRADO<-\n");
            break;
        case 'd':
            posX = getXPos();
            PosXAtual = getXPos();
            moveXRight();
            while (posX == -1 || posZ == PosXAtual) {
                posX = getXPos();
            }
            stopX();
            printf("->XCALIBRADO<-\n");
            break;
        case 'a':
            posX = getXPos();
            PosXAtual = getZPos();
            moveXLeft();
            while (posX == -1 || posX == PosXAtual) {
                posX = getXPos();
            }
            stopX();
            printf("->XCALIBRADO<-\n");
            break;
        }
    }
}

/*
void turnOffDownFlashingLight() {
    taskENTER_CRITICAL();
    uInt8 p = readDigitalU8(2); // read port 2
    taskEXIT_CRITICAL();

    setBitValue(&p, 0, 0);

    taskENTER_CRITICAL();
    writeDigitalU8(2, p); // update port 2
    taskEXIT_CRITICAL();
}

*/