#############################################################
## FlyTrap_Resistance.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale

#############################################################

Condition
	Event
		Type	Local
		At	Hips
		BhvrOverride
			TintGeom		1
		End
		Anim	FlyTrap_Arms
	End
End

#############################################################

End