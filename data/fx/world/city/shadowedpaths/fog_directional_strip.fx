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
			Alpha		200
			Scale		2.0 1.0 3.0
			PositionOffset	0.0 0.5 0.0
			StartColor 	255 170 045
			PYRRotate	0 270 0
		End
		Geom	FX_Fog_Plane_Narrow
	End

	Event
		Type	Local
		At	FogAnchor
		BhvrOverride
			PositionOffset	-5.0 2.0 0.0
		End
		Part	:Fog_Directional_Strip.part
	End
End

#############################################################

End