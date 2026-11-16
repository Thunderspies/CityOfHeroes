#############################################################
## Evil_DullPain.fx
#############################################################

FxInfo

#########################################################


#####################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		Type	Local
		At	UlegL
		part1	:DullPain_BodyGlowsDark.part
		PMagnet	LLegL
	End

	Event
		Type	Local
		At	UlegR
		part1	:DullPain_BodyGlowsDark.part
		PMagnet	LLegR
	End

	Event
		Type	Local
		At	LLegR
		part1	:DullPain_BodyGlowsDark.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	LLegL
		part1	:DullPain_BodyGlowsDark.part
		PMagnet	FootL
	End

	Event
		Type	Local
		At	FootL
		part1	:DullPain_BodyGlowsDark.part
	End

	Event
		Type	Local
		At	FootR
		part1	:DullPain_BodyGlowsDark.part
	End
End

#########################################################

End