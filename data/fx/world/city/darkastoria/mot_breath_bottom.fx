#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0
Flags DontSuppress

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	time
	Time 	0

	Event
		Ename	FogAnchor
		Type	Local
		At	Root
		BhvrOverride
			Alpha		255
			Scale		2.0 1.0 4.0
			StartColor 	80 100 50
			PositionOffset	0.0 2.0 0.0
		End
		Geom	FX_Fog_Plane_Narrow
	End
End

#############################################################

End