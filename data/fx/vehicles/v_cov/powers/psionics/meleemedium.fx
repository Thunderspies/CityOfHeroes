#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	200

###########################################################
Condition
	On	Time
	Time	0

	Event
		EName	coreL
		Type	Local
		At	WepL
		Geom	PsionicRigging		
		bhvr	:Psionic_Rotate.bhvr
		Sound psionic3 80 80 .9

		LifeSpan	22
	End

	Event
		EName	coreR
		Type	Local
		At	WepR
		Geom	PsionicRigging		
		bhvr	:Psionic_RotateR.bhvr
		Sound psionic3 80 80 .9
		LifeSpan	22
	End

End

##############################################################################

Condition
	On	Time
	Time	10

	Event
		EName	RingsAndParticles1
		Type	Local
		At	coreR
		altpiv	1
		
		Part1	:EnergyCoreLocal.part
		LifeSpan	22
		
	End

	Event
		EName	RingsAndParticles2
		Type	Local
		At	coreR
		altpiv	3
		
		Part1	:EnergyCoreLocal.part
		LifeSpan	22
		
	End

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreR
		altpiv	2
		
		Part1	:EnergyCore.part
		LifeSpan	22
		
	End

	Event
		EName	RingsAndParticles1a
		Type	Local
		At	coreL
		altpiv	1
		
		Part1	:EnergyCoreLocal.part
		LifeSpan	22
	End

	Event
		EName	RingsAndParticles2a
		Type	Local
		At	coreL
		altpiv	3
		
		Part1	:EnergyCoreLocal.part
		LifeSpan	22
		
	End

	Event
		EName	RingsAndParticles3a
		Type	Local
		At	coreL
		altpiv	2
		
		Part1	:EnergyCore.part
		LifeSpan	22
		
	End

	Event
		EName	Flash1
		Type	Local
		At	coreR
		altpiv	1
		
		Part2	:EnergyCoreFlash.part
		LifeSpan	22
	End

	Event
		EName	Flash2
		Type	Local
		At	coreL
		altpiv	2
		
		Part2	:EnergyCoreFlash.part
		LifeSpan	22

	End

	Event
		EName	Flash1
		Type	Local
		At	coreL
		altpiv	1
		
		Part2	:EnergyCoreFlash.part
		LifeSpan	22
	End

	Event
		EName	Flash2
		Type	Local
		At	coreR
		altpiv	2
		
		Part2	:EnergyCoreFlash.part
		LifeSpan	22
	End

##################################################################################################

End
	
Condition
	On	Time
	Time	0

	Event	
		ENAME	Chestnode
		Type	Local
		At	Head
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End	

	Event	
		ENAME	Chestnode
		Type	Local
		At	SpadL
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	SpadR
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	UarmL
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	UarmR
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	LarmL
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	LarmR
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	WepL
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End
	
	Event	
		ENAME	Chestnode
		Type	Local
		At	WepR
		Geom	ChestPivotsBuffHP
		LifeSpan	22
	End

##################################################################################################

	Event
		EName	RingsAndParticles1
		Type	Local
		At	Head
		
		pmagnet	SpadL
		
		Part1	:PsionicRings.part
		LifeSpan	22
		
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	SpadR
		
		pmagnet	UarmR
		Part1	:PsionicRingsa.part
		LifeSpan	22
	End
	
	Event
		EName	RingsAndParticles1
		Type	Local
		At	UarmR
		BHVR	:Offset90.bhvr
		pmagnet	LarmR
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	22
	End
	
	Event
		EName	RingsAndParticles1
		Type	Local
		At	LarmR
		BHVR	:Offset90.bhvr
		
		pmagnet	WepR
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	22
	End
##################################################################################################

	Event
		EName	RingsAndParticles1
		Type	Local
		At	head
		
		pmagnet	SpadR
		Part1	:PsionicRings.part
		LifeSpan	22
	End
	
	Event
		EName	RingsAndParticles1
		Type	Local
		At	SpadL
		
		pmagnet	UarmL
		Part1	:PsionicRingsa.part
		LifeSpan	22
	End
	
	Event
		EName	RingsAndParticles1
		Type	Local
		At	UarmL
		BHVR	:Offset90.bhvr
		
		pmagnet	LarmL
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	22
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	LarmL
		BHVR	:Offset90.bhvr
		
		pmagnet	WepL
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	22
	End

End
	
Condition
	On	Time
	Time	15

	Event
		EName	prime
		Type	Start
		At	CoreL
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		#bhvr	:Medianprojectile3.bhvr
		bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime
		  AltPiv 1
		Part1	:BlastStreak.part
		Part2	:BlastStreakGlowHead.part
		
	End
	
End

Condition
	On	Time
	Time	15

	Event
		EName	prime2
		Type	Start
		At	CoreR
		   Altpiv   2
		Geom	FrontFacingLocalPivot
		#bhvr	:Medianprojectile3.bhvr
		bhvr	behaviors/MedianProjectile3.bhvr
		Magnet	T_Head
		LookAt  T_Head
	End

	Event
		Type	Local
		At	Prime2
		  AltPiv 1
#		Part1	:BlastStreak.part
#		Part2	:BlastStreakGlowHead.part
		Part1	:BlastStreaks.part
		Part2	:ScrambleThoughtsTenticles.part
		
	End
	
End

Condition
	On	PrimeHit
		
	Event
		EName	Prime
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

End			