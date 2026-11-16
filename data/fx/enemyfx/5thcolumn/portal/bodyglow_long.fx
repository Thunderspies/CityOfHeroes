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
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Head
#		BhvrOverride
#			PositionOffset 0.0 .5 0.0
#		End
		part	:Glow_Long.part
		part	:Glow2_Long.part
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  Chest
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow_Long.part
		part	:Glow2_Long.part
		Part	:SharpStar_Long.part
		POther  Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  Hips
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther	LarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		Pmagnet LarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  SpadL
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  SpadR
	End

	Event
		Type	Local
		At	FootR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther	LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther	LLegL
	End





	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  LarmR
	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  LarmL
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  Llegl
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  LlegR

	End

	Event
		Type	Local
		At	LlegR
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow_Long.part
		part	:Glow2_Long.part
		POther  FootL
	End

End



End