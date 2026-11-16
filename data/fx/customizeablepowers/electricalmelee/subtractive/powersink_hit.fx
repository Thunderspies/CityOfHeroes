#############################################################
## PowerSink_Hit.fx
#############################################################

FxInfo
LifeSpan 200

#############################################################

Input
	Inpname	Hips
End

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
		Part1	:ElectricityGlow3.part
		Part2	:ElectricityGlowStar3.part
		Part3	:ElectricityArch4a.part
		Part4	:ElectricityArchSmalla.part
		Sound lightningbolthit 70 70 0.8
		LifeSpan 23
	End

	Event
		EName	continuingArchs
		Type	Local
		At	Chest
		Part1	:ElectricityArchContinuing.part
		Part2	:ElectricityArchContinuing2.part
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
		Part	:ElectricityControl01a.part
		Part	:ElectricityControl02a.part
		Part	:ElectricityInnerGlowa.part
		Part	:ElectricityOuterGlowa.part
		Part	:ElectricityRinga.part
		Part	:ElectricityArcha.part
		Part	:ElectricityArchInverta.part
		Part	:ElectricityBallTaila.part
		Part	:ElectricityBallTail2a.part
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