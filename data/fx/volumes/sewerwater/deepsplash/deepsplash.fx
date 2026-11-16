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
		Part   :Greensplash_dropsdeep.part
		Part   :Greensplash_drops2deep.part
		Part   :Greensplash_ripple.part
		Part   :Greensplash_mistdeep.part
		Part   :Greensplash_mist2deep.part
		Part   :Greensplash_mist3deep.part
		Part   :GreenSplashRipple.part
		Part   :GreenSplash_bodydeep.part
		Part   :GreenSplashbody2deep.part
		
	End

End

Condition
	On 	Time
	Time 	7

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	Origin
		Part   :GreenSplashbodyDeepCore.part
		Part   :GreenSplashbodyDeepCore2.part
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
		Part   :GreenSplashbody.part
		Part   :GreenSplashbody2.part
		Part   :GreenSplashbodyDrops.part

	End
End


End
