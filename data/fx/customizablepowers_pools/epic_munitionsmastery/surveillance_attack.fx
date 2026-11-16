#                                                          
FxInfo

	Flags InheritAnimScale DontSuppress

	LifeSpan 105


###########################################################

	Condition
		On Time
		Time 0

		Event
			At LArmL
			Type Local
			Sound Training_Surveillance_Activate_01 120 120 0.75
		End

	End

	Condition
		On Time
		Time 0

		Event
			At LArmL
			Type Local
			Bhvr CustomizablePowers_Pools\Epic_MunitionsMastery\PowerAnalyzerOffset_01.bhvr
			BhvrOverride
				TintGeom 1
				#StartColor 255 255 255
			End
			Geom Tintable_GEO_PowerAnalyzer_01
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizablePowers_Pools\Epic_MunitionsMastery\PowerAnalyzerOffset_01.bhvr
			BhvrOverride
				TintGeom 1
				StartColor 128 192 255
			End
			Geom Tintable_GEO_PowerAnalyzer_02
			LifeSpan 100
		End

		Event
			At LArmL
			Type Local
			Bhvr CustomizablePowers_Pools\Epic_MunitionsMastery\PowerAnalyzerOffset_01.bhvr
			BhvrOverride
				TintGeom 1
				StartColor 255 255 255
			End
			Geom Tintable_GEO_PowerAnalyzer_03
			LifeSpan 100
		End

	End

End
