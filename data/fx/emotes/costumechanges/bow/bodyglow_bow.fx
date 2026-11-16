#########################################################
##	template

FxInfo

#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	53

	Event
		Ename	Glow
		Type 	Local
		At	Root
		ChildFX	Emotes\CostumeChanges\BodyGlow\BodyGlow.fx
		Lifespan 10
	End
End

Condition
	On	Time
	Time	58
	Event
		Ename	Flash
		Type	Local
		At	Chest
		Part 	Emotes\CostumeChanges\BodyGlow\Blast.part
	End

	Event
		Ename	Flash
		Type	Local
		At	Hips
		Part	Emotes\CostumeChanges\BodyGlow\SmokeAdditive.part
	End

End



























































End