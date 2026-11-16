#############################################################
## TelekineticThrust_Attack.fx
#############################################################

FxInfo

LifeSpan 100

#############################################################

Condition
	On	Time
	Time	8

	Event
		EName	coreL
		Type	Local
		At	WepL
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_RotateL.bhvr
		Geom	PsionicRigging
		Sound psionicshove3 80 80 .9
		LifeSpan 30
	End

	Event
		EName	Flash2
		Type	Local
		At	coreL
		altpiv	2
		bhvr	Behaviors\GenericParticleFade.bhvr
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		LifeSpan 20
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	coreL
		altpiv	2
		bhvr	Behaviors\GenericParticleFade.bhvr
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreA.part
		LifeSpan 30
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	UarmL
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArm.part
		pmagnet	LarmL
		LifeSpan 10
	End

	Event
		Type	Local
		At	LarmL
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArm.part
		pmagnet	WepL
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	coreL
		altpiv	2
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_Rotater.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyShove2.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	24

	Event
		Type	Local
		At	coreL
		altpiv	2
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_Rotater.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyShove2.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	coreL
		altpiv	2
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_Rotater.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyShove2.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	32

	Event
		Type	Local
		At	coreL
		altpiv	2
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_Rotater.bhvr
		Part	CustomizeablePowers\MentalManipulation\EnergyShove2.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	PushDirection
		Type	Posit
		At	Flash2
		Lookat	Target
		LifeSpan 20
	End

	Event
		Ename	PushForce
		Type	Local
		At	PushDirection
		BhvrOverride
			PositionOffset		0 0 7.5
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		20
			PhysForcePower		200
			PhysForcePowerJitter	100
		End
		LifeSpan 20
	End
End

#############################################################

End