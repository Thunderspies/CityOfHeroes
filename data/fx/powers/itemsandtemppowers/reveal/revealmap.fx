#########################################################
##	template

FxInfo
Lifespan 60
#########################################################
#################### GEYSER ########################
#########################################################


Condition
	On	Time
	Time	0

	Event
		Ename 	Circle
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp2.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 100 100 255
			SplatFlags ADDITIVE
		End
		ChildFX	:Rings_Expanding.fx
	End

	Event
		Ename 	Circle
		Type	PositOnly
		At	WepR
		Part	:Rings_Down.part
		Geom	GEO_PoliceScannerHologram_Beam
		bhvr	Behaviors/PoliceScannerHologram_Beam.bhvr
		Lifespan 30
	End

End


Condition
	On	Time
	Time	10

	Event
		Ename 	Grid
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 100 100 255
			SplatFlags ADDITIVE
		End
		Splat	MapGrid.tga
		Lifespan 60
	End
End










