# game source file
set(
    app_src
	
	# game system extended
	${ISENGINE_SRC_DIR}/app_src/gamesystem_ext/GameSystemExtended.cpp

	# game scene

	${ISENGINE_SRC_DIR}/app_src/scenes/GameIntro/GameIntro.cpp
	${ISENGINE_SRC_DIR}/app_src/scenes/GameLevel/GameLevel.cpp
	${ISENGINE_SRC_DIR}/app_src/scenes/GameLevel/GameLevelLoadResource.cpp
	${ISENGINE_SRC_DIR}/app_src/scenes/GameMenu/GameMenu.cpp
	${ISENGINE_SRC_DIR}/app_src/scenes/GameMenu/GameMenuLoadResource.cpp
	${ISENGINE_SRC_DIR}/app_src/scenes/GameOver/GameOver.cpp

	# game level objects
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/Block.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/Bonus.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/Effect.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/Enemy.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/FinishObject.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/FireBall.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/GameController.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/HUD.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/LevelTile.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/Player.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/gamelevel/SpecialObject.cpp

	# game intro objects
	${ISENGINE_SRC_DIR}/app_src/objects/gameintro/GameIntroController.cpp
	
	# game menu objects
	${ISENGINE_SRC_DIR}/app_src/objects/gamemenu/MenuController.cpp
	
	# game over objects
	${ISENGINE_SRC_DIR}/app_src/objects/gameover/GameOverController.cpp
	
	# widgets
	${ISENGINE_SRC_DIR}/app_src/objects/widgets/CancelButton.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/widgets/GameDialog.cpp
	${ISENGINE_SRC_DIR}/app_src/objects/widgets/PauseOption.cpp
)
