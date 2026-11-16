#############################################################
## BodyGlow.fx
#############################################################

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
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:Glow2.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow2.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:Glow2.part
		Pmagnet LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:Glow2.part
		Pmagnet LLegL
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow2.part
		Pmagnet UarmL
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow2.part
	#	Pmagnet Neck
		POther  Neck

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow2.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Glow2.part
		Pmagnet SpadL
		POther  SpadL
	End


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:Glow2.part
		Pmagnet SpadR
		POther  SpadR
	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow2.part
		Pmagnet Chest
		POther  Chest
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow2.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Glow2.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow2.part
		Pmagnet LarmL
		POther  LarmL
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow2.part
		Pmagnet LarmL
		POther  LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow2.part
		Pmagnet LlegL
		POther  LlegL
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow2.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegR
		part	:Glow2.part
		Pmagnet ToeR
		POther  ToeR
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow2.part
		Pmagnet ToeL
		POther  ToeL
	End

End



End