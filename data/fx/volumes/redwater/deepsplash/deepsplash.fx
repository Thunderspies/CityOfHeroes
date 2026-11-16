#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Lifespan 12

Condition
	On 	Time
	Time 	0

	Event
		Power   4 10
		EName 	Prime8
		Type	Start
		At	Origin
		Part   :Redsplash_dropsdeep.part
		Part   :Redsplash_drops2deep.part
		Part   :Redsplash_ripple.part
		Part   :Redsplash_mistdeep.part
		Part   :Redsplash_mist2deep.part
		Part   :Redsplash_mist3deep.part
		Part   :RedSplashRipple.part
		Part   :RedSplash_bodydeep.part
		Part   :RedSplashbody2deep.part
		
	End

End

Condition
	On 	Time
	Time 	5

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	Origin
		Part   :RedSplashbodyDeepCore.part
		Part   :RedSplashbodyDeepCore2.part

	End
End

Condition
	On 	Time
	Time 	9

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	Origin
		Part   :RedSplashbody.part
		Part   :RedSplashbody2.part
		Part   :RedSplashbodyDrops.part

	End
End


End
