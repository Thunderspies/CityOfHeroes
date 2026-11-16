#                                                          
FxInfo

	LifeSpan 50


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Start
			Sound Training_Surveillance_Hit_01 120 120 0.5
		End

	End

	Condition
		On Time
		Time 0

		Event
			At T_LArmL
			Type Local
			EName WristbandOffset
			Bhvr :PowerAnalyzerOffset_01.bhvr
		End

		Event
			At Chest
			Type PositOnly
			EName TargetChestOffset
			LookAt WristbandOffset
		End

	End

	Condition
		On Time
		Time 0

		Event
			At TargetChestOffset
			Type Local
			EName ScanningLaser
			POther WristbandOffset
			Bhvr V_COV\Powers\ArachnosSoldier\Surveillance\ReticleScale1.bhvr
			Part CustomizablePowers_Pools\Epic_MunitionsMastery\Beam_Core1.part
			Part CustomizablePowers_Pools\Epic_MunitionsMastery\Beam_Glow1.part
		End

	End

	Condition
		On Time
		Time 0

		Event
			At TargetChestOffset
			Type Local
			EName TargetReticle
			Bhvr V_COV\Powers\ArachnosSoldier\Surveillance\ReticleScale1.bhvr
			BhvrOverride
				TintGeom 1
			End
			Part CustomizablePowers_Pools\Epic_MunitionsMastery\Target_Reticle1.part
		End

	End

End
