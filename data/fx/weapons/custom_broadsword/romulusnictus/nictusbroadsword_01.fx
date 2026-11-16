#########################################################
## Warshade - Eye Glow Continuing FX
########################################################

FxInfo

Flags InheritAlpha DontSuppress

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	SwordTip
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		0.0 0.0 3.5
		End
	End

	Event
		Type	Local
		At	Origin
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
		Part	:BlackTendrils.part
		pOther	SwordTip
	End
End

#############################################################

End