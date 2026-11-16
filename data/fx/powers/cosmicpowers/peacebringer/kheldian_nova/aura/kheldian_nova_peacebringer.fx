#############################################################
## FX System Name
#############################################################

FxInfo

Flags    InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest		
		Sound obsidian_loop 70 70 .12
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan	220
	End
End


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest		
		sound	peaceeyes1 60 50 .7
	
	End
End
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head	
		BhvrOverride
			PositionOffset		0.5 0.125 0.35
		End
		part	:PeaceAuraGlow_Eye_R.part
	End

	Event
		Type	Local
		At	Head	
		BhvrOverride
			PositionOffset		-0.5 0.125 0.35
		End
		part	:PeaceAuraGlow_Eye_R.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	Aura
		Type	Local
		At	Head	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		part2	:PeaceAuraEnergy_Head_01.part
		#part2	:PeaceAuraEnergy_Head_01trail.part
		##part2	:PeaceAuraGlow_Head_01.part
		##part2	:PeaceAuraGlow_Head_01trail.part
	End

	Event
		EName	Aura
		Type	Local
		At	SpadR		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01trail.part
		##part2	:PeaceAuraGlow_Spad_01.part
		##part2	:PeaceAuraGlow_Spad_01trail.part
	End

	Event
		EName	Aura
		Type	Local
		At	SpadL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraEnergy_Spad_01.part
		part2	:PeaceAuraEnergy_Spad_01trail.part
		##part2	:PeaceAuraGlow_Spad_01.part
		##part2	:PeaceAuraGlow_Spad_01trail.part
	End

	Event
		EName	Aura
		Type	Local
		At	Hips		
		BHVR	Behaviors\GenericParticleFade.bhvr
		#part2	:PeaceAuraEnergy_Hips_01.part
		part2	:PeaceAuraGlow_Hips_01.part
		part2	:PeaceAuraEnergy_Hips_01trail.part
		##part2	:PeaceAuraGlow_Hips_01trail.part
		pOther	Chest
	End

	Event
		EName	Aura
		Type	Local
		At	ULegL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		#part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	Hips
	End

	Event
		EName	Aura
		Type	Local
		At	ULegL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		#part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	LLegL
	End

	Event
		EName	Aura
		Type	Local
		At	LLegL		
		BHVR	Behaviors\GenericParticleFade.bhvr
		part2	:PeaceAuraGlow_LLegL_02.part
		#part2	:PeaceAuraGlow_LLegL_02trail.part
		POther	FootL
	End

#	Event
#		EName	Aura
#		Type	Local
#		At	ToeL		
#		BHVR	Behaviors\GenericParticleFade.bhvr
#		part2	:PeaceAuraGlow_ToeL_01.part
#		#part2	:PeaceAuraEnergy_ToeL_01trail.part
#		#part2	:PeaceAuraEnergy_ToeL_01.part
#		#part2	:PeaceAuraGlow_ToeL_01trail.part
#	End
End

#############################################################

End				