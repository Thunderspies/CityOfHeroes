#############################################################
## Irradiate.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hips
		Sound Nova2 100.0 100.0 .8
	End

	Event
		Type	Local
		At	WepR
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .35
		Lifespan 100
	End


	Event
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\RadiationBlast\RadiationInfectOthersHands.part
		Part2	CustomizeablePowers\RadiationBlast\RadiationHandElectrons.part
		Part3	CustomizeablePowers\RadiationBlast\InfectBubbleDotsHands.part
		Part4	CustomizeablePowers\RadiationBlast\InfectBubblesHands.part
		PMagnet	WepL
		Lifespan 100
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	Local
		At	hips
		Part2	CustomizeablePowers\RadiationBlast\Irradiate.part
		Lifespan 22
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Prime8
		Type	Local
		At	root
		Part1	CustomizeablePowers\RadiationBlast\Irradiateflash.part
		Part2	CustomizeablePowers\RadiationBlast\Irradiateflash2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	hips
		Part1	CustomizeablePowers\RadiationBlast\IrradiateRingsHips.part
		Part3	CustomizeablePowers\RadiationBlast\IrradiateGlowHips.part
		Lifespan 48 #17
	End

	Event
		Type	Local
		At	hips
		Part1	CustomizeablePowers\RadiationBlast\IrradiaterockBits.part
		Part2	CustomizeablePowers\RadiationBlast\IrradiaterockBits2.part
		Lifespan 1
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\RadiationBlast\Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\RadiationBlast\Infernobubble.bhvr
		Part1	CustomizeablePowers\RadiationBlast\IrradiateYellow2.part
		Part2	CustomizeablePowers\RadiationBlast\IrradiateYellowFlame01.part
		Lifespan 30
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\RadiationBlast\Infernobubble.bhvr
		Part4	CustomizeablePowers\RadiationBlast\IrradiateGlows.part
		Part3	CustomizeablePowers\RadiationBlast\IrradiateRings.part
		Lifespan 15
	End
End

#############################################################

End