
FxInfo

Lifespan	200

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Chest
		LifeSpan 	1
		#Sound	Ice7 100 80 .6
		#Part1	:GlassDust.part
	End

End

Condition
	On 	Time
	Time 	0
	Repeat 		0
	RepeatJitter	1

	Event
		Type	Start 
		At	Chest
		LifeSpan 	150
		ChildFX	:GlassCube.fx
		
	End

End

End