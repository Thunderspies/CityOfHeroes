#############################################################
## LaughSmoke_Huge.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	15

	Event
		Ename 	MainLight1
		Type	Local
		At	Root
		ChildFX	:HandFire.fx
		Lifespan 70
	End
End

#############################################################

Condition
	On	Time
	Time	67

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
		ChildFX :BodyFire_Huge.fx
		Lifespan 150
	End
End

#############################################################

Condition
	On	Time
	Time	70

	Event
		Ename	SmokeAdditive
		Type	Local
		At	Chest
		Part	:SmokeAdditive.part
		Part	:EmberBurst.part
		ChildFX	:BodyGlow.fx
		Lifespan 10
	End
End

Condition
	On	Time
	Time	72

	Event
		Ename 	Lightbeams2
		Type	Local
		At	Chest
		Part	:Blast.part
		Part	:CircleBlast.part
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	100

	Event
		Ename 	Lightbeams3
		Type	Local
		At	Chest
		ChildFX Auras\Male\Ash\BodyAura_NoTrail.fx
		Lifespan 150
	End
End

#############################################################

End