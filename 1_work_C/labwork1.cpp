// labwork1.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include<conio.h>
#include<stdlib.h>
#include <windows.h> //for Sleep function
#include "my_interaction_functions.h"

extern "C" {
#include "my_interaction_functions.h"
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <interface.h>
#include <timers.h>
#include <interrupts.h>
#include <time.h>
}


#define mainREGION_1_SIZE 8201
#define mainREGION_2_SIZE 29905
#define mainREGION_3_SIZE 7607
#define maxCarsHistoric 10000
#define priceSlot 1
#define MAX_CURRENT_CARS 9  // Define a maximum number of current cars
#define bool int      // Defining bool as int (0 for false, non-zero for true)
#define true 1        // Defining true as 1
#define false 0       // Defining false as 0

bool isEmergency = false;  // This flag tracks whether both switches have been clicked

xSemaphoreHandle xSemaphore = NULL;
xSemaphoreHandle sem_X_fim;
xSemaphoreHandle sem_Z_fim;

xQueueHandle mbx_X;
xQueueHandle mbx_Z;
xQueueHandle mbx_light;


//************************************************
xSemaphoreHandle xSemaphoreMenu;
xSemaphoreHandle xSemaphoreAutoCalibration;
xSemaphoreHandle xSemaphoretaskLightUpFlash;
xSemaphoreHandle xSemaphoretaskLightDownFlash;
xSemaphoreHandle xSemaphoreEmergencyFlash;
//*************************************************
xQueueHandle xQueueSavePort2;
xQueueHandle xQueueSavePlate;

xTaskHandle taskHandle0;
//xTaskHandle taskHandle1;
xTaskHandle taskHandle2;
xTaskHandle taskHandle3;
xTaskHandle emergencyTask;
xTaskHandle resumeTask;
xTaskHandle taskSender;
xTaskHandle taskHandleLightUpFlash;
xTaskHandle taskHandleLightDownFlash;
xTaskHandle taskHandleRestart;
xTaskHandle taskHandleEmergencyLights;


typedef struct {
    int plateNumber;        // Stores car's plate number, assuming a maximum length of 10
    ULONGLONG entryTime;          // Stores the timestamp of entry (using time_t for simplicity)
    ULONGLONG exitTime;           // Stores the timestamp of exit (useful for calculating duration)
    int duration;           // Stores the total duration the car stayed (in minutes)
    int cost;               // Stores the calculated cost for the parking duration
    int coordX;             // Stores the X-coordinate of the car's location
    int coordZ;             // Stores the Z-coordinate of the car's location
    bool isFinished;       // Boolean indicating if the car has finished parking (true = finished, false = still parking)
} CarInfo;


typedef struct {
    CarInfo finishedCars[maxCarsHistoric];    // Array to store historical data of finished parking services
    CarInfo currentCars[MAX_CURRENT_CARS];    // Array to store current parked cars
    int totalEarnings;                        // Stores total earnings from all finished parking services
    int carCount;                             // Number of cars in the history
    int currentCarCount;                      // Number of cars currently parked
} ParkingSystem;


ParkingSystem parkingSystem;


// Function to initialize a CarInfo object
void initializeCarInfo(CarInfo* car) {
    car->plateNumber = 0;
    car->entryTime = GetTickCount64();
    car->exitTime = 0;
    car->duration = 0;
    car->cost = 0;
    car->coordX = 0;
    car->coordZ = 0;
    car->isFinished = false;
}


// Function to initialize a ParkingSystem object
void initializeParkingSystem(ParkingSystem* parkingSystem) {
    for (int i = 0; i < maxCarsHistoric; i++) {
        initializeCarInfo(&(parkingSystem->finishedCars[i]));  // Initialize each CarInfo in the array
    }
    parkingSystem->totalEarnings = 0;
}


void restartParkingSystem(ParkingSystem* parkingSystem) {
    // Reinitialize all the CarInfo structures in the finishedCars array
    for (int i = 0; i < maxCarsHistoric; i++) {
        initializeCarInfo(&(parkingSystem->finishedCars[i]));
    }
    // Reset total earnings back to zero
    parkingSystem->totalEarnings = 0;
}


int calculateDurationCarInfo(CarInfo* car) {
    car->duration = (int)(car->exitTime - car->entryTime) * 60000;  // Calculo do custo por minuto(60s * 1000) 
    return car->duration;
}


int giveCarCount(ParkingSystem* parkingSystem) {
    return parkingSystem->carCount;
}

