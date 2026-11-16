#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

## HANDS ###########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy1
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 25
	End

	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 25
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Prime
		Type	start
		At	HandL
		Bhvr	Behaviors\Medianprojectile1.bhvr
		Part1	:ElectricityControl01a.part
		Part2	:ElectricityControl02a.part
		Part3	:ElectricityInnerGlowa.part
		Part5	:ElectricityBallTail.part
		Sound elecball2 80 80 .8
		Magnet	Target
		Lookat	Target
	End

	Event
		EName 	Prime2
		Type	start
		At	HandL
		Bhvr	Behaviors\Medianprojectile1.bhvr
		Part1	:ElectricityOuterGlowa.part
		Part2	:ElectricityArcha.part
		Part3	:ElectricityRinga.part
		Part4	:ElectricityArchInverta.part
		Part5	:ElectricityBallTail2.part
		Magnet	Target
		Lookat	Target
	End
End

## ON HIT ###########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	Target
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		Sound elecballhit 60 60 .6
		Lifespan 30
	End

	Event
		EName 	ChestOffset
		Type	start
		At	T_Root
		Geom	roottochestadjustment
		Lifespan 30
	End

	Event
		Type	start
		At	ChestOffset
		altpiv	1
		bhvr	:BubbleScale01.bhvr
		Geom	GEO_WhiteSphere
		Lifespan 15
	End

	Event
		Type	start
		At	ChestOffset
		altpiv	1
		Part1	:ElectricitySparkBurst.part
		Part2	:ElectricitySparkBurst.part
		Part4	:ShockFieldRingA.part
		Lifespan 30
	End

	Event

		Type	Local
		At	Target
		Part2	:ShockFieldGlow.part
		Lifespan 5
	End
End

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End
End

#############################################################

End