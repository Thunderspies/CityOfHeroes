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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlow.part
		Lifespan 25
	End


	Event
		EName 	ThingyB
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityRing.part
		Lifespan 25
	End

	Event
		EName 	Thingy1R
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlow.part
		Lifespan 25
	End


	Event
		EName 	ThingyBR
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityRing.part
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
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	MysticAnchor
		Type	Local
		At	Head
		Geom	PsionicRigging
		BhvrOverride
			PositionOffset	0.0 0.0 1.5
		End
	End

	Event
		EName 	Prime
		Type	start
		At	MysticAnchor
		Bhvr	CustomizeablePowers\ElectricalBlast\Subtractive\Medianprojectile1.bhvr
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl01a.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityControl02a.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityInnerGlowa.part
		Part5	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricityBallTail.part
		Sound elecball2 80 80 .8
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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ShockFieldArchsa.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ShockFieldArchs2a.part
		Part3	CustomizeablePowers\ElectricalBlast\Subtractive\ShockFieldArchs1a.part
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
		Bhvr	CustomizeablePowers\ElectricalBlast\Subtractive\ScaleBubble2.bhvr
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
		Part1	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricitySparkBurst.part
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ElectricitySparkBurst.part
		Part4	CustomizeablePowers\ElectricalBlast\Subtractive\ShockFieldRingA.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	CustomizeablePowers\ElectricalBlast\Subtractive\ShockFieldGlow.part
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