int calculateCostCarInfo(CarInfo* car) {
    car->cost = car->duration * priceSlot;  // Calculate cost based on duration
    return car->cost;
}


void updateTotalEarningsParkingSystem(ParkingSystem* parkingSystem, CarInfo* car) {
    // Add the cost of the specific car to the total earnings
    parkingSystem->totalEarnings += car->cost;
}


void finishParking(ParkingSystem* parkingSystem, CarInfo* car) {
    // Calculate the cost of this car's parking session
    calculateCostCarInfo(car);

    // Update the total earnings of the parking system
    updateTotalEarningsParkingSystem(parkingSystem, car);

    // You can also store the car in finishedCars array if necessary
    // Example (add it to history):
    // parkingSystem->finishedCars[index] = *car;
}


void setPlateNumber(CarInfo* car, int matricula) {
    car->plateNumber = matricula;
    printf("Set car's plate number to %d.\n", matricula);
}


void addCarToParkingSystem(ParkingSystem* parkingSystem, CarInfo* car) {

    if (parkingSystem->carCount < 10) {
        parkingSystem->finishedCars[parkingSystem->carCount] = *car;
        //printf("%d", parkingSystem->carCount);
        parkingSystem->carCount++;
        printf("Car with plate %d added to the parking system.\n", car->plateNumber);
    }
    else {
        printf("Parking system is full. Cannot add more cars.\n");
    }
}


void fullParkingSystems(ParkingSystem* parkingSystem) {
    if ((parkingSystem->currentCarCount) == 9) {
        printf("parking system is full");
    }
}


void showParkedCars(ParkingSystem* parkingSystem, int currentTime) {
    printf("List of currently parked cars:\n");
    printf("-------------------------------------------------------------\n");
    printf("| Plate Number | Cell (X, Z) | Entry Time | Current Cost (Euros) |\n");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < 10; i++) {
        CarInfo* car = &(parkingSystem->finishedCars[i]);

        // Only show cars that are still parked (not finished)
        if (!car->isFinished) {
            int timeParked = currentTime - car->entryTime;  // Time parked so far
            int currentCost = timeParked * priceSlot;       // Calculate current cost

            printf("| %11d | (%2d, %2d)    | %10d | %19d |\n",
                car->plateNumber,      // Plate number
                car->coordX,           // X-coordinate (cell)
                car->coordZ,           // Z-coordinate (cell)
                car->entryTime,        // Entry time (when the car parked)
                currentCost            // Current cost based on time parked
            );
        }
    }
    printf("-------------------------------------------------------------\n");
}


void setCarX(CarInfo* car, int xCoordinate) {
    car->coordX = xCoordinate;
    printf("Set car's X coordinate to %d.\n", xCoordinate);
}

void setCarZ(CarInfo* car, int zCoordinate) {
    car->coordZ = zCoordinate;
    printf("Set car's Z coordinate to %d.\n", zCoordinate);
}


void carIsParked(CarInfo* car) {
    car->isFinished = true;
}


int pedirCoordX(int x) {
    printf("|Insira a coordenada de x: ");
    scanf("%d", &x);

    return x;
}


int pedirCoordZ(int z) {
    printf("|Insira a coordenada de z: ");
    scanf("%d", &z);

    return z;
}


void gotoXZ(int x, int z) {

    xQueueSend(mbx_X, &x, portMAX_DELAY);
    xQueueSend(mbx_Z, &z, portMAX_DELAY);

    xQueueSemaphoreTake(sem_X_fim, portMAX_DELAY);
    xQueueSemaphoreTake(sem_Z_fim, portMAX_DELAY);

}

void putInCell(int x, int z) {

    uInt8 p1 = readDigitalU8(1);
    uInt8 p0 = readDigitalU8(0);

    gotoXZ(x, z);

    if (getZPos() == 1) {
        xSemaphoreGive(xSemaphoretaskLightUpFlash);

        moveZUp();
        while (getBitValue(p1, 2)) {
            p1 = readDigitalU8(1);
        }
        stopZ();
        gotoY(3);
        gotoZ(1);
        gotoY(2);

    }
    if (getZPos() == 2) {
        xSemaphoreGive(xSemaphoretaskLightUpFlash);

        moveZUp();
        while (getBitValue(p1, 0)) {
            p1 = readDigitalU8(1);
        }
        stopZ();
        gotoY(3);
        gotoZ(2);
        gotoY(2);

    }
    if (getZPos() == 3) {
        xSemaphoreGive(xSemaphoretaskLightUpFlash);

        moveZUp();
        while (getBitValue(p0, 6)) {
            p0 = readDigitalU8(0);
        }
        stopZ();
        gotoY(3);
        gotoZ(3);
        gotoY(2);

    }

    vTaskSuspend(taskHandleLightUpFlash);
    turnOffGreenUpLight();

}

