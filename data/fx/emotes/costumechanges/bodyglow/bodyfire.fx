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
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:Glow.part
		Pmagnet Head
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	FootR
		part	:Glow.part
		Pmagnet LLegR
	End

	Event
		Type	Local
		At	FootL
		part	:Glow.part
		Pmagnet LLegL
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		Pmagnet UarmL
	End


	Event
		ENAME	NeckNode
		Type	Local
		At	Neck
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow.part
		Pmagnet Neck
		POther  Neck

	End



	Event
		ENAME	HipsNode
		Type	Local
		At	Hips

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Glow.part
		Pmagnet SpadL
		POther  SpadL
	End


	Event
		ENAME	SpadRNode
		Type	Local
		At	UarmR
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:Glow.part
		Pmagnet SpadR
		POther  SpadR
	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow.part
		Pmagnet Chest
		POther  Chest
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow.part
		Pmagnet LarmR
		POther  LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Glow.part
		Pmagnet LarmR
		POther  LarmR



	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow.part
		Pmagnet LarmL
		POther  LarmL



	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow.part
		Pmagnet LarmL
		POther  LarmL



	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow.part
		Pmagnet Llegl
		POther  Llegl



	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow.part
		Pmagnet LlegR
		POther  LlegR

	End

	Event
		Type	Local
		At	LlegR
		part	:Glow.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow.part
		Pmagnet FootL
		POther  FootL
	End

End



End