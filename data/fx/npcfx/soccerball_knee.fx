#############################################################
## HandPatSmall_SFX.fx
#############################################################

FxInfo

Lifespan 15

Condition
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound SoccerBall_Bounce_01 75 75 .25
		Sound SoccerBall_Bounce_02 75 75 .25
		Sound SoccerBall_Bounce_03 75 75 .35
		Sound SoccerBall_Bounce_04 75 75 .35
		Sound SoccerBall_Bounce_05 75 75 .35
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