void getFromCell(int x, int z) {

    uInt8 p1 = readDigitalU8(1);
    uInt8 p0 = readDigitalU8(0);

    gotoXZ(x, z);

    if (getZPos() == 1) {
        xSemaphoreGive(xSemaphoretaskLightDownFlash);

        gotoY(3);
        moveZUp();
        while (getBitValue(p1, 2)) {
            p1 = readDigitalU8(1);
        }
        stopZ();
        gotoY(2);
        gotoZ(1);
    }
    if (getZPos() == 2) {
        xSemaphoreGive(xSemaphoretaskLightDownFlash);

        gotoY(3);
        moveZUp();
        while (getBitValue(p1, 0)) {
            p1 = readDigitalU8(1);
        }
        stopZ();
        gotoY(2);
        gotoZ(2);
    }
    if (getZPos() == 3) {
        xSemaphoreGive(xSemaphoretaskLightDownFlash);

        gotoY(3);
        moveZUp();
        while (getBitValue(p0, 6)) {
            p0 = readDigitalU8(0);
        }
        stopZ();
        gotoY(2);
        gotoZ(3);
    }
    vTaskSuspend(taskHandleLightDownFlash);
    turnOffGreenDownLight();
}



void task_menu(void* pvParameters) {
    int x = 0, z = 0;
    int matricula = 0;
    CarInfo carBuffer;
    initializeCarInfo(&carBuffer);

    while (true) {

        xSemaphoreTake(xSemaphoreMenu, portMAX_DELAY);

        int tecla = 0;
        system("cls");
        printf("\n\tMenu");
        printf("\nc => Semi-calibracao do parking kit");
        printf("\nm => Controlo manual");
        printf("\nh => Mostrar historico\n");


        tecla = _getch();
        switch (tecla) {

        case 'c':
            SemiAutoCalibration();
            xSemaphoreGive(xSemaphoreMenu);
            break;


        case 'm':
            //Limpa o terminal
            system("cls");
            printf("*********************************");
            printf("\nEntrada em modo manual");
            printf("\nComandos:\n");
            printf("\ntecla d => vai para a direita");
            printf("\ntecla a => vai mover para esquerda");
            printf("\ntecla s => stop");
            printf("\ntecla w => vai mover para cima");
            printf("\ntecla x => vai mover para baixo\n");
            printf("\ntecla p => Escolha a posicao para por o carro\n");
            printf("tecla g => Escolha a posicao para tirar o carro\n");
            printf("*********************************\n");


            while (tecla != 27) {
                tecla = _getch();
                if (tecla == 'd') {
                    printf("vai para a direita");
                    moveXRight();
                }
                if (tecla == 'a') {
                    printf("vai mover para esquerda");
                    moveXLeft();
                }
                if (tecla == 's') {
                    stopX();
                    stopY();
                    stopZ();
                    turnOffGreenUpLight();
                }
                if (tecla == 'w') {

                    printf("vai mover para cima");
                    turnOnGreenUpLight();
                    moveZUp();
                }
                if (tecla == 'x') {
                    printf("vai mover para baixo");
                    moveZDown();
                }
                if (tecla == 'p') {
                    x = pedirCoordX(x);
                    z = pedirCoordZ(z);
                    setCarX(&carBuffer, x);
                    setCarZ(&carBuffer, z);
                    printf("Escreva a matricula usando o switch1\n");
                    xQueueReceive(xQueueSavePlate, &matricula, portMAX_DELAY);
                    setPlateNumber(&carBuffer, matricula);
                    while (carBuffer.plateNumber == 0) {
                    }
                    Sleep(2000);
                    addCarToParkingSystem(&parkingSystem, &carBuffer);
                    putInCell(x, z);
                    carIsParked(&carBuffer);
                    showParkedCars(&parkingSystem, 0);
                    printf("\n");
                }

                if (tecla == 'g') {
                    x = pedirCoordX(x);
                    z = pedirCoordZ(z);
                    getFromCell(x, z);
                    printf("\n");
                }
            }
            xSemaphoreGive(xSemaphoreMenu);
            break;

        case 'h':
            break;

            //Ignorar Esc
        case 27:
            printf("\nSaida do programa");
            exit(1);
            break;
        default:
            xSemaphoreGive(xSemaphoreMenu);
            break;
        }
    }
}


