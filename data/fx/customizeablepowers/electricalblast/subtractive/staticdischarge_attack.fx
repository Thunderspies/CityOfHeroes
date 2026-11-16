#############################################################
## StaticDischarge_Attack.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Type	Local
		At	WepL
		Sound StaticDischarge2 90 90 0.9
		lifespan 200
	End

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		Sound eleccontrol2 100 100 0.5
		lifespan 50
	End

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		lifespan 50
	End

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		lifespan 50
	End
End

############################################################

Condition
	On 	Cycle
	Time 	3

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		lifespan 50
	End
End

Condition
	On 	Cycle
	Time 	5

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		lifespan 50
	End
End

Condition
	On 	Cycle
	Time 	7

	Event
		Type	Start
		At	WepL
		ChildFX	:Child_StaticDischarge_Arc.fx
		lifespan 50
	End
End

#############################################################

End