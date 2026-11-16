#############################################################
## Lunge_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

Lifespan 120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Streak8
		Type	local
		At	HandR
		Bhvr	:AssassinsSpine_Grow.bhvr
		ChildFX	:Child_WepRSpine.fx
	End
End

#############################################################

End