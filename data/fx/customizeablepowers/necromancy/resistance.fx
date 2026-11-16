#############################################################
## Resistance.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		ENAME	Base
		Type	start
		At	root
		ChildFX :SummonHit.fx
		Lifespan 180
	End
End

#############################################################

Condition
	On	DEATH

	Event
		Type	Start
		At	Root
		ChildFX	:Zero_DarkArmorCloak.fx
		Lifespan 60
	End

	Event
		Type	Start
		At	Root
		ChildFX	:Dismiss_Zombie.fx
		Lifespan 150
	End
End

#############################################################

End