void calibrationTask(void* pvParameters) {
    int x = 1, z = 1;  // Objetivo de calibração
    while (TRUE) {

        xSemaphoreTake(xSemaphoreAutoCalibration, portMAX_DELAY);
        printf("\nIniciando semi-calibracao para X = 1 e Z = 1\n");
        xQueueSend(mbx_X, &x, portMAX_DELAY);
        xQueueSend(mbx_Z, &z, portMAX_DELAY);

        xQueueSemaphoreTake(sem_X_fim, portMAX_DELAY);
        xQueueSemaphoreTake(sem_Z_fim, portMAX_DELAY);

    }
    // Liberar o semáforo do menu
    xSemaphoreGive(xSemaphoreMenu);
}

void task_gotoX(void* pvParameters) {
    int x;
    while (TRUE) {
        xQueueReceive(mbx_X, &x, portMAX_DELAY);
        gotoX(x);
        xSemaphoreGive(sem_X_fim);

    }
}

void task_gotoZ(void* pvParameters) {
    int z;
    while (TRUE) {
        xQueueReceive(mbx_Z, &z, portMAX_DELAY);
        gotoZ(z);
        xSemaphoreGive(sem_Z_fim);
    }
}


void inicializarPortos() {
    printf("\nwaiting for hardware simulator...");
    createDigitalInput(0);
    createDigitalInput(1);
    createDigitalOutput(2);
    writeDigitalU8(2, 0);
    printf("\ngot access to simulator...");
}


//**
// vTaskRestartSystem
//
// Purpose: Clears the history log, effectively restarting the system's data tracking.
// Problem Solved: Allows for a fresh start of the system by clearing old logs.
//**
void vTaskRestartSystem(void* pvParameters) {

    restartParkingSystem(&parkingSystem);
    printf("Sistema foi reiniciado.\n");
}


void taskLightUpFlash(void* pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(200); // 200 ms delay for flashing

    while (TRUE) {
        // Wait for the semaphore before starting the flashing loop
        xSemaphoreTake(xSemaphoretaskLightUpFlash, portMAX_DELAY);

        // Flash the light 5 times (for example)
        for (int i = 0; i < 5; i++) {
            turnOnGreenUpLight();   // This can be your UP light
            vTaskDelay(xDelay);   // Delay for 200ms with the light on
            turnOffGreenUpLight();
            vTaskDelay(xDelay);   // Delay for 200ms with the light off
        }

        // After flashing, give the semaphore back if needed
        xSemaphoreGive(xSemaphoretaskLightUpFlash);
    }
}


void taskLightDownFlash(void* pvParameters) {
    const TickType_t xDelay = pdMS_TO_TICKS(200); // 200 ms delay for flashing

    while (TRUE) {
        // Wait for the semaphore before starting the flashing loop
        xSemaphoreTake(xSemaphoretaskLightDownFlash, portMAX_DELAY);

        // Flash the light 5 times (for example)
        for (int i = 0; i < 5; i++) {
            turnOnGreenDownLight();      // This can be your UP light
            vTaskDelay(xDelay);         // Delay for 200ms with the light on
            turnOffGreenDownLight();
            vTaskDelay(xDelay);         // Delay for 200ms with the light off
        }

        // After flashing, give the semaphore back if needed
        xSemaphoreGive(xSemaphoretaskLightDownFlash);
    }
}


// Função para verificar o estado de ambos os botões
int areBothSwitchesPressed() {
    uInt8 port1Value = readDigitalU8(1);  // Lê o valor de todo o port 1

    // Verifica se os bits 5 e 6 estão ambos em nível baixo (pressionados)
    // Isto depende de como os switches estão ligados: nível baixo pode significar "pressionado"
    if (getBitValue(port1Value, 5) && getBitValue(port1Value, 6)) {
        return 1;  // Ambos pressionados
    }
    return 0;  // Não estão ambos pressionados
}


