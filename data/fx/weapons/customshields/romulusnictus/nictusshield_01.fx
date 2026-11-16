#########################################################
## Warshade - Eye Glow Continuing FX
########################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset		0.0 0.0 0.0
		End
		Part	:BlueTendrils.part
		Part	:PurpleTendrils.part
		Part	:BlackTendrils.part
	End
End

#############################################################

End