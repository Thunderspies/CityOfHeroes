#########################################################

FxInfo
Flags	InheritAlpha


###############################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Guy
		Type	local
		At	Root
		ChildFX :CHILD_Guardian_BodyFX.fx
	End


	Event
		EName 	allyourbase
		Type	local
		At	Mystic
		Bhvroverride
			scale 1 1.2 1
		end
	end


	Event
		EName 	Prime8
		Type	Start
		At	allyourbase
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
		Type	Start
		At	allyourbase

		Part    :SplashFroth2.part
		Part    :SplashbodyCore.part
		Part    :SplashbodyCore2.part

		lifespan 10
	End
End


Condition
	On	DEATH

	Event
		Type	Local
		At	Root
		Bhvroverride
			scale 1 1.2 1
			PositionOffset 0 0 5
		end
		Childfx :Summon_Guardian_Despawn_Child.fx
		Lifespan 100
	End

End