void switch1_rising_isr(ULONGLONG lastTime) {
    ULONGLONG time = GetTickCount64();
    int matricula = 0;

    printf("\nSwitch one RISING detected at time = %llu...\n", time);

    if (isEmergency) {
        // Se o sistema estiver em modo de emergência, retoma o sistema
        printf("\nResuming system from emergency mode...\n");
        BaseType_t xYieldRequired = xTaskResumeFromISR(resumeTask);
        isEmergency = false;  // Reiniciar o estado de emergência
    }
    else {
        // Verifica se ambos os switches estão pressionados
        if (areBothSwitchesPressed()) {
            printf("\nBoth switches pressed. Entering emergency mode...\n");
            isEmergency = true;  // Define a flag de emergência
            BaseType_t xYieldRequired = xTaskResumeFromISR(emergencyTask);  // Aciona a task de emergência
        }
        else {

            matricula = Platecount();
            xQueueSend(xQueueSavePlate, &matricula, portMAX_DELAY);
            printf("\n%d\n", matricula);
        }
    }
}


void switch2_rising_isr(ULONGLONG lastTime) {
    ULONGLONG time = GetTickCount64();
    uInt8 port1Value = readDigitalU8(1);
    printf("\nSwitch two RISING detected at time = %llu...\n", time);

    // Verifica se ambos os switches estão pressionados
    if (areBothSwitchesPressed()) {
        printf("\nBoth switches pressed. Entering emergency mode...\n");
        isEmergency = true;  // Define a flag de emergência
        BaseType_t xYieldRequired = xTaskResumeFromISR(emergencyTask);  // Aciona a task de emergência
    }
    else {
        BaseType_t xYieldRequired = xTaskResumeFromISR(taskHandleRestart);  // Aciona a task de emergência

        printf("\nRESET\n");
        vTaskSuspend(taskHandleEmergencyLights);
        xSemaphoreTake(xSemaphoreEmergencyFlash, portMAX_DELAY);
        turnOffGreenUpLight();
        turnOffGreenDownLight();
        SemiAutoCalibration();
        xSemaphoreGive(xSemaphoreMenu);

    }
}


// vTaskEmergency
//
// Purpose: Puts the system into emergency mode, stopping all movement and flashing an emergency light.
// Problem Solved: Provides a mechanism to quickly stop all operations in case of a critical fault.
//
void vTaskEmergency(void* pvParameters) {
    while (true) {
        vTaskSuspend(NULL);  // Wait until resumed by the ISR

        if (isEmergency) {
            printf("\nMODO DE EMERGENCIA ATIVADO\n");

            // Save current state, stop all movements, and suspend all tasks
            uInt8 p2 = getEmergencyP2();
            xQueueSend(xQueueSavePort2, &p2, portMAX_DELAY);
            stopX();
            stopY();
            stopZ();
            vTaskSuspend(taskHandle0);
            //vTaskSuspend(taskHandle1);
            vTaskSuspend(taskHandle2);
            vTaskSuspend(taskHandle3);

            // Ativar o semáforo para iniciar o piscar das luzes
            xSemaphoreGive(xSemaphoreEmergencyFlash);

        }
    }
}


//**
// vTaskResumeEmergency
//
// Purpose: Restores the system from emergency mode back to normal operation.
// Problem Solved: Safely brings the system back online after an emergency shutdown.
//**
void vTaskEmergencyResume(void* pvParameters) {
    while (true) {
        vTaskSuspend(NULL);  // Wait until resumed by the ISR

        if (!isEmergency) {
            printf("\nResuming system from emergency mode...\n");

            // Restore previous state and resume tasks
            uInt8 p2;
            xQueueReceive(xQueueSavePort2, &p2, portMAX_DELAY);
            updateEmergencyP2(p2);
            vTaskResume(taskHandle0);
            //vTaskResume(taskHandle1);
            vTaskResume(taskHandle2);
            vTaskResume(taskHandle3);

            // Sinalizar para parar de piscar as luzes
            isEmergency = false;
        }
    }
}


