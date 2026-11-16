#########################################################
##	Aura
FxInfo

################################################################################################

Flags    InheritAlpha

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Chest		
		Sound obsidian_loop 70 70 .12
		
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		Part	:InfernorockBitsLingering.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	Head	
		part2	:PeaceAuraEnergy_Head_01trail.part
		
	End

	Event
		EName	Aura
		Type	Local
		At	Head	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		part2	:PeaceAuraEnergy_Head_01.part
		part2	:PeaceAuraGlow_Head_01.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		part2	:PeaceAuraGlow_Head_01trail.part
		
	End

	Event
		EName	Aura
		Type	Local
		At	SpadR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraGlow_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01trail.part
		part2	:PeaceAuraGlow_Spad_01trail.part
		
	End


	Event
		Type	Local
		At	SpadR	
		BHVR	Behaviors\GenericParticleFade.bhvr		
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		LifeSpan	30
	End

	Event
		EName	Aura
		Type	Local
		At	SpadL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraGlow_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01trail.part
		part2	:PeaceAuraGlow_Spad_01trail.part
		
	End

	Event
		Type	Local
		At	SpadL
		BHVR	Behaviors\GenericParticleFade.bhvr			
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		LifeSpan	30
	End

	Event
		EName	Aura
		Type	Local
		At	Hips		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Hips_01.part
		part2	:PeaceAuraGlow_Hips_01.part
		part2	:PeaceAuraEnergy_Hips_01trail.part
		part2	:PeaceAuraGlow_Hips_01trail.part
		
	End

	Event
		Type	Local
		At	Hips	
		BHVR	Behaviors\GenericParticleFade.bhvr		
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		LifeSpan	30
	End

	Event
		EName	Aura
		Type	Local
		At	FootL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	LLegL
		
	End


	Event
		EName	Aura
		Type	Local
		At	FootL
		BHVR	Behaviors\GenericParticleFade.bhvr			
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End

	Event
		EName	Aura
		Type	Local
		At	LLegL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	Hips
		
	End

	Event
		EName	Aura
		Type	Local
		At	LLegL
		BHVR	Behaviors\GenericParticleFade.bhvr			
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End

	Event
		EName	Aura
		Type	Local
		At	ToeL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_ToeL_01.part
		part2	:PeaceAuraGlow_ToeL_01.part
		part2	:PeaceAuraEnergy_ToeL_01trail.part
		part2	:PeaceAuraGlow_ToeL_01trail.part
		
	End


	Event
		EName	Aura
		Type	Local
		At	ToeL	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End

		Event
		EName	Aura
		Type	Local
		At	FootR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	LLegR
		
	End


	Event
		EName	Aura
		Type	Local
		At	FootR
		BHVR	Behaviors\GenericParticleFade.bhvr			
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End

	Event
		EName	Aura
		Type	Local
		At	LLegR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	Hips
		
	End

	Event
		EName	Aura
		Type	Local
		At	LLegR
		BHVR	Behaviors\GenericParticleFade.bhvr			
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End

	Event
		EName	Aura
		Type	Local
		At	ToeR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_ToeL_01.part
		part2	:PeaceAuraGlow_ToeL_01.part
		part2	:PeaceAuraEnergy_ToeL_01trail.part
		part2	:PeaceAuraGlow_ToeL_01trail.part
		
	End


	Event
		EName	Aura
		Type	Local
		At	ToeR	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		Part	:InfernorockBitsLingering.part
		part2	:PeaceAuraEnergy_Head_01trail.part
		
		LifeSpan	30
		
	End
	

End


End				