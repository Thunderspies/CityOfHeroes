#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 180

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Part1	CustomizeablePowers\EnergyBlast\HandGlowWhite.part
		Part2	CustomizeablePowers\EnergyBlast\HandGlowEmber.part
		Lifespan 50
	End

	Event
		Type	local
		At	WepR
		Part1	CustomizeablePowers\EnergyBlast\SniperBubbles.part
		Part2	CustomizeablePowers\EnergyBlast\SniperBubblesWhite.part
		Lifespan 37
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	local
		At	WepR
		Part3	CustomizeablePowers\EnergyBlast\SniperCloudsWhite1.part
		Sound sniper3 80 80 .8
		Lifespan 55
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		EName	Prime
		Type	start
		At	WepR
		Bhvr	Behaviors\projectile3.bhvr
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		8
			PhysForcePower		250
			PhysForcePowerJitter	50
			TintGeom		1
		End
		Part1	CustomizeablePowers\EnergyBlast\RapidCometBubbles.part
		Part2	CustomizeablePowers\EnergyBlast\RapidCometBubblesBright.part
		Part3	CustomizeablePowers\EnergyBlast\RapidCometBubblesWhite.part
		Part4	CustomizeablePowers\EnergyBlast\RapidCometBubblesLocal.part
		Part5	CustomizeablePowers\EnergyBlast\RapidEnergyCoreLocal.part
		Geom	Tintable_SniperBolt
		Magnet	Target
		LookAt  Target
	End

	Event
		Ename	LiftForce
		Type	Local
		At	Prime
		BhvrOverride
			pyrRotate		0 0 15
			PhysForceType		Up
			PhysForceRadius		15
			PhysForcePower		250
			PhysForcePowerJitter	100
		End
	End

	Event
		Ename	PushForce
		Type	Local
		At	Prime
		BhvrOverride
			PositionOffset		0 0 3
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		12
			PhysForcePower		400
			PhysForcePowerJitter	50
		End
	End
End

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

#############################################################

End