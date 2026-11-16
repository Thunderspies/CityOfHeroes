#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 2

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	WEAPONS\DualPistols\MuzzleFlash_Glow01.part
		Part	WEAPONS\DualPistols\MuzzleFlash_Flare01.part
		Part	WEAPONS\DualPistols\MuzzleFlash_Flare02.part
		Part	WEAPONS\DualPistols\MuzzleFlash_Gas01.part
		Part	WEAPONS\DualPistols\MuzzleFlash_Gas02.part
		Lifespan 2
	End
End

#############################################################

End