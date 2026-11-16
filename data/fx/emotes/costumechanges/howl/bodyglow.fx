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
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		Part	:Glow2.part
		Pmagnet Head
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Part	:Glow2.part
		POther  Neck

	End

#########################################################
################## ARMS AND HANDS #######################
#########################################################

	Event
		Type	Local
		At	UarmR
		Part	:Glow2.part
		Pmagnet LArmR
	End

	Event
		Type	Local
		At	UarmL
		Part	:Glow2.part
		Pmagnet LArmL
	End

	Event
		Type	Local
		At	LarmR
		Part	:Glow2.part
		Pmagnet WepR
	End

	Event
		Type	Local
		At	LarmL
		Part	:Glow2.part
		Pmagnet WepL
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Glow2.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Glow2.part
		Pmagnet LarmL
		POther  LarmL
	End

#########################################################
################ HIPS, LEGS AND FEET ####################
#########################################################

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Glow2.part
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Glow2.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Glow2.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		Type	Local
		At	LlegL
		Part	:Glow2.part
		Part	:Glow2.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	LlegR
		Part	:Glow2.part
		Part	:Glow2.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	FootR
		Part	:Glow2.part
		Part	:Glow2.part
		Pmagnet LLegR
		POther	LLegR
	End



	Event
		Type	Local
		At	FootL
		Part	:Glow2.part
		Part	:Glow2.part
		Pmagnet LLegL
		Pother	LLegL
	End


End