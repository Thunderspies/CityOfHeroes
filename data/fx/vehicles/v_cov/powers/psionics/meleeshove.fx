#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	100

###########################################################
Condition
	On	Time
	Time	12


	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part1	:EnergyCoreA.part
		bhvr	behaviors/GenericParticleFade.bhvr
		
		LifeSpan	30
		
	End
End
	
Condition
	On	Time
	Time	10

	Event
		EName	coreL
		Type	Local
		At	WepL
		Geom	PsionicRigging		
		bhvr	:Psionic_RotateL.bhvr
		Sound psionic3 80 80 .9
		
		LifeSpan	30
	End

	Event
		EName	Flash2
		Type	Local
		At	coreL
		altpiv	2
		
		Part2	:EnergyCoreFlash.part
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	20
	End
##################################################################################################

	Event	
		ENAME	Chestnode
		Type	Local
		At	Head
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End	

	Event	
		Type	Local
		At	SpadL
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End
	
	Event	
		Type	Local
		At	SpadR
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End
	
	Event	
		Type	Local
		At	UarmL
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End
	
	Event	
		Type	Local
		At	LarmL
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End

	Event	
		Type	Local
		At	WepL
		Geom	ChestPivotsBuffHP
		bhvr	behaviors/GenericParticleFade.bhvr
		LifeSpan	10
	End
	

##################################################################################################

	Event
		EName	RingsAndParticles1
		Type	Local
		At	Head
		
		pmagnet	SpadL
		
		Part1	:PsionicRings.part
		bhvr	:Psionic_Rotate.bhvr
		LifeSpan	10
		
	End
	
	Event
		EName	RingsAndParticles1
		Type	Local
		At	UarmL
		BHVR	:Offset90.bhvr
		
		pmagnet	LarmL
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	10
	End

	Event
		EName	RingsAndParticles1
		Type	Local
		At	LarmL
		BHVR	:Offset90.bhvr
		
		pmagnet	WepL
		Part1	:PsionicRingsUpperArm.part
		LifeSpan	10
	End


End

##################################################################################################

Condition
	On	Time
	Time	20

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove.part
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	10
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove2.part
		bhvr	:Psionic_Rotater.bhvr
		LifeSpan	10
		
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Geom	PsionicPBAOERing
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	7
	End

End

##################################################################################################



Condition
	On	Time
	Time	28

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove.part
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	10
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove2.part
		bhvr	:Psionic_Rotater.bhvr
		LifeSpan	10
		
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Geom	PsionicPBAOERing
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	7
	End

End

##################################################################################################


Condition
	On	Time
	Time	23

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove.part
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	10
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove2.part
		bhvr	:Psionic_Rotater.bhvr
		LifeSpan	10
		
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Geom	PsionicPBAOERing
		bhvr	:Psionic_Rotate_Geom.bhvr
		
		LifeSpan	7
	End

End


Condition
	On	Time
	Time	32

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove.part
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	10
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove2.part
		bhvr	:Psionic_Rotater.bhvr
		LifeSpan	10
		
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Geom	PsionicPBAOERing
		bhvr	:Psionic_Rotate_Geom.bhvr
		
		LifeSpan	7
	End

End

Condition
	On	Time
	Time	32

	Event
		EName	RingsAndParticles3
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove.part
		bhvr	:Psionic_Rotater.bhvr
		
		LifeSpan	10
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Part	:EnergyShove2.part
		bhvr	:Psionic_Rotater.bhvr
		LifeSpan	10
		
	End

	Event
		Type	Local
		At	coreL
		altpiv	2
		
		Geom	PsionicPBAOERing
		bhvr	:Psionic_Rotate_Geom.bhvr
		
		LifeSpan	7
	End
End

End			