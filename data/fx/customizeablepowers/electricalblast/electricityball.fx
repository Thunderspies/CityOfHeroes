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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Lifespan 25
	End


	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
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
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01a.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02a.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityBallTail.part
		Sound elecball2 80 80 .8
		Magnet	Target
		Lookat	Target
	End

	Event
		EName 	Prime2
		Type	start
		At	HandL
		Bhvr	CustomizeablePowers\ElectricalBlast\Medianprojectile1.bhvr
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlowa.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArcha.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRinga.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchInverta.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityBallTail2.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ShockFieldArchsa.part
		Part2	CustomizeablePowers\ElectricalBlast\ShockFieldArchs2a.part
		Part3	CustomizeablePowers\ElectricalBlast\ShockFieldArchs1a.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Part4	CustomizeablePowers\ElectricalBlast\ShockFieldRingA.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	CustomizeablePowers\ElectricalBlast\ShockFieldGlow.part
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