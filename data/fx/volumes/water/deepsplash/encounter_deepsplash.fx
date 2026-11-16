#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	Origin
End

Lifespan 12

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Power   4 10
		EName 	Prime8
		Type	Start
		At	Root
		Part   :splash_mistdeep.part
		Part   :splash_mist2deep.part
		Part   :splash_mist3deep.part


	End

End

Condition
	On 	Time
	Time 	7

	Event
		Power   4 10
		EName 	PrimeUs
		Type	Start
		At	Root
		Part   :SplashbodyDeepCore.part
		Part   :SplashbodyDeepCore2.part
	End
End

########################################################


End
