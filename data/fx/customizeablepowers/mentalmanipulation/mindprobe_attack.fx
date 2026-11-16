#############################################################
## MindProbe_Attack.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	coreL
		Type	Local
		At	WepL
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_Rotate.bhvr
		Geom	PsionicRigging
		Sound psionicMindProbe2 80 80 1.0
		LifeSpan 22
	End

	Event
		EName	coreR
		Type	Local
		At	WepR
		bhvr	CustomizeablePowers\MentalManipulation\Psionic_RotateR.bhvr
		Geom	PsionicRigging
		LifeSpan 22
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	coreR
		altpiv	1
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocalLight.part
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		Type	Local
		At	coreR
		altpiv	2
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLight.part
		LifeSpan 22
	End

	Event
		Type	Local
		At	coreR
		altpiv	3
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocalLight.part
		LifeSpan 22
	End

	Event
		Type	Local
		At	coreL
		altpiv	1
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocalLight.part
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLight.part
		Part2	CustomizeablePowers\MentalManipulation\EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		Type	Local
		At	coreL
		altpiv	3
		Part1	CustomizeablePowers\MentalManipulation\EnergyCoreLocalLight.part
		LifeSpan 22
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsLight.part
		pmagnet	SpadL
		LifeSpan 22
	End

	Event
		Type	Local
		At	SpadR
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsaLight.part
		pmagnet	UarmR
		LifeSpan 22
	End

	Event
		Type	Local
		At	UarmR
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArmLight.part
		pmagnet	LarmR
		LifeSpan 22
	End

	Event
		Type	Local
		At	LarmR
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArmLight.part
		pmagnet	WepR
		LifeSpan 22
	End

	Event
		Type	Local
		At	head
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsLight.part
		pmagnet	SpadR
		LifeSpan 22
	End

	Event
		Type	Local
		At	SpadL
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsaLight.part
		pmagnet	UarmL
		LifeSpan 22
	End

	Event
		Type	Local
		At	UarmL
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArmLight.part
		pmagnet	LarmL
		LifeSpan 22
	End

	Event
		Type	Local
		At	LarmL
		BHVR	CustomizeablePowers\MentalManipulation\Offset90.bhvr
		Part1	CustomizeablePowers\MentalManipulation\PsionicRingsUpperArmLight.part
		pmagnet	WepL
		LifeSpan 22
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		EName	prime
		Type	Start
		At	T_Head
		bhvr	Behaviors\MedianProjectile3.bhvr
		Part1	CustomizeablePowers\MentalManipulation\BlastStreakLight.part
		Part2	CustomizeablePowers\MentalManipulation\BlastStreakGlowHead.part
		Part2	CustomizeablePowers\MentalManipulation\ScrambleThoughtsTenticles.part
		Magnet	WepL
	End
End

Condition
	On	Time
	Time	15

	Event
		EName	prime1
		Type	Start
		At	T_Head
		bhvr	Behaviors\MedianProjectile3.bhvr
		Part1	CustomizeablePowers\MentalManipulation\BlastStreakLight.part
		Part2	CustomizeablePowers\MentalManipulation\BlastStreakGlowHead.part
		Part2	CustomizeablePowers\MentalManipulation\ScrambleThoughtsTenticles.part
		Magnet	WepR
	End
End

#############################################################

Condition
	On	Time
	Time	21

	Event
		EName	prime2
		Type	Start
		At	T_Head
		bhvr	Behaviors\MedianProjectile3.bhvr
		Part1	CustomizeablePowers\MentalManipulation\BlastStreakLight.part
		Part2	CustomizeablePowers\MentalManipulation\BlastStreakGlowHead.part
		Part2	CustomizeablePowers\MentalManipulation\ScrambleThoughtsTenticles.part
		Magnet	WepL
	End
End

Condition
	On	Time
	Time	21

	Event
		EName	prime3
		Type	Start
		At	T_Head
		bhvr	Behaviors\MedianProjectile3.bhvr
		Part1	CustomizeablePowers\MentalManipulation\BlastStreakLight.part
		Part2	CustomizeablePowers\MentalManipulation\BlastStreakGlowHead.part
		Part2	CustomizeablePowers\MentalManipulation\ScrambleThoughtsTenticles.part
		Magnet	WepR
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		EName	Prime
		Type	Destroy
	End
End

Condition
	On	Prime1Hit

	Event
		EName	Prime1
		Type	Destroy
	End
End

Condition
	On	Prime2Hit

	Event
		EName	Prime2
		Type	Destroy
	End
End

Condition
	On	Prime3Hit

	Event
		EName	Prime3
		Type	Destroy
	End
End

#############################################################

End