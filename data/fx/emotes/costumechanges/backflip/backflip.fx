#############################################################
## Backflip.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Ename 	MainLight1
		Type	Local
		At	Chest
		Part	:InitialLight.part
		Part	:InitialCircle.part
		sound	Emote_BackFlip 200 190 1
		Lifespan 30
	End
End

Condition
	On	Time
	Time	12

	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	Emotes\CostumeChanges\Howl\BodyGlow.fx
		Lifespan 30
	End
End

Condition
	On	Time
	Time	20

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
	#	Bhvr	behaviors\GenericParticleFade.bhvr
		Part	Emotes\CostumeChanges\BodyGlow\Blast.part
		Part	Emotes\CostumeChanges\BodyGlow\CircleBlast.part
		Part	Emotes\CostumeChanges\Laugh\BigGlow.part
		Part	:Lightbeams.part
		Part	:Light2.part
		Lifespan 25
	End
End

#############################################################

End