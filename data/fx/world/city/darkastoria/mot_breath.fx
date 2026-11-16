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
		Type	Start
		At	Root
		BhvrOverride
			Alpha		255
			Scale		0.9 1.0 1.025
			StartColor 	230 255 150
			PositionOffset	1.0 1.1 1.1
			PYRRotate	0 0 0
		End
		Geom	FX_GEO_Mot_Breath
	End
End

#############################################################

End