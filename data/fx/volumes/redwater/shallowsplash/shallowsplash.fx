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
		At	Origin
		Part	:Redsplash_drops.part
		Part	:Redsplash_drops2.part
  		Part    :Redsplash_ripple.part
		Part    :Redsplash_mist.part
		Part    :Redsplash_mist2.part
		Part	:RedSplashRipple.part
		Part    :RedSplashbody.part
		Part    :RedSplashbody2.part
		Part    :RedSplashFroth.part

		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
	End

	Event
		EName 	Prime3
		Type	Start
		At	Origin

		Part    :RedSplashFroth2.part
		Part    :RedSplashbodyCore.part
		Part    :RedSplashbodyCore2.part

	End

	Event
		Power	2 5
		EName 	Prime9
		Type	Start
		At	Origin

		Part    :Redsplash_mista.part
		Part    :Redsplash_mist2a.part
		Part	:Redsplash_dropsa.part
		Part	:Redsplash_drops2a.part

	End

End



End
