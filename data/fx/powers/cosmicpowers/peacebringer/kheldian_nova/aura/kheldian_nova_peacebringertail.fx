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
		At	Hips	
		part2	:PeaceAuraGlow_ToeL_01.part
		pOther	Hips
	End

	Event
		Type	Local
		At	LLegR	
		part2	:PeaceAuraGlow_ToeL_01.part
		pOther	FootR
	End

	Event
		Type	Local
		At	FootR	
		part2	:PeaceAuraGlow_ToeL_01.part
		pOther	ToeR
	End
	
#	Event
#		EName	Aura
#		Type	Local
#		At	ToeR	
#		part2	:PeaceAuraGlow_ToeL_02trail.part
#		PMagnet	FootR
#	End
#
#	Event
#		EName	Aura
#		Type	Local
#		At	FootR	
#		part2	:PeaceAuraGlow_ToeL_02trail.part
#		PMagnet	LlegR
#	End
#
#	Event
#		EName	Aura
#		Type	Local
#		At	LlegR	
#		part2	:PeaceAuraGlow_ToeL_02trail.part
#		PMagnet	hips
#	End
End

#############################################################

End				