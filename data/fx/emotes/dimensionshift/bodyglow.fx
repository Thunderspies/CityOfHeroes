#############################################################
## Cryo_Body.fx
#############################################################

FxInfo
Lifespan 70

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Head
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	Neck
		Lifespan 	60
		LifespanJitter	10

	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Chest
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	Hips
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	LarmL
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	LArmR
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	HandL
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	HandR
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	ULegLFX
		Type	PositOnly
		At	ULegL
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	LLegL
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	ULegRFX
		Type	PositOnly
		At	ULegR
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	LLegR
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	LLegL
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	FootL
		Lifespan 	60
		LifespanJitter	10
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	LLegR
		Part	:Body_Glow.part
		Part	:Body_Steam.part
		POther	FootR
		Lifespan 	60
		LifespanJitter	10
	End
End

#########################################################

End