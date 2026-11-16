#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		Part	:Splatter_Tumors.part
		part	:Glow_Tumors.part
		POther  Chest
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow_Tumors.part
		ChildFX	:splatter.fx
		POther  Hips
	End


End

Condition
	On	Time
	Time	2

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow_Tumors.part
		Part	:Splatter_Tumors.part
		POther  LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow_Tumors.part
		Part	:Splatter_Tumors.part
		POther  LarmL
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow_Tumors.part
		part	:Body_Glow.part
		POther	LLegL
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow_Tumors.part
		part	:Body_Glow.part
		POther	LLegR
	End
End

Condition
	On	Time
	Time	3

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Body_Glow.part
		part	:Glow_Tumors.part

		POther  LarmR
	End




	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Body_Glow.part
		part	:Glow_Tumors.part
		POther  LarmL
	End



	Event
		Type	Local
		At	LlegR
		part	:Glow_Tumors.part
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow_Tumors.part
	End

	Event
		Type	Local
		At	ToeR
		part	:Body_Glow.part
		part	:Glow_Tumors.part
		POther	LLegR
	End

	Event
		Type	Local
		At	ToeL
		part	:Body_Glow.part
		part	:Glow_Tumors2.part
		POther	LLegL
	End

End


#############################################################

End