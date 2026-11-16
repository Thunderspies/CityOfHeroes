#########################################################
##	template

FxInfo

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Local
		At	Root
		sound	Emote_Salute 200 190 1
	End
End

Condition
	On	Time
	Time	15

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
	Time	18
	Event
		Ename	Flash
		Type	Local
		At	Chest
		Part 	Emotes\CostumeChanges\BodyGlow\SparkleBig.part
		Part	Emotes\CostumeChanges\BodyGlow\CircleBlast.part
	End

	Event
		Ename	Flash
		Type	Local
		At	Root
		Part	Emotes\CostumeChanges\BodyGlow\Sparkle.part
	Lifespan 45
	End

End



























































End