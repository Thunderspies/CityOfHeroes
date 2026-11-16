#############################################################
## Fire_Continuing.fx
#############################################################

FxInfo
Lifespan 45
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Posit
		At	hips
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
	End

	Event
		Type	Posit
		At	chest
		Part1	:FireBallTargetFlame2.part
		Sound torch2_loop 55.0 55.0 .5
	End
End

#############################################################

End