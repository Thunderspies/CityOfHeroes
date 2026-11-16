#########################################################
##	template

FxInfo

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0	

########## HEAD

	Event
		EName	Body_Glow
		Type	Local
		At	Head
		part	:GlowBanshee_01.part
		POther	Hair
	End

########## CHEST

	Event
		EName	Body_Glow
		Type	Local
		At	Neck
		part	:GlowBanshee_01.part
		POther	Col_R
	End

	Event
		EName	Body_Glow
		Type	Local
		At	Neck
		part	:GlowBanshee_01.part
		POther	Col_L
	End

	Event
		EName	Offset_Hips_R
		Type	Local
		At	Hips
		Bhvr	:Offset_Hips_R.bhvr
		part	:GlowBanshee_01.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	UArmR
		part	:GlowBanshee_01.part
		POther	Offset_Hips_R
	End

	Event
		EName	Offset_Hips_L
		Type	Local
		At	Hips
		Bhvr	:Offset_Hips_L.bhvr
		part	:GlowBanshee_01.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	UArmL
		part	:GlowBanshee_01.part
		POther	Offset_Hips_L
	End

########## ARM RIGHT

	Event
		EName	Body_Glow
		Type	Local
		At	UArmR
		part	:GlowBanshee_01.part
		POther	LArmR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LArmR
		part	:GlowBanshee_01.part
		POther	HandR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandR
		part	:GlowBanshee_01.part
		POther	RingR
	End

########## ARM LEFT

	Event
		EName	Body_Glow
		Type	Local
		At	UArmL
		part	:GlowBanshee_01.part
		POther	LArmL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LArmL
		part	:GlowBanshee_01.part
		POther	HandL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandL
		part	:GlowBanshee_01.part
		POther	RingL
	End

########## LEG RIGHT

	Event
		EName	Body_Glow
		Type	Local
		At	ULegR
		part	:GlowBanshee_01.part
		POther	LLegR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegR
		part	:GlowBanshee_01.part
		POther	FootR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootR
		part	:GlowBanshee_01.part
		POther	ToeR
	End

########## LEG LEFT

	Event
		EName	Body_Glow
		Type	Local
		At	ULegL
		part	:GlowBanshee_01.part
		POther	LLegL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegL
		part	:GlowBanshee_01.part
		POther	FootL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootL
		part	:GlowBanshee_01.part
		POther	ToeL
	End

End