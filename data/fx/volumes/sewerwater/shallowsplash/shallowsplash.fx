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
		Part	:Greensplash_drops.part
		Part	:Greensplash_drops2.part
  		Part    :Greensplash_ripple.part
		Part    :Greensplash_mist.part
		Part    :Greensplash_mist2.part
		Part	:GreenSplashRipple.part
		Part    :GreenSplashbody.part
		Part    :GreenSplashbody2.part
		Part    :GreenSplashFroth.part

		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
	End

	Event
		EName 	Prime3
		Type	Start
		At	Origin

		Part    :GreenSplashFroth2.part
		Part    :GreenSplashbodyCore.part
		Part    :GreenSplashbodyCore2.part

	End

	Event
		Power	2 5
		EName 	Prime9
		Type	Start
		At	Origin

		Part    :Greensplash_mista.part
		Part    :Greensplash_mist2a.part
		Part	:Greensplash_dropsa.part
		Part	:Greensplash_drops2a.part

	End

End



End
