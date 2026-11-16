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
		Type	Local
		At	Head
		part	:Body_Fog_Small_Lieu.part
		POther	Hair
	End

	Event
		Type	Local
		At	Chest
		part	:Body_Fog_Small_Lieu.part
		POther	Neck
	End

	Event
		Type	Local
		At	Chest
		part	:Body_Fog_Big_Lieu.part
		POther	Hips
	End

	Event
		Type	Local
		At	LArmL
		part	:Body_Fog_Small_Lieu.part
		part	:Body_Fog_Small_Highlight_Lieu.part
		POther	UArmL
	End

	Event
		Type	Local
		At	LArmR
		part	:Body_Fog_Small_Lieu.part
		part	:Body_Fog_Small_Highlight.part
		POther	UArmR
	End

	Event
		Type	Local
		At	WepL
		part	:Body_Fog_Small_Lieu.part
		part	:Body_Fog_Small_Highlight_Lieu.part
		POther	LArmL
	End

	Event
		Type	Local
		At	WepR
		part	:Body_Fog_Small_Lieu.part
		part	:Body_Fog_Small_Highlight_Lieu.part
		POther	LArmR
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Fog_Lieu.part
		POther	ULegL
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Fog_Lieu.part
		POther	ULegR
	End

	Event
		Type	Local
		At	LLegL
		part	:Body_Fog_Lieu.part
		part	:Body_Fog_Big_Highlight_Lieu.part

		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		part	:Body_Fog_Lieu.part
		part	:Body_Fog_Big_Highlight_Lieu.part
		POther	ULegR
	End


	Event
		Type	Local
		At	FootL
		part	:Body_Fog_Lieu.part
		part	:Body_Fog_Big_Highlight_Lieu.part

		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		part	:Body_Fog_Lieu.part
		part	:Body_Fog_Big_Highlight_Lieu.part

		POther	LLegR
	End
End

#############################################################

End