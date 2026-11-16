########################################################
## Black Dwarf Aura
########################################################

FxInfo

#Flags    InheritAlpha

## SOUNDS FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	Aura
		Type	Local
		At	Chest
		Sound wardwarf_loop 60 60 .18
	End
End

## AURA FX ######################################################

Condition
	On	Time
	Time	0

	Event
		EName	EyeR
		Type	Local
		At	Chest		
		part	:PeaceAuraGlow_Eye_R.part
		bhvr	:PeaceAuraGlow_Eye_R.bhvr
	End

	Event
		EName	EyeL
		Type	Local
		At	Chest		
		part	:PeaceAuraGlow_Eye_R.part
		bhvr	:PeaceAuraGlow_Eye_L.bhvr
	End

	Event
		EName	Aura
		Type	Local
		At	Chest	
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
		bhvr	:PeaceAuraGlow_Chest.bhvr
	End	

	Event
		EName	Aura
		Type	Local
		At	Col_R		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
		bhvr	:PeaceAuraGlow_Col_R.bhvr
	End

	Event
		EName	Aura
		Type	Local
		At	Col_L		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
		bhvr	:PeaceAuraGlow_Col_L.bhvr
	End

#	Event
#		EName	Aura
#		Type	Local
#		At	UarmL		
#		part2	:PeaceAuraEnergy_Foot_01.part
#		part2	:PeaceAuraEnergy_Foot_01Blue.part
#		part2	:PeaceAuraEnergy_Foot_01Purple.part
#	End
#
#	Event
#		EName	Aura
#		Type	Local
#		At	UarmR		
#		part2	:PeaceAuraEnergy_Foot_01.part
#		part2	:PeaceAuraEnergy_Foot_01Blue.part
#		part2	:PeaceAuraEnergy_Foot_01Purple.part
#	End
#
	Event
		EName	Aura
		Type	Local
		At	LarmL		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	LarmR		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	HandL		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	HandR		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	Hips		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		##part2	:PeaceAuraEnergy_Col_R_01.part
		##part2	:PeaceAuraGlow_Col_R_01.part
		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
		##part2	:PeaceAuraGlow_Col_R_01Blue.part
		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
		##part2	:PeaceAuraGlow_Col_R_01Purple.part
	End

#	Event
#		EName	Aura
#		Type	Local
#		At	ULegL		
#		part2	:PeaceAuraEnergy_Foot_01.part
#		part2	:PeaceAuraEnergy_Foot_01Blue.part
#		part2	:PeaceAuraEnergy_Foot_01Purple.part
#		##part2	:PeaceAuraEnergy_Col_R_01.part
#		##part2	:PeaceAuraGlow_Col_R_01.part
#		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
#		##part2	:PeaceAuraGlow_Col_R_01Blue.part
#		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
#		##part2	:PeaceAuraGlow_Col_R_01Purple.part
#	End
#
#	Event
#		EName	Aura
#		Type	Local
#		At	ULegR		
#		part2	:PeaceAuraEnergy_Foot_01.part
#		part2	:PeaceAuraEnergy_Foot_01Blue.part
#		part2	:PeaceAuraEnergy_Foot_01Purple.part
#		##part2	:PeaceAuraEnergy_Col_R_01.part
#		##part2	:PeaceAuraGlow_Col_R_01.part
#		##part2	:PeaceAuraEnergy_Col_R_01Blue.part
#		##part2	:PeaceAuraGlow_Col_R_01Blue.part
#		##part2	:PeaceAuraEnergy_Col_R_01Purple.part
#		##part2	:PeaceAuraGlow_Col_R_01Purple.part
#	End

	Event
		EName	Aura
		Type	Local
		At	LlegL		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	LlegR		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	FootL		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		#part2	:PeaceAuraGlow_Foot_01.part
		#part2	:PeaceAuraGlow_Foot_01Blue.part
		#part2	:PeaceAuraGlow_Foot_01Purple.part
	End

	Event
		EName	Aura
		Type	Local
		At	FootR		
		part2	:PeaceAuraEnergy_Foot_01.part
		part2	:PeaceAuraEnergy_Foot_01Blue.part
		part2	:PeaceAuraEnergy_Foot_01Purple.part
		#part2	:PeaceAuraGlow_Foot_01.part
		#part2	:PeaceAuraGlow_Foot_01Blue.part
		#part2	:PeaceAuraGlow_Foot_01Purple.part
	End
End

########################################################

End