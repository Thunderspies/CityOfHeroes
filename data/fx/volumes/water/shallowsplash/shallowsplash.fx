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
		At	Root
		Part	:splash_drops.part
		Part	:splash_drops2.part
  		Part    :splash_ripple.part
		Part    :splash_mist.part
		Part    :splash_mist2.part
		Part	:SplashRipple.part
		Part    :Splashbody.part
		Part    :Splashbody2.part
		Part    :SplashFroth.part

		Sound FF_Water_Enter_01 50 50 .35
		Sound FF_Water_Enter_02 50 50 .35
		Sound FF_Water_Enter_03 50 50 .35
	End

	Event
		EName 	Prime3
		Type	Start
		At	Root

		Part    :SplashFroth2.part
		Part    :SplashbodyCore.part
		Part    :SplashbodyCore2.part

	End

	Event
		Power	2 5
		EName 	Prime9
		Type	Start
		At	Root

		Part    :splash_mista.part
		Part    :splash_mist2a.part
		Part	:splash_dropsa.part
		Part	:splash_drops2a.part

	End

End



End
