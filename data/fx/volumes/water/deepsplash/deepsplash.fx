#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	root
End

Lifespan 12

Condition
	On 	Time
	Time 	0

	Event
		Power   4 10
		EName 	Prime8
		Type	Start
		At	root
		
		Part   :splash_dropsdeep.part
		Part   :splash_drops2deep.part
		Part   :splash_ripple.part
		Part   :splash_mistdeep.part
		Part   :splash_mist2deep.part
		Part   :splash_mist3deep.part

		Part   :SplashRipple.part

		Part   :Splash_bodydeep.part
		Part   :Splashbody2deep.part
	End
	Event
		Type Local
		At Root
		Sound FF_Water_Splash_01 50 50 .75
		Sound FF_Water_Splash_01 50 50 .75
		Sound FF_Water_Splash_01 50 50 .75
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	root
		Part   :SplashbodyDeepCore.part
		Part   :SplashbodyDeepCore2.part
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	root
		Part   :Splashbody.part
		Part   :Splashbody2.part
		Part   :SplashbodyDrops.part

	End
End


End
