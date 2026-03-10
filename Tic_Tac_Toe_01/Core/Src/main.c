/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <retarget_stdio.h>
#include "i2c_lcd.h"

#include "ui.h"
#include "game.h"
#include "ai.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void lcd_show_1line(uint8_t line, char* text)
{
    if (line > 1) return;   // nur 0 oder 1 erlaubt

    lcd_cur_pos(0, line);
    lcd_put_str("        "); // 8 Leerzeichen = Zeile löschen

    lcd_cur_pos(0, line);
    lcd_put_str(text);
}

void lcd_show_2lines(char* line1, char* line2)
{
    lcd_clr();

    lcd_cur_pos(0,0);
    lcd_put_str(line1);

    lcd_cur_pos(0,1);
    lcd_put_str(line2);
}

char uart_getchar(void)
{
    char c;
    HAL_UART_Receive(&huart2, (uint8_t*)&c, 1, HAL_MAX_DELAY);
    return c;
}

void lcd_show_player(char player)
{
    char line1[9];
    char line2[9];

    snprintf(line1, 9, "Start:%c", player);
    snprintf(line2, 9, "Turn: %c", player);

    lcd_show_2lines(line1, line2);
}



void greet() {

	lcd_show_2lines("Welcome ", "to...  ");
	HAL_Delay(2000);

	lcd_show_2lines(" TicTac ", "  Toe! ");
	HAL_Delay(2000);


	lcd_clr();
}


int askStartGame() {

    char choice;
    char line2[16] = "(j/n): ";

    lcd_clr();
    lcd_cur_pos(0,0);
    lcd_put_str("Start?");

    lcd_cur_pos(0,1);
    lcd_put_str(line2);

    // Warten auf Taste
    choice = uart_getchar();

    // Auf LCD anzeigen
    int len = strlen(line2);

    lcd_cur_pos(len, 1);
    lcd_putc(choice);

    HAL_Delay(500);

    if (choice == 'j' || choice == 'J')
        return 1;
    else
        return 0;
}


int askReplay() {

    char choice;

    printf("\nMoechtest du noch einmal spielen? (j/n): ");
	lcd_show_2lines("Again?", "");

    scanf(" %c", &choice);

    if (choice == 'j' || choice == 'J')
        return 1;

    return 0;
}


void startGame(Game *game) {

    int input;
    int move;

    lcd_show_2lines("Game ", "starts!   ");
    HAL_Delay(2000);
    initGame(game);

    /* Zufälliger Startspieler */
    if (rand() % 2 == 0)
        game->currentPlayer = 'X';
    else
        game->currentPlayer = 'O';


    printf("\nStartspieler: %c\r\n", game->currentPlayer);
    printf("%c ist am Zug!\r\n\r\n", game->currentPlayer);

    lcd_show_player(game->currentPlayer);
    HAL_Delay(1500);


    while (1) {

        printBoard(game);

        /* Mensch */
        if (game->currentPlayer == 'X') {

            printf("Dein Zug (1-9): ");
        	lcd_show_2lines("Ur turn:", "(1-9): ");
        	 // STATT scanf: Wir lesen ein Zeichen und prüfen es sofort
        	 char c = uart_getchar();
        	 printf("%c\r\n", c); // Zeige gedrückte Taste im Terminal

        	 // Prüfen, ob es eine Zahl zwischen 1 und 9 ist
        	if (c >= '1' && c <= '9') {
        	   input = c - '0'; // ASCII zu Integer umwandeln
        	   if (!playerMove(game, input)) {
        	          printf("Feld belegt!\r\n");
        	          lcd_show_2lines("Field", "Taken! ");
        	          HAL_Delay(1000);
        	          continue;
        	   }
        	   } else {
        	                 // Hier landen wir bei Buchstaben/Sonderzeichen -> KEINE Endlosschleife mehr!
        	      printf("Ungueltige Taste!\r\n");
        	      lcd_show_2lines("Only", "1-9! ");
        	      HAL_Delay(1000);
        	      continue;
        	   }

        }

        /* KI */
        else {

            printf("KI denkt...\r\n");
            lcd_show_2lines("AI is", "thinking...   ");

        	HAL_Delay(2000);

            move = aiMove(game);

            printf("KI waehlt: %d\r\n", move);
            lcd_show_2lines("AI is", "choosing...   ");


            playerMove(game, move);
        }


        /* Sieg */
        if (checkWin(game, game->currentPlayer)) {

            printBoard(game);


            if (game->currentPlayer == 'X') {
                printf("Du hast gewonnen!\r\n");
            	lcd_show_2lines("You", "win!");
            }
            else {
                printf("Gegner gewinnt!\r\n");
            	lcd_show_2lines("AI", "wins!");

            }

            break;
        }


        /* Unentschieden */
        if (isDraw(game)) {

            printBoard(game);


            printf("Unentschieden!\r\n");
        	lcd_show_2lines("Draw!", " ");



            break;
        }


        switchPlayer(game);
    }
}


void printHelp() {

    printf("\nPositionen:\r\n");

    printf(" 1 | 2 | 3 \r\n");

    printf("---+---+---\r\n");

    printf(" 4 | 5 | 6 \r\n");

    printf("---+---+---\r\n");

    printf(" 7 | 8 | 9 \r\n\r\n");
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  RetargetInit(&huart2);   // <<< wichtig!
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  lcd_init(&hi2c1);
  greet();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Game game;

  srand(time(NULL));


  if (!askStartGame()) {
	  printf("Spiel wurde beendet.\r\n");
      return 0;
  }

  printHelp();


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	startGame(&game);

	if (!askReplay()) {
		printf("Programm wird beendet.\r\n");
		break;
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
