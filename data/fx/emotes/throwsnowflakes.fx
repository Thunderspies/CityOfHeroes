#############################################################
## Thrown Confetti FX
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Mystic
		LifeSpan 	5
		Part1	:Snowflakes.part
		Part1	:Snow.part
		Part	:Snowdust.part
		Part	:Snowdust2.part
		Sound holiday_SnowThrow 50 50 .5
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	Start
		At	Mystic
		LifeSpan 	5
		Part1	:Snowflakes.part
		Part1	:Snow.part

	End
End

#############################################################

End