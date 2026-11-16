#############################################################
## ElectricityBall.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

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
		Bhvr	CustomizeablePowers\ElectricalBlast\Medianprojectile1.bhvr
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
		Bhvr	CustomizeablePowers\ElectricalBlast\Medianprojectile1.bhvr
		Part1	:ElectricityOuterGlowa.part
		Part2	:ElectricityArcha.part
		Part3	:ElectricityRinga.part
		Part4	:ElectricityArchInverta.part
		Part5	:ElectricityBallTail2.part
		Magnet	Target
		Lookat	Target
	End
End

#############################################################

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
		lifespan 30
	End

	Event
		ENAME	H
		Type	start
		At	T_Root
		geom	roottochestadjustment
		lifespan 30
	End

	Event
		EName 	Mallet
		Type	start
		At	H
		altpiv	1
		Bhvr	CustomizeablePowers\ElectricalBlast\ScaleBubble2.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_ElectricityHit
		lifespan 5
	End

	Event
		Type	start
		At	H
		altpiv	1
		Part1	:ElectricitySparkBurst.part
		Part2	:ElectricitySparkBurst.part
		Part4	:ShockFieldRingA.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	:ShockFieldGlow.part
		lifespan 5
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