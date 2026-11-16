#############################################################
## DominateMind_Attack.fx
#############################################################

FxInfo
LifeSpan	150

Condition
	On Time
	Time 0
	
	Event
		Type Local
		At Root
		Sound Seers_RevealWeakness_Hit 50 50 1
	End
End


## HANDS ###########################################################

Condition
	On	Time
	Time	0
	Chance	0.5

	Event
		Type	PositOnly
		At	Head
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	3
	Chance	0.5

	Event
		Type	PositOnly
		At	Chest
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	6
	Chance	0.5

	Event
		Type	PositOnly
		At	UArmL
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	9
	Chance	0.5

	Event
		Type	PositOnly
		At	UArmR
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	12
	Chance	0.5

	Event
		Type	PositOnly
		At	LLegL
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End

Condition
	On	Time
	Time	15
	Chance	0.5

	Event
		Type	PositOnly
		At	LLegR
		Part	:Body_Target_Glow.part
		Part	:Body_Target_Rings.part
		Lifespan 45
	End
End
##############################################################################

End