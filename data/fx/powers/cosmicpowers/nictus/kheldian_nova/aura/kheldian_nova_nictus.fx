#########################################################
## Dark Nova Aura
#########################################################

FxInfo

#Flags    InheritAlpha

## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head			
		Sound WarshadeDwarf2_loop 60 60 .44
	End
End

## AURA FX #######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Aura
		Type	Local
		At	Head		
		bhvr	:PeaceAuraGlow_Eye_L.bhvr
		part	:PeaceAuraGlow_Eye_R.part
	End

	Event
		EName	Aura
		Type	Local
		At	Head		
		bhvr	:PeaceAuraGlow_Eye_R.bhvr
		part	:PeaceAuraGlow_Eye_R.part
	End

##	Event
##		Type	Local
##		At	Head			
##		part2	:PeaceAuraEnergy_Head_01trail.part
##		part2	:PeaceAuraEnergy_Head_01trailBlue.part
##		part2	:PeaceAuraEnergy_Head_01trailPurple.part
##	End

	Event
		EName	Aura
		Type	Local
		At	Head	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		part2	:PeaceAuraEnergy_Head_01.part
		part2	:PeaceAuraEnergy_Head_01Blue.part
		part2	:PeaceAuraEnergy_Head_01Purple.part
		#part2	:PeaceAuraGlow_Head_01.part
	End

	Event
		EName	Aura
		Type	Local
		At	SpadR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_Spad_01trail.part
		part2	:PeaceAuraEnergy_Spad_01trailPurple.part
		#part2	:PeaceAuraEnergy_Spad_01trailBlue.part
		part2	:PeaceAuraGlow_Spad_01trail.part
	End
	
	Event
		EName	Aura
		Type	Local
		At	SpadR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01Purple.part
		part2	:PeaceAuraEnergy_Spad_01Blue.part
		#part2	:PeaceAuraGlow_Spad_01.part
	End

	Event
		EName	Aura
		Type	Local
		At	SpadL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_Spad_01trail.part
		part2	:PeaceAuraEnergy_Spad_01trailPurple.part
		#part2	:PeaceAuraEnergy_Spad_01trailBlue.part
		part2	:PeaceAuraGlow_Spad_01trail.part
	End

	Event
		EName	Aura
		Type	Local
		At	SpadL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01Blue.part
		part2	:PeaceAuraEnergy_Spad_01Purple.part
		#part2	:PeaceAuraGlow_Spad_01.part
	End

	Event
		EName	Aura
		Type	Local
		At	Hips		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_Hips_01trail.part
		#part2	:PeaceAuraGlow_Hips_01trailPurple.part
		#part2	:PeaceAuraGlow_Hips_01trailBlue.part
		#part2	:PeaceAuraEnergy_Hips_01trail.part
		#part2	:PeaceAuraEnergy_Hips_01trailBlue.part
		part2	:PeaceAuraEnergy_Hips_01trailPurple.part
	End

	Event
		EName	Aura
		Type	Local
		At	Hips		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Hips_01.part
		part2	:PeaceAuraEnergy_Hips_01Blue.part
		part2	:PeaceAuraEnergy_Hips_01Purple.part
		part2	:PeaceAuraGlow_Hips_01Purple.part
		part2	:PeaceAuraGlow_Hips_01Blue.part
		part2	:PeaceAuraGlow_Hips_01.part
	End

	Event
		EName	Aura
		Type	Local
		At	LLegL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_ToeL_01.part
		#part2	:PeaceAuraEnergy_ToeL_01Blue.part
		#part2	:PeaceAuraEnergy_ToeL_01Purple.part
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02Blue.part
		part2	:PeaceAuraGlow_LLegL_02Purple.part
		##part2	:PeaceAuraGlow_LLegL_02trail.part
		##part2	:PeaceAuraGlow_LLegL_02trailBlue.part
		##part2	:PeaceAuraGlow_LLegL_02trailPurple.part
		POther	Hips
	End

	Event
		EName	Aura
		Type	Local
		At	FootL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_ToeL_01.part
		#part2	:PeaceAuraEnergy_ToeL_01Blue.part
		#part2	:PeaceAuraEnergy_ToeL_01Purple.part
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02Blue.part
		part2	:PeaceAuraGlow_LLegL_02Purple.part
		##part2	:PeaceAuraGlow_LLegL_02trail.part
		##part2	:PeaceAuraGlow_LLegL_02trailBlue.part
		##part2	:PeaceAuraGlow_LLegL_02trailPurple.part
		POther	LLegL
	End

	Event
		EName	Aura
		Type	Local
		At	ToeL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_ToeL_01.part
		#part2	:PeaceAuraEnergy_ToeL_01Blue.part
		#part2	:PeaceAuraEnergy_ToeL_01Purple.part
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02Blue.part
		part2	:PeaceAuraGlow_LLegL_02Purple.part
		##part2	:PeaceAuraGlow_LLegL_02trail.part
		##part2	:PeaceAuraGlow_LLegL_02trailBlue.part
		##part2	:PeaceAuraGlow_LLegL_02trailPurple.part
	End
End

#########################################################

End				