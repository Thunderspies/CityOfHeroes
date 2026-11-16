#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Lifespan 20


Condition
	On	Time
	Time	0

	Event
		EName 	Prime8
		Type	Start
		At	Origin
		Part1 	:Lavasplash_Steam.part
		Part2	:Lavasplash_drops2.part
		Part3   :Lavasplash_Mist.part
		Part4   :Lavasplash_ripple.part
		sound	lavasplashout 40 30 .44
	End

End

End
