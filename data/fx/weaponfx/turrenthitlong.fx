#############################################################
## TurrentHitLong.fx
#############################################################

FxInfo
LifeSpan	45

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:PistolflashA.part
		Part2	:PistolflashStar2A.part
		LifeSpan	10

	End
End

#Condition
#	On	Time
#	Time	2
#
#	Event
#		Type	Local
#		At	Origin
#		#Sound	machgunhit 90 75 .76
#
#	End
#End


Condition
	On 	cycle
	Time 	2
	Chance	1

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:PistolflashA.part
		Part2	:PistolflashStar2A.part
		LifeSpan	10

	End

End

Condition
	On	cycle
	Time	3

	Chance	.5


	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60

	End

End


Condition
	On	cycle
	Time	3

	Chance	.9


	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60

	End

End

#############################################################

End

