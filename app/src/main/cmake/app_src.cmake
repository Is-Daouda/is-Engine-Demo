# game source file
set(
    app_src
	
	# game system extended
	${ISENGINE_SRC_DIR}/GameSystemExtended.cpp

	# game scene
	${ISENGINE_SRC_DIR}/GameIntro.cpp
	${ISENGINE_SRC_DIR}/GameLevel.cpp
	${ISENGINE_SRC_DIR}/GameLevelLoadResource.cpp
	${ISENGINE_SRC_DIR}/GameMenu.cpp
	${ISENGINE_SRC_DIR}/GameMenuLoadResource.cpp
	${ISENGINE_SRC_DIR}/GameOver.cpp

	# game level objects
	${ISENGINE_SRC_DIR}/Block.cpp
	${ISENGINE_SRC_DIR}/Bonus.cpp
	${ISENGINE_SRC_DIR}/Effect.cpp
	${ISENGINE_SRC_DIR}/Enemy.cpp
	${ISENGINE_SRC_DIR}/FinishObject.cpp
	${ISENGINE_SRC_DIR}/FireBall.cpp
	${ISENGINE_SRC_DIR}/GameController.cpp
	${ISENGINE_SRC_DIR}/HUD.cpp
	${ISENGINE_SRC_DIR}/Player.cpp
	${ISENGINE_SRC_DIR}/SpecialObject.cpp
	
	# game intro objects
	${ISENGINE_SRC_DIR}/GameIntroController.cpp
	
	# game menu objects
	${ISENGINE_SRC_DIR}/MenuController.cpp
	
	# game over objects
	${ISENGINE_SRC_DIR}/GameOverController.cpp

	# widgets
	${ISENGINE_SRC_DIR}/CancelButton.cpp
	${ISENGINE_SRC_DIR}/GameDialog.cpp
	${ISENGINE_SRC_DIR}/PauseOption.cpp
)