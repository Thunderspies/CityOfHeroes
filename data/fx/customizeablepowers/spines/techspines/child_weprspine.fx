#############################################################
## BarbSwipe_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		While	MALE
		BhvrOverride
			TintGeom		1
		End
		Geom	Male_TechSpines_WepR
	End

	Event
		Type	local
		At	Origin
		While	FEMALE
		BhvrOverride
			TintGeom		1
		End
		Geom	Fem_TechSpines_WepR
	End

	Event
		Type	local
		At	Origin
		While	HUGE
		BhvrOverride
			TintGeom		1
		End
		Geom	Huge_TechSpines_WepR
	End
End

#############################################################

End