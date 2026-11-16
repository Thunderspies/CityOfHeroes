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
		part	:Glow_01.part
		POther	Hair
	End

########## CHEST

	Event
		EName	Body_Glow
		Type	Local
		At	Neck
		part	:Glow_01.part
		POther	Col_R
	End

	Event
		EName	Body_Glow
		Type	Local
		At	Neck
		part	:Glow_01.part
		POther	Col_L
	End

	Event
		EName	Offset_Hips_R
		Type	Local
		At	Hips
		Bhvr	:Offset_Hips_R.bhvr
		part	:Glow_01.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	UArmR
		part	:Glow_01.part
		POther	Offset_Hips_R
	End

	Event
		EName	Offset_Hips_L
		Type	Local
		At	Hips
		Bhvr	:Offset_Hips_L.bhvr
		part	:Glow_01.part
	End

	Event
		EName	Body_Glow
		Type	Local
		At	UArmL
		part	:Glow_01.part
		POther	Offset_Hips_L
	End

########## ARM RIGHT

	Event
		EName	Body_Glow
		Type	Local
		At	UArmR
		part	:Glow_01.part
		POther	LArmR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LArmR
		part	:Glow_01.part
		POther	HandR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandR
		part	:Glow_01.part
		POther	RingR
	End

########## ARM LEFT

	Event
		EName	Body_Glow
		Type	Local
		At	UArmL
		part	:Glow_01.part
		POther	LArmL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LArmL
		part	:Glow_01.part
		POther	HandL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	HandL
		part	:Glow_01.part
		POther	RingL
	End

########## LEG RIGHT

	Event
		EName	Body_Glow
		Type	Local
		At	ULegR
		part	:Glow_01.part
		POther	LLegR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegR
		part	:Glow_01.part
		POther	FootR
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootR
		part	:Glow_01.part
		POther	ToeR
	End

########## LEG LEFT

	Event
		EName	Body_Glow
		Type	Local
		At	ULegL
		part	:Glow_01.part
		POther	LLegL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	LLegL
		part	:Glow_01.part
		POther	FootL
	End

	Event
		EName	Body_Glow
		Type	Local
		At	FootL
		part	:Glow_01.part
		POther	ToeL
	End

End
Condition
	On 	cycle
	Time 	20
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End	
End

Condition
	On 	cycle
	Time 	19
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End	
End

Condition
	On 	cycle
	Time 	21
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End	
End


Condition
	On 	cycle
	Time 	22
	Chance	.2
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	.2

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:FortitudeRaysSmall2.part
		lifespan	25
	End
End