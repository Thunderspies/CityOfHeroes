#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Origin
End

Lifespan 10

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime8
		Type	Start
		At	Chest
#		Part	Volumes\Water\ShallowSplash\splash_drops.part
		Part    Volumes\Water\ShallowSplash\Splashbody.part
#		Part    Volumes\Water\ShallowSplash\Splashbody2.part
#		Part    Volumes\Water\ShallowSplash\SplashFroth.part

		Sound tongueHit2 60 60 .9
	End

	Event
		EName 	Prime3
		Type	Start
		At	waist
		
		Part    Volumes\Water\ShallowSplash\SplashFroth2.part
#		Part    Volumes\Water\ShallowSplash\SplashbodyCore.part
		Part    Volumes\Water\ShallowSplash\SplashbodyCore2.part

	End


End



End
