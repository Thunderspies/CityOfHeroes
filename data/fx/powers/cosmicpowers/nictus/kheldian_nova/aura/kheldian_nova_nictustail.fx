#########################################################
## Dark Nova Aura (Tail)
#########################################################

FxInfo

#Flags    InheritAlpha

## AURA FX #######################################################

Condition
	On	Time
	Time	0
	
	Event
		EName	Aura
		Type	Local
		At	ToeR	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		#part2	:PeaceAuraGlow_ToeL_02trail.part
		#part2	:PeaceAuraGlow_ToeL_02trailBlue.part
		part2	:PeaceAuraGlow_ToeL_02trailPurple.part
		PMagnet	FootR
	End

	Event
		EName	Aura
		Type	Local
		At	FootR	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		#part2	:PeaceAuraGlow_ToeL_02trail.part
		#part2	:PeaceAuraGlow_ToeL_02trailBlue.part
		part2	:PeaceAuraGlow_ToeL_02trailPurple.part
		PMagnet	LlegR
	End

	Event
		EName	Aura
		Type	Local
		At	LlegR	
		BHVR	Behaviors\GenericParticleFade.bhvr	
		#part2	:PeaceAuraGlow_ToeL_02trail.part
		#part2	:PeaceAuraGlow_ToeL_02trailBlue.part
		part2	:PeaceAuraGlow_ToeL_02trailPurple.part
		PMagnet	hips
	End
End

#########################################################

End				