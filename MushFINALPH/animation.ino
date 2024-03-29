

// ------------------- Загрузка картинок и фреймов анимации -------------------
// Данный пример работает при размере матрицы 16x16 - такие картинки подготовлены в этом файле
// для других размеров матрицы - подготовьте собственные картинки


void loadImage(const uint16_t (*frame)) {
  int8_t offset_x = (WIDTH - FRAME_WIDTH) / 2;
  int8_t offset_y = (HEIGHT - FRAME_HEIGHT) / 2;
  for (byte i = 0; i < FRAME_WIDTH; i++) {
    if (offset_x + i < 0 || offset_x + i > WIDTH - 1) continue;
    for (byte j = 0; j < FRAME_HEIGHT; j++) {
      if (offset_y + j < 0  || offset_y + j > HEIGHT - 1) continue;
      drawPixelXY(offset_x + i, offset_y + j, gammaCorrection(expandColor((pgm_read_word(&(frame[(HEIGHT - j - 1) * WIDTH + i]))))));
    }
  }
}

void animation(byte n) {
  // "n" - индекс в наборе анимаций (подготовленных картинок)
  if (loadingFlag) {
    loadingFlag = false;

    frameNum = 0;
  }
  bool isReady = gifTimer.isReady();
  if (isReady) {
    switch (n) {
      case 1:
        // EFFECT_ANIMATION_1
        loadImage(mario_array[frameNum]);
        if (++frameNum >= sizeof(mario_array) / sizeof(mario_array[0])) frameNum = 0;
        break;
        
      case 2:
        // EFFECT_ANIMATION_2
        loadImage(flame_array[frameNum]);
        if (++frameNum >= sizeof(flame_array) / sizeof(flame_array[0])) frameNum = 0;
        break;
      case 3:
        // EFFECT_ANIMATION_3
        loadImage(ghost_array[frameNum]);
        if (++frameNum >= sizeof(ghost_array) / sizeof(ghost_array[0])) frameNum = 0;
        break;
           case 6:
        // EFFECT_ANIMATION_6
       loadImage(smile_array[frameNum]);
        if (++frameNum >= sizeof(smile_array) / sizeof(smile_array[0])) frameNum = 0;
        break;
           case 7:
        // EFFECT_ANIMATION_7
        loadImage(core_array[frameNum]);
        if (++frameNum >= sizeof(core_array) / sizeof(core_array[0])) frameNum = 0;
        break;
           case 8:
        // EFFECT_ANIMATION_8
        loadImage(cogu_array[frameNum]);
        if (++frameNum >= sizeof(cogu_array) / sizeof(cogu_array[0])) frameNum = 0;
        break;
            case 9:
        // EFFECT_ANIMATION_8
        loadImage(palitoallArray[frameNum]);
        if (++frameNum >= sizeof(palitoallArray) / sizeof(palitoallArray[0])) frameNum = 0;
        break;



        
    }
  }
}
