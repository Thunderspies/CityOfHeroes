#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Hair
		part	:Body_Fog.part
		#POther	Neck
	End

	Event
		Type	PositOnly
		At	Chest
		part	:Body_Fog.part
		POther	Hips
	End



	Event
		Type	PositOnly
		At	UArmL
		part	:Body_Fog.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	UArmR
		part	:Body_Fog.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	LArmL
		part	:Body_Fog.part
		POther	WepL
	End

	Event
		Type	PositOnly
		At	LArmR
		part	:Body_Fog.part
		POther	WepR
	End

	Event
		Type	PositOnly
		At	ULegL
		part	:Body_Fog.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	ULegR
		part	:Body_Fog.part
		POther	LLegR
	End

	Event
		Type	PositOnly
		At	LLegL
		part	:Body_Fog.part
		POther	FootL
	End

	Event
		Type	PositOnly
		At	LLegR
		part	:Body_Fog.part
		POther	FootR
	End

	Event
		Type	PositOnly
		At	FootL
		part	:Body_Fog.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	FootR
		part	:Body_Fog.part
		POther	LLegR
	End
End

#############################################################

End