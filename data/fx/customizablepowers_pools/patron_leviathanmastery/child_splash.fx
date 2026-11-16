#                                                          
FxInfo

	Input
		InpName Origin
	End

	LifeSpan 10


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			EName Prime8
			Part :splash_drops.part
			Part :splash_drops2.part
			Part :splash_ripple.part
			Part :splash_mist.part
			Part :splash_mist2.part
			Part :SplashRipple.part
			Part :Splashbody.part
			Part :Splashbody2.part
			Part :SplashFroth.part
			Sound FF_Water_Enter_01 50 50 0.75
			Sound FF_Water_Enter_02 50 50 0.75
			Sound FF_Water_Enter_03 50 50 0.75
		End

		Event
			At Origin
			Type Start
			EName Prime3
			Part :SplashFroth2.part
			Part :SplashbodyCore.part
			Part :SplashbodyCore2.part
		End

		Event
			At Origin
			Type Start
			EName Prime9
			Power 2 5
			Part :splash_mista.part
			Part :splash_mist2a.part
			Part :splash_dropsa.part
			Part :splash_drops2a.part
			Part :splash_mista.part
		End

	End

End
