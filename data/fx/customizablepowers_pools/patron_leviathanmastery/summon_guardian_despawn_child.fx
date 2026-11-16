FXinfo
Lifespan 50

Condition
	On time
	Time 42

	Event
		EName 	Prime8
		Type	StartPositOnly
		At	Origin
		Part	:splash_drops.part
		Part	:splash_drops2.part
  		Part    :splash_ripple.part
		Part    :splash_mist.part
		Part    :splash_mist2.part
		Part	:SplashRipple.part
		Part    :Splashbody.part
		Part    :Splashbody2.part
		Part    :SplashFroth.part
  		Part    :splash_ripple.part
		Part	:splash_drops.part
		Part	:splash_drops2.part
  		Part    :splash_ripple.part
		Part    :splash_mist.part
		Part    :splash_mist2.part

		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
		lifespan 10
	End

	Event
		EName 	Prime3
		Type	StartPositOnly
		At	Prime8

		Part    :SplashFroth2.part
		Part    :SplashbodyCore.part
		Part    :SplashbodyCore2.part

		lifespan 10
	End
End