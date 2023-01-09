// работа с бегущим текстом

// **************** НАСТРОЙКИ ****************
#define MIRR_V 0          // отразить текст по вертикали (0 / 1)
#define MIRR_H 0          // отразить текст по горизонтали (0 / 1)


#define LET_WIDTH 5       // ширина буквы шрифта
#define LET_HEIGHT 7      // высота буквы шрифта
#define SPACE 0           // пробел


// --------------------- ДЛЯ РАЗРАБОТЧИКОВ ----------------------
//boolean offset = 1;

int offset = WIDTHX;


void fillString(String text, uint32_t color) {
  if (loadingFlag) {
    offset = WIDTHX;   // перемотка в правый край
    loadingFlag = false;    
  
    fullTextFlag = false;
  }
  
  if (scrollTimer.isReady()) {
    FastLED.clear();
    byte i = 0, j = 0;
    while (text[i] != '\0') {
      if ((byte)text[i] > 191) {    // работаем с русскими буквами!
        i++;
      } else {
        drawLetter(j, text[i], offset + j * (LET_WIDTH + SPACE), color);
        i++;
        j++;
      }
    }
    fullTextFlag = false;

    offset--;
    if (offset < -j * (LET_WIDTH + SPACE)) {    // строка убежала
      offset = WIDTHX + 3;
      fullTextFlag = true;
    }
            
    FastLED.show();
  }
}


uint16_t getPixelNumber2(int8_t x, int8_t y) {
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE) {               // если чётная строка
    return (THIS_Y * WIDTHX + THIS_X);
  } else {                                              // если нечётная строка
    return (THIS_Y * WIDTHX + WIDTHX - THIS_X - 1);
  }
}

void drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color) {
  int8_t start_pos = 0, finish_pos = LET_WIDTH;
  int8_t LH = LET_HEIGHT;
  if (LH > HEIGHT) LH = HEIGHT;
  int8_t offset_y = (HEIGHT - LH) / 2;     // по центру матрицы по высоте
  
  CRGB letterColor;
  if (color == 1) letterColor = CHSV(byte(offset * 10), 255, 255);
  else if (color == 2) letterColor = CHSV(byte(index * 30), 255, 255);
  else letterColor = color;

  if (offset < -LET_WIDTH || offset > WIDTHX) return;
  if (offset < 0) start_pos = -offset;
  if (offset > (WIDTHX - LET_WIDTH)) finish_pos = WIDTHX - offset;

  for (byte i = start_pos; i < finish_pos; i++) {
    int thisByte;
    if (MIRR_V) thisByte = getFont((byte)letter, LET_WIDTH - 1 - i);
    else thisByte = getFont((byte)letter, i);

    for (byte j = 0; j < LH; j++) {
      boolean thisBit;

      if (MIRR_H) thisBit = thisByte & (1 << j);
      else thisBit = thisByte & (1 << (LH - 1 - j));

      // рисуем столбец (i - горизонтальная позиция, j - вертикальная)
      if (thisBit) leds[getPixelNumber2(offset + i, offset_y + TEXT_HEIGHT + j)] = letterColor;
    }
  }
}

// ------------- СЛУЖЕБНЫЕ ФУНКЦИИ --------------

// интерпретатор кода символа в массиве fontHEX (для Arduino IDE 1.8.* и выше)
uint8_t getFont(uint8_t font, uint8_t row) {
  font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
  if (font <= 90) return pgm_read_byte(&(fontHEX[font][row]));     // для английских букв и символов
  else if (font >= 112 && font <= 159) {    // и пизд*ц для русских
    return pgm_read_byte(&(fontHEX[font - 17][row]));
  } else if (font >= 96 && font <= 111) {
    return pgm_read_byte(&(fontHEX[font + 47][row]));
  }
  return 0;
}
