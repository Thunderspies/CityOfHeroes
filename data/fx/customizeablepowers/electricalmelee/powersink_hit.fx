#############################################################
## PowerSink_Hit.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Fire
		Type	Posit
		At	chest
		LifeSpan 6
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityGlow3.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityGlowStar3.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityArch4a.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchSmalla.part
		Sound lightningbolthit 70 70 0.8
		LifeSpan 23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuing.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArchContinuing2.part
		Sound suckup 70 70 0.83
		LifeSpan 120
	End
End

#############################################################

Condition
	On	Time
	Time	8.5

	Event
		EName 	Prime
		Type	Start
		At	chest
		Bhvr	CustomizeablePowers\ElectricalMelee\ConsumeProjectile.bhvr
		Part	CustomizeablePowers\ElectricalMelee\ElectricityControl01a.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityControl02a.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlowa.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlowa.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityRinga.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityArcha.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityArchInverta.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityBallTaila.part
		Part	CustomizeablePowers\ElectricalMelee\ElectricityBallTail2a.part
		Magnet	T_chest
		LookAt	T_chest
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	Prime
		Type	Destroy
	End
End

#############################################################

End