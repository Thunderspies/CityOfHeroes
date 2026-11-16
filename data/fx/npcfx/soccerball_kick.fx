#############################################################
## HandPatSmall_SFX.fx
#############################################################

FxInfo

Lifespan 10

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound SoccerBall_Kick_01 50 50 .15
		Sound SoccerBall_Kick_02 50 50 .15
		Sound SoccerBall_Kick_03 50 50 .15
		Sound SoccerBall_Kick_04 50 50 .15
		Sound SoccerBall_Kick_05 50 50 .15
	End
End
Condition
	On Time
	Time 7
	Event
		Type	Local
		At	Root
		SoundNoRepeat 3
		Sound FF_Sidewalk_01 50 50 .35
		Sound FF_Sidewalk_02 50 50 .35
		Sound FF_Sidewalk_03 50 50 .35
		Sound FF_Sidewalk_04 50 50 .35
		Sound FF_Sidewalk_05 50 50 .35
		Sound FF_Sidewalk_06 50 50 .35
		Sound FF_Sidewalk_07 50 50 .35
		Sound FF_Sidewalk_08 50 50 .35
		Sound FF_Sidewalk_09 50 50 .35
		Sound FF_Sidewalk_10 50 50 .35
	End
End
#############################################################
