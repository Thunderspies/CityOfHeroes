#############################################################
## Red_JacobsElectricityShieldChild.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet LLEGL
		LifeSpan 2
	End

	Event
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet LLEGR
		LifeSpan 2
	End

	Event
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01.part
		Part2	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part3	POWERS\ElectricityControl\Red_ElectricityInnerGlowSubtle.part
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part2	POWERS\ElectricityControl\Red_ElectricityControl02white.part
		Part1	POWERS\ElectricityControl\Red_ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArch.part
		Part4	POWERS\ElectricityControl\Red_ElectricityArchInvert.part
		LifeSpan 1
	End

	Event
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01.part
		Part2	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part3	POWERS\ElectricityControl\Red_ElectricityInnerGlowSubtle.part
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		Part2	POWERS\ElectricityControl\Red_ElectricityControl02white.part
		Part1	POWERS\ElectricityControl\Red_ElectricityOuterGlow.part
		Part2	POWERS\ElectricityControl\Red_ElectricityArch.part
		Part4	POWERS\ElectricityControl\Red_ElectricityArchInvert.part
		LifeSpan 1
	End
End

#############################################################

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	LLEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet ULEGL
		LifeSpan 2
	End

	Event
		Type	Local
		At	LLEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet UlegR
		LifeSpan 2
	End
End

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	ULEGL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet chest
		LifeSpan 2
	End

	Event
		Type	Local
		At	ULEGR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet Chest
		LifeSpan 2
	End
End

#############################################################

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	WepL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet LArmL
		LifeSpan 2
	End

	Event
		Type	Local
		At	WepR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet LArmR
		LifeSpan 2
	End
End

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	LArmR
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet UArmR
		LifeSpan 2
	End

	Event
		Type	Local
		At	LArmL
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part	POWERS\ElectricityControl\Red_ChargedBolt01.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt02.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt05.part
		Part	POWERS\ElectricityControl\Red_ChargedBolt06.part
		PMagnet UArmL
		LifeSpan 2
	End
End

#############################################################

Condition
	On 	Time
	Time	6

	Event
		Type	Local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01.part
		Part2	POWERS\ElectricityControl\Red_ElectricityControl02.part
		Part3	POWERS\ElectricityControl\Red_ElectricityInnerGlowSubtle.part
		Part1	POWERS\ElectricityControl\Red_ElectricityControl01white.part
		LifeSpan 1
	End
End

#############################################################

End