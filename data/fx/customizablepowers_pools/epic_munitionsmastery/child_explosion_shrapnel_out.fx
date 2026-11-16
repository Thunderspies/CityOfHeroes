#                                                          
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			CThresh 0.000001
			EName Test
			Bhvr :SparkOUT.bhvr
			BhvrOverride
				physDFriction 0.75
				physRestitution 0.5
				physSFriction 0.75
			End
			Part :ShrapnelSpark.part
			Part :HotSpark.part
			Part :HotSparkStreak.part
			LifeSpan 60
			LifeSpanJitter 10
		End

	End

End
