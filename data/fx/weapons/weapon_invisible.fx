#############################################################
## Custom Weapon - Broadsword Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
		#Sound	transformB 80 60 .86
	End
End

#############################################################

End