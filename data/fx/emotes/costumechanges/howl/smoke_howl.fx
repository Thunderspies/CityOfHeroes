#########################################################
##
##

FxInfo

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Root
		sound	Emote_Howl 200 190 1
	End
End

Condition
	On	Time
	Time	20

	Event
		EName 	Flash3
		Type	Local
		At	Chest
		part	:LowerBlast3.part
		Lifespan 10
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Flash3
		Type	Local
		At	Root
		ChildFX	:BodySmoke.fx
		Lifespan 90
	End

End

Condition
	On	Time
	Time	35

	Event
		EName 	Flash3
		Type	Local
		At	Chest
		part	:LargeBlast.part
		part	:LargeBlast2.part
		part	:LargeBlast3.part
		Lifespan 10
	End

	Event
		EName 	Flash3
		Type	Local
		At	Root
		ChildFX :BodyGlow.fx
		Lifespan 50
	End

End