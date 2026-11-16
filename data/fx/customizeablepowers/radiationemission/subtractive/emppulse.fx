#############################################################
## EmpPulse.fx
#############################################################

FxInfo

Lifespan 90

## HANDS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Part1	:RadiationInfectOthersHands.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepR
		Sound radiation3_loop 80 80 .35
		Lifespan 45
	End

	Event
		Type	Local
		At	WepR
		Part2	:RadiationHandElectrons.part
		PMagnet	WepR
		Lifespan 30
	End

	Event
		Type	Local
		At	WepL
		Part1	:RadiationInfectOthersHands.part
		Part3	:InfectBubbleDotsHands.part
		Part4	:InfectBubblesHands.part
		PMagnet	WepL
		Sound mutationthrow 66 66 .75
		Lifespan 45
	End

	Event
		Type	Local
		At	WepL
		Part2	:RadiationHandElectrons.part
		PMagnet	WepL
		Lifespan 30
	End
End

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	hips
		Part2	:Irradiate.part
		Part2	:Irradiate_Alpha.part

		Sound Nova2 100.0 100.0 .8
		Lifespan 32
	End

	Event
		Type	Local
		At	hips
		Part3	:EMPSpark.part
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName 	Hookup
		Type	Local
		At	root
		Geom	RootToChestAdjustment
	End
End

Condition
	On 	Time
	Time 	18

	Event
		Type	Local
		At	Hookup
		Altpiv	1
		Part1	:Irradiateflash.part
		Part2	:Irradiateflash2.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName   RadiusFxScale
		Type	local
		At	hookup
		altpiv	1
		Bhvr	:Inferno.bhvr
	End

	Event
		Type	Local
		At	RadiusFxScale
		bhvr	:Infernobubble.bhvr
		Part1	:IrradiateYellow2.part
		Part2	:IrradiateYellowFlame01.part
		Part3	:IrradiateRings.part
		Part4	:IrradiateGlows.part
		Lifespan 30
	End
End

#############################################################

End