#############################################################
## RevealMap_Villain.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	255 16 0
		End
		Geom	GEO_PowerAnalyzer_01
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	:PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	255 64 0
		End
		Geom	GEO_PowerAnalyzer_02
	End

	Event
		Type	Local
		At	LArmL
		Geom	GEO_PowerAnalyzer_04
		Bhvr	V_COV\Powers\ArachnosSoldier\PowerAnalyzerOffset_01.bhvr
		BhvrOverride
			StartColor	100 255 0
		End
		Lifespan 130
	End
End

#############################################################

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 	-0.6 0.5 0.0
			PyrRotate	0 90 70
		End
		Part	:Rings_Small.part
		Part	:BlinkingDot.part
		Part	:BlinkingGlow.part
		Lifespan 75
	End

	Event
		Ename 	Rings
		Type	Local
		At	LArmL
		Part	:Rings_Down.part
		BhvrOverride
			PositionOffset	-0.5 0.05 0.1
			PyrRotate	0 -80 70
		End
		Lifespan 40
	End

	Event
		Ename 	Beam
		Type	Local
		At	LArmL
		Geom	GEO_PoliceScannerHologram_Beam
		bhvr	Behaviors/PoliceScannerHologram_Beam.bhvr
		BhvrOverride
			PositionOffset	-0.4 0.05 0.1
			PyrRotate 	0 -75 -110
			Scale 		2 2 4
		End
		Lifespan 80
	End
End

#############################################################

Condition
	On	Time
	Time	65

	Event
		Ename 	Circle
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0 0.5 0
		End
		Part	:Rings_Expanding.part
		Part	:Glows_Expanding.part
		Lifespan 40
	End
End

Condition
	On	Time
	Time	75

	Event
		Ename 	Grid
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_Grid.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 100 200 255
			SplatFlags ADDITIVE
		End
		Splat	MapGrid.tga
		Lifespan 40
	End
End

#############################################################

End