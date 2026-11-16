#########################################################
##
########################################################
FxInfo

Input
	Inpname	Mystic
End


Input
	Inpname	Chest
End


####################################

Lifespan 60


########  water burst   ###################################################

Condition
	TriggerBits	INWATER
	Time 	0

	Event
		EName 	Prime8
		Type	Start
		At	Origin
		Part	Volumes\Water\ShallowSplash\splash_drops.part
		Part	Volumes\Water\ShallowSplash\splash_drops2.part
  		Part	Volumes\Water\ShallowSplash\splash_ripple.part
		Part    Volumes\Water\ShallowSplash\splash_mist.part
		Part    Volumes\Water\ShallowSplash\splash_mist2.part
		Part	Volumes\Water\ShallowSplash\SplashRipple.part
		Part    Volumes\Water\ShallowSplash\Splashbody.part
		Part    Volumes\Water\ShallowSplash\Splashbody2.part
		Part    Volumes\Water\ShallowSplash\SplashFroth.part
		Lifespan 10
		Sound FF_Water_Enter_01 50 50 .75
		Sound FF_Water_Enter_02 50 50 .75
		Sound FF_Water_Enter_03 50 50 .75
	End

End



########  default burst   ###################################################

Condition
	On	DEFAULTSURFACE
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local
		At	Mystic
		Part1	POWERS\SuperStrength\superStrengthDust1.part
		part2	POWERS\SuperStrength\superStrengthDust2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
		Sound Groundburst 80 80 .96
		Lifespan 40
	End


End

Condition
	On	DEFAULTSURFACE
	Time 	3

	Event
		EName 	ChestDust
		Type	Local
		At	Chest
		Part1	POWERS\EarthControl\StalagtiteDustHit.part
		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
		Lifespan 50
	End


End



End
