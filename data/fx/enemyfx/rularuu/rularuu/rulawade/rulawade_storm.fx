#                                                          
FxInfo

	Flags InheritAlpha


###########################################################

	Condition
		On Time
		Time 25

		Event
			At Origin
			Type Local
			Sound storm_loop 140 140 0.38
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			#Part :Storm_Center_02.part
		End

		Event
			At Hair
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			#Part :Storm_Center_01.part
			#Part :Storm_Center_02.part
		End

		Event
			At Col_R
			Type Local
			EName Cloud
			Bhvr :Offset_Point_Col_R.bhvr
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At LArmR
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At HandR
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At Col_L
			Type Local
			EName Cloud
			Bhvr :Offset_Point_Col_L.bhvr
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At LArmL
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At HandL
			Type Local
			EName Cloud
			Part :Storm_Heart_01.part
			Part :Storm_Center_01.part
			Part :Storm_Center_02.part
		End

		Event
			At Col_R
			Type Local
			EName Col_R_Offset
			Bhvr :Offset_Point_Col_R.bhvr
		End

		Event
			At Col_L
			Type Local
			EName Col_L_Offset
			Bhvr :Offset_Point_Col_L.bhvr
		End

		Event
			At Head
			Type Local
			EName 1
			Bhvr :Offset_Eye_R.bhvr
			Part :Glow_Eye.part
		End

		Event
			At Head
			Type Local
			EName 1
			Bhvr :Offset_Eye_L.bhvr
			Part :Glow_Eye.part
		End

	End

	Condition
		On Cycle
		Time 60
		Chance 0.5

		Event
			At Head
			Type Local
			EName Lightning
			#POther Hair
			Part :Storm_Lightning_01.part
			Part :Storm_Lightning_02.part
			Part :SSLSCloudLightning01a.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 50
		Chance 0.5

		Event
			At Col_R_Offset
			Type Local
			EName Lightning
			POther Neck
			Part :Storm_Lightning_01.part
			Part :Storm_Lightning_02.part
			Part :SSLSCloudLightning04.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 40
		Chance 0.5

		Event
			At Col_L_Offset
			Type Local
			EName Lightning
			POther Neck
			Part :Storm_Lightning_01.part
			Part :Storm_Lightning_02.part
			Part :SSLSCloudLightning02.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 40
		Chance 0.5

		Event
			At HandR
			Type Local
			EName Lightning
			POther LArmR
			Part :SSLSCloudLightning02.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 65
		Chance 0.5

		Event
			At HandL
			Type Local
			EName Lightning
			POther LArmL
			Part :SSLSCloudLightning03.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 45
		Chance 0.5

		Event
			At LArmR
			Type Local
			EName Lightning
			POther UArmR
			Part :Storm_Lightning_01.part
			Part :Storm_Lightning_02.part
			Part :SSLSCloudLightning01.part
			LifeSpan 15
		End

	End

	Condition
		On Cycle
		Time 55
		Chance 0.5

		Event
			At LArmL
			Type Local
			EName Lightning
			POther UArmL
			Part :Storm_Lightning_01.part
			Part :Storm_Lightning_02.part
			Part :SSLSCloudLightning04.part
			LifeSpan 15
		End

	End

End
