#############################################################
## Costume Anim Entity - Shield - T9I24_01
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On Time
	Time 	0
	// TriggerbitS SHIELD
	// Domany 1

	Event
		// Ename	ShieldFX
		Type	Local
		At	LArmL
			Bhvr	:Female_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Base_01
		// while	SHIELD
		// Flags	OneAtATime
		// ChildFX :Female_Shield_T9I24_01_CHILD.fx
	End

	Event
		Type	Local
		At	LArmL
			Bhvr	:Female_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Detail_01
	End

	Event
		Type	Local
		At	LArmL
			Bhvr	:Female_Shield_T9I24_01.bhvr
			BhvrOverride
				TintGeom 1
			End
			Geom	FX_Shield_T9i24_Detail_02
	End

End
#############################################################

End