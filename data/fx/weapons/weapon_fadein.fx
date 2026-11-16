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
		BhvrOverride
			FadeInLength	15
			Alpha		255
		End
		#Flags	AdoptParentEntity
		#Sound	transformB 80 60 .86
	End
End

#############################################################

End