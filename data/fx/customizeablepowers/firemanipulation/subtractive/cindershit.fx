#############################################################
## CindersHit.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	hips
		Part1	POWERS\FireControl\FireBallTargetSteam.part
		Part2	POWERS\FireControl\FireBallTargetFlame2.part
		Part3	POWERS\FireControl\FireBallSpark.part
		Sound torch2_loop 55.0 55.0 .5
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	POWERS\FireControl\FireBallTargetFlame2.part

	End
End

#############################################################

#Condition
#	On 	time
#	Time 	0
#	Repeat  5
#
#	Event
#		EName 	Sparkz
#		Type 	Local
#		At 	hips
#		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
#		Lifespan 70
#		LifespanJitter 	20
#	End
#End

#############################################################

End