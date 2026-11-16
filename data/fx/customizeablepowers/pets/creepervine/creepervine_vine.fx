#############################################################
## FlyTrap_Resistance.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			TintGeom		1
		End
		Anim	FX_CreeperVine
	End
End

#############################################################

End