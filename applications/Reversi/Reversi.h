#pragma once

#include "MatrixOS.h"
#include "ui/UI.h"
#include "ui/UIInterfaces.h"
#include "applications/Application.h"
#include "applications/BrightnessControl/BrightnessControl.h"


class Reversi : public Application {
 public:
  enum GameState {
    Waiting,
    Moving,
    Intermission,
    WinnerUnveil,
    Ended
  };

  struct boardInfo {
    uint8_t player : 2; // 0 = empty, 1 = player 1, 2 = player 2
    uint8_t wasPlayer : 2; // 0 = empty, 1 = player 1, 2 = player 2
    uint8_t newlyPlaced : 1; // 0 = no, 1 = yes
  };

  Timer renderTimer;

  boardInfo board[8][8];

  uint8_t currentPlayer;
  GameState gameState;

  uint32_t lastEventTime;
  uint32_t lastSubEventTime;

  Point placedPos;

  // Used in Done state
  uint8_t winner;

  static Application_Info info;
  void Setup() override;
  void Loop() override;
  void KeyEventHandler(uint16_t keyID, KeyInfo* keyInfo);
  void Place(Point pos);

  void Render();

  void Settings();

  bool IsValidMove(uint8_t player, Point pos);
  bool ResetGame(bool confirmed);
  uint8_t CheckGameOver(); // 0 = not over, 1 = player 1 win, 2 = player 2 win, 3 = draw

  Color GetPlayerColor(uint8_t player);

  CreateSavedVar("Reversi", player1Color, Color, Color(0xFF00FF));
  CreateSavedVar("Reversi", player2Color, Color, Color(0x00FFFF));
  CreateSavedVar("Reversi", firstPlayer, uint8_t, 1);
};

inline Application_Info Reversi::info = {
    .name = "Reversi",
    .author = "203 Systems",
    .color = Color(0x008F00),
    .version = 2,
    .visibility = true,
};

REGISTER_APPLICATION(Reversi);