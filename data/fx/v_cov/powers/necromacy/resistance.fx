#############################################################
## Resistance.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	root
		ChildFX V_COV\Powers\Necromacy\SummonHit.fx
		Lifespan 180
	End
End

#############################################################

Condition
	On	DEATH

	Event
		Type	Start
		At	Root
		ChildFX	powers\DarknessControl\Zero_DarkArmorCloak.fx
		Lifespan 60
	End

	Event
		Type	Start
		At	Root
		ChildFX	V_COV\Powers\Necromacy\Dismiss_zombie.fx
		Lifespan 150
	End
End

#############################################################

End