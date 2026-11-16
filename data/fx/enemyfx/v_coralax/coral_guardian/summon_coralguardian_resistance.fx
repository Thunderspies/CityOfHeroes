########################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

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

		Part	Volumes\Water\ShallowSplash\splash_drops.part
		Part	Volumes\Water\ShallowSplash\splash_drops2.part
  		Part    Volumes\Water\ShallowSplash\splash_ripple.part
		Part    Volumes\Water\ShallowSplash\splash_mist.part
		Part    Volumes\Water\ShallowSplash\splash_mist2.part

		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
		lifespan 10
	End

	Event
		EName 	Prime3
		Type	Start
		At	allyourbase

		Part    Volumes\Water\ShallowSplash\SplashFroth2.part
		Part    Volumes\Water\ShallowSplash\SplashbodyCore.part
		Part    Volumes\Water\ShallowSplash\SplashbodyCore2.part

		lifespan 10
	End

	Event
		Power	2 5
		EName 	Prime9
		Type	Start
		At	allyourbase

		Part    Volumes\Water\ShallowSplash\splash_mista.part
		Part    Volumes\Water\ShallowSplash\splash_mist2a.part
		Part	Volumes\Water\ShallowSplash\splash_dropsa.part
		Part	Volumes\Water\ShallowSplash\splash_drops2a.part
		Part    Volumes\Water\ShallowSplash\splash_mista.part

		lifespan 10
	End
End



Condition
	On 	Time
	Time 	15
############################################

	Event
		Type	local
		At	UarmR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LarmR
		Lifespan	40
	End
	Event
		Type	local
		At	LarmR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	HandR
		Lifespan	40
	End
	Event
		Type	local
		At	UlegR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LlegR
		Lifespan	40
	End


	Event
		Type	local
		At	UarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LarmL
		Lifespan	40
	End
	Event
		Type	local
		At	LarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	HandL
		Lifespan	40
	End
	Event
		Type	local
		At	UlegL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LlegL
		Lifespan	40
	End




End
Condition
	On 	Time
	Time 	15
############################################

	Event
		Type	local
		At	UarmR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LarmR
		Lifespan	60
	End
	Event
		Type	local
		At	LarmR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	HandR
		Lifespan	60
	End
	Event
		Type	local
		At	UlegR

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LlegR
		Lifespan	60
	End


	Event
		Type	local
		At	UarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LarmL
		Lifespan	60
	End
	Event
		Type	local
		At	LarmL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	HandL
		Lifespan	60
	End
	Event
		Type	local
		At	UlegL

		Part    V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_WaterDrips.part
		pother	LlegL
		Lifespan	60
	End

End

Condition
	On	DEATH

	Event
		Type	Start
		At	Root
		ChildFX	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Spiritshark_Circle_Hit_mystic.fx
		Lifespan 190
	End



End
