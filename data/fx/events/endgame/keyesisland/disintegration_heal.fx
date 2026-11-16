#############################################################
## Hit_Body_Antimatter.fx
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
	#	Sound eleccage_loop 70 70 .25
	End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

	Event
		Type 	Local
		At 	Origin
		Flags	AdoptParentEntity
		BhvrOverride
			Alpha	100
		End
	End

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	Head
		Part	:Glow_Bright_Head.part
		part	:Disintegration_Body.part
		Part	:Disintegration_Body_Fill.part
		POther	Neck
	End

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	UArmL
		Part	:Glow_Bright_Head.part
	End

	Event
		ENAME	HeadGlow
		Type	LOCAL
		At	UArmR
		Part	:Glow_Bright_Head.part
	End

	Event
		Type	LOCAL
		At	UArmR
		Part	:Glow_Bright.part
		part	:Disintegration_Body.part
		Part	:Disintegration_Body_Fill.part
		POther	LArmR
	End

	Event
		Type	LOCAL
		At	UArmL
		Part	:Glow_Bright.part
		part	:Disintegration_Body.part
		Part	:Disintegration_Body_Fill.part
		POther	LArmL
	End

	Event
		ENAME	RightLarmGlow
		Type	LOCAL
		At	WepR
		part	:Disintegration_Body.part
	End

	Event
		ENAME	LeftLarmGlow
		Type	LOCAL
		At	WepL
		part	:Disintegration_Body.part
	End

	Event
		Type	LOCAL
		At	LLegR
		part	:Disintegration_Foot.part
		part	:Glow_Bright.part
		POther	FootR
	End

	Event
		Type	LOCAL
		At	LLegL
		part	:Disintegration_Foot.part
		part	:Glow_Bright.part
		POther	FootL
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Part	:Disintegration_Body_Fill_Flat.part
		Part	:Disintegration_Body_Flat.part
	End
End



End