void vTaskEmergencyLights(void* pvParameters) {
    while (true) {
        // Aguardar o semáforo de emergência ser ativado
        xSemaphoreTake(xSemaphoreEmergencyFlash, portMAX_DELAY);

        // Enquanto estiver em modo de emergência, pisca as luzes
        while (isEmergency) {
            // Ligar ambas as luzes (verde e vermelha)
            turnOnGreenUpLight();
            turnOnGreenDownLight();

            vTaskDelay(50 / portTICK_PERIOD_MS);

            // Desligar ambas as luzes
            turnOffGreenUpLight();
            turnOffGreenDownLight();

            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
    }
}


void myDaemonTaskStartupHook(void) {

    inicializarPortos();


    attachInterrupt(1, 5, switch1_rising_isr, RISING);
    attachInterrupt(1, 6, switch2_rising_isr, RISING);

    //**********************************************************************************

    sem_X_fim = xSemaphoreCreateCounting(10, 0);
    sem_Z_fim = xSemaphoreCreateCounting(10, 0);
    mbx_X = xQueueCreate(10, sizeof(int));
    mbx_Z = xQueueCreate(10, sizeof(int));

    //**********************************************************************************
    //Criar mailbox que vai guardar valor de P2 em caso de emergencia
    xQueueSavePort2 = xQueueCreate(1, sizeof(uInt8));
    xQueueSavePlate = xQueueCreate(1, sizeof(int));

    xSemaphoreMenu = xSemaphoreCreateCounting(1, 1);
    xTaskCreate(task_menu, "vTask_Menu", 100, NULL, 0, &taskHandle0);

    xTaskCreate(vTaskEmergency, "vTaskEmergency ", 100, NULL, 0, &emergencyTask);
    xTaskCreate(vTaskEmergencyResume, "vTaskResume ", 100, NULL, 0, &resumeTask);
    xTaskCreate(vTaskRestartSystem, "vTaskRestartSystem", 100, NULL, 0, &taskHandleRestart);
    xTaskCreate(task_gotoX, "GOTO X Task", 100, NULL, 1, &taskHandle2);
    xTaskCreate(task_gotoZ, "GOTO Z Task", 100, NULL, 1, &taskHandle3);
    xSemaphoreAutoCalibration = xSemaphoreCreateCounting(1, 0);
    xTaskCreate(calibrationTask, "vTaskAutoCalibration", 100, NULL, 0, NULL);

    xSemaphoretaskLightUpFlash = xSemaphoreCreateCounting(1, 0);
    xTaskCreate(taskLightUpFlash, "vTaskLightUpFlash", 100, NULL, 0, &taskHandleLightUpFlash);

    xSemaphoretaskLightDownFlash = xSemaphoreCreateCounting(1, 0);
    xTaskCreate(taskLightDownFlash, "vTaskLightDownFlash", 100, NULL, 0, &taskHandleLightDownFlash);

    xSemaphoreEmergencyFlash = xSemaphoreCreateCounting(1, 0);
    xTaskCreate(vTaskEmergencyLights, "vTaskEmergencyLights", 100, NULL, 0, &taskHandleEmergencyLights);

}


void vAssertCalled(unsigned long ulLine,
    const char*
    const pcFileName) {
    static BaseType_t xPrinted = pdFALSE;
    volatile uint32_t ulSetToNonZeroInDebuggerToContinue = 0;
    /* Called if an assertion passed to configASSERT() fails. See
    http://www.freertos.org/a00110.html#configASSERT for more information. */
    /* Parameters are not used. */
    (void)ulLine;
    (void)pcFileName;
    printf("ASSERT! Line %ld, file %s, GetLastError() %ld\r\n", ulLine, pcFileName, GetLastError());
    taskENTER_CRITICAL();
    {
        /* Cause debugger break point if being debugged. */
        __debugbreak();
        /* You can step out of this function to debug the assertion by using
        the debugger to set ulSetToNonZeroInDebuggerToContinue to a non-zero
        value. */
        while (ulSetToNonZeroInDebuggerToContinue == 0) {
            __asm {
                NOP
            };
            __asm {
                NOP
            };
        }
    }
    taskEXIT_CRITICAL();
}


static void initialiseHeap(void) {
    static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
    /* Just to prevent 'condition is always true' warnings in configASSERT(). */
    volatile uint32_t ulAdditionalOffset = 19;
    const HeapRegion_t xHeapRegions[] = {
        /* Start address with dummy offsetsSize */
        {
          ucHeap + 1, mainREGION_1_SIZE
        },
        {
          ucHeap + 15 + mainREGION_1_SIZE,
          mainREGION_2_SIZE
        },
        {
          ucHeap + 19 + mainREGION_1_SIZE +
          mainREGION_2_SIZE,
          mainREGION_3_SIZE
        },
        {
          NULL,
          0
        }
    };
    configASSERT((ulAdditionalOffset +
        mainREGION_1_SIZE +
        mainREGION_2_SIZE +
        mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);
    /* Prevent compiler warnings when configASSERT() is not defined. */
    (void)ulAdditionalOffset;
    vPortDefineHeapRegions(xHeapRegions);
}


int main() {

    initializeParkingSystem(&parkingSystem);  // Initialize parking system before creating tasks
    initialiseHeap();
    vApplicationDaemonTaskStartupHook = &myDaemonTaskStartupHook;
    vTaskStartScheduler();

    Sleep(6000);
    closeChannels();
    return 0;
}