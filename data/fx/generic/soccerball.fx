#############################################################
## Soccer_ball.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SoccerBall
		Type	Local
		At	Mystic
		BhvrOverride
			Scale		0.5 0.5 0.5
		End
		Geom	Soccer_Ball
	End
End

#############################################################

End