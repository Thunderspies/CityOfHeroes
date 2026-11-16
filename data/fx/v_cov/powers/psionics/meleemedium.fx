#############################################################
## MeleeMedium.fx
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
		bhvr	:Psionic_Rotate.bhvr
		Geom	PsionicRigging
		Sound psionicMindProbe2 80 80 1.0
		LifeSpan 22
	End

	Event
		EName	coreR
		Type	Local
		At	WepR
		bhvr	:Psionic_RotateR.bhvr
		Geom	PsionicRigging
		LifeSpan 22
	End
End

#############################################################

Condition
	On	Time
	Time	10

	Event
		EName	RingsAndParticles1
		Type	Local
		At	coreR
		altpiv	1
		Part1	:EnergyCoreLocalLight.part
		Part2	:EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreR
		altpiv	2
		Part1	:EnergyCoreLight.part
		Part2	:EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	coreR
		altpiv	3
		Part1	:EnergyCoreLocalLight.part
		LifeSpan 22
	End

	Event
		EName	RingsAndParticles1a
		Type	Local
		At	coreL
		altpiv	1
		Part1	:EnergyCoreLocalLight.part
		Part2	:EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		EName	RingsAndParticles3a
		Type	Local
		At	coreL
		altpiv	2
		Part1	:EnergyCoreLight.part
		Part2	:EnergyCoreFlash.part
		LifeSpan 22
	End

	Event
		EName	RingsAndParticles2a
		Type	Local
		At	coreL
		altpiv	3
		Part1	:EnergyCoreLocalLight.part
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
		Part1	:PsionicRingsLight.part
		pmagnet	SpadL
		LifeSpan 22
	End

	Event
		Type	Local
		At	SpadR
		Part1	:PsionicRingsaLight.part
		pmagnet	UarmR
		LifeSpan 22
	End

	Event
		Type	Local
		At	UarmR
		BHVR	:Offset90.bhvr
		Part1	:PsionicRingsUpperArmLight.part
		pmagnet	LarmR
		LifeSpan 22
	End

	Event
		Type	Local
		At	LarmR
		BHVR	:Offset90.bhvr
		Part1	:PsionicRingsUpperArmLight.part
		pmagnet	WepR
		LifeSpan 22
	End

	Event
		Type	Local
		At	head
		Part1	:PsionicRingsLight.part
		pmagnet	SpadR
		LifeSpan 22
	End

	Event
		Type	Local
		At	SpadL
		Part1	:PsionicRingsaLight.part
		pmagnet	UarmL
		LifeSpan 22
	End

	Event
		Type	Local
		At	UarmL
		BHVR	:Offset90.bhvr
		Part1	:PsionicRingsUpperArmLight.part
		pmagnet	LarmL
		LifeSpan 22
	End

	Event
		Type	Local
		At	LarmL
		BHVR	:Offset90.bhvr
		Part1	:PsionicRingsUpperArmLight.part
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
		bhvr	behaviors/MedianProjectile3.bhvr
		Part1	:BlastStreakLight.part
		Part2	:BlastStreakGlowHead.part
		Part2	:ScrambleThoughtsTenticles.part
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
		bhvr	behaviors/MedianProjectile3.bhvr
		Part1	:BlastStreakLight.part
		Part2	:BlastStreakGlowHead.part
		Part2	:ScrambleThoughtsTenticles.part
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
		bhvr	behaviors/MedianProjectile3.bhvr
		Part1	:BlastStreakLight.part
		Part2	:BlastStreakGlowHead.part
		Part2	:ScrambleThoughtsTenticles.part
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
		bhvr	behaviors/MedianProjectile3.bhvr
		Part1	:BlastStreakLight.part
		Part2	:BlastStreakGlowHead.part
		Part2	:ScrambleThoughtsTenticles.part
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