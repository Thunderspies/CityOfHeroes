#############################################################
## LightningField_Activation.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
		Sound lightningfield 80 80 .6
		LifeSpan 50
	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityInnerGlow.part
		LifeSpan 50
	End

	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchInvert.part
		LifeSpan 50
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalMelee\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalMelee\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalMelee\ElectricityRing.part
		Part4	CustomizeablePowers\ElectricalMelee\ElectricityArchInvert.part
		LifeSpan 50
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	CustomizeablePowers\ElectricalMelee\ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		EName 	BurstL
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalMelee\ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName	lightning
		Type	local
		At	root
		Part2	CustomizeablePowers\ElectricalMelee\ChainLightningArchs3.part
		Part3	CustomizeablePowers\ElectricalMelee\ChainLightningArchs2.part
		Part4	CustomizeablePowers\ElectricalMelee\ChainLightningArchs1.part
		Part5	CustomizeablePowers\ElectricalMelee\ChainLightningArchs.part
		Sound ChainLight2_loop 80 80 .5
	End

	Event
		EName	lightning4
		Type	local
		At	root
		Part1	CustomizeablePowers\ElectricalMelee\ChainLightningArchsSpinner.part
	End
End

#############################################################

Condition
	On 	cycle
	Time 	33
	Chance	0.8

	Event
		EName	lightning2
		Type	local
		At	root
		Part2	CustomizeablePowers\ElectricalMelee\ChainLightningRing2.part
		LifeSpan 50
	End
End

#Condition
#	On 	cycle
#	Time 	33
#	Chance	.8
#
#	Event
#		EName	lightning2
#		Type	local
#		At	root
#		Bhvr	CustomizeablePowers\ElectricalMelee\LightningFieldSplat.bhvr
#		Splat	electricring1copy.tga
#		LifeSpan 10
#	End
#End

#############################################################

End