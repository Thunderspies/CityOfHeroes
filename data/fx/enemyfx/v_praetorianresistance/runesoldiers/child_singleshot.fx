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
		Part	:MuzzleFlash_Glow01.part
		Part	:MuzzleFlash_Flare01.part
		Part	:MuzzleFlash_Gas01.part
		Part	:MuzzleFlash_Gas02.part
		Part	:MuzzleFlash_Sparks01.part
		Lifespan 2
	End
End

#############################################################

End