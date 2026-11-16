#############################################################
## Steam_High.fx
#############################################################

FxInfo

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
		part	:Body_Explode.part
		POther	Neck
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Neck
		part	:Body_Explode.part
		POther	Chest
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Chest
		Part	:Body_Explode.part

		POther	Hips
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	UArmL
		part	:Body_Explode.part
		POther	LArmL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	UArmR
		part	:Body_Explode.part
		POther	LArmR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmL
		part	:Body_Explode.part
		POther	WepL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	LArmR
		part	:Body_Explode.part
		POther	WepR
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Explode.part

		POther	ULegL
	End

	Event
		Type	Local
		At	Hips
		part	:Body_Explode.part
		POther	ULegR
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegL
		part	:Body_Explode.part
		POther	LLegL
		Lifespan 	5
		LifespanJitter	3
	End

	Event
		Type	Local
		At	ULegR
		part	:Body_Explode.part
		POther	LLegR
		Lifespan 	5
		LifespanJitter	3
	End


	Event
		Type	Local
		At	FootL
		part	:Body_Explode.part
		POther	LLegL
		Lifespan 	3
		LifespanJitter	3
	End

	Event
		Type	Local
		At	FootR
		part	:Body_Explode.part
		POther	LLegR
		Lifespan 	3
		LifespanJitter	3
	End
End

#############################################################

End