#############################################################
## LaughFire.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		Ename 	MainLight1
		Type	Local
		At	Chest
		Part	:InitialLight.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	40

	Event
		Ename 	MainLight2
		Type	Local
		At	Chest
		Part	:Light2.part
		Lifespan 40
	End
End

Condition
	On	Time
	Time	60

	Event
		Ename	BodyGlow
		Type	Local
		At	Root
		ChildFX	Emotes\CostumeChanges\Howl\BodyGlow.fx
		Lifespan 20
	End
End

Condition
	On	Time
	Time	65

	Event
		Ename	SmokeAdditive
		Type	Local
		At	Chest
		Part	:SmokeAdditive.part
		Lifespan 10
	End

	Event
		Ename	Smoke
		Type 	Local
		At	Root
		Part	:SmokePillar.part
		Lifespan 10
	End
End

Condition
	On	Time
	Time	67

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
		Part	Emotes\CostumeChanges\BodyGlow\Blast.part
		Part	Emotes\CostumeChanges\BodyGlow\CircleBlast.part
		Lifespan 30
	End
End

Condition
	On	Time
	Time	67

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
		ChildFX :BodyFire.fx
		Lifespan 150
	End
End

Condition
	On	Time
	Time	130

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
		ChildFX Auras\Male\Ash\BodyAura_NoTrail.fx
		Lifespan 90
	End
End

#############################################################

End