#########################################################
## Electric Melee - Assassin's Strike Windup
########################################################

FxInfo

LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	RightArmMove
		Type	Local
		At	LArmR
		BhvrOverride
			InitialVelocity		0.0225 0.0 0.0
		End
	End

	Event
		EName	LeftArmMove
		Type	Local
		At	LArmL
		BhvrOverride
			InitialVelocity		-0.0225 0.0 0.0
		End
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName	GlowL
		Type	Local
		At	LeftArmMove
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ArmGlow1.part
		Part	:AssassinsStrike_ArmGlow2.part
		Part	:AssassinsStrike_ArmGlow3.part
		Sound Stalker_melee_elec_assassins_strike_v3 70 70 0.8
		Lifespan 40
	End

	Event
		EName	GlowR
		Type	Local
		At	RightArmMove
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ArmGlow1.part
		Part	:AssassinsStrike_ArmGlow2.part
		Part	:AssassinsStrike_ArmGlow3.part
		Lifespan 40
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	local
		At	LeftArmMove
		ChildFX	:Child_ElectricalArcs.fx
		Lifespan 25
	End

	Event
		Type	local
		At	RightArmMove
		ChildFX	:Child_ElectricalArcs.fx
		Lifespan 25
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Part4	:ElectricityArchInvert.part
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName	ElectricalArc
		Type	local
		At	LeftArmMove
		Bhvr	behaviors\GenericParticleFade.bhvr
		Part	:AssassinsStrike_ElectricArc1.part
		Part	:AssassinsStrike_ElectricArc2.part
		Pmagnet	RightArmMove
		#Lifespan 25
	End
End

#############################################################

End