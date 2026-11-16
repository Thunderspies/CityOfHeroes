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
	Time 	0

	Event
		EName 	Prime8
		Type	Start
		At	origin
		Part	Volumes\Water\ShallowSplash\splash_drops.part
		Part	Volumes\Water\ShallowSplash\splash_drops2.part
  		Part    Volumes\Water\ShallowSplash\splash_ripple.part
		Part    Volumes\Water\ShallowSplash\splash_mist.part
		Part    Volumes\Water\ShallowSplash\splash_mist2.part
		Part	Volumes\Water\ShallowSplash\SplashRipple.part
		Part    Volumes\Water\ShallowSplash\Splashbody.part
		Part    Volumes\Water\ShallowSplash\Splashbody2.part
		Part    Volumes\Water\ShallowSplash\SplashFroth.part
  		Part    Volumes\Water\ShallowSplash\splash_ripple.part

		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
	End

	Event
		EName 	Prime3
		Type	Start
		At	origin

		Part    Volumes\Water\ShallowSplash\SplashFroth2.part
		Part    Volumes\Water\ShallowSplash\SplashbodyCore.part
		Part    Volumes\Water\ShallowSplash\SplashbodyCore2.part

	End

	Event
		Power	2 5
		EName 	Prime9
		Type	Start
		At	origin

		Part    Volumes\Water\ShallowSplash\splash_mista.part
		Part    Volumes\Water\ShallowSplash\splash_mist2a.part
		Part	Volumes\Water\ShallowSplash\splash_dropsa.part
		Part	Volumes\Water\ShallowSplash\splash_drops2a.part
		Part    Volumes\Water\ShallowSplash\splash_mista.part

	End

End



End
