#############################################################
## HEADER
#############################################################

FXInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Ground1
		Type	Start
		At	Root
		BhvrOverride
			StartJitter		2.5 0.0 2.5
		End
	End

	Event
		EName	Ground2
		Type	Start
		At	Root
		BhvrOverride
			StartJitter		3.0 0.0 3.0
		End
	End

	Event
		EName	Ground3
		Type	Start
		At	Root
		BhvrOverride
			StartJitter		4.0 0.0 4.0
		End
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		EName	ElectricalArc1
		Type	local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ElectricArc1.part
		Part	:AssassinsStrike_ElectricArc2.part
		Pmagnet	Ground1
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		EName	ElectricalArc2
		Type	local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ElectricArc1.part
		Part	:AssassinsStrike_ElectricArc2.part
		Pmagnet	Ground2
		Lifespan 1
	End
End

Condition
	On 	Cycle
	Time 	5
	Chance	0.25

	Event
		EName	ElectricalArc3
		Type	local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ElectricArc1.part
		Part	:AssassinsStrike_ElectricArc2.part
		Pmagnet	Ground3
		Lifespan 1
	End
End

#############################################################

End