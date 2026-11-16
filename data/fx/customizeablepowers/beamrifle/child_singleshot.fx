#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 6

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
		#Part	:MuzzleFlash_Sparks01.part
		Lifespan 6
	End
End

#############################################################

End