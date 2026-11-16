#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
#		Sound	eleccage_loop 70 30 .25
	End

End

#########################################################
################ HEAD AND UPPER BODY ####################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		Part	:Body_Glow.part
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther  Neck

	End

#########################################################
################## ARMS AND HANDS #######################
#########################################################

	Event
		Type	Local
		At	UarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther 	Neck
	End

	Event
		Type	Local
		At	UarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther 	Neck
	End

	Event
		Type	Local
		At	LarmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther WepR
	End

	Event
		Type	Local
		At	LarmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther WepL
	End

	Event
		Type	Local
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther  LarmL
	End

#########################################################
################ HIPS, LEGS AND FEET ####################
#########################################################

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		POther  LlegR
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Pmagnet LlegL
		POther  LlegL
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Part	:Body_Glow.part
		POther  ULegL
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Part	:Body_Glow.part
		POther  ULegR
	End

	Event
		Type	Local
		At	FootR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Part	:Body_Glow.part
		POther	LLegR
	End



	Event
		Type	Local
		At	FootL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Body_Glow.part
		Part	:Body_Glow.part
		Pother	LLegL
	End


End