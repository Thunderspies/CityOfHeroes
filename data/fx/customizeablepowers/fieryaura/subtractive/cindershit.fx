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
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Sound torch2_loop 55.0 55.0 .5
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	:FireBallTargetFlame2.part

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